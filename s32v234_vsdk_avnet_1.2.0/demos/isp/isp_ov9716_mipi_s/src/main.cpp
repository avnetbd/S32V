/*
 * Copyright (c) 2016 Freescale Semiconductor
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __STANDALONE__
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#if(UDP264 == 1)
#include "udp264.hpp" // H.264 encoder via UDP
#endif                // if (UDP264 == 1)

// extern "C"
// {
//   #include "iniparser.h"
// }

#endif // #ifdef __STANDALONE__

#include <string.h>
#include <math.h>

#include "oal.h" // for malloc, cache and translating virtual to physical addressses
#include "vdb_log.h"
#include "isp_cam_maxim.h"  // includes IIC read write functions
#include "isp_cam_ov9716.h" // includes IIC read write functions
#include "sdi.hpp"          // camera middle ware

#ifdef __STANDALONE__
#include "frame_output_dcu.h"    // display controller in SA
#include "uartlinflex_driver.h"  // for keyboard input
#else                            // #ifdef __STANDALONE__
#include "frame_output_v234fb.h" // display controller for Linux
#endif                           // else from #ifdef __STANDALONE__

#include "isp_seq.h" // set ISP registers

// ISP functions
// graph related defines (FastDMA channels)
#if((OV9716_QUAD_GRAPH == MIPI) || (OV9716_QUAD_GRAPH == 0))
#include "ov9716_mipi_s_c.h"
#include <ov9716_graph_geometries.h>
#elif(OV9716_QUAD_GRAPH == 1)
#include "ov9716_quad1_c.h"
#include <ov9716_quad1_graph_geometries.h>
#else
#include "ov9716_quad_c.h"
#include <ov9716_quad_graph_geometries.h>
#endif

#include "global_defines.h"
#include "ov9716_defines.h" // camera register definition

// processing steps
#include "context.h"
#include "app_config.h"
#include "Ipu_coherent_reg.h"    // setRegisterCoherent
#include "mymemcpy.h"            // fast Neon memcopy routine
#include "exposure.h"            // functions for exposure control
#include "ctl_tonemap.h"         // tone_mapping with LUT
#include "yuv420torgb.h"         // convert yuv420 for display
#include "interactive_control.h" // keybopard control and logging

#include <common_time_measure.h>
#include <common_helpers.h>
#include <string>

#define CSI_ERROR_CNT

//***************************************************************************
// constants
//***************************************************************************
#define CAM_SAVE_PGM

// ***** ISP Graph Defines *******

// Possible to set input resolution (must be supported by the DCU)
#define SCR_WIDTH ((DISPLAY_PIXELS + 31) & ~31) ///< width of DDR buffer in pixels
#define SCR_HEIGHT (DISPLAY_LINES)              ///< height of DDR buffer in pixels

// for file saving
#define EXTRACT_YUV2RGB
#define DCU_OUT_FILE_EXT "ppm"
#define ISP_CHNL_CNT io::IO_DATA_CH1
#define DCU_TYPE DCU_BPP_82
#define DCU_CHNL_CNT io::IO_DATA_CH3
//  #define DCU_CHNL_CNT io::IO_DATA_CH1

#define OUTLUT_BITS 11                 // bits for 2048 lutsize
#define OUTLUT_SIZE (1 << OUTLUT_BITS) // 2048
#define OUTLUT_SCALE 256.0

// ***** APEX Processing Defines *******
#define EDGETHRDIFF 16

// ***** Run Control and Logging Defines *******

#define IPU_DUMP_STATE
#define DO_IPU_PROF
#define DO_CHGAIN_LOG
#define DO_BUFFER_LOG

#ifdef ISP_FRAME_RATE_MEASURE_NUM
#define LOOP_NUM ISP_FRAME_RATE_MEASURE_NUM
#else
#define LOOP_NUM 30
#endif

#ifdef DO_IPU_PROF
#define IPU_PROF_FRAMES ((LOOP_NUM < 10) ? LOOP_NUM : 10)
#endif

//***************************************************************************
// global variables
//***************************************************************************

// ** variables for main image loop
#if(CAM_SAVE == 1)
vsdk::Mat gFrameCAM; // current frame from CAM
#endif               // if (CAM_SAVE==1)
vsdk::Mat gFrameISP; // current frame from ISP

//Flickering LED control software global variables
double  gFrequency  = 50.0;
uint8_t gPwmPercent = 5;

// ** variables for exposure and channel gain control
SEQ_IpuHist_t gIpuHist[2]; // for even or odd frames
SEQ_IpuHist_t gCompHist;   //histogram to compute compression

SEQ_IpuStat_t gIpuStat; // statistic data

// Global pointer for access to AppData from other threads.
static AppContext_t* gpAppContext;

//***************************************************************************
// macro
//***************************************************************************

#if defined(__STANDALONE__) && defined(__ARMV8)
extern "C" {
void flush_dcache_range(void*, unsigned int);
}
#endif

//***************************************************************************
// local functions
//***************************************************************************

/************************************************************************/
/** Compute combined matrix from color correction matrix,
 * color space conversion matrix (RGB-> YUV) and color saturation
 * \param  apCcmCsCMatrix     resulting combine 3x3 matrix (as 9x1)
 * \param  apCcmMatrix        input CCM 3x3 matrix (as 9x1)
 * \param  apCcmCsCMatrix     input CSC 3x3 matrix (as 9x1)
 * \param  aSaturation        input saturation factor
 *
 * \return void
 ************************************************************************/
void CcmCscMatrix(float_t* apCcmCsCMatrix, float_t* apCcmMatrix, float_t* apCscMatrix, float_t aSaturation);

/***********************************************************************/
/** extract one 1280 pixles for being dispalyed
*
***********************************************************************/
void ExtractDisplay(void* appInVirtual, AppContext_t& arContext);

/************************************************************************/
/** Configure ISP preprocessing.
 *
 ************************************************************************/
void IspConfig(AppContext_t& arContext);

/************************************************************************/
/** Select Look Up Table for input
 *
 * \param  aLut          type of Look Up Tabel
 *                       0 = Linear
 *                       1 = 12 To 16 bit
 * \return               LIB_SUCCESS if LUT was updated
 *
 ************************************************************************/
uint8_t SetInLut(AppContext_t& arContext);

/************************************************************************/
/** Select Look Up Table for output
 *
 * \param  aLut          type of Look Up Tabel
 *                       0 = Linear
 *                       1 = 12 To 8 bit
 *                       2 = 16 To 8 bit
 * \param  aLutOffset    lower table: 0 or upper table: 2048
 * \return               LIB_SUCCESS if LUT was updated
 *
 ************************************************************************/
uint8_t SetOutLut(uint8_t aLut, uint16_t aLutOffset);

/************************************************************************/
/** Allocates contiguous DDR buffers for one ISP stream.
 *
 * \param  appVirtual    array of virtual buffer pointers to be filled
 * \param  apPhysical    array of buffer physical addresses to be filled
 * \param  aMemSize      size of the array in bytes
 * \param  aBufferCnt    number of buffers to be allocated
 *
 * \return 0 if all OK
 *         < 0 otherwise
 ************************************************************************/
int32_t DdrBuffersAlloc(void** appVirtual, uint32_t* apPhysical, size_t aMemSize, uint32_t aBufferCnt);

/************************************************************************/
/** Frees DDR buffers for one ISP stream.
 *
 * \param  appVirtual    array of virtual buffer pointers to be filled
 * \param  apPhysical    array of buffer physical addresses to be filled
 * \param  aBufferCnt    number of buffers to be freed
 ************************************************************************/
void DdrBuffersFree(void** appVirtual, uint32_t* apPhysical, uint32_t aBufferCnt);

#ifndef __STANDALONE__

/************************************************************************/
/** SIGINT handler.
 *
 * \param  aSigNo
 ************************************************************************/
void SigintHandler(int aSigNo);

/************************************************************************/
/** SIGINT handler.
 *
 * \param  aSigNo
 *
 * \return SEQ_LIB_SUCCESS if all ok
 *         SEQ_LIB_FAILURE if failed
 ************************************************************************/
int32_t SigintSetup(void);

#endif // #ifndef __STANDALONE__

/************************************************************************/
/** Handler for ISP events.
 *
 * \param  aEventType type of the event received from ISP
 ************************************************************************/
void SeqEventHandler(uint32_t aEventType);

//***************************************************************************

static bool sStop = false; ///< to signal Ctrl+c from command line

//***************************************************************************

static void ContextInit(AppContext_t& arContext);

/************************************************************************/
/** Csi Error Detection
 *
 * Check global variables set in the csi_func.c interrupt handler when CSI error occurs
 *
 ************************************************************************/
void CsiErrorDetection(AppContext_t& arContext);

//***************************************************************************

static void ContextInit(AppContext_t& arContext)
{
  arContext.mpGrabber = NULL;
  arContext.mpFdma    = NULL;
#ifdef __STANDALONE__
  arContext.mpSaveISP = NULL; // current frame from ISP
#endif

  // registered propcess after ISP
  arContext.mGpuActive   = 0;
  arContext.mGpuBufferNr = 0;

  // frame profiling stats
  arContext.mFrameIRQCnt     = 0;
  arContext.mFrameIRQCntLast = 0;
  arContext.mFrameIRQDiff    = 0;
  arContext.mFrmCnt          = 0;
  arContext.mTimes           = 0;
  arContext.mTimee = arContext.mTimeA53 = arContext.mTimeCSC = arContext.mTimeApex = 0;
  arContext.mTimed                                                                 = 0;

  // keyboard interaction
  arContext.mKbLog      = 0; // 0=off, ...
  arContext.mKbKey      = 0;
  arContext.mKbMode     = 0; // 0=off, ...
  arContext.mKbMeasure  = 0; // cycle counter for measurment
  arContext.mKbDisplay  = 1; // update or freeze display
  arContext.mKbCursorOn = 0;
  arContext.mKbCursorX  = 640;
  arContext.mKbCursorY  = 360;
  arContext.mKbXoffset  = 0;

  arContext.mKbCCM         = 0;  // select CCM table
  arContext.mKbEdgeThrLow  = 32; // edge enhancement lower  threshold
  arContext.mKbEdgeThrHigh =     // edge enhancement higher threshold
      arContext.mKbEdgeThrLow + EDGETHRDIFF;
  arContext.mKbEdgeMaxA    = 64; // edge enhancement strength in 2.6 fixed point
  arContext.mKbEdgeChannel = 1;  // Green Channel to be used for
                                 // edge enhancement
  arContext.mKbAEC   = 1;        // off/S32V/Camera auto exposure
  arContext.mExpMode = 0;
  arContext.mKbAWB   = 1; // auto white balancing

  arContext.mKbEngine     = 0;
  arContext.mKbGPR        = 15;
  arContext.mKbParVal     = 0;
  arContext.mKbExpSel     = 0; // all exposures on
  arContext.mExpIncrement = 8; // step size for manual exposure time setting
  arContext.mDgIncrement  = 8; // step size for manual digital gain setting

  arContext.mKbSaturation = 1.0; // Color saturation in 0<= sat < 2
  arContext.mRBIncrement  = 16;
  // arContext.mKbNoiseThr  = 1;  // Noise threshold 0=all, 0xffff=off
  arContext.mKbCCM = 0; // select CCM table
  arContext.mInLut = 1; // 12 to 8

  arContext.mOutLutOffset = 0; // 0 or 2048
  arContext.mOutLut       = 4; // 0/1 for dual, 2 for triple, 4 for equalized
  arContext.mpOutLut1[0]  = 2;
  arContext.mpOutLut1[1]  = 2;
  arContext.mpOutLut1[2]  = 1;
  arContext.mpOutLut1[3]  = 2;
  arContext.mOldOutLut    = -1; // 12 to 8
  arContext.mKbBlackLevel = 0;
  arContext.mKbChgain     = 0;    // 1=on 0=off
  arContext.mKbLtrgt      = 176;  // long exposure control target
  arContext.mKbVStrgt     = 240;  // Very short exposure control target
  arContext.mCamFps       = 30.0; // default 30 frames per second
  arContext.mRatio        = 16.0; // sensitivity ration between HCG and LCG

  arContext.mKbYDenoise          = 1024;   // Y channel denoise threshold level
  arContext.mKbViewEdge          = 0xffff; // 0xffff: view denoise values; 0: view edge
  arContext.mKbDeFlicker         = 0;      // deflicker mode: off, 50Hz, 60Hz
  arContext.mFlickerFrequency[0] = 100.0;
  arContext.mFlickerFrequency[1] = 100.0;
  arContext.mFlickerFrequency[2] = 120.0;
  arContext.mFlickerFrequency[3] = 90.0;
  arContext.mFlickerEnable[0]    = 0;
  arContext.mFlickerEnable[1]    = 1;
  arContext.mFlickerEnable[2]    = 1;
  arContext.mFlickerEnable[3]    = 1;
  arContext.mKbYuvToRgb          = 1; // 0=A53; 1=Neon
  arContext.mKbExpToChange       = OV9716_ExpType_VS;
  arContext.mKbExpParamChange    = Time;
  arContext.mKbExpVal            = 0;
  arContext.mKbExpTime           = 0;

} // ContextInit()

//***************************************************************************

void CsiErrorDetection(AppContext_t& arContext)
{
  static uint8_t  lCsiErrorCount       = 0;
  static uint8_t  lCsiErrorPrintOnce   = 1;
  static uint8_t  lCsiErrorPrintedOnce = 0;
  CSI_ErrStatus_t lErrStatus;
  bool            lError = false;

  // only first 10 errors evauluated
  if(lCsiErrorCount >= 10)
  {
    return;
  }

  if(CSI_ErrStatusGet(lErrStatus, CSI_IDX_CAM) != CSI_LIB_SUCCESS)
  {
    printf("Failed to get current CSI error status. %d\n", lCsiErrorCount);
    lCsiErrorCount++;
    return;
  }

  if(lErrStatus.mLineErr & 0x4444) // filter line length errors
  {
    lError = true;
    if((lCsiErrorPrintOnce != 0) || (lCsiErrorPrintedOnce == 0))
    {
      printf("Error: ***** CSI Line Length Error in frame %d,\n", arContext.mFrmCnt);
      for(uint32_t i = 0; i < CSI_CHNL_CNT; i++)
      {
        printf("VC %u: line %u has wrong length %u pixels\n", i, lErrStatus.mErrLength[i], lErrStatus.mErrLine[i]);
      }
      lCsiErrorPrintedOnce = 1;
    }
  }
  if(lErrStatus.mLineErr & 0x8888)
  {
    lError = true;
    printf("Error: ***** CSI Line Count Error in frame %d\n", arContext.mFrmCnt);
    for(uint32_t i = 0; i < CSI_CHNL_CNT; i++)
    {
      printf("VC %u: error line count %u \n", i, lErrStatus.mErrLineCnt[i]);
    }
  }
  if(lError)
  {
    if(CSI_ErrClear(CSI_IDX_CAM) != LIB_SUCCESS)
    {
      printf("Failed to clear CSI errors.\n");
    }
    lCsiErrorCount++;
  }
} // CsiErrorDetection()

//***************************************************************************

void CcmCscMatrix(float_t* apCcmCscMatrix, float_t* apCcmMatrix, float_t* apCscMatrix, float_t aSaturation)
{
  int i, j, k;

  if((!apCcmCscMatrix) || (!apCcmMatrix) || (!apCscMatrix))
  {
    printf(
        "Error during Matrix combination: one matrix has no array given\n"
        "result: %p, CCM: %p, CSC: %p\n",
        (void*)apCcmCscMatrix, (void*)apCcmMatrix, (void*)apCscMatrix);
    return;
  }

  // for all output matrix elements
  for(i = 0; i < 3; i++)
  { // line
    for(j = 0; j < 3; j++)
    { // column

      // compute matrix element
      *apCcmCscMatrix = 0;
      for(k = 0; k < 3; k++)
      {
        *apCcmCscMatrix += apCcmMatrix[k * 3 + j] * apCscMatrix[i * 3 + k];
      }
      if(i)
        *apCcmCscMatrix *= aSaturation; // only for U and V
      apCcmCscMatrix++;
    }
  }
  return;
} // CcmCscMatrix()

//***************************************************************************

void ExtractDisplay(void* appInVirtual, AppContext_t& arContext)
{
  uint32_t lStartTime, lEndTime;
  lStartTime = lEndTime = 0;
  if(arContext.mKbLog == 2)
  {
    GETTIME(&lStartTime);
  }

  // copy buffer to display
  {
    uint8_t* lpSrc  = (uint8_t*)appInVirtual;
    uint8_t* lpDest = (uint8_t*)arContext.mppGPUVirtual[arContext.mGpuBufferNr];
    //uint32_t y;

    OAL_MemoryInvalidate(appInVirtual, OUT_PIXELS * 2 * OUT_LINES * ISP_CHNL_CNT);

    lpSrc += arContext.mKbXoffset * ISP_CHNL_CNT;

#if(UDP264 == 1)
    H264Encoder::getInstance()->EncoderFrame((unsigned char*)gH264frame.data, lpSrc,
                                             lpSrc + OUT_PIXELS * ISP_CHNL_CNT * OUT_LINES, OUT_PIXELS,
                                             false); // +OUT_STRIDE16_Y*OUT_LINES
#endif                                               // if (UDP264 == 1)

#ifdef EXTRACT_MEMCOPY
#if(ISP_CHNL_CNT != DCU_CHNL_CNT)
#error Channel types do not match!
#endif

    for(y = 0; y < OUT_LINES; y++)
    {
      mymemcpy(lpDest, lpSrc, DISPLAY_PIXELS * DCU_CHNL_CNT);
      lpSrc += OUT_PIXELS * 2 * ISP_CHNL_CNT;
      lpDest += DISPLAY_PIXELS * DCU_CHNL_CNT;
    } // for all lines
#endif
#ifdef EXTRACT_YUV2RGB

    // line start pointers
    uint8_t* lpSrcUV;
    // pixel pointers
    // position counters
    uint32_t lYPos;

    lpSrcUV = lpSrc + OUT_PIXELS * ISP_CHNL_CNT * OUT_LINES;

    for(lYPos = 0; lYPos < OUT_LINES; lYPos++)
    {
      if(arContext.mKbYuvToRgb)
      {
        Neon_Yuv420ToRgb(lpDest, lpSrc, lpSrcUV, DISPLAY_PIXELS);
      }
      else
      {
        A53_Yuv420ToRgb(lpDest, lpSrc, lpSrcUV, DISPLAY_PIXELS);
      }

      lpSrc += OUT_PIXELS * ISP_CHNL_CNT;
      // increment every second line by two lines
      if(lYPos & 0x1)
      {
        lpSrcUV += OUT_PIXELS * ISP_CHNL_CNT;
      }
      lpDest += DISPLAY_PIXELS * DCU_CHNL_CNT;
    } // for all lines

#endif
  } // copy buffer to display

  OAL_MemoryFlush(arContext.mppGPUVirtual[arContext.mGpuBufferNr], DISPLAY_PIXELS * DCU_CHNL_CNT * DISPLAY_LINES);

  if(arContext.mKbLog == 2)
  {
    GETTIME(&lEndTime);
    arContext.mTimeCSC += lEndTime - lStartTime;
  }
} // ExtractDisplay()

//***************************************************************************

void IspConfig(AppContext_t& arContext)
{
  static uint8_t lsReInit = 0;
  uint8_t        lFaild   = 0;
  uint8_t        lAEC     = arContext.mKbAEC;

  printf("Initialize processing parameters\n");
#if(OV9716_QUAD_GRAPH == 1) // turn off synchronization
  if(!lsReInit)
  {
    // disable fsin...
    MAXIM_DES_RegWrite(CSI_IDX_CAM, 0x00, 0x01);
    MAXIM_DES_RegWrite(CSI_IDX_CAM, 0x01, 0xc0);
    CAM_RegWrite(CSI_IDX_CAM, 0x308c, 0xef);
    CAM_RegWrite(CSI_IDX_CAM, 0x30bd, 0x03);
  }
#endif
  // *** set maximum exposure range

  {
    uint8_t  val8;
    uint16_t lLS;
    uint32_t lVS;

    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_DCG_H, val8, MAXIM_CHNL_ORIG_BC);
    lLS = val8 << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_DCG_L, val8, MAXIM_CHNL_ORIG_BC);
    lLS += val8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_VS_H, val8, MAXIM_CHNL_ORIG_BC);
    lVS = val8 << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_VS_M, val8, MAXIM_CHNL_ORIG_BC);
    lVS += val8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_VS_L, val8, MAXIM_CHNL_ORIG_BC);

    printf("initial exposure times LS %d VS %d.(%d/32)\n", lLS, lVS, val8);
  }

    // recommended AEC initialization sequence
    // 1. set exposure range
    // 2. set flicker frequency
    // 3. enable deflicker
#define READVTS
#ifdef READVTS
  {
    uint8_t  val8;
    uint16_t val16;
    msleep(5);
    CAM_RegRead(CSI_IDX_CAM, OV9716_VTS, val8);
    val16 = val8 << 8;
    CAM_RegRead(CSI_IDX_CAM, OV9716_VTS + 1, val8);
    val16 += val8;
    SetExpRange(val16 - 2 - 3);
  }
#else
  SetExpRange(720);
#endif

  // fix exposure time if needed
  ExposureInit();
  SetDeflickerHz(arContext.mFlickerFrequency[arContext.mKbDeFlicker],
                 arContext.mCamFps); //needs to be executed after ExposureInit
  EnableDeflicker(arContext.mFlickerEnable[arContext.mKbDeFlicker]);

  if(!lsReInit)
  {
    MyOV9716_ExposureSet(CSI_IDX_CAM, MAXIM_CHNL_ORIG_BC, 1.001, 1.001);
    msleep(2);

    // *** min digital gain in camera
    CAM_RegWrite(CSI_IDX_CAM, OV9716_DIG_GAIN_HCG_H, (OV9716_DIGITAL_GAIN_L_MIN >> 8) & 0xff);
    CAM_RegWrite(CSI_IDX_CAM, OV9716_DIG_GAIN_HCG_L, OV9716_DIGITAL_GAIN_L_MIN & 0xff);
    msleep(2);
    CAM_RegWrite(CSI_IDX_CAM, OV9716_DIG_GAIN_LCG_H, (OV9716_DIGITAL_GAIN_S_MIN >> 8) & 0xff);
    CAM_RegWrite(CSI_IDX_CAM, OV9716_DIG_GAIN_LCG_L, OV9716_DIGITAL_GAIN_S_MIN & 0xff);
    CAM_RegWrite(CSI_IDX_CAM, OV9716_DIG_GAIN_VS_H, (OV9716_DIGITAL_GAIN_VS_MIN >> 8) & 0xff);
    msleep(2);
    CAM_RegWrite(CSI_IDX_CAM, OV9716_DIG_GAIN_VS_L, OV9716_DIGITAL_GAIN_VS_MIN & 0xff);
    //msleep(5);
  }

  // *** configure Channel Gain in Camera
  {
    uint8_t lAWB;
    lAWB             = arContext.mKbAWB;
    arContext.mKbKey = 'G';
    KbAction(arContext);
    arContext.mKbAWB = lAWB;
  }

  // Input LUT Engine (linear or decompression)
  arContext.mInLut = 1;
  if(LIB_SUCCESS == SetInLut(arContext)) // decompress always. enable disable is now selected via GPR15
  {
    seq_setReg(INLUT_ENGINE, 0, IPUS_GPR0 + 15, arContext.mInLut);
  }
  else
  {
    lFaild = 1;
  }

  // Histogram Engine

  // *** tell exposure control which engine is doing

  // Histogram Engine
  seq_setReg(EXPOSURE_ENGINE, 0, IPUS_GPR0 + 0, HIST_PROC >> 1);
  seq_setReg(EXPOSURE_ENGINE, 0, IPUS_GPR0 + 1, HIST_LINEGAP);
  // *** enable IPU histogram readout ***
  memset(&gIpuHist[0], 0, sizeof(SEQ_IpuHist_t));
  memset(&gIpuHist[1], 0, sizeof(SEQ_IpuHist_t));
  memset(&gCompHist, 0, sizeof(SEQ_IpuHist_t));
  gIpuHist[0].mIpuIdx = SEQ_EXP_HIST;
  gIpuHist[1].mIpuIdx = SEQ_EXP_HIST;
  SEQ_HistogramEnable(SEQ_EXP_HIST);
  gCompHist.mIpuIdx = SEQ_LUT_HIST;
  SEQ_HistogramEnable(SEQ_LUT_HIST);
  SEQ_StatEnable();

  // *** HDR configuration
  SetHDREngine(HDR_IPUSIDX); // tell exposure control which engine to change

  // do last two pixels manually
#if(OV9716_QUAD_GRAPH == 2) || (OV9716_QUAD_GRAPH == MIPI)
  seq_setReg(HDR_ENGINE, 0, HDR_GPR0 + 0, IN_PIXELS1 - 2); // (CAM_PIXEL1 -2)
#else
  seq_setReg(HDR_ENGINE, 0, HDR_GPR0 + 0, (IN_PIXELS1 >> 1) - 2); //(CAM_PIXEL1 -2)
#endif

  // initial combination values
  arContext.mKbMode = 'M';
  arContext.mKbKey  = '0' + arContext.mExpMode;
  KbAction(arContext);
  // GPR14 = VSEenable
  seq_setReg(HDR_ENGINE, 0, HDR_GPR0 + 14, 0x0);
  // GPR15 = L/S Shift
  seq_setReg(HDR_ENGINE, 0, HDR_GPR0 + 15, gHDRShift[arContext.mExpMode]);
#if defined(HDR_BLEND)
  seq_setReg(HDR_ENGINE, 0, HDR_GPR0 + 16, 0x1000);
  // low threshold (16bit)
#define HDRBLENDLOWTHR 4096
#define HDRBLENDHIGHTHR 0xf000

  seq_setReg(HDR_ENGINE, 0, HDR_GPR0 + 17, HDRBLENDLOWTHR);
  seq_setReg(HDR_ENGINE, 0, HDR_GPR0 + 18, 0xf000);
  seq_setReg(HDR_ENGINE, 0, HDR_GPR0 + 19, (uint16_t)(65535.0 / (float)(HDRBLENDHIGHTHR - HDRBLENDLOWTHR) * 4096.0));
#endif

  // *** debayer select start line
  // start with even line
  seq_setReg(DEBAYER_ENGINE0, 0, IPUS_GPR0 + 0, DEBAYER_OUTPIX >> 2); // line length
  seq_setReg(DEBAYER_ENGINE1, 0, IPUS_GPR0 + 0, DEBAYER_OUTPIX >> 2); // line length

  // *** RGB2Y Engine
  CcmCscMatrix(gCcmCscMatrix, gKbCcmMatrix[arContext.mKbCCM], gRgb2YuvMatrix, arContext.mKbSaturation);
  // color space conversion factors
  // gpr0: R->Y, gpr1: G->Y, gpr2: B->Y
  seq_setReg(RGB2Y_ENGINE0, 0, 0x54, // GPR0 (IPUV)
             (int16_t)(32768.0 * gCcmCscMatrix[0]));
  seq_setReg(RGB2Y_ENGINE0, 0, 0x55, // GPR1 (IPUV)
             (int16_t)(32768.0 * gCcmCscMatrix[1]));
  seq_setReg(RGB2Y_ENGINE0, 0, 0x56, // GPR2 (IPUV)
             (int16_t)(32768.0 * gCcmCscMatrix[2]));

  seq_setReg(RGB2Y_ENGINE1, 0, 0x54, // GPR0 (IPUV)
             (int16_t)(32768.0 * gCcmCscMatrix[0]));
  seq_setReg(RGB2Y_ENGINE1, 0, 0x55, // GPR1 (IPUV)
             (int16_t)(32768.0 * gCcmCscMatrix[1]));
  seq_setReg(RGB2Y_ENGINE1, 0, 0x56, // GPR2 (IPUV)
             (int16_t)(32768.0 * gCcmCscMatrix[2]));

  // *** TONEMAP Engine
  // gpr0: R->U, gpr1: G->U, gpr2: B->U
  // gpr3: R->V, gpr4: G->V, gpr5: B->V
  seq_setReg(TONEMAP_ENGINE0, 0, 0x50, // GPR0 (IPUV)
             (int16_t)(4096.0 * gCcmCscMatrix[3]));
  seq_setReg(TONEMAP_ENGINE0, 0, 0x51, // GPR1 (IPUV)
             (int16_t)(4096.0 * gCcmCscMatrix[4]));
  seq_setReg(TONEMAP_ENGINE0, 0, 0x52, // GPR2 (IPUV)
             (int16_t)(4096.0 * gCcmCscMatrix[5]));
  seq_setReg(TONEMAP_ENGINE0, 0, 0x53, // GPR3 (IPUV)
             (int16_t)(4096.0 * gCcmCscMatrix[6]));
  seq_setReg(TONEMAP_ENGINE0, 0, 0x54, // GPR4 (IPUV)
             (int16_t)(4096.0 * gCcmCscMatrix[7]));
  seq_setReg(TONEMAP_ENGINE0, 0, 0x55, // GPR5 (IPUV)
             (int16_t)(4096.0 * gCcmCscMatrix[8]));

  seq_setReg(TONEMAP_ENGINE1, 0, 0x50, // GPR0 (IPUV)
             (int16_t)(4096.0 * gCcmCscMatrix[3]));
  seq_setReg(TONEMAP_ENGINE1, 0, 0x51, // GPR1 (IPUV)
             (int16_t)(4096.0 * gCcmCscMatrix[4]));
  seq_setReg(TONEMAP_ENGINE1, 0, 0x52, // GPR2 (IPUV)
             (int16_t)(4096.0 * gCcmCscMatrix[5]));
  seq_setReg(TONEMAP_ENGINE1, 0, 0x53, // GPR3 (IPUV)
             (int16_t)(4096.0 * gCcmCscMatrix[6]));
  seq_setReg(TONEMAP_ENGINE1, 0, 0x54, // GPR4 (IPUV)
             (int16_t)(4096.0 * gCcmCscMatrix[7]));
  seq_setReg(TONEMAP_ENGINE1, 0, 0x55, // GPR5 (IPUV)
             (int16_t)(4096.0 * gCcmCscMatrix[8]));

  // *** DENOISEY Engine
  // Threshold value
  seq_setReg(DENOISEY_ENGINE0, 0, DENOISEY_GPR + 7, arContext.mKbYDenoise);  // threshold
  seq_setReg(DENOISEY_ENGINE0, 0, DENOISEY_GPR + 12, arContext.mKbViewEdge); // selector
  seq_setReg(DENOISEY_ENGINE1, 0, DENOISEY_GPR + 7, arContext.mKbYDenoise);  // threshold
  seq_setReg(DENOISEY_ENGINE1, 0, DENOISEY_GPR + 12, arContext.mKbViewEdge); // selector

  // *** rgb2y_lut kernel ***
  seq_setReg(OUTLUT_ENGINE0, 0, IPUS_GPR0 + 14, 16 - OUTLUT_BITS);        // shift bits
  seq_setReg(OUTLUT_ENGINE0, 0, IPUS_GPR0 + 15, arContext.mOutLutOffset); // 0 or 2048
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
  seq_setReg(OUTLUT_ENGINE1, 0, IPUS_GPR0 + 14, 16 - OUTLUT_BITS);        // shift bits
  seq_setReg(OUTLUT_ENGINE1, 0, IPUS_GPR0 + 15, arContext.mOutLutOffset); // 0 or 2048
#endif
  if(SetOutLut(arContext.mOldOutLut, arContext.mOutLutOffset) == LIB_SUCCESS)
  {
    arContext.mOutLutOffset ^= 2048;
  }

  // restore AEC mode;
  arContext.mKbAEC = lAEC;

  lsReInit = 1;

  if(lFaild == 0)
  {
    printf("Initialize processing parameters...done");
  }
  else
  {
    printf("Initialize processing parameters...failed");
  }
  return;
} // IspConfig()

//***************************************************************************

uint8_t SetInLut(AppContext_t& arContext)
{
  uint16_t lLutCnt = 0;
  uint16_t lpLutData[4096], *lpLutDataWrite = 0;
  uint8_t  lError = LIB_FAILURE;

  if(arContext.mInLut == 0)
  {
    while(lLutCnt < 4096)
    {
      lpLutData[lLutCnt] = (uint16_t)((lLutCnt) << 4);
      lLutCnt++;
    } // while lutcnt < OUTLUT_SIZE
    printf("Input  Lut: copy\n");
    lpLutDataWrite = lpLutData;
    lError         = LIB_SUCCESS;
  }

  else if(arContext.mInLut == 1)
  {
    while(lLutCnt < 512)
    {
      lpLutData[lLutCnt] = ((lLutCnt) << 1);
      lLutCnt++;
    } // while lutcnt < 512
    while(lLutCnt < 768)
    {
      lpLutData[lLutCnt] = ((lLutCnt - 256) << 2);
      lLutCnt++;
    } // while lutcnt < 768
    while(lLutCnt < 2560)
    {
      lpLutData[lLutCnt] = ((lLutCnt - 512) << 3);
      lLutCnt++;
    } // while lutcnt < 2560
    while(lLutCnt < 4096)
    {
      lpLutData[lLutCnt] = ((lLutCnt - 2048) << 5);
      lLutCnt++;
    } // while lutcnt < 4096
    printf("Input  Lut: decompress 12 to 16 linear\n");
    lpLutDataWrite = lpLutData;
    lError         = LIB_SUCCESS;
  }
  if(lError == LIB_SUCCESS)
  {
    if(ISP_LutSet(INLUT_IPUS, 0, lpLutDataWrite, 4096) != SEQ_LIB_SUCCESS)
    {
      lError = LIB_FAILURE;
      printf("Failed to update IPUS %d LUT\n", INLUT_IPUS);
    }
  }
  return lError;
} // SetInLut()

//***************************************************************************

uint8_t SetOutLut(uint8_t aLut, uint16_t aLutOffset)
{
  uint32_t val0;
  uint16_t lLutCnt = 0;
  uint16_t lpLutData[OUTLUT_SIZE], *lpLutDataWrite = 0;
  uint8_t  lError = LIB_FAILURE;

  if(aLut == 0)
  {
    while(lLutCnt < OUTLUT_SIZE)
    {
      lpLutData[lLutCnt++] = (uint16_t(OUTLUT_SCALE));
    } // while lLutCnt < OUTLUT_SIZE
    printf("Output Lut: copy\n");
    lpLutDataWrite = lpLutData;
    lError         = LIB_SUCCESS;
  }
  else if(aLut == 1)
  {
#define GAMMA 0.8
#define GAMMA_SCALE (0xFFF0)
    while(lLutCnt < OUTLUT_SIZE)
    {
      if(!lLutCnt)
        val0 = 0;
      else
      {
        val0 = (uint32_t)(OUTLUT_SCALE * // 8.8 factors
                          ((float_t)GAMMA_SCALE) *
                          pow(((float_t)lLutCnt) / ((float_t)(OUTLUT_SIZE - 1)),
                              GAMMA) /
                          (float_t(lLutCnt << (16 - OUTLUT_BITS))) // 15 bit input values
        );
      }
      if(val0 > 0xffff)
        val0 = 0xffff;
      lpLutData[lLutCnt++] = val0;
    } // while lLutCnt < OUTLUT_SIZE
    printf("Output Lut: gamma %f\n", GAMMA);
    lpLutDataWrite = lpLutData;
    lError         = LIB_SUCCESS;
  }
  else if(aLut == 2)
  {
#define LOG_A_12TO8 15453.32
#define LOG_B_12TO8 0.016704
    while(lLutCnt < OUTLUT_SIZE)
    {
      if(!lLutCnt)
      {
        val0 = 0;
      }
      else
      {
        val0 = (uint32_t)(OUTLUT_SCALE * LOG_A_12TO8 * logf((lLutCnt)*LOG_B_12TO8 + 1) /
                          ((float_t)(lLutCnt << (16 - OUTLUT_BITS))));
      }
      if(val0 > 0xffff)
        val0 = 0xffff;
      lpLutData[lLutCnt++] = val0;
    } // while lLutCnt < OUTLUT_SIZE
    printf("Output Lut: log 12to8\n");
    lpLutDataWrite = lpLutData;
    lError         = LIB_SUCCESS;
  }
  else if(aLut == 3)
  {
#define LOG_A_16TO8 8332.00
#define LOG_B_16TO8 0.634928
    while(lLutCnt < OUTLUT_SIZE)
    {
      if(!lLutCnt)
        val0 = 0x0;
      else
      {
        val0 = (uint32_t)(OUTLUT_SCALE * LOG_A_16TO8 * logf((lLutCnt)*LOG_B_16TO8 + 1) /
                          ((float_t)(lLutCnt << (16 - OUTLUT_BITS))));
      }
      if(val0 > 0xffff)
        val0 = 0xffff;
      lpLutData[lLutCnt++] = val0;

    } // while lLutCnt < OUTLUT_SIZE
    printf("Output Lut: log 16to8\n");
    lpLutDataWrite = lpLutData;
    lError         = LIB_SUCCESS;
  }
  else if(aLut == 4)
  {
    printf("Output Lut: equalized\n");
    //lError=LIB_SUCCESS; //do not make success to keep LUT offset
  }
  else if(aLut == OUTLUT_SETS)
  {
    // go to previous lut
    ISP_LutGet(OUTLUT_IPUS0, aLutOffset ^ 2048, lpLutData, OUTLUT_SIZE);
    printf("dump Output Lut:\n");
    while(lLutCnt < OUTLUT_SIZE)
    {
      printf("lut4k[%d]=%d %f\n", lLutCnt, lpLutData[lLutCnt],
             ((float)(lpLutData[lLutCnt] * lLutCnt)) / ((float)OUTLUT_SIZE - 1) / 256.0);
      lLutCnt++;
      // LUTD (IPUS)
    } // while lLutCnt < OUTLUT_SIZE
  }
  if(lError == LIB_SUCCESS)
  {
    // apply LUT updates
    if(ISP_LutSet(OUTLUT_IPUS0, aLutOffset, lpLutDataWrite, OUTLUT_SIZE) != SEQ_LIB_SUCCESS)
    {
      printf("Failed to update IPUS %d LUT\n", OUTLUT_IPUS0);
      return lError;
    }
    // lutoffset to gpr15
    setRegCoherent(OUTLUT_ENGINE0, 0, IPUS_GPR0 + 15, aLutOffset);
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
    if(ISP_LutSet(OUTLUT_IPUS1, aLutOffset, lpLutDataWrite, OUTLUT_SIZE) != SEQ_LIB_SUCCESS)
    {
      printf("Failed to update IPUS %d LUT\n", OUTLUT_IPUS1);
      return lError;
    }
    // lutoffset to gpr15
    setRegCoherent(OUTLUT_ENGINE1, 0, IPUS_GPR0 + 15, aLutOffset);
#endif
  }
  return lError;
} // SetOutLut()

// ============================================================================

int main(int argc, char** argv)
{
  std::string helpMsg =
      std::string("Demonstrates MIPI interface and H264 encoding with camera OmniVision_9716.\n\tUsage: ") +
      COMMON_ExtractProgramName(argv[0]);

  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp < 0)
  { // print help message even if no help option is provided by the user
    printf("%s", helpMsg.c_str());
  }

  LIB_RESULT lRet = LIB_SUCCESS;
  LIB_RESULT lRes = LIB_SUCCESS;

  AppContext_t arContext;
  // assign AppContext to global pointer
  gpAppContext = &arContext;

  // init app context
  ContextInit(arContext);

  printf(
      "\nstarting demo: isp_ov9716_mipi_s\n"
      "\t\tMEDIA %s\n"
      "\t\tCSI   %d\n"
      "\t\tCAMs  %d\n",
      CAM_MEDIA, CSI_IDX_CAM, CAMS);

  OAL_Initialize();

#ifdef __STANDALONE__
  //*** Init DCU Output ***

  io::FrameOutputDCU lDcuOutput(SCR_WIDTH, SCR_HEIGHT, io::IO_DATA_DEPTH_08, DCU_CHNL_CNT
#if(ISP_OUTPUT == ISP_OUTPUT_RGBY)
                                ,
                                DCU_BPP_32
#elif(ISP_OUTPUT == ISP_OUTPUT_RGB)
                                ,
                                DCU_BPP_24
#elif(ISP_OUTPUT == ISP_OUTPUT_YUV422)
                                ,
                                DCU_BPP_16
#elif(ISP_OUTPUT == ISP_OUTPUT_YUV420)
                                ,
                                DCU_BPP_24
#endif
  );

#else // #ifdef __STANDALONE__

  // prepare histogram data file
#ifdef LOGHIST
  FILE* lpHistFw = fopen(HISTOGRAM_DATA_FILE, "wt");
  if(lpHistFw == NULL)
  {
    printf("Failed to open histogram storage file.\n");
    return LIB_FAILURE;
  } // if failed to open file
#endif

  // setup Ctrl+C handler
  if(SigintSetup() != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to register Ctrl+C signal handler.");
    return -1;
  }
  printf("Press Ctrl+C to terminate the demo.\n");

  // *** set terminal to nonblock input ***
  TermNonBlockSet();

  io::FrameOutputV234Fb lDcuOutput(SCR_WIDTH, SCR_HEIGHT, io::IO_DATA_DEPTH_08, DCU_CHNL_CNT
#if(ISP_OUTPUT == ISP_OUTPUT_RGBY)
                                   ,
                                   DCU_BPP_32
#elif(ISP_OUTPUT == ISP_OUTPUT_RGB)
                                   ,
                                   DCU_BPP_24
#elif(ISP_OUTPUT == ISP_OUTPUT_YUV422)
                                   ,
                                   DCU_BPP_16
#elif(ISP_OUTPUT == ISP_OUTPUT_YUV420)
                                   ,
                                   DCU_BPP_24
#endif
  );

#endif // else from #ifdef __STANDALONE__

  //*** allocate Input buffer for edge enhancement ***
  if(0 != DdrBuffersAlloc(arContext.mppGPUVirtual, arContext.mpGPUPhysical,
                          DISPLAY_PIXELS * DISPLAY_LINES * DCU_CHNL_CNT, DDR_OUT_BUFFER_CNT))
  {
    printf("Failed to allocated Stitching output buffers (size %dbytes)\n",
           DISPLAY_PIXELS * DISPLAY_LINES * DCU_CHNL_CNT);
    return (-1);
  } // if failed to allocate buffers

    // *** allocate buffer for saving output image
#ifdef __STANDALONE__
  arContext.mpSaveISP = OAL_MemoryAllocFlag(DISPLAY_PIXELS * DISPLAY_LINES * DCU_CHNL_CNT,
                                            OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_MEMORY_FLAG_CONTIGUOUS);
  if(!arContext.mpSaveISP)
  {
    printf("Out of memory to alloc gpSaveIPS (size %dbytes)\n", OUT_PIXELS * OUT_LINES * DCU_CHNL_CNT);
  }
  arContext.mpSaveISP = OAL_MemoryReturnAddress(arContext.mpSaveISP, ACCESS_NCH_NB);
#endif

  //
  // *** Initialize SDI ***
  //
  lRes = sdi::Initialize(0);

  SEQ_EventHandlerSet(SeqEventHandler); // set the event handler

  // create grabber
  arContext.mpGrabber = new(sdi_grabber);
  if(arContext.mpGrabber == NULL)
  {
    printf("Failed to create sdi grabber.\n");
    return -1;
  } // if failed to create grabber

  if(arContext.mpGrabber->ProcessSet(gpGraph, &gGraphMetadata) != LIB_SUCCESS)
  {
    printf("Failed to set ISP graph to grabber.\n");
    return -1;
  } // if ISP graph not set

  // *** prepare FDMA channels ***
  // goal is to program FDMA to merge the 2 output streams (per line) into one
  // huge DDR buffer with width = 4*1280 pix and hight = 2*720 lines
  arContext.mpFdma = (sdi_FdmaIO*)arContext.mpGrabber->IoGet(SEQ_OTHRIX_FDMA);
  if(arContext.mpFdma == NULL)
  {
    printf("Failed to get FDMA object.\n");
    return -1;
  } // if no FDMA object

  // *** 4*1280x720 Y frame ***
  SDI_ImageDescriptor lQuadFrmDesc;
  SDI_DdrBufferArr    lQuadBuffArr;

  // tranform the full DDR image geometry to having the stride to be
  // 2x(4*1280) pix with 720/2 lines
  // this allows easy division into 2 rectangular ROIs thus merging
  // the 2 FDMA
  // channel outputs into one DDR buffer
  lQuadFrmDesc = SDI_ImageDescriptor(OUT_PIXELS * 2,   // two paths
                                     OUT_LINES2_0 * 2, // first Y and then UV
                                     GS8);

  // allocate contigous buffers for the full DDR image
  if(lQuadBuffArr.Allocate(lQuadFrmDesc, DDR_OUT_BUFFER_CNT) != LIB_SUCCESS)
  {
    printf("Failed to allocated ISP output buffers\n");
    return 1;
  }

  // FDMA Y path 0
  SDI_DdrBufferArr lQuadBuffArrRoi;
  lQuadBuffArrRoi = lQuadBuffArr.Roi(0,             // X-Offset
                                     0,             // Y-Offset ??
                                     OUT_PIXELS,    // ROI X-size
                                     OUT_LINES2_0); // ROI Y-Size
  lQuadBuffArrRoi.ChannelIdxSet(FDMA_IX_ISP_OUTPUT0);
  if(arContext.mpFdma->DdrBuffersSet(lQuadBuffArrRoi) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor.\n");
    return -1;
  } // if frame descriptor setup failed

  // FDMA Y path 1
  lQuadBuffArrRoi = lQuadBuffArr.Roi(OUT_PIXELS,    // X-Offset
                                     0,             // Y-Offset
                                     OUT_PIXELS,    // ROI X-size
                                     OUT_LINES2_1); // ROI Y-Size
  lQuadBuffArrRoi.ChannelIdxSet(FDMA_IX_ISP_OUTPUT1);
  if(arContext.mpFdma->DdrBuffersSet(lQuadBuffArrRoi) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor.\n");
    return -1;
  } // if frame descriptor setup failed

  // FDMA UV path 0
  lQuadBuffArrRoi                  = lQuadBuffArr.Roi(0,               // X-Offset
                                     OUT_LINES2_0,    // Y-Offset
                                     OUT_PIXELS_UV,   // ROI X-size
                                     OUT_LINESUV2_0); // ROI Y-Size
  lQuadBuffArrRoi.mDesc.mWidthStep = OUT_PIXELS;
  lQuadBuffArrRoi.ChannelIdxSet(FDMA_IX_ISP_OUTPUT_UV0);
  if(arContext.mpFdma->DdrBuffersSet(lQuadBuffArrRoi) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor.\n");
    return -1;
  } // if frame descriptor setup failed

  // FDMA CAM path
  SDI_ImageDescriptor lCamDsc(CAM_SAVE_STRIDE16, // two paths
                              CAM_LINES,         // first Y and then UV
                              GS8);
  if(arContext.mpFdma->DdrBufferDescSet(FDMA_IX_CAM_OUTPUT, lCamDsc) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor.\n");
    return -1;
  } // if frame descriptor setup failed

  // FDMA channel 2 CAM_OUTPUT
  SDI_ImageDescriptor lCamFrmDesc;
  SDI_DdrBufferArr    lCamFrmArr;
  lCamFrmDesc = SDI_ImageDescriptor(CAM_SAVE_PIXELS, // just one image
                                    CAM_LINES,
                                    GS8); // as GS16 is missing

  // allocate contigous buffers for the rest of the DDR images
  if(arContext.mpFdma->DdrBuffersAlloc(DDR_OUT_BUFFER_CNT) != LIB_SUCCESS)
  {
    printf("Failed to allocated remaining FastDMA buffers\n");
    return 1;
  }

  // *** prestart grabber ***
  msleep(1);
  // *** prestart grabber ***
  if(arContext.mpGrabber->PreStart() != LIB_SUCCESS)
  {
    printf("Failed to prestart the grabber.\n");
    return -1;
  } // if PreStart() failed

    // Only an API example. Graph might not work if enabled!!!
#if(CAMS != 1)
  {
#define HIST_IPUSIDX 10 // look up in generated graph file
#define PATCH_LIST_CNT 3
#define SIN_OFFS_OFFS(_i)                                                                                              \
  ((uint32_t)(uintptr_t)gpGraph[HIST_IPUSIDX] + offsetof(SEQ_IpuCfg_t, mInBuf) + (_i) * sizeof(SEQ_IpuSrc_t) +         \
   offsetof(SEQ_IpuSrc_t, mcInXfer) + offsetof(SEQ_Xfer_t, mcStartOffsBytes));

    SEQ_patch_list_t lpPatchListArr[PATCH_LIST_CNT];
    lpPatchListArr[0].mOffset = SIN_OFFS_OFFS(0); // SDMAin#0
    lpPatchListArr[0].mNumber = 2;                // two values to be toggle on V-Sync
    lpPatchListArr[0].mVal[0] = HIST_OFFSET0 * 2;
    lpPatchListArr[0].mVal[1] = HIST_OFFSET1 * 2;

    lpPatchListArr[1].mOffset = SIN_OFFS_OFFS(1); // SDMAin#1
    lpPatchListArr[1].mNumber = 2;
    lpPatchListArr[1].mVal[0] = HIST_OFFSET0 * 2;
    lpPatchListArr[1].mVal[1] = HIST_OFFSET1 * 2;

    lpPatchListArr[2].mOffset = SIN_OFFS_OFFS(2); // SDMAin#2
    lpPatchListArr[2].mNumber = 2;
    lpPatchListArr[2].mVal[0] = HIST_OFFSETVS0 * 2;
    lpPatchListArr[2].mVal[1] = HIST_OFFSETVS1 * 2;

    SEQ_PatchListSet(lpPatchListArr, PATCH_LIST_CNT);
  }
#endif // 0

    // *** configure camera ***
#ifndef __STANDALONE__
  IniParse(arContext);
#endif

#if(UDP264 == 1)
  gH264frame = cv::Mat(SCR_WIDTH, SCR_HEIGHT, CV_8UC1);
  if(UdpTransmitInit() != 0)
  {
    printf("Failed to initialize UDP transmitter.\n");
    return -1;
  }
  H264Encoder::H264Encoder_setup(1280, 720, STREAM_LINES_NUMBER, YUV420sp);
  H264Encoder::getInstance()->Initialize();
  H264Encoder::getInstance()->setOnEncoderDoneListener(&onH264encodingDone);
#endif // if (UDP264 == 1)

  msleep(100);

  IspConfig(arContext);

  // print keyboard command help

  // *** initial keyboard commands ***
  arContext.mKbKey = 'h';
  if(KbAction(arContext) != 0) // print keyboard help
  {
    printf("Keyboard control failed.\n");
    return -1;
  }

  uint32_t lLoop;
  uint8_t  lExpCnt = 0;

  // fetched frame buffer storage
  SDI_Frame lFrame[FDMA_IX_END];

  SDI_Frame lNextFrame;
  uint32_t  lSkippedFrames;
  uint32_t  lMainFdmaChannel = FDMA_IX_ISP_OUTPUT1;
  uint32_t  lPopFailed       = 0;
  uint32_t  lPopTime;

  // *** start grabbing ***
  if(arContext.mpGrabber->Start() != LIB_SUCCESS)
  {
    printf("Failed to start the grabber.\n");
    return -1;
  } // if Start() failed

  GETTIME(&arContext.mTimes);

  while(1)
  {
    for(lLoop = 0; lLoop < LOOP_NUM; lLoop++)
    {
      // make coherent update of registers
      setRegCoherentFlush();

      // wait for Neon to finished processing
      if(arContext.mGpuActive)
      {
        // *** done in any case as not in a parallel threat
        //ExtractDisplay.wait();
        // push to all channels to keep buffers in sync
        for(int lFdmaChannel = 0; lFdmaChannel < FDMA_IX_END; lFdmaChannel++)
        {
          arContext.mpGrabber->FramePush(lFrame[lFdmaChannel]);
        } // for all FDMA channels
      }

      if(arContext.mKbLog == 2)
      {
        GETTIME(&lPopTime);
        arContext.mTimeA53 -= lPopTime;
      }

      // **** frame pop start
      //grab main output
      lFrame[lMainFdmaChannel] = arContext.mpGrabber->FramePop(lMainFdmaChannel);
      if(lFrame[lMainFdmaChannel].mUMat.empty())
      {
        printf("Failed to grab a frame # %u. Terminating\n", arContext.mFrmCnt);
        lPopFailed = 1;
        break;
      } // if pop failed

      if(arContext.mKbLog == 2)
      {
        GETTIME(&lPopTime);
        arContext.mTimeA53 += lPopTime;
      }

      // if if there are more buffers in the queue already
      lSkippedFrames = 0;
      lNextFrame     = arContext.mpGrabber->FramePopNonBlock(lMainFdmaChannel);
      while(lNextFrame.mUMat.empty() == false)
      {
        arContext.mpGrabber->FramePush(lFrame[lMainFdmaChannel]);
        lFrame[lMainFdmaChannel] = lNextFrame;
        lSkippedFrames++;
        lNextFrame = arContext.mpGrabber->FramePopNonBlock(lMainFdmaChannel);
      }
      // CSI error detection with printf to console!

      if(lSkippedFrames > 0)
      {
        printf("Skipped %u frames\n", lSkippedFrames);
      } // if any frame skipped

      // grap all reamining fdma channels as well
      for(uint32_t lFdmaChannel = 0; lFdmaChannel < FDMA_IX_END; lFdmaChannel++)
      {
        if(lFdmaChannel == lMainFdmaChannel)
        {
          continue;
        } // if main channel

        lFrame[lFdmaChannel] = arContext.mpGrabber->FramePopNonBlock(lFdmaChannel);
        if(lFrame[lFdmaChannel].mUMat.empty())
        {
          printf("Failed to grab a frame # %u. Terminating\n", arContext.mFrmCnt);
          lPopFailed = 1;
          break;
        } // if pop failed

        while(lSkippedFrames)
        {
          arContext.mpGrabber->FramePush(lFrame[lFdmaChannel]);
          lFrame[lFdmaChannel] = arContext.mpGrabber->FramePopNonBlock(lFdmaChannel);
          if(lFrame[lFdmaChannel].mUMat.empty() == false)
          {
            lSkippedFrames--;
          } // if pop succeeded
          else
          {
            printf("Failed to grab a frame # %u. Terminating\n", arContext.mFrmCnt);
            lPopFailed = 1;
            break;
          } // else from if pop succeeded
        }   // while
      }     // for all FDMA channels
      //**** frame pop finished

      // do exposure control
      if((arContext.mKbAEC | arContext.mKbAWB) && arContext.mFrmCnt)
      {
        ExposureCtl(CSI_IDX_CAM, &lExpCnt, gIpuHist, arContext.mKbLog, arContext.mKbAEC, 4,
                    (256.0 - arContext.mKbLtrgt) / 256.0, 16, arContext.mKbVStrgt / 256.0, arContext.mKbAWB);

        SEQ_StatGet(&gIpuStat);
      }

      //**** processing pipeline from last to first

      // freeze display and processing
      if(arContext.mKbDisplay)
      {
        arContext.mpFrameIsp = lFrame[FDMA_IX_ISP_OUTPUT0].mUMat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data;
#if(CAM_SAVE == 1)
        arContext.mpFrameCAM = lFrame[FDMA_IX_CAM_OUTPUT].mUMat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data;
#endif //if(CAM_SAVE==1)

        if(arContext.mGpuActive)
        {
          arContext.mpFrameFbs = arContext.mppGPUVirtual[arContext.mGpuBufferNr];
          lDcuOutput.PutFrame(arContext.mpFrameFbs);
        }
      }

      // call GPU processing
      {
        arContext.mGpuBufferNr++;
        if(arContext.mGpuBufferNr == DDR_OUT_BUFFER_CNT)
        {
          arContext.mGpuBufferNr = 0;
        }
        vsdk::Mat lTmpMat = lFrame[FDMA_IX_ISP_OUTPUT0].mUMat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
        ExtractDisplay(lTmpMat.data, arContext);
        arContext.mGpuActive = 1;
      }
      if(arContext.mOutLut == 4)
      {
        // do histogram equalization
        if(LIB_SUCCESS == EqualizeHistogram(&gCompHist, arContext.mOutLutOffset /*,gOutLut*/))
        {
          arContext.mOutLutOffset ^= 2048;
        }
        if(arContext.mOutLut != arContext.mOldOutLut)
        {
          printf("Output Lut: equalized\n");
          arContext.mOldOutLut = arContext.mOutLut;
        }
      }
      //check if LUT needs update
      else if(arContext.mOutLut != arContext.mOldOutLut)
      {
        arContext.mOldOutLut = arContext.mOutLut;
        if(SetOutLut(arContext.mOldOutLut, arContext.mOutLutOffset) == LIB_SUCCESS)
        {
          arContext.mOutLutOffset ^= 2048;
        }
      }

      // act on keyboard input
      KeyboardInputProcess(arContext);

      if(arContext.mKbCursorOn)
      {
        uint8_t*  lpPixel;
        uint8_t   i;
        vsdk::Mat lTmpMat = lFrame[FDMA_IX_ISP_OUTPUT0].mUMat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);

        lpPixel = lTmpMat.data;

        lpPixel += ISP_CHNL_CNT * (arContext.mKbCursorX + arContext.mKbCursorY * OUT_PIXELS);
        for(i = 0; i < ISP_CHNL_CNT; i++)
          *(lpPixel++) = 255;
        {
          lpPixel -= ISP_CHNL_CNT;
        }
      }
      CsiErrorDetection(arContext);

      arContext.mFrmCnt++;
    } // for LOOP_NUM ********

    if(lPopFailed)
    {
      break;
    } // if gpFramout == NULL

#ifndef __STANDALONE__
    if(sStop)
    {
      break; // break if Ctrl+C pressed
    }        // if Ctrl+C
#endif       // #ifndef __STANDALONE__
  }          // while(1)

  printf("\nSequencer stopped working in frame %d\n", arContext.mFrmCnt);

  {
    uint8_t lCamStreaming;
    CAM_RegRead(CSI_IDX_CAM, OV9716_STREAM_ONOFF, lCamStreaming);
    printf("Camera is %sstreaming\n", (lCamStreaming) ? "" : "not ");
  }
  ISP_DumpIpuState();

#if(UDP264 == 1)
  printf("Thread #1 require end\n");
  // Ending
  {
    std::lock_guard< std::mutex > mLock(gMtxDataReady);
    gDataReady = true;

    std::lock_guard< std::mutex > mLock2(gMtxTerminate);
    gTerminate = true;
  }
  gCvWaiting.notify_one();
  printf("Thread #1 is waiting for thread #2 end\n");
  sending_thread.join();
  printf("Thread #1 end\n");
#endif // if (UDP264 == 1)

#ifdef DO_BUFFER_LOG
  {
    SEQ_Head_Ptr_t* lpFetchedGraph = NULL;
    SEQ_Buf_t**     lppFetchedBufferList;

    lppFetchedBufferList = arContext.mpGrabber->GraphFetch(&lpFetchedGraph);
    ISP_PrintGraphStat(lppFetchedBufferList);
  }
#endif

#ifndef __STANDALONE__
  // *** set terminal back to block input ***
  TermBlockSet();

#ifdef LOGHIST
  //fclose(lpHistFw);
#endif
#endif // ifndef __STANDALONE__

#ifdef __STANDALONE__
  for(;;)
  { // *** don't return ***
    KeyboardInputProcess(arContext);
    msleep(10);
  }
#endif // #ifdef __STANDALONE__

  if(arContext.mpGrabber)
  {
    //*** Stop ISP processing ***
    arContext.mpGrabber->Stop();

    // clean up grabber resources
    arContext.mpGrabber->Release();

    delete(arContext.mpGrabber);
  } // if grabber exists

  //
  // Close SDI
  //
  lRes = sdi::Close(0);
  if(lRes != LIB_SUCCESS)
  {
    lRet = LIB_FAILURE;
  } // if lRes not LIB_SUCCESS

  DdrBuffersFree(arContext.mppGPUVirtual, arContext.mpGPUPhysical, DDR_OUT_BUFFER_CNT);

  return lRet;

} // main()

//***************************************************************************

int32_t DdrBuffersAlloc(void** appVirtual, uint32_t* apPhysical, size_t aMemSize, uint32_t aBufferCnt)
{
  int32_t lRet = 0;

  // allocate buffers & get physical addresses
  for(uint32_t i = 0; i < aBufferCnt; i++)
  {
    apPhysical[i] = (uint32_t)(uint64_t)OAL_MemoryAllocFlag(aMemSize, OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) |
                                                                          OAL_MEMORY_FLAG_CONTIGUOUS | OAL_ALLOC_DDR0);

    if(apPhysical[i] == 0)
    {
      lRet = -1;
      break;
    }
    appVirtual[i] = OAL_MemoryReturnAddress((void*)(uint64_t)(apPhysical[i]),
                                            ACCESS_CH_WB); // get physical address

    memset(appVirtual[i], 0x00, aMemSize);
#if defined(__STANDALONE__) && defined(__ARMV8)
    flush_dcache_range(appVirtual[i], aMemSize);
#endif // #ifdef __STANDALONE__
  }    // for all framebuffers

  if(lRet != 0)
  {
    DdrBuffersFree(appVirtual, apPhysical, aBufferCnt);
  }

  return lRet;
} // DdrBuffersAlloc()

//***************************************************************************

void DdrBuffersFree(void** appVirtual, uint32_t* apPhysical, uint32_t aBufferCnt)
{
  for(uint32_t i = 0; i < aBufferCnt; i++)
  {
    if(appVirtual[i] != NULL)
    {
      OAL_MemoryFree((void*)(uint64_t)apPhysical[i]);
    } // if buffer allocated

    appVirtual[i] = NULL;
    apPhysical[i] = 0;
  } // for all framebuffers
} // DdrBuffersFree()

//***************************************************************************

void SeqEventHandler(uint32_t aEventType)
{
  switch(aEventType)
  {
    case SEQ_MSG_TYPE_FRAMEDONE:;
      //gFrameDoneCnt++;
      break;

    case SEQ_MSG_TYPE_OTHERERROR:
      char lString[SEQ_PRAM_AUX_DATA_SIZE] = {0};

      printf("*** Sequencer-Error: ");

      if(SEQ_MsgAuxDataGet(lString, SEQ_PRAM_AUX_DATA_SIZE) != SEQ_LIB_SUCCESS)
      {
        VDB_LOG_NOTE("Failed to get the auxiliary data from PRAM.\n");
      }
      printf("%s\n", lString);
      break;
  }

} // SeqEventHandler(uint32_t aEventType)

#ifndef __STANDALONE__

//***************************************************************************

void SigintHandler(int)
{
  sStop = true;
} // SigintHandler()

//***************************************************************************

int32_t SigintSetup()
{
  int32_t lRet = SEQ_LIB_SUCCESS;

  // prepare internal signal handler
  struct sigaction lSa;
  memset(&lSa, 0, sizeof(lSa));
  lSa.sa_handler = SigintHandler;

  if(sigaction(SIGINT, &lSa, NULL) != 0)
  {
    VDB_LOG_ERROR("Failed to register signal handler.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if signal not registered

  return lRet;
} // SigintSetup()

#endif // #ifndef __STANDALONE__

/* EOF */
