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
#include <APU_HISTOGRAM.hpp>
#include "apex.h"
#include <common_time_measure.h>
#include <common_helpers.h>
#include <string>
#include <oal.h>
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

#include "apu_histogram_graph.h"

/****************************************************************************
* Main function
****************************************************************************/
int main(int argc, char** argv)
{
  const char  inputImgName[] = INPUT_ROOT "in_grey_256x256.png";
  std::string helpMsg        = std::string("Computes the histogram of the input image on APEX.\n\tUsage: ") +
                        COMMON_ExtractProgramName(argv[0]) + "\n\n\tUses input image: " + inputImgName;
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  // HW and resources init
  int                lRetVal               = 0;
  const int          HIST_SIZE             = 256;
  unsigned int       histogramA[HIST_SIZE] = {};
  APU_HISTOGRAM      process;
  vsdk::UMat         out;
  int                ApuRuntimeStart;
  int                ApuRuntimeStop;
  unsigned int       histMax         = 0;
  const unsigned int histImageHeight = 128;
  cv::Mat            hist_out;
  bool               ok = true;
  vsdk::Mat          out_mat;

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

  {
    vsdk::Mat in_mat = in.getMat(ACCESS_READ | OAL_USAGE_CACHED);
    for(int i = 0; i < in.rows * in.cols; ++i)
    {
      ++histogramA[in_mat.data[i]];
    }
  }

  out = vsdk::UMat(1, HIST_SIZE, VSDK_CV_32SC1);

  lRetVal = 0;
  lRetVal |= process.Initialize();
  lRetVal |= process.ConnectIO(HISTOGRAM_IN, in);
  lRetVal |= process.ConnectIO(HISTOGRAM_OUT, out);

  ApuRuntimeStart = FSL_Ticks();
  // execute
  lRetVal |= process.Start();
  lRetVal |= process.Wait();

  ApuRuntimeStop = FSL_Ticks();
  printf("Histogram computation took %.6f sec and %d ticks APU runtime.\n",
         (float)FSL_TicksToSeconds(ApuRuntimeStop - ApuRuntimeStart), ApuRuntimeStop - ApuRuntimeStart);

  if(lRetVal)
  {
    printf("Process error!\n");
    goto END;
  }

  hist_out = cv::Mat(histImageHeight, HIST_SIZE, CV_8UC1);
  {
    vsdk::Mat out_mat = out.getMat(ACCESS_READ | OAL_USAGE_CACHED);
    for(int i = 0; i < HIST_SIZE; ++i)
    {
      if(out_mat.at< uint >(i) > histMax)
      {
        histMax = out_mat.at< uint >(i);
      }
    }

    for(unsigned int i = 0; i < HIST_SIZE; ++i)
    {
      float        ratio        = out_mat.at< uint >(i) / float(histMax);
      unsigned int columnHeight = (unsigned int)(ratio * histImageHeight + .5f);
      for(unsigned int y = 0; y < histImageHeight; ++y)
      {
        uint8_t value                                      = y < columnHeight ? 255 : 0;
        hist_out.at< uint8_t >(histImageHeight - y - 1, i) = value;
      }
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

      cv::Mat outRGB;
      cv::Mat inRGB;
      cvtColor(hist_out, outRGB, CV_GRAY2RGB);
      cvtColor((cv::UMat)in, inRGB, CV_GRAY2RGB);
      inRGB.copyTo(output_mat(cv::Rect(0, 232, 256, 256)));
      outRGB.copyTo(output_mat(cv::Rect(300, 296, 256, 128)));
    }

    //warpAffine(output_mat, output_mat, rot_mat, output_mat.size());
    output.PutFrame(output_umat);
  }
#else
  imwrite(OUTPUT_ROOT "histogram.png", (cv::UMat)out);
#endif

  out_mat = out.getMat(ACCESS_READ | OAL_USAGE_CACHED);
  for(int i = 0; i < HIST_SIZE; ++i)
  {
    if(histogramA[i] != out_mat.at< uint >(i))
    {
      ok = false;
      break;
    }
  }
  if(!ok)
  {
    printf("Reference and ACF histograms don't match:\n\n");
    printf("\n");
  }
  else
  {
    printf("Reference and ACF histograms match\n\n");
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
