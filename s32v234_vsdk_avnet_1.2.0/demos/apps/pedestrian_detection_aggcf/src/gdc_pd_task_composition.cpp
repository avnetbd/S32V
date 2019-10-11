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
#include "gdc_pd_task_composition.hpp"
#include "communications.hpp"
#include "global_errors.h"
#include <stdlib.h>
#include <stdint.h>
#include "iniparser.h"

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcPd_CompositionTask::GdcPd_CompositionTask()
    : mReceiveMessageQueueId(0), mDisplayTaskMessageQueueId(0), mIsTaskClosing(false), mIsConfigured(false)
//GdcPd_Composition mComposition;

//io::FrameOutputV234Fb mFrameOutput;
{
}

/***************************************************************************/
/***************************************************************************/
GdcPd_CompositionTask::~GdcPd_CompositionTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_CompositionTask::Configure(const int32_t cReceiveMessageQueueId,
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
LIB_RESULT GdcPd_CompositionTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT  libResult   = LIB_FAILURE;
  dictionary* pDictionary = 0;
  pDictionary             = iniparser_load(gcIniFileName);

  if(pDictionary != 0)
  {
    iniparser_freedict(pDictionary);
    pDictionary = 0;
  }

  libResult = mComposition.Initialize();
  SHOW_ERROR(libResult == LIB_SUCCESS);

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_CompositionTask::TaskService()
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
        case MESSAGE_ID_GDC_PD_AGGCF_DATA:
        {
          //
          // Process the received image
          //
          GdcPd_DetectorData* pData = (GdcPd_DetectorData*)pMessage;

          libResult = GdcPd_CompositionTask::HandleAggcfData(*pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcPd_DetectorData();

          break;
        }
        case MESSAGE_ID_GDC_PD_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult = GdcPd_Helper::SendRequest(MESSAGE_ID_GDC_PD_EXIT_REQUEST, mReceiveMessageQueueId,
                                                mDisplayTaskMessageQueueId, pMessage->mPriority);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          mIsTaskClosing = true;

          break;
        }
        case MESSAGE_ID_GDC_PD_RESET_REQUEST:
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
LIB_RESULT GdcPd_CompositionTask::TaskClose()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  //
  // Free resources owned by the task
  //
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
        case MESSAGE_ID_GDC_PD_AGGCF_DATA:
        {
          GdcPd_DetectorData* pData = (GdcPd_DetectorData*)pMessage;

          libResult = GdcPd_Helper::FreeDetectorData(pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          pData->~GdcPd_DetectorData();

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
      break; // No more messages to clean up
    }
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_CompositionTask::HandleAggcfData(GdcPd_DetectorData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  SHOW_ERROR(rData.mSceneRgb24_umat.empty() == false);

  if(rData.mSceneRgb24_umat.empty() == false)
  {
    //
    // Display
    //
    libResult = mComposition.PutFrame(rData);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = GdcPd_Helper::SendDetectorData(
        mDisplayTaskMessageQueueId, rData.mSceneRgb24_umat,
        rData.mSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
        rData.mSceneCounter, rData.mPedestrianRois, rData.mTicks, rData.mPerDetectorAcfProfilingInfo);
    ;
    SHOW_ERROR(libResult == LIB_SUCCESS);
    if(libResult == LIB_SUCCESS)
    {
      rData.mSceneRgb24_umat.release(); // Ownership passes to the receiver
    }

    libResult = GdcPd_Helper::FreeDetectorData(&rData);
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }

  return libResult;
}
