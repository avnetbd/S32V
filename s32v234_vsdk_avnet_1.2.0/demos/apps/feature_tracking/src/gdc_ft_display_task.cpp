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
#include "gdc_ft_display_task.hpp"
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
GdcFt_DisplayTask::GdcFt_DisplayTask()
    : mReceiveMessageQueueId(0), mSceneTaskMessageQueueId(0), mIsTaskClosing(false), mIsConfigured(false),
      mDisplayWidthInPixels(gcDisplayWidthInPixels), mDisplayHeightInPixels(gcDisplayHeightInPixels),
      mDisplayBitsPerPixel(gcDisplayBitsPerPixel), mWeRotateDisplay(gcDisplay_WeRotateDisplay), mEnableProfile(false),
      mSamplingDuration(0), mStartingFrame(0)

{
}

/***************************************************************************/
/***************************************************************************/
GdcFt_DisplayTask::~GdcFt_DisplayTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_DisplayTask::Configure(const int32_t cReceiveMessageQueueId, const int32_t cSceneTaskMessageQueueId)
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
LIB_RESULT GdcFt_DisplayTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsTaskClosing == false) // Keep going
  {
    dictionary* pDictionary = 0;

    pDictionary = iniparser_load(gcIniFileName);

    if(pDictionary != 0)
    {
      mDisplayWidthInPixels  = iniparser_getint(pDictionary, "Display:DisplayWidthInPixels", gcDisplayWidthInPixels);
      mDisplayHeightInPixels = iniparser_getint(pDictionary, "Display:DisplayHeightInPixels", gcDisplayHeightInPixels);
      mDisplayBitsPerPixel   = iniparser_getint(pDictionary, "Display:DisplayBitsPerPixel", gcDisplayBitsPerPixel);

      mWeRotateDisplay = (iniparser_getint(pDictionary, "Display:WeRotateDisplay", gcDisplay_WeRotateDisplay) == 1);

      mEnableProfile = (iniparser_getint(pDictionary, "Profiling:Enable", 0) == 1);
      if(true == mEnableProfile)
      {
        mSamplingDuration = iniparser_getint(pDictionary, "Profiling:SamplingDuration", 0);
        mStartingFrame    = iniparser_getint(pDictionary, "Profiling:StartingFrame", 0);

        char lString[gcFileNameSize + 1] = {0};
        strncpy(lString, gcProfileFileName, gcFileNameSize);

        strncpy(mOutputFilename, iniparser_getstring(pDictionary, "Profiling:OutputFilename", lString), gcFileNameSize);
        strncpy(mOutputfileFormat, iniparser_getstring(pDictionary, "Profiling:OutputFilFormat", "csv"), 3);
        printf("output file name = %s, %s\n", mOutputFilename, mOutputfileFormat);
        // allocate output data buffer
        mOutputDataUMat = vsdk::UMat(1, mSamplingDuration * sizeof(GdcFt_ProfilingData), CV_8UC1);
        mOutputDataMat  = mOutputDataUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
      }

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }

    libResult =
        mDisplay.Initialize(mDisplayWidthInPixels, mDisplayHeightInPixels, mDisplayBitsPerPixel, mWeRotateDisplay);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    //
    // This makes the input image task send messages into the application data path.
    // The input image task could be an: image sensor, file reader...
    //
    for(uint32_t i = 0; i < gcSceneTask_ImageRequests; ++i)
    {
      libResult =
          GdcFt_Helper::SendRequest(MESSAGE_ID_GDC_FT_IMAGE_REQUEST, mReceiveMessageQueueId, mSceneTaskMessageQueueId);
      SHOW_ERROR(libResult == LIB_SUCCESS);
    }
  }

  GDC_TRACE;

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_DisplayTask::TaskService()
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
        case MESSAGE_ID_GDC_FT_COMPOSITION_DATA:
        {
          //
          // Request a new image to continue the cycle
          //
          libResult = GdcFt_Helper::SendRequest(MESSAGE_ID_GDC_FT_IMAGE_REQUEST, mReceiveMessageQueueId,
                                                mSceneTaskMessageQueueId);
          SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_SENDING_NOT_ALLOWED);

          //
          // Process the received image
          //
          GdcFt_CompositionData* pData = (GdcFt_CompositionData*)pMessage;

          libResult = GdcFt_DisplayTask::HandleCompositionData(*pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcFt_CompositionData();

          break;
        }
        case MESSAGE_ID_GDC_FT_DETECTOR_DATA:
        {
          //
          // Request a new image to continue the cycle
          //
          libResult = GdcFt_Helper::SendRequest(MESSAGE_ID_GDC_FT_IMAGE_REQUEST, mReceiveMessageQueueId,
                                                mSceneTaskMessageQueueId);
          SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_SENDING_NOT_ALLOWED);

          //
          // Process the received image
          //
          GdcFt_DetectorData* pData = (GdcFt_DetectorData*)pMessage;

          libResult = GdcFt_DisplayTask::HandleDetectorData(*pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcFt_DetectorData();

          break;
        }
        case MESSAGE_ID_GDC_FT_PYRAMID_DATA:
        {
          //
          // Request a new image to continue the cycle
          //
          libResult = GdcFt_Helper::SendRequest(MESSAGE_ID_GDC_FT_IMAGE_REQUEST, mReceiveMessageQueueId,
                                                mSceneTaskMessageQueueId);
          SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_SENDING_NOT_ALLOWED);

          //
          // Process the received image
          //
          GdcFt_PyramidData* pData = (GdcFt_PyramidData*)pMessage;

          libResult = GdcFt_DisplayTask::HandlePyramidData(*pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          mIsTaskClosing = !(libResult == LIB_SUCCESS);

          pData->~GdcFt_PyramidData();
          break;
        }
        case MESSAGE_ID_GDC_FT_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
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
LIB_RESULT GdcFt_DisplayTask::TaskClose()
{
  GDC_TRACE;
  LIB_RESULT libResult = LIB_FAILURE;

  //
  // Free resources owned by the task
  //
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
        case MESSAGE_ID_GDC_FT_COMPOSITION_DATA:
        {
          GdcFt_CompositionData* pData = (GdcFt_CompositionData*)pMessage;

          libResult = GdcFt_Helper::FreeCompositionData(pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcFt_CompositionData();

          break;
        }
        case MESSAGE_ID_GDC_FT_DETECTOR_DATA:
        {
          GdcFt_DetectorData* pData = (GdcFt_DetectorData*)pMessage;

          libResult = GdcFt_Helper::FreeDetectorData(pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcFt_DetectorData();

          break;
        }
        case MESSAGE_ID_GDC_FT_PYRAMID_DATA:
        {
          GdcFt_PyramidData* pData = (GdcFt_PyramidData*)pMessage;

          libResult = GdcFt_Helper::FreePyramidData(pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcFt_PyramidData();
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
LIB_RESULT GdcFt_DisplayTask::HandleCompositionData(GdcFt_CompositionData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(rData.mSceneRgb24_umat.empty() == false)
  {
    rData.mTicks[gcDisplayTick] = FSL_Ticks();
    libResult                   = mDisplay.PutFrame(rData);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    rData.mTicks[gcDisplayTick + 1] = FSL_Ticks();

    // dump profiling data for detector
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

    libResult = GdcFt_Helper::FreeCompositionData(&rData);
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }

  return libResult;
}

LIB_RESULT GdcFt_DisplayTask::DumpProfilingInBuffer(uint8_t* pData, GdcFt_CompositionData& rData)
{
  LIB_RESULT     libResult    = LIB_FAILURE;
  const uint32_t cTimeDivisor = 1000; // In linux values are in nanoseconds

  if(0 <= ((int32_t)rData.mSceneCounter - (int32_t)mStartingFrame))
  {
    if(rData.mSceneCounter == mStartingFrame)
    {
      printf("--- ft profiling (usecond)---\n");
      printf("| frame |  SceneTask  | DetectorTask | pyramidTask  | trackerTask  |   CompTask   | DisplayTask  |\n");
    }
    printf("| %5d |  %8zu   |  %8zu    |  %8zu    |  %8zu    |  %8zu    |  %8zu    |\n", rData.mSceneCounter,
           (rData.mTicks[gcSceneTick + 1] - rData.mTicks[gcSceneTick]) / cTimeDivisor,
           (rData.mTicks[gcDetectorTick + 3] - rData.mTicks[gcDetectorTick]) / cTimeDivisor,
           (rData.mTicks[gcPyramidTick + 3] - rData.mTicks[gcPyramidTick]) / cTimeDivisor,
           (rData.mTicks[gcTrackerTick + 3] - rData.mTicks[gcTrackerTick]) / cTimeDivisor,
           (rData.mTicks[gcCompTick + 1] - rData.mTicks[gcCompTick]) / cTimeDivisor,
           (rData.mTicks[gcDisplayTick + 1] - rData.mTicks[gcDisplayTick]) / cTimeDivisor);

    GdcFt_ProfilingData* pOutputBuf       = (GdcFt_ProfilingData*)pData;
    GdcFt_ProfilingData* pProfilingOutput = &pOutputBuf[rData.mSceneCounter - mStartingFrame];
    pProfilingOutput->mSceneCounter       = rData.mSceneCounter;
    std::copy(rData.mTicks, rData.mTicks + gcApexCvEndTick, pProfilingOutput->mTicks);
    libResult = LIB_SUCCESS;
  }
  return libResult;
}

LIB_RESULT GdcFt_DisplayTask::DumpProfilingInFile(char* profilingoutputFilename, uint8_t* pData)
{
  FILE*          pPtr;
  LIB_RESULT     libResult       = LIB_FAILURE;
  size_t         elementsWritten = 0;
  const uint32_t cTimeDivisor    = 1000; // In linux values are in nanoseconds

  GdcFt_ProfilingData* pOutputBuf = (GdcFt_ProfilingData*)pData;

  char writeBuffer[256];
  if((nullptr != profilingoutputFilename) && (nullptr != pOutputBuf))
  {
    pPtr = fopen(profilingoutputFilename, "w");
    if(nullptr != pPtr)
    {
      if(strstr(mOutputfileFormat, "txt"))
      {
        sprintf(writeBuffer, "--- ft profiling (usecond)---\n");
        elementsWritten = fwrite(writeBuffer, strlen(writeBuffer), 1, pPtr);
        sprintf(writeBuffer,
                "| frame |  SceneTask  | DetectorTask | pyramidTask  | trackerTask  |  CompTask   | DisplayTask  |\n");
        elementsWritten = fwrite(writeBuffer, strlen(writeBuffer), 1, pPtr);
        for(uint32_t index = 0; index < mSamplingDuration; index++)
        {
          sprintf(writeBuffer, "| %5d |  %8zu   |  %8zu    |  %8zu   |  %8zu   |  %8zu   |  %8zu   |\n",
                  pOutputBuf->mSceneCounter,
                  (pOutputBuf->mTicks[gcSceneTick + 1] - pOutputBuf->mTicks[gcSceneTick]) / cTimeDivisor,
                  (pOutputBuf->mTicks[gcDetectorTick + 3] - pOutputBuf->mTicks[gcDetectorTick]) / cTimeDivisor,
                  (pOutputBuf->mTicks[gcPyramidTick + 3] - pOutputBuf->mTicks[gcPyramidTick]) / cTimeDivisor,
                  (pOutputBuf->mTicks[gcTrackerTick + 3] - pOutputBuf->mTicks[gcTrackerTick]) / cTimeDivisor,
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
        const uint32_t cNumberOfTasks               = 6;
        const char     cStrings[cNumberOfTasks][64] = {"scene",       "DetectorTask", "pyramidTask",
                                                   "trackerTask", "CompTask",     "DisplayTask"};
        const uint32_t cCounts[cNumberOfTasks]      = {gcDetectorTick - gcSceneTick,  gcPyramidTick - gcDetectorTick,
                                                  gcTrackerTick - gcPyramidTick, gcCompTick - gcTrackerTick,
                                                  gcDisplayTick - gcCompTick,    gcApexCvEndTick - gcDisplayTick};

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
LIB_RESULT GdcFt_DisplayTask::HandleDetectorData(GdcFt_DetectorData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(rData.mSceneRgb24_umat.empty() == false)
  {
    GdcFt_CompositionData lCompositionData;

    lCompositionData.mSceneRgb24_umat = rData.mSceneRgb24_umat;

    libResult = mDisplay.PutFrame(lCompositionData);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = GdcFt_Helper::FreeDetectorData(&rData);
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_DisplayTask::HandlePyramidData(GdcFt_PyramidData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(rData.mSceneRgb24_umat.empty() == false)
  {
    GdcFt_CompositionData lCompositionData;

    lCompositionData.mSceneRgb24_umat = rData.mSceneRgb24_umat;

    libResult = mDisplay.PutFrame(lCompositionData);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = GdcFt_Helper::FreePyramidData(&rData);
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }

  return libResult;
}
