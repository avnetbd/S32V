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
#include "gdc_cnn_task_classifier.hpp"
#include "GDC_OAL_Semaphore.hpp"
#include "iniparser.h"
#include "communications.hpp"
#include "global_errors.h"
#include <stdint.h>
#include <new>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcCnn_ClassifierTask::GdcCnn_ClassifierTask()
    : mReceiveMessageQueueId(0), mCompositionTaskMessageQueueId(0), mpSqueezeNet_WeightModel(0),
      mIsSqueezeNetInitialized(false),
      //apexcv::SqueezeV10Net mSqueezeNet;
      //std::vector<std::string> mSqueezeNet_SynonymSet;
      //std::vector<int32_t> mReferenceResults;
      mWeCompareWithReference(false), mIsTaskClosing(false), mIsConfigured(false)
{
}

/***************************************************************************/
/***************************************************************************/
GdcCnn_ClassifierTask::~GdcCnn_ClassifierTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_ClassifierTask::Configure(const int32_t cReceiveMessageQueueId,
                                            const int32_t cDisplayTaskMessageQueueId,
                                            const bool    cWeCompareWithReference)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId         = cReceiveMessageQueueId;
    mCompositionTaskMessageQueueId = cDisplayTaskMessageQueueId;
    mWeCompareWithReference        = cWeCompareWithReference;
    mIsConfigured                  = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_ClassifierTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  std::string lSqueezeNetModel_Filename;
  std::string lSqueezeNetSynonymSet_Filename;
  std::string lSqueezeNetReferenceResults_Filename;

  dictionary* pDictionary = 0;
  ;

  pDictionary = iniparser_load(gcIniFileName);

  if(pDictionary != 0)
  {
    const uint32_t cLenMax           = 256;
    char           lCString[cLenMax] = "";
    char*          pString           = 0;

    strncpy(lCString, gcSqueezeNetModel_Filename, cLenMax);
    pString                   = iniparser_getstring(pDictionary, "ClassifierTask:WeightModelFilename", lCString);
    lSqueezeNetModel_Filename = pString;

    strncpy(lCString, gcSqueezeNetSynonymSet_Filename, cLenMax);
    pString                        = iniparser_getstring(pDictionary, "ClassifierTask:SynonymSetFilename", lCString);
    lSqueezeNetSynonymSet_Filename = pString;

    strncpy(lCString, gcSqueezeNetReferenceResults_Filename, cLenMax);
    pString = iniparser_getstring(pDictionary, "ClassifierTask:ReferenceResultsFilename", lCString);
    lSqueezeNetReferenceResults_Filename = pString;

    iniparser_freedict(pDictionary);
    pDictionary = 0;
  }
  else
  {
    lSqueezeNetModel_Filename            = gcSqueezeNetModel_Filename;
    lSqueezeNetSynonymSet_Filename       = gcSqueezeNetSynonymSet_Filename;
    lSqueezeNetReferenceResults_Filename = gcSqueezeNetReferenceResults_Filename;
  }

  libResult = GdcCnn_Helper::ReadWeightModelFromFile(lSqueezeNetModel_Filename.c_str(), mpSqueezeNet_WeightModel);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  libResult = GdcCnn_Helper::ReadSynonymSetFromFile(lSqueezeNetSynonymSet_Filename.c_str(), mSqueezeNet_SynonymSet);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  if(mWeCompareWithReference == true)
  {
    libResult =
        GdcCnn_Helper::ReadReferenceResultsFromFile(lSqueezeNetReferenceResults_Filename.c_str(), mReferenceResults);
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcCnn_ClassifierTask::TaskService()
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
        case MESSAGE_ID_GDC_RESIZE_DATA:
        {
          GdcCnn_ResizeData* pData = (GdcCnn_ResizeData*)pMessage;

          libResult = GdcCnn_ClassifierTask::HandleResizeData(*pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          mIsTaskClosing = !(libResult == LIB_SUCCESS);

          pData->~GdcCnn_ResizeData();
          break;
        }
        case MESSAGE_ID_GDC_CNN_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult = GdcCnn_Helper::SendRequest(MESSAGE_ID_GDC_CNN_EXIT_REQUEST, mReceiveMessageQueueId,
                                                 mCompositionTaskMessageQueueId, pMessage->mPriority);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          mIsTaskClosing = true;

          break;
        }
        case MESSAGE_ID_GDC_CNN_RESET_REQUEST:
        {
          libResult = GdcCnn_Helper::SendRequest(MESSAGE_ID_GDC_CNN_RESET_REQUEST, mReceiveMessageQueueId,
                                                 mCompositionTaskMessageQueueId);
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
LIB_RESULT GdcCnn_ClassifierTask::TaskClose()
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
        case MESSAGE_ID_GDC_RESIZE_DATA:
        {
          GdcCnn_ResizeData* pData = (GdcCnn_ResizeData*)pMessage;

          libResult = GdcCnn_Helper::FreeResizeData(pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          pData->~GdcCnn_ResizeData();
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
LIB_RESULT GdcCnn_ClassifierTask::HandleResizeData(GdcCnn_ResizeData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;
  int        intResult = -1;

  rData.mTicks[gcClassifierTick] = FSL_Ticks();

  if(rData.mPaddedScene_umat.empty() == false)
  {
    vsdk::UMat lPaddedScene_umat(
        rData.mPaddedScene_umat,
        vsdk::Rect(0, gcPadding_TopInBytes, gcPaddedScene_WidthInPixels, gcSceneHeightInPixels));

    if(mIsSqueezeNetInitialized == false)
    {
#if SQUEEZENET11
      intResult = mSqueezeNet.Initialize(mpSqueezeNet_WeightModel);
      SHOW_ERROR(intResult == 0);
#else
      intResult = mSqueezeNet.Initialize(mpSqueezeNet_WeightModel, lPaddedScene_umat);
      SHOW_ERROR(intResult == 0);
#endif

      mIsSqueezeNetInitialized = true;
    }
#if !SQUEEZENET11
    else
    {
      intResult = mSqueezeNet.ReconnectIO(lPaddedScene_umat);
      SHOW_ERROR(intResult == 0);
    }
#else
    GdcCnn_Helper::transformUMat(lPaddedScene_umat, mSqueezeNet.net_input_tensor_);
#endif

    uint16_t lSqueezeNet_Top5[2 * gcSqueezeNet_NumberOfClassifications] = {0};

    rData.mTicks[gcClassifierTick + 1] = FSL_Ticks();
    intResult                          = mSqueezeNet.Process(lSqueezeNet_Top5);
    SHOW_ERROR(intResult == 0);
    rData.mTicks[gcClassifierTick + 2] = FSL_Ticks();

    uint16_t    lClassificationIndexes_Local[gcSqueezeNet_NumberOfClassifications] = {0};
    std::string lClassificationLabels_Local[gcSqueezeNet_NumberOfClassifications];
    float       lClassificationScores_Local[gcSqueezeNet_NumberOfClassifications] = {0.0};

    for(uint32_t i = 0; i < gcSqueezeNet_NumberOfClassifications; ++i)
    {
      lClassificationIndexes_Local[i] = lSqueezeNet_Top5[i * 2];

      if(lClassificationIndexes_Local[i] < mSqueezeNet_SynonymSet.size())
      {
        lClassificationLabels_Local[i] = mSqueezeNet_SynonymSet[lClassificationIndexes_Local[i]];
      }
      else
      {
        lClassificationLabels_Local[i] = "Unknown, Could not be found, Absent from Synonym Set";
      }

      lClassificationScores_Local[i] = (float)(lSqueezeNet_Top5[(i * 2) + 1]) / 65536.0; // Magic Number Bad!!!!!
    }

    int32_t     lClassificationIndex_Reference = -1;
    std::string lClassificationLabel_Reference;

    if(mWeCompareWithReference == true)
    {
      std::map< std::string, std::set< int32_t > >::iterator lIterator = mReferenceResults.find(rData.mSceneName);

      if(lIterator != mReferenceResults.end())
      {
        std::set< int32_t >&          rSet = lIterator->second;
        std::set< int32_t >::iterator lIterator;

        bool weFoundMatch = false;

        for(uint32_t i = 0; i < gcSqueezeNet_NumberOfClassifications; ++i)
        {
          lIterator = rSet.find(lClassificationIndexes_Local[i]);

          if(lIterator != rSet.end())
          {
            lClassificationIndex_Reference = (*lIterator);
            weFoundMatch                   = true;
            break;
          }
        }

        if(weFoundMatch == false)
        {
          lClassificationIndex_Reference = *(rSet.begin());
        }

        if(lClassificationIndex_Reference >= 0 &&
           (uint32_t)lClassificationIndex_Reference < mSqueezeNet_SynonymSet.size())
        {
          lClassificationLabel_Reference = mSqueezeNet_SynonymSet[lClassificationIndex_Reference];
        }
        else
        {
          lClassificationLabel_Reference = "Unknown, Could not be found, Absent from Synonym Set";
        }
      }
      else
      {
        lClassificationLabel_Reference = "Unknown, Could not be found, Absent from reference results";
      }
    }

    rData.mTicks[gcClassifierTick + 3] = FSL_Ticks();

    libResult = GdcCnn_Helper::SendClassifierData(mCompositionTaskMessageQueueId, rData.mSceneMat, rData.mSceneCounter,
                                                  lClassificationIndexes_Local, lClassificationLabels_Local,
                                                  lClassificationScores_Local, lClassificationIndex_Reference,
                                                  lClassificationLabel_Reference, rData.mTicks);
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }

  return libResult;
}
