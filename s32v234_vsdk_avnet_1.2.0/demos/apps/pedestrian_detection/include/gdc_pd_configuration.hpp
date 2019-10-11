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
#include "hog_config.h"
#include "acf_process_apu.h"
#include <stdint.h>


/***************************************************************************/
// Resource Files
/***************************************************************************/
const char gcIniFileName[] = "data/apps/pedestrian_detection/gdc_pd.ini";



/***************************************************************************/
// App
/***************************************************************************/
const bool gcWeUseFileReaderTask = true; // Should move this to configuration header

const bool gcIsRunningWithThreads = true;


const uint32_t gcInputImageQueueSize    = 3;
const uint32_t gcSceneWidthInPixels     = 640;
const uint32_t gcSceneHeightInPixels    = 480;
const uint32_t gcSceneBitsPerPixel      = 8;
const uint32_t gcScenePaddingInBytes    = 4;
const uint32_t gcScenePaddingInPixels   = (gcScenePaddingInBytes*8)/gcSceneBitsPerPixel;
const uint32_t gcSceneSpanInBytes       = (gcSceneWidthInPixels * gcSceneBitsPerPixel / 8) + 2*gcScenePaddingInBytes;
const uint32_t gcSceneSizeInBytes       = gcSceneWidthInPixels * gcSceneHeightInPixels * gcSceneBitsPerPixel / 8; // 8 bits per byte
const uint32_t gcSceneBufferSizeInBytes = gcSceneSpanInBytes * ((gcSceneHeightInPixels * gcSceneBitsPerPixel / 8) + 2*gcScenePaddingInBytes);
const uint32_t gcSceneOffsetInBytes     = (gcSceneSpanInBytes * gcScenePaddingInBytes) + gcScenePaddingInBytes;

const uint32_t gcSceneTask_ImageRequests = 5;



/***************************************************************************/
// FileReaderTask
/***************************************************************************/
const uint32_t gcFileNameSize = 64;
const char     gcFileName[]   = "out.bin";
const char     gcProfileFileName[] = "pd_profiling_result.txt";


/***************************************************************************/
// ImageSensorTask
/***************************************************************************/
const uint32_t gcImageSensorWidthInPixels     = 1280;
const uint32_t gcImageSensorHeightInPixels    = 720;
const uint32_t gcImageSensorBitsPerPixel      = 24;
const uint32_t gcImageSensorNumberOfBuffers   = 3;
const uint32_t gcImageSensorBufferSizeInBytes = gcImageSensorWidthInPixels * gcImageSensorHeightInPixels * gcImageSensorBitsPerPixel / 8; // 8 Bits per byte

const bool gcWeFlipImageSensorHorizontally = false;  // false for LCD, true for HDMI output
const bool gcWeFlipImageSensorVertically   = false;  // false for LCD, true for HDMI output

const uint32_t gcNumberOfDdrBuffers = 3;

const io::IO_DATA_CHANNELS gcImageSensorIoDataChannels = io::IO_DATA_CH3;



/***************************************************************************/
// ResizeTask
/***************************************************************************/

const uint32_t gcNumberOfMajorScales = 4;
const uint32_t gcNumberOfMinorScales = 5;
const uint32_t gcNumberOfSceneResizes = gcNumberOfMajorScales + gcNumberOfMinorScales;
const uint32_t gcMajorSceneResizes[gcNumberOfMajorScales][2] = {
                                                                  { 960, 720 },    //  1
                                                                  { 640, 480 },    //  2
                                                                  { 512, 384 },    //  3
                                                                  { 352, 264 }     //  4
                                                               };

const ACF_APU_CFG gcApexCvResizeAcfApuCfg = ACF_APU_CFG__DEFAULT;
const int32_t gcApexCvResizeApexId = 0;

/***************************************************************************/
// for profiling
/***************************************************************************/

const int32_t gcSceneTick               = 0;
const int32_t gcApexCvResizeTick        = 2;
const int32_t gcApexCvHogTick           = 4;
const int32_t gcCompTick                = 6;
const int32_t gcDisplayTick             = 8;
const int32_t gcApexCvEndTick           = 10;

/***************************************************************************/
// HogTask
/***************************************************************************/
const char gcFilenameSvm[] = "data/apps/pedestrian_detection/svm_function.bin";

const ACF_APU_CFG gcApexCvHogAcfApuCfg = ACF_APU_CFG__DEFAULT;
const int32_t gcApexCvHogApexId = 1;



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



/***************************************************************************/
// GdcPd_Helper::ConvertRgb24ToY
/***************************************************************************/
const uint32_t gcR2Y = 16;
const uint32_t gcG2Y = 32;
const uint32_t gcB2Y = 64;



/***************************************************************************/
/***************************************************************************/
const HogParam gcGdcPdHogParams = DefaultParams;
const int start_frame  = 248;

// Block matching parameters
const int max_diff = 17;
const int tsx = 16;
const int tsy = 16;
const int num_x = 8;
const int num_y = 1;

// HOG parameters
const int hog_threshold = 1000;
#endif /* GDCPEDESTRIANDETECTIONCONFIGURATION_HPP */ 
