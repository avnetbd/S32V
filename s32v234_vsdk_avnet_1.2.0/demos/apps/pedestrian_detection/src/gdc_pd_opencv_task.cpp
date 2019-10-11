/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2015 Freescale Semiconductor;
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
#include "gdc_pd_opencv_task.hpp"
#include "communications.hpp"
#include "global_errors.h"
#include "iniparser.h"
#include <algorithm>
#include <stdlib.h>
#include <stdint.h>
#include <new>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcPd_OpenCvTask::GdcPd_OpenCvTask()
    : mReceiveMessageQueueId(0),
      //cv::HOGDescriptor mHogDescriptor;
      mIsTaskClosing(false), mIsConfigured(false)
{
}

/***************************************************************************/
/***************************************************************************/
GdcPd_OpenCvTask::~GdcPd_OpenCvTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_OpenCvTask::Configure(const int32_t cReceiveMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId = cReceiveMessageQueueId;

    mIsConfigured = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_OpenCvTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  int intResult = -1;

  if(mIsConfigured == true)
  {
    cv::namedWindow("opencv", CV_WINDOW_AUTOSIZE);
    mHogDescriptor.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_OpenCvTask::TaskService()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  MessageBase* pMessage           = 0;
  int32_t      messageSizeInBytes = 0;

  while(mIsTaskClosing == false)
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
        case MESSAGE_ID_GDC_PD_IMAGE_REPLY:
        {
          GdcPd_ImageReply* pReply = (GdcPd_ImageReply*)pMessage;

          libResult = GdcPd_OpenCvTask::HandleImageReply(*pReply);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          mIsTaskClosing = !(libResult == LIB_SUCCESS);

          pReply->~GdcPd_ImageReply();
          break;
        }
        case MESSAGE_ID_GDC_PD_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          mIsTaskClosing = true;

          break;
        }
        case MESSAGE_ID_GDC_PD_RESET_REQUEST:
        {
          // Do nothing
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
LIB_RESULT GdcPd_OpenCvTask::TaskClose()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

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
        case MESSAGE_ID_GDC_PD_IMAGE_REPLY:
        {
          GdcPd_ImageReply* pReply = (GdcPd_ImageReply*)pMessage;

          libResult = GdcPd_Helper::FreeImageReply(pReply);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          mIsTaskClosing = !(libResult == LIB_SUCCESS);

          pReply->~GdcPd_ImageReply();
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
LIB_RESULT GdcPd_OpenCvTask::HandleImageReply(GdcPd_ImageReply& rImageReply)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(rImageReply.mhSceneOal != 0)
  {
    cv::Mat img(gcSceneHeightInPixels, gcSceneWidthInPixels, CV_8UC1, rImageReply.mhSceneOal);

    std::vector< cv::Rect > found, found_filtered;
    mHogDescriptor.detectMultiScale(img, found, 0, cv::Size(8, 8), cv::Size(32, 32), 1.05, 2);
    size_t i, j;
    for(i = 0; i < found.size(); i++)
    {
      cv::Rect r = found[i];
      for(j = 0; j < found.size(); j++)
        if(j != i && (r & found[j]) == r)
          break;
      if(j == found.size())
        found_filtered.push_back(r);
    }

    for(i = 0; i < found_filtered.size(); i++)
    {
      cv::Rect r = found_filtered[i];
      r.x += cvRound(r.width * 0.1);
      r.width = cvRound(r.width * 0.8);
      r.y += cvRound(r.height * 0.07);
      r.height = cvRound(r.height * 0.8);
      rectangle(img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 3);
    }

    cv::imshow("opencv", img);
    cv::waitKey(1);

    libResult = LIB_SUCCESS;
  }

  return libResult;
}
