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
#include "gdc_pd_configuration.hpp"
#include "gdc_pd_messages.hpp"
#include "gdc_pd_image_sensor_task.hpp"
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
GdcPd_ImageSensorTask::GdcPd_ImageSensorTask()
    : mReceiveMessageQueueId(0), mFeatureDetectorTaskMessageQueueId(0), mIsTaskClosing(false), mIsConfigured(false),
      //mSdiGrabber
      //vsdk::Rect mRect
      mSceneCounter(0), mImageSensorWidthInPixels(0), mImageSensorHeightInPixels(0), mImageSensorBitsPerPixel(0),
      mSceneWidthInPixels(0), mSceneHeightInPixels(0), mSceneBitsPerPixel(0), mScenePaddingInBytes(0)
{
}

/***************************************************************************/
/***************************************************************************/
GdcPd_ImageSensorTask::~GdcPd_ImageSensorTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_ImageSensorTask::Configure(const int32_t  cReceiveMessageQueueId,
                                            const int32_t  cFeatureDetectorTaskMessageQueueId,
                                            const int32_t  cSceneWidthInPixels,
                                            const int32_t  cSceneHeightInPixels,
                                            const uint32_t cSceneBitsPerPixel,
                                            const uint32_t cScenePaddingInBytes)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId             = cReceiveMessageQueueId;
    mFeatureDetectorTaskMessageQueueId = cFeatureDetectorTaskMessageQueueId;
    mSceneWidthInPixels                = cSceneWidthInPixels;
    mSceneHeightInPixels               = cSceneHeightInPixels;
    mSceneBitsPerPixel                 = cSceneBitsPerPixel;
    mScenePaddingInBytes               = cScenePaddingInBytes;

    mIsConfigured = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_ImageSensorTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  //set default
  mImageSensorWidthInPixels  = gcImageSensorWidthInPixels;
  mImageSensorHeightInPixels = gcImageSensorHeightInPixels;

  bool lWeFlipImageSensorVertically   = gcWeFlipImageSensorVertically;
  bool lWeFlipImageSensorHorizontally = gcWeFlipImageSensorHorizontally;

  //
  // Get info from .ini file
  //
  dictionary* pDictionary = 0;

  pDictionary = iniparser_load(gcIniFileName);

  if(pDictionary != 0)
  {
    mImageSensorWidthInPixels =
        iniparser_getint(pDictionary, "ImageSensor:ImageSensorWidthInPixels", gcImageSensorWidthInPixels);
    mImageSensorHeightInPixels =
        iniparser_getint(pDictionary, "ImageSensor:ImageSensorHeightInPixels", gcImageSensorHeightInPixels);
    mImageSensorBitsPerPixel =
        iniparser_getint(pDictionary, "ImageSensor:ImageSensorBitsPerPixel", gcImageSensorBitsPerPixel);
    lWeFlipImageSensorVertically   = (iniparser_getboolean(pDictionary, "ImageSensor:WeFlipImageSensorVertically",
                                                         gcWeFlipImageSensorVertically) == 1);
    lWeFlipImageSensorHorizontally = (iniparser_getboolean(pDictionary, "ImageSensor:WeFlipImageSensorHorizontally",
                                                           gcWeFlipImageSensorHorizontally) == 1);

    iniparser_freedict(pDictionary);
    pDictionary = 0;
  }

  libResult = sdi::Initialize(0);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  mIsTaskClosing = !(libResult == LIB_SUCCESS);

  if(mIsTaskClosing == false) // Keep going
  {
    libResult = mSdiGrabber.ProcessSet(gpGraph_mipi_simple, &gGraphMetadata_mipi_simple, kmem_mipi_simple_srec, sequencer_mipi_simple_srec); SHOW_ERROR(libResult == LIB_SUCCESS);

    sdi_FdmaIO* lpSdiFdmaIo;// = (sdi_FdmaIO*)mSdiGrabber.IoGet(SEQ_OTHRIX_FDMA);

    SDI_ImageDescriptor lSdiImgDesc = SDI_ImageDescriptor(mImageSensorWidthInPixels, mImageSensorHeightInPixels,
                                                          RGB888); // SASHBY: hard coded value bad!!!

    libResult = lpSdiFdmaIo->DdrBufferDescSet(0, lSdiImgDesc);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = lpSdiFdmaIo->DdrBuffersAlloc(0, gcImageSensorNumberOfBuffers);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mSdiGrabber.PreStart();
    SHOW_ERROR(libResult == LIB_SUCCESS);

    SONY_Geometry_t lSonyGeometry;
    lSonyGeometry.mCsiIdx = CSI_IDX_0;
    SONY_GeometryGet(lSonyGeometry);
    lSonyGeometry.mVerFlip = lWeFlipImageSensorVertically;
    lSonyGeometry.mHorFlip = lWeFlipImageSensorHorizontally;
    SONY_GeometrySet(lSonyGeometry);

    libResult = mSdiGrabber.Start();
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = GdcPd_Helper::CalculateSensorRoi(mImageSensorWidthInPixels, mImageSensorHeightInPixels,
                                                 mSceneWidthInPixels, mSceneHeightInPixels, mRect);
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }

  mIsTaskClosing = !(libResult == LIB_SUCCESS);

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_ImageSensorTask::TaskService()
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
        case MESSAGE_ID_GDC_PD_IMAGE_REQUEST:
        {
          GdcPd_ImageRequest* pRequest = (GdcPd_ImageRequest*)pMessage;

          libResult = GdcPd_ImageSensorTask::HandleImageRequest();
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          mIsTaskClosing = !(libResult == LIB_SUCCESS);

          pRequest->~GdcPd_ImageRequest();
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
LIB_RESULT GdcPd_ImageSensorTask::TaskClose()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  //*** Stop ISP processing ***
  mSdiGrabber.Stop();

  // clean up grabber resources
  mSdiGrabber.Release();

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_ImageSensorTask::HandleImageRequest()
{
  LIB_RESULT libResult = LIB_FAILURE;
  int        intResult = 0;

  uint64_t ticks[2] = {0};

  ticks[0] = FSL_Ticks();

  SDI_Frame lSdiFrame;

  lSdiFrame = mSdiGrabber.FramePop();

  if(lSdiFrame.mUMat.empty() == false)
  {
    vsdk::UMat lSensorRgb24_umat(lSdiFrame.mUMat, mRect);

    vsdk::UMat lSceneRgb24_umat(mRect.height, mRect.width, VSDK_CV_8UC3);

    vsdk::UMat lScene_umat(mRect.height, mRect.width, VSDK_CV_8UC1);

    libResult =
        GdcPd_Helper::CropAndConvertRgb24ToY(lSensorRgb24_umat, lSceneRgb24_umat, lScene_umat, gcR2Y, gcG2Y, gcB2Y);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    if(libResult == LIB_SUCCESS)
    {
      ticks[1] = FSL_Ticks();

      libResult =
          GdcPd_Helper::SendImageReply(mFeatureDetectorTaskMessageQueueId, lSceneRgb24_umat,
                                       lSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
                                       lScene_umat,
                                       lScene_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
                                       mSceneCounter, ticks);
      mSceneCounter++;
    }

    libResult = mSdiGrabber.FramePush(lSdiFrame);
    SHOW_ERROR(libResult == 0);
  }

  if(libResult != LIB_SUCCESS || intResult != 0)
  {
    libResult = LIB_FAILURE;
  }

  return libResult;
}
