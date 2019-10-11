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

#include "tinyxml2.h"

#ifdef APEX2_EMULATE
#define USE_OPENCV_TIMER
#endif
#ifdef __INTEGRITY
#define USE_OPENCV_TIMER
#endif

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <sstream>
#include <stdint.h>

using namespace std;
using namespace cv;
using namespace tinyxml2;

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

#define USE_FIXED_POINT 1

typedef int32_t STAGE_FIXED_POINT_TYPE;
const int       STAGE_FRACTIONAL_BITS        = 28;
const int       STAGE_FIXED_POINT_MULTIPLIER = (1 << STAGE_FRACTIONAL_BITS);

#ifdef __STANDALONE__
#include "../../../data/common/headers/input_faces.h"
#include "../../../data/common/headers/lbpcascade_frontalface.h"
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
#include <acf_lib.hpp>
using namespace APEX2;

#include "apu_face_detection_lbp_apu_process_desc.hpp"
REGISTER_PROCESS_TYPE(FACE_DETECTION_PI, apu_face_detection_lbp_apu_process_desc)

#else
#include <icp_data.h>
#include <APU_FACE_DETECTION_LBP.hpp>
#include "apex.h"
#include <oal.h>
#include <umat.hpp>
#endif

#include "chunk_config.hpp"

#include <time.h>
#include <common_time_measure.h>
#include <common_helpers.h>
#include <string>

struct lbpFeature
{
  int rectangleIndex;
  int values[8];
#if USE_FIXED_POINT
  STAGE_FIXED_POINT_TYPE leafValuesFixed[2];
#else
  double     leafValues[2];
#endif
};

struct lbpRectangle
{
  int x, y, width, height;
};

struct lbpStage
{
  int firstFeatureIndex;
  int featureCount;
#if USE_FIXED_POINT
  STAGE_FIXED_POINT_TYPE thresholdFixed;
#else
  double     threshold;
#endif
};

struct APEX_lbpFeature
{
  int32_t                values[8];
  STAGE_FIXED_POINT_TYPE leafValues[2];
  uint8_t                x, y, width, height;
};

struct APEX_lbpStage
{
  uint8_t                featureCount;
  STAGE_FIXED_POINT_TYPE threshold;
};

#include "apu_face_detection_lbp_graph_names.h"

int EndFct(int aRetVal, APEX_lbpFeature* apApexLBPFeatures, APEX_lbpStage* apApexLBPStages)
{
  if(0 != aRetVal)
  {
    printf("Program Ended Error 0x%X [ERROR]\n", aRetVal);
  }
  else
  {
    printf("Program Ended [SUCCESS]\n");
  }

  if(apApexLBPFeatures != NULL)
  {
    delete[] apApexLBPFeatures;
  }

  if(apApexLBPStages != NULL)
  {
    delete[] apApexLBPStages;
  }
  return aRetVal;
}

int main(int argc, char** argv)
{
  const char  inputImgName[]   = INPUT_ROOT "input_faces.png";
  const char  modelFileName1[] = INPUT_ROOT "lbpcascade_frontalface.xml";
  const char  modelFileName2[] = INPUT_ROOT "lbpcascade_frontalface_cv3.1.0.xml";
  std::string helpMsg          = std::string("Finds corners in the input image.\n\tUsage: ") +
                        COMMON_ExtractProgramName(argv[0]) + "\n\n\tUses input image: " + inputImgName +
                        "\n\tand model files: " + modelFileName1 + " and " + modelFileName2;
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return 0;
  }

  int                    lRetVal = 0;
  XMLDocument            cascades;
  XMLNode*               node;
  int                    windowWidth, windowHeight;
  vector< lbpFeature >   lbpFeatures;
  vector< lbpRectangle > lbpRectangles;
  vector< lbpStage >     lbpStages;
  XMLNode*               stageNode       = NULL;
  XMLNode*               rectangleNode   = NULL;
  APEX_lbpFeature*       apexLBPFeatures = NULL;
  APEX_lbpStage*         apexLBPStages   = NULL;
  int                    apexLBPFeaturesSize;
  int                    apexLBPStagesSize;
  FACE_DETECTION_PI      process;

  // HW and resources init
  APEX_Init();

/////////////////////////////////////////////////////////////////////////////////////////
// Read the input using OpenCV (in standalone version, read image array)
#ifdef __STANDALONE__
  vsdk::UMat input = cv::Mat(320, 384, VSDK_CV_8UC1, input_faces).getUMat(cv::ACCESS_READ);
#else
  vsdk::UMat input = cv::imread(inputImgName, 0).getUMat(cv::ACCESS_READ);
#endif

  if(input.empty())
  {
    cout << "Input image not found: " << inputImgName << endl;
    system("pwd");
    lRetVal = -1;
    return EndFct(lRetVal, apexLBPFeatures, apexLBPStages);
  }

#ifdef __STANDALONE__
  if(cascades.LoadString(lbpcascade_frontalface) != XML_SUCCESS)
  {
    cout << "Can't open the cascades file" << endl;
    lRetVal = -1;
    return EndFct(lRetVal, apexLBPFeatures, apexLBPStages);
  }
#else
  if(cascades.LoadFile(modelFileName1) != XML_SUCCESS)
  {
    cout << "Can't open the cascades file: " << modelFileName1 << endl;
    lRetVal = -1;
    return EndFct(lRetVal, apexLBPFeatures, apexLBPStages);
  }
#endif

  //start at the size node
  node = cascades.FirstChild()->NextSibling()->NextSibling()->FirstChild()->FirstChild()->NextSibling()->NextSibling();

  {
    stringstream ss(node->ToElement()->GetText());
    ss >> windowHeight;
  }

  node = node->NextSibling();

  {
    stringstream ss(node->ToElement()->GetText());
    ss >> windowWidth;
  }

  //TODO: stage params?

  //stages
  node = node->NextSibling()->NextSibling()->NextSibling()->NextSibling();

  //int windowViewScale = 20;

  stageNode = node->FirstChild();

  while(stageNode)
  {
    stageNode = stageNode->NextSibling();
    lbpStage lbps;
    lbps.firstFeatureIndex = (int)lbpFeatures.size();

    XMLNode*     stageSumNode = stageNode->FirstChild()->NextSibling();
    stringstream sss(stageSumNode->ToElement()->GetText());
#if USE_FIXED_POINT
    double dThr;
    sss >> dThr;
    lbps.thresholdFixed = STAGE_FIXED_POINT_TYPE(dThr * STAGE_FIXED_POINT_MULTIPLIER);
#else
    sss >> lbps.threshold;
#endif

    XMLNode* featureNode = stageSumNode->NextSibling()->FirstChild();
    while(featureNode)
    {
      featureNode = featureNode->NextSibling();
      lbpFeature lbpf;

      XMLNode*     innerNode = featureNode->FirstChild();
      stringstream fss0(innerNode->ToElement()->GetText());
      string       temp;
      fss0 >> temp >> temp >> lbpf.rectangleIndex >> lbpf.values[0] >> lbpf.values[1] >> lbpf.values[2] >>
          lbpf.values[3] >> lbpf.values[4] >> lbpf.values[5] >> lbpf.values[6] >> lbpf.values[7];

      innerNode = innerNode->NextSibling();

      stringstream fss1(innerNode->ToElement()->GetText());
#if USE_FIXED_POINT
      double dLV0, dLV1;
      fss1 >> dLV0 >> dLV1;
      lbpf.leafValuesFixed[0] = STAGE_FIXED_POINT_TYPE(dLV0 * STAGE_FIXED_POINT_MULTIPLIER);
      lbpf.leafValuesFixed[1] = STAGE_FIXED_POINT_TYPE(dLV1 * STAGE_FIXED_POINT_MULTIPLIER);
#else
      fss1 >> lbpf.leafValues[0] >> lbpf.leafValues[1];
#endif

      lbpFeatures.push_back(lbpf);

      featureNode = featureNode->NextSibling();
    }

    lbps.featureCount = (int)lbpFeatures.size() - lbps.firstFeatureIndex;
    lbpStages.push_back(lbps);

    stageNode = stageNode->NextSibling();
  }

  node          = node->NextSibling();
  rectangleNode = node->FirstChild();
  while(rectangleNode)
  {
    lbpRectangle lbpr;
    stringstream rss(rectangleNode->FirstChild()->ToElement()->GetText());
    rss >> lbpr.x >> lbpr.y >> lbpr.width >> lbpr.height;

    lbpRectangles.push_back(lbpr);

    rectangleNode = rectangleNode->NextSibling();
  }

  //apex structs
  apexLBPFeatures     = new APEX_lbpFeature[lbpFeatures.size()];
  apexLBPStages       = new APEX_lbpStage[lbpStages.size()];
  apexLBPFeaturesSize = lbpFeatures.size() * sizeof(APEX_lbpFeature);
  apexLBPStagesSize   = lbpStages.size() * sizeof(APEX_lbpStage);

  for(unsigned int i = 0; i < lbpFeatures.size(); ++i)
  {
    lbpFeature   lbpf = lbpFeatures[i];
    lbpRectangle lbpr = lbpRectangles[lbpf.rectangleIndex];
    for(int j = 0; j < 8; ++j)
    {
      apexLBPFeatures[i].values[j] = lbpf.values[j];
    }
    apexLBPFeatures[i].leafValues[0] = lbpf.leafValuesFixed[0];
    apexLBPFeatures[i].leafValues[1] = lbpf.leafValuesFixed[1];
    apexLBPFeatures[i].x             = lbpr.x;
    apexLBPFeatures[i].y             = lbpr.y;
    apexLBPFeatures[i].width         = lbpr.width;
    apexLBPFeatures[i].height        = lbpr.height;
  }

  for(unsigned int i = 0; i < lbpStages.size(); ++i)
  {
    apexLBPStages[i].threshold    = lbpStages[i].thresholdFixed;
    apexLBPStages[i].featureCount = lbpStages[i].featureCount;
  }

  ////////////////////////////////cascades ready
  vector< cv::Rect > faces; // OpenCV Bug crashes on destructor of this vector
  {
#ifndef __STANDALONE__
    cv::UMat outputCustom;
    cvtColor((cv::UMat)input, outputCustom, CV_GRAY2BGR);

    cv::Mat ii;
    integral((cv::UMat)input, ii, CV_32S);

    int64_t preCustomTicks = FSL_Ticks();

    cv::Mat outputCustomMat;
    for(int y = 0; y < input.rows - windowHeight; ++y)
    {
      for(int x = 0; x < input.cols - windowWidth; ++x)
      {
        bool detected = true;
        for(int si = 0; si < (int)lbpStages.size(); ++si)
        {
          lbpStage lbps = lbpStages[si];

#if USE_FIXED_POINT
          STAGE_FIXED_POINT_TYPE stageSumFixed = 0;
#else
          double stageSum = .0;
#endif
          for(int fi = lbps.firstFeatureIndex; fi < lbps.firstFeatureIndex + lbps.featureCount; ++fi)
          {
            lbpFeature   lbpf = lbpFeatures[fi];
            lbpRectangle lbpr = lbpRectangles[lbpf.rectangleIndex];

            //grid pattern values
            int gpv[16];

            int gridStartX  = x + lbpr.x;
            int gridStartY  = y + lbpr.y;
            int gridOffsetX = 0;
            int gridOffsetY = 0;

            for(int wpY = 0; wpY < 4; ++wpY)
            {
              gridOffsetX = 0;
              for(int wpX = 0; wpX < 4; ++wpX)
              {
                int wpi = wpY * 4 + wpX;

                gpv[wpi] = (int)ii.at< int32_t >(gridStartY + gridOffsetY, gridStartX + gridOffsetX);
                gridOffsetX += lbpr.width;
              }
              gridOffsetY += lbpr.height;
            }

            int center = gpv[5] + gpv[10] - gpv[6] - gpv[9];

            int c0 = gpv[0] + gpv[5] - gpv[1] - gpv[4];
            int c1 = gpv[1] + gpv[6] - gpv[2] - gpv[5];
            int c2 = gpv[2] + gpv[7] - gpv[3] - gpv[6];
            int c3 = gpv[6] + gpv[11] - gpv[7] - gpv[10];
            int c4 = gpv[10] + gpv[15] - gpv[11] - gpv[14];
            int c5 = gpv[9] + gpv[14] - gpv[10] - gpv[13];
            int c6 = gpv[8] + gpv[13] - gpv[9] - gpv[12];
            int c7 = gpv[4] + gpv[9] - gpv[5] - gpv[8];

            int code = (c0 >= center ? 0x0080 : 0) | (c1 >= center ? 0x0040 : 0) | (c2 >= center ? 0x0020 : 0) |
                       (c3 >= center ? 0x0010 : 0) | (c4 >= center ? 0x0008 : 0) | (c5 >= center ? 0x0004 : 0) |
                       (c6 >= center ? 0x0002 : 0) | (c7 >= center ? 0x0001 : 0);

            //get one of the 256 bits contained in values[8] indexed by code
            int featureVal = lbpf.values[code >> 5];

            int leafIndex = featureVal & (1 << (code & 0x1f)) ? 0 : 1;
#if USE_FIXED_POINT
            stageSumFixed += lbpf.leafValuesFixed[leafIndex];
#else
            stageSum += lbpf.leafValues[leafIndex];
#endif
          }

#if USE_FIXED_POINT
          if(stageSumFixed < lbps.thresholdFixed)
#else
          if(stageSum < lbps.threshold)
#endif
          {
            detected = false;
            break;
          }
        }

        if(detected)
        {
          outputCustomMat = outputCustom.getMat(ACCESS_WRITE);
          cv::rectangle(outputCustomMat, Rect(x, y, windowWidth, windowHeight), Scalar(0, 255, 0));
        }
      }
    }

    int64_t postCustomTicks = FSL_Ticks();

    std::cout << "custom implementation finished" << endl;
#endif // if 0 (custom implementation)

#ifndef __STANDALONE__
    int64_t preCascadeClassifierTicks;
    int64_t postCascadeClassifierTicks;
    int64_t preOCVTicks;
    int64_t postOCVTicks;
    cv::Mat outputRef;
    {
      preCascadeClassifierTicks = FSL_Ticks();
      cv::ocl::setUseOpenCL(false);
      CascadeClassifier cf(modelFileName2); // model file for face detection in OpenCV 3.1.0

      if(cf.empty())
      {
        cout << "cascades not found" << endl;
        return -1;
      }
      postCascadeClassifierTicks = FSL_Ticks();
      std::cout << "cascade classifier finished" << endl;

      cvtColor((cv::UMat)input, outputRef, CV_GRAY2BGR);

      preOCVTicks = FSL_Ticks();
      //  cf.detectMultiScale(input, faces, 1.1, 0, 0, Size(24, 24), Size(24, 24));
      cv::Mat inMat = input.getMat(ACCESS_READ | OAL_USAGE_CACHED);
      cf.detectMultiScale(inMat, faces, 1.1, 1, 0, Size(24, 24), Size(24, 24));
      postOCVTicks = FSL_Ticks();

      for(unsigned int i = 0; i < faces.size(); ++i)
      {
        rectangle(outputRef, faces[i], Scalar(0, 255, 0));
      }
    }
    std::cout << "OpenCV implementation finished" << endl;
#endif

    vsdk::UMat dataInCascadeSizesAndSkip(1, 3, VSDK_CV_16UC1);
    vsdk::UMat dataInCascadeFeatures(1, 10000, VSDK_CV_8UC1);
    vsdk::UMat dataInCascadeStages(1, 200, VSDK_CV_8UC1);
    vsdk::UMat dataInPixelShifts(1, 64, VSDK_CV_8UC1);
    vsdk::UMat dataInPixelOffsets(1, 64, VSDK_CV_8UC1);

    vsdk::UMat dataInSkip(1, 1, VSDK_CV_8UC1);
    vsdk::UMat dataOut(input.rows, input.cols, VSDK_CV_8UC1);

    {
      vsdk::Mat dataInCascadeSizesAndSkipMat         = dataInCascadeSizesAndSkip.getMat(ACCESS_READ | OAL_USAGE_CACHED);
      dataInCascadeSizesAndSkipMat.at< uint16_t >(0) = uint16_t(lbpFeatures.size());
      dataInCascadeSizesAndSkipMat.at< uint16_t >(1) = uint16_t(lbpStages.size());
      dataInCascadeSizesAndSkipMat.at< uint16_t >(2) = 0;

      memcpy(dataInCascadeFeatures.getMat(ACCESS_WRITE | OAL_USAGE_CACHED).data, apexLBPFeatures, apexLBPFeaturesSize);
      memcpy(dataInCascadeStages.getMat(ACCESS_WRITE | OAL_USAGE_CACHED).data, apexLBPStages, apexLBPStagesSize);

      uint8_t pixelShifts[64];
      uint8_t pixelOffsets[64];
      for(int i = 0; i < 64; ++i)
      {
        pixelShifts[i]  = uint8_t(i / CHUNK_WIDTH);
        pixelOffsets[i] = uint8_t(i % CHUNK_WIDTH);
      }

      memcpy(dataInPixelShifts.getMat(ACCESS_WRITE | OAL_USAGE_CACHED).data, pixelShifts, 64);
      memcpy(dataInPixelOffsets.getMat(ACCESS_WRITE | OAL_USAGE_CACHED).data, pixelOffsets, 64);
    }

    lRetVal = 0;
    lRetVal |= process.Initialize();
    lRetVal |= process.ConnectIO(FACE_DET_GR_IN_IMG, input);
    lRetVal |= process.ConnectIO(FACE_DET_GR_IN_CASC_SZ, dataInCascadeSizesAndSkip);
    lRetVal |= process.ConnectIO(FACE_DET_GR_IN_CASC_FEAT, dataInCascadeFeatures);
    lRetVal |= process.ConnectIO(FACE_DET_GR_IN_CASC_STAGES, dataInCascadeStages);
    lRetVal |= process.ConnectIO(FACE_DET_GR_IN_PIX_SHFT, dataInPixelShifts);
    lRetVal |= process.ConnectIO(FACE_DET_GR_IN_PIX_OFFS, dataInPixelOffsets);
    lRetVal |= process.ConnectIO(FACE_DET_GR_OUT, dataOut);

    int64_t preAPEXTicks = FSL_Ticks();
    lRetVal |= process.Start();
    lRetVal |= process.Wait();
    int64_t postAPEXTicks = FSL_Ticks();

    printf("APEX implementation finished\n");

#ifdef APEX2_EMULATE
    if(lRetVal || process.ErrorOccured())
    {
      printf("process error!\n");
      printf("%s\n", process.ErrorMessage().c_str());
    }
#endif

    cv::Mat outputAPEXmask = dataOut.getMat(ACCESS_READ | OAL_USAGE_CACHED);
    cv::Mat outputAPEX;
    cvtColor((cv::UMat)input, outputAPEX, CV_GRAY2BGR);
    for(int y = windowHeight; y < input.rows; ++y)
    {
      for(int x = 0; x < input.cols - windowWidth; ++x)
      {
        if(outputAPEXmask.at< uint8_t >(y, x) != 0)
        {
          rectangle(outputAPEX, Rect(x, y - windowHeight, windowWidth, windowHeight), Scalar(0, 255, 0));
          //rectangle(outputAPEX, Rect(x, y, 1, 1), Scalar(0, 255, 0));
        }
      }
    }

#ifndef __STANDALONE__
    printf("custom host implementation:        %fs.\n", FSL_TicksToSeconds(postCustomTicks - preCustomTicks));
    printf("cascade classifier implementation: %fs.\n",
           FSL_TicksToSeconds(postCascadeClassifierTicks - preCascadeClassifierTicks));
    printf("APEX implementation:               %fs.\n", FSL_TicksToSeconds(postAPEXTicks - preAPEXTicks));
    printf("OpenCV implementation:             %fs.\n", FSL_TicksToSeconds(postOCVTicks - preOCVTicks));
#endif

//////////////////////////////////////////////////
// Process the output - if standalone, compare with reference
//                    - if OS, save the images
#if !defined(APEX2_EMULATE) && !defined(__INTEGRITY__)
#ifndef __STANDALONE__
    imwrite(OUTPUT_ROOT "faces_ref.png", outputRef);
    imwrite(OUTPUT_ROOT "faces_custom.png", outputCustomMat);
    imwrite(OUTPUT_ROOT "faces_apex.png", (cv::Mat)outputAPEX);
#endif

#ifdef __STANDALONE__
    io::FrameOutputDCU output(1280, 720, io::IO_DATA_DEPTH_08, CHNL_CNT);
#else
    io::FrameOutputV234Fb output(1280, 720, io::IO_DATA_DEPTH_08, CHNL_CNT);
#endif

    vsdk::UMat output_umat = vsdk::UMat(720, 1280, CV_8UC3);
    {
      cv::Mat output_mat = output_umat.getMat(ACCESS_WRITE | OAL_USAGE_CACHED);
      memset(output_mat.data, 0, 1280 * 720 * 3);

      outputAPEX.copyTo(output_mat(cv::Rect(0, (720 - outputAPEX.rows) / 2, outputAPEX.cols, outputAPEX.rows)));
    }
    output.PutFrame(output_umat);

#else
#ifdef APEX2_EMULATE
    imshow("reference OpenCV output", outputRef);
    imwrite(OUTPUT_ROOT "faces_ref.png", outputRef);
    imshow("input", (cv::UMat)input);
    imshow("host output", outputCustomMat);
    imshow("APEX output", (cv::Mat)outputAPEX);
    waitKey();
#else
    imwrite(OUTPUT_ROOT "faces_ref.png", outputRef);
    imwrite(OUTPUT_ROOT "faces_custom.png", outputCustomMat);
    imwrite(OUTPUT_ROOT "faces_apex.png", (cv::Mat)outputAPEX);
#endif
#endif
  }

  return EndFct(lRetVal, apexLBPFeatures, apexLBPStages);
}
