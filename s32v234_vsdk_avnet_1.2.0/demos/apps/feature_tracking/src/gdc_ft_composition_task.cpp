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
#include "gdc_ft_configuration.hpp"
#include "gdc_ft_composition_task.hpp"
#include "communications.hpp"
#include "iniparser.h"
#include "global_errors.h"
#include <stdint.h>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcFt_CompositionTask::GdcFt_CompositionTask()
    : mReceiveMessageQueueId(0), mDisplayTaskMessageQueueId(0), mIsTaskClosing(false), mIsConfigured(false),
      mWeDrawFullPaths(gcComposition_WeDrawFullPaths)
//GdcFt_Composition mComposition;
//uint64_t mPreviousTicks[2];
//uint64_t mSessionTicks[2];
{
  memset(mPreviousTicks, 0, sizeof(mPreviousTicks));
  memset(mSessionTicks, 0, sizeof(mSessionTicks));
}

/***************************************************************************/
/***************************************************************************/
GdcFt_CompositionTask::~GdcFt_CompositionTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_CompositionTask::Configure(const int32_t cReceiveMessageQueueId,
                                            const int32_t cDisplayTaskMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId     = cReceiveMessageQueueId;
    mDisplayTaskMessageQueueId = cDisplayTaskMessageQueueId;

    mIsConfigured = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_CompositionTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsTaskClosing == false) // Keep going
  {
    dictionary* pDictionary = 0;

    pDictionary = iniparser_load(gcIniFileName);

    if(pDictionary != 0)
    {
      mWeDrawFullPaths =
          (iniparser_getint(pDictionary, "Composition:WeDrawFullPaths", gcComposition_WeDrawFullPaths) == 1);

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }

    libResult = mComposition.Initialize(mWeDrawFullPaths);
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }

  GDC_TRACE;

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_CompositionTask::TaskService()
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
        case MESSAGE_ID_GDC_FT_TRACKER_DATA:
        {
          //
          // Process the received image
          //
          GdcFt_TrackerData* pData = (GdcFt_TrackerData*)pMessage;

          libResult = GdcFt_CompositionTask::HandleTrackerData(*pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcFt_TrackerData();

          break;
        }
        case MESSAGE_ID_GDC_FT_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult = GdcFt_Helper::SendRequest(MESSAGE_ID_GDC_FT_EXIT_REQUEST, mReceiveMessageQueueId,
                                                mDisplayTaskMessageQueueId);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          mIsTaskClosing = true;

          break;
        }
        case MESSAGE_ID_GDC_FT_RESET_REQUEST:
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
LIB_RESULT GdcFt_CompositionTask::TaskClose()
{
  GDC_TRACE;
  LIB_RESULT libResult = LIB_FAILURE;

  //
  // Free resources owned by the task
  //

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
        case MESSAGE_ID_GDC_FT_TRACKER_DATA:
        {
          GdcFt_TrackerData* pData = (GdcFt_TrackerData*)pMessage;

          libResult = GdcFt_Helper::FreeTrackerData(pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcFt_TrackerData();

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
LIB_RESULT GdcFt_CompositionTask::HandleTrackerData(GdcFt_TrackerData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(rData.mScene_umat.empty() == false)
  {
    mPreviousTicks[1] = FSL_Ticks();

    mPreviousTicks[0] = FSL_Ticks();

    rData.mTicks[gcCompTick] = FSL_Ticks();
    libResult                = mComposition.PutFrame(rData);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    rData.mTicks[gcCompTick + 1] = FSL_Ticks();

    if(rData.mSceneRgb24_umat.empty() == false)
    {
      libResult = GdcFt_Helper::SendCompositionData(
          mDisplayTaskMessageQueueId, rData.mSceneRgb24_umat,
          rData.mSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
          rData.mSceneCounter, rData.mTicks);
      SHOW_ERROR(libResult == LIB_SUCCESS);
      if(libResult == LIB_SUCCESS)
      {
        rData.mSceneRgb24_umat.release(); // Ownership passes to the receiver
      }
    }

    libResult = GdcFt_Helper::FreeTrackerData(&rData);
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }

  return libResult;
}
