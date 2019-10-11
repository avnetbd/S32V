/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2015 Freescale Semiconductor;
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
#include "gdc_pd_task_aggcf_luv.hpp"
#include "iniparser.h"
#include "GDC_OAL_Semaphore.hpp"
#include "communications.hpp"
#include "global_errors.h"
#include <stdlib.h>
#include <stdint.h>
#include <new>
//#include <apexcv_aggcf_test.h>
//#define DUMP_PROFILING_TO_FILE
#define AGGCF_APPR_PROFILING "aggcf_appr_profiling.csv"
//#define DUMP_IMAGES

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcPd_AggcfLuvTask::GdcPd_AggcfLuvTask()
    : mReceiveMessageQueueId(0), mRealScaleTaskMessageQueueId(0),
      //GdcPd_FrameSize     mSceneSize;
      //std::vector<apex::bbs> mPedestrianRois;
      mWeUseApexAggcf(true), mIsScaledataReady(0), mIsTaskClosing(false), mIsConfigured(false),
      //mpOut_py_umat[gcInputImageQueueSize],
      mProcessNum(0)
{
}

/***************************************************************************/
/***************************************************************************/
GdcPd_AggcfLuvTask::~GdcPd_AggcfLuvTask()
{
  //
  // Due to multiple tasks accessing "mpOut_py_umat" concurrently,
  // it CANNOT be freed in "TaskClose".
  // Until the design is revised, this resource freeing needs to happen in destructor.
  //
  for(uint32_t i = 0; i < gcInputImageQueueSize; i++)
  {
    mAcf_detector.DeInitPyramidBuf(mpOut_py_umat[i]);
  }
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_AggcfLuvTask::Configure(const int32_t         cReceiveMessageQueueId,
                                         const int32_t         cRealScaleTaskMessageQueueId,
                                         const GdcPd_FrameSize cpcScene)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId       = cReceiveMessageQueueId;
    mRealScaleTaskMessageQueueId = cRealScaleTaskMessageQueueId;
    mSceneSize.mHeight           = cpcScene.mHeight;
    mSceneSize.mWidth            = cpcScene.mWidth;
    mSceneSize.mSpan             = cpcScene.mSpan;

    mIsConfigured = true;
    libResult     = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_AggcfLuvTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  if(mIsConfigured == true)
  {
    /****************************************************************************
        * initialize detector done in featureTask
        ****************************************************************************/
    const uint32_t cFileNameSize            = 256;
    char           cFileName[cFileNameSize] = {0};

    strncpy(cFileName, gcDataPath, gcFileNameSize);

    dictionary* pDictionary = 0;
    pDictionary             = iniparser_load(gcIniFileName);

    if(pDictionary != 0)
    {
      strncat(cFileName, iniparser_getstring_const(pDictionary, "DATA:Filename", gcFilenametraining),
              gcFileNameSize); // Bad should reduce by amount of char in gcDataPath

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }
    else
    {
      strncat(cFileName, gcFilenametraining,
              gcFileNameSize); // Bad should reduce by amount of char in gcDataPath
    }

    mAcf_detector.InitDetectorModel(cFileName);
    if(mAcf_detector.IsDetectorModelFailToLoaded() == true)
    {
      libResult = 1;
      printf("%s::could not load detector model %s\n", __FILE__, cFileName);
    }

    mAcf_detector.CalcScaleParameters(mSceneSize.mWidth, mSceneSize.mHeight);

    for(uint32_t i = 0; i < gcInputImageQueueSize; i++)
    {
      mpOut_py_umat[i] = mAcf_detector.InitPyramidBuf();
    }
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_AggcfLuvTask::TaskService()
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

          libResult = GdcPd_AggcfLuvTask::HandleImageReply(*pReply);
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

          libResult = GdcPd_Helper::SendRequest(MESSAGE_ID_GDC_PD_EXIT_REQUEST, mReceiveMessageQueueId,
                                                mRealScaleTaskMessageQueueId, pMessage->mPriority);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          mIsTaskClosing = true;

          break;
        }
        case MESSAGE_ID_GDC_PD_RESET_REQUEST:
        {
          libResult = GdcPd_Helper::SendRequest(MESSAGE_ID_GDC_PD_RESET_REQUEST, mReceiveMessageQueueId,
                                                mRealScaleTaskMessageQueueId);
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
LIB_RESULT GdcPd_AggcfLuvTask::TaskClose()
{
  GDC_TRACE;
  LIB_RESULT libResult = LIB_FAILURE;

  //
  // Free resources owned by the task
  //
  //
  // Due to multiple tasks accessing "mpOut_py_umat" concurrently,
  // it CANNOT be freed in "TaskClose".
  // Until the design is revised, this resource freeing needs to happen in destructor.
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
LIB_RESULT GdcPd_AggcfLuvTask::HandleImageReply(GdcPd_ImageReply& rImageReply)
{
  LIB_RESULT libResult = LIB_FAILURE;

  uint64_t         ticks[gcApexCvEndTick] = {0};
  AcfProfilingInfo acfProfilingInfo_per[gcNumberOfRealScales];

  // mSceneSize.mWidth should be 640
  // mSceneSize.mHeight should be 480

  if(rImageReply.mSceneRgb32_umat.empty() == false)
  {
    ticks[0] = rImageReply.mTicks[0];
    ticks[1] = rImageReply.mTicks[1];

    ticks[gcApexCvLUVTick] = FSL_Ticks();

    /****************************************************************************
      * detection
      ****************************************************************************/

    // convert RGB input to LUV components
    vsdk::UMat lSceneLuv_umat;
    libResult = mAcf_detector.ConvertRGBtoLUV(rImageReply.mSceneRgb32_umat, lSceneLuv_umat, mSceneSize.mWidth,
                                              mSceneSize.mHeight, gcApexCvLuvApexId);

    // mAcf_detector.calcChannelsPyramid(dst_luv, mpOut_py_umat[mProcessNum]);

    /****************************************************************************
      * No change in dimensions - Just copy the data
      ****************************************************************************/

    ticks[gcApexCvLUVTick + 1] = FSL_Ticks();

    libResult = GdcPd_Helper::SendLUVData(
        mRealScaleTaskMessageQueueId, rImageReply.mSceneRgb24_umat,
        rImageReply.mSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
        rImageReply.mSceneCounter, lSceneLuv_umat, mpOut_py_umat[mProcessNum], ticks, acfProfilingInfo_per);

    mProcessNum++;
    if(mProcessNum >= gcInputImageQueueSize)
      mProcessNum = 0;
  }
  return libResult;
}
