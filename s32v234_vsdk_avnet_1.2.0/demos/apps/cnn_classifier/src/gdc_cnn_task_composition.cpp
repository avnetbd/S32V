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
#include "gdc_cnn_task_composition.hpp"
#include "communications.hpp"
#include "iniparser.h"
#include "global_errors.h"
#include <stdint.h>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcCnn_CompositionTask::GdcCnn_CompositionTask()
    : mReceiveMessageQueueId(0), mDisplayMessageQueueId(0), mWeCompareWithReference(false), mIsTaskClosing(false),
      mIsConfigured(false)
//GdcCnn_Composition mComposition;
{
}

/***************************************************************************/
/***************************************************************************/
GdcCnn_CompositionTask::~GdcCnn_CompositionTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_CompositionTask::Configure(const int32_t cReceiveMessageQueueId,
                                             const int32_t cDisplayMessageQueueId,
                                             const bool    cWeCompareWithReference)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId = cReceiveMessageQueueId;
    mDisplayMessageQueueId = cDisplayMessageQueueId;

    mWeCompareWithReference = cWeCompareWithReference;

    mIsConfigured = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_CompositionTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  uint32_t lcDisplayScaleNumerator  = gcDisplayScaleNumerator;
  uint32_t lDisplayScaleDenominator = gcDisplayScaleDenominator;

  dictionary* pDictionary = 0;

  pDictionary = iniparser_load(gcIniFileName);

  if(pDictionary != 0)
  {
    lcDisplayScaleNumerator =
        iniparser_getint(pDictionary, "DisplayTask:DisplayScaleNumerator", gcDisplayScaleNumerator);
    lDisplayScaleDenominator =
        iniparser_getint(pDictionary, "DisplayTask:DisplayScaleDenominator", gcDisplayScaleDenominator);

    iniparser_freedict(pDictionary);
    pDictionary = 0;
  }

  SHOW_ERROR((lcDisplayScaleNumerator == 1 && lDisplayScaleDenominator == 1) ||
             (lcDisplayScaleNumerator == 2 && lDisplayScaleDenominator == 3));

  libResult = mComposition.Initialize(gcDisplayWidthInPixels, gcDisplayHeightPerPixels, gcDisplayBitsPerPixel,
                                      lcDisplayScaleNumerator, lDisplayScaleDenominator, mWeCompareWithReference);
  SHOW_ERROR(libResult == LIB_SUCCESS);
  GDC_TRACE;

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_CompositionTask::TaskService()
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
        case MESSAGE_ID_GDC_CNN_CLASSIFIER_DATA:
        {
          //
          // Process the received image
          //
          GdcCnn_ClassifierData* pData = (GdcCnn_ClassifierData*)pMessage;

          libResult = GdcCnn_CompositionTask::HandleClassifierData(*pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcCnn_ClassifierData();

          break;
        }
        case MESSAGE_ID_GDC_CNN_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult = GdcCnn_Helper::SendRequest(MESSAGE_ID_GDC_CNN_EXIT_REQUEST, mReceiveMessageQueueId,
                                                 mDisplayMessageQueueId, pMessage->mPriority);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          mIsTaskClosing = true;

          break;
        }
        case MESSAGE_ID_GDC_CNN_RESET_REQUEST:
        {
          // Do Nothing
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
LIB_RESULT GdcCnn_CompositionTask::TaskClose()
{
  GDC_TRACE;
  LIB_RESULT libResult = LIB_FAILURE;

  libResult = mComposition.Deinitialize();
  SHOW_ERROR(libResult == LIB_SUCCESS);

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
        case MESSAGE_ID_GDC_CNN_CLASSIFIER_DATA:
        {
          GdcCnn_ClassifierData* pData = (GdcCnn_ClassifierData*)pMessage;

          libResult = GdcCnn_Helper::FreeClassifierData(pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcCnn_ClassifierData();

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
LIB_RESULT GdcCnn_CompositionTask::HandleClassifierData(GdcCnn_ClassifierData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  uint32_t lBufferSizeInBytes = 0;

  libResult = mComposition.BufferSizeInBytes_Get(lBufferSizeInBytes);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  void* pCompositionBuffer = malloc(lBufferSizeInBytes);

  if(pCompositionBuffer != 0)
  {
    uint32_t lRightY = 0;

    icp::DataDescriptor lCompositionDd;
    rData.mTicks[gcCompTick] = FSL_Ticks();
    libResult = mComposition.Compose(rData, pCompositionBuffer, lBufferSizeInBytes, lCompositionDd, lRightY);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    rData.mTicks[gcCompTick + 1] = FSL_Ticks();

    if(libResult == LIB_SUCCESS)
    {
      libResult = GdcCnn_Helper::SendCompositionData(mDisplayMessageQueueId, pCompositionBuffer, lCompositionDd,
                                                     lRightY, rData.mSceneCounter, rData.mTicks);
      SHOW_ERROR(libResult == LIB_SUCCESS);
      if(libResult == LIB_SUCCESS)
      {
        // Ownership passes to the receiver
        pCompositionBuffer = 0;
        lCompositionDd.UpdatePtrs(0, 0);
      }
    }

    if(pCompositionBuffer != 0)
    {
      free(pCompositionBuffer);
      pCompositionBuffer = 0;
    }

    lCompositionDd.UpdatePtrs(0, 0);
  }

  return libResult;
}
