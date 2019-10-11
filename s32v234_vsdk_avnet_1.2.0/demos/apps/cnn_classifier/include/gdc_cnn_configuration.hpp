/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

/*!*********************************************************************************
*  @file
*  @brief Configuration related values for this application.
*
*
*  This file contains configuration information related to this application.<br>
*
***********************************************************************************/

#ifndef GDCCNNCONFIGURATION_HPP
#define GDCCNNCONFIGURATION_HPP

#define SQUEEZENET11 1

#include "frame_io_types.h"
#include "acf_process_apu.h"
#include <squeeze_net_v11.hpp>
#include <squeeze_net_v10.hpp>
#include <stdint.h>


/***************************************************************************/
// Resource Files
/***************************************************************************/
const char gcIniFileName[] = "data/apps/cnn_classifier/gdc_cnn.ini";



/***************************************************************************/
// App
/***************************************************************************/
enum GdcCnn_SceneTask_Enum
{
   GDC_CNN_SCENE_TASK_INVALID      = 0,
   GDC_CNN_SCENE_TASK_FILE_READER  = 1,
   GDC_CNN_SCENE_TASK_IMAGE_SENSOR = 2,
   GDC_CNN_SCENE_TASK_IP_CAMERA    = 3,
   GDC_CNN_SCENE_TASK_END          = 4
};

const GdcCnn_SceneTask_Enum gcSceneTaskEnum = GDC_CNN_SCENE_TASK_FILE_READER;


const bool gcIsRunningWithThreads = true;



const uint32_t gcInputImageQueueSize    = 5;

const uint32_t gcSceneWidthInPixels     = 227;
const uint32_t gcSceneHeightInPixels    = 227;
const uint32_t gcSceneBitsPerPixel      = 24;
const uint32_t gcScenePaddingInBytes    = 0;
const uint32_t gcScenePaddingInPixels   = (gcScenePaddingInBytes*8)/gcSceneBitsPerPixel;
const uint32_t gcSceneSpanInBytes       = (gcSceneWidthInPixels * gcSceneBitsPerPixel / 8) + 2*gcScenePaddingInBytes;
const uint32_t gcSceneSizeInBytes       = gcSceneWidthInPixels * gcSceneHeightInPixels * gcSceneBitsPerPixel / 8; // 8 bits per byte
const uint32_t gcSceneBufferSizeInBytes = gcSceneSpanInBytes * ((gcSceneHeightInPixels * gcSceneBitsPerPixel / 8) + 2*gcScenePaddingInBytes);
const uint32_t gcSceneOffsetInBytes     = (gcSceneSpanInBytes * gcScenePaddingInBytes) + gcScenePaddingInBytes;
const uint32_t gcSceneNumberOfChannels  = 3; // R G B, 8 bits each


const int32_t gcBgrChannelMeans[gcSceneNumberOfChannels] = {104, 117, 123};

//
// Padded image splits the original RGB scene into separate channels
// +---------------------+
// |   +---+ +---+ +---+ |
// |   | B | | G | | R | |
// |   +---+ +---+ +---+ |
// +---------------------+
const uint32_t gcPadding_TopInBytes            = 2;  // Number of rows at the top of image
const uint32_t gcPadding_BottomInBytes         = 4; // Number of rows at the bottom of image
const uint32_t gcPadding_LeftInBytes           = 16; // Number of columns at the start of image (1X)
const uint32_t gcPadding_RightInBytes          = 1;  // Number of columns after every channel   (3X)
const uint32_t gcPaddedScene_BitsPerPixel      = 8;
const uint32_t gcPaddedScene_WidthInPixels     = gcPadding_LeftInBytes + ((gcSceneWidthInPixels + gcPadding_RightInBytes) * (gcSceneBitsPerPixel/8)); // 8 bits per byte
const uint32_t gcPaddedScene_SpanInBytes       = gcPaddedScene_WidthInPixels * (gcPaddedScene_BitsPerPixel/8); // 8 bits per byte
const uint32_t gcPaddedScene_HeightInPixels    = gcPadding_TopInBytes + gcSceneHeightInPixels + gcPadding_BottomInBytes;
const uint32_t gcPaddedScene_BufferSizeInBytes = gcPaddedScene_SpanInBytes * gcPaddedScene_HeightInPixels;
const uint32_t gcPaddedScene_OffsetInBytes_B   = (gcPadding_TopInBytes * gcPaddedScene_SpanInBytes) + gcPadding_LeftInBytes + 0*(gcSceneWidthInPixels + gcPadding_RightInBytes);
const uint32_t gcPaddedScene_OffsetInBytes_G   = (gcPadding_TopInBytes * gcPaddedScene_SpanInBytes) + gcPadding_LeftInBytes + 1*(gcSceneWidthInPixels + gcPadding_RightInBytes);
const uint32_t gcPaddedScene_OffsetInBytes_R   = (gcPadding_TopInBytes * gcPaddedScene_SpanInBytes) + gcPadding_LeftInBytes + 2*(gcSceneWidthInPixels + gcPadding_RightInBytes);






/***************************************************************************/
// FileReaderTask
/***************************************************************************/
const char gcDatasetDirectory[]    = "squeeze_net/res/open_images/";
const char gcDatasetListFilename[] = "dataset_list_open_image.txt";
const char     gcProfileFileName[] = "cnn_profiling_result.txt";





/***************************************************************************/
// ImageSensorTask
/***************************************************************************/
const uint32_t gcImageSensorWidthInPixels     = 1280;
const uint32_t gcImageSensorHeightInPixels    = 720;
const uint32_t gcImageSensorBitsPerPixel      = 24;
const uint32_t gcImageSensorNumberOfBuffers   = 3;
const uint32_t gcImageSensorBufferSizeInBytes = gcImageSensorWidthInPixels * gcImageSensorHeightInPixels * gcImageSensorBitsPerPixel / 8; // 8 Bits per byte

const bool gcIsImageSensorFlippedHorizontally = false;  // false for LCD, true for HDMI output
const bool gcIsImageSensorFlippedVertically   = false;  // false for LCD, true for HDMI output

const io::IO_DATA_CHANNELS gcImageSensorIoDataChannels = io::IO_DATA_CH3;






/***************************************************************************/
// IpCameraTask
/***************************************************************************/
const char gcIpCameraAddress[] = "http://root:ces_gdc_demo@10.81.112.203/mjpg/video.mjpg";





/***************************************************************************/
// ClassifierTask
/***************************************************************************/
#if SQUEEZENET11
const char gcSqueezeNetModel_Filename[]   = "data/apps/cnn_classifier/SqueezeV11Quantized.model";
const uint32_t gcSqueezeNetModel_Filesize = APEXCV_SQUEEZENET_V11_WEIGHT_MODEL_BYTES;
#else
const char gcSqueezeNetModel_Filename[] = "data/apps/cnn_classifier/SqueezeV10Quant8_OPENIMAGES.model";
const uint32_t gcSqueezeNetModel_Filesize = APEXCV_SQUEEZENET_V10_WEIGHT_MODEL_BYTES;
#endif

const char gcSqueezeNetSynonymSet_Filename[]   = "data/apps/cnn_classifier/synset_words.txt";

const uint32_t gcSqueezeNet_NumberOfClassifications = 5;

const char gcSqueezeNetReferenceResults_Filename[] = "data/apps/cnn_classifier/val_openimages.txt";


/***************************************************************************/
// DisplayTask
/***************************************************************************/
const uint32_t gcDisplayScaleNumerator    = 1;     // Kludge for now...
const uint32_t gcDisplayScaleDenominator  = 1;     // Kludge for now...
const uint32_t gcDisplayWidthInPixels     = 1920;  // Will be scale according to (gcDisplayWidthInPixels*gcDisplayScaleNumerator)/gcDisplayScaleDenominator
const uint32_t gcDisplayHeightPerPixels   = 1080;  // Will be scale according to (gcDisplayHeightPerPixels*gcDisplayScaleNumerator)/gcDisplayScaleDenominator
const uint32_t gcDisplayBitsPerPixel      = 24;

const io::IO_DATA_DEPTH    gcDisplayIoDataDepth    = io::IO_DATA_DEPTH_08;
const io::IO_DATA_CHANNELS gcDisplayIoDataChannels = io::IO_DATA_CH3;






/***************************************************************************/
// DEBUG
/***************************************************************************/
#ifdef __STANDALONE__
      const uint32_t cTimeDivisor = 1; // In standalone values are already in microseconds
#else
      const uint32_t cTimeDivisor = 1000; // In linux values are in nanoseconds
#endif

/***************************************************************************/
// for profiling
/***************************************************************************/

const int32_t gcSceneTick               = 0;  // [0] Entry, [1] Return
const int32_t gcResizeTick              = 2;  // [2] Entry, [3] After Resize [4] Return
const int32_t gcClassifierTick          = 5;  // [5] Entry, [6] Before SqueezeNet, [7] After SqueezeNet, [8] Return
const int32_t gcCompTick                = 9;  // [9] Entry, [10] Return
const int32_t gcDisplayTick             = 11; // [11] Entry, [12] Return
const int32_t gcApexCvEndTick           = 13; // end
const int32_t gcFileNameSize            = 64;

#endif /* GDCCNNCONFIGURATION_HPP */
