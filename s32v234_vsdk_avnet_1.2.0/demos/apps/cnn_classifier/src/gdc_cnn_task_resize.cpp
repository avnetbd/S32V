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
#include "gdc_cnn_task_resize.hpp"
#include "communications.hpp"
#include "global_errors.h"
#include "iniparser.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <time.h>
#include <stdint.h>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcCnn_ResizeTask::GdcCnn_ResizeTask()
    : mReceiveMessageQueueId(0), mClassifierTaskMessageQueueId(0), mIsTaskClosing(false), mIsConfigured(false)
{
  mChannelMeans[0] = gcBgrChannelMeans[0];
  mChannelMeans[1] = gcBgrChannelMeans[1];
  mChannelMeans[2] = gcBgrChannelMeans[2];
}

/***************************************************************************/
/***************************************************************************/
GdcCnn_ResizeTask::~GdcCnn_ResizeTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_ResizeTask::Configure(const int32_t cReceiveMessageQueueId,
                                        const int32_t cClassifierTaskMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId        = cReceiveMessageQueueId;
    mClassifierTaskMessageQueueId = cClassifierTaskMessageQueueId;

    mIsConfigured = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_ResizeTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  if(mIsConfigured == true)
  {
    dictionary* pDictionary = 0;
    ;

    pDictionary = iniparser_load(gcIniFileName);

    if(pDictionary != 0)
    {
      mChannelMeans[0] = iniparser_getint(pDictionary, "ResizeTask:ChannelMeans_B", gcBgrChannelMeans[0]);
      mChannelMeans[1] = iniparser_getint(pDictionary, "ResizeTask:ChannelMeans_G", gcBgrChannelMeans[1]);
      mChannelMeans[2] = iniparser_getint(pDictionary, "ResizeTask:ChannelMeans_R", gcBgrChannelMeans[2]);

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }
  }
  else
  {
    mIsTaskClosing = true;
  }

  GDC_TRACE;

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_ResizeTask::TaskService()
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
        case MESSAGE_ID_GDC_CNN_IMAGE_REPLY:
        {
          GdcCnn_ImageReply* pReply = (GdcCnn_ImageReply*)pMessage;

          libResult = GdcCnn_ResizeTask::HandleImageReply(*pReply);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          if(mIsTaskClosing == false)
          {
            mIsTaskClosing = !(libResult == LIB_SUCCESS);
          }

          pReply->~GdcCnn_ImageReply();
          break;
        }
        case MESSAGE_ID_GDC_CNN_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult = GdcCnn_Helper::SendRequest(MESSAGE_ID_GDC_CNN_EXIT_REQUEST, mReceiveMessageQueueId,
                                                 mClassifierTaskMessageQueueId, pMessage->mPriority);
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
LIB_RESULT GdcCnn_ResizeTask::TaskClose()
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
        case MESSAGE_ID_GDC_CNN_IMAGE_REPLY:
        {
          GdcCnn_ImageReply* pReply = (GdcCnn_ImageReply*)pMessage;

          // Nothing to free

          pReply->~GdcCnn_ImageReply();
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
LIB_RESULT GdcCnn_ResizeTask::HandleImageReply(GdcCnn_ImageReply& rReply)
{
  LIB_RESULT libResult = LIB_FAILURE;

  rReply.mTicks[gcResizeTick] = FSL_Ticks();

  if(rReply.mSceneMat.empty() != true)
  {
    cv::Mat lResizedSceneMat(gcSceneHeightInPixels, gcSceneWidthInPixels,
                             CV_8UC3); // SASHBY: Magic Number bad!!!

    cv::Size lResizedSceneSize(gcSceneWidthInPixels, gcSceneHeightInPixels);

    cv::resize(rReply.mSceneMat, lResizedSceneMat, lResizedSceneSize);

    rReply.mTicks[gcResizeTick + 1] = FSL_Ticks();

    if(lResizedSceneMat.empty() != true)
    {
      //
      // Split and pad RGB channels
      //
      // +---------------------+
      // |   +---+ +---+ +---+ |
      // |   | R | | G | | B | |
      // |   +---+ +---+ +---+ |
      // +---------------------+

      vsdk::UMat lPaddedScene_umat(gcPaddedScene_HeightInPixels, gcPaddedScene_SpanInBytes, VSDK_CV_8SC1);

      if(lPaddedScene_umat.empty() == false)
      {
        { // Need this scope to allow vsdk::Mat to go out of scope prior to sending message
          vsdk::Mat lpPaddedScene_mat = lPaddedScene_umat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);

          void* lpPaddedScene_mat_data = (void*)(((uintptr_t)lpPaddedScene_mat.data));

          static_assert((sizeof(mChannelMeans) / sizeof(mChannelMeans[0])) == gcSceneNumberOfChannels,
                        "sizeof(mChannelMeans) != gcSceneNumberOfChannels");

          memset(lpPaddedScene_mat_data, 0, gcPaddedScene_BufferSizeInBytes);

          int8_t* pLine = (int8_t*)(((uintptr_t)lpPaddedScene_mat_data) + gcPaddedScene_OffsetInBytes_B);

          // for each line in height
          for(uint32_t h = 0; h < gcSceneHeightInPixels; ++h)
          {
            int8_t* pPixel = pLine;

            // for each channel, BGR
            for(uint32_t c = 0; c < gcSceneNumberOfChannels; ++c)
            {
              // for each column in width
              for(uint32_t w = 0; w < gcSceneWidthInPixels; ++w)
              {
                const uint32_t cPixelIndex = ((((h * gcSceneWidthInPixels) + w) * gcSceneNumberOfChannels) + c);
                SHOW_ERROR(cPixelIndex < gcPaddedScene_BufferSizeInBytes);
                int32_t lPixelValue = lResizedSceneMat.data[cPixelIndex] - mChannelMeans[c];
                if(lPixelValue > 127)
                {
                  lPixelValue = 127;
                } // SASHBY: Magic number bad!!!
                if(lPixelValue < -128)
                {
                  lPixelValue = -128;
                } // SASHBY: Magic number bad!!!
                *pPixel = lPixelValue;
                ++pPixel;
              }

              pPixel += gcPadding_RightInBytes;
            }

            pLine += gcPaddedScene_SpanInBytes;
          }
        }

        rReply.mTicks[gcResizeTick + 2] = FSL_Ticks();

        libResult = GdcCnn_Helper::SendResizeData(
            mClassifierTaskMessageQueueId, rReply.mSceneMat, rReply.mSceneCounter, rReply.mSceneName, lPaddedScene_umat,
            (void*)(((uintptr_t)lPaddedScene_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data)), // Debug only
            rReply.mTicks);
      }
    }
  }

  return libResult;
}
