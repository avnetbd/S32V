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
#include "gdc_ft_tracker_task.hpp"
#include "GDC_OAL_Semaphore.hpp"
#include "communications.hpp"
#include "iniparser.h"
#include "global_errors.h"
#include <stdint.h>
#include <new>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcFt_TrackerTask::GdcFt_TrackerTask()
    : mReceiveMessageQueueId(0), mCompositionTaskMessageQueueId(0),
      //apexcv::LKTrackerOpticalFlow mLktTrackerOpticalFlow;
      //LKTrackerRef                 mLkTrackerRef;
      mIsLktTrackerOpticalFlowInitialized(false),
      //void* mhImagePyramidOal_Current[gcDepthOfImagePyramid];
      //icp::DataDescriptor    mImagePyramidDd_Current[gcDepthOfImagePyramid];
      //void* mhImagePyramidOal_Previous[gcDepthOfImagePyramid];
      //icp::DataDescriptor    mImagePyramidDd_Previous[gcDepthOfImagePyramid];
      mhFeatures32S_PreviousOal_Temp(0),
      //icp::FeatureDescriptor mFeaturesCurrent_Temp;
      mhFeatures32S_CurrentOal_Temp(0),
      //icp::FeatureDescriptor mFeaturesPrevious_Temp;
      mpTrackedFeatureAllocator(0),
      //GdcFt_TrackedFeatureAllocator mTrackedFeatureAllocator;
      mppTrackedFeatureTemp(0), mppTrackedFeatures(0), mNumberOfTrackedFeatures(0),
      mTrackedFeatureStrength_Threshold(gcTrackedFeatureStrength_Threshold),
      mDetectedFeatureStrength_Threshold(gcDetectedFeatureStrength_Threshold), mWeUseApexLKTrackerOpticalFlow(true),
      //GdcFt_OutputFile  mOutputFile;
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
GdcFt_TrackerTask::~GdcFt_TrackerTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_TrackerTask::Configure(const int32_t cReceiveMessageQueueId,
                                        const int32_t cCompositionTaskMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  // TO USE: Add additional validity checks here.
  //         You can check to make sure that the parameters are valid. For example, you
  //         may want to make sure some parameter is non-zero.
  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId         = cReceiveMessageQueueId;
    mCompositionTaskMessageQueueId = cCompositionTaskMessageQueueId;
    mIsConfigured                  = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
int32_t GdcFt_TrackerTask::MismatchCounter_Get() const
{
  return mReadMismatchCounter;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_TrackerTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  mIsLktTrackerOpticalFlowInitialized = false;

  //
  // Get info from .ini file
  //
  dictionary* pDictionary = 0;

  pDictionary = iniparser_load(gcIniFileName);

  if(pDictionary != 0)
  {
    mWeUseApexLKTrackerOpticalFlow =
        (iniparser_getint(pDictionary, "TrackerTask:WeUseApexLKTrackerOpticalFlow", 1) == 1);

    mTrackedFeatureStrength_Threshold  = iniparser_getint(pDictionary, "TrackerTask:TrackedFeatureStrengthThreshold",
                                                         gcTrackedFeatureStrength_Threshold);
    mDetectedFeatureStrength_Threshold = iniparser_getint(pDictionary, "TrackerTask:DetectedFeatureStrengthThreshold",
                                                          gcDetectedFeatureStrength_Threshold);

    char* pString = 0;

    mOutputReadFilename[0] = 0;

    pString = iniparser_getstring(pDictionary, "TrackerTask:ReadFileName", 0);

    if(pString != 0 && strlen(pString) != 0)
    {
      strcat(mOutputReadFilename, pString);
    }

    mOutputWriteFilename[0] = 0;
    pString                 = iniparser_getstring(pDictionary, "TrackerTask:WriteFileName", 0);

    if(pString != 0 && strlen(pString) != 0)
    {
      strcat(mOutputWriteFilename, pString);
    }

    mWeReadOutputFile  = (iniparser_getint(pDictionary, "TrackerTask:Read", 0) == 1);
    mWeWriteOutputFile = (iniparser_getint(pDictionary, "TrackerTask:Write", 0) == 1);

    iniparser_freedict(pDictionary);
    pDictionary = 0;
  }

  libResult =
      mOutputFile.Open((mWeWriteOutputFile ? mOutputWriteFilename : 0), (mWeReadOutputFile ? mOutputReadFilename : 0));
  SHOW_ERROR(libResult == LIB_SUCCESS);

  for(uint32_t i = 0; i < gcDepthOfImagePyramid; ++i)
  {
    mImagePyramidSizes[i][0] = gcSceneWidthInPixels / gcImagePyramidDivisors[i];
    mImagePyramidSizes[i][1] = gcSceneHeightInPixels / gcImagePyramidDivisors[i];
    mImagePyramidSizes[i][2] = mImagePyramidSizes[i][0] + 2 * gcScenePaddingInBytes;
    mImagePyramidSizes[i][3] = mImagePyramidSizes[i][1] + 2 * gcScenePaddingInBytes;
    mImagePyramidSizes[i][4] = (mImagePyramidSizes[i][2] * gcScenePaddingInBytes) + gcScenePaddingInBytes;
  }

  mhFeatures32S_PreviousOal_Temp =
      OAL_MemoryAllocFlag(sizeof(icp::FeatureDescriptor) * gcMaxNumberOfTrackedFeatures,
                          OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_MEMORY_FLAG_CONTIGUOUS | OAL_MEMORY_FLAG_ZERO);
  SHOW_ERROR(mhFeatures32S_PreviousOal_Temp != 0);

  mFeatures32S_Previous_Temp.Init(OAL_MemoryReturnAddress(mhFeatures32S_PreviousOal_Temp, ACCESS_CH_WB),
                                  OAL_MemoryReturnAddress(mhFeatures32S_PreviousOal_Temp, ACCESS_PHY),
                                  gcMaxNumberOfTrackedFeatures);

  mhFeatures32S_CurrentOal_Temp =
      OAL_MemoryAllocFlag(sizeof(icp::FeatureDescriptor) * gcMaxNumberOfTrackedFeatures,
                          OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_MEMORY_FLAG_CONTIGUOUS | OAL_MEMORY_FLAG_ZERO);
  SHOW_ERROR(mhFeatures32S_CurrentOal_Temp != 0);

  mFeatures32S_Current_Temp.Init(OAL_MemoryReturnAddress(mhFeatures32S_CurrentOal_Temp, ACCESS_CH_WB),
                                 OAL_MemoryReturnAddress(mhFeatures32S_CurrentOal_Temp, ACCESS_PHY),
                                 gcMaxNumberOfTrackedFeatures);

  uint32_t sizeInBytes = 0;

  libResult = mTrackedFeatureAllocator.Initialize(0, 0,
                                                  gcMaxNumberOfTrackedFeatures * 2, // SASHBY: Why multiply by 2?
                                                  sizeInBytes);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  mpTrackedFeatureAllocator = malloc(sizeInBytes);
  SHOW_ERROR(mpTrackedFeatureAllocator != 0);

  libResult = mTrackedFeatureAllocator.Initialize(mpTrackedFeatureAllocator, sizeInBytes,
                                                  gcMaxNumberOfTrackedFeatures * 2, // SASHBY: Why multiply by 2?
                                                  sizeInBytes);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  mppTrackedFeatureTemp = (GdcFt_TrackedFeature**)calloc(gcMaxNumberOfTrackedFeatures, sizeof(mppTrackedFeatureTemp));
  SHOW_ERROR(mppTrackedFeatureTemp != 0);

  mppTrackedFeatures = (GdcFt_TrackedFeature**)calloc(gcMaxNumberOfTrackedFeatures, sizeof(mppTrackedFeatures));
  SHOW_ERROR(mppTrackedFeatures != 0);

  mNumberOfTrackedFeatures = 0; // Number of tracked features in mppTrackedFeatures

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_TrackerTask::TaskService()
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
        case MESSAGE_ID_GDC_FT_PYRAMID_DATA:
        {
          GdcFt_PyramidData* pData = (GdcFt_PyramidData*)pMessage;

          libResult = GdcFt_TrackerTask::HandlePyramidData(*pData);
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

          libResult = GdcFt_Helper::SendRequest(MESSAGE_ID_GDC_FT_EXIT_REQUEST, mReceiveMessageQueueId,
                                                mCompositionTaskMessageQueueId);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          mIsTaskClosing = true;

          break;
        }
        case MESSAGE_ID_GDC_FT_RESET_REQUEST:
        {
          mOutputFile.Close();

          if(mWeReadOutputFile == true)
          {
            printf("TrackerTask: Reference Read Complete, %d mismatche(s)\n", mReadMismatchCounter);
          }

          if(mWeWriteOutputFile == true)
          {
            printf("TrackerTask: Reference Write Complete\n");
          }

          mWeReadOutputFile  = false;
          mWeWriteOutputFile = false;

          libResult = GdcFt_Helper::SendRequest(MESSAGE_ID_GDC_FT_RESET_REQUEST, mReceiveMessageQueueId,
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
LIB_RESULT GdcFt_TrackerTask::TaskClose()
{
  GDC_TRACE;
  LIB_RESULT libResult = LIB_FAILURE;

  //
  // Free resources owned by the task
  //
  if(mhFeatures32S_PreviousOal_Temp != 0)
  {
    OAL_MemoryFree(mhFeatures32S_PreviousOal_Temp);
    mhFeatures32S_PreviousOal_Temp = 0;
    mFeatures32S_Previous_Temp.Init(0, 0, 0);
  }

  if(mhFeatures32S_CurrentOal_Temp != 0)
  {
    OAL_MemoryFree(mhFeatures32S_CurrentOal_Temp);
    mhFeatures32S_CurrentOal_Temp = 0;
    mFeatures32S_Current_Temp.Init(0, 0, 0);
  }

  if(mpTrackedFeatureAllocator != 0)
  {
    free(mpTrackedFeatureAllocator);
    mpTrackedFeatureAllocator = 0;
  }

  if(mppTrackedFeatureTemp != 0)
  {
    free(mppTrackedFeatureTemp);
    mppTrackedFeatureTemp = 0;
  }

  if(mppTrackedFeatures != 0)
  {
    free(mppTrackedFeatures);
    mppTrackedFeatures = 0;
  }

  mOutputFile.Close();

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
LIB_RESULT GdcFt_TrackerTask::HandlePyramidData(GdcFt_PyramidData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;
  int        intResult = -1;

  uint32_t lNumberOfDetectedFeatures = 0;

  rData.mTicks[gcTrackerTick] = FSL_Ticks();

  bool isPyramidDataValid = false;

  if(rData.mScene_umat.empty() == false && rData.mFeatureDescriptor.GetDataPtr() != 0)
  {
    isPyramidDataValid = true;

    for(uint32_t i = 0; i < gcDepthOfImagePyramid; ++i)
    {
      if(rData.mImagePyramid_umat[i].empty() == true)
      {
        isPyramidDataValid = false;
        break;
      }
    }
  }

  if(isPyramidDataValid == true)
  {
    lNumberOfDetectedFeatures = rData.mFeatureDescriptor.GetCount();

    //
    // Current becomes previous,
    // and received becomes current Image pyramid
    //
    for(uint32_t i = 0; i < gcDepthOfImagePyramid; ++i)
    {
      mImagePyramid_umat_Previous[i] = mImagePyramid_umat_Current[i];

      mImagePyramid_umat_Current[i] = rData.mImagePyramid_umat[i];
    }

    if(rData.mWeDetectedFeatures == true && mNumberOfTrackedFeatures == 0)
    {
      libResult = GdcFt_Helper::FilterFeatures(mTrackedFeatureAllocator, gcFilterGridCellWidthInPixels,
                                               gcFilterGridCellHeightInPixels, mppTrackedFeatureTemp,
                                               mppTrackedFeatures, mNumberOfTrackedFeatures, rData.mFeatureDescriptor,
                                               mTrackedFeatureStrength_Threshold, mDetectedFeatureStrength_Threshold);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      if(rData.mhFeatureDescriptorOal != 0)
      {
        OAL_MemoryFree(rData.mhFeatureDescriptorOal);

        rData.mhFeatureDescriptorOal = 0;
        rData.mFeatureDescriptor.SetCount(0);
        rData.mFeatureDescriptor.Init(0, 0, 0);
      }
    }
    else
    {
      //
      // Preparing the input arguments of LKT tracker optical flow
      //
      mFeatures32S_Previous_Temp.SetCount(0);
      mFeatures32S_Current_Temp.SetCount(0);

      for(int32_t i = 0; i < mNumberOfTrackedFeatures; ++i)
      {
        if(mppTrackedFeatures[i]->mCurrent.position.x >= 0 &&
           mppTrackedFeatures[i]->mCurrent.position.x < gcSceneWidthInPixels &&
           mppTrackedFeatures[i]->mCurrent.position.y >= 0 &&
           mppTrackedFeatures[i]->mCurrent.position.y < gcSceneHeightInPixels)
        {
          mFeatures32S_Previous_Temp.Add(
              (((int32_t)mppTrackedFeatures[i]->mCurrent.position.x) << LKTRACKER_FLOATING_COOR_Q) /
                  gcImagePyramidDivisors[gcDepthOfImagePyramid - 1],
              (((int32_t)mppTrackedFeatures[i]->mCurrent.position.y) << LKTRACKER_FLOATING_COOR_Q) /
                  gcImagePyramidDivisors[gcDepthOfImagePyramid - 1],
              mppTrackedFeatures[i]->mCurrent.strength);

          mFeatures32S_Current_Temp.Add(
              ((int32_t)mppTrackedFeatures[i]->mCurrent.position.x) << LKTRACKER_FLOATING_COOR_Q,
              ((int32_t)mppTrackedFeatures[i]->mCurrent.position.y) << LKTRACKER_FLOATING_COOR_Q,
              mppTrackedFeatures[i]->mCurrent.strength);
        }
        else
        {
          SHOW_ERROR(false);
        }
      }

      if(mWeUseApexLKTrackerOpticalFlow == true)
      {
        intResult = mLktTrackerOpticalFlow.SelectApuConfiguration(gcApexCvLKTrackerOpticalFlowAcfApuCfg,
                                                                  gcApexCvLKTrackerOpticalFlowApexId);
        SHOW_ERROR(intResult == 0);
        //
        // LKT tracker optical flow only needs to be initialized once
        //
        if(mIsLktTrackerOpticalFlowInitialized == false)
        {
          intResult = mLktTrackerOpticalFlow.Initialize(
              mImagePyramid_umat_Previous[gcDepthOfImagePyramid - 1],
              mImagePyramid_umat_Current[gcDepthOfImagePyramid - 1], mFeatures32S_Previous_Temp,
              mFeatures32S_Previous_Temp, mFeatures32S_Current_Temp, gcMaxNumberOfTrackedFeatures, gcSceneWidthInPixels,
              gcSceneHeightInPixels, gcLktBoxSize, gcLktNumberOfIterationPerPyramidLayer, gcLktIsSourcePadded,
              gcLktPadStartWidth);
          SHOW_ERROR(intResult == 0);
          mIsLktTrackerOpticalFlowInitialized = true;
        }
      }

      //
      // Tracking features from the previous to the current frame
      // This requires tracking them from the top layer of the pyramid to the base.
      //
      if(mWeUseApexLKTrackerOpticalFlow == true)
      {
        rData.mTicks[gcTrackerTick + 1] = FSL_Ticks();
      }

      for(int32_t i = (gcDepthOfImagePyramid - 1); i >= 0; --i)
      {
        if(mWeUseApexLKTrackerOpticalFlow == true)
        {
          if(i == (gcDepthOfImagePyramid - 1)) // Top of pyramid
          {
            intResult = mLktTrackerOpticalFlow.ReconnectIO(mImagePyramid_umat_Previous[i],
                                                           mImagePyramid_umat_Current[i], mFeatures32S_Previous_Temp,
                                                           mFeatures32S_Previous_Temp, mFeatures32S_Current_Temp);
            SHOW_ERROR(intResult == 0);
          }
          else
          {
            intResult = mLktTrackerOpticalFlow.ReconnectIO(mImagePyramid_umat_Previous[i],
                                                           mImagePyramid_umat_Current[i], mFeatures32S_Previous_Temp,
                                                           mFeatures32S_Current_Temp, mFeatures32S_Current_Temp);
            SHOW_ERROR(intResult == 0);
          }

          intResult = mLktTrackerOpticalFlow.Process();
          SHOW_ERROR(intResult == 0);
        }
        else if(mWeUseApexLKTrackerOpticalFlow == false)
        {
          LKTrackerRef lLkTrackerRef;

          vsdk::Mat lImagePyramid_mat_Previous =
              mImagePyramid_umat_Previous[i].getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
          vsdk::Mat lImagePyramid_mat_Current =
              mImagePyramid_umat_Current[i].getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);

          if(i == (gcDepthOfImagePyramid - 1)) // Top of pyramid
          {
            intResult = lLkTrackerRef.Initialize(lImagePyramid_mat_Previous, lImagePyramid_mat_Current,
                                                 mFeatures32S_Previous_Temp, mFeatures32S_Previous_Temp,
                                                 mFeatures32S_Current_Temp, gcLktBoxSize,
                                                 gcLktNumberOfIterationPerPyramidLayer);
            SHOW_ERROR(intResult == 0);
          }
          else
          {
            intResult = lLkTrackerRef.Initialize(lImagePyramid_mat_Previous, lImagePyramid_mat_Current,
                                                 mFeatures32S_Previous_Temp, mFeatures32S_Current_Temp,
                                                 mFeatures32S_Current_Temp, gcLktBoxSize,
                                                 gcLktNumberOfIterationPerPyramidLayer);
            SHOW_ERROR(intResult == 0);
          }

          intResult = lLkTrackerRef.Process();
          SHOW_ERROR(intResult == 0);
        }

        //
        // For each layer of the pyramid which is not the base I need to rescale/reset feature positions to the scale of the pyramid layer.
        //
        if(i > 0) // Not base of pyramid
        {
          for(int32_t j = 0; j < mFeatures32S_Current_Temp.GetCount(); ++j)
          {
            mFeatures32S_Current_Temp[j].position.x *= 2; // Magic Number bad!!!
            mFeatures32S_Current_Temp[j].position.y *= 2; // Magic Number bad!!!
          }

          for(int32_t j = 0; j < mNumberOfTrackedFeatures; ++j)
          {
            mFeatures32S_Previous_Temp[j].position.x =
                (((int32_t)mppTrackedFeatures[j]->mCurrent.position.x) << LKTRACKER_FLOATING_COOR_Q) /
                gcImagePyramidDivisors[i - 1];
            mFeatures32S_Previous_Temp[j].position.y =
                (((int32_t)mppTrackedFeatures[j]->mCurrent.position.y) << LKTRACKER_FLOATING_COOR_Q) /
                gcImagePyramidDivisors[i - 1];
          }
        }
      }

      if(mWeUseApexLKTrackerOpticalFlow == true)
      {
        rData.mTicks[gcTrackerTick + 2] = FSL_Ticks();
      }

      libResult = GdcFt_TrackerTask::VerifyOutput_LKTrackerOpticalFlow(rData.mSceneCounter, mFeatures32S_Current_Temp);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      //
      // Filter out the features which are no longer tracked
      // These are identified by the member variable "reserve" with a value equal to zero.
      //
      int32_t numberOfTrackedFeatures_New = 0;

      for(int32_t i = 0; i < mNumberOfTrackedFeatures; ++i)
      {
        /*const*/ icp::Feature32S& rcFeature32SCurrent = mFeatures32S_Current_Temp[i];

        const int32_t cRoundedX =
            (rcFeature32SCurrent.position.x + LKTRACKER_FLOATING_0_5_Q) >> LKTRACKER_FLOATING_COOR_Q;
        const int32_t cRoundedY =
            (rcFeature32SCurrent.position.y + LKTRACKER_FLOATING_0_5_Q) >> LKTRACKER_FLOATING_COOR_Q;

        if(rcFeature32SCurrent.reserve > 0 &&
           cRoundedX > 0 && // Tracker does not handle edges well so discarding those touching the boundary
           cRoundedY > 0 && cRoundedX < (gcSceneWidthInPixels - 1) && cRoundedY < (gcSceneHeightInPixels - 1))
        {
          icp::Feature32S featureCurrent;

          featureCurrent.position.x = cRoundedX;
          featureCurrent.position.y = cRoundedY;
          featureCurrent.strength   = rcFeature32SCurrent.strength;
          featureCurrent.reserve    = rcFeature32SCurrent.reserve;

          libResult = mppTrackedFeatures[i]->Update(featureCurrent);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          mppTrackedFeatureTemp[numberOfTrackedFeatures_New] = mppTrackedFeatures[i];
          ++numberOfTrackedFeatures_New;
        }
        else
        {
          libResult = mTrackedFeatureAllocator.Deallocate(mppTrackedFeatures[i]);
          SHOW_ERROR(libResult == LIB_SUCCESS);
        }
      }

      // All features which are still being tracked have been moved to "mppTrackedFeatureTemp"
      // Here we're just doing housekeeping by cleaning the original set.
      memset(mppTrackedFeatures, 0, gcMaxNumberOfTrackedFeatures * sizeof(mppTrackedFeatures));

      // The filtered set of features becomes the new current set of features
      std::swap(mppTrackedFeatureTemp, mppTrackedFeatures);

      mNumberOfTrackedFeatures = numberOfTrackedFeatures_New;

      mFeatures32S_Previous_Temp.SetCount(0);
      mFeatures32S_Current_Temp.SetCount(0);

      //
      // Remove features which are no longer satisfactory
      // Try to add features which were detected on this frame
      //
      libResult = GdcFt_Helper::FilterFeatures(mTrackedFeatureAllocator, gcFilterGridCellWidthInPixels,
                                               gcFilterGridCellHeightInPixels, mppTrackedFeatureTemp,
                                               mppTrackedFeatures, mNumberOfTrackedFeatures, rData.mFeatureDescriptor,
                                               mTrackedFeatureStrength_Threshold, mDetectedFeatureStrength_Threshold);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      if(rData.mhFeatureDescriptorOal != 0)
      {
        OAL_MemoryFree(rData.mhFeatureDescriptorOal);

        rData.mhFeatureDescriptorOal = 0;
        rData.mFeatureDescriptor.SetCount(0);
        rData.mFeatureDescriptor.Init(0, 0, 0);
      }
    }
  }
  rData.mTicks[gcTrackerTick + 3] = FSL_Ticks();

  libResult = GdcFt_Helper::SendTrackerData(
      mCompositionTaskMessageQueueId, rData.mSceneRgb24_umat,
      rData.mSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
      rData.mScene_umat,
      rData.mScene_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
      rData.mSceneCounter, mppTrackedFeatures,
      mNumberOfTrackedFeatures, // Function will do a copy of these, since the tracker task needs to keeps its own copy
      lNumberOfDetectedFeatures, rData.mTicks);

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_TrackerTask::VerifyOutput_LKTrackerOpticalFlow(const uint32_t             cSceneCounter,
                                                                icp::Feature32SDescriptor& rFeature32SDescriptor)
{
  LIB_RESULT libResult = LIB_FAILURE;

  //
  // If developers are making modifications to the algorithm code
  // this provides a means of capturing a reference output and/or
  // testing the local output with the reference.
  // It is a means of doing regression testing.
  //
  if(mWeWriteOutputFile == true || mWeReadOutputFile == true)
  {
    //
    // Setup
    //
    GdcFt_Output_LKTrackerOpticalFlow localOutput;

    localOutput.mSceneCounter = cSceneCounter;
    localOutput.mFeature32SDescriptor.Init(rFeature32SDescriptor.GetDataPtr(), rFeature32SDescriptor.GetDataPtrPhys(),
                                           rFeature32SDescriptor.GetSize());
    localOutput.mFeature32SDescriptor.SetCount(rFeature32SDescriptor.GetCount());

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
      GdcFt_Output_LKTrackerOpticalFlow referenceOutput;

      libResult = mOutputFile.Read(referenceOutput);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      if(!(localOutput == referenceOutput))
      {
        ++mReadMismatchCounter;
        GdcFt_Helper::PrintMismatch_LKTrackerOpticalFlow(stderr, localOutput, referenceOutput);
      }

      referenceOutput.mWeFreeBuffers =
          true; // Any buffers allocated during read of the reference will be freed in the destructor.
    }

    //
    // Tear down
    //
    localOutput.mFeature32SDescriptor.SetCount(0);
    localOutput.mFeature32SDescriptor.Init(0, 0, 0);
  }
  else
  {
    libResult = LIB_SUCCESS;
  }

  return libResult;
}
