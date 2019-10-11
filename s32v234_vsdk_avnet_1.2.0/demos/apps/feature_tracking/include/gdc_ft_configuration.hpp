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

#ifndef GDCFEATURETRACKINGCONFIGURATION_HPP
#define GDCFEATURETRACKINGCONFIGURATION_HPP

#include "frame_io_types.h"
#include "acf_process_apu.h"

#include <stdint.h>


/***************************************************************************/
// Resource Files
/***************************************************************************/
const char gcIniFileName[] = "data/apps/feature_tracking/gdc_ft.ini";



/***************************************************************************/
// App
/***************************************************************************/
const bool gcWeUseFileReaderTask = true;

const bool gcIsRunningWithThreads = true;


const uint32_t gcSceneTask_ImageRequests= 5; // One for each task

const  int32_t gcSceneWidthInPixels     = 1024;
const  int32_t gcSceneHeightInPixels    = 720;
const uint32_t gcSceneBitsPerPixel      = 8;
const uint32_t gcScenePaddingInBytes    = 16;
const uint32_t gcScenePaddingInPixels   = (gcScenePaddingInBytes*8)/gcSceneBitsPerPixel;
const uint32_t gcSceneSpanInBytes       = (gcSceneWidthInPixels * gcSceneBitsPerPixel / 8) + 2*gcScenePaddingInBytes;
const uint32_t gcSceneSizeInBytes       = gcSceneWidthInPixels * gcSceneHeightInPixels * gcSceneBitsPerPixel / 8; // 8 bits per byte
const uint32_t gcSceneBufferSizeInBytes = gcSceneSpanInBytes * ((gcSceneHeightInPixels * gcSceneBitsPerPixel / 8) + 2*gcScenePaddingInBytes);
const uint32_t gcSceneOffsetInBytes     = (gcSceneSpanInBytes * gcScenePaddingInBytes) + gcScenePaddingInBytes;



/***************************************************************************/
// FileReaderTask
/***************************************************************************/
const uint32_t gcFileNameSize = 64;
const char     gcFileName[]   = "out.bin";
const char     gcProfileFileName[] = "ft_profiling_result.txt";



/***************************************************************************/
// ImageSensorTask
/***************************************************************************/
const uint32_t gcImageSensorWidthInPixels     = 1280;
const uint32_t gcImageSensorHeightInPixels    = 720;
const uint32_t gcImageSensorBitsPerPixel      = 24;
const uint32_t gcImageSensorNumberOfBuffers   = 3;
const uint32_t gcImageSensorBufferSizeInBytes = gcImageSensorWidthInPixels * gcImageSensorHeightInPixels * gcImageSensorBitsPerPixel / 8; // 8 Bits per byte

const bool gcWeFlipImageSensorHorizontally = false;  // false for LCD, true for HDMI output. For LCD this works because both ImageSensor and LCD are mounted upside down on S32V234-EVB
const bool gcWeFlipImageSensorVertically   = false;  // false for LCD, true for HDMI output. For LCD this works because both ImageSensor and LCD are mounted upside down on S32V234-EVB

const io::IO_DATA_CHANNELS gcImageSensorIoDataChannels = io::IO_DATA_CH3;



/***************************************************************************/
// FeatureDetectionTask
/***************************************************************************/
const ACF_APU_CFG gcApexCvGfttCornersAcfApuCfg = ACF_APU_CFG__DEFAULT;
const int32_t     gcApexCvGfttCornersApexId    = 0;

const uint32_t gcTrackingToDetectionRatio = 1;

const int gcGfttCorners_UseHarris     = 1;
const int gcHarrisCornersQualityLevel = 0;
const int gcHarrisCornersMinDistance  = 0;
const int gcHarrisCornersCornerCoef   = 10;
const int gcHarrisCornersThreshold    = 32;
const int gcHarrisCornersBoxSize      = 7;
const int gcHarrisCornersNmsRadius    = 5;
const int gcHarrisCornersScaleFactor  = 5;
const int gcHarrisCornersMinCorners   = 100;
const int gcHarrisCornersMaxCorners   = 4096;


/***************************************************************************/
// ImagePyramidTask
/***************************************************************************/
const ACF_APU_CFG gcApexCvPyramidMultiCreationAcfApuCfg = ACF_APU_CFG__DEFAULT;
const int32_t     gcApexCvPyramidMultiCreationApexId    = 0;

const uint32_t gcDepthOfImagePyramid = 5;

const uint32_t gcImagePyramidDivisors[gcDepthOfImagePyramid] = {
                                                                 1,
                                                                 2,
                                                                 4,
                                                                 8,
                                                                 16
                                                               };


/***************************************************************************/
// FeatureTrackingTask
/***************************************************************************/
const ACF_APU_CFG gcApexCvLKTrackerOpticalFlowAcfApuCfg = ACF_APU_CFG__DEFAULT;
const int32_t     gcApexCvLKTrackerOpticalFlowApexId    = 1;

const uint32_t gcTrackedFeatureHistoryLength = 30;

const int gcLktBoxSize                            = 7;
const int gcLktNumberOfIterationPerPyramidLayer   = 10;
const int gcLktIsSourcePadded                     = (gcScenePaddingInBytes > 0) ? 1 : 0;
const int gcLktPadStartWidth                      = 30;

const int16_t gcDetectedFeatureStrength_Threshold = gcHarrisCornersThreshold;      // Anything below this value is discarded. (For tracking, lower values means that the found feature is closer to the original)
const int16_t gcTrackedFeatureStrength_Threshold  = 32*gcLktBoxSize*gcLktBoxSize; // Anything above this value is discarded. (For tracking, lower values means that the found feature is closer to the original)


const int32_t gcFilterGridCellWidthInPixels  = 8;
const int32_t gcFilterGridCellHeightInPixels = 8;
const int32_t gcFilterGridWidth              = (gcSceneWidthInPixels / gcFilterGridCellWidthInPixels);
const int32_t gcFilterGridHeight             = (gcSceneHeightInPixels / gcFilterGridCellHeightInPixels);
const int32_t gcMaxNumberOfTrackedFeatures   = gcFilterGridWidth * gcFilterGridHeight;



/***************************************************************************/
// CompositionTask
/***************************************************************************/
const bool gcComposition_WeDrawFullPaths = false;



/***************************************************************************/
// DisplayTask
/***************************************************************************/
const uint32_t gcDisplayWidthInPixels     = 1920;
const uint32_t gcDisplayHeightInPixels    = 1080;
const uint32_t gcDisplayBitsPerPixel      = 24;
const uint32_t gcDisplayBufferSizeInBytes = gcSceneWidthInPixels * gcSceneHeightInPixels * gcDisplayBitsPerPixel / 8; // 8 Bits per byte

const io::IO_DATA_DEPTH    gcDisplayIoDataDepth    = io::IO_DATA_DEPTH_08;
const io::IO_DATA_CHANNELS gcDisplayIoDataChannels = io::IO_DATA_CH3;

const bool gcDisplay_WeRotateDisplay = false;



/***************************************************************************/
// GdcFt_Helper::ConvertRgb24ToY
/***************************************************************************/
const uint32_t gcR2Y = 16;
const uint32_t gcG2Y = 32;
const uint32_t gcB2Y = 64;



/***************************************************************************/
// DEBUG
/***************************************************************************/
const uint32_t gcTimeDivisor = 1000; // In linux values are in nanoseconds

/***************************************************************************/
// for profiling
/***************************************************************************/

const int32_t gcSceneTick               = 0;  // [0] Entry, [1] Return
const int32_t gcDetectorTick            = 2;  // [2] Entry, [3] before apex, [4] after apex, [5] Return
const int32_t gcPyramidTick             = 6;  // [6] Entry, [7] before apex, [8] after apex, [9] Return
const int32_t gcTrackerTick             = 10; // [10] Entry, [11] before apex, [12] after apex, [13] Return
const int32_t gcCompTick                = 14; // [14] Entry, [15] Return
const int32_t gcDisplayTick             = 16; // [16] Entry, [17] Return
const int32_t gcApexCvEndTick           = 18; // end


#endif /* GDCFEATURETRACKINGCONFIGURATION_HPP */
