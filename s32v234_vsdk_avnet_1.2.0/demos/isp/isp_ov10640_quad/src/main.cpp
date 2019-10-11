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

#ifndef __STANDALONE__
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#endif // #ifdef __STANDALONE__
#include <string.h>
#include <math.h>

#include "../include/mymemcpy.h"
#include "oal.h"
#include "vdb_log.h"
#include "isp_cam_maxim.h"
#include "sdi.hpp"

#ifdef __STANDALONE__
#include "frame_output_dcu.h"
#include "uartlinflex_driver.h"
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"
#endif // else from #ifdef __STANDALONE__

#include "isp_seq.h"
//#include "csi_func.h"
#include "isp_cam_ov10640.h"
//#include "ov10640_func.h"
#ifdef __STANDALONE__
#include "uartlinflex_driver.h"
#endif // ifdef __STANDALONE__

#include "ov10640_12bcomb_surround_c.h" // graph defines

#include "iofiles.h" // *** from graph ***
#include <common_helpers.h>

//***************************************************************************
// constants
//***************************************************************************

#if(4 == PIX_DEPTH)
#define USE_RGBA
#endif

// ***** Graph Defines *******

#if(MIPI_PORT == 1)
#define CSI_IDX_CAM CSI_IDX_1
#else
#define CSI_IDX_CAM CSI_IDX_0
#endif

#define TEE_ENGINE 1, 0
#define ENDIAN_ENGINE 1, 0

#define DEBAYER_ENGINE0 1, 1
#define DEBAYER_ENGINE1 1, 4

#define RGB2Y_ENGINE0 0, 0
#define RGB2Y_ENGINE1 0, 2
#define OUTLUT_IPUS0 2
#define OUTLUT_IPUS1 3
#define RGB2Y_LUT_ENGINE0 1, OUTLUT_IPUS0
#define OUTLUT_ENGINE0 RGB2Y_LUT_ENGINE0
#define RGB2Y_LUT_ENGINE1 1, OUTLUT_IPUS1
#define OUTLUT_ENGINE1 RGB2Y_LUT_ENGINE1
#if(OUTLUT_IPUS0 == 2)
#define IPUS_OUTLUT0 IPUS_2
#define IPUS_OUTLUT1 IPUS_3
#else
#define IPUS_OUTLUT0 IPUS_3
#define IPUS_OUTLUT1 IPUS_2
#endif

#define TONEMAP_ENGINE0 0, 1
#define TONEMAP_ENGINE1 0, 3

// Possible to set input resolution (must be supported by the DCU)
#define SCR_WIDTH ((DISPLAY_PIXELS + 31) & ~31) ///< width of DDR buffer in pixels
#define SCR_HEIGHT (DISPLAY_LINES)              ///< height of DDR buffer in pixels

#define DDR_OUT_BUFFER_CNT 3 ///< number of DDR buffers per ISP stream
#ifdef USE_RGBA
#define CHNL_CNT io::IO_DATA_CH4
#else
#define CHNL_CNT io::IO_DATA_CH3
#endif

// ***** Run Control and Logging Defines *******

#define IPU_DUMP_STATE
#define DO_IPU_PROF
#define DO_CHGAIN_LOG
#define DO_BUFFER_LOG

#if defined(DO_IPU_PROF) || defined(DO_EXP_LOG) || defined(DO_CHGAIN_LOG)
#ifdef ISP_FRAME_RATE_MEASURE_NUM
#define LOOP_NUM ISP_FRAME_RATE_MEASURE_NUM
#else
#define LOOP_NUM 30
#endif
#else
#define LOOP_NUM 250
#endif

#ifdef DO_IPU_PROF
#define IPU_PROF_FRAMES ((LOOP_NUM < 10) ? LOOP_NUM : 10)
#endif

//***************************************************************************
// macros
//***************************************************************************

#ifdef __STANDALONE__
extern "C" {
unsigned long get_uptime_microS(void);
}
#define GETTIME(time) (*(time) = get_uptime_microS())
#else // #ifdef __STANDALONE__

#define GETTIME(time)                                                                                                  \
  {                                                                                                                    \
    struct timeval lTime;                                                                                              \
    gettimeofday(&lTime, 0);                                                                                           \
    *time = (lTime.tv_sec * 1000000 + lTime.tv_usec);                                                                  \
  }
#endif // else from #ifdef __STANDALONE__

#if defined(__STANDALONE__) && defined(__ARMV8)
extern "C" {
void flush_dcache_range(void*, unsigned int);
}
#endif

//***************************************************************************
// types
//***************************************************************************
struct AppContext
{
  sdi_grabber* mpGrabber; ///< pointer to grabber instance
  sdi_FdmaIO*  mpFdma;    ///< pointer to fdma object
  vsdk::UMat   mFrameIsp; ///< current frame from ISP

#ifndef __STANDALONE__
  io::FrameOutputV234Fb mDcu;
#else
  io::FrameOutputDCU mDcu;
#endif
  vsdk::UMat mDisplayBuffer; ///< buffer for pixel data to be displayed

  unsigned long mTimes;

  // ** variables for exposure and channel gain control
  uint8_t  mKbKey;
  uint8_t  mKbMode;    // 0=off, 'k'= knee point, r=redgain, b=bluegain
  uint8_t  mKbLog;     // 0=off
  int16_t  mKbXoffset; // 0 - ((4x1280)-1920 = 3200)
  uint32_t mKbI2c;     // IIC command 0xaaaavv  a=address v=value
  uint8_t  mKbChgain;  // 1=on 0=off
  uint8_t  mKbShift;   // 0-8
  uint8_t  mKbEndian;  // swap endianess
  uint8_t  mKbOutLut;  // 12 to 8
  uint8_t  mKbDisplay; // 0-1

  // ** event counters and flags **
  bool     mError;        ///< to signal ISP problems
  uint32_t mFrmCnt;       ///< number of frames grabbed by the app
  uint32_t mFrameDoneCnt; ///< number of frames done events
  uint32_t mFrameDoneCntLast;
}; // struct AppContext

//***************************************************************************
// local functions
//***************************************************************************

/************************************************************************/
/** Configure ISP preprocessing.
  *
  * \param  arContext application context structure
  *
  * \return 0 if all ok, <0 otherwise
  ************************************************************************/
static int32_t IspConfig(AppContext& arContext);

/************************************************************************/
/** Select Look Up Table for output
  *
  * \param  aLut          type of Look Up Tabel
  *                       0 = Linear
  *                       1 = 12 To 8 bit
  *                       2 = 16 To 8 bit
  *
  * \return 0 if all ok, <0 otherwise
  ************************************************************************/
static int32_t SetOutLut(uint8_t aLut);

/************************************************************************/
/** Status logging.
  *
  * \param  arContext application context structure
  *
  * \return 0 if all ok, <0 otherwise
  ************************************************************************/
static int32_t LogStatus(AppContext& arContext);

/************************************************************************/
/** Prepare everything before executing the main functionality .
  *
  * \param arContext structure capturing the context of the application
  *
  * \return 0 if all ok, <0 otherwise
  ************************************************************************/
static int32_t Prepare(AppContext& arContext);

/************************************************************************/
/** Initial setup of application context.
  *
  * \param arContext structure capturing the context of the application
  ************************************************************************/
static void ContextInit(AppContext& arContext);

/************************************************************************/
/** Prepares required libraries.
  *
  * \param arContext structure capturing the context of the application
  *
  * \return 0 if all ok, != 0 otherwise
  ************************************************************************/
static int32_t LibsPrepare(AppContext& arContext);

/************************************************************************/
/** Prepares DDR buffers.
  *
  * \param arContext structure capturing the context of the application
  *
  * \return 0 if all ok, != 0 otherwise
  ************************************************************************/
static int32_t DdrBuffersPrepare(AppContext& arContext);

/************************************************************************/
/** Execute main functionality of the application.
  *
  * \param arContext structure capturing the context of the application
  *
  * \return 0 if all ok, <0 otherwise
  ************************************************************************/
static int32_t Run(AppContext& arContext);

/************************************************************************/
/** Cleanup all resources before application end.
  *
  * \param arContext structure capturing the context of the application
  *
  * \return 0 if all ok, <0 otherwise
  ************************************************************************/
static int32_t Cleanup(AppContext& arContext);

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
static int32_t SigintSetup(void);

/************************************************************************/
/** Checks if key was pressed or not.
  *
  * \return 0 if no key pressed
  *         >0 if key pressed
  ************************************************************************/
static int KeyDown(void);

/************************************************************************/
/** Reads one character from standard input.
  *
  * If no key available in buffer function doesn't block and returns EOF.
  *
  * \return ASCI code of the read character
  *         EOF if no character available
  ************************************************************************/
static int GetCharNonBlock(void);

/************************************************************************/
/** Sets terminal to blocking character input.
  *
  ************************************************************************/
static void TermBlockSet(void);

/************************************************************************/
/** Sets terminal to nonblocking character input.
  *
  ************************************************************************/
static void TermNonBlockSet(void);

/************************************************************************/
/** Sets the number of I2c repeat
  *
  ************************************************************************/
static CAM_LIB_RESULT SetI2cRepeat(uint32_t aI2cRepeat);

#endif // #ifndef __STANDALONE__

/************************************************************************/
/** User defined call-back function for Sequencer event handling.
  *
  * \param  aEventType type of the event received from ISP
  * \param  apUserVal  user defined value pointer
  ************************************************************************/
static void SeqEventHandler(uint32_t aEventType, void* apUserVal);

/************************************************************************/
/** User defined call-back function for Sequencer event handling.
  *
  * \param  arContext application context structure
  ************************************************************************/
static void KeyboardInputProcess(AppContext& arContext);

/************************************************************************/
/** Reads character from command line.
 *
 * Hides differences between SA and Linux.
 *
 * \return character code being pressed
 ************************************************************************/
static char GetChar(void);

/************************************************************************/
/** Reads character from command line.
 *
 * \param  arContext application context structure
 ************************************************************************/
static void KbAction(AppContext& arContext);

//***************************************************************************
// static variables
//***************************************************************************

// channel gain presets
uint16_t sppChGainVal[7][4] = {{400, 256, 256, 400}, {400, 256, 256, 300}, {300, 256, 256, 400}, {256, 400, 400, 256},
                               {256, 300, 400, 256}, {256, 400, 300, 256}, {256, 256, 256, 256}};

static bool sStop = false; ///< to signal Ctrl+c from command line

//***************************************************************************

int main(int argc, char** argv)
{
  int lRet = 0;

  AppContext lContext;

  //*** process command line parameters ***
  const char helpMsg_str[] =
      "\n**************************************************************\n"
      "** Omnivision Ov10640 quad demo using Maxim Ser/Des HW setup\n"
      "** Description:\n"
      "**  o Maxim 9286 deserializer board with 4xOmnivision Ov10640\n"
      "**    cameras each with 9271 serializer (on MipiCsi_0) expected as\n"
      "**    image input.\n"
      "**  o ISP does debayering and HDR processing.\n"
      "**  o Resulting image is displayed live using DCU.\n"
      "**\n"
      "** Usage:\n"
      "**  o no cmd line parameters available.\n"
      "**  o runtime control using nonblocking keyboard input.\n"
      "**    press 'h' for available commands listing.\n"
      "**\n"
      "**************************************************************\n\n";
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg_str);
  if(idxHelp < 0)
  { // print help message even if no help option is provided by the user
    printf("%s", helpMsg_str);
  }

#ifndef __STANDALONE__
  // setup Ctrl+C handler
  if(SigintSetup() != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to register Ctrl+C signal handler.");
    return -1;
  } // if SigintSetup() failed
  printf("Press Ctrl+C to terminate the demo.\n");

  // set terminal to nonblock input ***
  TermNonBlockSet();
#endif // #ifndef __STANDALONE__

#ifndef __STANDALONE__
  fflush(stdout);
  sleep(1);
#endif // ifndef __STANDALONE__

  if(Prepare(lContext) == 0)
  {
    if(Run(lContext) != 0)
    {
      printf("Demo execution failed.\n");
      lRet = -1;
    } // if Run() failed
  }   // if Prepare() ok
  else
  {
    printf("Demo failed in preparation phase.\n");
    lRet = -1;
  } // else from if Prepare() ok

  if(Cleanup(lContext) != 0)
  {
    printf("Demo failed in cleanup phase.\n");
    lRet = -1;
  } // if cleanup failed

  return lRet;
} // main

//***************************************************************************

static int32_t Prepare(AppContext& arContext)
{
  // init app context
  ContextInit(arContext);

  // Set I2cRepeat in Linux
#ifndef __STANDALONE__
  if(CAM_LIB_FAILURE == SetI2cRepeat(2))
  {
    VDB_LOG_ERROR("Failed to set the number of I2c repeat\n");
  }
#endif

  // enable LIBS
  if(LibsPrepare(arContext) != 0)
  {
    printf("Failed to prepare libraries.\n");
    return -1;
  } // if failed to configure decoder
  // enable OAL

  if(DdrBuffersPrepare(arContext) != 0)
  {
    printf("Failed to prepare DDR buffers.\n");
    return -1;
  } // if fialed to prepare DDR buffers

  // *** prestart grabber ***
  if(arContext.mpGrabber->PreStart() != LIB_SUCCESS)
  {
    printf("Failed to prestart the grabber.\n");
    return -1;
  } // if PreStart() failed

  // *** configure camera ***
  if(IspConfig(arContext) != 0)
  {
    printf("Failed to prestart the grabber.\n");
    return -1;
  } // if CamConfig() failed

  // *** install sequencer event callback ***
  if(arContext.mpGrabber->SeqEventCallBackInstall(&SeqEventHandler, &arContext) != LIB_SUCCESS)
  {
    printf("Failed to install Sequencer event callback.\n");
    return -1;
  } // if callback setup failed

  // *** initial keyboard commands ***
  arContext.mKbKey = 'h';
  KbAction(arContext); // print keyboard help

  return 0;
} // Prepare()

//***************************************************************************

static void ContextInit(AppContext& arContext)
{
  /// *** object pointers ***
  arContext.mpGrabber = NULL;
  arContext.mpFdma    = NULL;

  // ** event counters and flags **
  arContext.mFrameDoneCnt     = 0;
  arContext.mFrameDoneCntLast = 0;
  arContext.mFrmCnt           = 0;
  arContext.mTimes            = 0;
  arContext.mError            = false;

  // keyboard interaction
  arContext.mKbKey     = 0;
  arContext.mKbMode    = 0;
  arContext.mKbLog     = 0;
  arContext.mKbXoffset = 0;
  arContext.mKbI2c     = 0;
  arContext.mKbChgain  = 0;
  arContext.mKbShift   = 0;
  arContext.mKbEndian  = 1;
  arContext.mKbOutLut  = 2;
  arContext.mKbDisplay = 1;
} // ContextInit()

//***************************************************************************

static int32_t LibsPrepare(AppContext& arContext)
{
  // *** Initialize SDI ***
  if(sdi::Initialize(0) != LIB_SUCCESS)
  {
    printf("Failed to initialzie SDI.\n");
    return -1;
  } // if failed to initialize SDI

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

  // get IOs
  arContext.mpFdma = (sdi_FdmaIO*)arContext.mpGrabber->IoGet(SEQ_OTHRIX_FDMA);
  if(arContext.mpFdma == NULL)
  {
    printf("Failed to get FDMA object.\n");
    return -1;
  } // if no FDMA object

  return 0;
} // LibsPrepare(AppContext &arContext)

//***************************************************************************

static int32_t DdrBuffersPrepare(AppContext& arContext)
{
  // *** prepare FDMA channels ***
  // goal is to program FDMA to merge the 2 output streams (per line) into one
  // huge DDR buffer with width = 4*1280 pix and hight = 720 lines

  // *** 4*1280x720 RGB frame ***
  SDI_ImageDescriptor lQuadFrmDesc;
  SDI_DdrBufferArr    lQuadBuffArr;

  // tranform the full DDR image geometry to having the stride of
  // 2x(4*1280) pix with 720/2 lines
  // this allows easy division into 2 rectangular ROIs thus merging
  // the 2 FDMA channel outputs into one DDR buffer
  lQuadFrmDesc = SDI_ImageDescriptor(OUT_PIXELS * 2, // two paths
                                     OUT_LINES2_0,
#ifdef USE_RGBA
                                     RGBA8888
#else
                                     RGB888
#endif
  );

  // allocate contigous buffers for the full DDR image
  if(lQuadBuffArr.Allocate(lQuadFrmDesc, DDR_OUT_BUFFER_CNT) != LIB_SUCCESS)
  {
    printf("Failed to allocate full DDR image buffer.\n");
    return -1;
  } // if frame buffer alloc failed

  // *** create rois for both FDMA ROI destination buffers ***
  // FDMA channel 0 ROI
  SDI_DdrBufferArr lQuadBuffArrRoi;
  lQuadBuffArrRoi = lQuadBuffArr.Roi(0, 0, OUT_PIXELS, OUT_LINES2_0);

  lQuadBuffArrRoi.ChannelIdxSet(FDMA_IX_ISP_OUTPUT0);
  if(arContext.mpFdma->DdrBuffersSet(lQuadBuffArrRoi) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor.\n");
    return -1;
  } // if frame descriptor setup failed

  // FDMA channel 1 FULL DDR data ROI
  lQuadBuffArrRoi = lQuadBuffArr.Roi(OUT_PIXELS, // Offset
                                     0, OUT_PIXELS, OUT_LINES2_1);
  lQuadBuffArrRoi.ChannelIdxSet(FDMA_IX_ISP_OUTPUT1);
  if(arContext.mpFdma->DdrBuffersSet(lQuadBuffArrRoi) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor.\n");
    return -1;
  } // if frame descriptor setup failed

  // *** allocate display buffer ***
  arContext.mDisplayBuffer = vsdk::UMat(SCR_HEIGHT, SCR_WIDTH, VSDK_CV_8UC3);
  if(arContext.mDisplayBuffer.empty())
  {
    VDB_LOG_ERROR("Failed to allocate DDR display buffer.");
    return (-1);
  } // if buffer allocation failed

  return 0;
} // DdrBuffersPrepare(AppContext &arContext)

//***************************************************************************

static int32_t Run(AppContext& arContext)
{
  //*** Init DCU Output ***
  arContext.mDcu.Init(SCR_WIDTH, SCR_HEIGHT, io::IO_DATA_DEPTH_08, io::IO_DATA_CH3
#ifdef USE_RGBA
                      ,
                      DCU_BPP_32
#endif
  );

  // *** start grabbing ***
  GETTIME(&arContext.mTimes);
  if(arContext.mpGrabber->Start() != LIB_SUCCESS)
  {
    printf("Failed to start the grabber.\n");
    return -1;
  } // if Start() failed

  SDI_Frame lFrame[FDMA_IX_END];
  // *** grabbing/processing loop ***
  for(;;)
  {
    for(uint32_t lLoop = 0; lLoop < LOOP_NUM; lLoop++)
    {
      // grab from all channels to keep buffers in sync
      for(int lFdmaChannel = FDMA_IX_ISP_OUTPUT0; lFdmaChannel < FDMA_IX_END; lFdmaChannel++)
      {
        lFrame[lFdmaChannel] = arContext.mpGrabber->FramePop(lFdmaChannel);
        if(lFrame[lFdmaChannel].mUMat.empty())
        {
          printf("Failed to pop ISP output frame.\n");
          arContext.mError = true;
          return -1;
        } // if pop failed
      }   // for all FDMA channels
      arContext.mFrameIsp = lFrame[FDMA_IX_ISP_OUTPUT0].mUMat;

      // *** act on keyboard input and display ***
      KeyboardInputProcess(arContext);

      // push to all channels to keep buffers in sync
      for(int lFdmaChannel = FDMA_IX_ISP_OUTPUT0; lFdmaChannel < FDMA_IX_END; lFdmaChannel++)
      {
        if(arContext.mpGrabber->FramePush(lFrame[lFdmaChannel]) != LIB_SUCCESS)
        {
          printf("Failed to push back the frame.\n");
          arContext.mError = true;
          return -1;
        } // if push failed
      }   // for all FDMA channels

      arContext.mFrmCnt++;

      // *** if stop requested ***
      if(sStop)
      {
        return 0; // break if Ctrl+C pressed
      }           // if Ctrl+C
    }             // if new frame read

    // *** do logging ***
    LogStatus(arContext);

    // *** if stop requested ***
    if(sStop)
    {
      return 0; // break if Ctrl+C pressed
    }           // if Ctrl+C
  }             // for ever

  return 0;
} // Run()

//***************************************************************************

static int32_t Cleanup(AppContext& arContext)
{
  int32_t lRet = 0;

  //Set I2cRepeat to default value in Linux
#ifndef __STANDALONE__
  if(CAM_LIB_FAILURE == SetI2cRepeat(0))
  {
    VDB_LOG_ERROR("Failed to set the number of I2c repeat to default value\n");
  }
#endif

  if(arContext.mpGrabber != NULL)
  {
    if(arContext.mpGrabber->Stop())
    {
      printf("Failed to stop the grabber.\n");
      lRet = -1;
    } // if grabber stop failed

    if(arContext.mpGrabber->Release())
    {
      printf("Failed to release grabber resources.\n");
      lRet = -1;
    } // if grabber resources not released

    delete(arContext.mpGrabber);
    arContext.mpGrabber = NULL;
  } // if grabber exists

#ifdef __STANDALONE__
  for(;;)
    ; // *** don't return ***
#else
  // *** set terminal back to block mode ***
  TermBlockSet();
#endif // #ifdef __STANDALONE__

  if(sdi::Close(0) != LIB_SUCCESS)
  {
    printf("Failed to terminate use of SDI.\n");
    lRet = -1;
  } // if SDI use termination failed

  return lRet;
} // Cleanup()

//***************************************************************************

void KeyboardInputProcess(AppContext& arContext)
{
  if(arContext.mKbDisplay)
  {
    // copy buffer to display
    {
      vsdk::Mat output_mat = arContext.mDisplayBuffer.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
      vsdk::Mat input_mat  = arContext.mFrameIsp.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);

      uint8_t* lpSrc  = (uint8_t*)input_mat.data;
      uint8_t* lpDest = (uint8_t*)output_mat.data;
      uint32_t y;

      lpSrc += arContext.mKbXoffset * CHNL_CNT;
      for(y = 0; y < OUT_LINES; y++)
      {
        mymemcpy(lpDest, lpSrc, DISPLAY_STRIDE8_RGB);
        lpSrc += OUT_STRIDE8_RGB;
        lpDest += DISPLAY_STRIDE8_RGB;
      } // for all lines
    }   // copy buffer to display

    // output frame
    arContext.mDcu.PutFrame(arContext.mDisplayBuffer);
  } // if display enabled

  // *** get keyboard input ***
  arContext.mKbKey = GetChar();
  //printf("char %c\n", gKbKey);
  if(arContext.mKbKey)
  {
    KbAction(arContext);
  }
  arContext.mKbKey = 0;

  // do exposure control and white balancing
  if(arContext.mKbLog == 4)
  {
    printf("Channel gain mode %d (%d,%d,%d,%d)\n", arContext.mKbChgain, sppChGainVal[arContext.mKbChgain][0],
           sppChGainVal[arContext.mKbChgain][1], sppChGainVal[arContext.mKbChgain][2],
           sppChGainVal[arContext.mKbChgain][3]);
  }

} // KeyboardInputProcess()

//***************************************************************************

static int32_t IspConfig(AppContext& arContext)
{
  // *** configure Channel Gain in Camera
  OV10640_Exposure_t lExp;
  memset(&lExp, 0, sizeof(lExp));
  lExp.mCsiIdx = CSI_IDX_CAM;

  for(uint32_t i = 0; i < 4; i++)
  {
    lExp.mpLongChnlGain[i]      = sppChGainVal[arContext.mKbChgain][i];
    lExp.mpShortChnlGain[i]     = sppChGainVal[arContext.mKbChgain][i];
    lExp.mpVeryShortChnlGain[i] = sppChGainVal[arContext.mKbChgain][i];
  } // for all color compononents

  if(MAXIM_OV10640_ExposureSet(lExp) != CAM_LIB_SUCCESS)
  {
    printf("Failed to set camera exposure config.\n");
    return -1;
  } // if SONY_ExposureSet failed

  // *** debayer select start line
  // start with even line
  if(seq_setReg(DEBAYER_ENGINE0, 0, 0x7e, 1) != SEQ_LIB_SUCCESS) // GPR14 (IPUS)
  {
    return -1;
  }                                                              // if seq_setReg() failed
  if(seq_setReg(DEBAYER_ENGINE0, 0, 0x7f, 0) != SEQ_LIB_SUCCESS) // GPR15 (IPUS)
  {
    return -1;
  } // if seq_setReg() failed
  // start with odd line
  if(seq_setReg(DEBAYER_ENGINE1, 0, 0x7e, 1) != SEQ_LIB_SUCCESS) // GPR14 (IPUS)
  {
    return -1;
  }                                                               // if seq_setReg() failed
  if(seq_setReg(DEBAYER_ENGINE1, 0, 0x7f, -1) != SEQ_LIB_SUCCESS) // GPR15 (IPUS)
  {
    return -1;
  } // if seq_setReg() failed

  // *** rgb2y_lut kernel ***
  if(SetOutLut(arContext.mKbOutLut) != 0)
  {
    return -1;
  }

  return 0;

} // IspConfig();

//***************************************************************************
static int32_t SetOutLut(uint8_t aLut)

{
  uint32_t val0;
  uint32_t val1;
  uint16_t lutcnt = 0;

#ifdef __STANDALONE__
#endif // __STANDALONE__
#define OUTLUT_OFFSET 0
#define OUTLUT_SIZE 4096

  // LUTA
  if(seq_setReg(OUTLUT_ENGINE0, 1, 0x78, ((OUTLUT_OFFSET) >> 1)) != SEQ_LIB_SUCCESS)
  {
    return -1;
  } // if seq_setReg() failed
  if(seq_setReg(OUTLUT_ENGINE1, 1, 0x78, ((OUTLUT_OFFSET) >> 1)) != SEQ_LIB_SUCCESS)
  {
    return -1;
  } // if seq_setReg() failed

  if(aLut == 0)
  {
    printf("output lut: copy\n");
    while(lutcnt < OUTLUT_SIZE)
    {
      val0 = 256; //((lutcnt++)<<4);
      lutcnt++;
      val1 = 256; //((lutcnt++)<<4);
      lutcnt++;

      // LUTD (IPUS)
      if(seq_setReg(OUTLUT_ENGINE0, 1, 0x7C, val0 | (val1 << 16)) != SEQ_LIB_SUCCESS)
      {
        return -1;
      } // if seq_setReg() failed
      if(seq_setReg(OUTLUT_ENGINE1, 1, 0x7C, val0 | (val1 << 16)) != SEQ_LIB_SUCCESS)
      {
        return -1;
      } // if seq_setReg() failed
    }   // while lutcnt < OUTLUT_SIZE
  }
  else if(aLut == 1)
  {
#define GAMMA 0.8
#define GAMMA_SCALE (0xFFF0)
    printf("output lut: gamma %f\n", GAMMA);
    while(lutcnt < OUTLUT_SIZE)
    {
      if(!lutcnt)
        val0 = 0;
      else
      {
        val0 = (uint32_t)(256.0 * // 8.8 factors
                          ((float)GAMMA_SCALE) *
                          pow(((float)lutcnt) / ((float)(OUTLUT_SIZE - 1)),
                              GAMMA) /
                          (float(lutcnt << 4)) // 15 bit input values
        );
      }
      lutcnt++;
      val1 = (uint32_t)(256.0 * // 8.8 factors
                        ((float)GAMMA_SCALE) *
                        pow(((float)lutcnt) / ((float)(OUTLUT_SIZE - 1)),
                            GAMMA) /
                        (float(lutcnt << 4)) // 15 bit input values
      );
      lutcnt++;
      //printf("inlut4k[%d]=%d\n"
      //       "inlut4k[%d]=%d\n",
      //       lutcnt-2, val0,
      //       lutcnt-1, val1
      //       );
      // LUTD (IPUS)
      if(val0 > 0xffff)
      {
        val0 = 0xffff;
      }
      if(val1 > 0xffff)
      {
        val1 = 0xffff;
      }

      if(seq_setReg(OUTLUT_ENGINE0, 1, 0x7C, val0 | (val1 << 16)) != SEQ_LIB_SUCCESS)
      {
        return -1;
      } // if seq_setReg() failed
      if(seq_setReg(OUTLUT_ENGINE1, 1, 0x7C, val0 | (val1 << 16)) != SEQ_LIB_SUCCESS)
      {
        return -1;
      } // if seq_setReg() failed
    }   // while lutcnt < OUTLUT_SIZE
  }
  else if(aLut == 2)
  {
#define LOG_A_12TO8 15453.32
#define LOG_B_12TO8 0.016704
    printf("output lut: log 12to8\n");
    while(lutcnt < OUTLUT_SIZE)
    {
      if(!lutcnt)
      {
        val0 = 0;
      }
      else
      {
        val0 = (uint32_t)(256.0 * LOG_A_12TO8 * logf((lutcnt)*LOG_B_12TO8 + 1) / ((float)(lutcnt << 4)));
      }
      lutcnt++;
      val1 = (uint32_t)(256.0 * LOG_A_12TO8 * logf((lutcnt)*LOG_B_12TO8 + 1) / ((float)(lutcnt << 4)));
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
      if(seq_setReg(OUTLUT_ENGINE0, 1, 0x7C, val0 | (val1 << 16)) != SEQ_LIB_SUCCESS)
      {
        return -1;
      } // if seq_setReg() failed
      if(seq_setReg(OUTLUT_ENGINE1, 1, 0x7C, val0 | (val1 << 16)) != SEQ_LIB_SUCCESS)
      {
        return -1;
      } // if seq_setReg() failed
    }   // while lutcnt < OUTLUT_SIZE
  }
  else if(aLut == 3)
  {
#define LOG_A_16TO8 8332.00
#define LOG_B_16TO8 0.634928
    uint16_t lutcnt = 0;
    printf("output lut: log 16to8\n");
    while(lutcnt < OUTLUT_SIZE)
    {
      if(!lutcnt)
        val0 = 0x0;
      else
      {
        val0 = (uint32_t)(256.0 * LOG_A_16TO8 * logf((lutcnt)*LOG_B_16TO8 + 1) / ((float)(lutcnt << 4)));
      }
      lutcnt++;
      val1 = (uint32_t)(256.0 * LOG_A_16TO8 * logf((lutcnt)*LOG_B_16TO8 + 1) / ((float)(lutcnt << 4)));
      lutcnt++;
      //printf("inlut4k[%d]=%d\n"
      //       "inlut4k[%d]=%d\n",
      //       lutcnt-2, val0,
      //       lutcnt-1, val1
      //       );
      // LUTD (IPUS)
      if(val0 > 0xffff)
      {
        val0 = 0xffff;
      }
      if(val1 > 0xffff)
      {
        val1 = 0xffff;
      }
      if(seq_setReg(OUTLUT_ENGINE0, 1, 0x7C, val0 | (val1 << 16)) != SEQ_LIB_SUCCESS)
      {
        return -1;
      } // if seq_setReg() failed
      if(seq_setReg(OUTLUT_ENGINE1, 1, 0x7C, val0 | (val1 << 16)) != SEQ_LIB_SUCCESS)
      {
        return -1;
      } // if seq_setReg() failed
    }   // while lutcnt < OUTLUT_SIZE
  }
  return 0;
}

//***************************************************************************

static int32_t LogStatus(AppContext& arContext)
{
  unsigned long lTimeEnd = 0, lTimeDiff = 0;

  GETTIME(&lTimeEnd);
  lTimeDiff                   = lTimeEnd - arContext.mTimes;
  arContext.mTimes            = lTimeEnd;
  uint32_t lFrameDoneDiff     = arContext.mFrameDoneCnt - arContext.mFrameDoneCntLast;
  arContext.mFrameDoneCntLast = arContext.mFrameDoneCnt;

  if(arContext.mKbLog != 0)
  {
    if(arContext.mKbLog == 1)
    {
      printf("Frame: %7d (Isp.GetFrame:%d)\n", arContext.mFrameDoneCnt, arContext.mFrmCnt);

      printf("%d frames took %lu usec (%5.2ffps)\n", lFrameDoneDiff, lTimeDiff,
             lFrameDoneDiff * 1000000.0 / ((float)lTimeDiff));
    }
    if(arContext.mKbLog == 2)
    {
      printf("Frame: %7d (Isp.GetFrame:%d)\n", arContext.mFrameDoneCnt, arContext.mFrmCnt);

      ISP_DumpPerfCounters(IPU_PROF_FRAMES);
      if(SEQ_IpuPerformanceInfoRequest(IPU_PROF_FRAMES + 1))
      {
        return -1;
      }
    }
    if(arContext.mKbLog == 5)
    {
      SEQ_Head_Ptr_t* lpFetchedGraph = NULL;
      SEQ_Buf_t**     lppFetchedBufferList;

      lppFetchedBufferList = arContext.mpGrabber->GraphFetch(&lpFetchedGraph);
      ISP_PrintGraphStat(lppFetchedBufferList);
    } // (if arContext.mKbLog ==5)
  }   // (if arContext.mKbLog !=0)
  return 0;
} // LogStatus()

//***************************************************************************

static void SeqEventHandler(uint32_t aEventType, void* apUserVal)
{
  AppContext* lpContext = (AppContext*)apUserVal;
  if(apUserVal == NULL)
  {
    printf("SeqEvent callbec parameter is NULL.\n");
    return;
  } // if param is NULL

  switch(aEventType)
  {
    case SEQ_MSG_TYPE_FRAMEDONE:
    {
      lpContext->mFrameDoneCnt++;
    } // case SEQ_MSG_TYPE_FRAMEDONE:
    break;
    case SEQ_MSG_TYPE_OTHERERROR:   // fallthrough
    case SEQ_MSG_TYPE_FDMAERROR:    // fallthrough
    case SEQ_MSG_TYPE_FDMACRCERROR: // fallthrough
    {
      char lString[SEQ_PRAM_AUX_DATA_SIZE] = {0};

      lpContext->mError = true;
      sStop             = true;
      if(aEventType != SEQ_MSG_TYPE_OTHERERROR)
      {
        VDB_LOG_WARNING("Unknown event type (%u) received.\n", aEventType);
      }
      VDB_LOG_NOTE("*** Sequencer error text:\n");
      if(SEQ_MsgAuxDataGet(lString, SEQ_PRAM_AUX_DATA_SIZE) != SEQ_LIB_SUCCESS)
      {
        VDB_LOG_NOTE("Failed to get the auxiliary data from PRAM.\n");
      } // if failed to read the error message
      printf("  %s\n", lString);
    } // default
    break;
    default:
      break;
  } // switch(aEventType)

} // SeqEventHandler(uint32_t aEventType)

#ifndef __STANDALONE__

//***************************************************************************

static int KeyDown()
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

static int GetCharNonBlock()
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

static void TermNonBlockSet()
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

static void TermBlockSet()
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

static CAM_LIB_RESULT SetI2cRepeat(uint32_t aI2cRepeat)
{
  if(CAM_LIB_SUCCESS == CAM_Open())
  {
    uint32_t lI2CNumLoop = 111;

    if(CAM_LIB_FAILURE == CAM_I2C_SetRepeat(aI2cRepeat))
    {
      VDB_LOG_ERROR("CAM SET I2C loop failed\n");
      return CAM_LIB_FAILURE;
    }

    if(CAM_LIB_FAILURE == CAM_I2C_GetRepeat(lI2CNumLoop))
    {
      VDB_LOG_ERROR("CAM READ I2C loop failed\n");
      return CAM_LIB_FAILURE;
    }

    if(aI2cRepeat != lI2CNumLoop)
    {
      VDB_LOG_ERROR("CAM SET I2C loop failed\n");
      return CAM_LIB_FAILURE;
    }
  }
  else
  {
    VDB_LOG_ERROR("CAM OPEN FAILED\n");
    return CAM_LIB_FAILURE;
  }
  return CAM_LIB_SUCCESS;
}

//***************************************************************************

void SigintHandler(int)
{
  sStop = true;
} // SigintHandler()

//***************************************************************************

static int32_t SigintSetup()
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

static char GetChar()
{
#ifdef __STANDALONE__
  return sci_0_testchar();
#else  // #ifdef __STANDALONE__
  int lChar = GetCharNonBlock();
  return (char)((lChar < 0) ? 0 : lChar & 0xff);
#endif // else from #ifdef __STANDALONE__
} // GetChar()

//***************************************************************************

static void KbAction(AppContext& arContext)
{
  uint8_t lKey = arContext.mKbKey;
  if(lKey == 'h')
  { // help
    printf(
        "keyboard help:\n"
        "h .........help\n"
        "D .........toggle display on off\n"
        "p .........change logging print mode\n"
        "i .........enter IIC command mode\n"
        "  aaaavv ..enter IIC command to 0xaaaa with value 0xvv\n"
        "  ret .....write to IIC\n"
        "  r .......read from IIC\n"
        "< .........toggle bit shift\n"
        "k .........select exposure mode (0/1/2/3)\n"
        "  0 .......12bit combined\n"
        "  1 .......very short\n"
        "  2 .......short\n"
        "  3 .......long\n"
        "x .........enter display xoffset mode (+/-)\n"
        "E .........toggle endian swap\n"
        "O .........toggle output lut\n"
        "g .........toggle channel gain\n"

    );
    return;
  } // if help requested

  if(lKey == 'k')
  {
    printf("Mode = exposure mode selection (0/1/2/3)\n");
    arContext.mKbMode = 'k';
    return;
  } // if k pressed

  if(lKey == 'x')
  {
    arContext.mKbMode = 'x';
    printf("Mode = X-Offset for Display set\n");
    arContext.mKbI2c = 0;
    return;
  } // if x pressed

  if(lKey == 'p')
  {
    static char lPrintmode[6][12] = {"off", "fps", "ipu prof", "exposure", "awb", "buffer size"};

    arContext.mKbLog++;
    if(arContext.mKbLog == 6)
    {
      arContext.mKbLog = 0;
    }

    printf("Statistics Print = %s\n", lPrintmode[arContext.mKbLog]);
    arContext.mKbI2c  = 0;
    arContext.mKbMode = 'p';
    return;
  } // if p pressed

  if(lKey == 'D')
  {
    arContext.mKbMode = 'D';
    arContext.mKbDisplay ^= 1;
    printf("Display update %s\n", (arContext.mKbDisplay) ? "on" : "off");
    if(!arContext.mKbDisplay)
    {
      printf("Frame Buffer %d\n", arContext.mFrmCnt);
    }
    arContext.mKbI2c = 0;
    return;
  } // if d pressed

  if(lKey == 'i')
  {
    arContext.mKbMode = 'i';
    printf("Mode = IIC command\n");
    arContext.mKbI2c = 0;
    return;
  } // if i pressed

  if(lKey == 'g')
  {
    if(arContext.mKbChgain < 6)
    {
      arContext.mKbChgain++;
    } // if chgain < 6
    else
    {
      arContext.mKbChgain = 0;
    } // else from if chgain < 6

    printf("chgain %d,%d,%d,%d\n", sppChGainVal[arContext.mKbChgain][0], sppChGainVal[arContext.mKbChgain][1],
           sppChGainVal[arContext.mKbChgain][2], sppChGainVal[arContext.mKbChgain][3]);

    OV10640_Exposure_t lExp;
    lExp.mCsiIdx = CSI_IDX_CAM;

    for(uint32_t i = 0; i < 4; i++)
    {
      lExp.mpLongChnlGain[i]      = sppChGainVal[arContext.mKbChgain][i];
      lExp.mpShortChnlGain[i]     = sppChGainVal[arContext.mKbChgain][i];
      lExp.mpVeryShortChnlGain[i] = sppChGainVal[arContext.mKbChgain][i];
    } // for all color compononents

    if(MAXIM_OV10640_ExposureSet(lExp) != CAM_LIB_SUCCESS)
    {
      printf("Failed to set camera exposure config.\n");
      return;
    } // if SONY_ExposureSet failed

    arContext.mKbI2c = 0;
    return;
  } // if g pressed

  if(lKey == '<')
  {
    arContext.mKbShift++;
    if(arContext.mKbShift > 8)
    {
      arContext.mKbShift = 0;
    } // if bit shift > 4

    printf("shift input by %d bits\n", arContext.mKbShift);
    seq_setReg(TEE_ENGINE, 0, 0x7f, arContext.mKbShift); // GPR15 (IPUS)
    arContext.mKbI2c = 0;
    return;
  } // if < pressed

  if(lKey == 'E')
  {
    arContext.mKbMode = 'E';
    arContext.mKbEndian ^= 1;
    printf("Endianess swap %s\n", (arContext.mKbEndian) ? "on" : "off");
    seq_setReg(ENDIAN_ENGINE, 0, 0x7e, arContext.mKbEndian); // GPR14 (IPUS)
    arContext.mKbI2c = 0;
    return;
  } // if d pressed

  if(lKey == 'O')
  {
    arContext.mKbOutLut++;
    if(arContext.mKbOutLut > 2)
    {
      arContext.mKbOutLut = 0;
    }

    //printf("Output Lut %d\n",arContext.mKbOutLut);
    SetOutLut(arContext.mKbOutLut);
    arContext.mKbI2c = 0;
    return;
  } // if < pressed

  if(arContext.mKbMode == 'p')
  {
    if(lKey == '0')
    { // show combined

      arContext.mKbLog = 0;
      printf("Statistics Print = off\n");
    }
  }

  if(arContext.mKbMode == 'k')
  {
    if(lKey == '0')
    { // show combined
      MAXIM_CAM_RegWrite(CSI_IDX_CAM, 0x3119, 0x04);
      printf("0 (12 bit combined)\n");
      return;
    } // if key 0
    if(lKey == '1')
    {
      MAXIM_CAM_RegWrite(CSI_IDX_CAM, 0x3119, 0x47);
      printf("1 (very short exposure)\n");
      return;
    } // if key 1
    if(lKey == '2')
    {
      MAXIM_CAM_RegWrite(CSI_IDX_CAM, 0x3119, 0x46);
      printf("2 (short exposure)\n");
      return;
    } // if key 2
    if(lKey == '3')
    {
      MAXIM_CAM_RegWrite(CSI_IDX_CAM, 0x3119, 0x45);
      printf("3 (long exposure)\n");
      return;
    } // // if key 3

    arContext.mKbI2c = 0;
  } // if mode k

  if(arContext.mKbMode == 'x')
  {
    if(lKey == '+')
    { // show combined
      if(arContext.mKbXoffset <= (UYVY_INPIX - DISPLAY_PIXELS - 32))
      {
        arContext.mKbXoffset += 32;
      } // if offset <= in range
    }   // if key +
    else if(lKey == '-')
    {
      if(arContext.mKbXoffset >= 32)
      {
        arContext.mKbXoffset -= 32;
      } // if offset >= 32
    }   // if key -
    else
    {
      printf("key %d %c\n", lKey, lKey);
      return;
    } // if other key

    printf("Display X-Offset =%d\n", arContext.mKbXoffset);

    arContext.mKbI2c = 0;

    return;
  } // if mode x

  if(arContext.mKbMode == 'i')
  {
    if((lKey >= '0') && (lKey <= '9'))
    { // enter IIC command
      arContext.mKbI2c <<= 4;
      arContext.mKbI2c += (lKey - '0') & 0xf;
      arContext.mKbI2c &= 0xffffff;
      printf("IIC command 0x%x - 0x%x\n", (arContext.mKbI2c >> 8), (arContext.mKbI2c & 0xff));
      return;
    } // if key <0;9>

    if((lKey >= 'a') && (lKey <= 'f'))
    { // enter IIC command
      arContext.mKbI2c <<= 4;
      arContext.mKbI2c += (lKey - 'a' + 10) & 0xf;
      arContext.mKbI2c &= 0xffffff;
      printf("IIC command 0x%x - 0x%x\n", (arContext.mKbI2c >> 8), (arContext.mKbI2c & 0xff));
      return;
    } // if key <a;f>
    if(strrchr("\n\rw", lKey))
    {
      printf("Writing to IIC\n");
      MAXIM_CAM_RegWrite(CSI_IDX_CAM, arContext.mKbI2c >> 8, arContext.mKbI2c & 0xff);
      return;
    } // if key w
    if(lKey == 'r')
    {
      printf("Read from IIC\n");

      uint8_t val;
      MAXIM_CAM_RegRead(CSI_IDX_CAM, (arContext.mKbI2c >> 8) & 0xffff, val);
      printf("%x = 0x%x / %d\n", arContext.mKbI2c >> 8, val, val);

      return;
    } // if key r
  }   // if mode i
} // KbAction()

//***************************************************************************

/* EOF */
