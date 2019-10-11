/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2015 Freescale Semiconductor;
* Copyright 2017 NXP
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

/***************************************************************************/
/* Include files. */
#include "gdc_cnn_configuration.hpp"
#include "gdc_cnn_messages.hpp"
#include "gdc_cnn_task_image_sensor.hpp"
#include "isp_cam_sony.h"
#include "mipi_simple_c.h"
#include "communications.hpp"
#include "iniparser.h"
#include "global_errors.h"
#include <stdint.h>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcCnn_ImageSensorTask::GdcCnn_ImageSensorTask()
    : mReceiveMessageQueueId(0), mFeatureDetectorTaskMessageQueueId(0), mIsTaskClosing(false), mIsConfigured(false),
      //mSdiGrabber
      //uint32_t mCropRoi[4];                          // Region of Interest: left, top, width, height
      mCroppedAndConvertedImageSpanInBytes(0), mCroppedAndConvertedImageBufferSizeInBytes(0),
      mCroppedAndConvertedImageOffsetInBytes(0), mSceneCounter(0),
      mImageSensorWidthInPixels(gcImageSensorWidthInPixels), mImageSensorHeightInPixels(gcImageSensorHeightInPixels)
{
  memset(mCropRoi, 0, sizeof(mCropRoi));
}

/***************************************************************************/
/***************************************************************************/
GdcCnn_ImageSensorTask::~GdcCnn_ImageSensorTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_ImageSensorTask::Configure(const int32_t cReceiveMessageQueueId,
                                             const int32_t cFeatureDetectorTaskMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId             = cReceiveMessageQueueId;
    mFeatureDetectorTaskMessageQueueId = cFeatureDetectorTaskMessageQueueId;

    mIsConfigured = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_ImageSensorTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  uint32_t imageSensorNumberOfBuffers       = gcImageSensorNumberOfBuffers;
  uint32_t isImageSensorFlippedVertically   = gcIsImageSensorFlippedVertically;
  uint32_t isImageSensorFlippedHorizontally = gcIsImageSensorFlippedHorizontally;

  //
  // Get info from .ini file
  //
  dictionary* pDictionary = 0;

  pDictionary = iniparser_load(gcIniFileName);

  if(pDictionary != 0)
  {
    isImageSensorFlippedVertically   = iniparser_getboolean(pDictionary, "ImageSensorTask:WeFlipImageSensorVertically",
                                                          gcIsImageSensorFlippedVertically);
    isImageSensorFlippedHorizontally = iniparser_getboolean(
        pDictionary, "ImageSensorTask:WeFlipImageSensorHorizontally", gcIsImageSensorFlippedHorizontally);

    iniparser_freedict(pDictionary);
    pDictionary = 0;
  }

  libResult = sdi::Initialize(0);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  mIsTaskClosing = !(libResult == LIB_SUCCESS);

   if(mIsTaskClosing == false) // Keep going
   {
      libResult = mSdiGrabber.ProcessSet(gpGraph_mipi_simple, &gGraphMetadata_mipi_simple, kmem_mipi_simple_srec, sequencer_mipi_simple_srec); SHOW_ERROR(libResult == LIB_SUCCESS);

    sdi_FdmaIO* lpSdiFdmaIo = (sdi_FdmaIO*)mSdiGrabber.IoGet(SEQ_OTHRIX_FDMA);

    SDI_ImageDescriptor lSdiImgDesc =
        SDI_ImageDescriptor(mImageSensorWidthInPixels, mImageSensorHeightInPixels, RGB888);
    libResult = lpSdiFdmaIo->DdrBufferDescSet(0, lSdiImgDesc);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = lpSdiFdmaIo->DdrBuffersAlloc(0, imageSensorNumberOfBuffers);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mSdiGrabber.PreStart();
    SHOW_ERROR(libResult == LIB_SUCCESS);

    SONY_Geometry_t lSonyGeometry;
    lSonyGeometry.mCsiIdx = CSI_IDX_0;
    SONY_GeometryGet(lSonyGeometry);
    lSonyGeometry.mVerFlip = isImageSensorFlippedVertically;
    lSonyGeometry.mHorFlip = isImageSensorFlippedHorizontally;
    SONY_GeometrySet(lSonyGeometry);

    libResult = mSdiGrabber.Start();
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }

  mIsTaskClosing = !(libResult == LIB_SUCCESS);

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_ImageSensorTask::TaskService()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  MessageBase* pMessage           = 0;
  int32_t      messageSizeInBytes = 0;

  while(mIsTaskClosing == false) // Keep going as long as there are more images in the file.
  {
    // This task uses a message handler pattern and will only run
    // if a message is sent to its queue.
    // It spends the rest of its time suspended on the message queue semaphore.
    libResult = MsgMgr_MessageReceive(mReceiveMessageQueueId, pMessage, messageSizeInBytes,
                                      0xFFFFFFFF); // 0xFFFFFFFF wait forever
    if(libResult == LIB_SUCCESS && pMessage != 0)
    {
      switch(pMessage->mMessageId)
      {
        case MESSAGE_ID_GDC_CNN_IMAGE_REQUEST:
        {
          GdcCnn_ImageRequest* pRequest = (GdcCnn_ImageRequest*)pMessage;

          libResult = GdcCnn_ImageSensorTask::HandleImageRequest();
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          mIsTaskClosing = !(libResult == LIB_SUCCESS);

          pRequest->~GdcCnn_ImageRequest();
          break;
        }
        case MESSAGE_ID_GDC_CNN_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult = GdcCnn_Helper::SendRequest(MESSAGE_ID_GDC_CNN_EXIT_REQUEST, mReceiveMessageQueueId,
                                                 mFeatureDetectorTaskMessageQueueId, pMessage->mPriority);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          mIsTaskClosing = true;

          break;
        }
        default:
        {
          SHOW_ERROR(false); // This should NEVER happen!
          break;
        }
      }
      free(pMessage);
      pMessage = 0;
    }

    if(gcIsRunningWithThreads == false)
    {
      break;
    }
  }

  if(libResult == LIB_SUCCESS && mIsTaskClosing == true)
  {
    libResult = LIB_APP_IS_CLOSING;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_ImageSensorTask::TaskClose()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  //*** Stop ISP processing ***
  mSdiGrabber.Stop();

  // clean up grabber resources
  mSdiGrabber.Release();

  sdi::Close(0);

  //
  // Free messages still in the tasks queue
  //
  MessageBase* pMessage           = 0;
  int32_t      messageSizeInBytes = 0;

  while(1)
  {
    libResult = MsgMgr_MessageReceive(mReceiveMessageQueueId, pMessage, messageSizeInBytes, 0);
    if(libResult == LIB_SUCCESS && pMessage != 0)
    {
      switch(pMessage->mMessageId)
      {
        case MESSAGE_ID_GDC_CNN_IMAGE_REQUEST:
        {
          GdcCnn_ImageRequest* pRequest = (GdcCnn_ImageRequest*)pMessage;

          // Nothing to free

          pRequest->~GdcCnn_ImageRequest();
          break;
        }
        case MESSAGE_ID_GDC_CNN_EXIT_REQUEST:
        {
          GdcCnn_ExitRequest* pRequest = (GdcCnn_ExitRequest*)pMessage;

          // Nothing to free

          pRequest->~GdcCnn_ExitRequest();
          break;
        }
        default:
        {
          SHOW_ERROR(false); // This should NEVER happen!
          break;
        }
      }
      free(pMessage);
      pMessage = 0;
    }
    else
    {
      break;
    }
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_ImageSensorTask::HandleImageRequest()
{
  LIB_RESULT libResult = LIB_FAILURE;

  uint64_t ticks[gcApexCvEndTick] = {0};

  ticks[0] = FSL_Ticks();

  SDI_Frame lSdiFrame;

  lSdiFrame = mSdiGrabber.FramePop();

  if(lSdiFrame.mUMat.empty() == false)
  {
    cv::Mat lSceneMat(mImageSensorHeightInPixels, mImageSensorWidthInPixels,
                      CV_8UC3); // SASHBY: Magic Number bad!!!

    { // Need this scope to allow vsdk::Mat to go out of scope prior to sending message
      vsdk::Mat lSdiFrame_mat = lSdiFrame.mUMat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);

      memcpy(lSceneMat.data, (void*)(lSdiFrame_mat.data),
             mImageSensorWidthInPixels * mImageSensorHeightInPixels * 3); // SASHBY: Magic Number bad!!!
    }

    ticks[1] = FSL_Ticks();

    libResult = GdcCnn_Helper::SendImageReply(mFeatureDetectorTaskMessageQueueId, lSceneMat, mSceneCounter, "", ticks);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    mSceneCounter++;

    libResult = mSdiGrabber.FramePush(lSdiFrame);
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }

  return libResult;
}
