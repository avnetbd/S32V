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

#ifdef APEX2_EMULATE
#include <tchar.h>
#include "apu_lib.hpp"
#include "apu_extras.hpp"
#include "acf_lib.hpp"
using namespace APEX2;

#include "apu_gauss_5x5_apu_process_desc.hpp"
#else
#include <umat.hpp>
#include <APU_GAUSS_5X5.hpp>
#include <apex.h>
#include <oal.h>
#include <stdio.h>
#include <common_stringify_macros.h>
#endif

#include <opencv2/opencv.hpp>
using namespace cv;

#ifdef INPUT_PATH_REL
#define INPUT_ROOT __FULL_DIR__ XSTR(INPUT_PATH_REL) / data / common /
#else
#ifdef APEX2_EMULATE
#define INPUT_ROOT "../../data/common/"
#else
#define INPUT_ROOT "data/common/"
#endif
#endif

#ifdef OUTPUT_PATH_REL
#define OUTPUT_ROOT __FULL_DIR__ XSTR(OUTPUT_PATH_REL) / data / output /
#else
#define OUTPUT_ROOT "data/output/"
#endif

#include <common_time_measure.h>
#include <common_helpers.h>
#include <string>

/****************************************************************************
* Main function
* The function will initialize two parallel processes for APEX0 and APEX1, which will
* execute the Gauss filter on the same image. Both processes apply ROI on images,
* however each process is set by different way. One ROi is set via image settings,
* the second via connect IO function
****************************************************************************/
int main(int argc, char** argv)
{
  const char  inputImgName[] = INPUT_ROOT "in_grey_256x256.png";
  std::string helpMsg        = std::string("Shows how to use the ROI feature with APEX.\n\tUsage: ") +
                        COMMON_ExtractProgramName(argv[0]) + "\n\n\tUses input image: " + inputImgName;
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  int        lRetVal = 0;
  vsdk::UMat out0;
  vsdk::UMat out1;

  APEX_Init();

/////////////////////////////////////////////////////////////////////////////////////////
// Read the input using OpenCV (in standalone version, read image array)
#ifdef __STANDALONE__
  vsdk::UMat in = cv::Mat(256, 256, VSDK_CV_8UC1, in_grey_256x256).getUMat(cv::ACCESS_READ);
#else
  vsdk::UMat in = cv::imread(inputImgName, 0).getUMat(cv::ACCESS_READ);
#endif

#ifdef APEX2_EMULATE
  REGISTER_PROCESS_TYPE(APU_GAUSS_5X5, apu_gauss_5x5_process_desc);
#endif
  if(in.empty())
  {
    std::cerr << "Could not open input image: " << inputImgName << std::endl;
    lRetVal = -1;
    goto END;
  }

  // Create images
  out0 = vsdk::UMat(in.rows, in.cols, VSDK_CV_8UC1);
  out1 = vsdk::UMat(in.rows, in.cols, VSDK_CV_8UC1);

  memset(out0.getMat(ACCESS_WRITE | OAL_USAGE_CACHED).data, 0, in.rows * in.cols);
  memset(out1.getMat(ACCESS_WRITE | OAL_USAGE_CACHED).data, 0, in.rows * in.cols);

  if(!out0.empty() && !out1.empty() && !in.empty())
  {
    ///////////////////////////////////////////////////////////////////////
    // Data init done, now set the APEX processes for full image
    APU_GAUSS_5X5 process1(APEX_APEX0);
    lRetVal |= process1.Initialize();
    lRetVal |= process1.ConnectIO("INPUT_0", in);
    lRetVal |= process1.ConnectIO("OUTPUT_0", out0);

    ///////////////////////////////////////////////////////////////////////
    // Data init done, now set the APEX processes for ROI
    APU_GAUSS_5X5 process2(APEX_APEX1);
    lRetVal |= process2.Initialize();

    for(int i = 0; i < 32; ++i)
    {
      unsigned long time[4];
      time[0] = FSL_Ticks();
      lRetVal |= process1.Start();
      lRetVal |= process1.Wait();
      time[1] = FSL_Ticks();

      memset(out1.getMat(ACCESS_WRITE | OAL_USAGE_CACHED).data, 0, out1.rows * out1.cols);
      //                                                   X,        Y,      W,      H
      lRetVal |= process2.ConnectIO_ROI("INPUT_0", in, 88 + i, 128 + i, 128, 88);
      lRetVal |= process2.ConnectIO_ROI("OUTPUT_0", out1, 88 + i, 128 + i, 128, 88);

      time[2] = FSL_Ticks();
      lRetVal |= process2.Start();
      lRetVal |= process2.Wait();
      time[3] = FSL_Ticks();

      //printf("Image size ratio: %f %% [%dx%d/%dx%d]\n", (((88.0+i)*(128.0+i)) / (256.0*256.0)) * 100, 88+i, 128+i, 256, 256);
      //printf("Image X ratio:    %f %% [%d/%d]\n", ((128.0+i) / (256.0)) * 100, 128+i, 256);
      //printf("Image Y ratio:    %f %% [%d/%d]\n", ((88.0+i) / (256.0)) * 100, 88+i, 256);
      printf("Time ratio:       %f %%\n", (float)((time[3] - time[2]) / (float)(time[1] - time[0])) * 100);
      printf("Time full image:   %0.6f s\n", (float)FSL_TicksToSeconds(time[1] - time[0]));
      printf("Time ROI  image:   %0.6f s\n", (float)FSL_TicksToSeconds(time[3] - time[2]));
      printf("=============================\n");

#ifdef _DEBUG
      {
        cv::imshow("out1", (cv::UMat)out0);
        cv::imshow("out2", (cv::UMat)out1);
        waitKey();
      }
#endif
    }
  }
  else
  {
    lRetVal = 1;
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
      cv::UMat out0RGB(in.rows, in.cols, CV_8UC3);
      cv::UMat out1RGB(in.rows, in.cols, CV_8UC3);

      cvtColor((cv::UMat)out0, out0RGB, CV_GRAY2RGB);
      cvtColor((cv::UMat)out1, out1RGB, CV_GRAY2RGB);
      cvtColor((cv::UMat)in, inRGB, CV_GRAY2RGB);

      cv::rectangle(out0RGB, cv::Point(0, 0), cv::Point(out0.cols - 1, out0.rows - 1), cv::Scalar(255, 255, 255));
      cv::rectangle(out1RGB, cv::Point(0, 0), cv::Point(out1.cols - 1, out1.rows - 1), cv::Scalar(255, 255, 255));
      inRGB.copyTo(output_mat(cv::Rect(0, 232, 256, 256)));
      out0RGB.copyTo(output_mat(cv::Rect(300, 232, 256, 256)));
      out1RGB.copyTo(output_mat(cv::Rect(600, 232, 256, 256)));
    }

    output.PutFrame(output_umat);
  }
#else
  imwrite(OUTPUT_ROOT "out_gauss5x5_roi_1.png", (cv::UMat)out0);
  imwrite(OUTPUT_ROOT "out_gauss5x5_roi_2.png", (cv::UMat)out1);
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
