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

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>

#include "frame_output_v234fb.h"
#define CHNL_CNT io::IO_DATA_CH3

#include "sdi.hpp" /* grabber */
#include "jpeg_4stream_c.h"

#include "isp_jpegdec.h"

#include "dec_jpeg_hl.h" /* this app */

#define JPEGD_DRV "/dev/jpegdcd"
#define DDR_BUFFER_CNT 3 ///< number of DDR buffers per ISP stream

static io::FrameOutputV234Fb lDcuOutput;

static int dec_jpeg_prepare_4channels(sdi_grabber* lpGrabber, int Width, int Height);
static int dec_jpeg_app_body_4channels(sdi_grabber* lpGrabber);

volatile bool dec_abort_reading = false;
volatile bool async_error       = false; /* Not used in this implementation */

int dec_jpeg_prepare_ios(sdi_grabber* lpGrabber, int Width, int Height)
{
  return dec_jpeg_prepare_4channels(lpGrabber, Width, Height);
}

int dec_jpeg_process_frame(sdi_grabber* lpGrabber)
{
  return dec_jpeg_app_body_4channels(lpGrabber);
}

#define FRM_POP_ATOMIC_TIMEOUT 2U // Miliseconds
static int dec_jpeg_app_body_4channels(sdi_grabber* lpGrabber)
{
  // fetched frame buffer storage
  static SDI_Frame lFrame[4];
  uint32_t         i           = 0U;
  uint32_t         timeout_ctr = 500U; // 500 * 2 (the FramePop timeout) = 1000 ms in total

  do
  { // Using loop of short waits instead of one long wait to be able to abort reading quickly
    static const uint32_t channel_idx[] = {FDMA_IX_UYVY_0, FDMA_IX_UYVY_1, FDMA_IX_UYVY_2, FDMA_IX_UYVY_3};

    if(true == dec_abort_reading)
      return -1; // Reading was aborted
    lFrame[i] = lpGrabber->FramePop(channel_idx[i], FRM_POP_ATOMIC_TIMEOUT);
    if(!lFrame[i].mUMat.empty())
      i++; // One frame was readed
    timeout_ctr--;
    if(0U == timeout_ctr)
    {
      printf("[dec_jpeg_hl] Frame pop FAILED\n");
      return -1; // Waiting for too long
    }
  } while(i < 4U);

  lDcuOutput.PutFrame(lFrame[0].mUMat);

  /*  For each frame quadrant */
  for(uint32_t i = 0U; i < 4U; i++)
  {
    if(lpGrabber->FramePush(lFrame[i]) != LIB_SUCCESS)
    {
      printf("[dec_jpeg_hl] Frame push FAILED\n");
      return -1;
    }
  }

  return 0;
}

static int dec_jpeg_prepare_4channels(sdi_grabber* lpGrabber, int Width, int Height)
{
  /*  Instance-specific data */
  static sdi_FdmaIO*         lpFdma;
  static SDI_DdrBufferArr    lYuvColorBuffArr;
  static SDI_ImageDescriptor lFrmDesc;
  static bool                bColBufAllocated = false;
  static SDI_DdrBufferArr    lYuvColorRoi;

  //*** Init DCU Output ***
  lDcuOutput.Init(Width, Height, io::IO_DATA_DEPTH_08, CHNL_CNT, DCU_BPP_YCbCr422);

  // *** prepare IOs ***
  lpFdma = (sdi_FdmaIO*)lpGrabber->IoGet(SEQ_OTHRIX_FDMA);
  if(NULL == lpFdma)
  {
    printf("[dec_jpeg_hl] lpGrabber->IoGet(FDMA) failed\n");
    return -1;
  }

  sdi_JpegDecIO* lpJpegDec = (sdi_JpegDecIO*)lpGrabber->IoGet(SEQ_OTHRIX_JPEGDEC);
  if(NULL == lpJpegDec)
  {
    printf("[dec_jpeg_hl] lpGrabber->IoGet(JPEGDEC) failed\n");
    return -1;
  }
  SDI_JpegDecDescriptor lJpegDesc = lpJpegDec->ConfigGet();
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

  if(lpJpegDec->ConfigSet(lJpegDesc) != LIB_SUCCESS)
  {
    printf("Failed to configure decoding.\n");
  } // if config failed

  printf("[dec_jpeg_hl] Setting resolution %dx%d\n", Width, Height);

  // *** RGB full color buffer array ***
  lFrmDesc = SDI_ImageDescriptor(Width, Height, YUV422Stream_YUYV);

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

  // *** get quadrant 0 ***
  lYuvColorRoi = lYuvColorBuffArr.Roi(0, 0, Width / 2, Height / 2);

  lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_0);
  lpFdma->DdrBuffersSet(lYuvColorRoi);

  // *** get quadrant 1 ***
  lYuvColorRoi = lYuvColorBuffArr.Roi(Width / 2, 0, Width / 2, Height / 2);

  lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_1);
  lpFdma->DdrBuffersSet(lYuvColorRoi);

  // *** get quadrant 2 ***
  lYuvColorRoi = lYuvColorBuffArr.Roi(0, Height / 2, Width / 2, Height / 2);

  lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_2);
  lpFdma->DdrBuffersSet(lYuvColorRoi);

  // *** get quadrant 3 ***
  lYuvColorRoi = lYuvColorBuffArr.Roi(Width / 2, Height / 2, Width / 2, Height / 2);

  lYuvColorRoi.ChannelIdxSet(FDMA_IX_UYVY_3);
  lpFdma->DdrBuffersSet(lYuvColorRoi);

  return 0;
}