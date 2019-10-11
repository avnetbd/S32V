/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
* Copyright (c) 2016-17
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
#endif // #ifdef __STANDALONE__
#include <string.h>

#ifdef __STANDALONE__
#include "frame_output_dcu.h"
#define CHNL_CNT io::IO_DATA_CH3
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"
#define CHNL_CNT io::IO_DATA_CH3
#endif // else from #ifdef __STANDALONE__

#include "isp_cam_sony.h"
#include "sdi.hpp"
#include "h264enc_c.h"

#include "vdb_log.h"

#include "iofiles.h"        // *** from graph ***
#include "../include/lut.h" // gamma table
#include <common_helpers.h>
#include <string>

//***************************************************************************
// constants
//***************************************************************************

// Possible to set input resolution (must be supported by the DCU)
#define WIDTH 1280       ///< width of DDR buffer in pixels
#define HEIGHT 720       ///< height of DDR buffer in pixels
#define DDR_BUFFER_CNT 3 ///< number of DDR buffers per ISP stream

// frame grabbing parameters
#define FRMS_TO_CATCH_DF 200 ///< number of frames to be encoded by default
#define FRM_TIMEOUT 1000000  ///< frame timeout in us
#define CHUNK_SIZE 0x10000   ///< size of output chunk in bytes
#define CHUNK_TIMEOUT 1000   ///< time to wait for output chunk in ms

// ISP preprocessing parameters
#define EXPOSURE_ENGINE 1, 1
#define TONEHDR_ENGINE 1, 1
#define GAMMA_ENGINE 1, 3
#define GAMMA_ON 1
#define KNEE_VAL 256
#define GAMMA_LUT_OFFSET 0

//***************************************************************************
// macros
//***************************************************************************

#ifdef __STANDALONE__
extern "C" {
unsigned long get_uptime_microS(void);
}

#define GETTIME(time) (*(time) = get_uptime_microS())
#else // ifdef __STANDALONE__
#define GETTIME(time)                                                                                                  \
  {                                                                                                                    \
    struct timeval lTime;                                                                                              \
    gettimeofday(&lTime, 0);                                                                                           \
    *time = (lTime.tv_sec * 1000000 + lTime.tv_usec);                                                                  \
  }
#endif // else from #ifdef __STANDALONE__

//***************************************************************************
// types
//***************************************************************************
struct AppContext
{
  uint32_t       mFramesToCatch; ///< number of frames to be catched
  sdi_grabber*   mpGrabber;      ///< pointer to grabber instance
  sdi_H264EncIO* mpEnc;          ///< encoder object pointer
  uint64_t*      mpOutChunk;     ///< output chunk storrage in DDR
  FILE*          mpEncFile;      ///< h264 stream output file

  bool     mError;     ///< to signal ISP problems
  bool     mFrameDone; ///< to signal frame was finished by ISP
  uint32_t mFrmCnt;    ///< number of frames captured so far

  unsigned long mTimeFrmStart; ///< frame interval start
};                             // struct AppContext

//***************************************************************************
// local functions
//***************************************************************************

#ifdef __STANDALONE__
//extern SEQ_Buf_t  producerless_buffer_1;
extern "C" {
unsigned long get_uptime_microS(void);
}

#define GETTIME(time) (*(time) = get_uptime_microS())
#else // ifdef __STANDALONE__
#define GETTIME(time)                                                                                                  \
  {                                                                                                                    \
    struct timeval lTime;                                                                                              \
    gettimeofday(&lTime, 0);                                                                                           \
    *time = (lTime.tv_sec * 1000000 + lTime.tv_usec);                                                                  \
  }
#endif // else from #ifdef __STANDALONE__

/************************************************************************/
/** User defined call-back function for Sequencer event handling.
  * 
  * \param  aEventType defines Sequencer event type
  * \param  apUserVal  pointer to any user defined object 
  ************************************************************************/
static void SeqEventCallBack(uint32_t aEventType, void* apUserVal);

/************************************************************************/
/** Configure camera and sensor data preprocessing.
  * 
  * \return 0 if all ok, <0 otherwise
  ************************************************************************/
static int32_t CamConfig(void);

/************************************************************************/
/** Update exposure setup.
  * 
  * \return 0 if all ok, <0 otherwise
  ************************************************************************/
static int32_t ExpControl(void);

/************************************************************************/
/** Prepare everything before executing the main functionality .
  * 
  * \param arContext structure capturing the context of the application
  * 
  * \return 0 if all ok, <0 otherwise 
  ************************************************************************/
static int32_t Prepare(AppContext& arContext);

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
static void SigintHandler(int);

/************************************************************************/
/** SIGINT handler.
  * 
  * \param  aSigNo 
  * 
  * \return SEQ_LIB_SUCCESS if all ok
  *         SEQ_LIB_FAILURE if failed
  ************************************************************************/
static int32_t SigintSetup(void);

static bool sStop = false; ///< to signal Ctrl+c from command line

#endif // #ifndef __STANDALONE__

//***************************************************************************

int main(int argc, char** argv)
{
  int lRet = 0;

  AppContext lContext = {FRMS_TO_CATCH_DF, NULL, NULL, NULL, NULL, false, false, 0, 0};

  //*** process command line parameters ***
  std::string helpMsg = std::string(
                            "\n**************************************************************\n"
                            "** H264 encoder demo\n"
                            "** Description:\n"
                            "**  o Sony imx224 camera (on MipiCsi_0) expected as image input.\n"
                            "**  o ISP generates histogram for AEC, converts pixel format from\n"
                            "**    RGB to YUV420 and trigers H264 encoding.\n"
                            "**  o H264Encoded stream data are located in SRAM.\n"
                            "**\n"
                            "** Usage:\n"
                            "**  o 1 parameter: number of frames to be captured and encoded.\n"
                            "**  o no params => ") +
                        std::to_string(FRMS_TO_CATCH_DF) +
                        " frames will be captured and encoded.\n"
                        "**\n"
                        "**************************************************************\n"
                        "**\n";

  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp < 0)
  { // print help message even if no help option is provided by the user
    printf("%s", helpMsg.c_str());
  }

  if(argc > 1)
  {
    lContext.mFramesToCatch = atoi(argv[1]);
  } // if there is a parameter given
  printf("**  %u frames will be encoded.\n", lContext.mFramesToCatch);
  printf("**\n");
  printf("**************************************************************\n\n");

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
  // setup Ctrl+C handler
  if(SigintSetup() != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to register Ctrl+C signal handler.");
    return -1;
  }

  printf("Press Ctrl+C to terminate the demo.\n");

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
  sdi_FdmaIO* lpFdma = (sdi_FdmaIO*)arContext.mpGrabber->IoGet(SEQ_OTHRIX_FDMA);
  if(lpFdma == NULL)
  {
    printf("Failed to get FDMA object.\n");
    return -1;
  } // if no FDMA object

  arContext.mpEnc = (sdi_H264EncIO*)arContext.mpGrabber->IoGet(SEQ_OTHRIX_H264ENC);
  if(arContext.mpEnc == NULL)
  {
    printf("Failed to get H264Encoder object.\n");
    return -1;
  } // if no Enc object

  // modify DDR frame geometry to fit display output
  SDI_ImageDescriptor lFrmDesc = SDI_ImageDescriptor(WIDTH, HEIGHT, RGB888);
  if(lpFdma->DdrBufferDescSet(0, lFrmDesc) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor.\n");
    return -1;
  } // if frame descriptor setup failed

  // allocate DDR buffers
  if(lpFdma->DdrBuffersAlloc(0, DDR_BUFFER_CNT) != LIB_SUCCESS)
  {
    printf("Failed to allocate DDR buffers.\n");
    return -1;
  } // if ddr buffers not allocated

  // configure encoding parameters
  H264ENC_Config_t lEncConfig = arContext.mpEnc->ConfigGet();
  lEncConfig.mFrameWidth      = 1280;
  lEncConfig.mFrameHeight     = 720;
  if(arContext.mpEnc->ConfigSet(lEncConfig) != LIB_SUCCESS)
  {
    printf("Failed to set encoding parameters.\n");
    return -1;
  } // if enc param setup failed

  // set chunk size and timeout
  if(arContext.mpEnc->OutChunkSet(CHUNK_SIZE, CHUNK_TIMEOUT) != LIB_SUCCESS)
  {
    printf("Failed to set chunk size.\n");
    return -1;
  } // if chunk size setup failed

  // *** prestart grabber ***
  if(arContext.mpGrabber->PreStart() != LIB_SUCCESS)
  {
    printf("Failed to prestart the grabber.\n");
    return -1;
  } // if PreStart() failed

  // *** configure camera ***
  if(CamConfig() != 0)
  {
    printf("Failed to prestart the grabber.\n");
    return -1;
  } // if CamConfig() failed

  // *** setup DDR stream storage ***
  // allocate temporary ddr storage

  arContext.mpOutChunk = (uint64_t*)malloc(CHUNK_SIZE);
  if(arContext.mpOutChunk == NULL)
  {
    printf("Failed to allocate temporary chunk storage in DDR.\n");
    return -1;
  } // if chunk storrage alloc failed

  // open a file to store the data into
  arContext.mpEncFile = fopen("encoded_stream.h264", "wb");
  if(arContext.mpEncFile == NULL)
  {
    printf("Failed to open stream output file.\n");
    return -1;
  } // if file not opened

  // *** install sequencer event callback ***
  if(arContext.mpGrabber->SeqEventCallBackInstall(&SeqEventCallBack, &arContext) != LIB_SUCCESS)
  {
    printf("Failed to install Sequencer event callback.\n");
    return -1;
  } // if callback setup failed

  return 0;
} // Prepare()

//***************************************************************************

static int32_t Run(AppContext& arContext)
{
  uint32_t lChunkNum = 0;

  //*** Init DCU Output ***
  io::FrameOutputV234Fb lDcuOutput(WIDTH, HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT);

  // *** start grabbing ***
  GETTIME(&arContext.mTimeFrmStart);
  if(arContext.mpGrabber->Start(FRMS_TO_CATCH_DF) != LIB_SUCCESS)
  {
    printf("Failed to start the grabber.\n");
    return -1;
  } // if Start() failed

  SDI_Frame     lFrame;
  unsigned long lTimeStart = arContext.mTimeFrmStart;
  unsigned long lTimeEnd = 0, lTimeDiff = 0;

  for(;;)
  {
    // *** display data if frame done ***
    if(arContext.mFrameDone)
    {
      lFrame               = arContext.mpGrabber->FramePop();
      arContext.mFrameDone = false;
      if(lFrame.mUMat.empty() && arContext.mpGrabber->IsLive())
      {
        printf("Failed to pop the frame.\n");
        break;
      } // if FramePop() failed

      lDcuOutput.PutFrame(lFrame.mUMat);

      if(arContext.mpGrabber->FramePush(lFrame) != LIB_SUCCESS)
      {
        printf("Failed to push back the frame.\n");
        break;
      } // if push failed

      // *** do exposure control ***
      ExpControl();
    } // if new frame read

    // *** stop if requested ***
    if(sStop || arContext.mError)
    {
      break; // break if Ctrl+C pressed
    }        // if Ctrl+C

    /// *** save available stream data ***
    if(arContext.mpEnc->DataPop(arContext.mpOutChunk) != LIB_SUCCESS)
    {
      if(arContext.mpGrabber->IsLive() == true)
      {
        printf("Failed to pop H264 stream data.\n");
      }
      break;
    } // if DataPop() failed

    fwrite(arContext.mpOutChunk, sizeof(uint8_t), CHUNK_SIZE, arContext.mpEncFile);

    fflush(arContext.mpEncFile);
    lChunkNum++;
  } // for ever

  GETTIME(&lTimeEnd);
  lTimeDiff = lTimeEnd - lTimeStart;

  //*** Stop ISP processing ***
  if(arContext.mpGrabber->Stop() != LIB_SUCCESS)
  {
    printf("Failed to stop grabber.\n");
    return -1;
  } // if Stop() failed

  // print statistics
  printf("%u frames took %lu usec (%5.2ffps) bandwidth: %5.2f MB/s\n", arContext.mFrmCnt, lTimeDiff,
         (arContext.mFrmCnt * 1000000.0) / ((float)lTimeDiff), (lChunkNum * CHUNK_SIZE) / ((float)lTimeDiff));

  sleep(1);

  return 0;
} // Run()

//***************************************************************************

static int32_t Cleanup(AppContext& arContext)
{
  int32_t lRet = 0;

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

  if(arContext.mpOutChunk != NULL)
  {
    free(arContext.mpOutChunk);
    arContext.mpOutChunk = NULL;
  } // if chunk storrage allocated

  if(arContext.mpEncFile != NULL)
  {
    fclose(arContext.mpEncFile);
    arContext.mpEncFile = NULL;
  } // if h264 stream output file opened

  arContext.mpEnc = NULL;

#ifdef __STANDALONE__
  for(;;)
    ;  // *** don't return ***
#endif // #ifdef __STANDALONE__

  if(sdi::Close(0) != LIB_SUCCESS)
  {
    printf("Failed to terminate use of SDI.\n");
    lRet = -1;
  } // if SDI use termination failed

  return lRet;
} // Cleanup()

//***************************************************************************

static int32_t CamConfig()
{
  // *** exposure kernel ***
  // black level correction
  if(seq_setReg(EXPOSURE_ENGINE, 0, 0x70, 0xf08) != SEQ_LIB_SUCCESS) // GPR0
  {
    return -1;
  }                                                                  // if seq_setReg() failed
  if(seq_setReg(EXPOSURE_ENGINE, 0, 0x71, 0xf08) != SEQ_LIB_SUCCESS) // GPR1
  {
    return -1;
  }                                                                  // if seq_setReg() failed
  if(seq_setReg(EXPOSURE_ENGINE, 0, 0x72, 0xf08) != SEQ_LIB_SUCCESS) // GPR2
  {
    return -1;
  }                                                                  // if seq_setReg() failed
  if(seq_setReg(EXPOSURE_ENGINE, 0, 0x73, 0xf08) != SEQ_LIB_SUCCESS) // GPR3
  {
    return -1;
  } // if seq_setReg() failed

  // channel gain
  if(seq_setReg(EXPOSURE_ENGINE, 0, 0x74, 450) != SEQ_LIB_SUCCESS) // GPR4: R
  {
    return -1;
  }                                                                // if seq_setReg() failed
  if(seq_setReg(EXPOSURE_ENGINE, 0, 0x75, 256) != SEQ_LIB_SUCCESS) // GPR5: GR
  {
    return -1;
  }                                                                // if seq_setReg() failed
  if(seq_setReg(EXPOSURE_ENGINE, 0, 0x76, 256) != SEQ_LIB_SUCCESS) // GPR6: GB
  {
    return -1;
  }                                                                // if seq_setReg() failed
  if(seq_setReg(EXPOSURE_ENGINE, 0, 0x77, 400) != SEQ_LIB_SUCCESS) // GPR7: B
  {
    return -1;
  } // if seq_setReg() failed

  // *** tonemap kernel ***
  if(seq_setReg(TONEHDR_ENGINE, 0, 0x78, KNEE_VAL) != SEQ_LIB_SUCCESS) // GPR8
  {
    return -1;
  } // if seq_setReg() failed

  // *** gamma kernel ***
  if(seq_setReg(GAMMA_ENGINE, 0, 0x71, GAMMA_LUT_OFFSET) != SEQ_LIB_SUCCESS) // GPR1
  {
    return -1;
  }                                                                                   // if seq_setReg() failed
  if(seq_setReg(GAMMA_ENGINE, 1, 0x78, ((GAMMA_LUT_OFFSET) >> 1)) != SEQ_LIB_SUCCESS) // LUTA
  {
    return -1;
  } // if seq_setReg() failed

  {
    uint16_t lutcnt = 0;
    while(lutcnt < 2048)
    {
      uint32_t val;
      val = lut[lutcnt] | ((long)lut[lutcnt] << 16);
      lutcnt++;
      if(seq_setReg(GAMMA_ENGINE, 1, 0x7C, val) != SEQ_LIB_SUCCESS) // LUTD
      {
        return -1;
      }                                                          // if seq_setReg() failed
    }                                                            // while not end of LUT
  }                                                              // LUT setup block
  if(seq_setReg(GAMMA_ENGINE, 0, 0x70, GAMMA_ON) != LIB_SUCCESS) // GPR0
  {
    return -1;
  } // if seq_setReg() failed

  // Camera Configuration
  // modify calmera geometry !!! setup before setting up expsoure control
  SONY_Geometry_t lGeo;
  lGeo.mCsiIdx = CSI_IDX_0;
  if(SONY_GeometryGet(lGeo) != CAM_LIB_SUCCESS)
  {
    printf("Failed to get camera geometry.\n");
    return -1;
  } // if SONY_GeometryGet failed
  lGeo.mWinMode = SONY_CROP;
  //lGeo.mHeight = 729;
  lGeo.mWidth   = 1288;
  lGeo.mHmax    = 0;
  lGeo.mOb      = 2;
  lGeo.mXoff    = 0;
  lGeo.mYoff    = 0;
  lGeo.mVerFlip = 1;
  lGeo.mHorFlip = 1;
  lGeo.mFps     = 20;

  if(SONY_GeometrySet(lGeo) != CAM_LIB_SUCCESS)
  {
    printf("Failed to set camera geometry.\n");
    return -1;
  } // if SONY_GeometrySet failed

  // modify camera exposure
  SONY_Exposure_t lExp;
  lExp.mCsiIdx = CSI_IDX_0;
  if(SONY_ExposureGet(lExp) != CAM_LIB_SUCCESS)
  {
    printf("Failed to get camera exposure config.\n");
    return -1;
  } // if SONY_ExposureGet failed
  lExp.mConversionGain = 1;
  lExp.mExposureLines  = 500;
  lExp.mAnalogGain     = 0;
  lExp.mConfigFlags    = SONY_EXP_EL | SONY_EXP_CG | SONY_EXP_AG;
  if(SONY_ExposureSet(lExp) != CAM_LIB_SUCCESS)
  {
    printf("Failed to set camera exposure config.\n");
    return -1;
  } // if SONY_ExposureSet failed

  printf("VMAX = %4d, Lines = %3d, OB = %2d\n", lGeo.mVmax, lGeo.mHeight, lGeo.mOb);

  // *** enable IPU histogram readout ***
  if(SEQ_HistogramEnable(SEQ_IPUS1) != SEQ_LIB_SUCCESS)
  {
    printf("Failed to enable histogram readout.\n");
    return -1;
  } // if histogram not enabled

  return 0;
} // CamConfig();

//***************************************************************************

static int32_t ExpControl()
{
  SONY_ExpState_t lExpState = SONY_EXP_SENT;
  if(SONY_ExposureStatusGet(CSI_IDX_0, lExpState) != CAM_LIB_SUCCESS)
  {
    printf("Failed to get camera exposure status.\n");
    return -1;
  } // if failed to get exp status

  if(lExpState == SONY_EXP_READY)
  {
    static SEQ_IpuHist_t lIpuHist;
    uint32_t             lExp = 0;
    memset(&lIpuHist, 0, sizeof(SEQ_IpuHist_t));
    lIpuHist.mIpuIdx = SEQ_IPUS1;

    // do expsoure control
    if(SEQ_HistogramGet(&lIpuHist) != SEQ_LIB_SUCCESS)
    {
      printf("Failed to get histogram data.\n");
      return -1;
    } // if failed to get exp status
    SONY_ExposureControlEqual(&lIpuHist, 0.0, &lExp);

    if(SONY_ExposureSet(CSI_IDX_0, lExp) != CAM_LIB_SUCCESS)
    {
      printf("Failed to update camera exposure.\n");
      return -1;
    } // if failed to get exp status
  }   // if ready for exp update

  return 0;
} // ExpControl

//****************************************************************************

static void SeqEventCallBack(uint32_t aEventType, void* apParam)
{
  unsigned long lTimeNow;
  AppContext*   lpContext = (AppContext*)apParam;
  if(apParam == NULL)
  {
    printf("SeqEvent callbec parameter is NULL.\n");
    return;
  } // if param is NULL

  GETTIME(&lTimeNow);
  if((lTimeNow - lpContext->mTimeFrmStart) > FRM_TIMEOUT)
  {
    lpContext->mError = true;
  } // if ISP timed out
  else
  {
    switch(aEventType)
    {
      case SEQ_MSG_TYPE_FRAMEDONE:
      {
        lpContext->mTimeFrmStart = lTimeNow;
        lpContext->mFrameDone    = true;

        if(((++lpContext->mFrmCnt) % 30) == 0)
        {
          printf("%u frames encoded.\n", lpContext->mFrmCnt);
        } // once per 10 frames

        if(lpContext->mFrmCnt >= lpContext->mFramesToCatch)
        {
          sStop = true;
        } // if requested number of frames received
      }   // case SEQ_MSG_TYPE_FRAMEDONE:
      break;
      case SEQ_MSG_TYPE_OTHERERROR:
      {
        lpContext->mError = true;
      } // case SEQ_MSG_TYPE_OTHERERROR:
      break;
      default:
      {
        char lString[SEQ_PRAM_AUX_DATA_SIZE] = {0};

        VDB_LOG_WARNING("Unknown event type (%u) received.\n", aEventType);
        VDB_LOG_NOTE("*** Sequencer error text:\n");
        if(SEQ_MsgAuxDataGet(lString, SEQ_PRAM_AUX_DATA_SIZE) != SEQ_LIB_SUCCESS)
        {
          VDB_LOG_NOTE("Failed to get the auxiliary data from PRAM.\n");
        } // if failed to read the error message
        else
        {
          VDB_LOG_NOTE("  %s\n", lString);
        } // else from if failed to read the error message
      }   // default
      break;
    } // switch(aEventType)
  }   // else form if ISP timed out
} // SeqEventCallBack()

  //***************************************************************************

#ifndef __STANDALONE__
static void SigintHandler(int)
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

//***************************************************************************
#endif // #ifndef __STANDALONE__
