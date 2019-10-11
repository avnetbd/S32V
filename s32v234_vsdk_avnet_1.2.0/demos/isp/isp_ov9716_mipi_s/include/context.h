/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2017-2018 NXP Semiconductors;
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

#ifndef __APP_CONTEXT_H__
#define __APP_CONTEXT_H__
#include "typedefs.h"
#include "global_defines.h"
#include "sdi.hpp"                      // camera middle ware
#ifdef __STANDALONE__
#include "frame_output_dcu.h"           // display controller in SA
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"        // display controller for Linux
#endif // else from #ifdef __STANDALONE__
#include "isp_cam_ov9716.h"             // includes IIC read write functions
#include "exposure.h"                   // includes IIC read write functions
#if (EXT_AEWB == 1)
#include "external AEC/AWB header file .h"
#endif // EXT_AEWB


// **************************************************************************
// global variables
// **************************************************************************

// **************************************************************************
// constants
// **************************************************************************

// **************************************************************************
// types
// **************************************************************************
struct AppContext_t
{
  // ** variables for main image loop
  sdi_grabber *mpGrabber;       ///< pointer to grabber instance
  sdi_FdmaIO  *mpFdma;          ///< pointer to fdma object
  void *mpFrameCAM;  // current frame from the camera
  void *mpFrameIsp;  // current frame from the output of the ISP
  void *mpFrameFbs;  // current frame to display (after YUV420 -> RGB conversion)

  // DDR display output buffer pointers
  void*     mppGPUVirtual[DDR_OUT_BUFFER_CNT]; ///< virtual buffer pointers
  uint32_t  mpGPUPhysical[DDR_OUT_BUFFER_CNT]; ///< physical buffer addresses
  void*     mppApexVirtual[DDR_OUT_BUFFER_CNT]; ///< virtual buffer pointers
  uint32_t  mpApexPhysical[DDR_OUT_BUFFER_CNT]; ///< physical buffer addresses

  // registered propcess after ISP
  uint8_t mGpuActive;
  uint8_t mGpuBufferNr;
  uint8_t mApexActive;
  uint8_t mApexBufferNr;

#ifdef __STANDALONE__
  void *mpSaveISP;     // current frame from ISP
  //void *mpSaveISPHndl; // handle to current frame from ISP
#endif

  // *** frame profiling stats ***
  uint32_t mFrameIRQCnt=0;
  uint32_t mFrameIRQCntLast=0;
  uint32_t mFrameIRQDiff;
  uint32_t mFrmCnt=0;
  unsigned long mTimes;
  uint32_t mTimee,mTimeA53,mTimeCSC,mTimeApex;
  int32_t  mTimed;

  // ** variables for keyboard interaction
  uint8_t  mKbKey;
  uint8_t  mKbMode;      // 0=off, 'K'= knee point, r=redgain, b=bluegain
  uint16_t mKbMeasure;   // cycle counter for measurment
  uint8_t  mKbDisplay;   // update or freeze display
  uint16_t mKbCursorOn;
  uint16_t mKbCursorX;
  uint16_t mKbCursorY;
  uint16_t mKbCursorVal;

  uint8_t  mKbLog;       // 0=off
  uint32_t mKbI2c;       // IIC command 0xaaaavv  a=address v=value
  uint8_t  mKbTestbar;   // 1=on 0=off
  int16_t  mKbXoffset; // 0 - ((4x1280)-1920 = 3200)
  uint8_t  mKbEngine;
  uint16_t mKbGPR;
  uint32_t mKbParVal;
  uint8_t  mKbExpSel;    // for "K" command"
  // manual exposure control
  OV9716_ExposureType_t mKbExpToChange;
  gParam mKbExpParamChange;
  uint16_t mKbExpVal;
  int32_t  mKbExpTime;
  uint16_t mExpIncrement;// step size for manual exposure time setting
  uint16_t mDgIncrement; // step size for manual digital gain setting

//   //** variables for tone mapping
  uint16_t mOutLutOffset; // 0 or 2048
  uint8_t  mOutLut;  // 0/1 for dual, 2 for triple, 4 for equalized
  uint8_t  mpOutLut1[4]; // back up for modes
  uint8_t  mOldOutLut;   // selected outlut in previous frame to see if changed by keyboard

  float_t  mKbSaturation;// Color saturation in 0<= sat < 2
  int16_t  mRBIncrement;
  uint8_t  mKbCCM ;      // select CCM table
  uint8_t  mKbEdgeThrLow;// edge enhancement lower  threshold
  uint8_t  mKbEdgeThrHigh; // edge enhancement higher threshold
  uint8_t  mKbEdgeMaxA;  // edge enhancement strength in 2.6 fixed point
  uint8_t  mKbEdgeMaxA1; // shadow for edge enhancement strength
  uint8_t  mKbEdgeChannel;// Green Channel to be used for
                          // edge enhancement

  uint8_t  mKbAEC;       // off/S32V/Camera auto exposure
  uint8_t  mExpMode;
  uint8_t  mKbLtrgt;        // long exposure control target
  uint8_t  mKbVStrgt;       // very short exposure control target
  float_t  mCamFps;         // fps setting at camera
  float_t  mRatio;          // sensitivity ratio between HGC and LCG
  //
  uint8_t  mKbAWB;       // auto white balancing
  uint8_t  mInLut;       // 12 to 8
  uint8_t  mKbBlackLevel;
  uint8_t  mKbChgain;    // 1=on 0=off

  uint16_t mKbYDenoise;
  uint16_t mKbViewEdge;  // 0xffff: view denoise values; 0: view edge
  uint8_t  mKbDeFlicker; // deflicker mode: off, 50Hz, 60Hz
  float_t  mFlickerFrequency[4];
  uint8_t  mFlickerEnable[4];
  uint8_t  mKbYuvToRgb;  // 0=A53; 1=Neon
}; // struct AppContext_t;

// **************************************************************************
// local functions
// **************************************************************************

#endif // __APP_CONTEXTH__

//***************************************************************************
/* EOF */
