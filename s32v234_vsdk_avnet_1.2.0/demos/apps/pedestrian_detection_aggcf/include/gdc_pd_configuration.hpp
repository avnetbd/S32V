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

#ifndef GDCPEDESTRIANDETECTIONCONFIGURATION_HPP
#define GDCPEDESTRIANDETECTIONCONFIGURATION_HPP

//#include "gdc_pd_types.hpp"
#include "frame_io_types.h"
#include "acf_process_apu.h"
#include <stdint.h>


/***************************************************************************/
// Resource Files
/***************************************************************************/
const char gcDataPath[]    = "data/apps/pedestrian_detection_aggcf/";
const char gcIniFileName[] = "data/apps/pedestrian_detection_aggcf/gdc_pd.ini";

const char gcProfileFileName[] = "pd_aggcf_profiling.txt";


/***************************************************************************/
// App
/***************************************************************************/
enum GdcPd_SceneTask_Enum
{
   GDC_PD_SCENE_TASK_INVALID      = 0,
   GDC_PD_SCENE_TASK_FILE_READER  = 1,
   GDC_PD_SCENE_TASK_IMAGE_SENSOR = 2,
   GDC_PD_SCENE_TASK_IP_CAMERA    = 3,
   GDC_PD_SCENE_TASK_END          = 4
};

const GdcPd_SceneTask_Enum gcSceneTaskEnum = GDC_PD_SCENE_TASK_FILE_READER;

const bool gcIsRunningWithThreads = true;


const uint32_t gcInputImageQueueSize    = 5;
const uint32_t gcSceneWidthInPixels     = 640;
const uint32_t gcSceneHeightInPixels    = 480;
const uint32_t gcSceneBitsPerPixel      = 32;
const uint32_t gcScenePaddingInBytes    = 0;
const uint32_t gcScenePaddingInPixels   = (gcScenePaddingInBytes*8)/gcSceneBitsPerPixel;
const uint32_t gcSceneSpanInBytes       = (gcSceneWidthInPixels * gcSceneBitsPerPixel / 8) + 2*gcScenePaddingInBytes;
const uint32_t gcSceneSizeInBytes       = gcSceneWidthInPixels * gcSceneHeightInPixels * gcSceneBitsPerPixel / 8; // 8 bits per byte
const uint32_t gcSceneBufferSizeInBytes = (gcSceneSpanInBytes * gcSceneHeightInPixels) + 2*gcScenePaddingInBytes;
const uint32_t gcSceneOffsetInBytes     = (gcSceneSpanInBytes * gcScenePaddingInBytes) + gcScenePaddingInBytes;

const uint32_t gcSceneTask_ImageRequests = gcInputImageQueueSize;



/***************************************************************************/
// FileReaderTask
/***************************************************************************/
const uint32_t gcFileNameSize = 64;
const char     gcFileName[]   = "out.bin";



/***************************************************************************/
// ImageSensorTask
/***************************************************************************/
const uint32_t gcImageSensorWidthInPixels     = 1280;
const uint32_t gcImageSensorHeightInPixels    = 720;
const uint32_t gcImageSensorBitsPerPixel      = 24;
const uint32_t gcImageSensorNumberOfBuffers   = 4;
const uint32_t gcImageSensorBufferSizeInBytes = gcImageSensorWidthInPixels * gcImageSensorHeightInPixels * gcImageSensorBitsPerPixel / 8; // 8 Bits per byte

const bool gcWeFlipImageSensorHorizontally = false;  // false for LCD, true for HDMI output
const bool gcWeFlipImageSensorVertically   = false;  // false for LCD, true for HDMI output

const io::IO_DATA_CHANNELS gcImageSensorIoDataChannels = io::IO_DATA_CH3;






/***************************************************************************/
// IpCameraTask
/***************************************************************************/
const char gcIpCameraAddress[] = "http://root:ces_gdc_demo@10.81.112.203/mjpg/video.mjpg";






/***************************************************************************/
// ResizeTask
/***************************************************************************/

const uint32_t gcNumberOfRealScales                            = 3;
const uint32_t gcNumberOfApproxScales                          = 3;
const uint32_t gcTotalNumberOfApproxScales                     = gcNumberOfApproxScales * gcNumberOfRealScales;
const uint32_t gcRealScaleIndex[gcNumberOfRealScales]          = {0,4,8};
const uint32_t gcApproxScaleIndex[gcTotalNumberOfApproxScales] = {1,2,3,5,6,7,9,10,11};
const uint32_t gcMajorSceneResizes[gcNumberOfRealScales][2]    = {
                                                                  { 640, 480 },    //  1
                                                                  { 320, 240 },    //  2
                                                                  { 160, 120 }     //  3
                                                                 };



/***************************************************************************/
// DetectorTask (apexcv_pro_aggcf)
/***************************************************************************/
const char gcFilenametraining[] = "detector_2048.txt";

const ACF_APU_CFG gcApexCvApuCfg        = ACF_APU_CFG__DEFAULT;
const int32_t gcApexCvApproxScaleApexId = 1;
const int32_t gcApexCvRealScaleApexId   = 0;
const int32_t gcApexCvLuvApexId         = 1;

const int32_t gcSceneTick               = 0;
const int32_t gcApexCvLUVTick           = 2;
const int32_t gcApexCv1stOctaveTick     = 4;
const int32_t gcApexCvNthOctaveTick     = 6;
const int32_t gcDetectorTick            = 8;
const int32_t gcDisplayTick             = 10;
const int32_t gcApexCvEndTick           = 12;



/***************************************************************************/
// DisplayrTask & ResizeTask
/***************************************************************************/
const  int32_t gcDisplayWidthInPixels     = 1920;
const  int32_t gcDisplayHeightInPixels    = 1080;
const uint32_t gcDisplayBitsPerPixel      = 24;
const uint32_t gcDisplayBufferSizeInBytes = gcDisplayWidthInPixels * gcDisplayHeightInPixels * gcDisplayBitsPerPixel / 8; // 8 Bits per byte

const bool gcDisplay_WeRotateDisplay = false;

const io::IO_DATA_DEPTH    gcDisplayIoDataDepth    = io::IO_DATA_DEPTH_08;
const io::IO_DATA_CHANNELS gcDisplayIoDataChannels = io::IO_DATA_CH3;






#endif /* GDCPEDESTRIANDETECTIONCONFIGURATION_HPP */
