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
#include "gdc_pd_resize_task.hpp"
#include "iniparser.h"
#include "GDC_OAL_Semaphore.hpp"
#include "communications.hpp"
#include "global_errors.h"
#include "time_helper.h"
#include <stdlib.h>
#include <stdint.h>
#include <new>

/***************************************************************************/
/* Local Literals & definitions */
int image_index = 0;

/***************************************************************************/
/***************************************************************************/
GdcPd_ResizeTask::GdcPd_ResizeTask()
    : mReceiveMessageQueueId(0), mHogTaskMessageQueueId(0), mNumberOfSceneResizes(0),
      //GdcPd_FrameSize mSceneResizes[gcNumberOfSceneResizes];
      //apexcv::Resize          mResize;
      //apexcv::PyramidCreation mPyramidCreation;
      //ref::Resize         mResizeRef;
      //vsdk::UMat  mSceneResize_umat[gcNumberOfSceneResizes];
      //mPyramidScales[gcNumberOfMajorScales];
      mWeUseApexResize(false), mIsTaskClosing(false), mIsConfigured(false), mIsResizeInitialized(false)
{
  memset(mpSceneResize_umat_data, 0, sizeof(mpSceneResize_umat_data));
  memset(mPyramidScales, 0, sizeof(mPyramidScales));
}

/***************************************************************************/
/***************************************************************************/
GdcPd_ResizeTask::~GdcPd_ResizeTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_ResizeTask::Configure(const int32_t                cReceiveMessageQueueId,
                                       const int32_t                cHogTaskMessageQueueId,
                                       const uint32_t               cNumberOfSceneResizes,
                                       const GdcPd_FrameSize* const cpcSceneResizes,
                                       uint32_t                     pyramidScales[])
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false && cNumberOfSceneResizes != 0 && cpcSceneResizes != 0)
  {
    mReceiveMessageQueueId = cReceiveMessageQueueId;
    mHogTaskMessageQueueId = cHogTaskMessageQueueId;
    mNumberOfSceneResizes  = cNumberOfSceneResizes;

    memcpy(mPyramidScales, pyramidScales, sizeof(uint32_t) * gcNumberOfMajorScales);

    if(cNumberOfSceneResizes <= gcNumberOfSceneResizes)
    {
      for(uint32_t i = 0; i < cNumberOfSceneResizes; ++i)
      {
        mSceneResizes[i] = cpcSceneResizes[i];
      }

      mIsConfigured = true;

      libResult = LIB_SUCCESS;
    }
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_ResizeTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  if(mIsConfigured == true)
  {
    dictionary* pDictionary       = 0;
    uint32_t    major_scale_index = 0;
    uint32_t    minor_scale_index = gcNumberOfMajorScales;

    pDictionary = iniparser_load(gcIniFileName);

    if(pDictionary != 0)
    {
      mWeUseApexResize = (iniparser_getint(pDictionary, "App:WeUseApexResize", 1) == 1);

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }

    while(major_scale_index < gcNumberOfMajorScales)
    {
      int width  = gcMajorSceneResizes[major_scale_index][0];
      int height = gcMajorSceneResizes[major_scale_index][1];
      // Allocate padded buffer for resize output
      mSceneResize_umat[major_scale_index] =
          vsdk::UMat(vsdk::UMat(height + 32, (width + 127) / 128 * 128 + 32, CV_8UC1), vsdk::Rect(0, 0, width, height));

      for(uint32_t i = 1; i < mPyramidScales[major_scale_index]; i++)
      {
        vsdk::UMat lSceneResize_umat_temp2(gcMajorSceneResizes[major_scale_index][1] >> i,
                                           mSceneResizes[minor_scale_index].mSpan, CV_8UC1);
        vsdk::Rect lRect1(0, 0, gcMajorSceneResizes[major_scale_index][0] >> i,
                          gcMajorSceneResizes[major_scale_index][1] >> i);

        vsdk::UMat lSceneResize_umat_temp3(lSceneResize_umat_temp2, lRect1);

        mSceneResize_umat[minor_scale_index] = lSceneResize_umat_temp3;

        minor_scale_index++;
      }
      major_scale_index++;
    }
    mPyramidCreation.Initialize(mSceneResize_umat[0], mSceneResize_umat[gcNumberOfMajorScales]);
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_ResizeTask::TaskService()
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
        case MESSAGE_ID_GDC_PD_IMAGE_REPLY:
        {
          GdcPd_ImageReply* pReply = (GdcPd_ImageReply*)pMessage;

          libResult = GdcPd_ResizeTask::HandleImageReply(*pReply);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          mIsTaskClosing = !(libResult == LIB_SUCCESS);

          pReply->~GdcPd_ImageReply();
          break;
        }
        case MESSAGE_ID_GDC_PD_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult =
              GdcPd_Helper::SendRequest(MESSAGE_ID_GDC_PD_EXIT_REQUEST, mReceiveMessageQueueId, mHogTaskMessageQueueId);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          mIsTaskClosing = true;

          break;
        }
        case MESSAGE_ID_GDC_PD_RESET_REQUEST:
        {
          libResult = GdcPd_Helper::SendRequest(MESSAGE_ID_GDC_PD_RESET_REQUEST, mReceiveMessageQueueId,
                                                mHogTaskMessageQueueId);
          SHOW_ERROR(libResult == LIB_SUCCESS);
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
LIB_RESULT GdcPd_ResizeTask::TaskClose()
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
        case MESSAGE_ID_GDC_PD_IMAGE_REPLY:
        {
          GdcPd_ImageReply* pReply = (GdcPd_ImageReply*)pMessage;

          libResult = GdcPd_Helper::FreeImageReply(pReply);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          pReply->~GdcPd_ImageReply();
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
LIB_RESULT GdcPd_ResizeTask::HandleImageReply(GdcPd_ImageReply& rReply)
{
  LIB_RESULT libResult = LIB_FAILURE;
  int        intResult = -1;

  uint64_t         ticks_per[gcNumberOfSceneResizes][2] = {{0}};
  AcfProfilingInfo acfProfilingInfo_per[gcNumberOfSceneResizes];

  uint32_t major_scale_index = 0;
  uint32_t minor_scale_index = gcNumberOfMajorScales;

  if(rReply.mSceneRgb32_umat.empty() == false)
  {
    /****************************************************************************
      * Setting Up Buffers
      ****************************************************************************/
    while(major_scale_index < gcNumberOfMajorScales)
    {
      int width  = gcMajorSceneResizes[major_scale_index][0];
      int height = gcMajorSceneResizes[major_scale_index][1];
      // Allocate padded buffer for resize output
      mSceneResize_umat[major_scale_index] =
          vsdk::UMat(vsdk::UMat(height + 32, (width + 127) / 128 * 128 + 32, CV_8UC1), vsdk::Rect(0, 0, width, height));

      for(uint32_t i = 1; i < mPyramidScales[major_scale_index]; i++)
      {
        vsdk::UMat lSceneResize_umat_temp2(gcMajorSceneResizes[major_scale_index][1] >> i,
                                           mSceneResizes[minor_scale_index].mSpan, CV_8UC1);
        vsdk::Rect lRect1(0, 0, gcMajorSceneResizes[major_scale_index][0] >> i,
                          gcMajorSceneResizes[major_scale_index][1] >> i);

        vsdk::UMat lSceneResize_umat_temp3(lSceneResize_umat_temp2, lRect1);

        mSceneResize_umat[minor_scale_index] = lSceneResize_umat_temp3;

        minor_scale_index++;
      }

      major_scale_index++;
    }

    rReply.mTicks[gcApexCvResizeTick] = FSL_Ticks();

    /****************************************************************************
      * Run Resize procedures
      ****************************************************************************/
    for(uint32_t i = 0; i < gcNumberOfMajorScales; ++i)
    {
      if(!(rReply.mSceneRgb32_umat.cols == mSceneResize_umat[i].cols &&
           rReply.mSceneRgb32_umat.rows == mSceneResize_umat[i].rows))
      {
        if(!(rReply.mSceneRgb32_umat.cols == mSceneResize_umat[i].cols &&
             rReply.mSceneRgb32_umat.rows == mSceneResize_umat[i].rows))
        {
          //SHOW_ERROR(mSceneResize_umat[i].empty() != true);
          if(mWeUseApexResize == true)
          {
            AcfProfilingInfo lAcfProfilingInfo;
            uint32_t         lStart;
            uint32_t         lStop;
            intResult = mResize.SelectApuConfiguration(gcApexCvResizeAcfApuCfg, gcApexCvResizeApexId);
            SHOW_ERROR(intResult == 0);

            ticks_per[i][0] = FSL_Ticks();
            if(mIsResizeInitialized == false)
            {
              intResult = mResize.Initialize(rReply.mSceneRgb32_umat, mSceneResize_umat[i]);
              SHOW_ERROR(intResult == 0);
              mIsResizeInitialized = true;
            }
            else
            {
              intResult = mResize.ReconnectIO(rReply.mSceneRgb32_umat, mSceneResize_umat[i]);
              SHOW_ERROR(intResult == 0);
            }

            if(0 == intResult)
            {
              lStart    = FSL_Ticks();
              intResult = mResize.Process();
              lStop     = FSL_Ticks();
              SHOW_ERROR(intResult == 0);
            }

            ticks_per[i][1]             = FSL_Ticks();
            lAcfProfilingInfo.apu_total = CONVERT_TICKS_TO_US(lStop - lStart);
            acfProfilingInfo_per[i]     = lAcfProfilingInfo;
          }
          else if(mWeUseApexResize == false) // Means use reference
          {
            intResult = mResizeRef.process(rReply.mSceneRgb32_umat, mSceneResize_umat[i]);
            SHOW_ERROR(intResult == 0);
          }
        }
      }
    }

    /****************************************************************************
      * No change in dimensions - Just copy the data line by line
      ****************************************************************************/
    for(uint32_t i = 0; i < gcNumberOfMajorScales; ++i)
    {
      if(rReply.mSceneRgb32_umat.cols == mSceneResize_umat[i].cols &&
         rReply.mSceneRgb32_umat.rows == mSceneResize_umat[i].rows)
      {
        // SHOW_ERROR(mSceneResize_umat[i].empty() != false);
        vsdk::Mat srcMat = rReply.mSceneRgb32_umat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
        vsdk::Mat dstMat = mSceneResize_umat[i].getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
        uint8_t*  pSrc   = srcMat.data;
        uint8_t*  pDst   = dstMat.data;

        for(int row = 0; row < rReply.mSceneRgb32_umat.rows; ++row)
        {
          memcpy(pDst, pSrc, rReply.mSceneRgb32_umat.cols * gcSceneBitsPerPixel / 8);
          pSrc += rReply.mSceneRgb32_umat.step[0];
          pDst += mSceneResize_umat[i].step[0];
        }
      }

      GdcPd_Helper::FillResizeUMatPaddingWithZeroes(mSceneResize_umat[i]);
    }

    /****************************************************************************
      * Perform Pyramid Procedures
      ****************************************************************************/
    major_scale_index = 0;
    minor_scale_index = gcNumberOfMajorScales;

    while(major_scale_index < gcNumberOfMajorScales)
    {
      if(mPyramidScales[major_scale_index] > 1)
      {
        ticks_per[minor_scale_index][0] = FSL_Ticks();

        mPyramidCreation.ReconnectIO(mSceneResize_umat[major_scale_index], mSceneResize_umat[minor_scale_index]);
        mPyramidCreation.SelectApexCore(gcApexCvResizeApexId);
        mPyramidCreation.Process();

        ticks_per[minor_scale_index][1] = FSL_Ticks();

        minor_scale_index++;

        for(uint32_t i = 2; i < mPyramidScales[major_scale_index]; i++)
        {
          ticks_per[minor_scale_index][0] = FSL_Ticks();

          mPyramidCreation.ReconnectIO(mSceneResize_umat[minor_scale_index - 1], mSceneResize_umat[minor_scale_index]);
          mPyramidCreation.SelectApexCore(gcApexCvResizeApexId);
          mPyramidCreation.Process();

          ticks_per[minor_scale_index][1] = FSL_Ticks();

          minor_scale_index++;
        }
      }
      major_scale_index++;
    }

    rReply.mTicks[gcApexCvResizeTick + 1] = FSL_Ticks();

    if(mWeUseApexResize == false)
    {
      rReply.mTicks[gcApexCvResizeTick]     = 0;
      rReply.mTicks[gcApexCvResizeTick + 1] = 0;
    }

    for(uint32_t i = 0; i < gcNumberOfSceneResizes; ++i)
    {
      mpSceneResize_umat_data[i] =
          (void*)(((uintptr_t)mSceneResize_umat[i].getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data)); // Debug only
    }

    libResult = GdcPd_Helper::SendResizeData(
        mHogTaskMessageQueueId, rReply.mSceneRgb24_umat,
        rReply.mSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
        rReply.mSceneRgb32_umat,
        rReply.mSceneRgb32_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
        rReply.mSceneCounter, mNumberOfSceneResizes, mSceneResize_umat, mpSceneResize_umat_data, rReply.mTicks,
        ticks_per, acfProfilingInfo_per);
  }
  return libResult;
}
