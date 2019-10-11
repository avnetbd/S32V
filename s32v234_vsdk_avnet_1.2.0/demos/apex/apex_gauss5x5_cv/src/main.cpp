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
#include "common_time_measure.h"
#include <umat.hpp>
#include <APU_GAUSS_5X5.hpp>
#include <oal.h>
#include <common_stringify_macros.h>
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

#include <common_helpers.h>
#include <string>

/****************************************************************************
* Main function
* Gauss 5x5 demo showing dual-Apex functionality
****************************************************************************/
int main(int argc, char** argv)
{
  const char  inputImgName[] = INPUT_ROOT "in_grey_256x256.png";
  std::string helpMsg = std::string("Applies an APEX Gauss 5x5 smoothing filter to the input image.\n\tUsage: ") +
                        COMMON_ExtractProgramName(argv[0]) + "\n\n\tUses input image: " + inputImgName;
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  ////////////////////////////////////////////////////////////////////
  // Init part
  int        lRetVal = 0;
  vsdk::UMat out0;
  vsdk::UMat out1;
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

  if(in.empty())
  {
    printf("Cannot find the input image %s\n", inputImgName);
    lRetVal = -1;
    goto END;
  }

  // Create images - one input and two outputs - each for each APEX
  out0 = vsdk::UMat(in.rows, in.cols, VSDK_CV_8UC1);
  out1 = vsdk::UMat(in.rows, in.cols, VSDK_CV_8UC1);

  ////////////////////////////////////////////////////////////////////
  // Processing part
  if(!in.empty() && !out0.empty() && !out1.empty())
  {
    // Create the APEX process - the parameter specifies the APEX instance
    // Until the destruction of the object, the process will be executed on specified APEX instance
    // The APEX instance cannot be changed during the lifecycle of the process
    APU_GAUSS_5X5 process0(APEX_APEX0);
    APU_GAUSS_5X5 process1(APEX_APEX1);

    // Init APEX 0
    lRetVal |= process0.Initialize();
    lRetVal |= process0.ConnectIO("INPUT_0", in);
    lRetVal |= process0.ConnectIO("OUTPUT_0", out0);

    // Init APEX 1
    lRetVal |= process1.Initialize();
    lRetVal |= process1.ConnectIO("INPUT_0", in);
    lRetVal |= process1.ConnectIO("OUTPUT_0", out1);

    int ApuRuntimeStart = FSL_Ticks();
    // execute both
    lRetVal |= process0.Start();
    lRetVal |= process1.Start();

    // wait for both
    lRetVal |= process0.Wait();
    lRetVal |= process1.Wait();

    int ApuRuntimeStop = FSL_Ticks();
    printf("APEX Gauss 5x5 on both APEX devices took %.6f sec and %d ticks APU runtime.\n",
           (float)FSL_TicksToSeconds(ApuRuntimeStop - ApuRuntimeStart), ApuRuntimeStop - ApuRuntimeStart);
  }
  else
  {
    lRetVal = 1;
    goto END;
  }

    ////////////////////////////////////////////////////////////////////
    // Display part

    // for Emulator and INTEGRITY, we use direct OpenCV to file, otherwise...
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
      cv::UMat out0RGB(in.rows, in.cols, CV_8UC3);
      cv::UMat out1RGB(in.rows, in.cols, CV_8UC3);

      cvtColor((cv::UMat)out0, out0RGB, CV_GRAY2RGB);
      cvtColor((cv::UMat)out1, out1RGB, CV_GRAY2RGB);
      cvtColor((cv::UMat)in, inRGB, CV_GRAY2RGB);

      inRGB.copyTo(output_mat(cv::Rect(0, 232, 256, 256)));
      out0RGB.copyTo(output_mat(cv::Rect(300, 232, 256, 256)));
      out1RGB.copyTo(output_mat(cv::Rect(600, 232, 256, 256)));
    }

    output.PutFrame(output_umat);
  }
    // Output for Emu and INTEGRITY
#else
  imwrite(OUTPUT_ROOT "out_gauss5x5_0.png", (cv::UMat)out0);
  imwrite(OUTPUT_ROOT "out_gauss5x5_1.png", (cv::UMat)out1);
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
