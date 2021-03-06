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
#endif // #ifdef __STANDALONE__
#include <string.h>

#ifdef __STANDALONE__
#include "frame_output_dcu.h"
#define CHNL_CNT io::IO_DATA_CH2
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"
#define CHNL_CNT io::IO_DATA_CH2
#endif // else from #ifdef __STANDALONE__

#include "oal.h"
#include "vdb_log.h"
#include "sdi.hpp"

#include "ov10635_surround_c.h"

#include "vdb_log.h"
#include <common_helpers.h>

//***************************************************************************
// constants
//***************************************************************************

// Possible to set input resolution (must be supported by the DCU)
#define WIDTH 1280       ///< width of DDR buffer in pixels
#define HEIGHT 800       ///< height of DDR buffer in pixels
#define DDR_BUFFER_CNT 3 ///< number of DDR buffers per ISP stream

//***************************************************************************

#define STREAM_CYCLE_FRM_CNT 150 ///< number of frames until stream switche
#define STREAM_CNT 4             ///< total number of availabe camear streams

#define FRM_TIME_MSR 300 ///< number of frames to measure the time and fps

//***************************************************************************
// macros
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

//***************************************************************************
// types
//***************************************************************************
struct AppContext
{
  sdi_grabber* mpGrabber; ///< pointer to grabber instance
  sdi_FdmaIO*  mpFdma;    ///< pointer to fdma object

  // ** event counters and flags **
  bool     mError;      ///< to signal ISP problems
  uint32_t mFrmDoneCnt; ///< number of frames done events
  uint32_t mFrmCnt;     ///< number of frames grabbed by the app
};                      // struct AppContext

/************************************************************************/
/** User defined call-back function for Sequencer event handling.
  * 
  * \param  aEventType defines Sequencer event type
  * \param  apUserVal  pointer to any user defined object 
  ************************************************************************/
static void SeqEventCallBack(uint32_t aEventType, void* apUserVal);

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

//***************************************************************************

static bool sStop = false; ///< to signal Ctrl+c from command line

#endif // #ifndef __STANDALONE__

static uint8_t sCamChanel = 5;
static int8_t  sCsiPort   = -1;

int main(int argc, char** argv)
{
  int lRet = 0;

  AppContext lContext;

  //*** process command line parameters ***
  const char helpMsg_str[] =
      "\n**************************************************************\n"
      "** Omnivision Ov10635 quad demo using Maxim Ser/Des HW setup\n"
      "** Description:\n"
      "**  o Maxim 9286 deserializer board with 4xOmnivision Ov10635\n"
      "**    cameras each with 9271 serializer (on MipiCsi_0) expected as\n"
      "**    image input.\n"
      "**  o ISP converts YUV422 10bit piexel data provided by the sensor\n"
      "**    to YUV422 8bit pixels and stores single camera images into\n"
      "**    separate DDR buffers.\n"
      "**  o Resulting YUV 1280x800 image are displayed live using DCU.\n"
      "**\n"
      "*********************************\n\n"
      "** Usage:\n"
      "**  o ./isp_ov10635_quad.elf <camera channel> [<csi port>]\n"
      "**\n"
      "** Options:\n"
      "**  o camera channel   1-5. 5: switch each camera between 150 frames\n"
      "**  o                      [default: 5]\n"
      "**  o csi port         0|1 [default: use graph's setting]\n"
      "**\n"
      "*********************************\n\n"
      "** Example:\n"
      "**  o Run camera #2, MAXIM pluged in CSI #1.\n"
      "**    ./isp_ov10635_quad.elf 2 1\n"
      "**  o Run all camera, use graph's setting for csi port.\n"
      "**    ./isp_ov10635_quad.elf\n"
      "**\n"
      "**************************************************************\n\n";
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg_str);
  if(idxHelp < 0)
  { // print help message even if no help option is provided by the user
    printf("%s", helpMsg_str);
  }

  if(argc > 1)
  {
    sCamChanel = atoi(argv[1]);
  }

  if(argc > 2)
  {
    sCsiPort = atoi(argv[2]);
  }

  if((sCamChanel < 1) || (sCamChanel > 5) || (sCsiPort < -1) || (sCsiPort > 1))
  {
    lRet = -1;
  } // if check params OK
  else
  {
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
  }   // else from if check params OK

  return lRet;
} // main()

//***************************************************************************

static int32_t Prepare(AppContext& arContext)
{
  // init app context
  ContextInit(arContext);

  // enable LIBS
  if(LibsPrepare(arContext) != 0)
  {
    printf("Failed to prepare libraries.\n");
    return -1;
  } // if failed to configure decoder
    // enable OAL

#ifndef __STANDALONE__

#endif // #ifndef __STANDALONE__

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

  if(arContext.mpGrabber->SeqEventCallBackInstall(&SeqEventCallBack, &arContext) != LIB_SUCCESS)
  {
    printf("Failed to install Sequencer event callback.\n");
    return -1;
  } // if callback setup failed

  return 0;
} // Prepare()

//***************************************************************************

static void ContextInit(AppContext& arContext)
{
  arContext.mpGrabber   = NULL;
  arContext.mpFdma      = NULL;
  arContext.mError      = false;
  arContext.mFrmCnt     = 0;
  arContext.mFrmDoneCnt = 0;
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

  if(arContext.mpGrabber->IoGet(SEQ_OTHRIX_MIPICSI0) && sCsiPort > -1)
  {
    printf("helloworld\n");
    arContext.mpGrabber->CsiSwap(sCsiPort + SEQ_OTHRIX_MIPICSI0, SEQ_OTHRIX_MIPICSI0);
  } // if(!lCamIo.mCsiIdxs.empty() && sCsiPort > -1)

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
  // *** 4x YUV full frame buffer array ***
  // modify DDR frame geometry to fit display output
  SDI_ImageDescriptor lFrmDesc;
  lFrmDesc = SDI_ImageDescriptor(WIDTH, HEIGHT, YUV422Stream_UYVY);

  if(arContext.mpFdma->DdrBufferDescSet(FDMA_IX_FastDMA_Out, lFrmDesc) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor 0.\n");
    return -1;
  } // if frame descriptor setup failed

  if(arContext.mpFdma->DdrBufferDescSet(FDMA_IX_FastDMA_Out1, lFrmDesc) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor 1.\n");
    return -1;
  } // if frame descriptor setup failed

  if(arContext.mpFdma->DdrBufferDescSet(FDMA_IX_FastDMA_Out2, lFrmDesc) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor 2.\n");
    return -1;
  } // if frame descriptor setup failed

  if(arContext.mpFdma->DdrBufferDescSet(FDMA_IX_FastDMA_Out3, lFrmDesc) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor 3.\n");
    return -1;
  } // if frame descriptor setup failed

  // allocate DDR buffers
  if(arContext.mpFdma->DdrBuffersAlloc(DDR_BUFFER_CNT) != LIB_SUCCESS)
  {
    printf("Failed to allocate DDR buffers.\n");
    return -1;
  } // if ddr buffers not allocated

  return 0;
} // DdrBuffersPrepare(AppContext &arContext)

//***************************************************************************

static int32_t Run(AppContext& arContext)
{
//*** Init DCU Output ***
#ifdef __STANDALONE__
  io::FrameOutputDCU lDcuOutput(WIDTH, HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT, DCU_BPP_YCbCr422);
#else  // #ifdef __STANDALONE__
  // setup Ctrl+C handler
  if(SigintSetup() != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to register Ctrl+C signal handler.");
    return -1;
  }

  printf("Press Ctrl+C to terminate the demo.\n");

  io::FrameOutputV234Fb lDcuOutput(WIDTH, HEIGHT, io::IO_DATA_DEPTH_08, io::IO_DATA_CH3, DCU_BPP_YCbCr422);
#endif // else from #ifdef __STANDALONE__

  unsigned long lTimeStart = 0, lTimeEnd = 0, lTimeDiff = 0;

  // *** start grabbing ***
  GETTIME(&lTimeStart);
  if(arContext.mpGrabber->Start() != LIB_SUCCESS)
  {
    printf("Failed to start the grabber.\n");
    return -1;
  } // if Start() failed

  uint32_t  lActiveStreamIndex = 0;
  SDI_Frame lpFrame[STREAM_CNT];
  // *** grabbing/processing loop ***

  for(;;)
  {
    // pop all
    for(int i = 0; i < 4; i++)
    {
      lpFrame[i] = arContext.mpGrabber->FramePop(i);
      if(lpFrame[i].mUMat.empty())
      {
        printf("Failed to grab image number %u\n", arContext.mFrmCnt);
        arContext.mError = true;
        return -1;
      } // if pop failed
    }   // for all channels
    if(sCamChanel < 5)
    {
      lDcuOutput.PutFrame(lpFrame[sCamChanel - 1].mUMat);
    }
    else
    {
      if(((++arContext.mFrmCnt) % STREAM_CYCLE_FRM_CNT) == 0)
      {
        ++lActiveStreamIndex;
        lActiveStreamIndex = lActiveStreamIndex % STREAM_CNT;
        printf("Selected camera = %u ", lActiveStreamIndex);
      } // if stream to be switched

      lDcuOutput.PutFrame(lpFrame[lActiveStreamIndex].mUMat);
    }

    for(int i = 0; i < 4; i++)
    {
      if(arContext.mpGrabber->FramePush(lpFrame[i]) != LIB_SUCCESS)
      {
        printf("Failed to push image number %u\n", arContext.mFrmCnt);
        arContext.mError = true;
        break;
      } // if push failed
    }

    if((arContext.mFrmCnt % FRM_TIME_MSR) == 0)
    {
      GETTIME(&lTimeEnd);
      lTimeDiff  = lTimeEnd - lTimeStart;
      lTimeStart = lTimeEnd;

      printf("%u frames took %lu usec (%5.2ffps)\n", FRM_TIME_MSR, lTimeDiff,
             (FRM_TIME_MSR * 1000000.0) / ((float)lTimeDiff));
    } // if time should be measured

#ifndef __STANDALONE__
    if(sStop)
    {
      break; // break if Ctrl+C pressed
    }        // if Ctrl+C
#endif       //#ifndef __STANDALONE__
  }          // for ever

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

static void SeqEventCallBack(uint32_t aEventType, void* apUserVal)
{
  AppContext* lpAppContext = (AppContext*)apUserVal;

  if(lpAppContext)
  {
    if(aEventType == SEQ_MSG_TYPE_FRAMEDONE)
    {
      printf("Frame done message arrived #%u.\n", lpAppContext->mFrmDoneCnt++);
    } // if frame done arrived
  }   // if user pointer is NULL
} // SeqEventCallBack()

  //***************************************************************************

#ifndef __STANDALONE__
static void SigintHandler(int)
{
  sStop = true;
} // SigintHandler()

//***************************************************************************

int32_t SigintSetup()
{
  static int32_t lRet = SEQ_LIB_SUCCESS;

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
