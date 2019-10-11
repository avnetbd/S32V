/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
* Copyright (c) 2016-17 NXP
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

#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "frame_output_v234fb.h"
#define CHNL_CNT io::IO_DATA_CH3

#include "sdi.hpp"
#include "jpeg_4stream_c.h"

#include "isp_jpegdec.h"

#include "vdb_log.h"
#include <common_helpers.h>
#include <string>

//***************************************************************************
// constants
//***************************************************************************

//***************************************************************************

// Possible to set input resolution (must be supported by the DCU)
#define WIDTH 1280       ///< width of DDR buffer in pixels
#define HEIGHT 800       ///< height of DDR buffer in pixels
#define DDR_BUFFER_CNT 3 ///< number of DDR buffers per ISP stream

//#define DO_BUFFER_LOG

//***************************************************************************

#define FRM_TIME_MSR 120         ///< number of frames to measure the time and fps
#define FRAME_WAIT_TIMEOUT 10000 /// number of us before frame wait timeout

#define SLEEP_USECS 50

#define JPEG_IN_BUFF_SIZE 0x32000

#define JPEG_IN_PACK_SIZE 0xa00

#define JPEG_ROW_DONE_IRQ 0x1000000
#define JPEG_IN_IMG_FILE "data/common/jpg_dcd_test.jpg"

#define JPEG_CHAN_CNT 4

//***************************************************************************
// macros
//***************************************************************************

#define GETTIME(time)                                                                                                  \
  {                                                                                                                    \
    struct timeval lTime;                                                                                              \
    gettimeofday(&lTime, 0);                                                                                           \
    *time = (lTime.tv_sec * 1000000 + lTime.tv_usec);                                                                  \
  }

//***************************************************************************
// types
//***************************************************************************
struct AppContext
{
  sdi_grabber*     mpGrabber;        ///< pointer to grabber instance
  sdi_FdmaIO*      mpFdma;           ///< pointer to fdma instance
  sdi_JpegDecIO*   mpDec;            ///< decoder object pointer
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
/** User defined call-back function for Sequencer event handling.
  * 
  * \param  aEventType defines Sequencer event type
  * \param  apParam    pointer to any user defined object 
  ************************************************************************/
static void SeqEventCallBack(uint32_t aEventType, void* apParam);

/************************************************************************/
/** Provides the jpeg decoder with new data input.
  * 
  * \param arContext structure capturing the context of the application
  ************************************************************************/
static void JpegDataInput(AppContext& arContext);

/************************************************************************/
/** Configures jpeg decoding parameters.
  * 
  * \param arJpegDec JpegDecIO object
  * 
  *\return 0 if all ok, -1 otherwise
  ************************************************************************/
static int32_t DecodingConfigure(sdi_JpegDecIO& arJpegDec);

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
/** Loads Jpeg input stream data.
  * 
  * \param arContext structure capturing the context of the application
  * 
  * \return 0 if all ok, != 0 otherwise 
  ************************************************************************/
static int32_t JpegStreamLoad(AppContext& arContext);

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
int32_t SigintSetup(void);

//***************************************************************************

static bool          sStop      = false; ///< to signal Ctrl+c from command line
static unsigned long sTimeStart = 0;

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
      "** JPEG decoder 4stream demo\n"
      "** Description:\n"
      "**  o Uses static image to feed all 4 streams of JPEG decoder\n"
      "**  o ISP converts pixel format from YUV420 to YUV422,\n"
      "**    subsamples data 2x and creates fullsize frames in DDR.\n"
      "**  o Final images displayed on display using DCU.\n"
      "**\n"
      "** Usage:\n"
      "**  o 1 parameter: file to be used as JpgeDec data input\n"
      "**  o no params => " JPEG_IN_IMG_FILE
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
    lContext.mpFileToOpen = (char*)JPEG_IN_IMG_FILE;
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
  // enable LIBS
  if(LibsPrepare(arContext) != 0)
  {
    printf("Failed to prepare libraries.\n");
    return -1;
  } // if failed to configure decoder

  if(DecodingConfigure(*arContext.mpDec) != 0)
  {
    printf("Failed to configre Jpeg decoder.\n");
    return -1;
  } // if failed to configure decoder

  if(DdrBuffersPrepare(arContext) != 0)
  {
    printf("Failed to prepare DDR buffers.\n");
    return -1;
  } // if fialed to prepare DDR buffers

  if(JpegStreamLoad(arContext) != 0)
  {
    printf("Failed to load Jpeg stream file.\n");
    return -1;
  } // if failed to load Jpeg stream

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

  arContext.mpDec = (sdi_JpegDecIO*)arContext.mpGrabber->IoGet(SEQ_OTHRIX_JPEGDEC);
  if(arContext.mpDec == NULL)
  {
    printf("Failed to get JPEGDecoder object.\n");
    return -1;
  } // if no Dec object
  return 0;
} // LibsPrepare(AppContext &arContext)

//***************************************************************************

static int32_t DdrBuffersPrepare(AppContext& arContext)
{
  // *** UYVY full buffer array ***
  SDI_ImageDescriptor lFrmDesc;
  lFrmDesc = SDI_ImageDescriptor(WIDTH, HEIGHT, YUV422Stream_YUYV);

  if(arContext.mYuvColorBuffArr.Allocate(lFrmDesc, DDR_BUFFER_CNT) != LIB_SUCCESS)
  {
    printf("Failed to allocate DDR.\n");
    return -1;
  } // if failed to allocate DDR

  // *** get quadrant 0 ***
  SDI_DdrBufferArr lYuvColorRoi;
  lYuvColorRoi = arContext.mYuvColorBuffArr.Roi(0, 0, WIDTH / 2, HEIGHT / 2);

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

  // *** get quadrant 1 ***
  lYuvColorRoi = arContext.mYuvColorBuffArr.Roi(WIDTH / 2, 0, WIDTH / 2, HEIGHT / 2);

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

  // *** get quadrant 2 ***
  lYuvColorRoi = arContext.mYuvColorBuffArr.Roi(0, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);

  if(lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_2) != LIB_SUCCESS)
  {
    printf("Failed to set channel index.\n");
    return -1;
  } // if frame descriptor setup failed
  if(arContext.mpFdma->DdrBuffersSet(lYuvColorRoi) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor.\n");
    return -1;
  } // if frame descriptor setup failed

  // *** get quadrant 3 ***
  lYuvColorRoi = arContext.mYuvColorBuffArr.Roi(WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);

  if(lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_3) != LIB_SUCCESS)
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

static int32_t JpegStreamLoad(AppContext& arContext)
{
  // *** prepare static Jpeg input ***
  FILE* lpFrb = fopen(arContext.mpFileToOpen, "rb");
  if(lpFrb == NULL)
  {
    printf("Failed to open jpeg test stream %s.\n", arContext.mpFileToOpen);

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

  arContext.mFrmCnt = 0;

  // *** read in the Jpeg stream ***
  size_t lReadSize = fread(arContext.mpSramIn, 1, arContext.mSramBuffSize, lpFrb);

  // close input file
  fclose(lpFrb);

  if(lReadSize != arContext.mFileSize)
  {
    printf("Failed to read full input stram.\n");
    return -1;
  } // if failed to read whole file
  return 0;
} // JpegStreamLoad(AppContext &arContext)

//***************************************************************************

static int32_t Run(AppContext& arContext)
{
  int32_t lRet = 0;
  // setup Ctrl+C handler
  if(SigintSetup() != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to register Ctrl+C signal handler.");
    return -1;
  } // if failed to set Sigint (Ctrl+C) handler

  printf("Press Ctrl+C to terminate the demo.\n");

  //*** Init DCU Output ***
  io::FrameOutputV234Fb lDcuOutput(WIDTH, HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT, DCU_BPP_YCbCr422);

  // *** start grabbing ***
  GETTIME(&arContext.mTimeFrmStart);
  if(arContext.mpGrabber->Start(0, JPEG_CHAN_CNT * HEIGHT) != LIB_SUCCESS)
  {
    printf("Failed to start the grabber.\n");
    return -1;
  } // if Start() failed

  // fetched frame buffer storage
  SDI_Frame lpFrame[FDMA_IX_END];

  unsigned long lTimeStart = 0, lTimeEnd = 0, lTimeDiff = 0;
  GETTIME(&lTimeStart);

  for(;;)
  {
    unsigned long lFrameWaitStart = 0, lFrameWaitStop = 0;
    JpegDataInput(arContext);

    GETTIME(&lFrameWaitStart);
    while((arContext.mFrameDone == false) && (arContext.mStop == false))
    {
      sleep(0);
      GETTIME(&lFrameWaitStop);
      if((lFrameWaitStop - lFrameWaitStart) > FRAME_WAIT_TIMEOUT)
      {
        lRet = -1;
        printf("Frame Wait timedout start %lu stop %lu.\n", lFrameWaitStart, lFrameWaitStop);
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

      lpFrame[2] = arContext.mpGrabber->FramePop(FDMA_IX_UYVY_2);
      if(lpFrame[2].mUMat.empty())
      {
        lRet = -1;
        printf("NULL pointer data.\n");
        break;
      } // if pop failed

      lpFrame[3] = arContext.mpGrabber->FramePop(FDMA_IX_UYVY_3);
      if(lpFrame[3].mUMat.empty())
      {
        lRet = -1;
        printf("NULL pointer data.\n");
        break;
      } // if pop failed

      lDcuOutput.PutFrame(lpFrame[0].mUMat);

      for(uint32_t i = 0; i < FDMA_IX_END; i++)
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
    }   //if all ok so far
    else
    {
      break;
    } // else from if all ok so far

    if(arContext.mStop || sStop)
    {
      break; // break if Ctrl+C pressed
    }        // if Ctrl+C
  }          // for ever

  return 0;
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

static int32_t DecodingConfigure(sdi_JpegDecIO& arJpegDec)
{
  SDI_JpegDecDescriptor lJpegDesc = arJpegDec.ConfigGet();
  if(lJpegDesc.mpStreamDesc[0].mHeight == 0)
  {
    printf("Bad jpeg descriptor.\n");
    return -1;
  } // if no height
  lJpegDesc.mMode        = CONTEXT_SWITCH;
  lJpegDesc.mOutBufSmpl  = JPEGDCD_ON;
  lJpegDesc.mStreamCnt   = 4;
  lJpegDesc.mBankStride  = 0;
  lJpegDesc.mRstInterval = 16;

  for(uint32_t i = DATASTREAM_1; i <= DATASTREAM_4; i++)
  {
    lJpegDesc.mpStreamDesc[i].mPrecision     = PREC_8BITS;
    lJpegDesc.mpStreamDesc[i].mWrapperConfig = JPEGDCD_OFF;
    lJpegDesc.mpStreamDesc[i].mComponentCnt  = 3;

    // sampling factors
    lJpegDesc.mpStreamDesc[i].mC1_samplingfactor_h = 1;
    lJpegDesc.mpStreamDesc[i].mC1_samplingfactor_v = 1;
    lJpegDesc.mpStreamDesc[i].mC2_samplingfactor_h = 2;
    lJpegDesc.mpStreamDesc[i].mC2_samplingfactor_v = 2;
    lJpegDesc.mpStreamDesc[i].mC3_samplingfactor_h = 2;
    lJpegDesc.mpStreamDesc[i].mC3_samplingfactor_v = 2;
    lJpegDesc.mpStreamDesc[i].mC4_samplingfactor_h = 1;
    lJpegDesc.mpStreamDesc[i].mC4_samplingfactor_v = 1;
  } // for all possible streams

  if(arJpegDec.ConfigSet(lJpegDesc) != LIB_SUCCESS)
  {
    printf("Failed to configure decoding.\n");
    return -1;
  } // if config failed

  return 0;
} // DecodingConfigure(SDI_JpegDecDescriptor &arJpegDec)

//***************************************************************************

void JpegDataInput(AppContext& arContext)
{
  size_t lStop = arContext.mFileSize;

  // *** start JPEG decoding ***
  STREAM_DATA_CONFIG lStreamInCfg;
  lStreamInCfg.buff_addr = arContext.mSramInBuff;
  lStreamInCfg.buff_size = JPEG_IN_PACK_SIZE;

  GETTIME(&sTimeStart);
  for(uint32_t i = 0; i < lStop / JPEG_IN_PACK_SIZE; i++)
  {
    lStreamInCfg.stream_no = DATASTREAM_1;
    JPEGDEC_InConfig(&lStreamInCfg);

#ifdef SLEEP_USECS
    usleep(SLEEP_USECS);
#endif
    lStreamInCfg.stream_no = DATASTREAM_2;
    JPEGDEC_InConfig(&lStreamInCfg);
#ifdef SLEEP_USECS
    usleep(SLEEP_USECS);
#endif
    lStreamInCfg.stream_no = DATASTREAM_3;
    JPEGDEC_InConfig(&lStreamInCfg);
#ifdef SLEEP_USECS
    usleep(SLEEP_USECS);
#endif
    lStreamInCfg.stream_no = DATASTREAM_4;
    JPEGDEC_InConfig(&lStreamInCfg);
#ifdef SLEEP_USECS
    usleep(SLEEP_USECS);
#endif
    lStreamInCfg.buff_addr += JPEG_IN_PACK_SIZE;
  } // for all packs in jpeg image

  if(lStop % JPEG_IN_PACK_SIZE != 0)
  {
    lStreamInCfg.buff_size = lStop % JPEG_IN_PACK_SIZE;
#ifdef SLEEP_USECS
    usleep(SLEEP_USECS);
#endif
    lStreamInCfg.stream_no = DATASTREAM_1;
    JPEGDEC_InConfig(&lStreamInCfg);
    lStreamInCfg.stream_no = DATASTREAM_2;
    JPEGDEC_InConfig(&lStreamInCfg);
    lStreamInCfg.stream_no = DATASTREAM_3;
    JPEGDEC_InConfig(&lStreamInCfg);
    lStreamInCfg.stream_no = DATASTREAM_4;
    JPEGDEC_InConfig(&lStreamInCfg);
  } // if some leftover
} // JpegDataInput()

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

//***************************************************************************
/* EOF */
