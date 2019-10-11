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
#include "gdc_pd_hog_task.hpp"
#include "gdc_pd_svm_function.h"
#include "GDC_OAL_Semaphore.hpp"
#include "communications.hpp"
#include "global_errors.h"
#include "iniparser.h"
#include "time_helper.h"
#include <algorithm>
#include <stdlib.h>
#include <stdint.h>
#include <new>

//#define DUMP_PROFILING_TO_FILE
#define HOG_PROFILING "hog_profiling.csv"

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcPd_HogTask::GdcPd_HogTask()
    : mReceiveMessageQueueId(0), mCompositionTaskMessageQueueId(0),
      //GdcPd_FrameSize     mSceneSize;
      mNumberOfSceneResizes(0),
      //GdcPd_FrameSize     mSceneResizes[gcNumberOfSceneResizes];
      //uint32_t mHogScoresWidth[gcNumberOfSceneResizes];
      //uint32_t mHogScoresHeight[gcNumberOfSceneResizes];
      //icp::DataDescriptor mHogScoresDd[gcNumberOfSceneResizes];
      //uint32_t            mSceneResizesHogCounters[gcNumberOfSceneResizes];
      //std::vector<ROI> mPedestrianRois;
      mWeUseApexHog(false), mIsApexHogInitialized(false),
      //GdcPd_OutputFile_PedestrianDetection  mOutputFile;
      //char    mOutputReadFilename[gcFileNameSize+1];
      //char    mOutputWriteFilename[gcFileNameSize+1];
      mWeReadOutputFile(false), mWeWriteOutputFile(false), mWeDumpDetectionImages(false), mReadMismatchCounter(0),
      mIsTaskClosing(false), mIsConfigured(false)
{
  memset(mHogScoresWidth, 0, sizeof(mHogScoresWidth));
  memset(mHogScoresHeight, 0, sizeof(mHogScoresHeight));
  //memset(mpHogScoresOal,  0, sizeof(mpHogScoresOal));
  memset(mSceneResizesHogCounters, 0, sizeof(mSceneResizesHogCounters));
  memset(mOutputReadFilename, 0, sizeof(mOutputReadFilename));
  memset(mOutputWriteFilename, 0, sizeof(mOutputWriteFilename));
}

/***************************************************************************/
/***************************************************************************/
GdcPd_HogTask::~GdcPd_HogTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_HogTask::Configure(const int32_t                cReceiveMessageQueueId,
                                    const int32_t                cDisplayTaskMessageQueueId,
                                    const uint32_t               cNumberOfSceneResizes,
                                    const GdcPd_FrameSize* const cpcSceneResizes)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false && cNumberOfSceneResizes != 0 && cpcSceneResizes != 0)
  {
    mReceiveMessageQueueId         = cReceiveMessageQueueId;
    mCompositionTaskMessageQueueId = cDisplayTaskMessageQueueId;

    mNumberOfSceneResizes = cNumberOfSceneResizes;

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
int32_t GdcPd_HogTask::MismatchCounter_Get() const
{
  return mReadMismatchCounter;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_HogTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  int intResult = -1;

  if(mIsConfigured == true)
  {
    dictionary* pDictionary = 0;
    ;

    pDictionary = iniparser_load(gcIniFileName);

    if(pDictionary != 0)
    {
      mWeUseApexHog = (iniparser_getint(pDictionary, "App:WeUseApexHog", 1) == 1);

      char* pString = 0;

      mOutputReadFilename[0] = 0;

      pString = iniparser_getstring(pDictionary, "Test:ReadFileName", 0);

      if(pString != 0 && strlen(pString) != 0)
      {
        strcat(mOutputReadFilename, pString);
      }

      mOutputWriteFilename[0] = 0;
      pString                 = iniparser_getstring(pDictionary, "Test:WriteFileName", 0);

      if(pString != 0 && strlen(pString) != 0)
      {
        strcat(mOutputWriteFilename, pString);
      }

      mWeReadOutputFile      = (iniparser_getint(pDictionary, "Test:Read", 0) == 1);
      mWeWriteOutputFile     = (iniparser_getint(pDictionary, "Test:Write", 0) == 1);
      mWeDumpDetectionImages = (iniparser_getint(pDictionary, "Test:DumpDetectionImages", 0) == 1);

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }

    libResult = mOutputFile.Open((mWeWriteOutputFile ? mOutputWriteFilename : 0),
                                 (mWeReadOutputFile ? mOutputReadFilename : 0));
    SHOW_ERROR(libResult == LIB_SUCCESS);

    mSvmDouble = vsdk::UMat(gcGdcPdHogParams.svm.height, gcGdcPdHogParams.svm.width + 1, VSDK_CV_64FC1);

    FILE* pFile = fopen(gcFilenameSvm, "rb");

    vsdk::Mat svmMat     = mSvmDouble.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
    double*   pSvmDouble = (double*)svmMat.data;

    if(pFile != 0 && pSvmDouble != 0)
    {
      size_t bytesRead = fread(pSvmDouble, 1, (gcGdcPdHogParams.svm.width + 1) * sizeof(double), pFile);

      SHOW_ERROR((uint32_t)bytesRead == (gcGdcPdHogParams.svm.width + 1) * sizeof(double));

      fclose(pFile);
      pFile = 0;
    }
    else if(pSvmDouble != 0)
    {
      SHOW_ERROR(gdc_pd_svm_function_bin_len == ((gcGdcPdHogParams.svm.width + 1) * sizeof(double)));

      memcpy(pSvmDouble, gdc_pd_svm_function_bin, gdc_pd_svm_function_bin_len);
    }
    else
    {
      intResult = 1;
    }

    uint32_t max_width  = 0;
    uint32_t max_height = 0;

    for(uint32_t i = 0; i < mNumberOfSceneResizes; ++i)
    {
      mHogScoresWidth[i] =
          (mSceneResizes[i].mWidth - (gcGdcPdHogParams.winPixels.width - gcGdcPdHogParams.winStride.width)) /
          gcGdcPdHogParams.winStride.width;
      mHogScoresHeight[i] =
          (mSceneResizes[i].mHeight - (gcGdcPdHogParams.winPixels.height - gcGdcPdHogParams.winStride.height)) /
          gcGdcPdHogParams.winStride.height;

      max_width  = MAX_OF(max_width, mHogScoresWidth[i]);
      max_height = MAX_OF(max_height, mHogScoresHeight[i]);
    }

    /***************************************************************************/
    //  Initialization Routine for HOG Score Buffer
    /***************************************************************************/

    intResult = mHogRef.SetSVM(mSvmDouble);
    SHOW_ERROR(intResult == 0);
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_HogTask::TaskService()
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
        case MESSAGE_ID_GDC_PD_RESIZE_DATA:
        {
          //
          // Process the received image
          //
          GdcPd_ResizeData* pData = (GdcPd_ResizeData*)pMessage;

          libResult = GdcPd_HogTask::HandleResizeData(*pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          GDC_TRACE;

          pData->~GdcPd_ResizeData();

          break;
        }
        case MESSAGE_ID_GDC_PD_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult = GdcPd_Helper::SendRequest(MESSAGE_ID_GDC_PD_EXIT_REQUEST, mReceiveMessageQueueId,
                                                mCompositionTaskMessageQueueId);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          mIsTaskClosing = true;

          break;
        }
        case MESSAGE_ID_GDC_PD_RESET_REQUEST:
        {
          mOutputFile.Close();

          if(mWeReadOutputFile == true)
          {
            printf("Reference Read Complete, %d mismatche(s)\n", mReadMismatchCounter);
          }

          if(mWeWriteOutputFile == true)
          {
            printf("Reference Write Complete\n");
          }

          mWeReadOutputFile  = false;
          mWeWriteOutputFile = false;

          libResult = GdcPd_Helper::SendRequest(MESSAGE_ID_GDC_PD_RESET_REQUEST, mReceiveMessageQueueId,
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
LIB_RESULT GdcPd_HogTask::TaskClose()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  //
  // Free resources owned by the task
  //
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
        case MESSAGE_ID_GDC_PD_RESIZE_DATA:
        {
          //
          // Process the received image
          //
          GdcPd_ResizeData* pData = (GdcPd_ResizeData*)pMessage;

          libResult = GdcPd_Helper::FreeResizeData(pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          pData->~GdcPd_ResizeData();

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
LIB_RESULT GdcPd_HogTask::HandleResizeData(GdcPd_ResizeData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;
  int        intResult = -1;

  uint64_t         ticks_per[gcNumberOfSceneResizes][2] = {{0}};
  AcfProfilingInfo acfProfilingInfo_per[gcNumberOfSceneResizes];

  SHOW_ERROR(mNumberOfSceneResizes <= gcNumberOfSceneResizes && rData.mNumberOfSceneResizes == gcNumberOfSceneResizes);

  rData.mResizeTicks[gcApexCvHogTick] = FSL_Ticks();

#ifdef DUMP_PROFILING_TO_FILE
  FILE* fp;
  fp = fopen(HOG_PROFILING, "a+");
  fprintf(fp, " , Hog(us)\n");
  fprintf(fp, "Resolution, Total, Init, Processing, Idle, Misc\n");
#endif

  //
  // APEX Hog
  //

  for(uint32_t i = 0; i < mNumberOfSceneResizes; ++i)
  {
    SHOW_ERROR(rData.mSceneResizes_umat[i].cols == mSceneResizes[i].mWidth &&
               rData.mSceneResizes_umat[i].rows == mSceneResizes[i].mHeight &&
               rData.mSceneResizes_umat[i].step[0] == mSceneResizes[i].mSpan);

    if(mWeUseApexHog == true)
    {
      AcfProfilingInfo lAcfProfilingInfo;
      uint32_t         lStart;
      uint32_t         lStop;
      ticks_per[i][0] = FSL_Ticks();

      if(!mIsApexHogInitialized)
      {
        intResult = mHog.Initialize(rData.mSceneResizes_umat[i], mHogScores_umat, apexcv::Hog::HogType::DETECT);
        SHOW_ERROR(intResult == 0);

        intResult = mHog.SetSVM(mSvmDouble);
        SHOW_ERROR(intResult == 0);

        intResult = mHog.SelectApexCore(gcApexCvHogApexId);
        SHOW_ERROR(intResult == 0);

        mIsApexHogInitialized = true;
      }
      else
      {
        mHog.ReconnectIO(rData.mSceneResizes_umat[i], mHogScores_umat);
      }
      lStart    = FSL_Ticks();
      intResult = mHog.Process();
      lStop     = FSL_Ticks();
      SHOW_ERROR(intResult == 0);
      ticks_per[i][1] = FSL_Ticks();
      lAcfProfilingInfo.apu_total = CONVERT_TICKS_TO_US(lStop - lStart);
      acfProfilingInfo_per[i]     = lAcfProfilingInfo;

#ifdef DUMP_PROFILING_TO_FILE
      uint32_t metric_per = (ticks_per[i][1] - ticks_per[i][0]) / 1000;

      fprintf(fp, "%dx%d, %d \n", mHogScores_umat[i].GetWidth(), mHogScores_umat[i].GetHeight(), metric_per);
#endif
    }
    else if(mWeUseApexHog == false)
    {
      intResult = mHogRef.Detect(rData.mSceneResizes_umat[i], mHogScores_umat);
      SHOW_ERROR(intResult == 0);
    }

    vsdk::Mat            lHogScores_mat  = mHogScores_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);
    const int16_t* const cpcHogScoresS16 = (const int16_t* const)(((uintptr_t)lHogScores_mat.data));
    SHOW_ERROR(cpcHogScoresS16 != 0);

    ROI pedestrianRoi;
    for(uint32_t y = 0; y < mHogScoresHeight[i]; ++y)
    {
      for(uint32_t x = 0; x < mHogScoresWidth[i]; ++x)
      {
        const int16_t cScore = cpcHogScoresS16[y * (mHogScores_umat.step[0] / mHogScores_umat.elemSize()) + x];
        if(cScore > hog_threshold)
        {
          pedestrianRoi.x = x * gcGdcPdHogParams.winStride.width * rData.mScene_umat.cols / mSceneResizes[i].mWidth;
          pedestrianRoi.y = y * gcGdcPdHogParams.winStride.height * rData.mScene_umat.rows / mSceneResizes[i].mHeight;
          pedestrianRoi.width  = gcGdcPdHogParams.winPixels.width * rData.mScene_umat.cols / mSceneResizes[i].mWidth;
          pedestrianRoi.height = gcGdcPdHogParams.winPixels.height * rData.mScene_umat.rows / mSceneResizes[i].mHeight;
          pedestrianRoi.score_low         = cScore;
          pedestrianRoi.score_high        = cScore;
          pedestrianRoi.h_scale           = 0.0;
          pedestrianRoi.v_scale           = 0.0;
          pedestrianRoi.s_width           = tsx;
          pedestrianRoi.s_height          = tsy;
          pedestrianRoi.mDetectionCounter = 1;
          memset(pedestrianRoi.mResolutionDetectionCounters, 0, sizeof(pedestrianRoi.mResolutionDetectionCounters));
          ++(pedestrianRoi.mResolutionDetectionCounters[i]);
          ++mSceneResizesHogCounters[i];

          mPedestrianRois.push_back(pedestrianRoi);
        }
      }
    }
  }
  rData.mResizeTicks[gcApexCvHogTick + 1] = FSL_Ticks();
  //uint32_t metric = (ticks[1] - ticks[0])/1000;

  //printf("HOG runtime: %i\n", metric);

#ifdef DUMP_PROFILING_TO_FILE
  fprintf(fp, "\n");
  fclose(fp);
#endif
  if(mWeUseApexHog == false)
  {
    rData.mResizeTicks[gcApexCvHogTick]     = 0;
    rData.mResizeTicks[gcApexCvHogTick + 1] = 0;
  }

  //uint32_t new_ticks [2];

  libResult = GdcPd_Helper::bbsNMS(mPedestrianRois, 0.55);
  SHOW_ERROR(libResult == LIB_SUCCESS);
  libResult =
      GdcPd_Helper::SendHogData(mCompositionTaskMessageQueueId, rData.mSceneRgb24_umat,
                                rData.mSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
                                rData.mScene_umat,
                                rData.mScene_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
                                rData.mSceneCounter, mPedestrianRois, rData.mPerResizeTicks,
                                rData.mPerResizeAcfProfilingInfo, rData.mResizeTicks, ticks_per, acfProfilingInfo_per);
  if(libResult != LIB_SUCCESS)
  {
    mPedestrianRois.resize(0);
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcPd_HogTask::VerifyOutput_PedestrianDetection(
    const uint32_t      cSceneCounter,
    std::vector< ROI >& rPedestrianRois,
    icp::DataDescriptor (&rSceneResizeDd)[gcNumberOfSceneResizes])
{
  LIB_RESULT libResult = LIB_FAILURE;

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
    GdcPd_Output_PedestrianDetection localOutput;

    localOutput.mImageNumber = cSceneCounter;
    localOutput.mPedestrianRois.swap(rPedestrianRois);

    for(uint32_t i = 0; i < mNumberOfSceneResizes; ++i)
    {
      localOutput.mSceneResizeDd[i].InitManual(rSceneResizeDd[i].GetWidth(), rSceneResizeDd[i].GetHeight(),
                                               rSceneResizeDd[i].GetDataPtr(), rSceneResizeDd[i].GetDataPtrPhys(),
                                               rSceneResizeDd[i].GetElementDataType(),
                                               rSceneResizeDd[i].GetElementDimX(), rSceneResizeDd[i].GetElementDimY());
      localOutput.mSceneResizeDd[i].SetSpan(rSceneResizeDd[i].GetSpan());
    }

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
      GdcPd_Output_PedestrianDetection referenceOutput;

      libResult = mOutputFile.Read(referenceOutput);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      if(!(localOutput == referenceOutput))
      {
        ++mReadMismatchCounter;
        GdcPd_Helper::PrintMismatch_PedestrianDetection(stderr, localOutput, referenceOutput);
      }

      referenceOutput.mWeFreeBuffers =
          true; // Any buffers allocated during read of the reference will be freed in the destructor.
    }

    //
    // Tear down
    //
    for(uint32_t i = 0; i < mNumberOfSceneResizes; ++i)
    {
      localOutput.mSceneResizeDd[i].UpdatePtrs(
          0, 0); // Must ensure that original owner has exclusive ownership of these buffers.
    }

    localOutput.mPedestrianRois.swap(rPedestrianRois); // Must return content to original container.
  }
  else
  {
    libResult = LIB_SUCCESS;
  }

  return libResult;
}
