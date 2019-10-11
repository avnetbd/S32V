/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

/****************************************************************************
* Includes
****************************************************************************/

#ifdef __STANDALONE__
#include "../../../data/common/headers/in_grey_256x256.h"
#include <printf.h>
#endif

#if !defined(APEX2_EMULATE) && !defined(__INTEGRITY__)
#ifdef __STANDALONE__
#include "frame_output_dcu.h"
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"
#endif // else from #ifdef __STANDALONE__

#define CHNL_CNT io::IO_DATA_CH3
#endif

#include <opencv2/opencv.hpp>
#include <umat.hpp>
#include <APU_ROTATE.hpp>
#include <apex.h>
#include <common_time_measure.h>
#include <common_helpers.h>
#include <string>
#include <common_stringify_macros.h>
#include <oal.h>
#include <stdint.h>
using namespace cv;

const int TRANSFER_TILE_SIZE_X = 8;
const int TRANSFER_TILE_SIZE_Y = 8;

#ifdef INPUT_PATH_REL
#define INPUT_ROOT __FULL_DIR__ XSTR(INPUT_PATH_REL) / data / common /
#else
#define INPUT_ROOT "data/common/"
#endif
#ifdef OUTPUT_PATH_REL
#define OUTPUT_ROOT __FULL_DIR__ XSTR(OUTPUT_PATH_REL) / data / output /
#else
#define OUTPUT_ROOT "data/output/"
#endif

/****************************************************************************
* Main function
****************************************************************************/
int main(int argc, char** argv)
{
  const char  inputImgName[] = INPUT_ROOT "in_grey_256x256.png";
  std::string helpMsg =
      std::string("Shows how to use indirect inputs with APEX, by rotating the input image.\n\tUsage: ") +
      COMMON_ExtractProgramName(argv[0]) + "\n\n\tUses input image: " + inputImgName;
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  // HW and resources init
  int        lRetVal = 0;
  vsdk::UMat out;
  vsdk::UMat dataOffsets;
  APU_ROTATE process;
  int        ApuRuntimeStart;
  int        ApuRuntimeStop;

  APEX_Init();

/////////////////////////////////////////////////////////////////////////////////////////
// Read the input using OpenCV (in standalone version, read image array)
#ifdef __STANDALONE__
  vsdk::UMat in = cv::Mat(256, 256, VSDK_CV_8UC1, in_grey_256x256).getUMat(cv::ACCESS_READ);
#else
  vsdk::UMat in = cv::imread(inputImgName, 0).getUMat(cv::ACCESS_READ);
#endif

  if(in.empty())
  {
    printf("Cannot find the input image %s\n", inputImgName);
    lRetVal = -1;
    goto END;
  }

  out         = vsdk::UMat(in.rows, in.cols, VSDK_CV_8UC1);
  dataOffsets = vsdk::UMat(in.rows / TRANSFER_TILE_SIZE_Y, in.cols / TRANSFER_TILE_SIZE_X, VSDK_CV_32SC1);

  {
    vsdk::Mat dataOffsetsMat = dataOffsets.getMat(ACCESS_WRITE | OAL_USAGE_CACHED);
    for(int y = 0; y < dataOffsets.rows; ++y)
    {
      for(int x = 0; x < dataOffsets.cols; ++x)
      {
        unsigned int index     = y * dataOffsets.cols + x;
        unsigned int xShuffled = dataOffsets.cols - x - 1;
        unsigned int yShuffled = dataOffsets.rows - y - 1;
        unsigned int offset    = yShuffled * in.cols * TRANSFER_TILE_SIZE_Y + xShuffled * TRANSFER_TILE_SIZE_X;
        dataOffsetsMat.at< uint >(index) = offset;
      }
    }
  }

  lRetVal = 0;
  lRetVal |= process.Initialize();
  lRetVal |= process.ConnectIndirectInput("INPUT_0", in, dataOffsets);
  lRetVal |= process.ConnectIO("OUTPUT_0", out);

  ApuRuntimeStart = FSL_Ticks();
  // execute
  lRetVal |= process.Start();
  lRetVal |= process.Wait();

  ApuRuntimeStop = FSL_Ticks();
  printf("Rotation of the image took %.6f sec and %d ticks APU runtime.\n",
         (float)FSL_TicksToSeconds(ApuRuntimeStop - ApuRuntimeStart), ApuRuntimeStop - ApuRuntimeStart);

  if(lRetVal)
  {
    printf("Process error!\n");
    goto END;
  }

//////////////////////////////////////////////////
// Process the output - if standalone, compare with reference
//                    - if OS, save the images
#if !defined(APEX2_EMULATE) && !defined(__INTEGRITY__)
  {
// Initialize different output class for Standalone and Linux
#ifdef __STANDALONE__
    io::FrameOutputDCU output(1280, 720, io::IO_DATA_DEPTH_08, CHNL_CNT);
#else
    io::FrameOutputV234Fb output(1280, 720, io::IO_DATA_DEPTH_08, CHNL_CNT);
#endif

    // Output buffer (screen size) and it's mapped version (using cv mat in order to have copyTo functions)
    vsdk::UMat output_umat = vsdk::UMat(720, 1280, VSDK_CV_8UC3);
    {
      cv::Mat output_mat = output_umat.getMat(ACCESS_WRITE | OAL_USAGE_CACHED);
      memset(output_mat.data, 0, 720 * 1280 * 3);

      cv::UMat inRGB(in.rows, in.cols, CV_8UC3);
      cv::UMat outRGB(in.rows, in.cols, CV_8UC3);

      cvtColor((cv::UMat)out, outRGB, CV_GRAY2RGB);
      cvtColor((cv::UMat)in, inRGB, CV_GRAY2RGB);

      inRGB.copyTo(output_mat(cv::Rect(0, 232, 256, 256)));
      outRGB.copyTo(output_mat(cv::Rect(300, 232, 256, 256)));
    }

    output.PutFrame(output_umat);
  }
#else
  imwrite(OUTPUT_ROOT "rotated.png", (cv::UMat)out);
#endif

END:
  if(0 != lRetVal)
  {
    printf("Program Ended Error 0x%X [ERROR]\n", lRetVal);
  }
  else
  {
    printf("Program Ended [SUCCESS]\n");
  }

  return lRetVal;
}
