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

#if !defined(APEX2_EMULATE) && !defined(__INTEGRITY__)
#ifdef __STANDALONE__
#include "frame_output_dcu.h"
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"
#endif // else from #ifdef __STANDALONE__

#define CHNL_CNT io::IO_DATA_CH3
#endif

#ifdef __STANDALONE__
#include "../../../data/common/headers/in_grey_256x256.h"
#include <printf.h>
#endif

#include <opencv2/opencv.hpp>
#include <umat.hpp>
#include <APU_SAT.hpp>
#include <apex.h>
#include <common_time_measure.h>
#include <common_helpers.h>
#include <string>
#include <oal.h>
#include <common_stringify_macros.h>
#include <stdint.h>
using namespace cv;

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
  std::string helpMsg        = std::string("Computes the integral image of the input image on APEX.\n\tUsage: ") +
                        COMMON_ExtractProgramName(argv[0]) + "\n\n\tUses input image: " + inputImgName;
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  // HW and resources init
  int        lRetVal = 0;
  APU_SAT    process(APEX_APEX1);
  int        ApuRuntimeStart;
  int        ApuRuntimeStop;
  vsdk::UMat out_sat;
  vsdk::UMat out_filter;
  vsdk::UMat dataOutSAT;

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

  lRetVal = 0;

  out_sat    = vsdk::UMat(in.rows, in.cols, VSDK_CV_8UC1);
  out_filter = vsdk::UMat(in.rows, in.cols, VSDK_CV_8UC1);
  dataOutSAT = vsdk::UMat(in.rows, in.cols, VSDK_CV_32SC1);

  lRetVal |= process.Initialize();
  lRetVal |= process.ConnectIO("INPUT_0", in);
  lRetVal |= process.ConnectIO("OUTPUT_0", dataOutSAT);
  lRetVal |= process.ConnectIO("OUTPUT_1", out_filter);

  ApuRuntimeStart = FSL_Ticks();
  // execute
  lRetVal |= process.Start();
  lRetVal |= process.Wait();

  ApuRuntimeStop = FSL_Ticks();
  printf("Summed area table computation + filtering took %.6f sec and %d ticks APU runtime.\n",
         (float)FSL_TicksToSeconds(ApuRuntimeStop - ApuRuntimeStart), ApuRuntimeStop - ApuRuntimeStart);

  if(lRetVal)
  {
    printf("Process error!\n");
    goto END;
  }

  {
    vsdk::Mat dataOutSAT_mat = dataOutSAT.getMat(ACCESS_READ | OAL_USAGE_CACHED);
    vsdk::Mat out_sat_mat    = out_sat.getMat(ACCESS_READ | OAL_USAGE_CACHED);
    for(int i = 0; i < in.cols * in.rows; ++i)
    {
      uint32_t iiv = dataOutSAT_mat.at< uint32_t >(i) / 20000;
      if(iiv > 255)
      {
        iiv = 255;
      }
      out_sat_mat.at< uint8_t >(i) = uint8_t(iiv);
    }
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
      cv::UMat out_satRGB(out_sat.rows, out_sat.cols, CV_8UC3);
      cv::UMat out_filterRGB(out_filter.rows, out_filter.cols, CV_8UC3);

      cvtColor((cv::UMat)out_filter, out_filterRGB, CV_GRAY2RGB);
      cvtColor((cv::UMat)out_sat, out_satRGB, CV_GRAY2RGB);
      cvtColor((cv::UMat)in, inRGB, CV_GRAY2RGB);

      inRGB.copyTo(output_mat(cv::Rect(0, 232, 256, 256)));
      out_satRGB.copyTo(output_mat(cv::Rect(300, 232, 256, 256)));
      out_filterRGB.copyTo(output_mat(cv::Rect(600, 232, 256, 256)));
    }

    output.PutFrame(output_umat);
  }
#else
  imwrite(OUTPUT_ROOT "sat.png", (cv::UMat)out_sat);
  imwrite(OUTPUT_ROOT "sat_box_filter.png", (cv::UMat)out_filter);
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
