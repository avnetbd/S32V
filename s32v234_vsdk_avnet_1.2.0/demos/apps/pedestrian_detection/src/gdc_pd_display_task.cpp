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
#include "gdc_pd_display_task.hpp"
#include "gdc_pd_messages.hpp"
#include "communications.hpp"
#include "iniparser.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "global_errors.h"
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcPd_DisplayTask::GdcPd_DisplayTask()
    : mReceiveMessageQueueId(0), mSceneTaskMessageQueueId(0), mIsTaskClosing(false), mIsConfigured(false),
      mDisplayWidthInPixels(0), mDisplayHeightInPixels(0), mDisplayBitsPerPixel(0), mWeRotateDisplay(false),
      mpSceneRgb24_Rotated(0), mSceneRgb24_Rotated_SizeInBytes(0), mpSceneRgb24_Resized(0),
      mSceneRgb24_Resized_SizeInBytes(0)
//io_FrameOutput_t mFrameOutput;
{
}

/***************************************************************************/
/***************************************************************************/
GdcPd_DisplayTask::~GdcPd_DisplayTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_DisplayTask::Configure(const int32_t cReceiveMessageQueueId, const int32_t cSceneTaskMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId   = cReceiveMessageQueueId;
    mSceneTaskMessageQueueId = cSceneTaskMessageQueueId;
    mIsConfigured            = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_DisplayTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  mDisplayWidthInPixels  = gcDisplayWidthInPixels;
  mDisplayHeightInPixels = gcDisplayHeightInPixels;
  mDisplayBitsPerPixel   = gcDisplayBitsPerPixel;
  mWeRotateDisplay       = gcDisplay_WeRotateDisplay;

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
        strncpy(mOuputFileFormat, iniparser_getstring(pDictionary, "Profiling:OutputFilFormat", "csv"), 3);
        printf("output file name = %s, %s\n", mOutputFilename, mOuputFileFormat);
        // allocate output data buffer
        mOutputDataUMat = vsdk::UMat(1, mSamplingDuration * sizeof(GdcPd_ProfilingData), CV_8UC1);
        mOutputDataMat  = mOutputDataUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
      }

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }

    mSceneRgb24_Resized_SizeInBytes =
        (mDisplayWidthInPixels * mDisplayHeightInPixels * mDisplayBitsPerPixel) / 8; // bits per byte

    mpSceneRgb24_Resized = malloc(mSceneRgb24_Resized_SizeInBytes);

    if(mpSceneRgb24_Resized != 0)
    {
      //
      // io::FrameOutputV234Fb
      //
      mFrameOutput.Init(mDisplayWidthInPixels, mDisplayHeightInPixels, gcDisplayIoDataDepth, gcDisplayIoDataChannels);

      //
      // This makes the input image task send messages into the application data path.
      // The input image task could be an: image sensor, file reader...
      //
      for(uint32_t i = 0; i < gcSceneTask_ImageRequests; ++i)
      {
        libResult = GdcPd_Helper::SendRequest(MESSAGE_ID_GDC_PD_IMAGE_REQUEST, mReceiveMessageQueueId,
                                              mSceneTaskMessageQueueId);
        SHOW_ERROR(libResult == LIB_SUCCESS);
      }

      libResult = LIB_SUCCESS;
    }
    else
    {
      mIsTaskClosing = true;
      libResult      = LIB_FAILURE;
    }
  }

  GDC_TRACE;

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_DisplayTask::TaskService()
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
        case MESSAGE_ID_GDC_PD_HOG_DATA:
        {
          //
          // Request a new image to continue the cycle
          //
          libResult = GdcPd_Helper::SendRequest(MESSAGE_ID_GDC_PD_IMAGE_REQUEST, mReceiveMessageQueueId,
                                                mSceneTaskMessageQueueId);
          SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_SENDING_NOT_ALLOWED);

          //
          // Request to send image to the display
          //
          GdcPd_HogData* pData = (GdcPd_HogData*)pMessage; // Placement new

          libResult = GdcPd_DisplayTask::HandleCompositionData(*pData);

          pData->~GdcPd_HogData();
          break;
        }
        case MESSAGE_ID_GDC_PD_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
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

/***************************************************************************
 **************************************************************************/
LIB_RESULT GdcPd_DisplayTask::TaskClose()
{
  GDC_TRACE;
  LIB_RESULT libResult = LIB_FAILURE;

  //
  // Free resources owned by the task
  //
  if(mpSceneRgb24_Rotated != 0)
  {
    free(mpSceneRgb24_Rotated);
    mpSceneRgb24_Rotated            = 0;
    mSceneRgb24_Rotated_SizeInBytes = 0;
  }

  if(mpSceneRgb24_Resized != 0)
  {
    free(mpSceneRgb24_Resized);
    mpSceneRgb24_Resized            = 0;
    mSceneRgb24_Resized_SizeInBytes = 0;
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
        case MESSAGE_ID_GDC_PD_COMPOSITION_DATA:
        {
          GdcPd_HogData* pData = (GdcPd_HogData*)pMessage;

          libResult = GdcPd_Helper::FreeHogData(pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcPd_HogData();

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

LIB_RESULT GdcPd_DisplayTask::DumpProfilingInBuffer(uint8_t* pData, GdcPd_HogData& rData)
{
  LIB_RESULT     libResult    = LIB_FAILURE;
  const uint32_t cTimeDivisor = 1000; // In linux values are in nanoseconds

  if(0 <= ((int32_t)rData.mSceneCounter - (int32_t)mStartingFrame))
  {
    if(rData.mSceneCounter == mStartingFrame)
    {
      printf("--- pd profiling (usecond)---\n");
      printf("| frame |  SceneTask  | resizeTask  |   HogTask   |  CompTask   | DisplayTask  |\n");
    }
    printf("| %5d |  %8zu   |  %8zu   |  %8zu   |  %8zu   |  %8zu   |\n", rData.mSceneCounter,
           (rData.mHogTicks[gcSceneTick + 1] - rData.mHogTicks[gcSceneTick]) / cTimeDivisor,
           (rData.mHogTicks[gcApexCvResizeTick + 1] - rData.mHogTicks[gcApexCvResizeTick]) / cTimeDivisor,
           (rData.mHogTicks[gcApexCvHogTick + 1] - rData.mHogTicks[gcApexCvHogTick]) / cTimeDivisor,
           (rData.mHogTicks[gcCompTick + 1] - rData.mHogTicks[gcCompTick]) / cTimeDivisor,
           (rData.mHogTicks[gcDisplayTick + 1] - rData.mHogTicks[gcDisplayTick]) / cTimeDivisor);

    GdcPd_ProfilingData* pBuffer                = (GdcPd_ProfilingData*)pData;
    GdcPd_ProfilingData* pProfilingOutputBuffer = &pBuffer[rData.mSceneCounter - mStartingFrame];
    pProfilingOutputBuffer->mSceneCounter       = rData.mSceneCounter;
    std::copy(&(rData.mPerResizeAcfProfilingInfo[0]), &(rData.mPerResizeAcfProfilingInfo[0]) + (gcNumberOfSceneResizes),
              &(pProfilingOutputBuffer->mPerResizeAcfProfilingInfo[0]));
    std::copy(rData.mHogTicks, rData.mHogTicks + gcApexCvEndTick, pProfilingOutputBuffer->mTicks);
    std::copy(&(rData.mPerHogTicks[0][0]), &(rData.mPerHogTicks[0][0]) + (gcNumberOfSceneResizes * 2),
              &(pProfilingOutputBuffer->mPerHogTicks[0][0]));
    std::copy(&(rData.mPerHogAcfProfilingInfo[0]), &(rData.mPerHogAcfProfilingInfo[gcNumberOfSceneResizes]),
              &(pProfilingOutputBuffer->mPerHogAcfProfilingInfo[0]));
    libResult = LIB_SUCCESS;
  }
  return libResult;
}

LIB_RESULT GdcPd_DisplayTask::DumpProfilingInFile(char* profilingoutputFilename, uint8_t* profilingData)
{
  FILE*                pPtr;
  LIB_RESULT           libResult       = LIB_FAILURE;
  size_t               elementsWritten = 0;
  GdcPd_ProfilingData* pOutputBuf      = (GdcPd_ProfilingData*)profilingData;
  const uint32_t       cTimeDivisor    = 1000; // In linux values are in nanoseconds

  char writeBuffer[256];
  if((nullptr != profilingoutputFilename) && (nullptr != profilingData))
  {
    pPtr = fopen(profilingoutputFilename, "w");
    if(nullptr != pPtr)
    {
      if(strstr(mOuputFileFormat, "txt"))
      {
        sprintf(writeBuffer, "--- pd profiling (nanosecond)---\n");
        elementsWritten = fwrite(writeBuffer, strlen(writeBuffer), 1, pPtr);
        sprintf(writeBuffer, "| frame |  SceneTask  | resizeTask  |   HogTask   |  CompTask   | DisplayTask  |\n");
        elementsWritten = fwrite(writeBuffer, strlen(writeBuffer), 1, pPtr);
        for(uint32_t index = 0; index < mSamplingDuration; index++)
        {
          GdcPd_ProfilingData* pBuf = (GdcPd_ProfilingData*)&pOutputBuf[index];
          sprintf(writeBuffer, "| %5d |  %8zu   |  %8zu   |  %8zu   |  %8zu   |  %8zu   |\n", pOutputBuf->mSceneCounter,
                  (pOutputBuf->mTicks[gcSceneTick + 1] - pBuf->mTicks[gcSceneTick]) / cTimeDivisor,
                  (pBuf->mTicks[gcApexCvResizeTick + 1] - pBuf->mTicks[gcApexCvResizeTick]) / cTimeDivisor,
                  (pBuf->mTicks[gcApexCvHogTick + 1] - pBuf->mTicks[gcApexCvHogTick]) / cTimeDivisor,
                  (pBuf->mTicks[gcCompTick + 1] - pBuf->mTicks[gcCompTick]) / cTimeDivisor,
                  (pBuf->mTicks[gcDisplayTick + 1] - pBuf->mTicks[gcDisplayTick]) / cTimeDivisor);
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
        const uint32_t cNumberOfTasks               = 5;
        const char     cStrings[cNumberOfTasks][64] = {"scene", "resizeTask", "HogTask", "CompTask", "DisplayTask"};
        const uint32_t cCounts[cNumberOfTasks]      = {gcApexCvResizeTick - gcSceneTick,
                                                  gcApexCvHogTick - gcApexCvResizeTick, gcCompTick - gcApexCvHogTick,
                                                  gcDisplayTick - gcCompTick, gcApexCvEndTick - gcDisplayTick};

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
/**
 * \par Description
 * Displays the result of the LDW algorithm
 *
 **************************************************************************/
LIB_RESULT GdcPd_DisplayTask::HandleCompositionData(GdcPd_HogData& rData)
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  const uint32_t lcSceneWidthInPixels  = rData.mSceneRgb24_umat.cols;
  const uint32_t lcSceneHeightInPixels = rData.mSceneRgb24_umat.rows;
  const uint32_t lcSceneSizeInBytes = lcSceneWidthInPixels * lcSceneHeightInPixels * 3; // 3 bytes per pixel for RGB24

  rData.mHogTicks[gcDisplayTick] = FSL_Ticks();

  if(mSceneRgb24_Rotated_SizeInBytes < lcSceneSizeInBytes)
  {
    if(mpSceneRgb24_Rotated != 0)
    {
      free(mpSceneRgb24_Rotated);
    }

    mpSceneRgb24_Rotated = malloc(lcSceneSizeInBytes);

    mSceneRgb24_Rotated_SizeInBytes = lcSceneSizeInBytes;
  }

  if(mpSceneRgb24_Rotated != 0)
  {
    vsdk::Mat lSceneRgb24_mat = rData.mSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);

    void* lpSceneRgb24 = (void*)(lSceneRgb24_mat.data);
    SHOW_ERROR(lpSceneRgb24 != 0);
    void* lpDisplayRgb24 = 0;

    if(mWeRotateDisplay == false && lcSceneWidthInPixels == mDisplayWidthInPixels &&
       lcSceneHeightInPixels == mDisplayHeightInPixels)
    {
      lpDisplayRgb24 = lpSceneRgb24;
    }
    else if(mWeRotateDisplay == true && lcSceneWidthInPixels == mDisplayWidthInPixels &&
            lcSceneHeightInPixels == mDisplayHeightInPixels)
    {
      libResult = GdcPd_Helper::ReverseCopyRgb24(mpSceneRgb24_Rotated, lpSceneRgb24, lcSceneWidthInPixels,
                                                 lcSceneHeightInPixels);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      lpDisplayRgb24 = mpSceneRgb24_Rotated;
    }
    else if(mWeRotateDisplay == false &&
            (lcSceneWidthInPixels != mDisplayWidthInPixels || lcSceneHeightInPixels != mDisplayHeightInPixels))
    {
      cv::Mat lSceneMat(
          lcSceneHeightInPixels, lcSceneWidthInPixels, CV_8UC3,
          lpSceneRgb24); // The external data is not automatically deallocated, so you should take care of it.

      cv::Mat lDisplayMat(
          mDisplayHeightInPixels, mDisplayWidthInPixels, CV_8UC3,
          mpSceneRgb24_Resized); // The external data is not automatically deallocated, so you should take care of it.

      cv::Size lDisplaySize(mDisplayWidthInPixels, mDisplayHeightInPixels);

      cv::resize(lSceneMat, lDisplayMat, lDisplaySize);

      lpDisplayRgb24 = mpSceneRgb24_Resized;
    }
    else if(mWeRotateDisplay == true &&
            (lcSceneWidthInPixels != mDisplayWidthInPixels || lcSceneHeightInPixels != mDisplayHeightInPixels))
    {
      libResult = GdcPd_Helper::ReverseCopyRgb24(mpSceneRgb24_Rotated, lpSceneRgb24, lcSceneWidthInPixels,
                                                 lcSceneHeightInPixels);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      cv::Mat lSceneMat(
          lcSceneHeightInPixels, lcSceneWidthInPixels, CV_8UC3,
          mpSceneRgb24_Rotated); // The external data is not automatically deallocated, so you should take care of it.

      cv::Mat lDisplayMat(
          mDisplayHeightInPixels, mDisplayWidthInPixels, CV_8UC3,
          mpSceneRgb24_Resized); // The external data is not automatically deallocated, so you should take care of it.

      cv::Size lDisplaySize(mDisplayWidthInPixels, mDisplayHeightInPixels);

      cv::resize(lSceneMat, lDisplayMat, lDisplaySize);

      lpDisplayRgb24 = mpSceneRgb24_Resized;
    }

    if(lpDisplayRgb24 != 0)
    {
      mFrameOutput.PutFrame(lpDisplayRgb24, false);

      libResult = LIB_SUCCESS;
    }
    else
    {
      SHOW_ERROR(false);
    }
    rData.mHogTicks[gcDisplayTick + 1] = FSL_Ticks();
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
  }

  libResult = GdcPd_Helper::FreeHogData(&rData);
  SHOW_ERROR(libResult == LIB_SUCCESS);
  GDC_TRACE;

  GDC_TRACE;

  return libResult;
}
