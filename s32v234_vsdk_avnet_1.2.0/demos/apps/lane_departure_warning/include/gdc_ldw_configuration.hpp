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
*  @brief Configuration related values for LDW.
*
*  \section fcw_desc Description
*
*  This file contains configuration information related to the Forward Collision Warning
*  System.  It also specifies the name and location of the BSD configuration file.
*
***********************************************************************************/

#ifndef GDCLDWCONFIGURATION_HPP
#define GDCLDWCONFIGURATION_HPP

#include <icp_data.h>
#include <icp_datatype.h>
//#include <icp_image_format.h>
#include <umat.hpp>
#include <stdint.h>
#include <Kernels_Configuration.h>
#include <frame_io_types.h>


/***************************************************************************/
// Resource Files
/***************************************************************************/

const char gcIniFileName[]       = "data/apps/lane_departure_warning/gdc_ldw.ini";
const char gcFileNameCalibLoad[] = "data/apps/lane_departure_warning/gdc_ldw_calib_loader.txt";

const char     gcProfileFileName[] = "ldw_profiling_result.txt";


/***************************************************************************/
// App
/***************************************************************************/
const bool gcWeUseFileReaderTask = true;

const bool gcIsRunningWithThreads = true;


const  int32_t gcSceneWidthInPixels     = 720;
const  int32_t gcSceneHeightInPixels    = 480;
const uint32_t gcSceneBitsPerPixel      = 8;
const uint32_t gcScenePaddingInBytes    = 0;
const uint32_t gcScenePaddingInPixels   = (gcScenePaddingInBytes*8)/gcSceneBitsPerPixel;
const uint32_t gcSceneSpanInBytes       = (gcSceneWidthInPixels * gcSceneBitsPerPixel / 8) + 2*gcScenePaddingInBytes;
const uint32_t gcSceneSizeInBytes       = gcSceneWidthInPixels * gcSceneHeightInPixels * gcSceneBitsPerPixel / 8; // 8 bits per byte
const uint32_t gcSceneBufferSizeInBytes = gcSceneSpanInBytes * ((gcSceneHeightInPixels * gcSceneBitsPerPixel / 8) + 2*gcScenePaddingInBytes);
const uint32_t gcSceneOffsetInBytes     = (gcSceneSpanInBytes * gcScenePaddingInBytes) + gcScenePaddingInBytes;

const uint32_t gcSceneTask_ImageRequests = 4;

// ICP_COLOR_GRAYSCALE
const uint8_t gcSceneColorModel_PlaneCount = 1;
const icp::DATATYPE gcSceneColorModel_DataType = icp::DATATYPE_08U;

const uint32_t gcStringMaxSize = 128;



/***************************************************************************/
// FileReaderTask
/***************************************************************************/
const uint32_t gcFileNameSize = 64;
const char     gcFileName[]   = "data/apps/lane_departure_warning/720x480_GS8_Short.gs8";
//const char     gcFileName[]   = "417ouest1seqYUV420.y";



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
// DetectionTask
/***************************************************************************/
const int32_t gcLdwSceneDataType       = VSDK_CV_8UC1; // grayscale
const int32_t gcLdwLutOffsetDataType   = VSDK_CV_16UC1;
const int32_t gcLdwLutDeltaDataType    = VSDK_CV_8UC1; // @2
const int32_t gcLdwBlockOffsetDataType = VSDK_CV_32SC1; // Supposed to be equivalent to icp::DATATYPE_32U;
const int32_t gcLdwBirdEyeDataType     = VSDK_CV_8UC1;     // grayscale

//const icp::DataDescriptor gcLdwBirdEyeColorModel = ICP_COLOR_YCbCr422_CbYCrY;
const uint8_t gcLdwBirdEyeColorModel_PlaneCount = 1;
const icp::DATATYPE gcLdwBirdEyeColorModel_DataType = icp::DATATYPE_32U;



const float gcLdwKalmanMeasurementNoiseVar = 0.01;
const float gcLdwKalmanPostErrorVar        = 0.01;
const float gcLdwAngleKalmanInitialValue   = 180;

const uint32_t gcLdwConversionMatrixWidth  = 3;
const uint32_t gcLdwConversionMatrixHeight = 3;
const uint32_t gcLdwConversionMatrixDepth  = 1;

const uint32_t gcLdwPointMatrixWidth  = 3;
const uint32_t gcLdwPointMatrixHeight = 1;
const uint32_t gcLdwPointMatrixDepth  = 1;



/***************************************************************************/
// CompositionTask
/***************************************************************************/
const bool gcWeDrawLanes                 = true;
const bool gcWeDrawAlarmIcon             = true;
const bool gcWeDrawTopView               = false;
const uint32_t gcAlarmIconWidthInPixels  = 180;
const uint32_t gcAlarmIconHeightInPixels = 90;
const char gcAlarmIcon_NoLane[]          = "data/apps/lane_departure_warning/LDWS_Icon_20150609_180x90_no.raw";
const char gcAlarmIcon_InLane[]          = "data/apps/lane_departure_warning/LDWS_Icon_20150609_180x90_ok.raw";
const char gcAlarmIcon_TurningRight[]    = "data/apps/lane_departure_warning/LDWS_Icon_20150609_180x90_right.raw";
const char gcAlarmIcon_TurningLeft[]     = "data/apps/lane_departure_warning/LDWS_Icon_20150609_180x90_left.raw";



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

const uint32_t gcLdwIndicatorWidthInPixels   = 180;
const uint32_t gcLdwIndicatorHeightInPixels   = 90;

/***************************************************************************/
// GdcPd_Helper::ConvertRgb24ToY
/***************************************************************************/
const uint32_t gcR2Y = 16;
const uint32_t gcG2Y = 32;
const uint32_t gcB2Y = 64;

/***************************************************************************/
// for profiling
/***************************************************************************/

const int32_t gcSceneTick               = 0;
const int32_t gcFiltersTick             = 2;
const int32_t gcDetectionTick           = 4;
const int32_t gcCompTick                = 6;
const int32_t gcDisplayTick             = 8;
const int32_t gcApexCvEndTick           = 10;


#endif /* GDCLDWCONFIGURATION_HPP */ 
