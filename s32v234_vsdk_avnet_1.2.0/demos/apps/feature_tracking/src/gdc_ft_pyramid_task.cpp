/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2015 Freescale Semiconductor;
* Copyright 2017-2018 NXP
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
#include "gdc_ft_pyramid_task.hpp"
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
GdcFt_PyramidTask::GdcFt_PyramidTask()
    : mReceiveMessageQueueId(0), mTrackerTaskMessageQueueId(0),
      //uint32_t mImagePyramidSizes[gcDepthOfImagePyramid][5]; // [0] Width, [1] Height, [2] Padded Width, [3] Padded Height, [4] Offset
      //apexcv::PyramidMultiCreation mPyramidMultiCreation;
      mIsPyramidMultiCreationInitialized(false),

      mWeUseApexPyramidMultiCreation(true),
      //GdcFt_OutputFile_PyramidMultiCreation  mOutputFile;
      //char    mOutputReadFilename[gcFileNameSize+1];
      //char    mOutputWriteFilename[gcFileNameSize+1];
      mWeReadOutputFile(false), mWeWriteOutputFile(false), mReadMismatchCounter(0), mIsTaskClosing(false),
      mIsConfigured(false)
{
  memset(mImagePyramidSizes, 0, sizeof(mImagePyramidSizes));
  memset(mOutputReadFilename, 0, sizeof(mOutputReadFilename));
  memset(mOutputWriteFilename, 0, sizeof(mOutputWriteFilename));
}

/***************************************************************************/
/***************************************************************************/
GdcFt_PyramidTask::~GdcFt_PyramidTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_PyramidTask::Configure(const int32_t cReceiveMessageQueueId, const int32_t cTrackerTaskMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId     = cReceiveMessageQueueId;
    mTrackerTaskMessageQueueId = cTrackerTaskMessageQueueId;
    mIsConfigured              = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_PyramidTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  dictionary* pDictionary = 0;
  ;

  pDictionary = iniparser_load(gcIniFileName);

  if(pDictionary != 0)
  {
    mWeUseApexPyramidMultiCreation =
        (iniparser_getint(pDictionary, "PyramidTask:WeUseApexPyramidMultiCreation", 1) == 1);

    char* pString = 0;

    mOutputReadFilename[0] = 0;

    pString = iniparser_getstring(pDictionary, "PyramidTask:ReadFileName", 0);

    if(pString != 0 && strlen(pString) != 0)
    {
      strcat(mOutputReadFilename, pString);
    }

    mOutputWriteFilename[0] = 0;
    pString                 = iniparser_getstring(pDictionary, "PyramidTask:WriteFileName", 0);

    if(pString != 0 && strlen(pString) != 0)
    {
      strcat(mOutputWriteFilename, pString);
    }

    mWeReadOutputFile  = (iniparser_getint(pDictionary, "PyramidTask:Read", 0) == 1);
    mWeWriteOutputFile = (iniparser_getint(pDictionary, "PyramidTask:Write", 0) == 1);

    iniparser_freedict(pDictionary);
    pDictionary = 0;
  }

  libResult =
      mOutputFile.Open((mWeWriteOutputFile ? mOutputWriteFilename : 0), (mWeReadOutputFile ? mOutputReadFilename : 0));
  SHOW_ERROR(libResult == LIB_SUCCESS);

  for(uint32_t i = 0; i < gcDepthOfImagePyramid; ++i)
  {
    mImagePyramidSizes[i][0] = gcSceneWidthInPixels / gcImagePyramidDivisors[i];     // Width
    mImagePyramidSizes[i][1] = gcSceneHeightInPixels / gcImagePyramidDivisors[i];    // Height
    mImagePyramidSizes[i][2] = mImagePyramidSizes[i][0] + 2 * gcScenePaddingInBytes; // Padded Width
    mImagePyramidSizes[i][3] = mImagePyramidSizes[i][1] + 2 * gcScenePaddingInBytes; // Padded Height
    mImagePyramidSizes[i][4] =
        (mImagePyramidSizes[i][2] * gcScenePaddingInBytes) + gcScenePaddingInBytes; // Offset in bytes
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcFt_PyramidTask::TaskService()
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
        case MESSAGE_ID_GDC_FT_DETECTOR_DATA:
        {
          GdcFt_DetectorData* pData = (GdcFt_DetectorData*)pMessage;

          libResult = GdcFt_PyramidTask::HandleDetectorData(*pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcFt_DetectorData();

          break;
        }
        case MESSAGE_ID_GDC_FT_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult = GdcFt_Helper::SendRequest(MESSAGE_ID_GDC_FT_EXIT_REQUEST, mReceiveMessageQueueId,
                                                mTrackerTaskMessageQueueId);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          mIsTaskClosing = true;

          break;
        }
        case MESSAGE_ID_GDC_FT_RESET_REQUEST:
        {
          mOutputFile.Close();

          if(mWeReadOutputFile == true)
          {
            printf("PyramidTask: Reference Read Complete, %d mismatche(s)\n", mReadMismatchCounter);
          }

          if(mWeWriteOutputFile == true)
          {
            printf("PyramidTask: Reference Write Complete\n");
          }

          mWeReadOutputFile  = false;
          mWeWriteOutputFile = false;

          libResult = GdcFt_Helper::SendRequest(MESSAGE_ID_GDC_FT_RESET_REQUEST, mReceiveMessageQueueId,
                                                mTrackerTaskMessageQueueId);
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
LIB_RESULT GdcFt_PyramidTask::TaskClose()
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
        case MESSAGE_ID_GDC_FT_DETECTOR_DATA:
        {
          GdcFt_DetectorData* pData = (GdcFt_DetectorData*)pMessage;

          libResult = GdcFt_Helper::FreeDetectorData(pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcFt_DetectorData();

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
LIB_RESULT GdcFt_PyramidTask::HandleDetectorData(GdcFt_DetectorData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;
  int        intResult = -1;

  rData.mTicks[gcPyramidTick] = FSL_Ticks();

  vsdk::UMat lImagePyramid_umat[gcDepthOfImagePyramid];
  vsdk::UMat lImagePyramidTemp_umat[apexcv::PyramidMultiCreation::PYRAMID_LEVELS];
  void*      lpImagePyramid_data[gcDepthOfImagePyramid] = {0};

  if(rData.mScene_umat.empty() == false)
  {
    for(uint32_t i = 0; i < gcDepthOfImagePyramid; ++i)
    {
      SHOW_ERROR(lImagePyramid_umat[i].empty() == true && lpImagePyramid_data[i] == 0);

      vsdk::UMat lImagePyramid_umat_temp0(mImagePyramidSizes[i][3], mImagePyramidSizes[i][2], CV_8UC1);
      vsdk::Rect lRect(gcScenePaddingInBytes, gcScenePaddingInBytes, mImagePyramidSizes[i][0],
                       mImagePyramidSizes[i][1]);

      vsdk::UMat lImagePyramid_umat_temp1(lImagePyramid_umat_temp0, lRect);

      lImagePyramid_umat[i] = lImagePyramid_umat_temp1;

      lpImagePyramid_data[i] = lImagePyramid_umat[i].getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data; // Debug only

      if(i == 0)
      {
        //
        // Copy data over
        //
        { // Need this scope to allow vsdk::Mat to go out of scope prior to calling APEX
          vsdk::Mat lImagePyramid_mat = lImagePyramid_umat[i].getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
          vsdk::Mat lscene_mat        = rData.mScene_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);

          const uint32_t lcSpanInBytes_Dst = lImagePyramid_mat.step[0];
          const uint32_t lcSpanInBytes_Src = lscene_mat.step[0];

          uint8_t* pLine_Dst = (uint8_t*)(lImagePyramid_mat.data);
          uint8_t* pLine_Src = (uint8_t*)(lscene_mat.data);

          for(int32_t h = 0; h < lImagePyramid_mat.rows; ++h) // For every line
          {
            memcpy(pLine_Dst, pLine_Src, lImagePyramid_mat.cols * lImagePyramid_mat.step[1]);

            pLine_Dst += lcSpanInBytes_Dst;
            pLine_Src += lcSpanInBytes_Src;
          }
        }
      }
    }

    for(int32_t i = 0; i < apexcv::PyramidMultiCreation::PYRAMID_LEVELS; i++)
    {
      lImagePyramidTemp_umat[i] = lImagePyramid_umat[i + 1];
    }

    if(mWeUseApexPyramidMultiCreation == true)
    {
      if(mIsPyramidMultiCreationInitialized == false)
      {
        intResult = mPyramidMultiCreation.Initialize(lImagePyramid_umat[0], lImagePyramidTemp_umat);
        SHOW_ERROR(intResult == 0);

        mIsPyramidMultiCreationInitialized = true;
      }
      else
      {
        intResult = mPyramidMultiCreation.ReconnectIO(lImagePyramid_umat[0], lImagePyramidTemp_umat);
        SHOW_ERROR(intResult == 0);
      }

      intResult = mPyramidMultiCreation.SelectApexCore(gcApexCvPyramidMultiCreationApexId);
      SHOW_ERROR(intResult == 0);

      rData.mTicks[gcPyramidTick + 1] = FSL_Ticks();

      intResult = mPyramidMultiCreation.Process();
      SHOW_ERROR(intResult == 0);

      rData.mTicks[gcPyramidTick + 2] = FSL_Ticks();
    }
    else if(mWeUseApexPyramidMultiCreation == false)
    {
      intResult = mPyramidMultiCreation_ref.process(lImagePyramid_umat[0], &lImagePyramid_umat[1], false);
      SHOW_ERROR(intResult == 0);
    }

    rData.mTicks[gcPyramidTick + 3] = FSL_Ticks();
  }

  //
  // STEP: If developers are making modifications to the algorithm code
  //       this provides a means of capturing a reference output and/or
  //       testing the local output with the reference.
  //       It is a means of doing regression testing.
  //

  libResult = GdcFt_Helper::SendPyramidData(
      mTrackerTaskMessageQueueId, rData.mSceneRgb24_umat,
      rData.mSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
      rData.mScene_umat,
      rData.mScene_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
      rData.mSceneCounter, rData.mhFeatureDescriptorOal, rData.mFeatureDescriptor, rData.mWeDetectedFeatures,
      lImagePyramid_umat, rData.mTicks);
  if(libResult != LIB_SUCCESS)
  {
    if(rData.mhFeatureDescriptorOal != 0)
    {
      OAL_MemoryFree(rData.mhFeatureDescriptorOal);
    }
  }

  rData.mhFeatureDescriptorOal = 0;
  rData.mFeatureDescriptor.SetCount(0);
  rData.mFeatureDescriptor.Init(0, 0, 0);

  return libResult;
}
