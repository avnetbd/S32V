/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor, Inc.
* Copyright (c) 2016-2017 NXP
* 
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
#include "admir_bgr.h"
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"
#endif // else from #ifdef __STANDALONE__

#include "sdi.hpp"
#include "static_simple_c.h"
#include "vdb_log.h"
#include <common_helpers.h>

//***************************************************************************
// constants
//***************************************************************************

// Expected input image properties
#define WIDTH 1920         ///< width of DDR buffer in pixels
#define HEIGHT 1080        ///< height of DDR buffer in pixels
#define DDR_IN_BUFF_CNT 1  ///< number of DDR buffer in input stream
#define DDR_OUT_BUFF_CNT 3 ///< number of DDR buffers in output stream

#define CHNL_CNT io::IO_DATA_CH3 ///< number of bytes per pixel
#define IN_IMG_BYTE_CNT (WIDTH * HEIGHT * CHNL_CNT)
#define IN_IMG_FILE "data/common/admir_bgr_1920x1080_24bpp.raw"

//***************************************************************************

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
  char*    mpImgFile; ///< name of the file to be read
  uint8_t* mpImgData; ///< input image data buffer

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
  * 
  * \return 0 if all ok, <0 otherwise 
  ************************************************************************/
static int32_t ContextInit(AppContext& arContext);

/************************************************************************/
/** Loads input image data.
  * 
  * \param arContext structure capturing the context of the application
  * 
  * \return 0 if all ok, <0 otherwise 
  ************************************************************************/
static int32_t InImgRead(AppContext& arContext);

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

//*****************************************************************************
// function declarations
//*****************************************************************************

#if defined(__STANDALONE__) && defined(__ARMV8)
extern "C" {
void flush_dcache_range(void*, unsigned int);
}
#endif

//*****************************************************************************
// static variables
//*****************************************************************************

static bool sStop = false; ///< to signal Ctrl+c from command line

int main(int argc, char** argv)
#else  // #ifndef __STANDALONE__
int main(int argc, char** argv)
#endif // else #ifndef __STANDALONE__
{
  int lRet = 0;

  AppContext lContext;

  //*** process command line parameters ***
  const char helpMsg_str[] =
      "\n**************************************************************\n"
      "** static_simple -> dcu demo\n"
      "**\n"
      "** Usage:\n"
#ifdef __STANDALONE__
      "**  o no cmd line parameters available.\n"
#else
      "**  o 1 optional parameter: img_file\n"
      "**    - required image properties: 1920x1080, 24bpp, BGR.\n"
#endif
      "**\n"
      "**************************************************************\n\n";

  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg_str);
  if(idxHelp < 0)
  { // print help message even if no help option is provided by the user
    printf("%s", helpMsg_str);
  }

#ifndef __STANDALONE__
  fflush(stdout);
  sleep(1);

  switch(argc)
  {
    case 1:
    {
      lContext.mpImgFile = (char*)IN_IMG_FILE;
    } // case 1: default image
    break;
    case 2:
    {
      lContext.mpImgFile = argv[1];
      printf("%s file will be used as input image.\n", argv[1]);
    } // case 2: user defined image
    break;
    default:
    {
      printf("Error. Unsupported command line arguments. Terminating...\n");
      return -1;
    } // default: bad parameter set
  }   // switch argc
#else
  lContext.mpImgFile = (char*)IN_IMG_FILE;
#endif // else from ifndef __STANDALONE__

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
} // main()

//***************************************************************************

static int32_t Prepare(AppContext& arContext)
{
  // init app context
  if(ContextInit(arContext) != 0)
  {
    printf("Failed to init application context.\n");
    return -1;
  } // if failed to init context

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

  if(arContext.mpGrabber->SeqEventCallBackInstall(&SeqEventCallBack, &arContext) != LIB_SUCCESS)
  {
    printf("Failed to install Sequencer event callback.\n");
    return -1;
  } // if callback setup failed

  return 0;
} // Prepare()

//***************************************************************************

static int32_t ContextInit(AppContext& arContext)
{
  arContext.mpGrabber   = NULL;
  arContext.mpFdma      = NULL;
  arContext.mpImgData   = NULL;
  arContext.mError      = false;
  arContext.mFrmCnt     = 0;
  arContext.mFrmDoneCnt = 0;

  return InImgRead(arContext);
} // ContextInit()

//***************************************************************************

static int32_t InImgRead(AppContext& arContext)
{
#ifdef __STANDALONE__
  arContext.mpImgData = (uint8_t*)lStatic_image_data;
#else
  // *** prepare static H264 input ***
  FILE* lpFrb = fopen(arContext.mpImgFile, "rb");
  if(lpFrb == NULL)
  {
    printf("Failed to open input image file %s.\n", arContext.mpImgFile);
    return -1;
  } // if failed to open image file

  if(fseek(lpFrb, 0L, SEEK_END) != 0)
  {
    printf("Failed to find the end of the input file.\n");
    fclose(lpFrb);
    return -1;
  } // fseek failed

  long lFileSize = ftell(lpFrb);
  if(lFileSize < IN_IMG_BYTE_CNT)
  {
    printf("Only %ld bytes in the file. %u bytes needed.\n", lFileSize, IN_IMG_BYTE_CNT);
    fclose(lpFrb);
    return -1;
  } // if ftell failed

  if(fseek(lpFrb, 0L, SEEK_SET) != 0)
  {
    printf("Failed to find the sstart of the input file.\n");
    fclose(lpFrb);
    return -1;
  } // fseek failed

  // allocate input image data
  arContext.mpImgData = (uint8_t*)malloc(sizeof(uint8_t) * IN_IMG_BYTE_CNT);
  if(arContext.mpImgData == NULL)
  {
    printf("Failed to allocate input image data buffer.\n");
    fclose(lpFrb);
    return -1;
  } // if in img alloc failed

  // *** read in the H264 stream ***
  size_t lReadSize = fread(arContext.mpImgData, 1, IN_IMG_BYTE_CNT, lpFrb);

  // close input file
  fclose(lpFrb);

  if(lReadSize != IN_IMG_BYTE_CNT)
  {
    printf("Failed to read full input image.\n");
    free(arContext.mpImgData);
    return -1;
  } // if failed to read whole file
#endif // else from ifdef __STANDALONE__
  return 0;
} // InImgRead(AppContext &arContext)

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
  // *** RGB full buffer array ***
  // modify DDR frame geometry to fit display output
  SDI_ImageDescriptor lFrmDesc;
  lFrmDesc = SDI_ImageDescriptor(WIDTH, HEIGHT, BGR888);

  if(arContext.mpFdma->DdrBuffersAlloc(FDMA_IX_FDMA_0, DDR_IN_BUFF_CNT, lFrmDesc) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor.\n");
    return -1;
  } // if frame descriptor setup failed

  // *** fill with content ***
  vsdk::UMat umat;
  arContext.mpFdma->DdrBufferQuery(FDMA_IX_FDMA_0, 0, umat);
  vsdk::Mat matrix = umat.getMat(OAL_USAGE_NONCACHED);
  memcpy(matrix.data, arContext.mpImgData, IN_IMG_BYTE_CNT);

  if(arContext.mpFdma->DdrBufferDescSet(FDMA_IX_FDMA_1, lFrmDesc) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor.\n");
    return -1;
  } // if frame descriptor setup failed

  // allocate DDR buffers
  if(arContext.mpFdma->DdrBuffersAlloc(DDR_OUT_BUFF_CNT) != LIB_SUCCESS)
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
  io::FrameOutputDCU lDcuOutput(WIDTH, HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT);
#else  // #ifdef __STANDALONE__
  // setup Ctrl+C handler
  if(SigintSetup() != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to register Ctrl+C signal handler.");
    return -1;
  }

  printf("Press Ctrl+C to terminate the demo.\n");

  io::FrameOutputV234Fb lDcuOutput(WIDTH, HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT);
#endif // else from #ifdef __STANDALONE__

  unsigned long lTimeStart = 0, lTimeEnd = 0, lTimeDiff = 0;

  // *** start grabbing ***
  GETTIME(&lTimeStart);
  if(arContext.mpGrabber->Start(0, HEIGHT) != LIB_SUCCESS)
  {
    printf("Failed to start the grabber.\n");
    return -1;
  } // if Start() failed

  SDI_Frame              lFrame;
  SEQM_start_node_para_t np[1];

  // *** grabbing/processing loop ***
  for(;;)
  {
    np[0].mNode = 0;
    np[0].mChan = 0;
    // *** start graph ***
    if(seq_startNode(np, 1) != SEQ_LIB_SUCCESS)
    {
      VDB_LOG_NOTE("Failed to send a message.\n");
      return (-1);
    }

    lFrame = arContext.mpGrabber->FramePop(FDMA_IX_FDMA_1);

    if(lFrame.mUMat.empty())
    {
      printf("Failed to grab image number %u\n", arContext.mFrmCnt);
      arContext.mError = true;
      break;
    } // if pop failed

    arContext.mFrmCnt++;

    lDcuOutput.PutFrame(lFrame.mUMat);

    if(arContext.mpGrabber->FramePush(lFrame) != LIB_SUCCESS)
    {
      printf("Failed to push image number %u\n", arContext.mFrmCnt);
      arContext.mError = true;
      break;
    } // if push failed

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

  if(arContext.mpImgData != NULL)
  {
    free(arContext.mpImgData);
  } // if image data available

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
      lpAppContext->mFrmDoneCnt++;
      //      printf("Frame done message arrived #%u.\n", lpAppContext->mFrmDoneCnt);
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
