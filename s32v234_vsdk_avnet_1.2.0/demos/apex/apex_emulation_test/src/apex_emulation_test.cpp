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

/*	This application shows how to create processes from ACF_Graphs. 
    It exemplifies it in two different ways: 
        1. In a direct way, where the process is constructed and initialized in the main function directly
          (exemplified by the processes: APU_ROTATE, APU_DOWNSAMPLE_UPSAMPLE, APU_HISTOGRAM.hpp, APU_FAST9.hpp, APU_FAST9_COLOR). 
          This method is well suited for applications with not more than three processes.

        2. By constructing for each process a separate class which takes care of all initializations and process construction mechanisms.
            (exemplified by the processes: GAUSS_3x3, GAUSS_5x5, CONVOLUTION, SAT)
*/

#ifdef APEX2_EMULATE
// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _APEX2_EMULATE_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include <SDKDDKVer.h>

#include <stdio.h>
#include <tchar.h>

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#endif

#ifdef __STANDALONE__
#include "data/common/headers/in_grey_256x256.h"
#include "data/common/headers/in_color_256x256.h"
#include <frame_output_dcu.h>
#endif

#ifndef APEX2_EMULATE
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#ifndef APU_VSIZE
#define APU_VSIZE 32
#endif
#else
#include "apu_lib.hpp"
#include "apu_extras.hpp"
#include "acf_lib.hpp"
using namespace APEX2;
#endif

//to activate time measuring, please define the DEF_TIME_MEASURE in \s32v234_sdk\libs\arm\utils\common\include\config_time_measure.h
#include <common_time_measure.h>
#include <common_helpers.h>
#include <string>

//#ifdef DEF_TIME_MEASURE
//uint64_t total_apex_ticks = 0;
//uint64_t total_load_ticks = 0;
//#endif

#include "chunk_size_definitions.h"

#ifndef APEX2_EMULATE // This is needed only for the Target Compiler
#include <umat.hpp>
#include <apex.h>
#include <oal.h>

// next graphs are included in the direct way
#include "apu_rotate_graph_names.h"
#include <APU_ROTATE.hpp>

#include "apu_downsample_upsample_graph_names.h"
#include <APU_DOWNSAMPLE_UPSAMPLE.hpp>

#include "apu_histogram_graph_names.h"
#include <APU_HISTOGRAM.hpp>

#include "apu_fast9_graph_names.h"
#include <APU_FAST9.hpp>
#include <APU_FAST9_COLOR.hpp>

#else
// next graphs are included in the direct way
#include "apu_rotate_apu_process_desc.hpp"
#include "apu_downsample_upsample_apu_process_desc.hpp"
#include "apu_histogram_apu_process_desc.hpp"
#include "apu_fast9_apu_process_desc.hpp"
#include "apu_fast9_color_apu_process_desc.hpp"
///////////
#endif

//graphs included by declaring a process class around them
#include "apu_gauss_3x3_process.h"
#include "apu_gauss_5x5_process.h"
#include "apu_sat_process.h"
#include "apu_convolution_process.h"

// ////////Includes for convolution
#include "utils_filter.hpp"
using namespace Convolution;
#include "symmetry_flags.h"

using namespace cv;
using namespace std;

#define IMG_WIDTH 256
#define IMG_HEIGHT 256
const int        tileWidth  = IMG_WIDTH / APU_VSIZE;
const int        tileHeight = IMG_HEIGHT / 32;
extern const int SAT_TILE_HEIGHT;

#define INPUT_ROOT "data/common/"
#define OUTPUT_ROOT "data/output/"

int main(int argc, char** argv)
{
  const char  firstImg[]  = INPUT_ROOT "in_grey_256x256.png";
  const char  secondImg[] = INPUT_ROOT "in_color_256x256.png";
  std::string helpMsg     = std::string(
                            "Shows how to construct different APEX graphs and demonstrates also how to emulate them on "
                            "a PC.\n\n\tUsage: ") +
                        COMMON_ExtractProgramName(argv[0]) + "\n\n\tUses input images: \n\t" + firstImg + "\n\t" +
                        secondImg;
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  APEX_Init();

#ifdef APEX2_EMULATE
  REGISTER_PROCESS_TYPE(ROTATE_PI, apu_rotate_apu_process_desc)
  REGISTER_PROCESS_TYPE(DOWNSAMPLE_UPSAMPLE_PI, apu_downsample_upsample_apu_process_desc)
  REGISTER_PROCESS_TYPE(HISTOGRAM_PI, apu_histogram_apu_process_desc)
  REGISTER_PROCESS_TYPE(FAST9_PI, apu_fast9_apu_process_desc)
  REGISTER_PROCESS_TYPE(FAST9_COLOR_PI, apu_fast9_color_apu_process_desc)
#endif

  // Read the input using OpenCV
#ifdef __STANDALONE__
  vsdk::UMat in      = cv::Mat(256, 256, CV_8UC1, in_grey_256x256).getUMat(ACCESS_READ);
  vsdk::UMat inColor = cv::Mat(256, 256, CV_8UC3, in_color_256x256).getUMat(ACCESS_READ);
#else
  vsdk::UMat in      = imread(firstImg, CV_LOAD_IMAGE_GRAYSCALE).getUMat(ACCESS_READ);
  vsdk::UMat inColor = imread(secondImg, CV_LOAD_IMAGE_COLOR).getUMat(ACCESS_READ);
#endif

  if(in.empty() || inColor.empty())
  {
    std::cout << "input images " << firstImg << " or " << secondImg << " not found" << std::endl
              << "Press any key to resume" << std::endl;
    return -1;
  }

#ifdef APEX2_EMULATE
  imshow("input", (cv::UMat)in);
  imshow("color input", (cv::UMat)inColor);
#endif

  // GAUSS 5x5
  {
    printf("Gauss 5x5\n");
    int lRetVal = 0;

    if(!in.empty())
    {
      // Run the APEX process
      CGauss_5x5_Proc process(in);

      // execute
      lRetVal |= process.Exec();

#ifdef APEX2_EMULATE
      imshow("out_gauss5x5.png", (cv::UMat)process.lOutput);
#else
#ifndef __STANDALONE__
      imwrite(OUTPUT_ROOT "out_gauss5x5.png", (cv::UMat)process.lOutput);
#endif
#endif
    }
    else
    {
      lRetVal = 1;
    }
  }

  // GAUSS 3x3
  {
    printf("Gauss 3x3\n");
    int lRetVal = 0;

    if(!in.empty())
    {
      // Run the APEX process
      CGauss_3x3_Proc process;
      // initialize explicitely, outside the constructor
      process.InitData(in);
      process.Connect();

      // execute
      lRetVal |= process.Exec();

#ifdef APEX2_EMULATE
      imshow("out_gauss_3x3", (cv::UMat)process.lOutput);
#else
#ifndef __STANDALONE__
      imwrite(OUTPUT_ROOT "out_gauss_3x3.png", (cv::UMat)process.lOutput);
#endif
#endif
    }
    else
    {
      lRetVal = 1;
    }
  }

  //HISTOGRAM
  {
    printf("histogram: \n");

    const int    HIST_SIZE             = 256;
    unsigned int histogramA[HIST_SIZE] = {};
    {
      vsdk::Mat inMat = in.getMat(ACCESS_READ | OAL_USAGE_CACHED);
      for(int i = 0; i < inMat.rows * inMat.cols; ++i)
      {
        ++histogramA[inMat.data[i]];
      }
    }

    vsdk::UMat dataOut(1, HIST_SIZE, VSDK_CV_32SC1);

    APU_HISTOGRAM process;

    int lRetVal = 0;
    lRetVal |= process.Initialize();
    lRetVal |= process.ConnectIO(HISTOGRAM_GRAPH_IN, in);
    lRetVal |= process.ConnectIO(HISTOGRAM_GRAPH_OUT, dataOut);

#ifdef APEX2_EMULATE
    printf("histogram process graph plan: \n");
    printf("%s\n", process.GetExecutionPlanDescription().c_str());
#endif

    int ApuRuntimeStart = FSL_Ticks();
    // execute
    lRetVal |= process.Start();
    lRetVal |= process.Wait();

    int ApuRuntimeStop = FSL_Ticks();
    printf("\n%.6f sec and %d ticks ApuRUNTIME histogram, \n\n",
           (float)FSL_TicksToSeconds(ApuRuntimeStop - ApuRuntimeStart), ApuRuntimeStop - ApuRuntimeStart);

#ifdef APEX2_EMULATE
    if(lRetVal)
    {
      printf("process error!\n");
      printf("%s\n", process.ErrorMessage().c_str());
    }
#endif

    vsdk::Mat    dataOutMat = dataOut.getMat(ACCESS_READ | OAL_USAGE_CACHED);
    unsigned int histMax = 0, histMaxRef = 0;
    for(int i = 0; i < HIST_SIZE; ++i)
    {
      if(dataOutMat.at< unsigned int >(i) > histMax)
      {
        histMax = dataOutMat.at< unsigned int >(i);
      }

      if(histogramA[i] > histMaxRef)
      {
        histMaxRef = histogramA[i];
      }
    }

    const unsigned int histImageHeight = 128;
    Mat                out(histImageHeight, HIST_SIZE, CV_8UC1);
    Mat                outRef(histImageHeight, HIST_SIZE, CV_8UC1);
    for(unsigned int i = 0; i < HIST_SIZE; ++i)
    {
      float        ratio           = dataOutMat.at< unsigned int >(i) / float(histMax);
      float        ratioRef        = histogramA[i] / float(histMaxRef);
      unsigned int columnHeight    = (unsigned int)(ratio * histImageHeight + .5f);
      unsigned int columnHeightRef = (unsigned int)(ratioRef * histImageHeight + .5f);
      for(unsigned int y = 0; y < histImageHeight; ++y)
      {
        uint8_t value                                 = y < columnHeight ? 255 : 0;
        out.at< uint8_t >(histImageHeight - y - 1, i) = value;

        uint8_t valueRef                                 = y < columnHeightRef ? 255 : 0;
        outRef.at< uint8_t >(histImageHeight - y - 1, i) = valueRef;
      }
    }

#ifdef APEX2_EMULATE
    imshow("histogram", out);
    imshow("histogram REF", outRef);
    //cv::waitKey();
#endif

    bool ok = true;
    for(int i = 0; i < HIST_SIZE; ++i)
    {
      if(histogramA[i] != dataOutMat.at< unsigned int >(i))
      {
        ok = false;
      }
    }
    if(!ok)
    {
      printf("reference and ACF histograms don't match:\n");
      printf("reference:\n");
      for(int i = 0; i < HIST_SIZE; ++i)
      {
        printf("%d ", histogramA[i]);
      }
      printf("\n");
      printf("ACF2:\n");
      for(int i = 0; i < HIST_SIZE; ++i)
      {
        printf("%d ", dataOutMat.at< unsigned int >(i));
      }
      printf("\n");
    }
    else
    {
      printf("reference and ACF histograms match\n\n");
    }
  }

  //#ifdef APEX2_EMULATE
  /* */
  //FAST9
  {
    printf("FAST9: \n\n");
    unsigned char threshold = 50;

    vsdk::UMat out(in.rows, in.cols, VSDK_CV_8UC1);
    vsdk::UMat dataThreshold(1, 1, VSDK_CV_8UC1);

    dataThreshold.getMat(ACCESS_WRITE | OAL_USAGE_CACHED).at< uchar >(0) = threshold;

    FAST9_PI process;

    int lRetVal = 0;
    lRetVal |= process.Initialize();
    lRetVal |= process.ConnectIO(FAST9_GRAPH_IN0, in);
    lRetVal |= process.ConnectIO(FAST9_GRAPH_IN1, dataThreshold);
    lRetVal |= process.ConnectIO(FAST9_GRAPH_OUT, out);

#ifdef APEX2_EMULATE
    printf("FAST9 process graph plan: \n");
    printf("%s\n", process.GetExecutionPlanDescription().c_str());
#endif

    int ApuRuntimeStart = FSL_Ticks();
    // execute
    lRetVal |= process.Start();
    lRetVal |= process.Wait();

    int ApuRuntimeStop = FSL_Ticks();
    printf("\n%.6f sec and %d ticks ApuRUNTIME Fast9, \n\n",
           (float)FSL_TicksToSeconds(ApuRuntimeStop - ApuRuntimeStart), ApuRuntimeStop - ApuRuntimeStart);

#ifdef APEX2_EMULATE
    if(lRetVal)
    {
      printf("process error!\n");
      printf("%s\n", process.ErrorMessage().c_str());
    }
#endif

#ifdef APEX2_EMULATE
    imshow("output_fast9", (cv::UMat)out);
#endif
  }

  //#ifdef APEX2_EMULATE
  // FAST9_COLOR
  {
    printf("FAST9 color: \n\n");
    unsigned char threshold        = 50;
    unsigned char markColorChannel = 1;

    vsdk::UMat out(inColor.rows, inColor.cols, VSDK_CV_8UC3);
    //TODO: use datadescriptor's pixel size, pass to the constructor?

    vsdk::UMat dataThreshold(1, 1, VSDK_CV_8UC1);
    vsdk::UMat dataMarkColorChannel(1, 1, VSDK_CV_8UC1);

    dataThreshold.getMat(ACCESS_WRITE | OAL_USAGE_CACHED).at< uchar >(0)        = threshold;
    dataMarkColorChannel.getMat(ACCESS_WRITE | OAL_USAGE_CACHED).at< uchar >(0) = markColorChannel;

    FAST9_COLOR_PI process;

    int lRetVal = 0;
    lRetVal |= process.Initialize();
    lRetVal |= process.ConnectIO(FAST9_GRAPH_IN0, inColor);
    lRetVal |= process.ConnectIO(FAST9_GRAPH_THR, dataThreshold);
    lRetVal |= process.ConnectIO(FAST9_GRAPH_MARK, dataMarkColorChannel);
    lRetVal |= process.ConnectIO(FAST9_GRAPH_OUT, out);

#ifdef APEX2_EMULATE
    printf("FAST9 color process graph plan: \n");
    printf("%s\n", process.GetExecutionPlanDescription().c_str());
#endif
    int ApuRuntimeStart = FSL_Ticks();
    // execute
    lRetVal |= process.Start();
    lRetVal |= process.Wait();

    int ApuRuntimeStop = FSL_Ticks();
    printf("\n%.6f sec and %d ticks ApuRUNTIME Fast9 color, \n\n",
           (float)FSL_TicksToSeconds(ApuRuntimeStop - ApuRuntimeStart), ApuRuntimeStop - ApuRuntimeStart);

#ifdef APEX2_EMULATE
    if(lRetVal)
    {
      printf("process error!\n");
      printf("%s\n", process.ErrorMessage().c_str());
    }
#endif

#ifdef APEX2_EMULATE
    imshow("output_fast9_color", (cv::UMat)out);
#else
#ifndef __STANDALONE__
    imwrite(OUTPUT_ROOT "out_fast9.png", (cv::UMat)out);
#endif
#endif
  }
  //#endif

  //ROTATE
  { // This is the explicit way to  initialize a graph and run it, as opposed to the Gauss_5x5 example.
    printf("ROTATE: \n\n");
    //unsigned char threshold = 50;

    vsdk::UMat out(in.cols, in.rows, VSDK_CV_8UC1);

    vsdk::UMat dataOffsets(in.rows / ROT_TRANSFER_TILE_SIZE_Y, in.cols / ROT_TRANSFER_TILE_SIZE_X, VSDK_CV_32SC1);

    {
      vsdk::Mat dataOffsetsMat = dataOffsets.getMat(ACCESS_WRITE | OAL_USAGE_CACHED);
      for(int y = 0; y < dataOffsets.rows; ++y)
      {
        for(int x = 0; x < dataOffsets.cols; ++x)
        {
          unsigned int index     = y * dataOffsets.cols + x;
          unsigned int xShuffled = dataOffsets.cols - x - 1;
          unsigned int yShuffled = dataOffsets.rows - y - 1;
          unsigned int offset = yShuffled * in.cols * ROT_TRANSFER_TILE_SIZE_Y + xShuffled * ROT_TRANSFER_TILE_SIZE_X;
          //unsigned int offset = yShuffled * in.cols * tileHeight + xShuffled * tileWidth;
          dataOffsetsMat.at< unsigned int >(index) = offset;
        }
      }
    }

    ROTATE_PI process;

    int lRetVal = 0;
    lRetVal |= process.Initialize();
    //lRetVal |= process.ConnectIO(process.mGraph.m_inputName, dataIn);
    lRetVal |= process.ConnectIndirectInput(ROTATE_GRAPH_IN, in, dataOffsets);
    lRetVal |= process.ConnectIO(ROTATE_GRAPH_OUT, out);

    printf("ROTATE process graph plan: \n");
#ifdef APEX2_EMULATE
    printf("%s\n", process.GetExecutionPlanDescription().c_str());
#endif

    int ApuRuntimeStart = FSL_Ticks();
    // execute
    lRetVal |= process.Start();
    lRetVal |= process.Wait();

    int ApuRuntimeStop = FSL_Ticks();
    printf("\n%.6f sec and %d ticks ApuRUNTIME Rotate, \n\n",
           (float)FSL_TicksToSeconds(ApuRuntimeStop - ApuRuntimeStart), ApuRuntimeStop - ApuRuntimeStart);

#ifdef APEX2_EMULATE
    if(lRetVal)
    {
      printf("process error!\n");
      printf("%s\n", process.ErrorMessage().c_str());
    }
#endif

#ifdef APEX2_EMULATE
    imshow("output_indirect_rotate", (cv::UMat)out);
#else
#ifndef __STANDALONE__
    imwrite(OUTPUT_ROOT "output_indirect_rotate.png", (cv::UMat)out);
#endif
#endif
  }

  //SAT
  {
    printf("SAT (integral image): \n\n");

    CSat_Proc process(in);
    process.Exec();

    double minV = 0, maxV = 0;
    cv::minMaxLoc((cv::UMat)process.lOutput, &minV, &maxV);
    printf("minV: %f maxV: %f\n", minV, maxV);
    cv::Mat out_sat(in.rows, in.cols, CV_8UC1);
    cv::convertScaleAbs((cv::UMat)process.lOutput, out_sat, 1.0 / 20000.0f);

#ifdef APEX2_EMULATE
    imshow("summed area table (integral image)", out_sat);
    imshow("box filter using SAT (II)", (cv::UMat)process.lOutputFilter);
#else
#ifndef __STANDALONE__
    imwrite(OUTPUT_ROOT "integral_image.png", (cv::Mat)out_sat);
    imwrite(OUTPUT_ROOT "box_filter_using_SAT_II.png", (cv::UMat)process.lOutputFilter);
#endif
#endif
  }

  //DOWNSAMPLE_UPSAMPLE
  { // This is the explicit way to  initialize a graph and run it, as opposed to the Gauss_5x5 example.
    printf("DOWNSAMPLE_UPSAMPLE: \n\n");

    vsdk::UMat dataOutDownsample(in.rows / 2, in.cols / 2, VSDK_CV_8UC1);
    vsdk::UMat dataOutUpsample(in.rows * 2, in.cols * 2, VSDK_CV_8UC1);

    DOWNSAMPLE_UPSAMPLE_PI process;
    int                    lRetVal = 0;
    lRetVal |= process.Initialize();
    //lRetVal |= process.ConnectIO(process.mGraph.m_inputName, dataIn);
    lRetVal |= process.ConnectIO(DOWNSAMPLE_UPSAMPLE_IN, in);
    lRetVal |= process.ConnectIO(DOWNSAMPLE_UPSAMPLE_OUT_DOWN, dataOutDownsample);
    lRetVal |= process.ConnectIO(DOWNSAMPLE_UPSAMPLE_OUT_UP, dataOutUpsample);

    printf("DOWNSAMPLE_UPSAMPLE process graph plan: \n");
#ifdef APEX2_EMULATE
    printf("%s\n", process.GetExecutionPlanDescription().c_str());
#endif
    int ApuRuntimeStart = FSL_Ticks();
    // execute
    lRetVal |= process.Start();
    lRetVal |= process.Wait();

    int ApuRuntimeStop = FSL_Ticks();
    printf("\n%.6f sec and %d ticks ApuRUNTIME Downsample_upsample, \n\n",
           (float)FSL_TicksToSeconds(ApuRuntimeStop - ApuRuntimeStart), ApuRuntimeStop - ApuRuntimeStart);

#ifdef APEX2_EMULATE
    if(lRetVal)
    {
      printf("process error!\n");
      printf("%s\n", process.ErrorMessage().c_str());
    }
#endif

#ifdef APEX2_EMULATE
    imshow("downsampled", (cv::UMat)dataOutDownsample);
    imshow("upsampled", (cv::UMat)dataOutUpsample);
#else
#ifndef __STANDALONE__
    imwrite(OUTPUT_ROOT "downsampled.png", (cv::UMat)dataOutDownsample);
    imwrite(OUTPUT_ROOT "upsampled.png", (cv::UMat)dataOutUpsample);
#endif
#endif
  }

  // CONVOLUTION
  {
    printf("CONVOLUTION: \n\n");

    // Pick a filter

    // Others could be (see c_filter.h in the matrix project), or add your own filter to CFilter.cpp
    /*
      const CFilter<3, 3, short> gaussFilter3x3(16, gaussFilter3x3Data);
      const CFilter<5, 5, short> gaussFilter5x5(64, gaussFilter5x5Data);
      const CFilter<3, 1, short> gradientXFilter(1, gradientX);
      const CFilter<1, 3, short> gradientYFilter(1, gradientY);
      const CFilter<3, 3, short> gradientXYFilter(1, gradientXY);
      const CFilter<3, 3, short> scharrXFilter(scharrX);
      const CFilter<3, 3, short> scharrYFilter(scharrY);

      */

#ifndef __INTEGRITY__
//! Needed for FW, FH definitions
#include "correlation_acf.h"
    /* Need to adapt these in correlation_apu.h
        #define FW 3
        #define FH 3
        #define HFW 1 // half filter width, needed for the filter's spatial dependencies
        #define HFH 1 // half filter height, needed for the filter's spatial dependencies
        */
    CConvolution_Proc< FW, FH, short > process(in, &scharrYFilter);
    int                                lRetVal = 0;

    printf("\nBefore execution of convolution\n");
    // execute
    lRetVal |= process.Exec();

    printf("After execution of convolution\n");

    {
      cv::Mat out = process.lOutput.getMat(ACCESS_RW | OAL_USAGE_CACHED);

      double minV = 0, maxV = 0;
      cv::minMaxLoc(out, &minV, &maxV);
      printf("Convolution min: %.2lf max: %.2lf\n", minV, maxV);

      cv::Mat out8uc;
      convertScaleAbs(out, out8uc);

#ifdef APEX2_EMULATE
      imshow("output_convolution", out8uc);
#else
#ifndef __STANDALONE__
      imwrite(OUTPUT_ROOT "output_convolution.png", out8uc);
#endif
#endif
    }
    printf("\nEnd of convolution\n");
#endif
  }

  printf("\nEnd of demo\n");

#ifdef APEX2_EMULATE
  waitKey();
  APEX_Deinit();
#endif

  return 0;
}
