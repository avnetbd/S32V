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
#include "gdc_cnn_task_display.hpp"
#include "communications.hpp"
#include "iniparser.h"
#include "global_errors.h"
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcCnn_DisplayTask::GdcCnn_DisplayTask()
    : mReceiveMessageQueueId(0), mSceneMessageQueueId(0), mIsTaskClosing(false), mIsConfigured(false),
      //GdcCnn_Composition mDisplay;
      mEnableProfile(false), mSamplingDuration(0), mStartingFrame(0)
{
}

/***************************************************************************/
/***************************************************************************/
GdcCnn_DisplayTask::~GdcCnn_DisplayTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_DisplayTask::Configure(const int32_t cReceiveMessageQueueId, const int32_t cSceneMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId = cReceiveMessageQueueId;
    mSceneMessageQueueId   = cSceneMessageQueueId;

    mIsConfigured = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_DisplayTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  uint32_t lcDisplayScaleNumerator  = gcDisplayScaleNumerator;
  uint32_t lDisplayScaleDenominator = gcDisplayScaleDenominator;
  bool     lWeRotateDisplay         = false;

  dictionary* pDictionary = 0;

  pDictionary = iniparser_load(gcIniFileName);

  if(pDictionary != 0)
  {
    lcDisplayScaleNumerator =
        iniparser_getint(pDictionary, "DisplayTask:DisplayScaleNumerator", gcDisplayScaleNumerator);
    lDisplayScaleDenominator =
        iniparser_getint(pDictionary, "DisplayTask:DisplayScaleDenominator", gcDisplayScaleDenominator);
    lWeRotateDisplay = iniparser_getboolean(pDictionary, "DisplayTask:WeRotateDisplay", 0);

    mEnableProfile = (iniparser_getint(pDictionary, "Profiling:Enable", 0) == 1);
    if(true == mEnableProfile)
    {
      mSamplingDuration = iniparser_getint(pDictionary, "Profiling:SamplingDuration", 0);
      mStartingFrame    = iniparser_getint(pDictionary, "Profiling:StartingFrame", 0);

      char lString[gcFileNameSize + 1] = {0};
      strncpy(lString, gcProfileFileName, gcFileNameSize);

      strncpy(mOutputFilename, iniparser_getstring(pDictionary, "Profiling:OutputFilename", lString), gcFileNameSize);
      strncpy(mOutputFileFormart, iniparser_getstring(pDictionary, "Profiling:OutputFilFormat", "csv"), 3);
      printf("output file name = %s, %s\n", mOutputFilename, mOutputFileFormart);
      // allocate output data buffer
      mOutputDataUMat = vsdk::UMat(1, mSamplingDuration * sizeof(GdcCnn_ProfilingData), CV_8UC1);
      mOutputDataMat  = mOutputDataUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
    }

    iniparser_freedict(pDictionary);
    pDictionary = 0;
  }

  SHOW_ERROR((lcDisplayScaleNumerator == 1 && lDisplayScaleDenominator == 1) ||
             (lcDisplayScaleNumerator == 2 && lDisplayScaleDenominator == 3));

  libResult = mDisplay.Initialize(gcDisplayWidthInPixels, gcDisplayHeightPerPixels, gcDisplayBitsPerPixel,
                                  lcDisplayScaleNumerator, lDisplayScaleDenominator, lWeRotateDisplay);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  //
  // This makes the input image task send messages into the application data path.
  // The input image task could be an: image sensor, file reader...
  //
  for(uint32_t i = 0; i < gcInputImageQueueSize; ++i)
  {
    libResult =
        GdcCnn_Helper::SendRequest(MESSAGE_ID_GDC_CNN_IMAGE_REQUEST, mReceiveMessageQueueId, mSceneMessageQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }

  GDC_TRACE;

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_DisplayTask::TaskService()
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
        case MESSAGE_ID_GDC_CNN_COMPOSITION_DATA:
        {
          //
          // Request a new image to continue the cycle
          //
          libResult = GdcCnn_Helper::SendRequest(MESSAGE_ID_GDC_CNN_IMAGE_REQUEST, mReceiveMessageQueueId,
                                                 mSceneMessageQueueId);
          SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_SENDING_NOT_ALLOWED);

          //
          // Process the received image
          //
          GdcCnn_CompositionData* pData = (GdcCnn_CompositionData*)pMessage;

          libResult = GdcCnn_DisplayTask::HandleCompositionData(*pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcCnn_CompositionData();

          break;
        }
        case MESSAGE_ID_GDC_CNN_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
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
LIB_RESULT GdcCnn_DisplayTask::TaskClose()
{
  GDC_TRACE;
  LIB_RESULT libResult = LIB_FAILURE;

  libResult = mDisplay.Deinitialize();
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
        case MESSAGE_ID_GDC_CNN_COMPOSITION_DATA:
        {
          GdcCnn_CompositionData* pData = (GdcCnn_CompositionData*)pMessage;

          libResult = GdcCnn_DisplayTask::HandleCompositionData(*pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcCnn_CompositionData();

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

LIB_RESULT GdcCnn_DisplayTask::DumpProfilingInBuffer(uint8_t* pData, GdcCnn_CompositionData& rData)
{
  LIB_RESULT     libResult    = LIB_FAILURE;
  const uint32_t cTimeDivisor = 1000; // In linux values are in nanoseconds

  if(0 <= ((int32_t)rData.mSceneCounter - (int32_t)mStartingFrame))
  {
    if(rData.mSceneCounter == mStartingFrame)
    {
      printf("--- ft profiling (usecond)---\n");
      printf("| frame |  SceneTask  | ResizeTask  |ClassifierTask|  CompTask   | DisplayTask  |\n");
    }
    printf("| %5d |  %8zu   |  %8zu    |  %8zu   |  %8zu   |  %8zu   |\n", rData.mSceneCounter,
           (rData.mTicks[gcSceneTick + 1] - rData.mTicks[gcSceneTick]) / cTimeDivisor,
           (rData.mTicks[gcResizeTick + 1] - rData.mTicks[gcResizeTick]) / cTimeDivisor,
           (rData.mTicks[gcClassifierTick + 2] - rData.mTicks[gcClassifierTick]) / cTimeDivisor,
           (rData.mTicks[gcCompTick + 1] - rData.mTicks[gcCompTick]) / cTimeDivisor,
           (rData.mTicks[gcDisplayTick + 1] - rData.mTicks[gcDisplayTick]) / cTimeDivisor);

    GdcCnn_ProfilingData* pBuff                  = (GdcCnn_ProfilingData*)pData;
    GdcCnn_ProfilingData* pProfilingOutputBuffer = &pBuff[rData.mSceneCounter - mStartingFrame];
    pProfilingOutputBuffer->mSceneCounter        = rData.mSceneCounter;
    std::copy(rData.mTicks, rData.mTicks + gcApexCvEndTick, pProfilingOutputBuffer->mTicks);
    libResult = LIB_SUCCESS;
  }
  return libResult;
}

LIB_RESULT GdcCnn_DisplayTask::DumpProfilingInFile(char* profilingoutputFilename, uint8_t* profilingData)
{
  FILE*          pPtr;
  LIB_RESULT     libResult       = LIB_FAILURE;
  size_t         elementsWritten = 0;
  const uint32_t cTimeDivisor    = 1000; // In linux values are in nanoseconds

  GdcCnn_ProfilingData* pOutputBuf = (GdcCnn_ProfilingData*)profilingData;

  char writeBuffer[512];
  if((nullptr != profilingoutputFilename) && (nullptr != profilingData))
  {
    pPtr = fopen(profilingoutputFilename, "w");
    if(nullptr != pPtr)
    {
      if(strstr(mOutputFileFormart, "txt"))
      {
        sprintf(writeBuffer, "--- ft profiling (nanosecond)---\n");
        elementsWritten = fwrite(writeBuffer, strlen(writeBuffer), 1, pPtr);
        sprintf(writeBuffer, "| frame |  SceneTask  | ResizeTask  |ClassifierTask|  CompTask   | DisplayTask  |\n");
        elementsWritten = fwrite(writeBuffer, strlen(writeBuffer), 1, pPtr);
        for(uint32_t index = 0; index < mSamplingDuration; index++)
        {
          sprintf(writeBuffer, "| %5d |  %8zu   |  %8zu    |  %8zu   |  %8zu   |  %8zu   |\n",
                  pOutputBuf->mSceneCounter,
                  (pOutputBuf->mTicks[gcSceneTick + 1] - pOutputBuf->mTicks[gcSceneTick]) / cTimeDivisor,
                  (pOutputBuf->mTicks[gcResizeTick + 1] - pOutputBuf->mTicks[gcResizeTick]) / cTimeDivisor,
                  (pOutputBuf->mTicks[gcClassifierTick + 2] - pOutputBuf->mTicks[gcClassifierTick]) / cTimeDivisor,
                  (pOutputBuf->mTicks[gcCompTick + 1] - pOutputBuf->mTicks[gcCompTick]) / cTimeDivisor,
                  (pOutputBuf->mTicks[gcDisplayTick + 1] - pOutputBuf->mTicks[gcDisplayTick]) / cTimeDivisor);
          elementsWritten = fwrite(writeBuffer, 1, strlen(writeBuffer), pPtr);
          pOutputBuf++;
          if(elementsWritten != strlen(writeBuffer))
          {
            libResult = LIB_FAILURE;
            break;
          }
          else
          {
            libResult = LIB_SUCCESS;
          }
        }
      }
      else
      {
        // file format is .csv
        const uint32_t cNumberOfTasks           = 5;
        const char cStrings[cNumberOfTasks][64] = {"scene", "ResizeTask", "ClassifierTask", "CompTask", "DisplayTask"};
        const uint32_t cCounts[cNumberOfTasks]  = {gcResizeTick - gcSceneTick, gcClassifierTick - gcResizeTick,
                                                  gcCompTick - gcClassifierTick, gcDisplayTick - gcCompTick,
                                                  gcApexCvEndTick - gcDisplayTick};

        //
        // Heading
        //
        fprintf(pPtr, "%s,%s,%s", "frame", "threads", "tasks");

        for(uint32_t j = 0; j < cNumberOfTasks; ++j)
        {
          fprintf(pPtr, ",%s,%s", "name", "timestamps");
          for(uint32_t k = 0; k < cCounts[j]; ++k)
          {
            fprintf(pPtr, ",%s", "nanoseconds");
          }
        }
        fprintf(pPtr, "\n");

        //
        // Records
        //
        uint64_t lcFirstAndSmallestTimestamp = pOutputBuf->mTicks[gcSceneTick];
        for(uint32_t i = 0; i < mSamplingDuration; ++i) // For every frame
        {
          const uint64_t* pCurrentTick = pOutputBuf->mTicks;

          fprintf(pPtr, "%d,%d,%d", pOutputBuf->mSceneCounter, gcIsRunningWithThreads ? cNumberOfTasks : 1,
                  cNumberOfTasks);

          for(uint32_t j = 0; j < cNumberOfTasks; ++j) // For every task
          {
            fprintf(pPtr, ",%s,%d", cStrings[j], cCounts[j]);
            for(uint32_t k = 0; k < cCounts[j]; ++k)
            {
              fprintf(pPtr, ",%" PRIu64,
                      (pOutputBuf->mTicks[j * 2 + k] -
                       lcFirstAndSmallestTimestamp)); // substract to normalize outputs, so they all start from 0
            }
          }
          fprintf(pPtr, "\n");
          pOutputBuf++;
        } //for (uint32_t i = 0; i < mSamplingDuration; ++i)
      }
    }
    fclose(pPtr);
  }
  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_DisplayTask::HandleCompositionData(GdcCnn_CompositionData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  rData.mTicks[gcDisplayTick] = FSL_Ticks();
  libResult                   = mDisplay.PutFrame(rData);
  SHOW_ERROR(libResult == LIB_SUCCESS);
  if(rData.mpComposition != 0)
  {
    free(rData.mpComposition);
    rData.mpComposition = 0;
  }
  rData.mCompositionDd.UpdatePtrs(0, 0);
  rData.mTicks[gcDisplayTick + 1] = FSL_Ticks();
  printf("fps: %d\n", rData.mSceneCounter);
  if(true == mEnableProfile)
  {
    // check start mStartingFrame
    if(mStartingFrame <= rData.mSceneCounter)
    {
      if((mStartingFrame + mSamplingDuration) > rData.mSceneCounter)
      {
        // accurate time stamp up frame = mStartingFrame + mSamplingDuration
        DumpProfilingInBuffer(mOutputDataMat.data, rData);
      }
      else
      {
        // then dump profiling data
        // and disable profiling
        DumpProfilingInFile(mOutputFilename, mOutputDataMat.data);
        mEnableProfile = false;
      }
    }
  }

  return libResult;
}
