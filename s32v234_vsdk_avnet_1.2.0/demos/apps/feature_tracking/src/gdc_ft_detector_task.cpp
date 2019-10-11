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
#include "gdc_ft_messages.hpp"
#include "gdc_ft_detector_task.hpp"
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
GdcFt_DetectorTask::GdcFt_DetectorTask()
    : mReceiveMessageQueueId(0), mPyramidTaskMessageQueueId(0),
      //apexcv::HarrisCorners  mHarrisCorners;
      //ref::HarrisCorners     mHarrisCornersRef;
      mIsGfttCornersInitialized(false), mhFeatureDescriptorOal(0),
      //icp::FeatureDescriptor mFeatureDescriptor;
      mGfttCorners_UseHarris(0), mHarrisCornersQualityLevel(0), mHarrisCornersMinDistance(0),
      mHarrisCornersCornerCoef(0), mHarrisCornersThreshold(0), mHarrisCornersBoxSize(0), mHarrisCornersNmsRadius(0),
      mHarrisCornersScaleFactor(0), mHarrisCornersMinCorners(0), mHarrisCornersMaxCorners(0),
      mWeUseApexGfttCorners(true),
      //GdcFt_OutputFile_HarrisCorners  mOutputFile;
      //char    mOutputReadFilename[gcFileNameSize+1];
      //char    mOutputWriteFilename[gcFileNameSize+1];
      mWeReadOutputFile(false), mWeWriteOutputFile(false), mReadMismatchCounter(0), mIsTaskClosing(false),
      mIsConfigured(false)
{
  memset(mOutputReadFilename, 0, sizeof(mOutputReadFilename));
  memset(mOutputWriteFilename, 0, sizeof(mOutputWriteFilename));
}

/***************************************************************************/
/***************************************************************************/
GdcFt_DetectorTask::~GdcFt_DetectorTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_DetectorTask::Configure(const int32_t cReceiveMessageQueueId, const int32_t cPyramidTaskMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId     = cReceiveMessageQueueId;
    mPyramidTaskMessageQueueId = cPyramidTaskMessageQueueId;
    mIsConfigured              = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_DetectorTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  dictionary* pDictionary = 0;
  ;

  pDictionary = iniparser_load(gcIniFileName);

  if(pDictionary != 0)
  {
    mWeUseApexGfttCorners = (iniparser_getint(pDictionary, "DetectorTask:WeUseApexGfttCorners", 1) == 1);

    char* pString = 0;

    mOutputReadFilename[0] = 0;

    pString = iniparser_getstring(pDictionary, "DetectorTask:ReadFileName", 0);

    if(pString != 0 && strlen(pString) != 0)
    {
      strcat(mOutputReadFilename, pString);
    }

    mOutputWriteFilename[0] = 0;
    pString                 = iniparser_getstring(pDictionary, "DetectorTask:WriteFileName", 0);

    if(pString != 0 && strlen(pString) != 0)
    {
      strcat(mOutputWriteFilename, pString);
    }

    mWeReadOutputFile  = (iniparser_getint(pDictionary, "DetectorTask:Read", 0) == 1);
    mWeWriteOutputFile = (iniparser_getint(pDictionary, "DetectorTask:Write", 0) == 1);

    iniparser_freedict(pDictionary);
    pDictionary = 0;
  }

  libResult =
      mOutputFile.Open((mWeWriteOutputFile ? mOutputWriteFilename : 0), (mWeReadOutputFile ? mOutputReadFilename : 0));
  SHOW_ERROR(libResult == LIB_SUCCESS);

  mIsGfttCornersInitialized = false;

  mGfttCorners_UseHarris     = gcGfttCorners_UseHarris;
  mHarrisCornersQualityLevel = gcHarrisCornersQualityLevel;
  mHarrisCornersMinDistance  = gcHarrisCornersMinDistance;
  mHarrisCornersCornerCoef   = gcHarrisCornersCornerCoef;
  mHarrisCornersThreshold    = gcHarrisCornersThreshold;
  mHarrisCornersBoxSize      = gcHarrisCornersBoxSize;
  mHarrisCornersNmsRadius    = gcHarrisCornersNmsRadius;
  mHarrisCornersScaleFactor  = gcHarrisCornersScaleFactor;
  mHarrisCornersMinCorners   = gcHarrisCornersMinCorners;
  mHarrisCornersMaxCorners   = gcHarrisCornersMaxCorners;

  if(mHarrisCornersMaxCorners < mHarrisCornersMinCorners)
  {
    mHarrisCornersMaxCorners = mHarrisCornersMinCorners;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_DetectorTask::TaskService()
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
        case MESSAGE_ID_GDC_FT_IMAGE_REPLY:
        {
          GdcFt_ImageReply* pReply = (GdcFt_ImageReply*)pMessage;

          libResult = GdcFt_DetectorTask::HandleImageReply(*pReply);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          mIsTaskClosing = !(libResult == LIB_SUCCESS);

          pReply->~GdcFt_ImageReply();
          break;
        }
        case MESSAGE_ID_GDC_FT_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult = GdcFt_Helper::SendRequest(MESSAGE_ID_GDC_FT_EXIT_REQUEST, mReceiveMessageQueueId,
                                                mPyramidTaskMessageQueueId);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          mIsTaskClosing = true;

          break;
        }
        case MESSAGE_ID_GDC_FT_RESET_REQUEST:
        {
          mOutputFile.Close();

          if(mWeReadOutputFile == true)
          {
            printf("DetectorTask: Reference Read Complete, %d mismatche(s)\n", mReadMismatchCounter);
          }

          if(mWeWriteOutputFile == true)
          {
            printf("DetectorTask: Reference Write Complete\n");
          }

          mWeReadOutputFile  = false;
          mWeWriteOutputFile = false;

          libResult = GdcFt_Helper::SendRequest(MESSAGE_ID_GDC_FT_RESET_REQUEST, mReceiveMessageQueueId,
                                                mPyramidTaskMessageQueueId);
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
LIB_RESULT GdcFt_DetectorTask::TaskClose()
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
        case MESSAGE_ID_GDC_FT_IMAGE_REPLY:
        {
          GdcFt_ImageReply* pReply = (GdcFt_ImageReply*)pMessage;

          libResult = GdcFt_Helper::FreeImageReply(pReply);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          pReply->~GdcFt_ImageReply();
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
LIB_RESULT GdcFt_DetectorTask::HandleImageReply(GdcFt_ImageReply& rReply)
{
  LIB_RESULT libResult = LIB_FAILURE;
  int        intResult = -1;

  bool weDetectedFeatures = false;

  uint64_t ticks[gcApexCvEndTick] = {0};

  ticks[gcSceneTick]     = rReply.mTicks[gcSceneTick];
  ticks[gcSceneTick + 1] = rReply.mTicks[gcSceneTick + 1];
  ticks[gcDetectorTick]  = FSL_Ticks();

  if(rReply.mSceneCounter % gcTrackingToDetectionRatio == 0)
  {
    SHOW_ERROR(mhFeatureDescriptorOal == 0 && mFeatureDescriptor.GetDataPtr() == 0 &&
               mFeatureDescriptor.GetDataPtrPhys() == 0);

    mhFeatureDescriptorOal = OAL_MemoryAllocFlag(sizeof(icp::Feature) * mHarrisCornersMaxCorners,
                                                 OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_MEMORY_FLAG_CONTIGUOUS |
                                                     OAL_MEMORY_FLAG_ZERO);
    SHOW_ERROR(mhFeatureDescriptorOal != 0);

    mFeatureDescriptor.Init(OAL_MemoryReturnAddress(mhFeatureDescriptorOal, ACCESS_CH_WB),
                            OAL_MemoryReturnAddress(mhFeatureDescriptorOal, ACCESS_PHY), mHarrisCornersMaxCorners);

    if(mWeUseApexGfttCorners == true)
    {
      intResult = mGfttCorners.SelectApuConfiguration(gcApexCvGfttCornersAcfApuCfg, gcApexCvGfttCornersApexId);
      SHOW_ERROR(intResult == 0);
      if(mIsGfttCornersInitialized == false)
      {
        intResult = mGfttCorners.Initialize(rReply.mScene_umat, mFeatureDescriptor, mHarrisCornersQualityLevel,
                                            mHarrisCornersMinDistance, mHarrisCornersMaxCorners, mGfttCorners_UseHarris,
                                            mHarrisCornersCornerCoef, mHarrisCornersThreshold);
        SHOW_ERROR(intResult == 0);
        mIsGfttCornersInitialized = true;
      }
      else
      {
        intResult = mGfttCorners.ReconnectIO(rReply.mScene_umat, mFeatureDescriptor);
        SHOW_ERROR(intResult == 0);
      }

      ticks[gcDetectorTick + 1] = FSL_Ticks();

      intResult = mGfttCorners.Process();
      SHOW_ERROR(intResult == 0);

      ticks[gcDetectorTick + 2] = FSL_Ticks();
    }
    else if(mWeUseApexGfttCorners == false)
    {
      intResult = mGfttCornersRef.process(rReply.mScene_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED),
                                          mFeatureDescriptor, mHarrisCornersMinDistance, mHarrisCornersQualityLevel,
                                          mHarrisCornersBoxSize, mHarrisCornersNmsRadius, mHarrisCornersMaxCorners,
                                          mGfttCorners_UseHarris, mHarrisCornersCornerCoef, mHarrisCornersThreshold);
      SHOW_ERROR(intResult == 0);
    }

    weDetectedFeatures = true;
  }

  ticks[gcDetectorTick + 3] = FSL_Ticks();

  //
  // STEP: If developers are making modifications to the algorithm code
  //       this provides a means of capturing a reference output and/or
  //       testing the local output with the reference.
  //       It is a means of doing regression testing.
  //
  if(mWeWriteOutputFile == true || mWeReadOutputFile == true)
  {
    //
    // Setup
    //
    GdcFt_Output_HarrisCorners localOutput;

    localOutput.mSceneCounter = rReply.mSceneCounter;
    localOutput.mFeatureDescriptor.Init(mFeatureDescriptor.GetDataPtr(), mFeatureDescriptor.GetDataPtrPhys(),
                                        mFeatureDescriptor.GetSize());
    localOutput.mFeatureDescriptor.SetCount(mFeatureDescriptor.GetCount());

    //
    // Body
    //
    if(mWeWriteOutputFile == true)
    {
      libResult = mOutputFile.Write(localOutput);
      SHOW_ERROR(libResult == LIB_SUCCESS);
    }

    if(mWeReadOutputFile == true)
    {
      GdcFt_Output_HarrisCorners referenceOutput;

      libResult = mOutputFile.Read(referenceOutput);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      if(!(localOutput == referenceOutput))
      {
        ++mReadMismatchCounter;
        GdcFt_Helper::PrintMismatch_HarrisCorners(stderr, localOutput, referenceOutput);
      }

      referenceOutput.mWeFreeBuffers =
          true; // Any buffers allocated during read of the reference will be freed in the destructor.
    }

    //
    // Tear down
    //
    localOutput.mFeatureDescriptor.SetCount(0);
    localOutput.mFeatureDescriptor.Init(0, 0, 0);
  }

  libResult = GdcFt_Helper::SendDetectorData(
      mPyramidTaskMessageQueueId, rReply.mSceneRgb24_umat,
      rReply.mSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
      rReply.mScene_umat,
      rReply.mScene_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
      rReply.mSceneCounter, mhFeatureDescriptorOal, mFeatureDescriptor, weDetectedFeatures, ticks);
  if(libResult != LIB_SUCCESS)
  {
    if(mhFeatureDescriptorOal != 0)
    {
      OAL_MemoryFree(mhFeatureDescriptorOal);
    }
  }

  mhFeatureDescriptorOal = 0;
  mFeatureDescriptor.SetCount(0);
  mFeatureDescriptor.Init(0, 0, 0);

  return libResult;
}
