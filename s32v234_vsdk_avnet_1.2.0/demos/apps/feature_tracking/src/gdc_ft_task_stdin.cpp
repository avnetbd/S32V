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
#include "gdc_ft_task_stdin.hpp"
#include "gdc_ft_configuration.hpp"
#include "gdc_ft_messages.hpp"
#include "communications.hpp"
#include "iniparser.h"
#include "global_errors.h"
#include <stdint.h>

/***************************************************************************/
/* Local Literals & definitions */
static bool gcRequestClose = false;

void SigintHandler(int /*aSigNo*/)
{
  // Request to close
  gcRequestClose = true;
  printf("\n");
} // SigintHandler()

int32_t SigintSetup()
{
  int32_t lRet = SEQ_LIB_SUCCESS;
  // prepare internal signal handler
  struct sigaction lSa;
  memset(&lSa, 0, sizeof(lSa));
  lSa.sa_handler = SigintHandler;

  if(sigaction(SIGINT, &lSa, NULL) != 0)
  {
    printf("Failed to register signal handler.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if signal not registered

  return lRet;
} // SigintSetup()

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcFt_StdinTask::GdcFt_StdinTask()
    : mReceiveMessageQueueId(0), mSceneTaskMessageQueueId(0), mIsTaskClosing(false), mIsConfigured(false)
{
}

/***************************************************************************/
/***************************************************************************/
GdcFt_StdinTask::~GdcFt_StdinTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_StdinTask::Configure(const int32_t cReceiveMessageQueueId, const int32_t cSceneTaskMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId   = cReceiveMessageQueueId;
    mSceneTaskMessageQueueId = cSceneTaskMessageQueueId;

    mIsConfigured = true;

    libResult = LIB_SUCCESS;

    SigintSetup();
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_StdinTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  mIsTaskClosing = !(libResult == LIB_SUCCESS);

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_StdinTask::TaskService()
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
        case MESSAGE_ID_GDC_FT_EXIT_REQUEST:
        {
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

    if(mIsTaskClosing == false)
    {
      //
      // Receive commands from "stdin"
      //
      fd_set         rfds;
      struct timeval tv;
      int            retval, len;
      char           buff[255] = {0};

      FD_ZERO(&rfds);
      FD_SET(0, &rfds);

      // Wait up to 100 milis seconds.
      tv.tv_sec  = 0;
      tv.tv_usec = 100000;

      retval = select(1, &rfds, nullptr, nullptr, &tv);

      if(retval == -1)
      {
        fprintf(stderr, "GdcFt_StdinTask: select stdin error");
      }
      else if(retval)
      {
        // Read data from stdin using fgets.
        fgets(buff, sizeof(buff), stdin);

        // Remove trailing newline character from the input buffer if needed.
        len = strlen(buff) - 1;
        if(buff[len] == '\n')
          buff[len] = '\0';

        if(strcmp(buff, "quit") == 0 || strcmp(buff, "q") == 0)
        {
          gcRequestClose = true;
          mIsTaskClosing = true;
        }
        else
        {
          fprintf(stderr, "GdcFt_StdinTask: Unknown command \"%s\" : %lu\n", buff, strlen(buff));
        }
      }
    }

    if(gcRequestClose)
    {
      gcRequestClose = false;
      mIsTaskClosing = true;
      libResult      = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                    /*cIsSendingAllowed*/ false,
                                                    /*cIsReceivingAllowed*/ true);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      libResult =
          GdcFt_Helper::SendRequest(MESSAGE_ID_GDC_FT_EXIT_REQUEST, mReceiveMessageQueueId, mSceneTaskMessageQueueId);
      SHOW_ERROR(libResult == LIB_SUCCESS);
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
LIB_RESULT GdcFt_StdinTask::TaskClose()
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
        case MESSAGE_ID_GDC_FT_EXIT_REQUEST:
        {
          GdcFt_ExitRequest* pRequest = (GdcFt_ExitRequest*)pMessage;

          // Nothing to free

          pRequest->~GdcFt_ExitRequest();
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
