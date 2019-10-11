/*
  Copyright (c) 2016-2018 NXP Semiconductors
  All Rights Reserved.

  This file contains sample code only.

  THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*/

/*==================================================================================================
                                         INCLUDE FILES
==================================================================================================*/
#include "video_app.h"
#include "video_app_cfg.h"
#include "frame_output_v234fb.h"
#include "sdi.hpp" /* grabber */

#if(SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
#include "h264dec_single_stream_c.h"
#elif(FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
#include "h264dec_4stream_c.h"
#else
#error Unsupported stream variant
#endif /* APP_CFG_STREAM_VARIANT */

//#include "isp_sram.h"
#include "dec_h264_hl.h"
#include "oal.h"

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
#define FRM_POP_ATOMIC_TIMEOUT 2U /* Miliseconds */
#define CHNL_CNT io::IO_DATA_CH3
#define DDR_BUFFER_CNT 3 ///< number of DDR buffers per ISP stream
#if(SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
#define STRM_CNT 1
#elif(FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
#define STRM_CNT 4
#endif /* APP_CFG_STREAM_VARIANT */

/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/
static io::FrameOutputV234Fb lDcuOutput;
static bool                  sFrmDone = false;
static uint8_t*              lpSram;
volatile bool                dec_abort_reading;
volatile bool                async_error = false;

/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#if(SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
static int dec_h264_prepare_single_stream_input(SDI_DdrBufferArr* lYuvColorBuffArr,
                                                sdi_FdmaIO*       lpFdma,
                                                int               Width,
                                                int               Height);
static int dec_h264_process_frame_single_stream_input(sdi_grabber* lpGrabber);
#elif(FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
static int dec_h264_prepare_four_stream_inputs(SDI_DdrBufferArr* lYuvColorBuffArr,
                                               sdi_FdmaIO*       lpFdma,
                                               int               Width,
                                               int               Height);
static int dec_h264_process_four_stream_inputs(sdi_grabber* lpGrabber);
#endif /* APP_CFG_STREAM_VARIANT */
static void SeqEventCallBack(uint32_t aEventType, void*);

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/
int dec_h264_shutdown(void)
{
  if(lpSram != NULL)
  {
    munmap(lpSram, SRAM_SIZE);
  }

  return 0;
}

int dec_h264_prepare_ios(sdi_grabber* lpGrabber, int Width, int Height)
{
  int                        RetVal;
  static SDI_DdrBufferArr    lYuvColorBuffArr;
  static SDI_ImageDescriptor lFrmDesc;
  static sdi_FdmaIO*         lpFdma;
  static bool                bColBufAllocated = false;

  /*
        Initialize the frame buffer output
    */
  lDcuOutput.Init(Width, Height, io::IO_DATA_DEPTH_08, CHNL_CNT, DCU_BPP_YCbCr422);

  // Get access to FDMA
  lpFdma = (sdi_FdmaIO*)lpGrabber->IoGet(SEQ_OTHRIX_FDMA);
  if(lpFdma == NULL)
  {
    printf("Failed to get FDMA object.\n");
    return -1;
  } // if no FDMA object

  // Get access to decoder
  sdi_H264DecIO* mpDec = (sdi_H264DecIO*)lpGrabber->IoGet(SEQ_OTHRIX_H264DEC);
  if(mpDec == NULL)
  {
    printf("Failed to get H264Decoder object.\n");
    return -1;
  } // if no Dec object

  // Configure decoder
  SDI_H264DecDescriptor lH264DecConfig = mpDec->ConfigGet();
  lH264DecConfig.mDataModeChroma       = OFFSET_BINARY_FORMAT;
  lH264DecConfig.mDataFlowMode         = MODE_1;
  lH264DecConfig.mDecConfig.mem_type   = MEMORY_DDR;
  lH264DecConfig.mDecConfig.filter_en  = H264DCD_ON;
  for(uint32_t i = 0; i < STRM_CNT; i++)
  {
    if(lH264DecConfig.mpStreamDesc[i].mActive == 0)
    {
      printf("Error: h264 channel %u is not active in ISP graph.", i);
      return -1;
    } // if stream not active
    /*CompSync2*/
    lH264DecConfig.mpStreamDesc[i].mColFmt            = COL_YUV420;
    lH264DecConfig.mpStreamDesc[i].mBitWidth          = MODE_8BITS;
    lH264DecConfig.mpStreamDesc[i].mOutBufRestartMode = H264DCD_OFF;
  } // for all streams
  mpDec->ConfigSet(lH264DecConfig);

#if(SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
  lFrmDesc = SDI_ImageDescriptor(Width, Height, RGB888);
  if(LIB_SUCCESS != lpFdma->DdrBufferDescSet(FDMA_IX_GRAY_AS_RGB, lFrmDesc))
  {
    printf("Failed to set GrayAsRgb buffer descriptor.\n");
    return -1;
  } // if failed to allocate DDR
  //*** allocate GrayAsRGB DDR buffer ***
  if(LIB_SUCCESS != lpFdma->DdrBuffersAlloc(FDMA_IX_GRAY_AS_RGB, DDR_BUFFER_CNT))
  {
    printf("Failed to allocate GrayAsRgb buffers.\n");
    return -1;
  } // if failed to allocate DDR

  lFrmDesc = SDI_ImageDescriptor(Width * 2, Height / 2, YUV422Stream_YUYV);
#elif(FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
  lFrmDesc = SDI_ImageDescriptor(Width, Height, YUV422Stream_YUYV);
#endif /* APP_CFG_STREAM_VARIANT */

  // Allocate contiguous DDR memory
  // Only once. This hackish approach proved to be more reliable than using alloc & free approach
  if(false == bColBufAllocated)
  {
    if(LIB_SUCCESS != lYuvColorBuffArr.Allocate(lFrmDesc, DDR_BUFFER_CNT))
    {
      printf("Failed to allocate DDR.\n");
      return -1;
    } // if failed to allocate DDR
    else
    {
      bColBufAllocated = true;
    }
  }

    /* SOURCE and STREAM NUMBER SPECIFIC */

#if(SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
  RetVal = dec_h264_prepare_single_stream_input(&lYuvColorBuffArr, lpFdma, Width, Height);
#elif(FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
  RetVal   = dec_h264_prepare_four_stream_inputs(&lYuvColorBuffArr, lpFdma, Width, Height);
#endif /* APP_CFG_STREAM_VARIANT */

  /* Hook a sequencer event callback */
  if(LIB_SUCCESS != lpGrabber->SeqEventCallBackInstall(&SeqEventCallBack, NULL))
  {
    printf("Failed to install Sequencer event callback.\n");
    return -1;
  } // if callback setup failed

  return RetVal;
}

int dec_h264_process_frame(sdi_grabber* lpGrabber)
{
#if(SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
  return dec_h264_process_frame_single_stream_input(lpGrabber);
#elif(FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
  return dec_h264_process_four_stream_inputs(lpGrabber);
#endif /* APP_CFG_STREAM_VARIANT */
}

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/
#if(FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
static int dec_h264_prepare_four_stream_inputs(SDI_DdrBufferArr* lYuvColorBuffArr,
                                               sdi_FdmaIO*       lpFdma,
                                               int               Width,
                                               int               Height)
{
  /*  Instance-specific data */
  static SDI_DdrBufferArr lYuvColorRoi;

  // *** get quadrant 0 ***
  lYuvColorRoi = lYuvColorBuffArr->Roi(0, 0, Width / 2, Height / 2);

  lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_0);
  lpFdma->DdrBuffersSet(lYuvColorRoi);

  // *** get quadrant 1 ***
  lYuvColorRoi = lYuvColorBuffArr->Roi(Width / 2, 0, Width / 2, Height / 2);

  lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_1);
  lpFdma->DdrBuffersSet(lYuvColorRoi);

  // *** get quadrant 2 ***
  lYuvColorRoi = lYuvColorBuffArr->Roi(0, Height / 2, Width / 2, Height / 2);

  lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_2);
  lpFdma->DdrBuffersSet(lYuvColorRoi);

  // *** get quadrant 3 ***
  lYuvColorRoi = lYuvColorBuffArr->Roi(Width / 2, Height / 2, Width / 2, Height / 2);

  lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_3);
  lpFdma->DdrBuffersSet(lYuvColorRoi);

  return 0;
}

static int dec_h264_process_four_stream_inputs(sdi_grabber* lpGrabber)
{
  /*  Fetched frame buffer storage */
  static SDI_Frame lpFrame[4];
  uint32_t         i           = 0U;
  uint32_t         timeout_ctr = 500U; // 500 * 2 (the FramePop timeout) = 1000 ms in total

  do
  { // Using loop of short waits instead of one long wait to be able to abort reading quickly
    static const uint32_t channel_idx[] = {FDMA_IX_UYVY_0, FDMA_IX_UYVY_1, FDMA_IX_UYVY_2, FDMA_IX_UYVY_3};

    if(true == dec_abort_reading)
      return -1; // Reading was aborted
    if(false == sFrmDone)
    { // Just wait for signal
      usleep(1000U * FRM_POP_ATOMIC_TIMEOUT);
    }
    else
    { // Try to pop the frames, one by one
      lpFrame[i] = lpGrabber->FramePop(channel_idx[i], FRM_POP_ATOMIC_TIMEOUT);
      if(!lpFrame[i].mUMat.empty())
        i++; // One frame was readed
    }
    timeout_ctr--;
    if(0U == timeout_ctr)
    {
      printf("[dec_h264_hl] Frame pop FAILED\n");
      return -1; // Waiting for too long
    }
  } while(i < 4U);

  lDcuOutput.PutFrame(lpFrame[0].mUMat);

  /*  For each frame quadrant */
  for(uint32_t i = 0U; i < 4U; i++)
  {
    if(lpGrabber->FramePush(lpFrame[i]) != LIB_SUCCESS)
    {
      printf("[dec_h264_hl] Frame push FAILED\n");
      return -1;
    }
  }
  sFrmDone = false;

  return 0;
}

#elif(SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
static int dec_h264_prepare_single_stream_input(SDI_DdrBufferArr* lYuvColorBuffArr,
                                                sdi_FdmaIO*       lpFdma,
                                                int               Width,
                                                int               Height)
{
  /*  Instance-specific data */
  static SDI_ImageDescriptor lFrmDesc;
  static SDI_DdrBufferArr    lYuvColorRoi;

  // *** get quadrant 0 ***
  lYuvColorRoi = lYuvColorBuffArr->Roi(0, 0, Width, Height / 2);

  lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_0);
  lpFdma->DdrBuffersSet(lYuvColorRoi);

  // *** get quadrant 1 ***
  lYuvColorRoi = lYuvColorBuffArr->Roi(Width, 0, Width, Height / 2);

  lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_1);
  lpFdma->DdrBuffersSet(lYuvColorRoi);

  return 0;
}

static int dec_h264_process_frame_single_stream_input(sdi_grabber* lpGrabber)
{
  /*  Fetched frame buffer storage */
  static SDI_Frame lpFrame[2];
  uint32_t         i           = 0U;
  uint32_t         timeout_ctr = 500U; // 500 * 2 (the FramePop timeout) = 1000 ms in total

  do
  { // Using loop of short waits instead of one long wait to be able to abort reading quickly
    static const uint32_t channel_idx[] = {FDMA_IX_UYVY_0, FDMA_IX_UYVY_1};

    if(true == dec_abort_reading)
      return -1; // Reading was aborted
    if(false == sFrmDone)
    { // Just wait for signal
      usleep(1000U * FRM_POP_ATOMIC_TIMEOUT);
    }
    else
    { // Try to pop the frames, one by one
      lpFrame[i] = lpGrabber->FramePop(channel_idx[i], FRM_POP_ATOMIC_TIMEOUT);
      if(!lpFrame[i].mUMat.empty())
        i++; // One frame was readed
    }
    timeout_ctr--;
    if(0U == timeout_ctr)
    {
      printf("[dec_h264_hl] Frame pop FAILED\n");
      return -1; // Waiting for too long
    }
  } while(i < 2U);

  lDcuOutput.PutFrame(lpFrame[0].mUMat);

  for(uint32_t i = 0U; i < 2U; i++)
  {
    if(lpGrabber->FramePush(lpFrame[i]) != LIB_SUCCESS)
    {
      printf("[dec_h264_hl] Frame push FAILED\n");
      return -1;
    }
  }

  sFrmDone = false;

  return 0;
}
#endif /* APP_CFG_STREAM_VARIANT */

static void SeqEventCallBack(uint32_t aEventType, void*)
{
  if(aEventType == SEQ_MSG_TYPE_FRAMEDONE)
  {
    sFrmDone = true;
  }
  else if(aEventType == SEQ_MSG_TYPE_RAB)
  {
    printf("[dec_h264] Callback: READY AFTER BOOT\n");
  }
  else if(aEventType == SEQ_MSG_TYPE_OTHERERROR)
  {
    printf("[dec_h264] Callback: OTHERERROR\n");
    async_error       = true;
    dec_abort_reading = true;
  }
}
