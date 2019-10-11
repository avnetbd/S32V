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

#ifndef APEX2_EMULATE // This is needed only for the Target Compiler

#include <stdio.h>
#ifdef __STANDALONE__
#include "../../../data/common/headers/in_color_256x256.h"
#endif

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>

#include <umat.hpp>
#include <apex.h>
#include <oal.h>
#include <APU_FAST9_COLOR.hpp>
#else
#include "apu_lib.hpp"
#include "apu_extras.hpp"
#include "acf_lib.hpp"
using namespace APEX2;

#include "DefineAllProcessDesc.h"
#include "apu_fast9_color_apu_process_desc.hpp"

#define IMG_WIDTH 256
#define IMG_HEIGHT 256
const int tileWidth       = IMG_WIDTH / VSIZE;
const int tileHeight      = IMG_HEIGHT / VSIZE;
const int SAT_TILE_HEIGHT = 8;
#endif

using namespace cv;
using namespace std;

#include "GraphNames.h"

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

#ifdef APEX2_EMULATE
#include "apu_dilate_diamond.h"
#include "apu_mark.h"
#include "apu_mark_color_channel.h"
#include "apu_16low_to_8.h"
#include "apu_nms.h"
#include "apu_fast9.h"
#include "apu_rgb_to_grayscale.h"
#include "apu_gauss_3x3.h"
#endif

#include <common_helpers.h>
#include <string>

/****************************************************************************
* Main function
****************************************************************************/
int main(int argc, char** argv)
{
  const char  inputImgName[] = INPUT_ROOT "in_color_256x256.png";
  std::string helpMsg        = std::string("Finds Fast9 corners with APEX on the input image.\n\tUsage: ") +
                        COMMON_ExtractProgramName(argv[0]) + "\n\n\tUses input image: " + inputImgName;
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  int            lRetVal          = 0;
  uint8_t        threshold        = 50;
  uint8_t        markColorChannel = 1;
  vsdk::UMat     out;
  vsdk::UMat     dataThreshold;
  vsdk::UMat     dataMarkColorChannel;
  FAST9_COLOR_PI process;
  // This is needed only for the Target Compiler
  // HW and resources init
#ifndef APEX2_EMULATE
  APEX_Init();
#endif

#ifdef APEX2_EMULATE
  REGISTER_PROCESS_TYPE(FAST9_COLOR_PI, apu_fast9_color_apu_process_desc)
  XREGISTER_ACF_KERNEL(DILATE_DIAMOND_K)
  XREGISTER_ACF_KERNEL(MARK_K)
  XREGISTER_ACF_KERNEL(NMS_K)
  XREGISTER_ACF_KERNEL(_16_LOW_TO_8_K)
  XREGISTER_ACF_KERNEL(FAST9_K)
  XREGISTER_ACF_KERNEL(SAT_NONZERO_K)
  XREGISTER_ACF_KERNEL(MARK_COL_K)
  XREGISTER_ACF_KERNEL(RGB_TO_GRAY_K)
  XREGISTER_ACF_KERNEL(GAUSS_3x3_K)
#endif

/////////////////////////////////////////////////////////////////////////////////////////
// Read the input using OpenCV (in standalone version, read image array)
#ifdef __STANDALONE__
  vsdk::UMat in = cv::Mat(256, 256, VSDK_CV_8UC3, in_color_256x256).getUMat(cv::ACCESS_READ);
#else
  vsdk::UMat in = cv::imread(inputImgName, CV_LOAD_IMAGE_COLOR).getUMat(cv::ACCESS_READ);
#endif

  if(in.empty())
  {
    printf("Cannot find the input image (in_color_256x256.png\n");
    lRetVal = -1;
    goto END;
  }

#ifdef APEX2_EMULATE
  imshow("Input Image", (cv::UMat)in);
  waitKey();
#endif

  out                  = vsdk::UMat(in.rows, in.cols, VSDK_CV_8UC3);
  dataThreshold        = vsdk::UMat(1, 1, VSDK_CV_8UC1);
  dataMarkColorChannel = vsdk::UMat(1, 1, VSDK_CV_8UC1);

  dataThreshold.getMat(ACCESS_WRITE | OAL_USAGE_CACHED).at< uint8_t >(0)        = threshold;
  dataMarkColorChannel.getMat(ACCESS_WRITE | OAL_USAGE_CACHED).at< uint8_t >(0) = markColorChannel;

  lRetVal |= process.Initialize();
  lRetVal |= process.ConnectIO("INPUT", in);
  lRetVal |= process.ConnectIO("THRESHOLD", dataThreshold);
  lRetVal |= process.ConnectIO("MARK_COLOR_CHANNEL", dataMarkColorChannel);
  lRetVal |= process.ConnectIO("OUTPUT", out);

  // execute
  lRetVal |= process.Start();
  lRetVal |= process.Wait();

#ifdef APEX2_EMULATE
  cv::imshow("Fast9", (cv::UMat)out);
  cv::waitKey();
#endif

  //////////////////////////////////////////////////
  // Process the output - if standalone, compare with reference
  //                    - if OS, save the images
  {
#if !defined(APEX2_EMULATE) && !defined(__INTEGRITY__)

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
      cv::Mat out_mat    = out.getMat(ACCESS_WRITE | OAL_USAGE_CACHED);
      memset(output_mat.data, 0, 720 * 1280 * 3);

      out_mat.copyTo(output_mat(cv::Rect(0, 232, 256, 256)));
    }
    output.PutFrame(output_umat);
#else
    imwrite(OUTPUT_ROOT "out_fast9_color.png", (cv::UMat)out);
#endif
  }

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
