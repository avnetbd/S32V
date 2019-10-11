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
#include "gdc_pd_task_ip_camera.hpp"
#include "communications.hpp"
#include "iniparser.h"
#include "global_errors.h"
#include <stdint.h>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcPd_IpCameraTask::GdcPd_IpCameraTask()
    : mReceiveMessageQueueId(0), mAggcfLuvMessageQueueId(0), mIsTaskClosing(false), mIsConfigured(false),
      //cv::VideoCapture mVideoCapture;
      //std::string mIpCameraAddress;
      mSceneCounter(0)
{
}

/***************************************************************************/
/***************************************************************************/
GdcPd_IpCameraTask::~GdcPd_IpCameraTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_IpCameraTask::Configure(const int32_t cReceiveMessageQueueId, const int32_t cAggcfLuvMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId  = cReceiveMessageQueueId;
    mAggcfLuvMessageQueueId = cAggcfLuvMessageQueueId;

    mIsConfigured = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_IpCameraTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult  = LIB_FAILURE;
  bool       boolResult = false;

  //
  // Get info from .ini file
  //
  dictionary* pDictionary = 0;

  pDictionary = iniparser_load(gcIniFileName);

  if(pDictionary != 0)
  {
    char* pString = 0;

    pString = iniparser_getstring(pDictionary, "IpCameraTask:IpCameraAddress", 0);

    if(pString != 0)
    {
      mIpCameraAddress = pString;
    }
    else
    {
      //
      // Failed to obtain Ip Camera Address from .ini; reverting to default.
      //
      mIpCameraAddress = gcIpCameraAddress;
      SHOW_ERROR(false);
    }

    iniparser_freedict(pDictionary);
    pDictionary = 0;

    libResult = LIB_SUCCESS;
  }

  boolResult = mVideoCapture.open(mIpCameraAddress);
  SHOW_ERROR(boolResult == true);

  if(boolResult == false)
  {
    libResult = LIB_FAILURE;
  }

  mIsTaskClosing = !(libResult == LIB_SUCCESS);

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_IpCameraTask::TaskService()
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

          libResult = GdcPd_IpCameraTask::HandleImageRequest();
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          mIsTaskClosing = !(libResult == LIB_SUCCESS);

          pRequest->~GdcPd_ImageRequest();
          break;
        }
        case MESSAGE_ID_GDC_PD_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult = GdcPd_Helper::SendRequest(MESSAGE_ID_GDC_PD_EXIT_REQUEST, mReceiveMessageQueueId,
                                                mAggcfLuvMessageQueueId, pMessage->mPriority);
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
LIB_RESULT GdcPd_IpCameraTask::TaskClose()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  mVideoCapture.release();

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
        case MESSAGE_ID_GDC_PD_IMAGE_REQUEST:
        {
          GdcPd_ImageRequest* pRequest = (GdcPd_ImageRequest*)pMessage;

          // Nothing to free

          pRequest->~GdcPd_ImageRequest();
          break;
        }
        case MESSAGE_ID_GDC_PD_EXIT_REQUEST:
        {
          GdcPd_ExitRequest* pRequest = (GdcPd_ExitRequest*)pMessage;

          // Nothing to free

          pRequest->~GdcPd_ExitRequest();
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
LIB_RESULT GdcPd_IpCameraTask::HandleImageRequest()
{
  LIB_RESULT libResult  = LIB_FAILURE;
  bool       boolResult = false;

  uint64_t ticks[2] = {0};

  ticks[0] = FSL_Ticks();

  cv::Mat lIpCameraRgb24_mat;

  boolResult = mVideoCapture.read(lIpCameraRgb24_mat);
  SHOW_ERROR(boolResult == true);

  if(boolResult == true)
  {
    vsdk::UMat lIpCameraRgb24_umat = lIpCameraRgb24_mat.getUMat(cv::ACCESS_READ);

    vsdk::UMat lSceneRgb24_umat(lIpCameraRgb24_mat.rows, lIpCameraRgb24_mat.cols, VSDK_CV_8UC3);

    vsdk::UMat lSceneRgb32_umat(lIpCameraRgb24_mat.rows, lIpCameraRgb24_mat.cols, VSDK_CV_32SC1);

    libResult = GdcPd_Helper::CropAndConvertBgr24ToRgb32(lIpCameraRgb24_umat, lSceneRgb24_umat, lSceneRgb32_umat);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    ticks[1] = FSL_Ticks();

    libResult =
        GdcPd_Helper::SendImageReply(mAggcfLuvMessageQueueId, lSceneRgb24_umat,
                                     lSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
                                     lSceneRgb32_umat,
                                     lSceneRgb32_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
                                     mSceneCounter, ticks);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    mSceneCounter++;
  }

  return libResult;
}
