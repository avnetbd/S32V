/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
* Copyright (c) 2016-17 NXP
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
#include <unistd.h>

#ifdef __STANDALONE__
#include "frame_output_dcu.h"
#define CHNL_CNT io::IO_DATA_CH3
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"
#define CHNL_CNT io::IO_DATA_CH3
#endif // else from #ifdef __STANDALONE__

#include "sdi.hpp"
#include "h264dec_single_stream_c.h"

#include "vdb_log.h"
#include <common_helpers.h>

//***************************************************************************
// constants
//***************************************************************************

//***************************************************************************

// Possible to set input resolution (must be supported by the DCU)
#define WIDTH 1280       ///< width of DDR buffer in pixels
#define HEIGHT 960       ///< height of DDR buffer in pixels
#define DDR_BUFFER_CNT 3 ///< number of DDR buffers per ISP stream

//***************************************************************************

#define FRM_TIME_MSR 120         ///< number of frames to measure the time and fps
#define FRAME_WAIT_TIMEOUT 10000 /// number of ms before frame wait timeout

#define SLEEP_USECS 100

#define H264_IN_BUFF_SIZE 0x7D000
#define H264_IN_PACK_SIZE 0x2d0

#define H264_IN_STRM_FILE "data/common/img_1280x960.h264"

#define FDMA_CHNL_CNT_TO_POP 2 ///< two FDMA channels to be popped

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
  sdi_grabber*     mpGrabber;        ///< pointer to grabber instance
  sdi_FdmaIO*      mpFdma;           ///< pointer to fdma instance
  sdi_H264DecIO*   mpDec;            ///< decoder object pointer
  SDI_DdrBufferArr mYuvColorBuffArr; ///< full frame buffer array

  uint32_t mFileSize;    ///< size of the input file
  char*    mpFileToOpen; ///< name of the input file

  void*     mpSramBuffHandle; ///< SRAM buffer handle
  uintptr_t mSramInBuff;      ///< SRAM buffer physical address
  uint8_t*  mpSram;           ///< SRAM mapping
  uint8_t*  mpSramIn;         ///< SRAM input buffer mapping
  uint32_t  mSramBuffSize;    ///< SRAM buffer size

  bool     mStop;      ///< to signal Ctrl+C pressed
  bool     mFrameDone; ///< to signal frame was finished by ISP
  uint32_t mFrmCnt;    ///< number of frames captured so far

  unsigned long mTimeFrmStart; ///< frame interval start
};                             // struct AppContext

//***************************************************************************
// local functions
//***************************************************************************

/************************************************************************/
/** Configures h264 decoding parameters.
  * 
  * \param arH264Dec H264DecIO object
  * 
  * \return 0 if all ok, != 0 otherwise
  ************************************************************************/
static int32_t DecodingConfigure(sdi_H264DecIO& arH264Dec);

/************************************************************************/
/** User defined call-back function for Sequencer event handling.
  *
  * \param  aEventType defines Sequencer event type
  * \param  apUserVal  pointer to any user defined object
  ************************************************************************/
static void SeqEventCallBack(uint32_t aEventType, void* apUserVal);

/************************************************************************/
/** Provides the h264 decoder with new data input.
  * 
  * \param arContext structure capturing the context of the application
  ************************************************************************/
static void H264DataInput(AppContext& arContext);

/************************************************************************/
/** Prepare everything before executing the main functionality .
  * 
  * \param arContext structure capturing the context of the application
  * 
  * \return 0 if all ok, <0 otherwise 
  ************************************************************************/
static int32_t Prepare(AppContext& arContext);

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
/** Loads H264 input stream data.
  * 
  * \param arContext structure capturing the context of the application
  * 
  * \return 0 if all ok, != 0 otherwise 
  ************************************************************************/
static int32_t H264StreamLoad(AppContext& arContext);

/************************************************************************/
/** Execute main functionality of the application.
  * 
  * \param arContext structure capturing the context of the application
  * 
  * \return 0 if all ok, !=0 otherwise 
  ************************************************************************/
static int32_t Run(AppContext& arContext);

/************************************************************************/
/** Cleanup all resources before application end.
  * 
  * \param arContext structure capturing the context of the application
  * 
  * \return 0 if all ok, != 0 otherwise 
  ************************************************************************/
static int32_t Cleanup(AppContext& arContext);

/************************************************************************/
/** SIGINT handler.
  *
  * \param  aSigNo
  ************************************************************************/
static void SigintHandler(int aSigNo);

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

int main(int argc, char** argv)
{
  int lRet = 0;

  AppContext lContext;
  lContext.mpGrabber        = NULL;
  lContext.mpFdma           = NULL;
  lContext.mpDec            = NULL;
  lContext.mFileSize        = 0;
  lContext.mpFileToOpen     = NULL;
  lContext.mpSramBuffHandle = NULL;
  lContext.mSramInBuff      = 0;
  lContext.mpSram           = NULL;
  lContext.mpSramIn         = NULL;
  lContext.mSramBuffSize    = 0;
  lContext.mStop            = false;
  lContext.mFrameDone       = false;
  lContext.mFrmCnt          = 0;

  //*** process command line parameters ***
  const char helpMsg_str[] =
      "\n**************************************************************\n"
      "** H264 decoder single stream demo\n"
      "** Description:\n"
      "**  o Uses static image to feed stream 0 of H264 decoder\n"
      "**  o ISP converts pixel format from YUV420 to YUV422.\n"
      "**  o Final images displayed on display using DCU.\n"
      "**\n"
      "** Usage:\n"
      "**  o 1 parameter: file to be used as h264dec data input\n"
      "**  o no params => " H264_IN_STRM_FILE
      " file used as data input.\n"
      "**    (file has to be present in working directory).\n"
      "**\n"
      "**************************************************************\n\n";
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg_str);
  if(idxHelp < 0)
  { // print help message even if no help option is provided by the user
    printf("%s", helpMsg_str);
  }

  if(argc > 1)
  {
    lContext.mpFileToOpen = argv[1];

  } // if there is a parameter given
  else
  {
    lContext.mpFileToOpen = (char*)H264_IN_STRM_FILE;
  } // if no other parameter
  printf("**  %s file will be used as input.\n", lContext.mpFileToOpen);

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
  if(LibsPrepare(arContext) != 0)
  {
    printf("Failed to initialize libraries.\n");
    return -1;
  } // if fialed to init libs

  // configure decoding
  if(DecodingConfigure(*arContext.mpDec) != 0)
  {
    printf("Failed to configre H264 decoder.\n");
    return -1;
  } // if failed to configure decoder

  if(DdrBuffersPrepare(arContext) != 0)
  {
    printf("Failed to prepare DDR buffers.\n");
    return -1;
  } // if fialed to prepare DDR buffers

  if(H264StreamLoad(arContext) != 0)
  {
    printf("Failed to load H264 stream file.\n");
    return -1;
  } // if failed to load H264 stream

  // *** prestart grabber ***
  if(arContext.mpGrabber->PreStart() != LIB_SUCCESS)
  {
    printf("Failed to prestart the grabber.\n");
    return -1;
  } // if PreStart() failed

  // *** install sequencer event callback ***
  if(arContext.mpGrabber->SeqEventCallBackInstall(&SeqEventCallBack, &arContext) != LIB_SUCCESS)
  {
    printf("Failed to install Sequencer event callback.\n");
    return -1;
  } // if callback setup failed

  return 0;
} // Prepare()

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

  arContext.mpDec = (sdi_H264DecIO*)arContext.mpGrabber->IoGet(SEQ_OTHRIX_H264DEC);
  if(arContext.mpDec == NULL)
  {
    printf("Failed to get H264Decoder object.\n");
    return -1;
  } // if no Dec object
  return 0;
} // LibsPrepare(AppContext &arContext)

//***************************************************************************

static int32_t DdrBuffersPrepare(AppContext& arContext)
{
  // *** Gray as RGB buffer array ***
  SDI_ImageDescriptor lFrmDesc;

  lFrmDesc = SDI_ImageDescriptor(WIDTH, HEIGHT, RGB888);
  if(arContext.mpFdma->DdrBufferDescSet(FDMA_IX_GRAY_AS_RGB, lFrmDesc) != LIB_SUCCESS)
  {
    printf("Failed to set GrayAsRgb buffer descriptor.\n");
    return -1;
  } // if failed to allocate DDR

  if(arContext.mpFdma->DdrBuffersAlloc(FDMA_IX_GRAY_AS_RGB, DDR_BUFFER_CNT) != LIB_SUCCESS)
  {
    printf("Failed to allocate GrayAsRgb buffers.\n");
    return -1;
  } // if failed to allocate DDR

  // *** UYVY full buffer array ***
  lFrmDesc = SDI_ImageDescriptor(WIDTH * 2, HEIGHT / 2, YUV422Stream_YUYV);

  // allocate contiguous memory
  // memory allocated outside SDI - the mYuvColorBuffArr must not be deallocated
  // before application end => exists in application context
  if(arContext.mYuvColorBuffArr.Allocate(lFrmDesc, DDR_BUFFER_CNT) != LIB_SUCCESS)
  {
    printf("Failed to allocate DDR.\n");
    return -1;
  } // if failed to allocate DDR

  // *** even lines ROI ***
  SDI_DdrBufferArr lYuvColorRoi;
  lYuvColorRoi = arContext.mYuvColorBuffArr.Roi(0, 0, WIDTH, HEIGHT / 2);

  if(lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_0) != LIB_SUCCESS)
  {
    printf("Failed to set channel index.\n");
    return -1;
  } // if frame descriptor setup failed

  if(arContext.mpFdma->DdrBuffersSet(lYuvColorRoi) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor.\n");
    return -1;
  } // if frame descriptor setup failed

  // *** odd lines ROI ***
  lYuvColorRoi = arContext.mYuvColorBuffArr.Roi(WIDTH, 0, WIDTH, HEIGHT / 2);

  if(lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_1) != LIB_SUCCESS)
  {
    printf("Failed to set channel index.\n");
    return -1;
  } // if frame descriptor setup failed
  if(arContext.mpFdma->DdrBuffersSet(lYuvColorRoi) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor.\n");
    return -1;
  } // if frame descriptor setup failed
  return 0;
} // DdrBuffersPrepare(AppContext &arContext)

//***************************************************************************

static int32_t DecodingConfigure(sdi_H264DecIO& arH264Dec)
{
  SDI_H264DecDescriptor lH264DecConfig = arH264Dec.ConfigGet();

  if(lH264DecConfig.mpStreamDesc[0].mActive == 0)
  {
    printf("Error: stream 0 is inactive in the ISP graph.\n");
    return -1;
  } // if stream 0 not active

  if((lH264DecConfig.mpStreamDesc[1].mActive > 0) || (lH264DecConfig.mpStreamDesc[2].mActive > 0) ||
     (lH264DecConfig.mpStreamDesc[3].mActive > 0))
  {
    printf("Error: streams 1-3 are NOT inactive in the ISP graph.\n");
    return -1;
  } // if stream 0 not active

  lH264DecConfig.mDataModeChroma = OFFSET_BINARY_FORMAT;
  lH264DecConfig.mDataFlowMode   = MODE_1;

  lH264DecConfig.mDecConfig.mem_type  = MEMORY_DDR;
  lH264DecConfig.mDecConfig.filter_en = H264DCD_ON;

  // configure only stream 0
  lH264DecConfig.mpStreamDesc[0].mColFmt            = COL_YUV420;
  lH264DecConfig.mpStreamDesc[0].mBitWidth          = MODE_8BITS;
  lH264DecConfig.mpStreamDesc[0].mOutBufRestartMode = H264DCD_OFF;

  arH264Dec.ConfigSet(lH264DecConfig);
  return 0;
} // DecodingConfigure(sdi_H264DecIO &arH264Dec)

//***************************************************************************

static int32_t H264StreamLoad(AppContext& arContext)
{
  // *** prepare static H264 input ***
  FILE* lpFrb = fopen(arContext.mpFileToOpen, "rb");
  if(lpFrb == NULL)
  {
    printf("Failed to open h264 test stream %s.\n", arContext.mpFileToOpen);

    return -1;
  } // if failed to open image file

  if(fseek(lpFrb, 0L, SEEK_END) != 0)
  {
    printf("Failed to find out the size of the input stream.\n");
    return -1;
  } // fseek failed

  long lFileSize = ftell(lpFrb);
  if(lFileSize <= 0)
  {
    printf("Failed to find out the size of the input stream.\n");
    return -1;
  }

  if(fseek(lpFrb, 0L, SEEK_SET) != 0)
  {
    printf("Failed to find out the size of the input stream.\n");
    return -1;
  } // fseek failed

  arContext.mFileSize     = (uint32_t)lFileSize;
  arContext.mSramBuffSize = (arContext.mFileSize + 0x1000) & 0xfffff000;

  // allocate input SRAM buffer (shared for all channels)
  arContext.mpSramBuffHandle = NULL;
  arContext.mSramInBuff      = 0;

  arContext.mpSramBuffHandle =
      OAL_MemoryAllocFlag(arContext.mSramBuffSize, OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_ALLOC_SRAM_SINGLE);
  if(arContext.mpSramBuffHandle == NULL)
  {
    printf("Failed to allocate input SRAM buffer.\n");
    return -1;
  } // if sram alloc failed

  arContext.mSramInBuff = (uintptr_t)OAL_MemoryReturnAddress(arContext.mpSramBuffHandle, ACCESS_PHY);

  if(arContext.mSramInBuff == 0)
  {
    printf("Failed to get physical address for input SRAM buffer.\n");
    return -1;
  } // if failed to get physical address

  // map whole SRAM
  int32_t lFd = open("/dev/oal_noncached", O_RDWR, 0);
  if(lFd < 0)
  {
    printf("Failed open /dev/oal_noncached.\n");
    return -1;
  } // if open failed
  arContext.mpSram = (uint8_t*)mmap(NULL, SRAM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, lFd, SRAM_BASE);

  close(lFd);
  if(arContext.mpSram == NULL)
  {
    printf("Failed to mapp SRAM memory.\n");
    return -1;
  }

  // get Input buffer SRAM mapping
  arContext.mpSramIn = (uint8_t*)((arContext.mSramInBuff - SRAM_BASE) + (uintptr_t)arContext.mpSram);

  // enable dow done interrupt
  //H264DEC_IrqMaskSet(H264_ROW_DONE_IRQ);

  arContext.mFrmCnt = 0;

  // *** read in the H264 stream ***
  size_t lReadSize = fread(arContext.mpSramIn, 1, arContext.mSramBuffSize, lpFrb);

  // close input file
  fclose(lpFrb);

  if(lReadSize != arContext.mFileSize)
  {
    printf("Failed to read full input stram.\n");
    return -1;
  } // if failed to read whole file
  return 0;
} // H264StreamLoad(AppContext &arContext)

//***************************************************************************

static int32_t Run(AppContext& arContext)
{
  int32_t lRet = 0;

  // setup Ctrl+C handler
  if(SigintSetup() != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to register Ctrl+C signal handler.");
    return -1;
  } // if failed to setup Ctrl+C handler

  printf("Press Ctrl+C to terminate the demo.\n");

  //*** Init DCU Output ***
  io::FrameOutputV234Fb lDcuOutput(WIDTH, HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT, DCU_BPP_YCbCr422);

  // *** start grabbing ***
  GETTIME(&arContext.mTimeFrmStart);
  if(arContext.mpGrabber->Start(0, HEIGHT) != LIB_SUCCESS)
  {
    printf("Failed to start the grabber.\n");
    return -1;
  } // if Start() failed

  // fetched frame buffer storage
  SDI_Frame lpFrame[FDMA_CHNL_CNT_TO_POP];

  unsigned long lTimeStart = 0, lTimeEnd = 0, lTimeDiff = 0;
  GETTIME(&lTimeStart);

  for(;;)
  {
    unsigned long lFrameWaitStart = 0, lFrameWaitStop = 0;
    H264DataInput(arContext);

    GETTIME(&lFrameWaitStart);
    while((arContext.mFrameDone == false) && (arContext.mStop == false))
    {
      sleep(0);
      GETTIME(&lFrameWaitStop);
      if((lFrameWaitStop - lFrameWaitStart) > FRAME_WAIT_TIMEOUT)
      {
        lRet = -1;
        printf("Frame Wait timedout.\n");
        break;
      } // check for frame wait timeout
    }   // wait for frame done

    if(lRet == 0) //if all ok so far
    {
      lpFrame[0] = arContext.mpGrabber->FramePop(FDMA_IX_UYVY_0);
      if(lpFrame[0].mUMat.empty())
      {
        lRet = -1;
        printf("NULL pointer data.\n");
        break;
      } // if pop failed

      lpFrame[1] = arContext.mpGrabber->FramePop(FDMA_IX_UYVY_1);
      if(lpFrame[1].mUMat.empty())
      {
        lRet = -1;
        printf("NULL pointer data.\n");
        break;
      } // if pop failed

      lDcuOutput.PutFrame(lpFrame[0].mUMat);

      for(uint32_t i = 0; i < FDMA_CHNL_CNT_TO_POP; i++)
      {
        if(arContext.mpGrabber->FramePush(lpFrame[i]) != LIB_SUCCESS)
        {
          lRet = -1;
          printf("Failed to push SDI frame to channel %u.\n", i);
          break;
        } // if push failed
      }   // for all frame quadrants
      arContext.mFrameDone = false;

      if((arContext.mFrmCnt % FRM_TIME_MSR) == 0)
      {
        GETTIME(&lTimeEnd);
        lTimeDiff  = lTimeEnd - lTimeStart;
        lTimeStart = lTimeEnd;

        printf("%u frames took %lu usec (%5.2ffps)\n", FRM_TIME_MSR, lTimeDiff,
               (FRM_TIME_MSR * 1000000.0) / ((float)lTimeDiff));
      } // if time should be measured
    }   // if frame done
    else
    {
      break;
    } // else from if all ok so far

    if(arContext.mStop || sStop)
    {
      break; // break if Ctrl+C pressed
    }        // if Ctrl+C
  }          // for ever

  return lRet;
} // Run()

//***************************************************************************

static int32_t Cleanup(AppContext& arContext)
{
  int32_t lRet = 0;

  if(arContext.mpGrabber != NULL)
  {
    if(arContext.mpGrabber->Stop() != LIB_SUCCESS)
    {
      printf("Failed to stop the grabber.\n");
      lRet = -1;
    } // if grabber stop failed

    if(arContext.mpGrabber->Release() != LIB_SUCCESS)
    {
      printf("Failed to release grabber resources.\n");
      lRet = -1;
    } // if grabber resources not released

    delete(arContext.mpGrabber);
    arContext.mpGrabber = NULL;
    arContext.mpFdma    = NULL;
    arContext.mpDec     = NULL;
  } // if grabber exists

  if(arContext.mpSram != NULL)
  {
    munmap(arContext.mpSram, SRAM_SIZE);
  } // if not NULL

  arContext.mpSram   = NULL;
  arContext.mpSramIn = NULL;

  if(arContext.mpSramBuffHandle != NULL)
  {
    OAL_MemoryFree(arContext.mpSramBuffHandle);
  } // if SRAM allocated

  arContext.mpSramBuffHandle = NULL;
  arContext.mSramInBuff      = 0;

  if(sdi::Close(0) != LIB_SUCCESS)
  {
    printf("Failed to terminate use of SDI.\n");
    lRet = -1;
  } // if SDI use termination failed

  return lRet;
} // Cleanup()

//***************************************************************************

static void SeqEventCallBack(uint32_t aEventType, void* apParam)
{
  AppContext* lpContext = (AppContext*)apParam;

  if(apParam == NULL)
  {
    printf("SeqEvent callbec parameter is NULL.\n");
    return;
  } // if param is NULL

  switch(aEventType)
  {
    case SEQ_MSG_TYPE_RAB:
    {
      ; // nothing to be done
    }
    break;
    case SEQ_MSG_TYPE_FRAMEDONE:
    {
      lpContext->mFrameDone = true;

      if(((++lpContext->mFrmCnt) % 30) == 0)
      {
        printf("%u frames decoded.\n", lpContext->mFrmCnt);
      } // once per 10 frames
    }   // case SEQ_MSG_TYPE_FRAMEDONE:
    break;
    case SEQ_MSG_TYPE_OTHERERROR:
    {
      lpContext->mStop = true;
    } // case SEQ_MSG_TYPE_OTHERERROR:
    default:
    {
      char lString[SEQ_PRAM_AUX_DATA_SIZE] = {0};

      if(aEventType == SEQ_MSG_TYPE_OTHERERROR)
      {
        printf("Sequencer other error:");
      } // if other error detected
      else
      {
        printf("Unknown event type (%u) received.\n", aEventType);
      } // else from if other error detected
      printf("*** Sequencer error text:\n");
      if(SEQ_MsgAuxDataGet(lString, SEQ_PRAM_AUX_DATA_SIZE) != SEQ_LIB_SUCCESS)
      {
        printf("Failed to get the auxiliary data from PRAM.\n");
      } // if failed to read the error message
      else
      {
        printf("  %s\n", lString);
      } // else from if failed to read the error message
      lpContext->mStop = true;
    } // default
    break;
  } // switch(aEventType)
} // SeqEventCallBack()

//***************************************************************************

static void H264DataInput(AppContext& arContext)
{
  size_t lStop = arContext.mFileSize;

  // *** start H264 decoding ***
  H264DCD_STREAM_DATA_CONFIG lStreamInCfg;
  lStreamInCfg.stream_no = H264DCD_DATASTREAM_1;
  lStreamInCfg.buff_addr = arContext.mSramInBuff;
  lStreamInCfg.buff_size = H264_IN_PACK_SIZE;
  lStreamInCfg.wr_delay  = 0;

  for(uint32_t i = 0; i < lStop / H264_IN_PACK_SIZE; i++)
  {
    H264DEC_InConfig(lStreamInCfg);
#ifdef SLEEP_USECS
    usleep(SLEEP_USECS);
#endif
    lStreamInCfg.buff_addr += H264_IN_PACK_SIZE;
  } // for all packs in jpeg image

  if(lStop % H264_IN_PACK_SIZE != 0)
  {
    lStreamInCfg.buff_size = lStop % H264_IN_PACK_SIZE;
#ifdef SLEEP_USECS
    usleep(SLEEP_USECS);
#endif
    H264DEC_InConfig(lStreamInCfg);
  } // if some leftover
} // H264DataInput()

//***************************************************************************

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
/* EOF */
