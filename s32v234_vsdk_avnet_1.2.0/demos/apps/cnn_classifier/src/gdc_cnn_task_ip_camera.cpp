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
#include "gdc_cnn_task_ip_camera.hpp"
#include "communications.hpp"
#include "iniparser.h"
#include "global_errors.h"
#include <stdint.h>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcCnn_IpCameraTask::GdcCnn_IpCameraTask()
    : mReceiveMessageQueueId(0), mResizeTaskMessageQueueId(0), mIsTaskClosing(false), mIsConfigured(false),
      //cv::VideoCapture mVideoCapture;
      //std::string mIpCameraAddress;
      mSceneCounter(0)
{
}

/***************************************************************************/
/***************************************************************************/
GdcCnn_IpCameraTask::~GdcCnn_IpCameraTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_IpCameraTask::Configure(const int32_t cReceiveMessageQueueId, const int32_t cResizeTaskMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId    = cReceiveMessageQueueId;
    mResizeTaskMessageQueueId = cResizeTaskMessageQueueId;

    mIsConfigured = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_IpCameraTask::TaskOpen()
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
LIB_RESULT GdcCnn_IpCameraTask::TaskService()
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

          libResult = GdcCnn_IpCameraTask::HandleImageRequest();
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
                                                 mResizeTaskMessageQueueId, pMessage->mPriority);
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
LIB_RESULT GdcCnn_IpCameraTask::TaskClose()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

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
LIB_RESULT GdcCnn_IpCameraTask::HandleImageRequest()
{
  LIB_RESULT libResult  = LIB_FAILURE;
  bool       boolResult = false;

  uint64_t ticks[gcApexCvEndTick] = {0};

  ticks[0] = FSL_Ticks();

  cv::Mat lSceneMat;

  boolResult = mVideoCapture.read(lSceneMat);
  SHOW_ERROR(boolResult == true);

  if(boolResult == true)
  {
    ticks[1] = FSL_Ticks();

    libResult = GdcCnn_Helper::SendImageReply(mResizeTaskMessageQueueId, lSceneMat, mSceneCounter, "", ticks);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    mSceneCounter++;
  }

  return libResult;
}
