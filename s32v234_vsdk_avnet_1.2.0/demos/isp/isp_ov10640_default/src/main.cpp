/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright (c) 2016-2017  NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __STANDALONE__
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
extern "C" {
#include "iniparser.h"
}
#endif // #ifdef __STANDALONE__
#include <string.h>
#include <math.h>

#include "../include/mymemcpy.h"
#include "../include/yuv420torgb.h"

#include "oal.h" // for malloc, cache and translating virtual to physical addressses
#include "vdb_log.h"
#include "isp_cam_maxim.h"   // includes IIC read write functions
#include "isp_cam_ov10640.h" // includes IIC read write functions
#include "sdi.hpp"           // camera middle ware

#ifdef __STANDALONE__
#include "frame_output_dcu.h"    // display controller in SA
#include "uartlinflex_driver.h"  // for keyboard input
#else                            // #ifdef __STANDALONE__
#include "frame_output_v234fb.h" // display controller for Linux
#endif                           // else from #ifdef __STANDALONE__

#include "isp_seq.h" // set ISP registers
// ISP functions
// graph related defines (FastDMA channels)
#include "ov10640_default_c.h"
#include "iofiles.h"             // parameters from graph (e.g., geometry)
#include "../include/exposure.h" // functions for exposure control

#include <common_time_measure.h>
#include <common_helpers.h>
#include <string>

#define CSI_ERROR_CNT

//***************************************************************************
// constants
//***************************************************************************
#define CAM_SAVE_PGM

#if(CAM_SAVE == 1)
#define CAM_OUT_FILE "ov10640_default_cam"
#ifdef CAM_SAVE_PGM
#define CAM_OUT_FILE_EXT "pgm"
#else
#define CAM_OUT_FILE_EXT "raw"
#endif
#endif // if (CAM_SAVE==1)

#ifdef LOGHIST
#define HISTOGRAM_DATA_FILE "hist_pix_cnt.txt"
#endif

#define INI_FILE_NAME "ov10640_quad.ini"

// ***** ISP Graph Defines *******

// MIPI_PORT is defined as constant in Graph and propagated
// via the Graph header file
#if(MIPI_PORT == 1) // for IIC communication
#define CSI_IDX_CAM CSI_IDX_1
#else
#define CSI_IDX_CAM CSI_IDX_0
#endif

#define IPUVTYPE 0
#define IPUSTYPE 1

#define EXPHIST_ENGINE 0
#if(EXPHIST_ENGINE == 0)
#define SEQ_EXP_HIST SEQ_IPUS0
#elif(EXPHIST_ENGINE == 1)
#define SEQ_EXP_HIST SEQ_IPUS1
#endif
#define EXPOSURE_ENGINE IPUSTYPE, EXPHIST_ENGINE

#define HDR_IPUSIDX 7
#define HDR_ENGINE IPUSTYPE, HDR_IPUSIDX

#define DEBAYER_ENGINE0 IPUSTYPE, 1
#define DEBAYER_ENGINE1 IPUSTYPE, 4

#define RGB2Y_ENGINE0 IPUVTYPE, 0
#define RGB2Y_ENGINE1 IPUVTYPE, 3

#define INLUT_IPUS 2
#define INLUT_ENGINE IPUSTYPE, INLUT_IPUS
#define IPUS_INLUT IPUS_2

#define OUTLUT_IPUS0 3
#define OUTLUT_IPUS1 3
#define OUTLUT_ENGINE0 IPUSTYPE, OUTLUT_IPUS0
#define OUTLUT_ENGINE1 IPUSTYPE, OUTLUT_IPUS1
#if(OUTLUT_IPUS0 == 2)
#define IPUS_OUTLUT0 IPUS_2
#else
#define IPUS_OUTLUT0 IPUS_3
#endif
#if(OUTLUT_IPUS1 == 2)
#define IPUS_OUTLUT1 IPUS_2
#define SEQ_LUT_HIST SEQ_IPUS2
#else
#define IPUS_OUTLUT1 IPUS_3
#define SEQ_LUT_HIST SEQ_IPUS3
#endif

#define TONEMAP_ENGINE0 IPUVTYPE, 1
#define TONEMAP_ENGINE1 IPUVTYPE, 2

#define DENOISEY_TYPE IPUSTYPE
#if(DENOISEY_TYPE == IPUVTYPE)
#define DENOISEY_ENGINE0 IPUVTYPE, 0
#define DENOISEY_ENGINE1 IPUVTYPE, 3
#define DENOISEY_GPR 0x50
#else
#define DENOISEY_ENGINE0 IPUSTYPE, 5
#define DENOISEY_ENGINE1 IPUSTYPE, 6
#define DENOISEY_GPR 0x70
#endif

#define DENOISEUV_ENGINE IPUVTYPE, 0

#include "global_defines.h"

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

// ***** Run Control and Logging Defines *******
#define CAM_DGAIN_REG 0x3162
#define CAM_VTS 0x30b2
#define MAX_COHERENT_REGS 32

// ** Histograms from the cameras (3 exposure - B, Gb, Gr, R)
#if(EMBEDDED_DATA == 1)
// enum color_channel {BLUE=0, GREEN_B, GREEN_R, RED};

uint8_t* buff_add[4];
uint8_t  var_test = 2;
uint16_t exp_time;
uint8_t  cam_hdr_config, cam_hdr_config_b, cam_hdr_config_r;
uint32_t pixel_level_correction = 11640;
#endif

//***************************************************************************
// global variables
//***************************************************************************

// ** variables for main image loop
#if(CAM_SAVE == 1)
vsdk::Mat gFrameCAM;         // current frame from CAM
#endif                       // if (CAM_SAVE==1)
vsdk::Mat gFrameISP;         // current frame from ISP
void*     gpFrameFbs = NULL; // current frame to display

// registered propcess after ISP
uint8_t gGpuActive    = 0;
uint8_t gGpuBufferNr  = 0;
uint8_t gApexActive   = 0;
uint8_t gApexBufferNr = 0;

#ifdef __STANDALONE__
void* gpSaveISP = NULL; // current frame from ISP
#endif
uint32_t      gFrameIRQCnt     = 0;
uint32_t      gFrameIRQCntLast = 0;
uint32_t      gFrameIRQDiff;
uint32_t      gFrmCnt = 0;
unsigned long gTimes;
uint32_t      gTimee, gTimeA53, gTimeCSC, gTimeApex;
int32_t       gTimed;

/* IPU registers with coherent update */
SEQM_ipu_reg_t gRegVal[MAX_COHERENT_REGS];
uint16_t       gCoherentRegIx = 0;
uint8_t        gDoOnce        = 0;

#ifdef __STANDALONE__
//FPS measurement externed from csi_func.c
extern unsigned int gMeasuredFramePeriod;
extern unsigned int gMeasuredFramePeriodOld;

#endif // __STANDALONE__
//Flickering LED control software global variables
double  gFrequency  = 50.0;
uint8_t gPwmPercent = 5;

// ** variables for exposure and channel gain control
//uint32_t gExpRange;
SEQ_IpuHist_t gIpuHist[2]; // for even or odd frames
SEQ_IpuHist_t gCompHist;   //histogram to compute compression

SEQ_IpuStat_t gIpuStat; // statistic data

// ** variables for exposure control and white balancing
#define CHGAINFACTOR (256) // factor over 8.8
uint32_t gAWB_Mean[4];
uint8_t  gChannelMap[4] = {0, 2, 1, 3}; //(chg: g swpapped)
uint32_t gChGainVal[4];
uint32_t gChGainMax[4] = {800, 320, 320, 800};
#define CHGAINSETS 10
uint16_t gChGainTab[CHGAINSETS][4] = {
    {400, 256, 256, 400}, {400, 256, 256, 300}, {300, 256, 256, 400}, {256, 400, 400, 256}, {256, 300, 400, 256},
    {256, 400, 300, 256}, {256, 0, 0, 0},       {0, 256, 256, 0},     {0, 0, 0, 256},       {256, 256, 256, 256}};

#define CCM_SETS 9
float_t gKbCcmMatrix[CCM_SETS][9] = // signed values in s1.6 bits
    {{                              // default for config file
      1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
     {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
     {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
     {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
     //fixed tables
     {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
     {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
     {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
     {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
     {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f}}; // do nothing matrix
float_t gRgb2YuvMatrix[9] = {0.299, 0.587, 0.144, -0.14741, -0.28939, 0.422008, 0.614777, -0.5148, -0.12629};

float_t gCcmCscMatrix[9] = {0.299, 0.587, 0.144, -0.14741, -0.28939, 0.422008, 0.614777, -0.5148, -0.12629};

// ** variables for keyboard interaction
uint8_t  gKbKey      = 0;
uint8_t  gKbMode     = 0; // 0=off, 'K'= knee point, r=redgain, b=bluegain
uint16_t gKbMeasure  = 0; // cycle counter for measurment
uint8_t  gKbDisplay  = 1; // 0-1
uint16_t gKbCursorOn = 0;
uint16_t gKbCursorX  = 640;
uint16_t gKbCursorY  = 360;
uint8_t  gKB_shift   = 3; // 0-4
#define MAXLOG 7
uint8_t  gKbLog         = 0; // 0=off
uint32_t gKbI2c         = 0; // IIC command 0xaaaavv  a=address v=value
uint8_t  gKbTestbar     = 0; // 1=on 0=off
int16_t  gKbXoffset     = 0; // 0 - ((4x1280)-1920 = 3200)
uint8_t  gKbEngine      = 0;
uint16_t gKbGPR         = 15;
uint32_t gKbParVal      = 0;
uint8_t  gKbExpSel      = 0; // all exposures on
uint16_t gExpIncrement  = 8; // step size for manual exposure time setting
uint16_t gDgIncrement   = 8; // step size for manual digital gain setting
uint8_t  gExpName[6][9] = {"L/S + VS", "VS", "S", "L", "L/S", "L + VS"};
uint8_t  gKbAlpha       = 16;  // long exposure factor in 8.8
float_t  gKbSaturation  = 1.0; // Color saturation in 0<= sat < 2
int16_t  gRBIncrement   = 16;
uint8_t  gKbCCM         = 0; // select CCM table
#define EDGETHRDIFF 16
uint8_t gKbEdgeThrLow  = 32; // edge enhancement lower  threshold
uint8_t gKbEdgeThrHigh =     // edge enhancement higher threshold
    gKbEdgeThrLow + EDGETHRDIFF;
uint8_t gKbEdgeMaxA = 64;   // edge enhancement strength in 2.6 fixed point
uint8_t gKbEdgeMaxA1;       // shadow for edge enhancement strength
uint8_t gKbEdgeChannel = 1; // Green Channel to be used for
                            // edge enhancement
uint32_t gKbAEC = 1;        // off/S32V/Camera auto exposure
uint32_t gKbAWB = 1;        // auto white balancing
#define MAXINLUT 3
uint8_t gInLut = 3; // 12 to 8
#define MAXOUTLUT 5
#define OUTLUT_BITS 11                 // bits for 2048 lutsize
#define OUTLUT_SIZE (1 << OUTLUT_BITS) // 2048
#define OUTLUT_SCALE 256.0
uint16_t          gOutLutOffset        = 0;            // 0 or 2048
uint8_t           gOutLut              = 2;            // 0/1 for dual, 2 for triple, 4 for equalized
uint8_t           gOutLut1[4]          = {2, 2, 1, 2}; // back up for modes
uint8_t           gOldOutLut           = -1;           // 12 to 8
uint8_t           gKbBlackLevel        = 0;
uint8_t           gKbChgain            = 0;      // 1=on 0=off
uint8_t           gKbLtrgt             = 80;     // long exposure control target
uint8_t           gKbVStrgt            = 28;     // Very short exposure control target
uint16_t          gKbYDenoise          = 1024;   // Y channel denoise threshold level
uint16_t          gKbViewEdge          = 0xffff; // 0xffff: view denoise values; 0: view edge
uint8_t           gKbDeFlicker         = 0;      // deflicker mode: off, 50Hz, 60Hz
float_t           gFlickerFrequency[4] = {100.0, 100.0, 120.0, 90.0};
uint8_t           gFlickerEnable[4]    = {0, 1, 1, 1};
uint8_t           gKbYuvToRgb          = 1; // 0=A53; 1=Neon
OV10640_ExpTime_t gKbExpToChange       = ExpTime_VShort;
gParam            gKbExpParamChange    = Time;
uint16_t          gKbExpVal            = 0;
int32_t           gKbExpTime           = 0;

const char* gExposureName[3] = {"VS", "S", "L"};
const char* gParamName[4]    = {"Time", "AG", "DG", "CG"};

//***************************************************************************
// macro
//***************************************************************************

#if defined(__STANDALONE__) && defined(__ARMV8)
extern "C" {
void flush_dcache_range(void*, unsigned int);
}
#endif

/************************************************************************/
/** Compute combined matrix from color correction matrix,
 * color space conversion matrix (RGB-> YUV) and color saturation
 * \param  aCcmCsCMatrix     resulting combine 3x3 matrix (as 9x1)
 * \param  aCcmMatrix        input CCM 3x3 matrix (as 9x1)
 * \param  aCcmCsCMatrix     input CSC 3x3 matrix (as 9x1)
 * \param  aCcmCsCMatrix     input saturation factor
 *
 * \return 0 if all ok -1 otherwise
 ************************************************************************/
void CcmCscMatrix(float_t* aCcmCsCMatrix, float_t* aCcmMatrix, float_t* aCscMatrix, float_t aSaturation);

//***********************************************************************
// extract one 1280 pixles for bein dispalyed
//***********************************************************************

void ExtractDisplay(void* appInVirtual, void* appOutVirtual);

//***********************************************************************
/** call histogram equalization: read histogram and create new OutLut
 *
 * \param  aLutOffset    lower table: 0 or upper table: 2048
 *
 * \return 0 if all ok -1 otherwise
 **********************************************************************/

uint8_t EqualizeHistogram(uint16_t aLutOffset);

//***************************************************************************
// local functions
//***************************************************************************

void KeyboardInputProcess(void);

/************************************************************************/
/** Configure ISP preprocessing.
 *
 ************************************************************************/
void IspConfig(void);

/************************************************************************/
/** Select Look Up Table for input
 *
 * \param  aLut          type of Look Up Tabel
 *                       0 = Linear
 *                       1 = Decompress to 16 bit
 *                       2 = 12 To 8 bit
 *
 ************************************************************************/
uint8_t SetInLut(uint8_t aLut);

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
/** Do Aoutomatic White Balancing (AWB)
  *
  * \param  pEmbd         pointer to embedded data array
  ************************************************************************/
void AWB(uint8_t* pEmbd);

/************************************************************************/
/** Status logging.
 *
 ************************************************************************/
void LogStatus(sdi_grabber* apGrabber);

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

/************************************************************************/
/** Save input or output frame (selected by gKbMode)
 * gKbMode == 'S': input  image (raw camera data)
 * gKbMode == 's': output image (ISP preprocessed data)
 *
 * \return 0 if all ok -1 otherwise
 ************************************************************************/
int32_t FrameSave(void);

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

/************************************************************************/
/** Checks if key was pressed or not.
 *
 * \return 0 if no key pressed
 *         >0 if key pressed
 ************************************************************************/
int KeyDown(void);

//***************************************************************************

int GetCharNonBlock(void);

//***************************************************************************

void TermBlockSet(void);

//***************************************************************************

void TermNonBlockSet(void);

/************************************************************************/
/** Parses infile and setsup default parameter values.
************************************************************************/
void IniParse(void);

#endif // #ifndef __STANDALONE__

/************************************************************************/
/** Handler for ISP events.
 *
 * \param  aEventType type of the event received from ISP
 ************************************************************************/
void SeqEventHandler(uint32_t aEventType);

//***************************************************************************

void KeyboardInputProcess(void);

//***************************************************************************

char GetChar(void);

//***************************************************************************

void KbAction(uint8_t key);

//***************************************************************************

static bool sStop = false; ///< to signal Ctrl+c from command line

//***************************************************************************

/************************************************************************/
/** Csi Error Detection
 *
 * Check global variables set in the csi_func.c interrupt handler when CSI error occurs
 *
 ************************************************************************/
void CsiErrorDetection(uint32_t arFrameNumber);

//***************************************************************************
/** FlexTimer SET function for PWM deflickering control
 *
 * Check global variables set in the csi_func.c interrupt handler when CSI error occurs
 *
 * \param  aFrequency Frequency in mHz
 * \param  aPwmPercent 5 - 20%, 4-25% , 2 - 50%
 *
 ************************************************************************/
void FlexTimer0_Set(double aFrequency, uint8_t aPwmPercent);

/************************************************************************/
#if(EMBEDDED_DATA == 1)
//***************************************************************************
void CamHistGet(color_channel ch, uint8_t* stats);

//***************************************************************************
void CamHistPrint();

//***************************************************************************
#endif
/** PWM base configuration
 *
 * This confiures SIUL and base FlexTIMER registers which are requried to run Flickering application
 *
 ************************************************************************/
void PWM_BaseConfig(void);

// ============================================================================

static SEQ_LIB_RESULT setRegCoherent(uint16_t aIsIPUS,
                                     uint16_t aEngNr,
                                     uint16_t aDirect,
                                     uint16_t aIndex,
                                     uint32_t aData);
static SEQ_LIB_RESULT setRegCoherentClose(void);

// ============================================================================
void PWM_BaseConfig(void)
{
#ifdef __STANDALONE__
  // set output pins
  SIUL2.MSCR[34].B.MUX_MODE = 4;
  SIUL2.MSCR[34].B.OBE      = 1; // output buffer enabled
  SIUL2.MSCR[34].B.DSE      = 7; // maximum drive strength

  SIUL2.MSCR[35].B.MUX_MODE = 3;
  SIUL2.MSCR[35].B.OBE      = 1; // output buffer enabled
  SIUL2.MSCR[35].B.DSE      = 7; // maximum drive strength

  // select internal clock as source
  FTM_0.SC.B.CLKS = 0x1;
  // divided by 128
  FTM_0.SC.B.PS = 0x7;
  // set timer max value
  FTM_0.MOD.B.MOD = 10416;

  // set output compare mode
  FTM_0.CONTROLS[4].CSC.B.MSB = 1;
  FTM_0.CONTROLS[4].CSC.B.MSA = 0;
  // set togled mode
  FTM_0.CONTROLS[4].CSC.B.ELSB = 1;
  FTM_0.CONTROLS[4].CSC.B.ELSA = 0;
  // set value
  FTM_0.CONTROLS[4].CV.B.VAL = 5208;

  FTM_0.CONF.B.BDMMODE = 0x3;

  /// J32 pins: 18, 20
  FlexTimer0_Set(gFrequency, gPwmPercent);

#endif //#ifdef __STANDALONE__
}

//***************************************************************************
void FlexTimer0_Set(double aFrequency, uint8_t aPwmPercent)
{
#ifdef __STANDALONE__
  int timerMaxValue  = 0;
  int timerHalfValue = 0;
  int divider        = 1;
  int divReg         = 0;

  timerMaxValue = round((40000000 / aFrequency) / divider);
  while(timerMaxValue > 65536)
  {
    divReg++;
    divider       = 1 << divReg;
    timerMaxValue = round((40000000 / aFrequency) / divider);
  }
  timerHalfValue = round(timerMaxValue / 100 * aPwmPercent); // 5 - 20%, 4-25% , 2 - 50%

  // turn off
  FTM_0.SC.B.CLKS = 0x0;

  // set
  FTM_0.SC.B.PS              = divReg;
  FTM_0.MOD.B.MOD            = timerMaxValue;
  FTM_0.CONTROLS[4].CV.B.VAL = timerHalfValue;

  // clear counter
  FTM_0.CNT.B.COUNT = 0x0;
  // turn on
  FTM_0.SC.B.CLKS = 0x1;
#else  //#ifdef __STANDALONE__
  (void)aFrequency;
  (void)aPwmPercent;
#endif //#ifdef __STANDALONE__
}

//***************************************************************************

void CsiErrorDetection(uint32_t arFrameNumber)
{
  static uint8_t  lCsiErrorCount = 0;
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
    return;
  }

  if(lErrStatus.mLineErr & 0x4444) // filter line length errors
  {
    lError = true;
    if(gDoOnce == 0)
    {
      printf("Error: ***** CSI Line Length Error in frame %d,\n", arFrameNumber);
      for(uint32_t i = 0; i < CSI_CHNL_CNT; i++)
      {
        printf("VC %u: error length %d pixels\n", i, lErrStatus.mErrLength[i]);
      }
      //gDoOnce = 1;
    }
  }
  if(lErrStatus.mLineErr & 0x8888)
  {
    lError = true;
    printf("Error: ***** CSI Line Count Error in frame %d\n", arFrameNumber);
    for(uint32_t i = 0; i < CSI_CHNL_CNT; i++)
    {
      printf("VC %u: error line %u \n", i, lErrStatus.mErrLine[i]);
    }
  }
  if(lError)
  {
    lCsiErrorCount++;
  }
}

//***************************************************************************

void CcmCscMatrix(float_t* aCcmCscMatrix, float_t* aCcmMatrix, float_t* aCscMatrix, float_t aSaturation)
{
  int i, j, k;

  if((!aCcmCscMatrix) | (!aCcmMatrix) || (!aCscMatrix))
  {
    printf(
        "Error during Matrix combination: one matrix has no array given\n"
        "result: %p, CCM: %p, CSC: %p\n",
        (void*)aCcmCscMatrix, (void*)aCcmMatrix, (void*)aCscMatrix);
    return;
  }

  // for all output matrix elements
  for(i = 0; i < 3; i++)
  { // line
    for(j = 0; j < 3; j++)
    { // column

      // compute matrix element
      *aCcmCscMatrix = 0;
      for(k = 0; k < 3; k++)
      {
        *aCcmCscMatrix += aCcmMatrix[k * 3 + j] * aCscMatrix[i * 3 + k];
      }
      if(i)
        *aCcmCscMatrix *= aSaturation; // only for U and V
      aCcmCscMatrix++;
    }
  }
  return;
}

//***************************************************************************

void ExtractDisplay(void* appInVirtual, void* appOutVirtual)
{
  uint32_t lStartTime, lEndTime;
  lStartTime = lEndTime = 0;
  if(gKbLog == 2)
  {
    GETTIME(&lStartTime);
  }

  //  gpFrameFbs = appOutVirtual[gFbsBufferNr];

  // copy buffer to display
  {
    uint8_t* lpSrc  = (uint8_t*)appInVirtual;
    uint8_t* lpDest = (uint8_t*)appOutVirtual;
    //uint32_t y;

    OAL_MemoryInvalidate(appInVirtual, OUT_PIXELS * 2 * OUT_LINES * ISP_CHNL_CNT);

    lpSrc += gKbXoffset * ISP_CHNL_CNT;

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
      if(gKbYuvToRgb)
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
        lpSrcUV += OUT_PIXELS * ISP_CHNL_CNT;
      lpDest += DISPLAY_PIXELS * DCU_CHNL_CNT;
    } // for all lines

#endif
  } // copy buffer to display

  OAL_MemoryFlush(appOutVirtual, DISPLAY_PIXELS * DCU_CHNL_CNT * DISPLAY_LINES);

  if(gKbLog == 2)
  {
    GETTIME(&lEndTime);
    gTimeCSC += lEndTime - lStartTime;
  }
}

//***************************************************************************

uint8_t EqualizeHistogram(uint16_t aLutOffset)
{
  uint32_t lTotal;
  uint32_t lNull;
  int32_t* lpData;
  uint32_t lNextSum;
  int32_t  lVal0, lVal1;
  //float_t  lfTotal;
  float_t  lfThisTarget;
  float_t  lfNextTarget;
  float_t  lfSlopeTarget;
  uint16_t lLutCnt;
  uint16_t lHistCnt;
  uint8_t  lError      = LIB_FAILURE;
  uint8_t  lLutSegSize = OUTLUT_SIZE / 256;
  uint8_t  lSegStep;

  SEQ_HistogramGet(&gCompHist);

  lpData = (int32_t*)gCompHist.mpData;
  lNull  = lpData[0];
  lTotal = 0;
  for(uint32_t i = 0; i < 256; i++)
  {
    lTotal += lpData[i];
  }

    // update LUT
    // LUTA
#ifdef __STANDALONE__
  volatile struct IPUS_tag* lpOutLut0 = &(IPUS_OUTLUT0);
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
  volatile struct IPUS_tag* lpOutLut1 = &(IPUS_OUTLUT1);
#endif
  lpOutLut0->HOST_LUTA.R = aLutOffset;
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
  lpOutLut1->HOST_LUTA.R = aLutOffset;
#endif

#else // ifdef __STANDALONE__
  seq_setReg(OUTLUT_ENGINE0, 1, 0x78, aLutOffset);
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
  seq_setReg(OUTLUT_ENGINE1, 1, 0x78, aLutOffset);
#endif
#endif // ifndef __STANDALONE__

  // check if single camera
  if(lTotal != RGB2YUV_PIX * (RGB2YUV_LINES2_1))
  { // sth why -1??
    printf("Error in Compression Histogram total=%d (should be %d)\n", lTotal, RGB2YUV_PIX * (RGB2YUV_LINES2_1));
    return lError;
  }

  // check if single camera
  if(lNull >= (RGB2YUV_PIX * (RGB2YUV_LINES2_1)-RGB2YUV_PIX * (RGB2YUV_LINES2_1) / 4 - 1000))
  {
    lpData[0] -= RGB2YUV_PIX * (RGB2YUV_LINES2_1)-RGB2YUV_PIX * (RGB2YUV_LINES2_1) / 4;
    int i = 0;
    while(lpData[i] < 0)
    {
      lpData[i + 1] += lpData[i];
      lpData[i] = 0;
      i++;
    }
    lTotal -= RGB2YUV_PIX * (RGB2YUV_LINES2_1)-RGB2YUV_PIX * (RGB2YUV_LINES2_1) / 4;
  }

  lNextSum     = 0;
  lfThisTarget = lfNextTarget = 0.0;
  lLutCnt                     = 0;
  lHistCnt                    = 0;

  while(lLutCnt < OUTLUT_SIZE)
  {
    lfThisTarget = lfNextTarget;
    lNextSum += lpData[lHistCnt++];

    lfNextTarget = 65535.0 * ((float)lNextSum) / ((float)lTotal);

    lfSlopeTarget = (lfNextTarget - lfThisTarget) / (float)lLutSegSize;

    lSegStep = 0;
    while(lSegStep < lLutSegSize)
    {
      // 512 means factor of 1!

      if(lLutCnt)
      {
        lVal0 = (int32_t)(OUTLUT_SCALE * (lfThisTarget + (float)lSegStep * lfSlopeTarget) /
                          (float)(lLutCnt << (16 - OUTLUT_BITS)));
      }
      else
      {
        lVal0 = 256;
      }
      if(lVal0 > 0xffff)
        lVal0 = 0xffff;
      if(lVal0 < 0)
        printf("Lut[%d]=%d\n", lLutCnt, lVal0);
      lLutCnt++;
      lSegStep++;
      lVal1 = (uint32_t)(OUTLUT_SCALE * (lfThisTarget + (float)lSegStep * lfSlopeTarget) /
                         // OUTLUT_SCALE);
                         (float)(lLutCnt << (16 - OUTLUT_BITS)));
      if(lVal1 > 0xffff)
        lVal1 = 0xffff;
      if(lVal1 < 0)
        printf("Lut[%d]=%d\n", lLutCnt, lVal1);
      lLutCnt++;
      lSegStep++;
      //printf("outlut4k[%d]=%d\n"
      //       "outlut4k[%d]=%d\n",
      //       lutcnt-2, val0,
      //       lutcnt-1, val1
      //       );
      // LUTD (IPUS)

      // write first two values
#if defined(__STANDALONE__)
      lpOutLut0->HOST_LUTD.R = lVal0 | (lVal1 << 16);
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
      lpOutLut1->HOST_LUTD.R = lVal0 | (lVal1 << 16);
#endif
#else // ifdef __STANDALONE__
      seq_setReg(OUTLUT_ENGINE0, 1, 0x7C, lVal0 | (lVal1 << 16));
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
      seq_setReg(OUTLUT_ENGINE1, 1, 0x7C, lVal0 | (lVal1 << 16));
#endif
#endif // ifndef __STANDALONE__
    }
  } // while lutcnt < OUTLUT_SIZE
  lError = LIB_SUCCESS;

  // lutoffset to gpr15
  setRegCoherent(OUTLUT_ENGINE0, 0, 0x7f, aLutOffset);
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
  setRegCoherent(OUTLUT_ENGINE1, 0, 0x7f, aLutOffset);
#endif

  return lError;
}

//***************************************************************************

int32_t FrameSave()
{
  int32_t lRet = 0;
  if(gKbMode == 's')
  {
    if(gpFrameFbs != NULL)
    {
#ifndef __STANDALONE__
      FILE* lpSavefile = 0;
      char  lFileName[256];

      if(gExpMode != 255)
      {
        snprintf(lFileName, 255, "ov10640_quad_new_%d.%s", gExpMode, DCU_OUT_FILE_EXT);
      }
      else
      {
        snprintf(lFileName, 255, "ov10640_quad_new_%s.%s", gExpName[gKbExpSel], DCU_OUT_FILE_EXT);
      }
      lpSavefile = fopen(lFileName, "w+");
      if(lpSavefile && gpFrameFbs)
      {
#if(ISP_OUTPUT == ISP_OUTPUT_RGB) || (ISP_OUTPUT == ISP_OUTPUT_YUV420)
        fprintf(lpSavefile,
                "P6\n"
                "#ExpMode %d, L: t=%d CG=%d AG=%d DG=%d - S t=%d CG=%d AG=%d DG=%d - VS t=%d CG=%d AG=%d DG=%d\n"
                "%dx%d\n255\n",
                gExpMode, gOVExposure[0].mExposureTime[L], gOVExposure[0].mConversionGain[L],
                gOVExposure[0].mAnalogGain[L], gOVExposure[0].mDigitalGain[L], gOVExposure[0].mExposureTime[S],
                gOVExposure[0].mConversionGain[S], gOVExposure[0].mAnalogGain[S], gOVExposure[0].mDigitalGain[S],
                gOVExposure[0].mExposureTime[VS], gOVExposure[0].mConversionGain[VS], gOVExposure[0].mAnalogGain[VS],
                gOVExposure[0].mDigitalGain[VS], DISPLAY_PIXELS, DISPLAY_LINES);
#endif
        fwrite((void*)(uintptr_t)gpFrameFbs, DISPLAY_PIXELS * DCU_CHNL_CNT, DISPLAY_LINES, lpSavefile);
        printf("saved output images as %s\n", lFileName);
        fclose(lpSavefile);
      } // if file opened
      else
      {
        lRet = -1;
      } // else from if file opened

      if(gKbMeasure)
      {
        gKbMode = 'm';
      }
      else
      {
        gKbMode = 0;
      }

#else // ifndef __STANDALONE__

      // copy to debugger readable buffer
      mymemcpy(gpSaveISP, gpFrameFbs, DISPLAY_PIXELS * DISPLAY_LINES * DCU_CHNL_CNT);

      printf(
#if(ISP_OUTPUT == ISP_OUTPUT_RGBY)
          "data.image eAHB:0x%x %d. %d. /RGBX888LE /Stride %d.\n",
#elif(ISP_OUTPUT == ISP_OUTPUT_RGB)
          "data.image eAHB:0x%x %d. %d. /RGB888LE /Stride %d.\n",
#elif(ISP_OUTPUT == ISP_OUTPUT_YUV420)
          "data.image eAHB:0x%x %d. %d. /RGB888LE /Stride %d.\n",
#endif
          gpSaveISP, DISPLAY_PIXELS, DISPLAY_LINES, DISPLAY_PIXELS * DCU_CHNL_CNT);
      printf(
#if(ISP_OUTPUT == ISP_OUTPUT_RGBY)
          "data.save.binary image.rgba eAHB:0x%x++(%d.)\n",
#elif(ISP_OUTPUT == ISP_OUTPUT_RGB)
          "data.save.binary image.rgb eAHB:0x%x++(%d.)\n",
#elif(ISP_OUTPUT == ISP_OUTPUT_YUV420)
          "data.save.binary image.rgb eAHB:0x%x++(%d.)\n",
#endif
          gpSaveISP, DISPLAY_PIXELS * DISPLAY_LINES * DCU_CHNL_CNT);
#endif // __STANDALONE__

    } // if image available
    else
    {
      printf("output image NULL\n");
    } // else from if image available
  }   // if save output image

#if(CAM_SAVE == 1)
#ifndef __STANDALONE__
  if(gKbMode == 'S')
  {
    if(gpFrameFbs != NULL)
    {
      FILE* lpSavefile = 0;
      char  lFileName[256];

      if(gExpMode != 255)
      {
        snprintf(lFileName, 255, CAM_OUT_FILE "_%d.%s", gExpMode, CAM_OUT_FILE_EXT);
      }
      else
      {
        snprintf(lFileName, 255, CAM_OUT_FILE "_%s.%s", gExpName[gKbExpSel], CAM_OUT_FILE_EXT);
      }

      lpSavefile = fopen(lFileName, "w+");

      if(!lpSavefile)
      {
        printf("Error: could not open save file %s\n", lFileName);
        return -1;
      }

      int            j;
      unsigned char* data;

      data = (unsigned char*)gFrameCAM.data;

      OAL_MemoryInvalidate(data, CAM_SAVE_STRIDE16 * CAM_LINES);

#if defined(CAM_SAVE_PGM)
      fprintf(lpSavefile, "P5\n#Ov10640 quad raw image 12 bit");
      fprintf(lpSavefile,
              "#L: t=%d CG=%d AG=%d DG=%d - "
              "S: t=%d CG=%d AG=%d DG=%d - "
              "VS t=%d CG=%d AG=%d DG=%d\n"
              "%dx%d\n65535\n",
              gOVExposure[0].mExposureTime[L], gOVExposure[0].mConversionGain[L], gOVExposure[0].mAnalogGain[L],
              gOVExposure[0].mDigitalGain[L], gOVExposure[0].mExposureTime[S], gOVExposure[0].mConversionGain[S],
              gOVExposure[0].mAnalogGain[S], gOVExposure[0].mDigitalGain[S], gOVExposure[0].mExposureTime[VS],
              gOVExposure[0].mConversionGain[VS], gOVExposure[0].mAnalogGain[VS], gOVExposure[0].mDigitalGain[VS],
              CAM_SAVE_PIXELS, HDR_LINES);

#endif

      for(j = 0; j < HDR_LINES; j++)
      {
        //memset(data,(255-(j>>1)) & 0xff,CAM_INPUT_STRIDE);
        fwrite(data, CAM_SAVE_PIXELS * 2, 1, lpSavefile);
        data = data + CAM_SAVE_STRIDE16;
      }
      printf("saved images as %s\n", lFileName);
      //printf("%dx%d %d \n",CAM_INPUT_STRIDE,NUM_CAM_LINES,CAM_INPUT_STRIDE);
      fclose(lpSavefile);
    } // if image available
    else
    {
      printf("input image NULL\n");
    } // else from if image available

    if(gKbMeasure)
    {
      gKbMode = 'm';
    }
    else
    {
      gKbMode = 0;
    }
  } // if save input image

#else  // #ifndef __STANDALONE__
    // not support
#endif // #ifndef __STANDALONE__
#endif // if (CAM_SAVE==1)

  return lRet;
} // FrameSave()

//***************************************************************************

void KeyboardInputProcess(void)
{
  uint8_t lVal;
  // *** get keyboard input ***
  if(gKbMode != 'm')
  {
    gKbKey = GetChar();
    //printf("char %c\n", gKbKey);
    if(gKbKey)
    {
      KbAction(gKbKey);
    }
    gKbKey = 0;
  }
  else
  {
    gKbMeasure++;
    switch(gKbMeasure)
    {
      case 1:
        gKbEdgeMaxA1 = gKbEdgeMaxA;
        gKbEdgeMaxA  = 0; // turn off edge enhancement to reduce noise
        gKbAEC       = 0;
        // Turn off the AEC of the camera
        CAM_RegRead(CSI_IDX_CAM, 0x30fa, lVal);
        lVal &= 0xbf;
        CAM_RegWrite(CSI_IDX_CAM, 0x30fa, lVal);
        gKbAWB   = 0;
        gKbMode  = 0;
        gExpMode = 0;
        KbAction('M');
        gKbMode = 'm';
        break;
      case 5:
        KbAction('s');
        gKbMode = 'm';
        break;
      case 10:
        KbAction('S');
        gKbMode = 'm';
        break;
      case 15:
        gExpMode = 255;
        gKbMode  = 'K';
        gOutLut  = 1;
        KbAction('1');
        gKbMode = 'm';
        break;
      case 20:
        KbAction('s');
        gKbMode = 'm';
        break;
      case 25:
        KbAction('S');
        gKbMode = 'm';
        break;
      case 30:
        gExpMode = 255;
        gKbMode  = 'K';
        KbAction('2');
        gKbMode = 'm';
        break;
      case 35:
        KbAction('s');
        gKbMode = 'm';
        break;
      case 40:
        KbAction('S');
        gKbMode = 'm';
        break;
      case 45:
        gExpMode = 255;
        gKbMode  = 'K';
        KbAction('3');
        gKbMode = 'm';
        break;
      case 50:
        KbAction('s');
        gKbMode = 'm';
        break;
      case 55:
        KbAction('S');
        gKbMode = 'm';
        break;
      case 60:
        printf("Measurment stopped\n");
        gKbMode  = 0;
        gExpMode = 0;
        KbAction('M');
        gKbMode     = 0;
        gKbMeasure  = 0;
        gKbEdgeMaxA = gKbEdgeMaxA1; // restore edge enhancement
        break;
    }
  }
} // KeyboardInputProcess()

//***************************************************************************

void IspConfig(void)
{
  // *** lut input kernel ***
  int     i;
  uint8_t val8;
  SetInLut(1);
  SetInLut(1);
  seq_setReg(INLUT_ENGINE, 0, 0x7e, 0x4); // GPR14 (IPUS)

  OV10640_Exposure_t lExp;
  memset(&lExp, 0, sizeof(lExp));

  lExp.mCsiIdx = CSI_IDX_CAM;

  for(i = 0; i < 4; i++)
  {
    gChGainVal[i]          = CHGAINFACTOR * gChGainTab[gKbChgain][i];
    lExp.mpLongChnlGain[i] = lExp.mpShortChnlGain[i] = lExp.mpVeryShortChnlGain[i] = gChGainTab[gKbChgain][i];
  }
#ifdef OV10640
  OV10640_Geometry_t lGeom;
  lGeom.mCsiIdx = CSI_IDX_CAM;

  lGeom.mExpMode = OV10640_EXP_12COM;
  lGeom.mExpMode |= OV10640_EXP_AUTO;
#if(EMBEDDED_DATA == 1)
  lGeom.mEmbd = OV10640_EMBD_REGS | OV10640_EMBD_STAT;
#endif

  OV10640_GeometrySet(lGeom);
#else
  MAXIM_CsiEnable(CSI_IDX_CAM);
  MAXIM_OV10640_ExposureSet(lExp);
  MAXIM_CsiDisable(CSI_IDX_CAM);
#endif //OV10640
  // Histogram Engine
  gCompHist.mIpuIdx = SEQ_LUT_HIST;
  SEQ_HistogramEnable(SEQ_LUT_HIST);
  SEQ_StatEnable();

  // *** HDR configuration
  SetHDREngine(HDR_IPUSIDX); // tell exposure control which engine to change
  //printf("HDR Linegap %d (%d - %d)\n",
  //       HDR_LINEGAP,
  //       IN_PIXELS1>>1,
  //       DEBAYER_INPIX>>2);

  // do last two pixels manually
  seq_setReg(HDR_ENGINE, 0, 0x70, (IN_PIXELS1 >> 1) - 2); // (CAM_PIXEL1 -2)
  //seq_setReg(HDR_ENGINE, 0, 0x71, HDR_LINEGAP);  // two input pixels

  // initial combination values
  gKbMode = 'M';
  gExpMode--;
  KbAction('M');
  seq_setReg(HDR_ENGINE, 0, 0x7e, 0x0);                 // GPR14 (IPUS)
  seq_setReg(HDR_ENGINE, 0, 0x7f, gHDRShift[gExpMode]); // GPR15 (IPUS)

  // *** debayer select start line
  // start with even line
  seq_setReg(DEBAYER_ENGINE0, 0, 0x70, DEBAYER_OUTPIX >> 2); // line length
  seq_setReg(DEBAYER_ENGINE1, 0, 0x70, DEBAYER_OUTPIX >> 2); // line length

  // *** RGB2Y Engine
  CcmCscMatrix(gCcmCscMatrix, gKbCcmMatrix[gKbCCM], gRgb2YuvMatrix, gKbSaturation);
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
  seq_setReg(DENOISEY_ENGINE0, 0, DENOISEY_GPR | 0x7, gKbYDenoise); // threshold
  seq_setReg(DENOISEY_ENGINE1, 0, DENOISEY_GPR | 0x7, gKbYDenoise); // threshold
  seq_setReg(DENOISEY_ENGINE0, 0, DENOISEY_GPR | 0xc, gKbViewEdge); // selector
  seq_setReg(DENOISEY_ENGINE1, 0, DENOISEY_GPR | 0xc, gKbViewEdge); // selector

  // *** rgb2y_lut kernel ***
  seq_setReg(OUTLUT_ENGINE0, 0, 0x7e, 16 - OUTLUT_BITS); // shift bits
  seq_setReg(OUTLUT_ENGINE0, 0, 0x7f, gOutLutOffset);    // 0 or 2048
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
  seq_setReg(OUTLUT_ENGINE1, 0, 0x7e, 16 - OUTLUT_BITS); // shift bits
  seq_setReg(OUTLUT_ENGINE1, 0, 0x7f, gOutLutOffset);    // 0 or 2048
#endif
  if(SetOutLut(gOldOutLut, gOutLutOffset) == LIB_SUCCESS)
  {
    gOutLutOffset ^= 2048;
  }

  printf("Initialize processing parameters...done");
#ifdef OV10640
  OV10640_ExposureInit(gRatioLSExp);
  OV10640_ExposureSet(gOVExposure[0]);
  OV10640_ExposureAllExpSet(gOVExposure[0], ExpTime_Long);
  OV10640_ExposureAllExpSet(gOVExposure[0], ExpTime_Short);
  OV10640_ExposureAllExpSet(gOVExposure[0], ExpTime_VShort);
  KbAction('I');
  // Set-up mask for statistics
  CAM_RegRead(CSI_IDX_CAM, 0x30fa, val8);
  val8 = (val8 & 0xF0) + 0x2; //Mask to so Statistics on Green_b
  CAM_RegWrite(CSI_IDX_CAM, 0x30fa, val8);
  SetInLut(gInLut);
  OV10640_Exposure_Print(CSI_IDX_CAM, SourceCamera);

  // Turn off automatic AEC
  if(gKbAEC != 2)
  {
    CAM_RegRead(CSI_IDX_CAM, 0x30fa, val8);
    val8 = (val8 & 0xBF); // turn off AEC of the camera
    CAM_RegWrite(CSI_IDX_CAM, 0x30fa, val8);
    printf("Camera AEC is off \n");
  }
  printf("Initialize processing parameters...done");
#endif

} // IspConfig()

//***************************************************************************

uint8_t SetInLut(uint8_t aLut)
{
  uint32_t val0   = 0;
  uint32_t val1   = 0;
  uint16_t lutcnt = 0;
  uint8_t  lError = LIB_FAILURE;

#if defined(__STANDALONE__)
  volatile struct IPUS_tag* lpInLut = &(IPUS_INLUT);
#endif

#define INLUT 1.0
#define INLUT_SIZE 4096
#define INLUT_OFFSET 0
  // LUTA
#if defined(__STANDALONE__)
  lpInLut->HOST_LUTA.R = INLUT_OFFSET;
#else
  seq_setReg(INLUT_ENGINE, 1, 0x78, (INLUT_OFFSET));
#endif

  if(aLut == 0)
  {
#define FIX_AEC (1.5)
    while(lutcnt < INLUT_SIZE)
    {
      val0 = (((uint32_t)(FIX_AEC * lutcnt++)) << (gKB_shift + 4));
      val1 = (((uint32_t)(FIX_AEC * lutcnt++)) << (gKB_shift + 4));
      //printf("inlut4k[%d]=%d\n"
      //       "inlut4k[%d]=%d\n",
      //       lutcnt-2, val       & 0xffff,
      //       lutcnt-1, (val>>16) & 0xffff
      //       );

      // LUTD (IPUS)
      if(val0 > 0xffff)
        val0 = 0xffff;
      if(val1 > 0xffff)
        val1 = 0xffff;
#if defined(__STANDALONE__)
      lpInLut->HOST_LUTD.R = val0 | (val1 << 16);
#else
      seq_setReg(INLUT_ENGINE, 1, 0x7C, val0 | (val1 << 16));
#endif

    } // while lutcnt < INLUT_SIZE
    printf("input  lut: copy\n");
    lError = LIB_SUCCESS;
  }

  else if(aLut == 1)
  {
#define DECOMPRESS_SCALE 1.0
    uint16_t lutcnt = 0;
    while(lutcnt < 512)
    {
      val0 = (uint32_t)((float)((lutcnt++) >> 2) * DECOMPRESS_SCALE);
      val1 = (uint32_t)((float)((lutcnt++) >> 2) * DECOMPRESS_SCALE);
#if defined(__STANDALONE__)
      lpInLut->HOST_LUTD.R = val0 | (val1 << 16);
#else
      seq_setReg(INLUT_ENGINE, 1, 0x7C, val0 | (val1 << 16));
#endif
    } // while lutcnt < 384
    printf(
        "inlut4k[%d]=%d\n"
        "inlut4k[%d]=%d\n",
        lutcnt - 2, val0 & 0xffff, lutcnt - 1, val1 & 0xffff);
    while(lutcnt < 1408)
    {
      val0 = (uint32_t)((float)((lutcnt++) - 384) * DECOMPRESS_SCALE);
      val1 = (uint32_t)((float)((lutcnt++) - 384) * DECOMPRESS_SCALE);
      // LUTD (IPUS)
#if defined(__STANDALONE__)
      lpInLut->HOST_LUTD.R = val0 | (val1 << 16);
#else
      seq_setReg(INLUT_ENGINE, 1, 0x7C, val0 | (val1 << 16));
#endif
    } // while lutcnt < 1152
    printf(
        "inlut4k[%d]=%d\n"
        "inlut4k[%d]=%d\n",
        lutcnt - 2, val0 & 0xffff, lutcnt - 1, val1 & 0xffff);
    while(lutcnt < 2176)
    {
      val0 = (uint32_t)((float)(((lutcnt++) - 1152) << 2) * DECOMPRESS_SCALE);
      val1 = (uint32_t)((float)(((lutcnt++) - 1152) << 2) * DECOMPRESS_SCALE);
      // LUTD (IPUS)
#if defined(__STANDALONE__)
      lpInLut->HOST_LUTD.R = val0 | (val1 << 16);
#else
      seq_setReg(INLUT_ENGINE, 1, 0x7C, val0 | (val1 << 16));
#endif
    } // while lutcnt < 2048
    printf(
        "inlut4k[%d]=%d\n"
        "inlut4k[%d]=%d\n",
        lutcnt - 2, val0 & 0xffff, lutcnt - 1, val1 & 0xffff);
    while(lutcnt < INLUT_SIZE)
    {
      val0 = (uint32_t)((float)(((lutcnt++) - 2048) << 5) * DECOMPRESS_SCALE);
      val1 = (uint32_t)((float)(((lutcnt++) - 2048) << 5) * DECOMPRESS_SCALE);
      // LUTD (IPUS)
#if defined(__STANDALONE__)
      lpInLut->HOST_LUTD.R = val0 | (val1 << 16);
#else
      seq_setReg(INLUT_ENGINE, 1, 0x7C, val0 | (val1 << 16));
#endif
    } // while lutcnt < INLUT_SIZE
    printf(
        "inlut4k[%d]=%d\n"
        "inlut4k[%d]=%d\n",
        lutcnt - 2, val0 & 0xffff, lutcnt - 1, val1 & 0xffff);
    printf("input  lut: decompress 12 to 16 linear\n");
    lError = LIB_SUCCESS;
  }

  else if(aLut == 2)
  {
    while(lutcnt < INLUT_SIZE)
    {
      val0 = (uint32_t)(15453.32 * logf((lutcnt++) * 0.016704 + 1));
      val1 = (uint32_t)(15453.32 * logf((lutcnt++) * 0.016704 + 1));

      // LUTD (IPUS)
      if(val0 > 0xffff)
        val0 = 0xffff;
      if(val1 > 0xffff)
        val1 = 0xffff;
#if defined(__STANDALONE__)
      lpInLut->HOST_LUTD.R = val0 | (val1 << 16);
#else
      seq_setReg(INLUT_ENGINE, 1, 0x7C, val0 | (val1 << 16));
#endif
    } // while lutcnt < INLUT_SIZE
    printf("input  lut: log 12to8\n");
    lError = LIB_SUCCESS;
  }

  else if(aLut == 3)
  {
    uint32_t val3;
    uint32_t val16;
    uint16_t lutcnt = 0;
#define FACT1 12
#define FACT2 64
#define FACT3 256
#define ALPHA2 5
#define ALPHA3 5
    while(lutcnt < INLUT_SIZE)
    {
      val3  = (lutcnt * FACT3);
      val16 = (val3 > 0xffff) ? 0xffff / ALPHA3 : val3 / ALPHA3;
      val3  = (lutcnt * FACT2);
      val16 += (val3 > 0xffff) ? 0xffff / ALPHA3 : val3 / ALPHA3;
      val16 += (lutcnt * FACT1);
      val0 = (val16 > 0xffff) ? 0xffff : val16;
      lutcnt++;
      val3  = (lutcnt * FACT3);
      val16 = (val3 > 0xffff) ? 0xffff / ALPHA3 : val3 / ALPHA3;
      val3  = (lutcnt * FACT2);
      val16 += (val3 > 0xffff) ? 0xffff / ALPHA3 : val3 / ALPHA3;
      val16 += (lutcnt * FACT1);
      val1 = (val16 > 0xffff) ? 0xffff : val16;
      lutcnt++;
      // LUTD (IPUS)
#if defined(__STANDALONE__)
      lpInLut->HOST_LUTD.R = val0 | (val1 << 16);
#else
      seq_setReg(INLUT_ENGINE, 1, 0x7C, val0 | (val1 << 16));
#endif
    } // while lutcnt < INLUT_SIZE
    printf(
        " input lut: remap\n"
        "    (val = "
        "clipped(in*%f + "
        "clipped(in*%f)/%d + "
        "clipped(in*%f)/%d)\n",
        FACT1 / 16.0, FACT2 / 16.0, ALPHA2, FACT3 / 16.0, ALPHA3);
    lError = LIB_SUCCESS;
  }
  return lError;
}

//***************************************************************************

uint8_t SetOutLut(uint8_t aLut, uint16_t aLutOffset)
{
  uint32_t val0;
  uint32_t val1;
  uint16_t lutcnt = 0;
  uint8_t  lError = LIB_FAILURE;

#ifdef __STANDALONE__
  volatile struct IPUS_tag* lpOutLut0 = &(IPUS_OUTLUT0);
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
  volatile struct IPUS_tag* lpOutLut1 = &(IPUS_OUTLUT1);
#endif
#endif

  // LUTA
  seq_setReg(OUTLUT_ENGINE0, 1, 0x78, aLutOffset);
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
  seq_setReg(OUTLUT_ENGINE1, 1, 0x78, aLutOffset);
#endif

  if(aLut == 0)
  {
    val0 = (uint16_t(OUTLUT_SCALE)); //((lutcnt++)<<4);
    val1 = (uint16_t(OUTLUT_SCALE)); //((lutcnt++)<<4);
    while(lutcnt < OUTLUT_SIZE)
    {
      lutcnt += 2;

      // LUTD (IPUS)
      seq_setReg(OUTLUT_ENGINE0, 1, 0x7C, val0 | (val1 << 16));
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
      seq_setReg(OUTLUT_ENGINE1, 1, 0x7C, val0 | (val1 << 16));
#endif
    } // while lutcnt < OUTLUT_SIZE
    printf("Output Lut: copy\n");
    lError = LIB_SUCCESS;
  }

  else if(aLut == 1)
  {
#define GAMMA 0.8
#define GAMMA_SCALE (0xFFF0)
    while(lutcnt < OUTLUT_SIZE)
    {
      if(!lutcnt)
        val0 = 0;
      else
      {
        val0 = (uint32_t)(OUTLUT_SCALE * // 8.8 factors
                          ((float_t)GAMMA_SCALE) *
                          pow(((float_t)lutcnt) / ((float_t)(OUTLUT_SIZE - 1)),
                              GAMMA) /
                          (float_t(lutcnt << (16 - OUTLUT_BITS))) // 15 bit input values
        );
      }
      lutcnt++;
      val1 = (uint32_t)(OUTLUT_SCALE * // 8.8 factors
                        ((float_t)GAMMA_SCALE) *
                        pow(((float_t)lutcnt) / ((float_t)(OUTLUT_SIZE - 1)),
                            GAMMA) /
                        (float_t(lutcnt << (16 - OUTLUT_BITS))) // 15 bit input values
      );
      lutcnt++;
      // LUTD (IPUS)
      if(val0 > 0xffff)
        val0 = 0xffff;
      if(val1 > 0xffff)
        val1 = 0xffff;

      seq_setReg(OUTLUT_ENGINE0, 1, 0x7C, val0 | (val1 << 16));
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
      seq_setReg(OUTLUT_ENGINE1, 1, 0x7C, val0 | (val1 << 16));
#endif
    } // while lutcnt < OUTLUT_SIZE
    printf("Output Lut: gamma %f\n", GAMMA);
    lError = LIB_SUCCESS;
  }
  else if(aLut == 2)
  {
#define LOG_A_12TO8 15453.32
#define LOG_B_12TO8 0.016704
    while(lutcnt < OUTLUT_SIZE)
    {
      if(!lutcnt)
      {
        val0 = 0;
      }
      else
      {
        val0 = (uint32_t)(OUTLUT_SCALE * LOG_A_12TO8 * logf((lutcnt)*LOG_B_12TO8 + 1) /
                          ((float_t)(lutcnt << (16 - OUTLUT_BITS))));
      }
      lutcnt++;
      val1 = (uint32_t)(OUTLUT_SCALE * LOG_A_12TO8 * logf((lutcnt)*LOG_B_12TO8 + 1) /
                        ((float_t)(lutcnt << (16 - OUTLUT_BITS))));
      lutcnt++;
      // LUTD (IPUS)
      if(val0 > 0xffff)
      {
        val0 = 0xffff;
      }
      if(val1 > 0xffff)
      {
        val1 = 0xffff;
      }
      seq_setReg(OUTLUT_ENGINE0, 1, 0x7C, val0 | (val1 << 16));
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
      seq_setReg(OUTLUT_ENGINE1, 1, 0x7C, val0 | (val1 << 16));
#endif
    } // while lutcnt < OUTLUT_SIZE
    printf("Output Lut: log 12to8\n");
    lError = LIB_SUCCESS;
  }
  else if(aLut == 3)
  {
#define LOG_A_16TO8 8332.00
#define LOG_B_16TO8 0.634928
    uint16_t lutcnt = 0;
    while(lutcnt < OUTLUT_SIZE)
    {
      if(!lutcnt)
        val0 = 0x0;
      else
      {
        val0 = (uint32_t)(OUTLUT_SCALE * LOG_A_16TO8 * logf((lutcnt)*LOG_B_16TO8 + 1) /
                          ((float_t)(lutcnt << (16 - OUTLUT_BITS))));
      }
      lutcnt++;
      val1 = (uint32_t)(OUTLUT_SCALE * LOG_A_16TO8 * logf((lutcnt)*LOG_B_16TO8 + 1) /
                        ((float_t)(lutcnt << (16 - OUTLUT_BITS))));
      lutcnt++;

      // LUTD (IPUS)
      if(val0 > 0xffff)
      {
        val0 = 0xffff;
      }
      if(val1 > 0xffff)
      {
        val1 = 0xffff;
      }
      seq_setReg(OUTLUT_ENGINE0, 1, 0x7C, val0 | (val1 << 16));
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
      seq_setReg(OUTLUT_ENGINE1, 1, 0x7C, val0 | (val1 << 16));
#endif
    } // while lutcnt < OUTLUT_SIZE
    printf("Output Lut: log 16to8\n");
    lError = LIB_SUCCESS;
  }
  else if(aLut == 4)
  {
    printf("Output Lut: equalized\n");
  }
  else if(aLut == 5)
  {
    uint16_t lutcnt = 0;
    // go to previous lut
    seq_setReg(OUTLUT_ENGINE0, 1, 0x78, aLutOffset ^ 2048);
    printf("dump Output Lut:\n");
    while(lutcnt < OUTLUT_SIZE)
    {
      uint32_t val32;
      seq_getReg(OUTLUT_ENGINE0, 1, 0x7C, &val32);
      val0 = val32 & 0xffff;
      val1 = (val32 >> 16) & 0xffff;
      printf(
          "lut4k[%d]=%d\n"
          "lut4k[%d]=%d\n",
          lutcnt, val0, lutcnt + 1, val1);
      lutcnt += 2;
      // LUTD (IPUS)
    } // while lutcnt < OUTLUT_SIZE
  }

  if(lError == LIB_SUCCESS)
  {
    // lutoffset to gpr15
    setRegCoherent(OUTLUT_ENGINE0, 0, 0x7f, aLutOffset);
#if(OUTLUT_IPUS0 != OUTLUT_IPUS1)
    setRegCoherent(OUTLUT_ENGINE1, 0, 0x7f, aLutOffset);
#endif
  }
  return lError;

} // SetOutLut()

//***************************************************************************

void LogStatus(sdi_grabber* apGrabber)
{
  GETTIME(&gTimee);
  gTimed           = gTimee - gTimes;
  gTimes           = gTimee;
  gFrameIRQCnt     = gFrmCnt;
  gFrameIRQDiff    = gFrameIRQCnt - gFrameIRQCntLast;
  gFrameIRQCntLast = gFrameIRQCnt;

  if(gKbLog != 0)
  {
    if(gKbLog == 1)
    {
      printf("Frame: %7d (Isp.GetFrame:%d)\n", gFrameIRQCnt, gFrmCnt);

      printf("%d frames took %d usec (%5.2ffps)\n", gFrameIRQDiff, gTimed,
             (gFrameIRQDiff)*1000000.0 / ((float_t)gTimed));
    }
    if(gKbLog == 2)
    {
      printf("Frame: %7d (Isp.GetFrame:%d)\n", gFrameIRQCnt, gFrmCnt);

      ISP_DumpPerfCounters(IPU_PROF_FRAMES);
      SEQ_IpuPerformanceInfoRequest(IPU_PROF_FRAMES);
      if(gTimeCSC)
      {
        printf("YUV2RGB   took %9.2f      ms/frame; %6.2ffps\n", (float_t)gTimeCSC / (float_t)gFrameIRQDiff / 1000.0,
               1000000.0 * (float_t)gFrameIRQDiff / (float_t)gTimeCSC);
        gTimeCSC = 0;
      }
      if(gTimed && gTimeA53)
      {
        printf("A53 total took %9.2f      ms/frame; %6.2ffps\n",
               (float_t)(gTimed - gTimeA53) / (float_t)gFrameIRQDiff / 1000.0,
               1000000.0 * (float_t)gFrameIRQDiff / (float_t)(gTimed - gTimeA53));
        gTimeA53 = 0;
      }
      if(gTimed)
      {
        printf("A53 + Pop took %9.2f      ms/frame; %6.2ffps\n", (float_t)gTimed / (float_t)gFrameIRQDiff / 1000.0,
               1000000.0 * (float_t)gFrameIRQDiff / (float_t)gTimed);
        gTimeA53 = 0;
      }

    } // (if gKbLog ==2)
    if(gKbLog == 5)
    {
      SEQ_Head_Ptr_t* lpFetchedGraph = NULL;
      SEQ_Buf_t**     lppFetchedBufferList;

      lppFetchedBufferList = apGrabber->GraphFetch(&lpFetchedGraph);
      ISP_PrintGraphStat(lppFetchedBufferList);
    } // (if gKbLog ==5)
    if(gKbLog == 7)
    {
      if(gTimeApex)
      {
        printf("APEX      took %9.2f      ms/frame; %6.2ffps\n", (float_t)gTimeApex / (float_t)gFrameIRQDiff / 1000.0,
               1000000.0 * (float_t)gFrameIRQDiff / (float_t)gTimeApex);
        gTimeApex = 0;
      }
    } // (if gKbLog ==7)
  }   // (if gKbLog !=0)
} // LogStatus()

// ============================================================================

int main(int argc, char** argv)
{
  std::string helpMsg = std::string("Demonstrates ISP usage with camera Omnivision_10640.\n\tUsage: ") +
                        COMMON_ExtractProgramName(argv[0]);

  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp < 0)
  { // print help message even if no help option is provided by the user
    printf("%s", helpMsg.c_str());
  }

  LIB_RESULT lRet = LIB_SUCCESS;
  LIB_RESULT lRes = LIB_SUCCESS;

  // DDR display output buffer pointers
  void*    lppGPUVirtual[DDR_OUT_BUFFER_CNT];  ///< virtual buffer pointers
  uint32_t lpGPUPhysical[DDR_OUT_BUFFER_CNT];  ///< physical buffer addresses
  void*    lppApexVirtual[DDR_OUT_BUFFER_CNT]; ///< virtual buffer pointers
  uint32_t lpApexPhysical[DDR_OUT_BUFFER_CNT]; ///< physical buffer addresses

  printf(
      "starting demo: isp_ov10640_default"
      "\n"
      "\t\tCSI  %d\n"
      "\t\tCAMs %d\n",
      CSI_IDX_CAM, CAMS);

  OAL_Initialize();

#ifdef __STANDALONE__
  //*** Init DCU Output ***

  // set CLUT for gray scale... done in FrameOutputDCU
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
  if(DdrBuffersAlloc(lppGPUVirtual, lpGPUPhysical, DISPLAY_PIXELS * DISPLAY_LINES * DCU_CHNL_CNT, DDR_OUT_BUFFER_CNT) !=
     0)
  {
    printf("Failed to allocated GPU output buffers (size %dbytes)\n", DISPLAY_PIXELS * DISPLAY_LINES * DCU_CHNL_CNT);

    return (-1);
  } // if failed to allocate buffers

  //*** allocate Frame Buffer in DDR ***
  if(DdrBuffersAlloc(lppApexVirtual, lpApexPhysical, DISPLAY_PIXELS * DISPLAY_LINES * DCU_CHNL_CNT,
                     DDR_OUT_BUFFER_CNT) != 0)
  {
    printf("Failed to allocated APEX output buffers (size %dbytes)\n", DISPLAY_PIXELS * DISPLAY_LINES * DCU_CHNL_CNT);

    return (-1);
  } // if failed to allocate buffers

    // *** allocate buffer for saving output image
#ifdef __STANDALONE__
  gpSaveISP = OAL_MemoryAllocFlag(DISPLAY_PIXELS * DISPLAY_LINES * DCU_CHNL_CNT,
                                  OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_MEMORY_FLAG_CONTIGUOUS);
  if(!gpSaveISP)
    printf("Out of memory to alloc gpSaveIPS (size %dbytes)\n", OUT_PIXELS * OUT_LINES * DCU_CHNL_CNT);
  gpSaveISP = OAL_MemoryReturnAddress(gpSaveISP, ACCESS_NCH_NB);
#endif

  //
  // *** Initialize SDI ***
  //
  lRes = sdi::Initialize(0);

  SEQ_EventHandlerSet(SeqEventHandler); // set the event handler

  // *** create grabber ***
  sdi_grabber* lpGrabber = new(sdi_grabber);
  lpGrabber->ProcessSet(gpGraph, &gGraphMetadata);

  // *** prepare FDMA channels ***
  // goal is to program FDMA to merge the 2 output streams (per line) into one
  // huge DDR buffer with width = 4*1280 pix and hight = 2*720 lines
  sdi_FdmaIO* lpFdma = (sdi_FdmaIO*)lpGrabber->IoGet(SEQ_OTHRIX_FDMA);

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
  lQuadBuffArrRoi = lQuadBuffArr.Roi(0,           // X-Offset
                                     0,           // Y-Offset ??
                                     OUT_PIXELS,  // ROI X-size
                                     OUT_LINES2_0 // ROI Y-Size
  );
  lQuadBuffArrRoi.ChannelIdxSet(FDMA_IX_ISP_OUTPUT0);
  lpFdma->DdrBuffersSet(lQuadBuffArrRoi);

  // FDMA Y path 1
  lQuadBuffArrRoi = lQuadBuffArr.Roi(OUT_PIXELS,  // X-Offset
                                     0,           // Y-Offset
                                     OUT_PIXELS,  // ROI X-size
                                     OUT_LINES2_1 // ROI Y-Size
  );
  lQuadBuffArrRoi.ChannelIdxSet(FDMA_IX_ISP_OUTPUT1);
  lpFdma->DdrBuffersSet(lQuadBuffArrRoi);

  // FDMA UV path 0
  lQuadBuffArrRoi = lQuadBuffArr.Roi(0,             // X-Offset
                                     OUT_LINES2_0,  // Y-Offset
                                     OUT_PIXELS_UV, // ROI X-size
                                     OUT_LINESUV2_0 // ROI Y-Size
  );

  lQuadBuffArrRoi.mDesc.mWidthStep = OUT_PIXELS;
  lQuadBuffArrRoi.ChannelIdxSet(FDMA_IX_ISP_OUTPUT_UV0);
  lpFdma->DdrBuffersSet(lQuadBuffArrRoi);

  // FDMA channel 2 CAM_OUTPUT
  SDI_ImageDescriptor lCamFrmDesc;
  lCamFrmDesc = SDI_ImageDescriptor(CAM_SAVE_STRIDE16, // just one image
                                    CAM_LINES,
                                    GS8 // as GS16 is missing
  );
  lpFdma->DdrBufferDescSet(FDMA_IX_CAM_OUTPUT, lCamFrmDesc);

  // emmbedded regs & stats
  lCamFrmDesc = SDI_ImageDescriptor(CAM_SAVE_STRIDE16, // just one image
                                    2,
                                    GS8 // as GS16 is missing
  );
  lpFdma->DdrBufferDescSet(FDMA_IX_EMBD_REGS, lCamFrmDesc);
  lpFdma->DdrBufferDescSet(FDMA_IX_EMBD_STATS, lCamFrmDesc);

  // allocate contigous buffers for the rest of the DDR images
  if(lpFdma->DdrBuffersAlloc(DDR_OUT_BUFFER_CNT) != LIB_SUCCESS)
  {
    printf("Failed to allocated remaining FastDMA buffers\n");
    return 1;
  }

#ifdef MOBIS_OV10642
  /* Power_down pin to be turned on*/

  // AD23 - Port PC5
  SIUL2.MSCR[37].B.OBE      = 0x1; // output buffer enable
  SIUL2.MSCR[37].B.MUX_MODE = 0x0; // source selection
  SIUL2.MSCR[37].B.DSE      = 0x7; // drive strength 34 ohm
  SIUL2.MSCR[37].B.SRE      = 0x3; // High frequency slew rate
  SIUL2.GPDO[9].B.PDO_4n1   = 0;
  SIUL2.GPDO[9].B.PDO_4n1   = 1;
#endif

  // *** prestart grabber ***
  msleep(1);
  lpGrabber->PreStart(); // Sequencer FW b

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
    lpPatchListArr[0].mVal[0] = HIST_OFFSET0;
    lpPatchListArr[0].mVal[1] = HIST_OFFSET1;

    lpPatchListArr[1].mOffset = SIN_OFFS_OFFS(1); // SDMAin#1
    lpPatchListArr[1].mNumber = 2;
    lpPatchListArr[1].mVal[0] = HIST_OFFSET0;
    lpPatchListArr[1].mVal[1] = HIST_OFFSET1;

    lpPatchListArr[2].mOffset = SIN_OFFS_OFFS(2); // SDMAin#2
    lpPatchListArr[2].mNumber = 2;
    lpPatchListArr[2].mVal[0] = HIST_OFFSETVS0;
    lpPatchListArr[2].mVal[1] = HIST_OFFSETVS1;

    SEQ_PatchListSet(lpPatchListArr, PATCH_LIST_CNT);
  }
#endif // 0

#ifndef __STANDALONE__
  IniParse();
  printf("****\n");
#endif
  // *** configure camera ***
  IspConfig();

  //PWM_BaseConfig
  PWM_BaseConfig();
  // print keyboard command help
  KbAction('h');

  uint32_t lLoop;

  // fetched frame buffer storage
  SDI_Frame lFrame[FDMA_IX_END - 2];
#if(EMBEDDED_DATA == 1)
  SDI_Frame lpEmbd[2];
#endif

  SDI_Frame lNextFrame;
  uint32_t  lSkippedFrames = 0, lSkippedFrames1 = 0;
  uint32_t  lMainFdmaChannel = FDMA_IX_ISP_OUTPUT1;
  uint32_t  lPopFailed       = 0;
  uint32_t  lPopTime;

  // *** start grabbing ***
  lpGrabber->Start();

  // clear time counters for A53/Neon and APEX
  gTimeA53 = gTimeCSC = gTimeApex = 0;

  GETTIME(&gTimes);

  while(1)
  {
    uint32_t* lhist_ptr;
    float     lLCorr, lVSCorr;

    for(lLoop = 0; lLoop < LOOP_NUM; lLoop++)
    {
      // make coherent update of registers
      setRegCoherentClose();

      // wait for Neon to finished processing
      if(gGpuActive)
      {
        // *** done in any case as not in a parallel threat
        //ExtractDispaly.wait();
        // push to all channels to keep buffers in sync
        for(int lFdmaChannel = 0; lFdmaChannel < (FDMA_IX_END - 2); lFdmaChannel++)
        {
          lpGrabber->FramePush(lFrame[lFdmaChannel]);
        } // for all FDMA channels
        lpGrabber->FramePush(lpEmbd[0]);
        lpGrabber->FramePush(lpEmbd[1]);
      }

      // clear list of registers to be updated coherently
      //gCoherentRegIx=0;

      if(gKbLog == 2)
      {
        GETTIME(&lPopTime);
        gTimeA53 -= lPopTime;
      }

      // **** frame pop start
      //grab main output
      lFrame[lMainFdmaChannel] = lpGrabber->FramePop(lMainFdmaChannel);
      if(lFrame[lMainFdmaChannel].mUMat.empty())
      {
        printf("Failed to grab a frame # %u. Terminating\n", gFrmCnt);
        lPopFailed = 1;
        break;
      } // if pop failed

      if(gKbLog == 2)
      {
        GETTIME(&lPopTime);
        gTimeA53 += lPopTime;
      }

      // if if there are more buffers in the queue already
      lSkippedFrames = 0;
      lNextFrame     = lpGrabber->FramePopNonBlock(lMainFdmaChannel);
      while(lNextFrame.mUMat.empty() == false)
      {
        lpGrabber->FramePush(lFrame[lMainFdmaChannel]);
        lFrame[lMainFdmaChannel] = lNextFrame;
        lSkippedFrames++;
        lSkippedFrames1 = lSkippedFrames;
        lNextFrame      = lpGrabber->FramePopNonBlock(lMainFdmaChannel);
      }

      if(lSkippedFrames > 0)
      {
        printf("Skipped %u frames\n", lSkippedFrames);
      } // if any frame skipped

      // grap all reamining fdma channels as well
      for(uint32_t lFdmaChannel = 0; lFdmaChannel < (FDMA_IX_END - 2); lFdmaChannel++)
      {
        if(lFdmaChannel == lMainFdmaChannel)
        {
          continue;
        } // if main channel

        lFrame[lFdmaChannel] = lpGrabber->FramePopNonBlock(lFdmaChannel);
        if(lFrame[lFdmaChannel].mUMat.empty())
        {
          printf("Failed to grab a frame # %u. Terminating\n", gFrmCnt);
          lPopFailed = 1;
          break;
        } // if pop failed

        while(lSkippedFrames)
        {
          lpGrabber->FramePush(lFrame[lFdmaChannel]);
          lFrame[lFdmaChannel] = lpGrabber->FramePopNonBlock(lFdmaChannel);
          if(lFrame[lFdmaChannel].mUMat.empty() == false)
          {
            lSkippedFrames--;
          } // if pop succeeded
          else
          {
            printf("Failed to grab a frame # %u. Terminating\n", gFrmCnt);
            lPopFailed = 1;
            break;
          } // else from if pop succeeded
        }   // while
      }     // for all FDMA channels
            //**** frame pop finished

#if(EMBEDDED_DATA == 1)
      for(uint8_t i = 0; i < 2; i++)
      {
        lpEmbd[i] = lpGrabber->FramePopNonBlock(FDMA_IX_END - 2 + i);
        if(lpEmbd[i].mUMat.empty())
        {
          printf("Failed to grab embedded data image number %u\n", gFrmCnt);
          lPopFailed = 1;
          break;
        } // if get frame failed

        while(lSkippedFrames1)
        {
          lpGrabber->FramePush(lpEmbd[i]);
          lpEmbd[i] = lpGrabber->FramePopNonBlock(FDMA_IX_END - 2 + i);
          if(lpEmbd[1].mUMat.empty() == false)
          {
            lSkippedFrames1--;
          } // if pop succeeded
          else
          {
            printf("Failed to grab a frame # %u. Terminating\n", gFrmCnt);
            lPopFailed = 1;
            break;
          } // else from if pop succeeded
        }   // while
      }     // for()

      CamHistGet(GREEN_B, (uint8_t*)lpEmbd[1].mUMat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data);
      lhist_ptr = (uint32_t*)(&hist_L[GREEN_B]);
      lLCorr    = OV10640_ExposureCorrection(
          lhist_ptr, ((256.0 - gKbLtrgt) / 256.0) * 55285 /*pixel_level_correction *27640  *35935 = 55285*0.65 */, 12);

      lhist_ptr = (uint32_t*)(&hist_VS[GREEN_B]);
      lVSCorr   = OV10640_ExposureCorrection(
          lhist_ptr, ((256.0 - gKbVStrgt) / 256.0) * 55285 /*pixel_level_correction *27640 * 35935 = 55285*0.65 */, 3);

      /* Exposure control of camera 0*/
      if(((lLoop % 3) == 0) && (gKbAEC == 1))
      {
        // Not doing the exposure control every frames
        // because there is a delay in the parameters updates

        OV10640_ExposureComputationInd(gOVExposure[0], lLCorr * gOVExposure[0].mExposure[L],
                                       lVSCorr * gOVExposure[0].mExposure[VS]);
        // grouped update
        CAM_RegWrite(CSI_IDX_CAM, OV10640_GPR_HOLD_ADDR, 0);
        CAM_RegWrite(CSI_IDX_CAM, OV10640_GPR_SEL_ADDR, 0);
        // Update registers
        OV10640_ExposureAllExpSet(gOVExposure[0], ExpTime_Long);
        OV10640_ExposureAllExpSet(gOVExposure[0], ExpTime_Short);
        OV10640_ExposureAllExpSet(gOVExposure[0], ExpTime_VShort);
        OV10640_ExposureAnalogConversionGainSet(gOVExposure[0]);
        // update group
        //single launch
        CAM_RegWrite(CSI_IDX_CAM, OV10640_GPR_SEL_ADDR, 0x20);
        CAM_RegWrite(CSI_IDX_CAM, OV10640_GPR_HOLD_ADDR, 1);

        if(gKbLog == 3)
        {
          OV10640_Exposure_Print(CSI_IDX_CAM, SourceCamera);
        }
      }
#endif // EMBEDDED_DATA  // sth wrong location... no implementation w/o embedded
      else if(((lLoop % 3) == 0) && (gKbAEC == 2)) //Camera AEC
      {                                            // Print parameters based on camera AEC
        if(gKbLog == 3)
        {
          OV10640_Exposure_Print(CSI_IDX_CAM, SourceCamera);
        }
      }

      // **** do white balancing
      if(gKbAWB)
      {
#if(EMBEDDED_DATA == 1)
        cv::Mat lEmbdMat = lpEmbd[1].mUMat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
        OAL_MemoryInvalidate(lEmbdMat.data, lEmbdMat.step[0] * lEmbdMat.rows);
        // do Automatic White balancing
        AWB(lEmbdMat.data);
#else
        AWB(0);
#endif
      }

      // *** print statistics
      if(((lLoop % 29) == 0) && (gKbLog == 6))
      {
        CamHistPrint();
      }

      //**** processing pipeline from last to first

      // freeze display and processing
      if(gKbDisplay)
      {
        gFrameISP = lFrame[FDMA_IX_ISP_OUTPUT0].mUMat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
#if(CAM_SAVE == 1)
        gFrameCAM = lFrame[FDMA_IX_CAM_OUTPUT].mUMat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
#endif //if (CAM_SAVE==1)
        if(gApexActive)
        {
          //display APEX output
          gpFrameFbs = lppApexVirtual[gApexBufferNr];
          lDcuOutput.PutFrame(gpFrameFbs);
        }
        else if(gGpuActive)
        {
          //display APEX output
          gpFrameFbs = lppGPUVirtual[gGpuBufferNr];
          lDcuOutput.PutFrame(gpFrameFbs);
        }
      }

      // call GPU processing
      {
        gGpuBufferNr++;
        if(gGpuBufferNr == DDR_OUT_BUFFER_CNT)
        {
          gGpuBufferNr = 0;
        }
        vsdk::Mat lTmpMat = lFrame[FDMA_IX_ISP_OUTPUT0].mUMat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
        ExtractDisplay(lTmpMat.data, lppGPUVirtual[gGpuBufferNr]);
        gGpuActive = 1;
      }

      if(gOutLut == MAXOUTLUT - 1)
      {
        // do histogram equalization
        if(EqualizeHistogram(gOutLutOffset) == LIB_SUCCESS)
        {
          gOutLutOffset ^= 2048;
        }
      }
      // act on keyboard input
      KeyboardInputProcess();

      //check if LUT needs update
      if(gOutLut != gOldOutLut)
      {
        gOldOutLut = gOutLut;
        if(SetOutLut(gOldOutLut, gOutLutOffset) == LIB_SUCCESS)
        {
          gOutLutOffset ^= 2048;
        }
      }

      if(gKbCursorOn)
      {
        uint8_t*  lpPixel;
        uint8_t   i;
        vsdk::Mat lTmpMat = lFrame[FDMA_IX_ISP_OUTPUT0].mUMat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);

        lpPixel = lTmpMat.data;

        lpPixel += ISP_CHNL_CNT * (gKbCursorX + gKbCursorY * OUT_PIXELS);
        for(i = 0; i < ISP_CHNL_CNT; i++)
          *(lpPixel++) = 255;
        lpPixel -= ISP_CHNL_CNT;
      }

      CsiErrorDetection(gFrmCnt);

      gFrmCnt++;
    } // for LOOP_NUM ********

    if(lPopFailed)
    {
      break;
    } // if gpFramout == NULL

    // *** do logging ***
    LogStatus(lpGrabber);

#ifndef __STANDALONE__
    if(sStop)
    {
      break; // break if Ctrl+C pressed
    }        // if Ctrl+C
#endif       // #ifndef __STANDALONE__
  }          // while(1)

  printf("\nSequencer stopped working in frame %d\n", gFrmCnt);

  ISP_DumpIpuState();

#ifdef DO_BUFFER_LOG
  {
    SEQ_Head_Ptr_t* lpFetchedGraph = NULL;
    SEQ_Buf_t**     lppFetchedBufferList;

    lppFetchedBufferList = lpGrabber->GraphFetch(&lpFetchedGraph);
    ISP_PrintGraphStat(lppFetchedBufferList);
  }
#endif

#ifndef __STANDALONE__
  // *** set terminal back to block input ***
  TermBlockSet();

#ifdef LOGHIST
  fclose(lpHistFw);
#endif
#endif // ifndef __STANDALONE__

#ifdef __STANDALONE__
  for(;;)
  { // *** don't return ***
    KeyboardInputProcess();
    msleep(10);
  }
#endif // #ifdef __STANDALONE__

#if(CAM_SAVE == 1)
  gFrameCAM = vsdk::Mat();
#endif // #if (CAM_SAVE==1)
  gFrameISP = vsdk::Mat();

  if(lpGrabber)
  {
    //*** Stop ISP processing ***
    lpGrabber->Stop();

    // clean up grabber resources
    lpGrabber->Release();

    delete(lpGrabber);
  } // if grabber exists

  //
  // Close SDI
  //
  lRes = sdi::Close(0);
  if(lRes != LIB_SUCCESS)
  {
    lRet = LIB_FAILURE;
  } // if lRes not LIB_SUCCESS

  DdrBuffersFree(lppGPUVirtual, lpGPUPhysical, DDR_OUT_BUFFER_CNT);
  DdrBuffersFree(lppApexVirtual, lpApexPhysical, DDR_OUT_BUFFER_CNT);

  return lRet;

} // main()

  //***************************************************************************

#ifndef __STANDALONE__
void IniParse(void)
{
  dictionary* lpIniFile;

  // open the ini file
  lpIniFile = iniparser_load(INI_FILE_NAME);
  if(lpIniFile == NULL)
  {
    printf("Cannot parse ini file: %s\n", INI_FILE_NAME);
    printf("  Application default values will be used.\n");
    return;
  } // if ini file not parsed

  printf("Reading default values from ini file %s\n", INI_FILE_NAME);

  gKbAEC        = iniparser_getint(lpIniFile, "aec:AECON", gKbAEC);
  gExpMode      = iniparser_getint(lpIniFile, "aec:AECMODE", gExpMode);
  gKbBlackLevel = iniparser_getint(lpIniFile, "aec:BLACKLEVEL", gKbBlackLevel);
  gKbDeFlicker  = iniparser_getint(lpIniFile, "aec:FLICKERMODE", gKbDeFlicker);

  gHDRShift[0] = iniparser_getint(lpIniFile, "aec:HDRSHIFT0", gHDRShift[0]);

  gHDRShift[1] = iniparser_getint(lpIniFile, "aec:HDRSHIFT1", gHDRShift[1]);

  gHDRShift[2] = iniparser_getint(lpIniFile, "aec:HDRSHIFT2", gHDRShift[2]);

  gHDRShift[3] = iniparser_getint(lpIniFile, "aec:HDRSHIFT3", gHDRShift[3]);

  gKbLtrgt  = iniparser_getint(lpIniFile, "aec:LTRGT", gKbLtrgt);
  gKbVStrgt = iniparser_getint(lpIniFile, "aec:VSTRGT", gKbVStrgt);

  gKbAWB     = iniparser_getboolean(lpIniFile, "awb:AWBON", gKbAWB);
  gRedTarget = (uint16_t)(256.0 * iniparser_getdouble(lpIniFile, "awb:RBALANCE", ((double)gRedTarget) / 256.0));

  gBlueTarget   = (uint16_t)(256.0 * iniparser_getdouble(lpIniFile, "awb:BBALANCE", ((double)gBlueTarget) / 256.0));
  gKbSaturation = iniparser_getdouble(lpIniFile, "awb:SATURATION", gKbSaturation);

  gRBIncrement = iniparser_getint(lpIniFile, "awb:INCREMENTRB", gRBIncrement);

  gKbCcmMatrix[gKbCCM][0] = iniparser_getdouble(lpIniFile, "ccm:CCM00", gKbCcmMatrix[gKbCCM][0]);
  gKbCcmMatrix[gKbCCM][1] = iniparser_getdouble(lpIniFile, "ccm:CCM01", gKbCcmMatrix[gKbCCM][1]);
  gKbCcmMatrix[gKbCCM][2] = iniparser_getdouble(lpIniFile, "ccm:CCM02", gKbCcmMatrix[gKbCCM][2]);
  gKbCcmMatrix[gKbCCM][3] = iniparser_getdouble(lpIniFile, "ccm:CCM10", gKbCcmMatrix[gKbCCM][3]);
  gKbCcmMatrix[gKbCCM][4] = iniparser_getdouble(lpIniFile, "ccm:CCM11", gKbCcmMatrix[gKbCCM][4]);
  gKbCcmMatrix[gKbCCM][5] = iniparser_getdouble(lpIniFile, "ccm:CCM12", gKbCcmMatrix[gKbCCM][5]);
  gKbCcmMatrix[gKbCCM][6] = iniparser_getdouble(lpIniFile, "ccm:CCM20", gKbCcmMatrix[gKbCCM][6]);
  gKbCcmMatrix[gKbCCM][7] = iniparser_getdouble(lpIniFile, "ccm:CCM21", gKbCcmMatrix[gKbCCM][7]);
  gKbCcmMatrix[gKbCCM][8] = iniparser_getdouble(lpIniFile, "ccm:CCM22", gKbCcmMatrix[gKbCCM][8]);
  // CcmMatrix[1]
  gKbCcmMatrix[1][0] = iniparser_getdouble(lpIniFile, "ccm:CCM1_00", gKbCcmMatrix[1][0]);
  gKbCcmMatrix[1][1] = iniparser_getdouble(lpIniFile, "ccm:CCM1_01", gKbCcmMatrix[1][1]);
  gKbCcmMatrix[1][2] = iniparser_getdouble(lpIniFile, "ccm:CCM1_02", gKbCcmMatrix[1][2]);
  gKbCcmMatrix[1][3] = iniparser_getdouble(lpIniFile, "ccm:CCM1_10", gKbCcmMatrix[1][3]);
  gKbCcmMatrix[1][4] = iniparser_getdouble(lpIniFile, "ccm:CCM1_11", gKbCcmMatrix[1][4]);
  gKbCcmMatrix[1][5] = iniparser_getdouble(lpIniFile, "ccm:CCM1_12", gKbCcmMatrix[1][5]);
  gKbCcmMatrix[1][6] = iniparser_getdouble(lpIniFile, "ccm:CCM1_20", gKbCcmMatrix[1][6]);
  gKbCcmMatrix[1][7] = iniparser_getdouble(lpIniFile, "ccm:CCM1_21", gKbCcmMatrix[1][7]);
  gKbCcmMatrix[1][8] = iniparser_getdouble(lpIniFile, "ccm:CCM1_22", gKbCcmMatrix[1][8]);

  // CcmMatrix[2]
  gKbCcmMatrix[2][0] = iniparser_getdouble(lpIniFile, "ccm:CCM2_00", gKbCcmMatrix[2][0]);
  gKbCcmMatrix[2][1] = iniparser_getdouble(lpIniFile, "ccm:CCM2_01", gKbCcmMatrix[2][1]);
  gKbCcmMatrix[2][2] = iniparser_getdouble(lpIniFile, "ccm:CCM2_02", gKbCcmMatrix[2][2]);
  gKbCcmMatrix[2][3] = iniparser_getdouble(lpIniFile, "ccm:CCM2_10", gKbCcmMatrix[2][3]);
  gKbCcmMatrix[2][4] = iniparser_getdouble(lpIniFile, "ccm:CCM2_11", gKbCcmMatrix[2][4]);
  gKbCcmMatrix[2][5] = iniparser_getdouble(lpIniFile, "ccm:CCM2_12", gKbCcmMatrix[2][5]);
  gKbCcmMatrix[2][6] = iniparser_getdouble(lpIniFile, "ccm:CCM2_20", gKbCcmMatrix[2][6]);
  gKbCcmMatrix[2][7] = iniparser_getdouble(lpIniFile, "ccm:CCM2_21", gKbCcmMatrix[2][7]);
  gKbCcmMatrix[2][8] = iniparser_getdouble(lpIniFile, "ccm:CCM2_22", gKbCcmMatrix[2][8]);
  // CcmMatrix[3]
  gKbCcmMatrix[3][0] = iniparser_getdouble(lpIniFile, "ccm:CCM3_00", gKbCcmMatrix[3][0]);
  gKbCcmMatrix[3][1] = iniparser_getdouble(lpIniFile, "ccm:CCM3_01", gKbCcmMatrix[3][1]);
  gKbCcmMatrix[3][2] = iniparser_getdouble(lpIniFile, "ccm:CCM3_02", gKbCcmMatrix[3][2]);
  gKbCcmMatrix[3][3] = iniparser_getdouble(lpIniFile, "ccm:CCM3_10", gKbCcmMatrix[3][3]);
  gKbCcmMatrix[3][4] = iniparser_getdouble(lpIniFile, "ccm:CCM3_11", gKbCcmMatrix[3][4]);
  gKbCcmMatrix[3][5] = iniparser_getdouble(lpIniFile, "ccm:CCM3_12", gKbCcmMatrix[3][5]);
  gKbCcmMatrix[3][6] = iniparser_getdouble(lpIniFile, "ccm:CCM3_20", gKbCcmMatrix[3][6]);
  gKbCcmMatrix[3][7] = iniparser_getdouble(lpIniFile, "ccm:CCM3_21", gKbCcmMatrix[3][7]);
  gKbCcmMatrix[3][8] = iniparser_getdouble(lpIniFile, "ccm:CCM3_22", gKbCcmMatrix[3][8]);

  gKbYDenoise = iniparser_getint(lpIniFile, "ynr:THRESHOLD", gKbYDenoise);

  gKbEdgeThrLow = iniparser_getint(lpIniFile, "edge:THRLOW", gKbEdgeThrLow);
  gKbEdgeMaxA   = (uint8_t)(64.0 * iniparser_getdouble(lpIniFile, "edge:MAXA", ((double)gKbEdgeMaxA) / 64.0));

  {
    int  i    = 0;
    int  lIic = 0;
    char lParname[32];

    while(1)
    {
      snprintf(lParname, 31, "cam:IIC%03d", i);
      lParname[31] = 0;
      lIic         = iniparser_getint(lpIniFile, lParname, 0);
      if(!lIic)
        break;
      printf("%s : 0x%06x\n", lParname, lIic);
      CAM_RegWrite(CSI_IDX_CAM, (lIic >> 8) & 0xffff, lIic & 0xff);
      lIic = 0;
      i++;
    }

  } // end read camera parameters

  if(lpIniFile)
  {
    iniparser_freedict(lpIniFile);
  } // if ini exists

} // IniParse()
#endif // ifndef __STANDALONE__

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

int KeyDown()
{
  struct timeval lTv;
  fd_set         lFdSet;
  lTv.tv_sec  = 0;
  lTv.tv_usec = 50;
  FD_ZERO(&lFdSet);
  FD_SET(STDIN_FILENO, &lFdSet);
  select(STDIN_FILENO + 1, &lFdSet, NULL, NULL, &lTv);
  return FD_ISSET(STDIN_FILENO, &lFdSet);
} // KeyDown()

//***************************************************************************

int GetCharNonBlock()
{
  int lChar = EOF;

  usleep(1);
  if(KeyDown())
  {
    lChar = fgetc(stdin);
  } // if Key pressed

  return lChar;
} // KeyDown()

//***************************************************************************

void TermNonBlockSet()
{
  struct termios lTermState;

  // get current state of the terminal
  tcgetattr(STDIN_FILENO, &lTermState);

  // disable canonical mode
  lTermState.c_lflag &= ~ICANON;
  // set minimum number of input characters to read
  lTermState.c_cc[VMIN] = 1;

  // set the terminal attributes
  tcsetattr(STDIN_FILENO, TCSANOW, &lTermState);
} // TermNonBlockSet()

//***************************************************************************

void TermBlockSet()
{
  struct termios lTermState;

  // get current state of the terminal
  tcgetattr(STDIN_FILENO, &lTermState);

  //enable canonical mode
  lTermState.c_lflag |= ICANON;

  // set the terminal attributes
  tcsetattr(STDIN_FILENO, TCSANOW, &lTermState);
} // TermBlockSet()

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

//***************************************************************************

char GetChar()
{
#ifdef __STANDALONE__
  return sci_0_testchar();
#else  // #ifdef __STANDALONE__
  int lChar = GetCharNonBlock();
  return (char)((lChar < 0) ? 0 : lChar & 0xff);
#endif // else from #ifdef __STANDALONE__
} // GetChar()

//***************************************************************************

void KbAction(uint8_t aKey)
{
  if(aKey == 'h')
  { // help
    printf(
        "\nkeyboard help:\n"
        "\n*** Generic commands\n"
        "h .........help\n"
#ifndef __STANDALONE__
        "X..........reload init file\n"
#endif
        "d .........toggle display on off\n"
        "i .........enter IIC command mode\n"
        "  aaaavv ..enter IIC command to HEX aaaa addr with HEX vv value\n"
        "  ret .....write to IIC\n"
        "  r .......read from IIC\n"
        "D .........Camera I2C dump to the console. This kills streaming\n"
        "t .........toggle test bar\n"
        "m .........measure camera using multiple settings\n"
        "s .........save output image to file\n"
#if(CAM_SAVE == 1)
#ifndef __STANDALONE__
        "S .........Save input  image to file\n"
#endif
#endif //if (CAM_SAVE==1)
        "e .........toggle engine to set parameter (0-11)\n"
        "g .........toggle GPR register to write parameter (0-15/31)\n"
        "v .........parameter value at Engine E Register G\n"
        "\n*** Input control commands\n"
        "I .........toggle input LUT\n"
        "J .........manual exposure setting\n"
        "    E........exposure  to change (L/S/VS)\n"
        "    P........parameter to change (Time/AG/DG/CG)\n"
        "b .........select black level mode (+/-)\n"
        "\n*** Exposure control commands\n"
        "K .........select exposure mode (0/1/2/3)\n"
        "H .........toggle HDR shift for Long+Short\n"
        "A .........toggle AEC on off\n"
        "r .........changes exposure ratio between L and S exposure\n"
        "  +........increase by 5 percent % \n"
        "  -........decrease by 5 percent % \n"
        "M .........toggle exposure mode 0:outdoor, 1:dark\n"
        "O .........toggle output LUT\n"
        "C .......toggle conversion gain enable\n"
        "\n*** White balance control commands\n"
        "W .........toggle AWB on off\n"
        "G .........toggle channel gain\n"
        "T .........select color saturation mode (+/-)\n"
        "R .........select red  target mode (+/-)\n"
        "B .........select blue target mode (+/-)\n"
        "\n*** Denoising control commands\n"
        "Y .........set  Y-denoising mode (+/-/E/C)\n"
        "  E .......toggle edge view in denoising\n"
        "\n*** YUV420 to RGB control commands ***\n"
        "y .........toggle mode between A53 and Neon\n"
        "\n*** CCM control*\n"
        "C .........toggle CCM table\n"
        "\n*** Edge enhancement control commands\n");
    gKbMode = 'h';
    return;
  } // if help requested
#ifndef __STANDALONE__
  if(aKey == 'X')
  {
    printf("Reloading init file...\n");
    IniParse();
    printf("****\n");
    // TODO: Is this necessary?
    // SetDeflickerHz(gFlickerFrequency[gKbDeFlicker],gCamFps); //needs to be executed after ExposureInit
    return;
  }
#endif

  if(gKbMode == 'i')
  {
    if((aKey >= '0') && (aKey <= '9'))
    { // enter IIC command
      gKbI2c <<= 4;
      gKbI2c += (aKey - '0') & 0xf;
      gKbI2c &= 0xffffff;
      printf("IIC command 0x%x - 0x%x\n", (gKbI2c >> 8), (gKbI2c & 0xff));
      return;
    } // if key <0;9>

    if((aKey >= 'a') && (aKey <= 'f'))
    { // enter IIC command
      gKbI2c <<= 4;
      gKbI2c += (aKey - 'a' + 10) & 0xf;
      gKbI2c &= 0xffffff;
      printf("IIC command 0x%x - 0x%x\n", (gKbI2c >> 8), (gKbI2c & 0xff));
      return;
    } // if key <a;f>
    if(strrchr("\n\rw", aKey))
    {
      printf("Writing to IIC\n");
      CAM_RegWrite(CSI_IDX_CAM, gKbI2c >> 8, gKbI2c & 0xff);
      return;
    } // if key w
    if(aKey == 'r')
    {
      printf("Read from IIC\n");

      uint8_t val;
      CAM_RegRead(CSI_IDX_CAM, (gKbI2c >> 8) & 0xffff, val);
      printf("%x = 0x%x / %d\n", gKbI2c >> 8, val, val);

      return;
    } // if key r
  }   // if mode i

  if(gKbMode == 'v')
  {
    if((aKey >= '0') && (aKey <= '9'))
    { // enter parameter
      gKbParVal <<= 4;
      gKbParVal += (aKey - '0') & 0xf;
      gKbParVal &= 0xffff;
      printf("Engine %d GRP%d: 0x%x\n", gKbEngine, gKbGPR, gKbParVal);
      return;
    }
    if((aKey >= 'a') && (aKey <= 'f'))
    { // enter parameter
      gKbParVal <<= 4;
      gKbParVal += (aKey - 'a' + 10) & 0xf;
      gKbParVal &= 0xffff;
      printf("Engine %d GRP%d: 0x%x\n", gKbEngine, gKbGPR, gKbParVal);
      return;
    }
    if(aKey == '+')
    { // enter parameter
      gKbParVal += 16;
      gKbParVal &= 0xffff;
      printf("Engine %d GRP%d: 0x%x\n", gKbEngine, gKbGPR, gKbParVal);
      aKey = 'w';
    }
    if(aKey == '-')
    { // enter parameter
      gKbParVal -= 16;
      gKbParVal &= 0xffff;
      printf("Engine %d GRP%d: 0x%x\n", gKbEngine, gKbGPR, gKbParVal);
      aKey = 'w';
    }
    if(strrchr("\n\rw", aKey))
    {
      printf("Writing IPU%c%d GRP%d: 0x%x\n", (gKbEngine & 0x8) ? 'V' : 'S', gKbEngine & 0x7, gKbGPR, gKbParVal);
      setRegCoherent(((gKbEngine >> 3) & 0x1) ^ 0x1, (gKbEngine & 0x7), 0, ((gKbEngine & 0x8) ? 0x50 : 0x70) + gKbGPR,
                     gKbParVal);
      return;
    }
    if(aKey == 'r')
    {
      printf("Reading IPU%c%d GRP%d: ", (gKbEngine & 0x8) ? 'V' : 'S', gKbEngine & 0x7, gKbGPR);
      {
        seq_getReg(((gKbEngine >> 3) & 0x1) ^ 0x1, (gKbEngine & 0x7), 0, ((gKbEngine & 0x8) ? 0x50 : 0x70) + gKbGPR,
                   &gKbParVal);
        printf("0x%x / %d\n", gKbParVal, gKbParVal);
      }
      return;
    }
  }

  if(gKbMode == 'r')
  {
    if(aKey == '+')
    {
      gRatioLSExp = gRatioLSExp * 1.05;
      if(gRatioLSExp > 16)
      {
        gRatioLSExp = 16;
      }
      ExposureLSRatioChange(gRatioLSExp, gKbLog);
      printf("New Ratio is %f\n", gRatioLSExp);
      return;
    }
    if(aKey == '-')
    {
      gRatioLSExp = gRatioLSExp * 0.95;
      if(gRatioLSExp < 0.01)
      {
        gRatioLSExp = 0.01;
      }
      ExposureLSRatioChange(gRatioLSExp, gKbLog);
      printf("New Ratio is %f\n", gRatioLSExp);
      return;
    }

  } // if 'r' pressed

  if(gKbMode == 'K')
  {
    if(aKey == '0')
    { // show combined

      OV10640_RegWrite(CSI_IDX_CAM, 0x311b, 0x4a);
      OV10640_RegWrite(CSI_IDX_CAM, 0x3119, 0x14);
      setRegCoherent(INLUT_ENGINE, 0, 0x7f, gInLut); // GPR15 (IPUS)
      setRegCoherent(HDR_ENGINE, 0, 0x7e, 0x0);      // GPR14 (IPUS)
      setRegCoherent(HDR_ENGINE, 0, 0x7f,            // GPR15 (IPUS)
                     (gExpMode < 4) ? gHDRShift[gExpMode] : 4);
      gKbExpSel = 0;
      gOutLut   = gOutLut1[0];
      printf("%d %s HDRShift %d ILUT %d OLUT %d\n", gKbExpSel, gExpName[gKbExpSel],
             (gExpMode < 4) ? gHDRShift[gExpMode] : 4, gInLut, gOutLut);
      return;
    } // if key 0
    if(aKey == '1')
    { // VS only
      OV10640_RegWrite(CSI_IDX_CAM, 0x311b, 0x4a);
      OV10640_RegWrite(CSI_IDX_CAM, 0x3119, 0x57);
      setRegCoherent(INLUT_ENGINE, 0, 0x7f, 0); // GPR15 (IPUS)
      setRegCoherent(HDR_ENGINE, 0, 0x7e, 0x0); // Shift VS  GPR14
      setRegCoherent(HDR_ENGINE, 0, 0x7f, 0xf); // Shift L/S GPR15
      gKbExpSel = 1;
      gOutLut   = 1;
      printf("%d %s HDRShift %d ILUT %d OLUT %d\n", gKbExpSel, gExpName[gKbExpSel], 0xf, 0, gOutLut);
      return;
    } // if key 1
    if(aKey == '2')
    { // S only
      OV10640_RegWrite(CSI_IDX_CAM, 0x311b, 0x4a);
      OV10640_RegWrite(CSI_IDX_CAM, 0x3119, 0x56);
      setRegCoherent(INLUT_ENGINE, 0, 0x7f, 0); // GPR15 (IPUS)
      setRegCoherent(HDR_ENGINE, 0, 0x7e, 0xf); // Shift VS  GPR14
      setRegCoherent(HDR_ENGINE, 0, 0x7f, 0);   // Shift L/S GPR15
      gKbExpSel = 2;
      gOutLut   = 1;
      printf("%d %s HDRShift %d ILUT %d OLUT %d\n", gKbExpSel, gExpName[gKbExpSel], 0, 0, gOutLut);
      return;
    } // if key 2
    if(aKey == '3')
    { // L only
      OV10640_RegWrite(CSI_IDX_CAM, 0x311b, 0x4a);
      OV10640_RegWrite(CSI_IDX_CAM, 0x3119, 0x55);
      setRegCoherent(INLUT_ENGINE, 0, 0x7f, 0); // GPR15 (IPUS)
      setRegCoherent(HDR_ENGINE, 0, 0x7e, 0xf); // GPR14 (IPUS)
      setRegCoherent(HDR_ENGINE, 0, 0x7f, 0);   // Shift L/S GPR15
      gKbExpSel = 3;
      gOutLut   = 1;
      printf("%d %s HDRShift %d ILUT %d OLUT %d\n", gKbExpSel, gExpName[gKbExpSel], 0, 0, gOutLut);
      return;
    } // if key 3
  }   // if mode k

  if(gKbMode == 'b')
  {
    uint8_t lSet = 0;
    if(aKey == '+')
    {
      if(gKbBlackLevel < 255)
      {
        gKbBlackLevel++;
      } // if offset <= in range
      lSet = 1;
    } // if key +
    else if(aKey == '-')
    {
      if(gKbBlackLevel)
      {
        gKbBlackLevel--;
      } // if offset >= X_PAN_STEPSIZE
      lSet = 1;
    } // if key -

    if(lSet)
    {
      printf("Black level =%d\n", gKbBlackLevel);
      CAM_RegWrite(CSI_IDX_CAM, 0x3168, 0x00);
      CAM_RegWrite(CSI_IDX_CAM, 0x3169, gKbBlackLevel);
      CAM_RegWrite(CSI_IDX_CAM, 0x316a, 0x00);
      CAM_RegWrite(CSI_IDX_CAM, 0x316b, gKbBlackLevel);
      CAM_RegWrite(CSI_IDX_CAM, 0x316c, 0x00);
      CAM_RegWrite(CSI_IDX_CAM, 0x316d, gKbBlackLevel);
      return;
    }
  } // if mode B

  if(gKbMode == 'T')
  {
    uint8_t lSet = 0;
    if(aKey == '+')
    {
      if(gKbSaturation <= 1.95)
      {
        gKbSaturation += 0.05;
      } // if offset <= 1.95
      lSet = 1;
    } // if key +
    else if(aKey == '-')
    {
      if(gKbSaturation >= 0.05)
      {
        gKbSaturation -= 0.05;
      } // if offset >= 0.05
      lSet = 1;
    } // if key -

    if(lSet)
    {
      printf("Color saturation = %f\n", gKbSaturation);
      CcmCscMatrix(gCcmCscMatrix, gKbCcmMatrix[gKbCCM], gRgb2YuvMatrix, gKbSaturation);

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
      return;
    }
  } // if mode T

  if(gKbMode == 'R')
  {
    uint8_t lSet = 0;
    if(gKbAWB)
    {
      if(aKey == '+')
      {
        gRedTarget += gRBIncrement;
        if(gRedTarget > 0x3FF)
        {
          gRedTarget = 0x3FF;
        }
        lSet = 1;
      } // if key +
      else if(aKey == '-')
      {
        gRedTarget -= gRBIncrement;
        if(gRedTarget > 0xF000)
        { // negative
          gRedTarget = 0;
        }
        lSet = 1;
      } // if key -

      if(lSet)
      {
        printf("Red target value = %f\n", (float_t)gRedTarget / 256.0);
        return;
      }
    }
    else
    { // gKBAWB off
      if(aKey == '+')
      {
        gChGainTab[CHGAINSETS - 1][0] += gRBIncrement;
        if(gChGainTab[CHGAINSETS - 1][0] > 0x3FF)
        {
          gChGainTab[CHGAINSETS - 1][0] = 0x3FF;
        }
        lSet = 1;
      } // if key +
      else if(aKey == '-')
      {
        gChGainTab[CHGAINSETS - 1][0] -= gRBIncrement;
        if(gChGainTab[CHGAINSETS - 1][0] > 0xF000)
        { // negative
          gChGainTab[CHGAINSETS - 1][0] = 0;
        }
        lSet = 1;
      } // if key -

      if(lSet)
      {
        OV10640_Exposure_t lExp;
        memset(&lExp, 0, sizeof(lExp));
        lExp.mCsiIdx = CSI_IDX_CAM;

        for(uint32_t i = 0; i < 4; i++)
        {
          lExp.mpLongChnlGain[i]      = gChGainTab[CHGAINSETS - 1][i];
          lExp.mpShortChnlGain[i]     = gChGainTab[CHGAINSETS - 1][i];
          lExp.mpVeryShortChnlGain[i] = gChGainTab[CHGAINSETS - 1][i];
        } // for all color compononents

        MAXIM_OV10640_ExposureSet(lExp);
        printf("chgain %d (%d,%d,%d,%d)\n", gKbChgain, gChGainTab[CHGAINSETS - 1][0], gChGainTab[CHGAINSETS - 1][1],
               gChGainTab[CHGAINSETS - 1][2], gChGainTab[CHGAINSETS - 1][3]);

        return;
      }
    }
  } // if mode R

  if(gKbMode == 'B')
  {
    uint8_t lSet = 0;
    if(gKbAWB)
    {
      if(aKey == '+')
      {
        gBlueTarget += gRBIncrement;
        if(gBlueTarget > 0x3FF)
        {
          gBlueTarget = 0x3FF;
        }
        lSet = 1;
      } // if key +
      else if(aKey == '-')
      {
        gBlueTarget -= gRBIncrement;
        if(gBlueTarget > 0xF000)
        {
          gBlueTarget = 0;
        }
        lSet = 1;
      } // if key -

      if(lSet)
      {
        printf("Blue target value = %f\n", (float_t)gBlueTarget / 256.0);
        return;
      }
    }
    else
    { // gKBAWB off
      if(aKey == '+')
      {
        gChGainTab[CHGAINSETS - 1][3] += gRBIncrement;
        if(gChGainTab[CHGAINSETS - 1][3] > 0x3FF)
        {
          gChGainTab[CHGAINSETS - 1][3] = 0x3FF;
        }
        lSet = 1;
      } // if key +
      else if(aKey == '-')
      {
        gChGainTab[CHGAINSETS - 1][3] -= gRBIncrement;
        if(gChGainTab[CHGAINSETS - 1][3] > 0xF000)
        { // negative
          gChGainTab[CHGAINSETS - 1][3] = 0;
        }
        lSet = 1;
      } // if key -

      if(lSet)
      {
        OV10640_Exposure_t lExp;
        memset(&lExp, 0, sizeof(lExp));
        lExp.mCsiIdx = CSI_IDX_CAM;

        for(uint32_t i = 0; i < 4; i++)
        {
          lExp.mpLongChnlGain[i]      = gChGainTab[CHGAINSETS - 1][i];
          lExp.mpShortChnlGain[i]     = gChGainTab[CHGAINSETS - 1][i];
          lExp.mpVeryShortChnlGain[i] = gChGainTab[CHGAINSETS - 1][i];
        } // for all color compononents

        MAXIM_OV10640_ExposureSet(lExp);
        printf("chgain %d (%d,%d,%d,%d)\n", gKbChgain, gChGainTab[CHGAINSETS - 1][0], gChGainTab[CHGAINSETS - 1][1],
               gChGainTab[CHGAINSETS - 1][2], gChGainTab[CHGAINSETS - 1][3]);

        return;
      }
    }
  } // if mode B

  if(gKbMode == 'H')
  {
    uint8_t lHDRShift;
    lHDRShift = aKey - '0';
    if(lHDRShift < 8)
    {
      gHDRShift[gExpMode] = lHDRShift;
      printf("HDR Long/Short Shift Right = %d\n", gHDRShift[gExpMode]);
      setRegCoherent(HDR_ENGINE, 0, 0x7f, gHDRShift[gExpMode]); // Shift VS  GPR14
      return;
    }
  }

  if(gKbMode == 'I')
  {
    uint8_t lInLut;
    lInLut = aKey - '0';
    if(lInLut <= MAXINLUT)
    {
      gInLut = lInLut;
      setRegCoherent(INLUT_ENGINE, 0, 0x7f, gInLut); // Shift VS  GPR15

      printf("Input  Lut %d\n", gInLut);

      return;
    }
  }

  if(gKbMode == 'O')
  {
    uint8_t lOutLut;
    lOutLut = aKey - '0';
    if(lOutLut <= MAXOUTLUT)
    {
      gOutLut = lOutLut;
      if(gExpMode < 4)
        gOutLut1[gExpMode] = gOutLut;
      return;
    }
  }
  if(gKbMode == 'J')
  {
    // grouped update
    CAM_RegWrite(CSI_IDX_CAM, OV10640_GPR_HOLD_ADDR, 0);
    CAM_RegWrite(CSI_IDX_CAM, OV10640_GPR_SEL_ADDR, 0);
    if(aKey == 'E')
    {
      gKbExpToChange = (OV10640_ExpTime_t)(((uint8_t)gKbExpToChange + 1) % 3);
      printf(" %s       - %s    (exposure changed)\n", gExposureName[gKbExpToChange], gParamName[gKbExpParamChange]);
      gKbExpVal  = 0;
      gKbExpTime = 0;
      return;
    }
    if(aKey == 'P')
    {
      gKbExpParamChange = (gParam)(((uint8_t)gKbExpParamChange + 1) % 4);
      printf(" %s       - %s    (parameter changed)\n", gExposureName[gKbExpToChange], gParamName[gKbExpParamChange]);
      gKbExpVal  = 0;
      gKbExpTime = 0;
      return;
    }
    if(strchr("w\n", aKey))
    {
      gOVExposure[0].mCsiIdx = CSI_IDX_CAM;
      printf("Update %s - %s\n", gExposureName[gKbExpToChange], gParamName[gKbExpParamChange]);
      if(gKbExpParamChange == Time)
      {
        if(gKbExpToChange == ExpTime_VShort)
        {
          if(gKbExpVal > 0x7f)
          {
            gKbExpVal = 0x7f;
          }
          gOVExposure[0].mExposureTime[VS] = gKbExpVal;
        }
        else
        { //S or L
          gOVExposure[0].mExposureTime[gKbExpToChange] = gKbExpTime;
        }
        OV10640_ExposureTimeSet(gOVExposure[0], gKbExpToChange);
      }
      else if(gKbExpParamChange == AG)
      {
        gOVExposure[0].mAnalogGain[gKbExpToChange] = gKbExpVal;
        OV10640_ExposureAnalogConversionGainSet(gOVExposure[0]);
      }
      else if(gKbExpParamChange == DG)
      {
        gOVExposure[0].mDigitalGain[gKbExpToChange] = gKbExpVal;
        OV10640_ExposureDigitalGainSet(gOVExposure[0], gKbExpToChange);
      }
      else if(gKbExpParamChange == CG)
      {
        gOVExposure[0].mConversionGain[gKbExpToChange] = gKbExpVal;
        OV10640_ExposureAnalogConversionGainSet(gOVExposure[0]);
      }
      OV10640_Exposure_Print(CSI_IDX_CAM, SourceCamera);
      gKbExpVal  = 0;
      gKbExpTime = 0;
      return;
    }

    if(aKey == '-')
    {
      gOVExposure[0].mCsiIdx = CSI_IDX_CAM;
      printf("Update %s - %s\n", gExposureName[gKbExpToChange], gParamName[gKbExpParamChange]);

      if(gKbExpParamChange == Time)
      {
        if(gKbExpToChange == ExpTime_VShort)
        {
          if(gOVExposure[0].mExposureTime[VS] > 0)
          {
            gOVExposure[0].mExposureTime[VS]--;
          }
        }
        else
        { //S or L
          if(gOVExposure[0].mExposureTime[gKbExpToChange] > (gExpIncrement + 16))
          {
            gOVExposure[0].mExposureTime[gKbExpToChange] -= gExpIncrement;
          }
          else
          {
            gOVExposure[0].mExposureTime[gKbExpToChange] = 16;
          }
        }
        OV10640_ExposureTimeSet(gOVExposure[0], gKbExpToChange);
      }

      else if(gKbExpParamChange == AG)
      {
        if(gOVExposure[0].mAnalogGain[gKbExpToChange] > 0)
        {
          gOVExposure[0].mAnalogGain[gKbExpToChange]--;
          OV10640_ExposureAnalogConversionGainSet(gOVExposure[0]);
        }
      }

      else if(gKbExpParamChange == DG)
      {
        if(gOVExposure[0].mDigitalGain[gKbExpToChange] > (gDgIncrement + 0x100))
        {
          gOVExposure[0].mDigitalGain[gKbExpToChange] -= gDgIncrement;
        }
        else
        {
          gOVExposure[0].mDigitalGain[gKbExpToChange] = 0x100;
        }
        OV10640_ExposureDigitalGainSet(gOVExposure[0], gKbExpToChange);
      }

      else if(gKbExpParamChange == CG)
      {
        gOVExposure[0].mConversionGain[gKbExpToChange] = 0;
        OV10640_ExposureAnalogConversionGainSet(gOVExposure[0]);
      }

      OV10640_Exposure_Print(CSI_IDX_CAM, SourceS32v);
      return;
    } //  '-' key

    if(aKey == '+')
    {
      gOVExposure[0].mCsiIdx = CSI_IDX_CAM;
      printf("Update %s - %s\n", gExposureName[gKbExpToChange], gParamName[gKbExpParamChange]);

      if(gKbExpParamChange == Time)
      {
        if(gKbExpToChange == ExpTime_VShort)
        {
          if(gOVExposure[0].mExposureTime[VS] < 0x7f)
          {
            gOVExposure[0].mExposureTime[VS]++;
          }
        }
        else
        { //S or L
          if(gOVExposure[0].mExposureTime[gKbExpToChange] < (gExpRange - gExpIncrement))
          {
            gOVExposure[0].mExposureTime[gKbExpToChange] += gExpIncrement;
          }
          else
          {
            gOVExposure[0].mExposureTime[gKbExpToChange] = gExpRange;
          }
        }
        OV10640_ExposureTimeSet(gOVExposure[0], gKbExpToChange);
      }
      else if(gKbExpParamChange == AG)
      {
        if(gOVExposure[0].mAnalogGain[gKbExpToChange] < 3)
        {
          gOVExposure[0].mAnalogGain[gKbExpToChange]++;
          OV10640_ExposureAnalogConversionGainSet(gOVExposure[0]);
        }
      }
      else if(gKbExpParamChange == DG)
      {
        if(gOVExposure[0].mDigitalGain[gKbExpToChange] < (0x800 - gDgIncrement))
        {
          gOVExposure[0].mDigitalGain[gKbExpToChange] += gDgIncrement;
        }
        else
        {
          gOVExposure[0].mDigitalGain[gKbExpToChange] = 0x800;
        }
        OV10640_ExposureDigitalGainSet(gOVExposure[0], gKbExpToChange);
      }
      else if(gKbExpParamChange == CG)
      {
        if(gOVExposure[0].mConversionGain[gKbExpToChange] == S)
        {
          printf("No conversion gain available for S\n");
        }
        else
        {
          gOVExposure[0].mConversionGain[gKbExpToChange] = 1;
          OV10640_ExposureAnalogConversionGainSet(gOVExposure[0]);
        }
      }
      OV10640_Exposure_Print(CSI_IDX_CAM, SourceCamera);
      return;
    } //  '+' key

    if((aKey >= '0') && (aKey <= '9'))
    { // enter IIC command
      gKbExpVal <<= 4;
      gKbExpVal += (aKey - '0') & 0xf;
      if((gKbExpToChange != ExpTime_VShort) & (gKbExpParamChange == Time)) //DCG time
      {
        if(gKbExpTime < 1000)
        {
          gKbExpTime = gKbExpTime * 10 + ((aKey - '0') & 0xf);
        }
        else
        {
          gKbExpTime = (aKey - '0') & 0xf;
        }
        printf("Entered Exp time: %d\n", gKbExpTime);
        return;
      }
      if(gKbExpParamChange == AG)
      {
        gKbExpVal &= 0x3;
        printf("Entered AG: %d (x%d)\n", gKbExpVal, 1 << gKbExpVal);
        return;
      }
      if(gKbExpParamChange == DG)
      {
        gKbExpVal &= 0x3fff; //6.8b value
        printf("Entered DG: 0x%04x\n", gKbExpVal);
        return;
      }
      if(gKbExpParamChange == CG)
      {
        gKbExpVal &= 0x1;
        printf("Entered CG: %d\n", gKbExpVal);
        return;
      }
      else //Exposure time VS
      {
        gKbExpVal &= 0x7f;
        printf("Entered VS Exposure time [0x10-0x7f]: 0x%02x\n", gKbExpVal);
        return;
      }
    }

    if((aKey >= 'a') && (aKey <= 'f'))
    { // enter IIC command
      gKbExpVal <<= 4;
      gKbExpVal += (aKey - 'a' + 10) & 0xf;
      if((gKbExpToChange != ExpTime_VShort) & (gKbExpParamChange == Time)) //DCG time
      {
        //gKbExpVal &=0xffff;
        printf("Entered Exp time in decimal number\n");
        return;
      }
      if(gKbExpParamChange == AG)
      {
        gKbExpVal &= 0x3;
        printf("Entered AG: %d (x%d)\n", gKbExpVal, 1 << gKbExpVal);
        return;
      }
      if(gKbExpParamChange == DG)
      {
        gKbExpVal &= 0x3fff; //6.8b bvalue
        printf("Entered DG (6.8b): 0x%04x\n", gKbExpVal);
        return;
      }
      if(gKbExpParamChange == CG)
      {
        gKbExpVal &= 0x1;
        printf("Entered CG: %d\n", gKbExpVal);
        return;
      }
      else //Exposure time VS
      {
        gKbExpVal &= 0x7f;
        printf("Entered VS Exposure time [0x10-0x7f]: 0x%02x\n", gKbExpVal);
        return;
      }
    } // if key <a;f>
  }

  if(gKbMode == 'Y')
  {
    if(aKey == 'E')
    {
      if(gKbViewEdge)
      {
        gKbViewEdge = 0;
      }
      else
      {
        gKbViewEdge = 0xffff;
      }
      // selector for edge view
      seq_setReg(DENOISEY_ENGINE0, 0, DENOISEY_GPR | 0xc, gKbViewEdge);
      seq_setReg(DENOISEY_ENGINE1, 0, DENOISEY_GPR | 0xc, gKbViewEdge);

      printf("Denoise view: %s\n", (gKbViewEdge) ? "normal" : "edge");
      return;
    } // if E pressed

    if(aKey == 'C') // camera denoise on off
    {
#define CAM_ISP_DNR_BIT (0x8)
      uint8_t lCamIspConfig = 0;

      CAM_RegRead(CSI_IDX_CAM, 0x3127, lCamIspConfig);
      lCamIspConfig ^= CAM_ISP_DNR_BIT;
      if(lCamIspConfig & CAM_ISP_DNR_BIT)
      {
        printf("Camera DNR on\n");
      }
      else
      {
        printf("Camera DNR off\n");
      }
      CAM_RegWrite(CSI_IDX_CAM, 0x3127, lCamIspConfig);
      return;
    } // if C pressed

    uint8_t lSet = 0;
    if(aKey == '+')
    { // show combined
      gKbYDenoise += 64;
      lSet = 1;
    } // if key +
    else if(aKey == '-')
    {
      gKbYDenoise -= 64;
      lSet = 1;
    } // if key -

    if(lSet)
    {
      printf("Y de-noise level = %d", gKbYDenoise);

      if(gKbYDenoise == 0)
        printf(" = off\n");
      else if(gKbYDenoise == 65535 - 64 + 1)
        printf(" = on\n");
      else
        printf("\n");
      // set threshold
      seq_setReg(DENOISEY_ENGINE0, 0, DENOISEY_GPR | 0x7, gKbYDenoise);
      seq_setReg(DENOISEY_ENGINE1, 0, DENOISEY_GPR | 0x7, gKbYDenoise);
      return;
    }
  } // if mode B

  // check if we need to enter a mode
  if(aKey == 'd')
  {
    gKbMode = 'd';
    gKbDisplay ^= 1;
    printf("Display update %s\n", (gKbDisplay) ? "on" : "off");
    if(!gKbDisplay)
      printf("Frame Buffer %d\n", gApexActive ? gApexBufferNr : gGpuBufferNr);
    return;
  } // if d pressed

  if(gKbMode == 'M')
  {
    if(aKey == '0')
    {
      gExpMode = 0;
      printf("HDR ");
      //gHDRShift=gHDRShift1;
      gKbMode = 'K';
      KbAction('0');
      gKbMode = 'M';
      return;
    }
    else if(aKey == '1')
    {
      gExpMode = 1;
      printf("Single \n");
      gKbMode = 'K';
      KbAction('3');
      gKbMode = 'M';
      return;
    }
  }

  if(aKey == 'i')
  {
    printf("IIC command mode\n");
    printf("IIC command 0x%x - 0x%x\n", (gKbI2c >> 8), (gKbI2c & 0xff));
    gKbMode = 'i';
    return;
  } // if i pressed

  if(aKey == 't')
  {
    if(gKbTestbar)
    {
      gKbTestbar = 0;
      printf("Testbar off\n");
#ifdef OV10640
      CAM_RegWrite(CSI_IDX_CAM, 0x3129, 0x00);
#else
      CAM_RegWrite(CSI_IDX_CAM, 0x3253, 0x00);
#endif
    }
    else
    {
      gKbTestbar = 1;
#ifdef OV10640
      CAM_RegWrite(CSI_IDX_CAM, 0x3129, 0x8c);
#else
      CAM_RegWrite(CSI_IDX_CAM, 0x3253, 0x83);
#endif
      printf("Testbar on\n");
    }
    return;
  }

  if(aKey == 's')
  {
    gKbMode = 's';
    FrameSave();
    return;
  }
#if(CAM_SAVE == 1)
  if(aKey == 'S')
  {
    gKbMode = 'S';
    FrameSave();
    return;
  }
#endif //if (CAM_SAVE==1)

  if(aKey == 'm')
  {
    gKbMode = 'm';
    printf("Measurment start\n");
    gKbMeasure = 0;
    return;
  }

  if(aKey == 'x')
  {
    printf("Set X-Offset for Display\n");
    gKbMode = 'x';
    return;
  } // if x pressed

  if(aKey == 'e')
  {
    gKbMode = 0;
    gKbEngine++;
    if(gKbEngine > 0xb)
      gKbEngine = 0;
    printf("Parameter Engine = %d\n", gKbEngine);
    return;
  }

  if(aKey == 'D')
  {
    uint8_t lVal;
#ifndef __STANDALONE__
    FILE* outfile;
    outfile = fopen("ov10640_quad_IIC.txt", "w+");
#endif

    printf("Doing IIC register dump:\n");
    for(uint16_t i = 0x3000; i < 0x3500; i++)
    {
      CAM_RegRead(CSI_IDX_CAM, i, lVal);
      printf("0x%x = 0x%x\n", i, lVal);
#ifndef __STANDALONE__
      if(outfile)
      {
        fprintf(outfile, "0x%x = 0x%x\n", i, lVal);
      }
#endif
    }

    for(uint16_t i = 0x4000; i < 0x4080; i++)
    {
      CAM_RegRead(CSI_IDX_CAM, i, lVal);
      printf("0x%x = 0x%x\n", i, lVal);
#ifndef __STANDALONE__
      if(outfile)
      {
        fprintf(outfile, "0x%x = 0x%x\n", i, lVal);
      }
#endif
    }

    for(uint16_t i = 0x7000; i < 0x7800; i++)
    {
      CAM_RegRead(CSI_IDX_CAM, i, lVal);
      printf("0x%x = 0x%x\n", i, lVal);
#ifndef __STANDALONE__
      if(outfile)
      {
        fprintf(outfile, "0x%x = 0x%x\n", i, lVal);
      }
#endif
    }
#ifndef __STANDALONE__
    if(outfile)
    {
      fclose(outfile);
    }
#endif
    gKbMode = 0;
    return;
  }

  if(aKey == 'g')
  {
    gKbMode = 0;
    gKbGPR++;
    if(gKbGPR > 15)
      gKbGPR = 0;
    printf("Parameter Engine = %d GPR%d\n", gKbEngine, gKbGPR);
    return;
  }
  if(aKey == 'v')
  {
    printf("Mode = parameter value selection (0-f,+,-,w,r)\n");
    gKbMode = 'v';
    return;
  }

  if(aKey == 'K')
  {
    printf("Mode = exposure mode selection (0[hdr]/1[vs]/2[s]/3[l])\n");
    gKbMode = 'K';
    return;
  } // if k pressed

  if(aKey == 'r')
  {
    printf("Set L/S ratio %f \n", gRatioLSExp);
    gKbMode = 'r';
    return;
  } // if Y pressed

  if(aKey == 'G')
  {
    if(gKbMode == 'G')
    {
      if(gKbChgain < (CHGAINSETS - 1))
      {
        gKbChgain++;
      } // if chgain < 6
      else
      {
        gKbChgain = 0;
      } // else from if chgain < 6
    }

    printf("chgain %d (%d,%d,%d,%d)\n", gKbChgain, gChGainTab[gKbChgain][0], gChGainTab[gKbChgain][1],
           gChGainTab[gKbChgain][2], gChGainTab[gKbChgain][3]);

    {
#ifdef OV10640
      // *** configure Channel Gain in Camera
      OV10640_Exposure_t lExp;
      memset(&lExp, 0, sizeof(lExp));
      lExp.mCsiIdx = CSI_IDX_CAM;

      for(uint32_t i = 0; i < 4; i++)
      {
        lExp.mpLongChnlGain[i]      = gChGainTab[gKbChgain][i];
        lExp.mpShortChnlGain[i]     = gChGainTab[gKbChgain][i];
        lExp.mpVeryShortChnlGain[i] = gChGainTab[gKbChgain][i];
      } // for all color compononents

      MAXIM_OV10640_ExposureSet(lExp);
#else
      ChgainSet(CSI_IDX_CAM, MAXIM_CHNL_ORIG_BC, (double)gChGainTab[gKbChgain][0],
                (double)((gChGainTab[gKbChgain][1] + gChGainTab[gKbChgain][2]) / 2), (double)gChGainTab[gKbChgain][3],
                4);
#endif
      gKbAWB = 0;
    }

    gKbMode = 'G';
    return;
  } // if 'G' pressed

  if(aKey == 'M') // TODO: update this snippet
  {
    if(gKbMode == 'M')
    {
      if(gExpMode >= 2)
      {
        gExpMode = 0;
      }
      else
      {
        gExpMode++;
      }
    }

    if(gExpMode == 0)
      printf("Exposure Mode = %d\n", gExpMode);
    if(!gExpMode)
    {
      printf("HDR ");
      gKbMode = 'K';
      KbAction('0');
    }
    else if(gExpMode == 1)
    {
      printf("Single \n");
      gKbMode = 'K';
      KbAction('3');
    }
    gKbMode = 'M';
    return;
  } // if M pressed

  if(aKey == 'b')
  {
    printf("Black Level = %d  (+/-)\n", gKbBlackLevel);
    gKbMode = 'b';
    return;
  } // if B pressed

  if(aKey == 'T')
  {
    printf("Color Saturation = %f  (+/-)\n", gKbSaturation);
    gKbMode = 'T';
    return;
  } // if T pressed

  if(aKey == 'R')
  {
    printf("Red Target = %f (+/-)\n", (float_t)gRedTarget / 256.0);
    gKbMode = 'R';
    return;
  } // if R pressed

  if(aKey == 'B')
  {
    printf("Blue Target = %f (+/-)\n", (float_t)gBlueTarget / 256.0);
    gKbMode = 'B';
    return;
  } // if B pressed

  if(aKey == 'H')
  {
    uint8_t lVal;
    if(gKbMode == 'H')
    {
      gHDRShift[gExpMode]++;
      if(gHDRShift[gExpMode] > 8)
        gHDRShift[gExpMode] = 0;
    }

    printf("HDR Long+Short Shift Right = %d\n", gHDRShift[gExpMode]);
    // set the IPU register in a frame/synced way
    setRegCoherent(HDR_ENGINE, 0, 0x7f, gHDRShift[gExpMode]);
    gKbMode = 'H';
    gKbAEC  = 0;
    //Turn off the camera AEC
    CAM_RegRead(CSI_IDX_CAM, 0x30fa, lVal);
    lVal &= 0xbf;
    CAM_RegWrite(CSI_IDX_CAM, 0x30fa, lVal);
    printf("AEC off\n");
    return;
  } // if H pressed

  if(aKey == 'A')
  {
    uint8_t lVal;
    gKbAEC = (gKbAEC + 1) % 3;

    if(gKbAEC == 0)
    {
      CAM_RegRead(CSI_IDX_CAM, 0x30fa, lVal);
      lVal &= 0xbf;
      CAM_RegWrite(CSI_IDX_CAM, 0x30fa, lVal);
      printf("AEC is off\n");
    }
    else if(gKbAEC == 1)
    {
      CAM_RegRead(CSI_IDX_CAM, 0x30fa, lVal);
      lVal &= 0xbf;
      CAM_RegWrite(CSI_IDX_CAM, 0x30fa, lVal);
      //Reuse internal exposure to make sure it is between the min/max
      OV10640_ExposureAllExpSet(gOVExposure[0], ExpTime_Long);
      OV10640_ExposureAllExpSet(gOVExposure[0], ExpTime_Short);
      OV10640_ExposureAllExpSet(gOVExposure[0], ExpTime_VShort);
      printf("AEC is executed by S32V\n");
    }
    else
    {
      CAM_RegRead(CSI_IDX_CAM, 0x30fa, lVal);
      lVal &= 0xbf;
      lVal |= (1 << 6);
      CAM_RegWrite(CSI_IDX_CAM, 0x30fa, lVal);
      printf("AEC is executed by the camera\n");
    }

    gKbMode = 'A';
    return;
  } // if A pressed

  if(aKey == 'W')
  {
    gKbAWB ^= 1;
    printf("AWB %s\n", (gKbAWB) ? "on" : "off");
    if(!gKbAWB)
    {
      // copy in current AWB value
      gChGainTab[CHGAINSETS - 1][0] = gChGainVal[0] / CHGAINFACTOR;
      gChGainTab[CHGAINSETS - 1][1] = gChGainVal[1] / CHGAINFACTOR;
      gChGainTab[CHGAINSETS - 1][2] = gChGainVal[2] / CHGAINFACTOR;
      gChGainTab[CHGAINSETS - 1][3] = gChGainVal[3] / CHGAINFACTOR;
      gKbChgain                     = CHGAINSETS - 1;
    }
    gKbMode = 'W';
    return;
  } // if A pressed

  if(aKey == 'I')
  {
    if(gKbMode == 'I')
    {
      gInLut++;
      if(gInLut > MAXINLUT)
      {
        gInLut = 0;
      }
      SetInLut(gInLut);
    }
    printf("Input  Lut %d\n", gInLut);
    gKbMode = 'I';
    return;
  }

  if(aKey == 'O')
  {
    if(gKbMode == 'O')
    {
      gOutLut++;
      if(gOutLut > MAXOUTLUT)
      {
        gOutLut = 0;
      }
      gOutLut1[gExpMode] = gOutLut;
    }

    else
    {
      printf("Output Lut %d\n", gOutLut);
    }
    gKbMode = 'O';
    return;
  } // if < pressed

  if(aKey == 'Y')
  {
    printf("Set Y de-noise level %d (+/- E:show edge C:camera DNR)\n", gKbYDenoise);
    gKbMode = 'Y';
    return;
  } // if Y pressed

  if(aKey == 'y')
  {
    if(gKbMode == 'y')
    {
      gKbYuvToRgb ^= 0x1;
    }
    printf("Toggle YUV420 to RGB conversion to %s\n", (gKbYuvToRgb) ? "Neon" : "A53");
    gKbMode = 'y';
    return;
  } // if y pressed

  if(aKey == 'J')
  {
    uint8_t lVal;
    gKbMode = 'J';
    gKbAEC  = 0;
    //Turn off the AEC off the camera
    CAM_RegRead(CSI_IDX_CAM, 0x30fa, lVal);
    lVal &= 0xbf;
    CAM_RegWrite(CSI_IDX_CAM, 0x30fa, lVal);
    gKbAWB = 0;
    printf(
        "AEC off\n"
        "AWB off\n"
        " E ....... select Exposure\n"
        " P ....... select Parameter (time, ag, dg, cg)\n"
        " w/ret ... write parameter\n"
        "  + .......increase the parameter and update it directly \n"
        "  - .......decrease the parameter and update it directly \n"
        "Manual change of camera parameter: %s - %s\n",
        gExposureName[gKbExpToChange], gParamName[gKbExpParamChange]);
    OV10640_Exposure_Print(CSI_IDX_CAM, SourceCamera);
    return;
  }

  if(aKey == 'C')
  {
    if(gKbMode == 'C')
    {
      if(gKbCCM < (CCM_SETS - 1))
      {
        gKbCCM++;
      } // if table < CCM_SET
      else
      {
        gKbCCM = 0;
      }
    }

    {
      printf(
          "CCM table %d\n"
          "%6.3f %6.3f %6.3f\n"
          "%6.3f %6.3f %6.3f\n"
          "%6.3f %6.3f %6.3f\n",
          gKbCCM, gKbCcmMatrix[gKbCCM][0], gKbCcmMatrix[gKbCCM][1], gKbCcmMatrix[gKbCCM][2], gKbCcmMatrix[gKbCCM][3],
          gKbCcmMatrix[gKbCCM][4], gKbCcmMatrix[gKbCCM][5], gKbCcmMatrix[gKbCCM][6], gKbCcmMatrix[gKbCCM][7],
          gKbCcmMatrix[gKbCCM][8]);
    }

    CcmCscMatrix(gCcmCscMatrix, gKbCcmMatrix[gKbCCM], gRgb2YuvMatrix, gKbSaturation);
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

    gKbMode = 'C';
    return;
  } // if C pressed

  if(strchr(" \n\r", aKey))
  {
    printf("Clear key mode\n");
    gKbMode = 0;
    return;
  } // if space pressed

  printf("unknown key %d %c\n", aKey, aKey);

} // KbAction()

//***************************************************************************

static SEQ_LIB_RESULT setRegCoherent(uint16_t aIsIPUS,
                                     uint16_t aEngNr,
                                     uint16_t aDirect,
                                     uint16_t aIndex,
                                     uint32_t aData)
{
  SEQ_LIB_RESULT err  = SEQ_LIB_SUCCESS;
  uint32_t       addr = (uint32_t)(uintptr_t)&IPUS_0;

  if(gCoherentRegIx >= MAX_COHERENT_REGS)
  {
    return (SEQ_LIB_FAILURE);
  }

  if(aIsIPUS)
  {
    switch(aEngNr)
    {
      case 0:
        addr = (uint32_t)(uintptr_t)&IPUS_0;
        break;

      case 1:
        addr = (uint32_t)(uintptr_t)&IPUS_1;
        break;

      case 2:
        addr = (uint32_t)(uintptr_t)&IPUS_2;
        break;

      case 3:
        addr = (uint32_t)(uintptr_t)&IPUS_3;
        break;

      case 4:
        addr = (uint32_t)(uintptr_t)&IPUS_4;
        break;

      case 5:
        addr = (uint32_t)(uintptr_t)&IPUS_5;
        break;

      case 6:
        addr = (uint32_t)(uintptr_t)&IPUS_6;
        break;

      case 7:
        addr = (uint32_t)(uintptr_t)&IPUS_7;
        break;

      default:
        err = SEQ_LIB_FAILURE;
    }
  }
  else
  {
    switch(aEngNr)
    {
      case 0:
        addr = (uint32_t)(uintptr_t)&IPUV_0;
        break;

      case 1:
        addr = (uint32_t)(uintptr_t)&IPUV_1;
        break;

      case 2:
        addr = (uint32_t)(uintptr_t)&IPUV_2;
        break;

      case 3:
        addr = (uint32_t)(uintptr_t)&IPUV_3;
        break;

      default:
        err = SEQ_LIB_FAILURE;
    }
  }
  if(err == SEQ_LIB_FAILURE)
  {
    printf("Selected wrong IPU for resister updaten\n");
    return err;
  }
  else
  {
    addr -= SEQ_ADDR_OFF;

    gRegVal[gCoherentRegIx].mEngBase = addr;
    gRegVal[gCoherentRegIx].mIndex   = aIndex;
    gRegVal[gCoherentRegIx].mData    = aData;
    gRegVal[gCoherentRegIx].mDirect  = aDirect;

    gCoherentRegIx++;
  }

  return (err);

} // setRegCoherent()

//***************************************************************************

static SEQ_LIB_RESULT setRegCoherentClose(void)
{
  SEQ_LIB_RESULT err = SEQ_LIB_SUCCESS;
  SEQ_RegList_t  lRegList;

  lRegList.mCnt       = gCoherentRegIx;
  lRegList.mDirection = IPU_REGLIST_WRITE;
  lRegList.mpData     = gRegVal;

  err = SEQ_RegListSet(&lRegList);

  gCoherentRegIx = 0;

  return (err);

} // setRegCoherentClose()

//***************************************************************************
void AWB(uint8_t* pEmbd)
{
  OV10640_Exposure_t lExp;
  uint32_t           lAWB_Max_Mean;
  uint32_t           lAWB_Min_Gain;
  int32_t            lChGainUpdate;

  uint32_t lAddr = 0x9; // offset of channel mean value
  uint16_t lVal16;
  uint8_t  i, j;

  static uint8_t lAWBState = 0; // 0 = update AWB; 1=wait for values to become activce

  // do only every 3rd frame an update
  if(lAWBState != 0)
  {
    lAWBState--;
    return;
  }
  lAWBState = 2;

  lAWB_Max_Mean = 0;
  lAWB_Min_Gain = 0xffffffff;
  for(i = 0; i < 4; i++)
  {
    j = gChannelMap[i];
    if(pEmbd != NULL)
    {
#if(EMBEDDED_DATA == 1)
      lVal16 = (*(pEmbd + ((lAddr++) * 4 + 3))) << 8;
      lVal16 |= (*(pEmbd + ((lAddr++) * 4 + 3)));
#else
      CAM_RegRead(CSI_IDX_CAM, 0x4000 + (lAddr++), lVal8);
      lVal16 = lVal8 << 8;
      CAM_RegRead(CSI_IDX_CAM, 0x4000 + (lAddr++), lVal8);
      lVal16 |= lVal8;
#endif // EMBEDDED_DATA
    }
    else
    {
      lVal16 = 0;
    }
    gAWB_Mean[j] = (lVal16) >> 5;
    gAWB_Mean[j] <<= (lVal16 & 0x1f);
    if(j == 0)
    {
      gAWB_Mean[j] = (uint32_t)((float)gAWB_Mean[j] * 256.0 / (float)gRedTarget);
    }
    else if(j == 3)
    {
      gAWB_Mean[j] = (uint32_t)((float)gAWB_Mean[j] * 256.0 / (float)gBlueTarget);
    }
    if(gAWB_Mean[j] > lAWB_Max_Mean)
      lAWB_Max_Mean = gAWB_Mean[j];
  }

  if(gKbAWB == 1)
  {
    memset(&lExp, 0, sizeof(lExp));
    lExp.mCsiIdx = CSI_IDX_CAM;

#define CHGAIN_TAU 5
    // *** compute updated values before normalization
    for(i = 0; i < 4; i++)
    {
      lChGainUpdate = ((uint32_t)( //256.0*CHGAINFACTOR/lAWB_Min_Gain*
          ((float)gChGainVal[i]) * lAWB_Max_Mean / gAWB_Mean[i]));
      lChGainUpdate -= gChGainVal[i];
      lChGainUpdate >>= CHGAIN_TAU;

      gChGainVal[i] += lChGainUpdate;

      if(gChGainVal[i] < lAWB_Min_Gain)
        lAWB_Min_Gain = gChGainVal[i];
    }

    if(lAWB_Min_Gain == 0)
    {
      printf("lAWB_Min_Gain = %d\n", lAWB_Min_Gain);
      lAWB_Min_Gain = 256 * CHGAINFACTOR;
    }
    for(i = 0; i < 4; i++)
    {
      // normalize to 256.0*CHGAINFACTOR
      gChGainVal[i] = (uint32_t)((float)gChGainVal[i] * 256.0 * CHGAINFACTOR / lAWB_Min_Gain);

      if(gChGainVal[i] > (gChGainMax[i] * CHGAINFACTOR))
        gChGainVal[i] = (gChGainMax[i] * CHGAINFACTOR);
      if(gChGainVal[i] < (256 * CHGAINFACTOR))
        gChGainVal[i] = (256 * CHGAINFACTOR);

      lExp.mpLongChnlGain[i] = lExp.mpShortChnlGain[i] = lExp.mpVeryShortChnlGain[i] = gChGainVal[i] / CHGAINFACTOR;
    }
    MAXIM_OV10640_ExposureSet(lExp);
  } // gKbAWB

  if(gKbLog == 4)
  {
    printf("AWB - R:%f, Gr:%f, Gb:%f, B:%f\n", gChGainVal[0] / (256.0 * CHGAINFACTOR),
           gChGainVal[1] / (256.0 * CHGAINFACTOR), gChGainVal[2] / (256.0 * CHGAINFACTOR),
           gChGainVal[3] / (256.0 * CHGAINFACTOR));
  }
}

/* EOF */
