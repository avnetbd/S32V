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
#include "gdc_pd_task_file_reader.hpp"
#include "communications.hpp"
#include "iniparser.h"
#include "global_errors.h"
#include <stdint.h>
#include <common_time_measure.h>

#include <oal_protected.h>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcPd_FileReaderTask::GdcPd_FileReaderTask()
    : mReceiveMessageQueueId(0), mAggcfLuvMessageQueueId(0), mSceneCounter(0),
      //char    mFileName[gcFileNameSize];
      mpFile(0), mWeRunVideoInALoop(false), mIsTaskClosing(false), mIsConfigured(false)
{
  strncpy(mFileName, gcFileName, gcFileNameSize);
}

/***************************************************************************/
/***************************************************************************/
GdcPd_FileReaderTask::~GdcPd_FileReaderTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_FileReaderTask::Configure(const int32_t cReceiveMessageQueueId, const int32_t cAggcfLuvMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId  = cReceiveMessageQueueId;
    mAggcfLuvMessageQueueId = cAggcfLuvMessageQueueId;
    mIsConfigured           = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_FileReaderTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  if(mIsConfigured == true && mpFile == 0)
  {
    dictionary* pDictionary = 0;
    pDictionary             = iniparser_load(gcIniFileName);

    char cFileName[256] = {0};

    if(pDictionary != 0)
    {
      mWeRunVideoInALoop = (iniparser_getint(pDictionary, "FileReader:WeLoop", 0) == 1);

      char lString[gcFileNameSize + 1] = {0};

      strncpy(lString, gcFileName, gcFileNameSize);

      strncpy(cFileName, iniparser_getstring(pDictionary, "FileReader:Filename", lString), gcFileNameSize);

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }
    sprintf(mFileName, "%s%s", gcDataPath, cFileName);

    mpFile = fopen(mFileName, "rb");
    SHOW_ERROR(mpFile != 0);

    if(mpFile == 0)
    {
      fprintf(stderr, "%s: error %s: No such file\n", __PRETTY_FUNCTION__, mFileName);
      mIsTaskClosing = true;
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
LIB_RESULT GdcPd_FileReaderTask::TaskService()
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
        case MESSAGE_ID_GDC_PD_IMAGE_REQUEST:
        {
          GdcPd_ImageRequest* pRequest = (GdcPd_ImageRequest*)pMessage;

          libResult = GdcPd_FileReaderTask::HandleImageRequest(*pRequest);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          if(mIsTaskClosing == false)
          {
            mIsTaskClosing = !(libResult == LIB_SUCCESS);
          }

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
LIB_RESULT GdcPd_FileReaderTask::TaskClose()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;
  int        intResult = -1;

  //
  // Free resources owned by the task
  //
  if(mpFile != 0)
  {
    intResult = fclose(mpFile);
    SHOW_ERROR(intResult == 0);
  }

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
LIB_RESULT GdcPd_FileReaderTask::HandleImageRequest(const GdcPd_ImageRequest&)
{
  LIB_RESULT libResult = LIB_FAILURE;

  int intResult = -1;

  uint64_t ticks[2] = {0};

  ticks[0] = FSL_Ticks();

  const uint32_t cFileSceneWidthInPixels =
      gcSceneWidthInPixels +
      (2 * (gcScenePaddingInBytes * 8 / gcSceneBitsPerPixel)); // 2 for left and right padding, 8 bits per byte
  const uint32_t cFileSceneHeightInPixels =
      gcSceneHeightInPixels +
      (2 * (gcScenePaddingInBytes * 8 / gcSceneBitsPerPixel)); // 2 for top and bottom padding, 8 bits per byte

  vsdk::Rect lRect((gcScenePaddingInBytes * 8 / gcSceneBitsPerPixel), (gcScenePaddingInBytes * 8 / gcSceneBitsPerPixel),
                   gcSceneWidthInPixels, gcSceneHeightInPixels);

  // note: input image has rgb32 (RGBX) color format
  vsdk::UMat lFileScene_umat(cFileSceneHeightInPixels, cFileSceneWidthInPixels, VSDK_CV_32SC1);

  if(lFileScene_umat.empty() == false)
  {
    bool wasImageRead = false;

    size_t bytesRead = 0;

    { // Need this scope to allow vsdk::Mat to go out of scope prior to sending message
      vsdk::Mat lFileScene_mat = lFileScene_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
      void*     pFileScene     = lFileScene_mat.data;

      bytesRead = fread(pFileScene, 1, gcSceneBufferSizeInBytes, mpFile);

      if(gcSceneBufferSizeInBytes > 0 && bytesRead == (uint32_t)gcSceneBufferSizeInBytes)
      {
        wasImageRead = true;
      }
      else if(mWeRunVideoInALoop == true)
      {
        intResult = fseek(mpFile, 0, SEEK_SET);

        if(intResult == 0)
        {
          bytesRead = fread(pFileScene, 1, gcSceneBufferSizeInBytes, mpFile);

          if(gcSceneBufferSizeInBytes > 0 && bytesRead == (uint32_t)gcSceneBufferSizeInBytes)
          {
            wasImageRead = true;
          }
          else
          {
            printf("image data reading error %d, %d\n", (int32_t)bytesRead, gcSceneBufferSizeInBytes);
          }
        }
      }
      else if(mWeRunVideoInALoop == false)
      {
        libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                      /*cIsSendingAllowed*/ false,
                                                      /*cIsReceivingAllowed*/ true);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        libResult =
            GdcPd_Helper::SendRequest(MESSAGE_ID_GDC_PD_EXIT_REQUEST, mReceiveMessageQueueId, mAggcfLuvMessageQueueId);
        SHOW_ERROR(libResult == LIB_SUCCESS);
        mIsTaskClosing = true;

        libResult = LIB_APP_IS_CLOSING;
      }
    }

    if(wasImageRead == true)
    {
      vsdk::UMat lSceneRgb32_umat(lFileScene_umat, lRect);

      vsdk::UMat lSceneRgb24_umat(gcSceneHeightInPixels, gcSceneWidthInPixels, VSDK_CV_8UC3);

      // convert RGBX(32bit) image to RGB(24bit) image for aggcf demo
      libResult = GdcPd_Helper::ConvertRgb32ToRgb24(lSceneRgb32_umat, lSceneRgb24_umat);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      ticks[1] = FSL_Ticks();

      libResult =
          GdcPd_Helper::SendImageReply(mAggcfLuvMessageQueueId, lSceneRgb24_umat,
                                       lSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
                                       lSceneRgb32_umat,
                                       lSceneRgb32_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
                                       mSceneCounter++, ticks);
      SHOW_ERROR(libResult == LIB_SUCCESS);
    }
  }

  return libResult;
}
