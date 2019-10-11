/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
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
#include <apex.h>
#include <common_time_measure.h>
#include <common_helpers.h>
#include <string>

#include <APU_FACE_DETECTION_LBP_4.hpp>

#ifdef __STANDALONE__
#include "frame_output_dcu.h"
#define CHNL_CNT io::IO_DATA_CH3
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"
#define CHNL_CNT io::IO_DATA_CH3
#include <signal.h>
#endif // else from #ifdef __STANDALONE__

#include <auxiliary.h>
#include <xml_input.h>
#include <camera_input.h>

using namespace icp;

//***************************************************************************
// Possible to set input resolution (must be supported by the DCU)
#define WIDTH 1920       ///< width of DDR buffer in pixels
#define HEIGHT 1080      ///< height of DDR buffer in pixels
#define DDR_BUFFER_CNT 3 ///< number of DDR buffers per ISP stream

// Input resolution
const int  RESX        = 1280;
const int  RESY        = 720;
int        APEX_STAGES = 20;
const bool SKIP_ODD    = true;

std::vector< DetectedFaceFiltered > filtered_faces;
std::vector< DetectedFaceFiltered > filtered_faces_now;
int                                 color_i = 0xc;

//***************************************************************************
// Main
int main(int argc, char** argv)
{
  std::string helpMsg =
      std::string("APEX Face detection demo, which uses the camera and the ISP for input.\n\n\tUsage: ") +
      COMMON_ExtractProgramName(argv[0]);
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  APEX_Init();

  /////////////////////////////////////////////////////////////////////////////
  // Algo setup - allocate the output display buffer
  vsdk::UMat out_buffer_umat = vsdk::UMat(1080, 1920, VSDK_CV_8UC3);
  memset(out_buffer_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, 0, 1920 * 1080 * 3);

  // Read the XML input with cascades (in standalone, it's hexdump)
  XMLReader xmlreader;
  xmlreader.Init();

  /////////////////////////////////////////////////////////////////////////////
  // I/O setup
  // Number of APEX stages - has to be even number
  const int SIZES               = 12;
  const int MIN_FACE_RECTANGLES = 2;
  int       cusActive[SIZES]    = {40, 36, 32, 28, 24, 22, 20, 18, 16, 14, 12, 10};

  const int CHUNK_WIDTH = 4;

  /////////////////////////////////////////////////////////////////////////////
  // Data setting
  Size pyramidStartSize =
      Size(cusActive[0] * CHUNK_WIDTH, int((cusActive[0] * CHUNK_WIDTH) * (float(RESY) / float(RESX))));

  vsdk::UMat inputs[SIZES];
  vsdk::UMat dataInCascadeSizesAndSkip(1, 3, VSDK_CV_16UC1);
  vsdk::UMat dataInCascadeFeatures(1, 10000, VSDK_CV_8UC1);
  vsdk::UMat dataInCascadeStages(1, 200, VSDK_CV_8UC1);
  vsdk::UMat dataInPixelShiftsA(1, 64, VSDK_CV_8UC1);
  vsdk::UMat dataInPixelOffsetsA(1, 64, VSDK_CV_8UC1);
  vsdk::UMat dataInPixelShiftsB(1, 64, VSDK_CV_8UC1);
  vsdk::UMat dataInPixelOffsetsB(1, 64, VSDK_CV_8UC1);
  vsdk::UMat dataOuts[SIZES];
  vsdk::UMat dataOutIntegralImages[SIZES];
  vsdk::UMat frame(720, 1280, VSDK_CV_8UC3);
  vsdk::UMat pyramidStartFrame(pyramidStartSize.height, pyramidStartSize.width, VSDK_CV_8UC3);

  memset(frame.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, 0, 720 * 1280 * 3);

  {
    vsdk::Mat dataInCascadeSizesAndSkipMat = dataInCascadeSizesAndSkip.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
    uint16_t* cascadeSizesAndSkipPtr       = (uint16_t*)dataInCascadeSizesAndSkipMat.data;
    cascadeSizesAndSkipPtr[0]              = uint16_t(xmlreader.lbpFeatures.size());
    cascadeSizesAndSkipPtr[1]              = uint16_t(xmlreader.lbpStages.size());

    if(APEX_STAGES < 0 || APEX_STAGES >= int(xmlreader.lbpFeatures.size()))
    {
      APEX_STAGES = 0;
    }

    cascadeSizesAndSkipPtr[1] = uint16_t(APEX_STAGES);
    cascadeSizesAndSkipPtr[2] = SKIP_ODD ? 1 : 0;
  }

  memcpy(dataInCascadeFeatures.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, xmlreader.apexLBPFeatures,
         xmlreader.apexLBPFeaturesSize);
  memcpy(dataInCascadeStages.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, xmlreader.apexLBPStages,
         xmlreader.apexLBPStagesSize);

  uint8_t pixelShiftsA[64];
  uint8_t pixelOffsetsA[64];
  uint8_t pixelShiftsB[64];
  uint8_t pixelOffsetsB[64];
  for(int i = 0; i < 64; ++i)
  {
    pixelShiftsA[i]  = uint8_t(i / CHUNK_WIDTH);
    pixelOffsetsA[i] = uint8_t(i % CHUNK_WIDTH);
    pixelShiftsB[i]  = uint8_t(i / CHUNK_WIDTH);
    pixelOffsetsB[i] = uint8_t(i % CHUNK_WIDTH);
  }
  memcpy(dataInPixelShiftsA.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, pixelShiftsA, 64);
  memcpy(dataInPixelOffsetsA.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, pixelOffsetsA, 64);
  memcpy(dataInPixelShiftsB.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, pixelShiftsB, 64);
  memcpy(dataInPixelOffsetsB.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, pixelOffsetsB, 64);

  /////////////////////////////////////////////////////////////////////////////
  // APEX Process setup

  APU_FACE_DETECTION_LBP_4 process0(APEX_APEX0);
  APU_FACE_DETECTION_LBP_4 process1(APEX_APEX1);

  // Bind the data with the APEX process
  int lRetVal = 0;
  lRetVal |= process0.Initialize();
  lRetVal |= process0.ConnectIO("INPUT_CASCADE_SIZES_AND_SKIP", dataInCascadeSizesAndSkip);
  lRetVal |= process0.ConnectIO("INPUT_CASCADE_FEATURES", dataInCascadeFeatures);
  lRetVal |= process0.ConnectIO("INPUT_CASCADE_STAGES", dataInCascadeStages);
  lRetVal |= process0.ConnectIO("INPUT_PIXEL_SHIFTS", dataInPixelShiftsA);
  lRetVal |= process0.ConnectIO("INPUT_PIXEL_OFFSETS", dataInPixelOffsetsA);
  lRetVal |= process1.Initialize();
  lRetVal |= process1.ConnectIO("INPUT_CASCADE_SIZES_AND_SKIP", dataInCascadeSizesAndSkip);
  lRetVal |= process1.ConnectIO("INPUT_CASCADE_FEATURES", dataInCascadeFeatures);
  lRetVal |= process1.ConnectIO("INPUT_CASCADE_STAGES", dataInCascadeStages);
  lRetVal |= process1.ConnectIO("INPUT_PIXEL_SHIFTS", dataInPixelShiftsB);
  lRetVal |= process1.ConnectIO("INPUT_PIXEL_OFFSETS", dataInPixelOffsetsB);

  ////////////////////////////////////////////////////////////////////////////
  // Initial data setup

  Resample((char*)pyramidStartFrame.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data,
           (char*)frame.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, pyramidStartSize.width,
           pyramidStartSize.height, frame.cols, frame.rows);
  inputs[0] = vsdk::UMat(pyramidStartFrame.rows, pyramidStartFrame.cols, VSDK_CV_8UC1);

  GrayScale((unsigned char*)inputs[0].getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data,
            (unsigned char*)pyramidStartFrame.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data,
            pyramidStartFrame.cols, pyramidStartFrame.rows);
  Size  sizes[SIZES];
  float scales[SIZES];
  sizes[0]  = Size(inputs[0].cols, inputs[0].rows);
  scales[0] = frame.cols / float(pyramidStartFrame.cols);

  dataOuts[0]              = vsdk::UMat(sizes[0].height, sizes[0].width, VSDK_CV_8UC1);
  dataOutIntegralImages[0] = vsdk::UMat(sizes[0].height, sizes[0].width, VSDK_CV_32SC1);

  for(int i = 1; i < SIZES; ++i)
  {
    int   originalImageIndex = i - 1; //i % 2;
    int   chunkWidth         = originalImageIndex == 0 ? CHUNK_WIDTH : CHUNK_WIDTH;
    int   width              = cusActive[i] * chunkWidth;
    float interPyramidScale  = width / float(sizes[0].width);
    int   height             = int(sizes[0].height * interPyramidScale);

    sizes[i]  = Size(width, height);
    scales[i] = frame.cols / float(width);

    inputs[i] = vsdk::UMat(sizes[i].height, sizes[i].width, VSDK_CV_8UC1);
    ResampleG((char*)inputs[i].getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data,
              (char*)inputs[originalImageIndex].getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, sizes[i].width,
              sizes[i].height, sizes[originalImageIndex].width,
              sizes[originalImageIndex].height); //inputs[originalImageIndex].cols, inputs[originalImageIndex].rows);

    dataOuts[i]              = vsdk::UMat(sizes[i].height, sizes[i].width, VSDK_CV_8UC1);
    dataOutIntegralImages[i] = vsdk::UMat(sizes[i].height, sizes[i].width, VSDK_CV_32SC1);
  }

////////////////////////////////////////////////////////////////////////////
// DCU out setup
#ifdef __STANDALONE__
  io::FrameOutputDCU lDcuOutput(WIDTH, HEIGHT, io::IO_DATA_DEPTH_08, io::IO_DATA_CH3);
#else
  // setup Ctrl+C handler
  if(ISPCameraInputStereo::SigintSetup() != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to register Ctrl+C signal handler.");
    return -1;
  }

  printf("Press Ctrl+C to terminate the demo.\n");

  io::FrameOutputV234Fb lDcuOutput(RESX, RESY, io::IO_DATA_DEPTH_08, io::IO_DATA_CH3);
#endif

  ////////////////////////////////////////////////////////////////////////////
  // ISP in setup

  ISPCameraInputStereo camerainput;
  camerainput.Init();

  // fetched frame buffer storage
  SDI_Frame lFrameRgb;
  SDI_Frame lFrameYsub;

  camerainput.Start();

  //unsigned long startframe = 0;
  //unsigned long endframe = 0;
//#define __DEBUG_OUT__
#ifdef __DEBUG_OUT__
  unsigned long startalgo = 0;
  unsigned long endalgo   = 0;
#endif
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // Main loop
  for(;;)
  {
    //startframe = FSL_Ticks();
    /////////////////////////////////////////////////////////////////////////////
    // Get Frames
    camerainput.GetFrame(FDMA_IX_RGB888, lFrameRgb);
    if(lFrameRgb.mUMat.empty())
    {
      printf("Failed to pop the RGB frame.\n");
      break;
    }

    camerainput.GetFrame(FDMA_IX_G_SUB_2, lFrameYsub);
    if(lFrameYsub.mUMat.empty())
    {
      printf("Failed to pop the G subsumpled frame.\n");
      break;
    }

#ifdef __DEBUG_OUT__
    startalgo = FSL_Ticks();
#endif
    /////////////////////////////////////////////////////////////////////////////
    // Resample the pyramid
    ResampleG((char*)inputs[0].getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data,
              (char*)lFrameYsub.mUMat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, pyramidStartSize.width,
              pyramidStartSize.height, 640, 360);
    for(int i = 1; i < SIZES; ++i)
    {
      int originalImageIndex = i - 1; // % 2;
      ResampleG((char*)inputs[i].getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data,
                (char*)inputs[originalImageIndex].getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, sizes[i].width,
                sizes[i].height, sizes[originalImageIndex].width,
                sizes[originalImageIndex].height); //inputs[originalImageIndex].cols, inputs[originalImageIndex].rows);
    }

    /////////////////////////////////////////////////////////////////////////////
    // Start the APEXes
    for(int i = SIZES - 1; i > 0; i -= 2)
    {
      int i2 = i - 1;

      lRetVal |= process1.ConnectIO("INPUT_IMAGE", inputs[i]);
      lRetVal |= process1.ConnectIO("OUTPUT", dataOuts[i]);
      lRetVal |= process1.ConnectIO("OUTPUT_INTEGRAL_IMAGE", dataOutIntegralImages[i]);
      lRetVal |= process1.Start();
      lRetVal |= process0.ConnectIO("INPUT_IMAGE", inputs[i2]);
      lRetVal |= process0.ConnectIO("OUTPUT", dataOuts[i2]);
      lRetVal |= process0.ConnectIO("OUTPUT_INTEGRAL_IMAGE", dataOutIntegralImages[i2]);
      lRetVal |= process0.Start();

      lRetVal |= process1.Wait();
      lRetVal |= process0.Wait();
    }

    /////////////////////////////////////////////////////////////////////////////
    // Go through the output and get the face data
    vector< DetectedFace > faces;
    for(int i = 0; i < SIZES; ++i)
    {
      vsdk::Mat dataOutsMat        = dataOuts[i].getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);
      uint8_t*  data               = (uint8_t*)dataOutsMat.data;
      int       scaledWindowWidth  = int(xmlreader.windowWidth * scales[i]);
      int       scaledWindowHeight = int(xmlreader.windowHeight * scales[i]);
      for(int y = xmlreader.windowHeight; y < inputs[i].rows; y += 2)
      {
        for(int x = 0; x < inputs[i].cols - xmlreader.windowWidth; x += 2)
        {
          if(data[y * dataOuts[i].cols + x] != 0)
          {
            int           realX = int(x * scales[i]);
            int           realY = int(y * scales[i] - scaledWindowHeight);
            FaceRectangle fr(realX, realY, scaledWindowWidth, scaledWindowHeight);
            bool          addNewFace = true;
            for(unsigned int j = 0; j < faces.size(); ++j)
            {
              if(faces[j].TryAddFaceRectangle(fr))
              {
                addNewFace = false;
                break;
              }
            }

            if(addNewFace)
            {
              faces.push_back(DetectedFace(fr));
            }
          }
        }
      }
    }

    for(unsigned int j = 0; j < filtered_faces.size(); ++j)
    {
      filtered_faces[j].framesNotDetected += 1;
    }

    filtered_faces_now.clear();
    unsigned long passed = 0x0;
    for(unsigned int i = 0; i < faces.size(); ++i)
    {
      bool          found_match = false;
      int           found_j     = -1;
      int           bestx       = 9999;
      int           besty       = 9999;
      FaceRectangle fr          = faces[i].GetMidFaceRectangle();
      for(unsigned int j = 0; j < filtered_faces.size(); ++j)
      {
        FaceRectangle fr_filtered = filtered_faces[j]._detectedFace.testingFaceRectangle;
        int           xdiff       = abs((fr.x + (fr.width >> 1)) - (fr_filtered.x + (fr_filtered.width >> 1)));
        int           ydiff       = abs((fr.y + (fr.height >> 1)) - (fr_filtered.y + (fr_filtered.height >> 1)));
        int           wdiff       = abs(fr.width - fr_filtered.width);
        int           hdiff       = abs(fr.height - fr_filtered.height);
        if((passed & (1 << j)) == 0 && xdiff < fr.width && ydiff < fr.height && wdiff < fr.width && hdiff < fr.height &&
           xdiff < bestx && ydiff < besty)
        {
          found_match = true;
          found_j     = j;
          bestx       = xdiff;
          besty       = ydiff;
          passed |= 1 << j;
        }
      }
      if(found_match)
      {
        filtered_faces[found_j].Update(faces[i]);
        filtered_faces_now.push_back(filtered_faces[found_j]);
      }
      else
      {
        filtered_faces_now.push_back(DetectedFaceFiltered(faces[i]));
        if(filtered_faces_now.size() & 0x00000002)
          filtered_faces_now[filtered_faces_now.size() - 1].color[0] = 255;
        else
          filtered_faces_now[filtered_faces_now.size() - 1].color[0] = 0;
        if(filtered_faces_now.size() & 0x00000001)
          filtered_faces_now[filtered_faces_now.size() - 1].color[1] = 255;
        else
          filtered_faces_now[filtered_faces_now.size() - 1].color[1] = 0;
        if(filtered_faces_now.size() & 0x00000004)
          filtered_faces_now[filtered_faces_now.size() - 1].color[2] = 255;
        else
          filtered_faces_now[filtered_faces_now.size() - 1].color[2] = 0;
        ++color_i;
      }
    }

    for(unsigned int j = 0; j < filtered_faces.size(); ++j)
    {
      if((passed & (1 << j)) == 0 && filtered_faces[j].framesNotDetected > 0 && filtered_faces[j].framesNotDetected < 8)

        filtered_faces_now.push_back(filtered_faces[j]);
    }
    filtered_faces.clear();
    for(unsigned int j = 0; j < filtered_faces_now.size(); ++j)
    {
      filtered_faces.push_back(filtered_faces_now[j]);
    }

    if(filtered_faces.size() == 0)
      color_i = 0xc;

    {
      vsdk::Mat lFrameRgbmat   = lFrameRgb.mUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
      int       finalFaceCount = 0;
      for(unsigned int i = 0; i < filtered_faces.size(); ++i)
      {
        if(filtered_faces[i].framesDetected >= 5)
        {
          ++finalFaceCount;
          FaceRectangle fr = filtered_faces[i]._detectedFace.testingFaceRectangle;
          DrawRectangle(lFrameRgbmat.data, fr.x, fr.y, fr.width, fr.height, filtered_faces[i].color[0],
                        filtered_faces[i].color[1], filtered_faces[i].color[2]);
        }
      }

#ifdef __DEBUG_OUT__
      endalgo = FSL_Ticks();
      printf("FPS: %f\n", 1.0 / FSL_TicksToSeconds(endalgo - startalgo));
#endif
#ifndef __STANDALONE__
      neon_memcpy_1280((char*)out_buffer_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data,
                       (char*)lFrameRgbmat.data);
#else
      neon_memcpy_1280_to_hd((char*)out_buffer_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data,
                             (char*)lFrameRgbmat.data);
#endif
    }
    lDcuOutput.PutFrame(out_buffer_umat);

    camerainput.PutFrame(lFrameRgb);
    camerainput.PutFrame(lFrameYsub);

    camerainput.ExpControl();

//endframe = FSL_Ticks();
//printf("Done frame in %f\n", FSL_TicksToSeconds(endframe-startframe));
#ifndef __STANDALONE__
    if(ISPCameraInputStereo::sStop)
    {
      printf("*** STOP ***\n");
      break; // break if Ctrl+C pressed
    }        // if Ctrl+C
#endif       //#ifndef __STANDALONE__
  }          // for(;;)

  //*** Stop ISP processing ***
  camerainput.Stop();
  camerainput.Deinit();

  //
  // Close SDI
  //
  sdi::Close(0);
  return 0;

} // main()

//***************************************************************************
