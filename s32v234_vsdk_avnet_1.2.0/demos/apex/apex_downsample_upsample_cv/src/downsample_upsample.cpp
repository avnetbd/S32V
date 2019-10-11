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
#include "data/common/headers/in_grey_256x256.h"
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

#include "apex.h"

#include <opencv2/opencv.hpp>
#include <APU_DOWNSAMPLE_UPSAMPLE.hpp>
#include <oal.h>
#include <umat.hpp>
#include <common_stringify_macros.h>
#include <common_time_measure.h>
#include <common_helpers.h>
#include <string>

using namespace cv;

#ifdef INPUT_PATH_REL
#define INPUT_ROOT __FULL_DIR__ XSTR(INPUT_PATH_REL) / data / common /
#else
#define INPUT_ROOT "data/common/"
#endif
#ifdef OUTPUT_PATH_REL
#define OUTPUT_ROOT __FULL_DIR__ XSTR(OUTPUT_PATH_REL)
#else
#define OUTPUT_ROOT ""
#endif
/****************************************************************************
* Main function
****************************************************************************/
int main(int argc, char** argv)
{
  const char  inputImgName[] = INPUT_ROOT "in_grey_256x256.png";
  std::string helpMsg        = std::string("Upsamples and downsamples the input image by 2.\n\tUsage: ") +
                        COMMON_ExtractProgramName(argv[0]) + "\n\n\tUses input image: " + inputImgName;
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  int        lRetVal = 0;
  vsdk::UMat outDownsampled;
  vsdk::UMat outUpsampled;
  int        ApuRuntimeStart;
  int        ApuRuntimeStop;

  // Create APEX process and start it
  APU_DOWNSAMPLE_UPSAMPLE process;

// HW and resources init
#ifndef APEX2_EMULATE
  APEX_Init();
#endif

/////////////////////////////////////////////////////////////////////////////////////////
// Read the input using OpenCV (in standalone version, read image array)
#ifdef __STANDALONE__
  vsdk::UMat in = cv::Mat(256, 256, VSDK_CV_8UC1, in_grey_256x256).getUMat(cv::ACCESS_READ);
#else
  vsdk::UMat in = cv::imread(inputImgName, 0).getUMat(cv::ACCESS_READ);
#endif

  // Check the input
  if(in.empty())
  {
    printf("Cannot find the input image %s\n", inputImgName);
    lRetVal = -1;
    goto END;
  }

  // Create output buffers
  outDownsampled = vsdk::UMat(in.rows / 2, in.cols / 2, VSDK_CV_8UC1);
  outUpsampled   = vsdk::UMat(in.rows * 2, in.cols * 2, VSDK_CV_8UC1);

  lRetVal |= process.Initialize();
  lRetVal |= process.ConnectIO("INPUT", in);
  lRetVal |= process.ConnectIO("OUTPUT_DOWN", outDownsampled);
  lRetVal |= process.ConnectIO("OUTPUT_UP", outUpsampled);

  ApuRuntimeStart = FSL_Ticks();

  lRetVal |= process.Start();
  lRetVal |= process.Wait();

  ApuRuntimeStop = FSL_Ticks();

  // Printf the result time
  printf("Downsample upsample took %.6f sec and %d ticks APU runtime.\n",
         (float)FSL_TicksToSeconds(ApuRuntimeStop - ApuRuntimeStart), ApuRuntimeStop - ApuRuntimeStart);

  if(lRetVal)
  {
    printf("Process error!\n");
    goto END;
  }

//////////////////////////////////////////////////
// Process the output
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

      // Init RGB arrays
      cv::UMat inRGB(256, 256, CV_8UC3);
      cv::UMat outDownsampledRGB(in.rows / 2, in.cols / 2, CV_8UC3);
      cv::UMat outUpsampledRGB(in.rows * 2, in.cols * 2, CV_8UC3);

      // Convert the result into RGB arrays
      cvtColor((cv::UMat)outDownsampled, outDownsampledRGB, CV_GRAY2RGB);
      cvtColor((cv::UMat)outUpsampled, outUpsampledRGB, CV_GRAY2RGB);
      cvtColor((cv::UMat)in, inRGB, CV_GRAY2RGB);

      // Copy RGB arrays into output buffer
      inRGB.copyTo(output_mat(cv::Rect(0, 232, 256, 256)));
      outDownsampledRGB.copyTo(output_mat(cv::Rect(300, 296, 128, 128)));
      outUpsampledRGB.copyTo(output_mat(cv::Rect(500, 104, 512, 512)));
    }
    // Put it on screen
    output.PutFrame(output_umat);
  }
#else
  imwrite(OUTPUT_ROOT "downsampled.png", (cv::UMat)outDownsampled);
  imwrite(OUTPUT_ROOT "upsampled.png", (cv::UMat)outUpsampled);
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
