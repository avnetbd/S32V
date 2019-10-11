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
#include "gdc_cnn_task_file_reader.hpp"
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
GdcCnn_FileReaderTask::GdcCnn_FileReaderTask()
    : mReceiveMessageQueueId(0), mResizeTaskMessageQueueId(0),
      //std::string                         mDatasetDirectory;
      //std::vector<std::string>            mDatasetList;
      //std::vector<std::string>::iterator  mDatasetList_Iterator;
      mSceneCounter(0), mFrameDurationInUs(0), mWeLoop(false), mIsTaskClosing(false), mIsConfigured(false)
{
  timespec lTimeSpec = {0, 0};

  mTimeSpecLastSend = lTimeSpec;
  mTimeSpecNow      = lTimeSpec;
  mTimeSpecDiff     = lTimeSpec;
  mTimeSpecSleep    = lTimeSpec;
}

/***************************************************************************/
/***************************************************************************/
GdcCnn_FileReaderTask::~GdcCnn_FileReaderTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_FileReaderTask::Configure(const int32_t cReceiveMessageQueueId,
                                            const int32_t cResizeTaskMessageQueueId)
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
LIB_RESULT GdcCnn_FileReaderTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  if(mIsConfigured == true)
  {
    std::string lDatasetListFilename = gcDatasetListFilename;

    dictionary* pDictionary = 0;
    ;

    pDictionary = iniparser_load(gcIniFileName);

    if(pDictionary != 0)
    {
      const uint32_t cLenMax           = 256;
      char           lCString[cLenMax] = "";
      char*          pString           = 0;

      strncpy(lCString, gcDatasetDirectory, cLenMax);
      pString = iniparser_getstring(pDictionary, "FileReaderTask:DatasetDirectory", lCString);

      mDatasetDirectory = pString;

      strncpy(lCString, gcDatasetListFilename, cLenMax);
      pString = iniparser_getstring(pDictionary, "FileReaderTask:DatasetListFilename", lCString);

      lDatasetListFilename = pString;

      mFrameDurationInUs = iniparser_getint(pDictionary, "FileReaderTask:FrameDurationInMs", 0);
      mFrameDurationInUs *= 1000; // *1000 to go from miliseconds to microseconds
      mWeLoop = iniparser_getboolean(pDictionary, "FileReaderTask:WeLoop", 0);

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }

    if(mDatasetDirectory.back() != '/')
    {
      mDatasetDirectory.push_back('/');
    }

    libResult = GdcCnn_Helper::ReadDatasetListFromFile(lDatasetListFilename.c_str(), mDatasetList);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    if(mDatasetList.empty() == false)
    {
      mDatasetList_Iterator = mDatasetList.begin();
    }
    else
    {
      SHOW_ERROR(false);
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
LIB_RESULT GdcCnn_FileReaderTask::TaskService()
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
        case MESSAGE_ID_GDC_CNN_IMAGE_REQUEST:
        {
          GdcCnn_ImageRequest* pRequest = (GdcCnn_ImageRequest*)pMessage;

          libResult = GdcCnn_FileReaderTask::HandleImageRequest(*pRequest);
          SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
          GDC_TRACE;

          if(mIsTaskClosing == false)
          {
            mIsTaskClosing = !(libResult == LIB_SUCCESS);
          }

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
LIB_RESULT GdcCnn_FileReaderTask::TaskClose()
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
LIB_RESULT GdcCnn_FileReaderTask::HandleImageRequest(const GdcCnn_ImageRequest& /*rRequest*/)
{
  LIB_RESULT libResult = LIB_FAILURE;

  uint64_t ticks[gcApexCvEndTick] = {0};

  ticks[0] = FSL_Ticks();

  bool weFoundFile = false;

  while(weFoundFile == false)
  {
    std::string lFilename;

    lFilename.append(mDatasetDirectory);
    lFilename.append(*mDatasetList_Iterator);

    FILE* pFile = fopen(lFilename.c_str(), "rb");

    if(pFile != 0)
    {
      fclose(pFile);

      pFile = 0;

      cv::Mat lSceneMat;

      lSceneMat = cv::imread(lFilename.c_str(), CV_LOAD_IMAGE_COLOR);

      if(lSceneMat.empty() != true)
      {
        weFoundFile = true;

        //
        // Pacing code
        //
        if(mFrameDurationInUs > 0)
        {
          const timespec ctimeSpecSleep = {mFrameDurationInUs / (1000 * 1000),
                                           (mFrameDurationInUs * 1000) % (1000 * 1000 * 1000)};

          if(mTimeSpecLastSend.tv_sec == 0 && mTimeSpecLastSend.tv_nsec == 0)
          {
            clock_gettime(CLOCK_REALTIME, &mTimeSpecLastSend);
          }
          else
          {
            int intResult = -1;

            clock_gettime(CLOCK_REALTIME, &mTimeSpecNow);

            mTimeSpecDiff = GdcCnn_Helper::timespec_diff(mTimeSpecNow, mTimeSpecLastSend);

            mTimeSpecSleep = GdcCnn_Helper::timespec_diff(ctimeSpecSleep, mTimeSpecDiff);

            do
            {
              intResult = nanosleep(&mTimeSpecSleep, &mTimeSpecSleep);
            } while(intResult == -1 && errno == EINTR);

            clock_gettime(CLOCK_REALTIME, &mTimeSpecLastSend);
          }
        }

        ticks[1] = FSL_Ticks();

        libResult = GdcCnn_Helper::SendImageReply(mResizeTaskMessageQueueId, lSceneMat, mSceneCounter,
                                                  *mDatasetList_Iterator, ticks);
      }
    }

    ++mSceneCounter;

    ++mDatasetList_Iterator;

    if(mDatasetList_Iterator == mDatasetList.end())
    {
      if(mWeLoop == true)
      {
        mDatasetList_Iterator = mDatasetList.begin();
        //mSceneCounter = 0;
      }
      else
      {
        libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                      /*cIsSendingAllowed*/ false,
                                                      /*cIsReceivingAllowed*/ true);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        libResult = GdcCnn_Helper::SendRequest(MESSAGE_ID_GDC_CNN_EXIT_REQUEST, mReceiveMessageQueueId,
                                               mResizeTaskMessageQueueId);
        SHOW_ERROR(libResult == LIB_SUCCESS);
        mIsTaskClosing = true;

        libResult = LIB_APP_IS_CLOSING;
        break;
      }
    }
  }

  return libResult;
}
