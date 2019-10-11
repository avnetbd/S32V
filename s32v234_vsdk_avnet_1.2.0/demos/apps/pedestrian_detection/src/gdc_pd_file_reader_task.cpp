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
#include "gdc_pd_file_reader_task.hpp"
#include "communications.hpp"
#include "iniparser.h"
#include "global_errors.h"
#include <stdint.h>
#include <common_time_measure.h>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcPd_FileReaderTask::GdcPd_FileReaderTask()
    : mReceiveMessageQueueId(0), mResizeTaskMessageQueueId(0), mSceneCounter(0),
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
LIB_RESULT GdcPd_FileReaderTask::Configure(const int32_t cReceiveMessageQueueId,
                                           const int32_t cResizeTaskMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId    = cReceiveMessageQueueId;
    mResizeTaskMessageQueueId = cResizeTaskMessageQueueId;
    mIsConfigured             = true;

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
    ;

    pDictionary = iniparser_load(gcIniFileName);

    if(pDictionary != 0)
    {
      mWeRunVideoInALoop = (iniparser_getint(pDictionary, "FileReader:WeLoop", 0) == 1);

      char lString[gcFileNameSize + 1] = {0};

      strncpy(lString, gcFileName, gcFileNameSize);

      strncpy(mFileName, iniparser_getstring(pDictionary, "FileReader:Filename", lString), gcFileNameSize);

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }

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
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId, false, true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult = GdcPd_Helper::SendRequest(MESSAGE_ID_GDC_PD_EXIT_REQUEST, mReceiveMessageQueueId,
                                                mResizeTaskMessageQueueId);
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

  vsdk::UMat lFileScene_umat(cFileSceneHeightInPixels, cFileSceneWidthInPixels, VSDK_CV_8UC1);

  uint8_t* pFileBuffer = (uint8_t*)malloc(gcSceneBufferSizeInBytes);

  if((lFileScene_umat.empty() == false) && (pFileBuffer != nullptr))
  {
    bool wasImageRead = false;

    size_t bytesRead = fread((void*)pFileBuffer, 1, gcSceneSizeInBytes, mpFile);

    if((bytesRead != gcSceneSizeInBytes) && (mWeRunVideoInALoop == true))
    {
      intResult = fseek(mpFile, 0, SEEK_SET);

      if(intResult == 0)
      {
        bytesRead = fread((void*)pFileBuffer, 1, gcSceneSizeInBytes, mpFile);
      }
    }
    else if(mWeRunVideoInALoop == false)
    {
      libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                    /*cIsSendingAllowed*/ false,
                                                    /*cIsReceivingAllowed*/ true);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      libResult =
          GdcPd_Helper::SendRequest(MESSAGE_ID_GDC_PD_EXIT_REQUEST, mReceiveMessageQueueId, mResizeTaskMessageQueueId);
      SHOW_ERROR(libResult == LIB_SUCCESS);
      mIsTaskClosing = true;
    }

    if(gcSceneSizeInBytes > 0 && bytesRead == gcSceneSizeInBytes)
    {
      // Copy buffer to padded Mat, ensure Mat goes out of scope before message is sent
      wasImageRead             = true;
      vsdk::Mat lFileScene_mat = lFileScene_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
      uint8_t*  pFileSceneData = lFileScene_mat.data;

      memset(pFileSceneData, 0, lFileScene_umat.cols * lFileScene_umat.rows);

      int readOffset  = 0;
      int writeOffset = gcScenePaddingInBytes * (1 + lFileScene_mat.step[0]);
      for(uint32_t row = 0; row < gcSceneHeightInPixels; ++row)
      {
        memcpy(pFileSceneData + writeOffset, pFileBuffer + readOffset, gcSceneWidthInPixels);
        readOffset += gcSceneWidthInPixels;
        writeOffset += lFileScene_mat.step[0];
      }
    }

    if(wasImageRead == true)
    {
      vsdk::UMat lScene_umat(lFileScene_umat, lRect);

      vsdk::UMat lSceneRgb24_umat(gcSceneHeightInPixels, gcSceneWidthInPixels, VSDK_CV_8UC3);

      libResult = GdcPd_Helper::ConvertYToRgb24(lScene_umat, lSceneRgb24_umat);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      ticks[1] = FSL_Ticks();

      libResult =
          GdcPd_Helper::SendImageReply(mResizeTaskMessageQueueId, lSceneRgb24_umat,
                                       lSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
                                       lScene_umat,
                                       lScene_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
                                       mSceneCounter++, ticks);
      SHOW_ERROR(libResult == LIB_SUCCESS);
    }
  }
  free(pFileBuffer);

  return libResult;
}
