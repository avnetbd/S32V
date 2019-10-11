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
#include "gdc_cnn_task_stdin.hpp"
#include "communications.hpp"
#include "iniparser.h"
#include "global_errors.h"
#include <stdint.h>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcCnn_StdinTask::GdcCnn_StdinTask()
    : mReceiveMessageQueueId(0), mSceneTaskMessageQueueId(0), mIsTaskClosing(false), mIsConfigured(false)
{
}

/***************************************************************************/
/***************************************************************************/
GdcCnn_StdinTask::~GdcCnn_StdinTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_StdinTask::Configure(const int32_t cReceiveMessageQueueId, const int32_t cSceneTaskMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId   = cReceiveMessageQueueId;
    mSceneTaskMessageQueueId = cSceneTaskMessageQueueId;

    mIsConfigured = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_StdinTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  mIsTaskClosing = !(libResult == LIB_SUCCESS);

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_StdinTask::TaskService()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  MessageBase* pMessage           = 0;
  int32_t      messageSizeInBytes = 0;

  while(mIsTaskClosing == false)
  {
    // Check and Handle messages after every "getlin"
    // Only one I can think about is the EXIT message to close the task
    libResult = MsgMgr_MessageReceive(mReceiveMessageQueueId, pMessage, messageSizeInBytes,
                                      0); // Don't wait
    if(libResult == LIB_SUCCESS && pMessage != 0)
    {
      switch(pMessage->mMessageId)
      {
        case MESSAGE_ID_GDC_CNN_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult = GdcCnn_Helper::SendRequest(MESSAGE_ID_GDC_CNN_EXIT_REQUEST, mReceiveMessageQueueId,
                                                 mSceneTaskMessageQueueId, 0);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          fprintf(stderr, "GdcPd_StdinTask forced exit\n");

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

    //
    // Receive commands from "stdin"
    //
    char*  lpLine     = NULL;
    size_t lpLineSize = 0;

    ssize_t lSSizeResult = getline(&lpLine, &lpLineSize, stdin);
    if(lSSizeResult > 0 && lpLine != 0)
    {
      lpLine[strlen(lpLine) - 1] = 0; // Get rid of delimiter

      if(strcmp(lpLine, "quit") == 0 || strcmp(lpLine, "q") == 0)
      {
        libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                      /*cIsSendingAllowed*/ false,
                                                      /*cIsReceivingAllowed*/ true);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        libResult = GdcCnn_Helper::SendRequest(MESSAGE_ID_GDC_CNN_EXIT_REQUEST, mReceiveMessageQueueId,
                                               mSceneTaskMessageQueueId, 0);
        SHOW_ERROR(libResult == LIB_SUCCESS);
        mIsTaskClosing = true;
      }
      else
      {
        fprintf(stderr, "GdcCnn_StdinTask: Unknown command \"%s\" : %lu\n", lpLine, strlen(lpLine));
      }

      free(lpLine);
      lpLine = 0;
    }
    else
    {
      mIsTaskClosing = true;
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
LIB_RESULT GdcCnn_StdinTask::TaskClose()
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
