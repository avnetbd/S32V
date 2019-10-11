/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * Copyright 2013-2017 NXP 
 * All Rights Reserved
 *
 *****************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************************************/

/***************************************************************************/
/* Include files. */
#include <gdc_ldw_configuration.hpp>
#include <gdc_ldw_messages.hpp>
#include <gdc_ldw_types.hpp>
#include "iniparser.h"
#include "apexcv_pro_hough.h"
#include "apexcv_pro_remap.h"
#include <oal.h>
#include <ShowMessage.h>
#include <cmath>
#include <limits>
#include <algorithm>
#include <string.h>
#include <stdint.h>
#include <toggle_regression.h>
#include <Kernels_Configuration.h>

#include "Font7x13.h"
#include "nxp_logo.h"

using namespace apexcv;

/***************************************************************************/
/* Local Literals & definitions */

GdcLdw_MovingAverage::GdcLdw_MovingAverage()
    : mSum(0), mCount_Current(0), mCount_Max(1) // Must be positive otherwise might lead to division by zero
{
}

GdcLdw_MovingAverage::~GdcLdw_MovingAverage()
{
}

void GdcLdw_MovingAverage::Initialize(const uint32_t cCount_Max)
{
  mCount_Max = cCount_Max;
}

uint64_t GdcLdw_MovingAverage::Update(const uint64_t cNewValue)
{
  uint64_t lAverage = 0;

  if(mCount_Current < mCount_Max)
  {
    mSum += cNewValue;
    ++mCount_Current;
    lAverage = mSum / mCount_Current;
  }
  else
  {
    mSum -= mSum / mCount_Current;
    mSum += cNewValue;
    lAverage = mSum / mCount_Current;
  }

  return lAverage;
}

uint64_t GdcLdw_MovingAverage::Get() const
{
  return (mSum / mCount_Current);
}

/**********************************************************************************************************/
// GdcLdw_ConstantValueFilter function definitions
/**********************************************************************************************************/

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
GdcLdw_ConstantValueFilter::GdcLdw_ConstantValueFilter()
    : //ICP_KalmanFilter<float> mKalman;
      //ICP_Mat<float>          mMeasurementMat;
      mIsInitialized(false)
{
}

GdcLdw_LinePolar::GdcLdw_LinePolar() : mRho(0), mAngleInDegrees(0), mAngleInRadians(0.0), mVotes(0)
{
}

GdcLdw_LinePolar::GdcLdw_LinePolar(int32_t rho, int32_t angle, int32_t votes)
    : mRho(rho), mAngleInDegrees(angle), mAngleInRadians(0.0), mVotes(votes)
{
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_ConstantValueFilter::Initialize(const float cMeasurementNoiseVar,
                                                  const float cPostErrorVar,
                                                  const float cInitialValue,
                                                  const float cProcessNoiseVar)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsInitialized == false)
  {
    libResult = mKalman.Initialize(1, 1, 0);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    // mKalman Filter Parameters
    libResult = mKalman.mStateTransitionMatrix.SetIdentity(1);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = mKalman.mMeasurementMatrix.SetIdentity(1);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = mKalman.mProcessNoiseCovariance.SetIdentity(cProcessNoiseVar);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = mKalman.mMeasurementNoiseCovariance.SetIdentity(cMeasurementNoiseVar);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    // Initial Conditions
    libResult = mKalman.mCorrectedState.SetIdentity(cInitialValue);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = mKalman.mPosterioriErrorCovariance.SetIdentity(cPostErrorVar);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    // Generate the first prediction state
    libResult = mKalman.Predict();
    SHOW_ERROR(libResult == LIB_SUCCESS);

    // Allocate measurement matrix
    libResult = mMeasurementMat.Create(1, 1);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    mIsInitialized = true;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_ConstantValueFilter::Reset(const float cMeasurementNoiseVar,
                                             const float cPostErrorVar,
                                             const float cInitialValue,
                                             const float cProcessNoiseVar)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsInitialized == true)
  {
    libResult = mKalman.Initialize(1, 1, 0);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    // mKalman Filter Parameters
    libResult = mKalman.mStateTransitionMatrix.SetIdentity(1);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = mKalman.mMeasurementMatrix.SetIdentity(1);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = mKalman.mProcessNoiseCovariance.SetIdentity(cProcessNoiseVar);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = mKalman.mMeasurementNoiseCovariance.SetIdentity(cMeasurementNoiseVar);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    // Initial Conditions
    libResult = mKalman.mCorrectedState.SetIdentity(cInitialValue);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = mKalman.mPosterioriErrorCovariance.SetIdentity(cPostErrorVar);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    // Generate the first prediction state
    libResult = mKalman.Predict();
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_ConstantValueFilter::NextMeasurement(const float cMeasurement)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsInitialized == true)
  {
    mMeasurementMat(0, 0) = cMeasurement;

    libResult = mKalman.Correct(mMeasurementMat);

    if(libResult == LIB_SUCCESS)
    {
      libResult = mKalman.Predict();
    }
  }
  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
float GdcLdw_ConstantValueFilter::GetValue()
{
  return mKalman.mCorrectedState(0, 0);
}

int GdcLdw_Remap::convertBirdEyeLinesToScene(const uint16_t cBirdEyeWidth,    // input
                                             const ICP_Line birdEyeLeftLane,  // input
                                             const ICP_Line birdEyeRightLane, // input
                                             ICP_Line&      sceneLeftLane,    // output
                                             ICP_Line&      sceneRightLane)        // output
{
  int8_t             rval               = 0;
  ParamLoader&       rParamLoader       = *((ParamLoader*)mpParamLoader);
  PerspectiveMapper& rPerspectiveMapper = *((PerspectiveMapper*)mpGroundToWarp);
  if(cBirdEyeWidth != 0)
  {
    uint16_t            cHalfBirdEyeWidth = cBirdEyeWidth >> 1;
    CameraMapper::Point groundPt, warpPt;
    groundPt.x =
        (std::max((int)birdEyeLeftLane.point1.x, 0) * mTopViewPixelToMeterRatioX) + rParamLoader.topGroundROI.x;
    groundPt.y = (birdEyeLeftLane.point1.y * mTopViewPixelToMeterRatioY) + rParamLoader.topGroundROI.y;
    groundPt.x = groundPt.x < -cHalfBirdEyeWidth ? -cHalfBirdEyeWidth
                                                 : groundPt.x > cHalfBirdEyeWidth ? cHalfBirdEyeWidth : groundPt.x;
    rPerspectiveMapper.map(groundPt, warpPt);
    sceneLeftLane.point1.x = warpPt.x;
    sceneLeftLane.point1.y = warpPt.y;

    groundPt.x =
        (std::max((int)birdEyeLeftLane.point2.x, 0) * mTopViewPixelToMeterRatioX) + rParamLoader.topGroundROI.x;
    groundPt.y = (birdEyeLeftLane.point2.y * mTopViewPixelToMeterRatioY) + rParamLoader.topGroundROI.y;
    groundPt.x = groundPt.x < -cHalfBirdEyeWidth ? -cHalfBirdEyeWidth
                                                 : groundPt.x > cHalfBirdEyeWidth ? cHalfBirdEyeWidth : groundPt.x;
    rPerspectiveMapper.map(groundPt, warpPt);
    sceneLeftLane.point2.x = warpPt.x;
    sceneLeftLane.point2.y = warpPt.y;

    groundPt.x = (std::min((int)birdEyeRightLane.point1.x, (int)cBirdEyeWidth) * mTopViewPixelToMeterRatioX) +
                 rParamLoader.topGroundROI.x;
    groundPt.y = (birdEyeRightLane.point1.y * mTopViewPixelToMeterRatioY) + rParamLoader.topGroundROI.y;
    groundPt.x = groundPt.x < -cHalfBirdEyeWidth ? -cHalfBirdEyeWidth
                                                 : groundPt.x > cHalfBirdEyeWidth ? cHalfBirdEyeWidth : groundPt.x;
    rPerspectiveMapper.map(groundPt, warpPt);
    sceneRightLane.point1.x = warpPt.x;
    sceneRightLane.point1.y = warpPt.y;

    groundPt.x = (std::min((int)birdEyeRightLane.point2.x, (int)cBirdEyeWidth) * mTopViewPixelToMeterRatioX) +
                 rParamLoader.topGroundROI.x;
    groundPt.y = (birdEyeRightLane.point2.y * mTopViewPixelToMeterRatioY) + rParamLoader.topGroundROI.y;
    groundPt.x = groundPt.x < -cHalfBirdEyeWidth ? -cHalfBirdEyeWidth
                                                 : groundPt.x > cHalfBirdEyeWidth ? cHalfBirdEyeWidth : groundPt.x;
    rPerspectiveMapper.map(groundPt, warpPt);
    sceneRightLane.point2.x = warpPt.x;
    sceneRightLane.point2.y = warpPt.y;
  }
  else
  {
    rval |= 1;
  }

  return rval;
}

/***************************************************************************/
/**
 * \details
 * This function ...
 *
 **************************************************************************/
GdcLdw_Task::GdcLdw_Task()
    : mStackSizeInBytes(0), mPriority(0), mIsSemaphoreThreadSafetyCreated(false), mIsTaskSpawned(false),
      mIsTaskFinished(false)

{
  LIB_RESULT libResult = LIB_FAILURE;

  mName[0] = 0;

  // mSemaphoreThreadSafety: Starting count should be 1;
  libResult = GDC_OAL_SemaphoreCreate(&mSemaphoreThreadSafety, (char*)gcGdcLdTaskSemaphoreThreadSafetyName,
                                      GDC_OAL_SEMAPHORE_COUNTING, 1);
  SHOW_ERROR(libResult == LIB_SUCCESS);
  if(libResult == LIB_SUCCESS)
  {
    mIsSemaphoreThreadSafetyCreated = true;
  }
}

/***************************************************************************/
/**
 * \details
 * This function ...
 *
 **************************************************************************/
GdcLdw_Task::~GdcLdw_Task()
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsTaskSpawned == true && gcIsRunningWithThreads == true)
  {
    libResult = GdcLdw_Task::Delete();
    SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_FAILURE_UNSUPPORTED_FUNCTION);
  }

  if(mIsSemaphoreThreadSafetyCreated == true)
  {
    libResult = GDC_OAL_SemaphoreDelete(mSemaphoreThreadSafety);
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }
}

/***************************************************************************/
/**
 * \details
 * This function creates and activates a new task.
 * It is equivalent to calling "Init" and "Activate".
 *
 *
 * \param  cStackSizeInBytes - Size in bytes of the tasks stack
 * \param  cTaskPriority     - Specifies a priority value between 0 and 255.
 *                             The lower the numeric value, the higher the tasks priority.
 * \param  cTaskName         - Pointer to a 31 character name for the task.
 *                             This name is shared by the task and its default message queue.
 *                             For name registration to succeed this name must be unique.
 *
 *
 * \return
 *    #LIB_SUCCESS
 *    #LIB_FAILURE
 *    #OAL_ERR_LOG_EINVALIDPARAM
 **************************************************************************/
LIB_RESULT GdcLdw_Task::Spawn(const uint32_t cStackSizeInBytes, const uint32_t cTaskPriority, const char cTaskName[])
{
  if(gcIsRunningWithThreads == true)
  {
    LIB_RESULT libResult    = LIB_FAILURE;
    LIB_RESULT libResultInt = LIB_FAILURE;

    if((cStackSizeInBytes == 0) || (!cTaskName) || (cTaskPriority > 255))
    {
      libResult = OAL_ERR_LOG_EINVALIDPARAM;
    }
    else
    {
      libResult = GDC_OAL_SemaphoreObtain(mSemaphoreThreadSafety);
      SHOW_ERROR(libResult == LIB_SUCCESS);
    }

    if(libResult == LIB_SUCCESS)
    {
      libResult = LIB_FAILURE;

      if(mIsTaskSpawned == false)
      {
        int intResult = -1;

        bool weNeedToDestroyAttr = false;

        pthread_attr_t pthreadAttr;

        intResult = pthread_attr_init(&pthreadAttr);

        if(intResult == 0)
        {
          weNeedToDestroyAttr = true;

          intResult = pthread_attr_setdetachstate(&pthreadAttr, PTHREAD_CREATE_JOINABLE);
          SHOW_ERROR(intResult == 0);
        }

        //int   pthread_attr_setguardsize(pthread_attr_t *, size_t);
        // DEFAULT

        if(intResult == 0)
        {
          intResult = pthread_attr_setinheritsched(&pthreadAttr, PTHREAD_EXPLICIT_SCHED);
          SHOW_ERROR(intResult == 0);
        }

        if(intResult == 0)
        {
          intResult = pthread_attr_setschedpolicy(&pthreadAttr, SCHED_FIFO);
          SHOW_ERROR(intResult == 0);
        }

        sched_param schedParam;

        if(intResult == 0)
        {
          intResult = pthread_attr_getschedparam(&pthreadAttr, &schedParam);
          SHOW_ERROR(intResult == 0);
        }

        if(intResult == 0)
        {
          schedParam.sched_priority += cTaskPriority;
#if !defined(__CYGWIN__) && !defined(_WIN32)
          intResult = pthread_attr_setschedparam(&pthreadAttr, &schedParam);
          SHOW_ERROR(intResult == 0);
#endif
        }

        if(intResult == 0)
        {
#if !defined(__CYGWIN__) && !defined(_WIN32)
          intResult = pthread_attr_setscope(&pthreadAttr, PTHREAD_SCOPE_SYSTEM);
          SHOW_ERROR(intResult == 0);
#endif
        }

        //int   pthread_attr_setstack(pthread_attr_t *, void *, size_t);
        // DEFAULT

        size_t stackSizeDefault = 0;

        if(intResult == 0)
        {
          intResult = pthread_attr_getstacksize(&pthreadAttr, &stackSizeDefault);
          SHOW_ERROR(intResult == 0);
        }

        if(intResult == 0)
        {
          intResult = pthread_attr_setstacksize(
              &pthreadAttr, (stackSizeDefault > cStackSizeInBytes) ? stackSizeDefault : cStackSizeInBytes);
          SHOW_ERROR(intResult == 0);
        }

        pthread_t* pThread = 0;

        if(intResult == 0)
        {
          pThread = (pthread_t*)calloc(1, sizeof(*pThread));

          if(pThread != 0)
          {
            intResult = pthread_create(pThread, NULL, GdcLdw_Task::TaskEntryRoutine, this);
            SHOW_ERROR(intResult == 0);
            if(intResult == 0)
            {
#if !defined(__CYGWIN__) && !defined(_WIN32)
              intResult = pthread_setname_np(*pThread, cTaskName);
              SHOW_ERROR(intResult == 0);
#endif
              if(intResult == 0)
              {
                mTaskControlBlock = (GdcLdw_TCB)pThread;
              }
              else
              {
                printf("0x%X\n", intResult);
              }
            }
          }
          else
          {
            intResult = -1;
            libResult = OAL_ERR_MEMORY_OUT_OF_MEMORY;
          }
        }

        int oldstate = 0;

        if(intResult == 0)
        {
          intResult = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldstate);
          SHOW_ERROR(intResult == 0);
        }

        if(intResult == 0)
        {
          intResult = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldstate);
          SHOW_ERROR(intResult == 0);
        }

        if(weNeedToDestroyAttr == true)
        {
          pthread_attr_destroy(&pthreadAttr);
        }

        if(intResult == 0)
        {
          mStackSizeInBytes = cStackSizeInBytes;
          mPriority         = cTaskPriority;
          strncpy(mName, cTaskName, gcGdcLdTaskNameMaximumSizeInChar);
          mIsTaskSpawned = true;
          libResult      = LIB_SUCCESS;
        }
      }

      libResultInt = GDC_OAL_SemaphoreRelease(mSemaphoreThreadSafety);
      SHOW_ERROR(libResultInt == LIB_SUCCESS);
    }

    return libResult;
  }
  else
  {
    LIB_RESULT libResult = LIB_SUCCESS;

    this->TaskOpen();

    return libResult;
  }
}

/***************************************************************************/
/**
 * \details
 * This function waits for the task to terminate.
 *
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GdcLdw_Task::Join()
{
  if(gcIsRunningWithThreads == true)
  {
    LIB_RESULT libResult    = LIB_FAILURE;
    LIB_RESULT libResultInt = LIB_FAILURE;

    libResult = GDC_OAL_SemaphoreObtain(mSemaphoreThreadSafety);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    if(libResult == LIB_SUCCESS)
    {
      int intResult = -1;

      intResult = pthread_join(*((pthread_t*)mTaskControlBlock), NULL);

      if(intResult == 0)
      {
        libResult = LIB_SUCCESS;
      }

      libResultInt = GDC_OAL_SemaphoreRelease(mSemaphoreThreadSafety);
      SHOW_ERROR(libResultInt == LIB_SUCCESS);
    }

    return libResult;
  }
  else
  {
    LIB_RESULT libResult = LIB_SUCCESS;

    this->TaskClose();

    return libResult;
  }
}

/***************************************************************************/
/**
 * \details
 * This function causes the task to cease to exist; the stack and control block
 * resources are freed. If the task is safe from deletion, This function will block
 * until the task is made unsafe.
 *
 *
 * \return
 *    #LIB_SUCCESS
 *    #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GdcLdw_Task::Delete()
{
  if(gcIsRunningWithThreads == true)
  {
    LIB_RESULT libResult    = LIB_FAILURE;
    LIB_RESULT libResultInt = LIB_FAILURE;

    libResult = GDC_OAL_SemaphoreObtain(mSemaphoreThreadSafety);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    if(libResult == LIB_SUCCESS)
    {
      if(mIsTaskSpawned == true)
      {
        int intResult = -1;

        if(mIsTaskSpawned == true && mIsTaskFinished == false)
        {
          intResult = pthread_cancel(*((pthread_t*)mTaskControlBlock));
        }
        else
        {
          intResult = 0;
        }

        if(intResult == 0 || intResult == 3)
        {
          libResult = LIB_SUCCESS;
        }

        mIsTaskSpawned  = false;
        mIsTaskFinished = false;
      }
      else
      {
        libResult = LIB_FAILURE;
      }

      libResultInt = GDC_OAL_SemaphoreRelease(mSemaphoreThreadSafety);
      SHOW_ERROR(libResultInt == LIB_SUCCESS);
    }

    return libResult;
  }
  else
  {
    LIB_RESULT libResult = LIB_FAILURE;

    return libResult;
  }
}

/***************************************************************************/
/**
 * \details
 * This function ...
 *
 *
 * \param
 *
 *
 **************************************************************************/
void* GdcLdw_Task::TaskEntryRoutine(void* pVoid)
{
  if(gcIsRunningWithThreads == true)
  {
    SHOW_ERROR(pVoid != 0);

    GdcLdw_Task* const cpTask = (GdcLdw_Task*)pVoid;

    cpTask->TaskOpen();
    cpTask->TaskService();
    cpTask->TaskClose();

    cpTask->mIsTaskFinished = true;

    return 0;
  }
  else
  {
    return 0;
  }
}

/**********************************************************************************************************/
// GdcLdw_Helper function definitions
/**********************************************************************************************************/

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::ReadLdwConfigFromIniFile(const char cConfigFilename[], GdcLdw_LdwConfig& rReturnedConfig)
{
  LIB_RESULT libResult = LIB_FAILURE;

  dictionary* pDictionary = 0;
  ;

  pDictionary = iniparser_load(cConfigFilename);

  if(pDictionary != 0)
  {
    rReturnedConfig.mBirdeyeLutFilename[0] = 0;

    const char* pString = 0;

    pString = iniparser_getstring(pDictionary, "BirdEye:LutFileName", 0);

    if(pString != 0 && strlen(pString) != 0)
    {
      strcat(rReturnedConfig.mBirdeyeLutFilename, pString);
    }
    rReturnedConfig.BirdeyeLutOffsetBpp = (int16_t)iniparser_getint(pDictionary, "BirdEye:LutOffsetBpp", 0);
    rReturnedConfig.BirdeyeLutDeltaBpp  = (int16_t)iniparser_getint(pDictionary, "BirdEye:LutDeltaBpp", 0);
    rReturnedConfig.mBirdEyeHeight      = (int16_t)iniparser_getint(pDictionary, "BirdEye:HeightInPixels", 0);
    rReturnedConfig.mBirdEyeWidth       = (int16_t)iniparser_getint(pDictionary, "BirdEye:WidthInPixels", 0);

    rReturnedConfig.mMaxLaneWidth      = (int16_t)iniparser_getint(pDictionary, "Lane:MaxWidthInPixels", 0);
    rReturnedConfig.mMinLaneWidth      = (int16_t)iniparser_getint(pDictionary, "Lane:MinWidthInPixels", 0);
    rReturnedConfig.mInitialKLGap      = (int16_t)iniparser_getint(pDictionary, "Lane:InitKLGap", 0);
    rReturnedConfig.mInitialKCentre    = (int16_t)iniparser_getint(pDictionary, "Lane:InitKCentre", 0);
    rReturnedConfig.mLaneVoteThreshold = (uint8_t)iniparser_getint(pDictionary, "Lane:LaneVoteThreshold", 0);
    rReturnedConfig.mMaxLRAngleDiffInDegrees =
        (int16_t)iniparser_getint(pDictionary, "Lane:MaxLRAngleDiffInDegrees", 0);
    rReturnedConfig.mMaxAngleDeltaInDegrees = (int16_t)iniparser_getint(pDictionary, "Lane:MaxAngleDeltaInDegrees", 0);
    rReturnedConfig.mMaxLaneCenterDelta     = (int16_t)iniparser_getint(pDictionary, "Lane:MaxLaneCenterDelta", 0);
    rReturnedConfig.mWarningWidthFromCenter = (int16_t)iniparser_getint(pDictionary, "Lane:WarningWidthFromCenter", 0);
    rReturnedConfig.mWarningHysteresisPct   = (int16_t)iniparser_getint(pDictionary, "Lane:WarningHysteresisPct", 0);
    rReturnedConfig.mMaxWarningFrameCount   = (int16_t)iniparser_getint(pDictionary, "Lane:MaxWarningFrameCount", 0);

    rReturnedConfig.mOutputReadFilename[0] = 0;

    pString = iniparser_getstring(pDictionary, "Test:ReadFileName", 0);

    if(pString != 0 && strlen(pString) != 0)
    {
      strcat(rReturnedConfig.mOutputReadFilename, pString);
    }

    rReturnedConfig.mOutputWriteFilename[0] = 0;
    pString                                 = iniparser_getstring(pDictionary, "Test:WriteFileName", 0);

    if(pString != 0 && strlen(pString) != 0)
    {
      strcat(rReturnedConfig.mOutputWriteFilename, pString);
    }

    rReturnedConfig.mWeReadOutputFile  = (iniparser_getint(pDictionary, "Test:Read", 0) == 1);
    rReturnedConfig.mWeWriteOutputFile = (iniparser_getint(pDictionary, "Test:Write", 0) == 1);

    iniparser_freedict(pDictionary);
    pDictionary = 0;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

  /***************************************************************************/
  /**
 * \par Description
 *
 **************************************************************************/
  //static int32_t gSebMagic = 0;

#ifndef _TOGGLE_REG_GRAPH_
LIB_RESULT GdcLdw_Helper::AcfFilters_no_reg(ACF_FILTERS_NO_REG& rAcfNoRegProcess,
                                            vsdk::UMat&         rcImageSrc_umat,
                                            vsdk::UMat&         rcLutDelta_umat,
                                            vsdk::UMat&         rcLutOffset_umat,
                                            vsdk::UMat&         rcLutBlockOffset_umat,
                                            vsdk::UMat&         rcVoteThreshold_umat,
                                            vsdk::UMat&         rcLaneData_umat,
                                            vsdk::UMat&         rcPreviousThreshold_umat,
                                            vsdk::UMat&         rcBirdEyeSobelDst_umat,
                                            vsdk::UMat&         rcLeftMax_umat,  // used for regression
                                            vsdk::UMat&         rcRightMax_umat, // used for regerssion
                                            vsdk::UMat&         rcPoints_umat)           // used for regression
{
  LIB_RESULT libResult = LIB_FAILURE;

  int8_t intResult = 0;

  /* We need to know the number of accumulators so that we can conduct a
    * best fit regression on the data (this correlates to number of points). */
#include "Kernels_Configuration.h"

  //int32_t num_cu;

  intResult |= rAcfNoRegProcess.Initialize();
  //num_cu = rAcfNoRegProcess.RetCuArrayWidth();
  //printf("CU:%d\n", num_cu);
  // set 32CUs and apex0
  //rAcfNoRegProcess.SelectApuConfiguration(ACF_APU_CFG__APU_0_CU_0_31_SMEM_0_1, 0);
  // remap

  intResult |= rAcfNoRegProcess.ConnectIndirectInput("SRC_IMG", rcImageSrc_umat, rcLutBlockOffset_umat);

  intResult |= rAcfNoRegProcess.ConnectIO("LOCAL_OFFSET", rcLutOffset_umat);
  intResult |= rAcfNoRegProcess.ConnectIO("DELTA", rcLutDelta_umat);
  intResult |= rAcfNoRegProcess.ConnectIO("THRESHOLD_PREVIOUS", rcPreviousThreshold_umat);
  intResult |= rAcfNoRegProcess.ConnectIO("DST_IMG", rcBirdEyeSobelDst_umat);
  intResult |= rAcfNoRegProcess.ConnectIO("THRESHOLD", rcPreviousThreshold_umat);
  intResult |= rAcfNoRegProcess.ConnectIO("VOTE_THRESHOLD", rcVoteThreshold_umat);
  intResult |= rAcfNoRegProcess.ConnectIO("MAX_LEFT", rcLeftMax_umat);
  intResult |= rAcfNoRegProcess.ConnectIO("MAX_RIGHT", rcRightMax_umat);
  intResult |= rAcfNoRegProcess.ConnectIO("POINTS_DATA", rcPoints_umat);

  intResult |= rAcfNoRegProcess.SelectScenario("LOCAL_OFFSET", 4, 2);
  intResult |= rAcfNoRegProcess.OverridePortChunkSize("SRC_IMG", 12, 4);

  intResult |= rAcfNoRegProcess.Start();
  intResult |= rAcfNoRegProcess.Wait();

  // Linear regression calls
  vsdk::Mat lLeftMax_mat  = rcLeftMax_umat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
  vsdk::Mat lRightMax_mat = rcRightMax_umat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
  vsdk::Mat lPoints_umat  = rcPoints_umat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
  vsdk::Mat lLaneData_mat = rcLaneData_umat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);

  RegressionLine(*((uint8_t*)lLeftMax_mat.data), (uint32_t*)(lPoints_umat.data), gcDataOutStride,
                 (uint32_t*)((uint32_t*)lLaneData_mat.data));

  RegressionLine(*((uint8_t*)lRightMax_mat.data), (((uint32_t*)lPoints_umat.data) + 3), gcDataOutStride,
                 (uint32_t*)(((uint32_t*)lLaneData_mat.data) + 6));

  if(intResult == 0)
  {
    libResult = LIB_SUCCESS;
  }

  return libResult;
}
#endif // end: #ifndef _TOGGLE_REG_GRAPH_

#ifdef _TOGGLE_REG_GRAPH_
LIB_RESULT GdcLdw_Helper::AcfFilters_reg(ACF_FILTERS_REG&     rAcfRegProcess,
                                         icp::DataDescriptor& rcImageSrcCdd,
                                         icp::DataDescriptor& rcLutDeltaCdd,
                                         icp::DataDescriptor& rcLutOffsetCdd,
                                         icp::DataDescriptor& rcLutBlockOffsetCdd,
                                         icp::DataDescriptor& rcVoteThresholdCdd,
                                         icp::DataDescriptor& rcLaneDataCdd,
                                         icp::DataDescriptor& rcPreviousThresholdCdd,
                                         icp::DataDescriptor& rcBirdEyeSobelDstCdd)
{
  LIB_RESULT libResult = LIB_FAILURE;

  int intResult = 0;
  intResult |= rAcfRegProcess.Initialize();
  //remap
  intResult |= rAcfRegProcess.ConnectIndirectInput("SRC_IMG", rcImageSrcCdd, rcLutBlockOffsetCdd);
  intResult |= rAcfRegProcess.ConnectIO("LOCAL_OFFSET", rcLutOffsetCdd);
  intResult |= rAcfRegProcess.ConnectIO("DELTA", rcLutDeltaCdd);
  intResult |= rAcfRegProcess.ConnectIO("THRESHOLD_PREVIOUS", rcPreviousThresholdCdd);
  intResult |= rAcfRegProcess.ConnectIO("DST_IMG", rcBirdEyeSobelDstCdd);
  intResult |= rAcfRegProcess.ConnectIO("THRESHOLD", rcPreviousThresholdCdd);
  intResult |= rAcfRegProcess.ConnectIO("VOTE_THRESHOLD", rcVoteThresholdCdd);
  intResult |= rAcfRegProcess.ConnectIO("LANE_DATA", rcLaneDataCdd);

  intResult |= rAcfRegProcess.Start();
  intResult |= rAcfRegProcess.Wait();

  if(intResult == 0)
  {
    libResult = LIB_SUCCESS;
  }

  return libResult;
}

#endif // end: #ifdef _TOGGLE_REG_GRAPH_

// Linear Regression on ARM
#ifndef _TOGGLE_REG_GRAPH_
const uint32_t gcSectionHeight  = TOPVIEW_HEIGHT / gcNumOfAccumalators;
const uint32_t halfTopviewWidth = TOPVIEW_WIDTH / 2;
const uint8_t  cMinPointsFound  = 2; // minimum number of valid points required to make a line

int16_t GdcLdw_Helper::FastArcTan(float x)
{
  return (int16_t)fabsf((M_PI_4 * x - x * (fabsf(x) - 1) * (0.2447f + 0.0663f * fabsf(x))) * 180 / M_PI);
}

void GdcLdw_Helper::RegressionLine(const uint8_t   cMaxVotes,
                                   const uint32_t* cPointsData,
                                   const uint8_t   cDataStride,
                                   uint32_t*       rLane)
{
  // TODO magic number 2
  const uint8_t cMinValidVotes = cMaxVotes / 2;
  uint32_t      sumY = 0, sumX = 0, sumXY = 0, sumX2 = 0, x, y, validPointsCounter = 0;

  // Find sums for linear regression
  for(uint8_t i = 0; i < gcNumOfAccumalators; i++)
  {
    x = TOPVIEW_HEIGHT - (TOPVIEW_HEIGHT / gcNumOfAccumalators / 2) - (TOPVIEW_HEIGHT / gcNumOfAccumalators * i);
    y = cPointsData[i * cDataStride + 1];

    // Check for valid points
    if(cPointsData[i * cDataStride] > cMinValidVotes && cPointsData[i * cDataStride + 2])
    {
      sumY += y;
      sumXY += y * x;
      sumX += x;
      sumX2 += x * x;
      ++validPointsCounter;
    } // end if: check for valid points
  }   // end for: find sums

  // CASE 1: No valid lane information
  if(validPointsCounter < cMinPointsFound)
  {
    rLane[0] = 0;
  }
  // CASE 2: Lanes were found
  else
  {
    rLane[0] = 1;

    float   xMean       = (float)sumX / validPointsCounter;
    float   yMean       = (float)sumY / validPointsCounter;
    int32_t denominator = sumX2 - sumX * xMean;
    float   slope       = (sumXY - sumX * yMean) / denominator;
    float   yInt        = yMean - slope * xMean;

    rLane[1] = 180 + FastArcTan(slope);

    rLane[2] = 0; // get rid of this?
    rLane[3] = TOPVIEW_HEIGHT - 1;
    rLane[4] = (uint32_t)(rLane[3] * slope + yInt);
    rLane[5] = (uint32_t)yInt;
  }
} // end: regressionLine()
#endif

void GdcLdw_Helper::SetLogo(int32_t const cWidthInPixels,
                            int32_t const /*cHeightInPixels*/,
                            uint32_t const cHorizontalOffsetInPixels,
                            uint32_t const cVerticalOffsetInPixels,
                            uint8_t* const cpBuffer)
{
  const uint32_t cPixelSizeInBytes = 3;
  const uint32_t cLineSpanInBytes  = (cWidthInPixels * cPixelSizeInBytes);

  uint8_t* pLine      = (uint8_t*)(cpBuffer + ((cVerticalOffsetInPixels * cLineSpanInBytes) +
                                          cHorizontalOffsetInPixels * cPixelSizeInBytes));
  uint32_t pLogoPixel = NXP_LOGO_HEIGHT * NXP_LOGO_WIDTH * cPixelSizeInBytes - 1;
  uint8_t* pPixel     = pLine;

  pLogoPixel = 0;
  for(uint32_t i = 0; i < NXP_LOGO_HEIGHT; i++)
  {
    for(uint32_t j = 0; j < (NXP_LOGO_WIDTH); j++)
    {
      if((nxp_logo[pLogoPixel + 2] != 0xff) || (nxp_logo[pLogoPixel + 1] != 0xff) || (nxp_logo[pLogoPixel] != 0xff))
      {
        pPixel[0] = nxp_logo[pLogoPixel + 2];
        pPixel[1] = nxp_logo[pLogoPixel + 1];
        pPixel[2] = nxp_logo[pLogoPixel];
      }
      pPixel += cPixelSizeInBytes;
      pLogoPixel += cPixelSizeInBytes;
    }
    pLine += cLineSpanInBytes;
    pPixel = pLine;
  }
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::UpdateLaneRawData(const vsdk::UMat& rcLaneData_umat,
                                            GdcLdw_LinePolar& rLeftLanePolar,
                                            GdcLdw_LinePolar& rRightLanePolar,
                                            ICP_Line&         rLeftLaneCartesian,
                                            ICP_Line&         rRightLaneCartesian)
{
  LIB_RESULT libResult = LIB_SUCCESS;

  rLeftLanePolar.mAngleInDegrees = 180;
  rLeftLanePolar.mRho            = 0;
  rLeftLanePolar.mVotes          = 0;
  rLeftLaneCartesian.point1.x    = 0;
  rLeftLaneCartesian.point1.y    = 0;
  rLeftLaneCartesian.point2.x    = 0;
  rLeftLaneCartesian.point2.y    = 0;

  rRightLanePolar.mAngleInDegrees = 180;
  rRightLanePolar.mRho            = 0;
  rRightLanePolar.mVotes          = 0;
  rRightLaneCartesian.point1.x    = 0;
  rRightLaneCartesian.point1.y    = 0;
  rRightLaneCartesian.point2.x    = 0;
  rRightLaneCartesian.point2.y    = 0;

  vsdk::Mat lLaneData_mat = rcLaneData_umat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);

  uint32_t* pLaneData  = (uint32_t*)lLaneData_mat.data;
  uint8_t   dataStride = rcLaneData_umat.cols;

  if(pLaneData != 0)
  {
    rLeftLanePolar.mVotes          = pLaneData[0];
    rLeftLanePolar.mAngleInDegrees = pLaneData[1];
    rLeftLaneCartesian.point1.x    = pLaneData[4];
    rLeftLaneCartesian.point1.y    = pLaneData[2];
    rLeftLaneCartesian.point2.x    = pLaneData[5];
    rLeftLaneCartesian.point2.y    = pLaneData[3];

    rRightLanePolar.mVotes          = pLaneData[dataStride + 0];
    rRightLanePolar.mAngleInDegrees = pLaneData[dataStride + 1];
    rRightLaneCartesian.point1.x    = pLaneData[dataStride + 4];
    rRightLaneCartesian.point1.y    = pLaneData[dataStride + 2];
    rRightLaneCartesian.point2.x    = pLaneData[dataStride + 5];
    rRightLaneCartesian.point2.y    = pLaneData[dataStride + 3];
  }
  else
  {
    libResult = LIB_FAILURE;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description  Uses the APU kernel for Hough transform.
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::FindLanesHough(vsdk::UMat&       rcBirdEyeSobelMedian_umat,
                                         const uint8_t     cMinGrayscaleValue,
                                         const int32_t     cVoteThreshold,
                                         GdcLdw_LinePolar& rLeftLanePolar,     // OUT
                                         GdcLdw_LinePolar& rRightLanePolar,    // OUT
                                         ICP_Line&         rLeftLaneCartesian, // OUT
                                         ICP_Line&         rRightLaneCartesian)        // OUT
{
  static int firstPass = true;

  LIB_RESULT libResult = LIB_SUCCESS;

  rLeftLanePolar.mAngleInDegrees = 0;
  rLeftLanePolar.mRho            = 0;
  rLeftLanePolar.mVotes          = 0;

  rRightLanePolar.mAngleInDegrees = 0;
  rRightLanePolar.mRho            = 0;
  rRightLanePolar.mVotes          = 0;

  rLeftLaneCartesian.point1.x = 0;
  rLeftLaneCartesian.point1.y = 0;
  rLeftLaneCartesian.point2.x = 0;
  rLeftLaneCartesian.point2.y = 0;

  rRightLaneCartesian.point1.x = 0;
  rRightLaneCartesian.point1.y = 0;
  rRightLaneCartesian.point2.x = 0;
  rRightLaneCartesian.point2.y = 0;

  const int32_t cWidthInPixels  = rcBirdEyeSobelMedian_umat.cols;
  const int32_t cHeightInPixels = rcBirdEyeSobelMedian_umat.rows;
  //const int32_t cSpanInBytes    = rcBirdEyeSobelMedian_umat.GetSpan();

  vsdk::Mat lBirdEyeSobelMedian_mat = rcBirdEyeSobelMedian_umat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);

  uint8_t* pPixel = (uint8_t*)lBirdEyeSobelMedian_mat.data;

  static icp::DataDescriptor lLinesBufferSize; // The count of lLinesBufferOut
  static icp::DataDescriptor lLinesBufferOut;  // each line is a packed 32 bit value

  //static uint16_t accumulator_size = (uint16_t)(sqrt((float)(cWidthInPixels*cWidthInPixels)+(cHeightInPixels*cHeightInPixels))+3);

  if(firstPass)
  {
    firstPass = false;
  }

  // Detect Lines
  if(libResult == LIB_SUCCESS && pPixel != 0)
  {
    apexcv::HoughLineDetector hough;
    hough.Initialize(rcBirdEyeSobelMedian_umat, cMinGrayscaleValue, cVoteThreshold, 60,
                     150.0 * apexcv::HoughLineDetector::mcDeg2Rad, (double)3.49e-2f, 0);
    hough.Process();

    // Find the best candidates for the left and right lanes using polar coordinates
    const int32_t cHalfWidthInPixels = cWidthInPixels >> 1;
    //const int32_t cHalfHeightInPixels = cHeightInPixels >> 1;

    const int       lineCount   = hough.GetLineCount();
    const uint32_t* packedLines = hough.GetPackedLineData();

    for(int i = 0; i < lineCount; i++)
    {
      // convert theta/rho to cartesian - see hough reference code
      // NB Hough Transform APU kernel packs the theta/rho/vote counts as shown.

      uint32_t                        packedLine = packedLines[i];
      int                             votes      = apexcv::HoughLineDetector::GetAccumulator(packedLine);
      apexcv::HoughLineDetector::Line line       = hough.GetLine(packedLine);

      int32_t midX = cHalfWidthInPixels + (int32_t)(line.rho / cos(line.theta));

      // Check against best fit for either left or right lane, as appropriate
      if(midX < (cHalfWidthInPixels - 2) && // SASHBY: Reference code did -2 but I can't explain why
         votes >= rLeftLanePolar.mVotes)
      {
        rLeftLanePolar.mAngleInDegrees = line.theta * (float)apexcv::HoughLineDetector::mcRad2Deg;
        rLeftLanePolar.mAngleInRadians = line.theta;
        rLeftLanePolar.mRho            = line.rho;
        rLeftLanePolar.mVotes          = votes;
      }
      else if(midX > (cHalfWidthInPixels + 2) && // SASHBY: Reference code did +2 but I can't explain why
              votes > rRightLanePolar.mVotes)
      {
        rRightLanePolar.mAngleInDegrees = line.theta * (float)apexcv::HoughLineDetector::mcRad2Deg;
        rRightLanePolar.mAngleInRadians = line.theta;
        rRightLanePolar.mRho            = line.rho;
        rRightLanePolar.mVotes          = votes;
      }
    }

    // If we found lanes then convert them to cartesian coordinates
    if(rLeftLanePolar.mVotes > 0)
    {
      if(rLeftLanePolar.mRho < 0)
      {
        rLeftLanePolar.mAngleInRadians = rLeftLanePolar.mAngleInRadians + (float)apexcv::HoughLineDetector::mcPi;
        rLeftLanePolar.mAngleInDegrees = rLeftLanePolar.mAngleInDegrees + 180;
        rLeftLanePolar.mRho            = -rLeftLanePolar.mRho;
      }

      float midX = cHalfWidthInPixels + (int32_t)((rLeftLanePolar.mRho) / cos(rLeftLanePolar.mAngleInRadians));
      float offX = -(int32_t)((tan(rLeftLanePolar.mAngleInRadians) * cHalfWidthInPixels) + 0.5);

      // not in degrees, converted to radians above
      rLeftLaneCartesian.point1.x = (int32_t)(midX + offX);
      rLeftLaneCartesian.point1.y = cHeightInPixels - 1;
      rLeftLaneCartesian.point2.x = (int32_t)(midX - offX);
      rLeftLaneCartesian.point2.y = 0;
    }

    if(rRightLanePolar.mVotes > 0)
    {
      if(rRightLanePolar.mRho > 0)
      {
        rRightLanePolar.mAngleInRadians = rRightLanePolar.mAngleInRadians + (float)apexcv::HoughLineDetector::mcPi;
        rRightLanePolar.mAngleInDegrees = rRightLanePolar.mAngleInDegrees + 180;
        rRightLanePolar.mRho            = -rRightLanePolar.mRho;
      }
      float midX = cHalfWidthInPixels + (int32_t)((rRightLanePolar.mRho) / cos(rRightLanePolar.mAngleInRadians));
      float offX = -(int32_t)((tan(rRightLanePolar.mAngleInRadians) * cHalfWidthInPixels) + 0.5);

      rRightLaneCartesian.point1.x = (int32_t)(midX + offX);
      rRightLaneCartesian.point1.y = cHeightInPixels - 1;
      rRightLaneCartesian.point2.x = (int32_t)(midX - offX);
      rRightLaneCartesian.point2.y = 0;
    }
  }
  else
  {
    libResult = LIB_FAILURE;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::ConvertBirdEyeToScene(const ICP_Mat< float >& rcBirdEyeToSceneMat,
                                                ICP_Mat< float >&       rTempPointMat1,
                                                ICP_Mat< float >&       rTempPointMat2,
                                                const ICP_Line&         rcBirdEyeLeftLane,
                                                const ICP_Line&         rcBirdEyeRightLane,
                                                ICP_Line&               rSceneLeftLane,
                                                ICP_Line&               rSceneRightLane)
{
  LIB_RESULT libResult = LIB_FAILURE;

  float sceneX = 0.0;
  float sceneY = 0.0;
  float sceneZ = 0.0;

  const int32_t cNumberOfPoints = 4; // Number of elements in the following arrays

  const ICP_Point* const cpcBirdEyePoint[] = {&(rcBirdEyeLeftLane.point1), &(rcBirdEyeLeftLane.point2),
                                              &(rcBirdEyeRightLane.point1), &(rcBirdEyeRightLane.point2)};

  ICP_Point* const cpScenePoint[] = {&(rSceneLeftLane.point1), &(rSceneLeftLane.point2), &(rSceneRightLane.point1),
                                     &(rSceneRightLane.point2)};

  for(int32_t i = 0; i < cNumberOfPoints; ++i)
  {
    // set coordinate on the source image
    libResult = rTempPointMat1.SetValue(cpcBirdEyePoint[i]->x, 0, 0, 0);
    libResult = rTempPointMat1.SetValue(cpcBirdEyePoint[i]->y, 1, 0, 0);
    libResult = rTempPointMat1.SetValue(1.0, 2, 0, 0); // z == 1

    // multiply
    rcBirdEyeToSceneMat.Multiply(rTempPointMat1, rTempPointMat2);

    libResult = rTempPointMat2.GetValue(sceneX, 0, 0, 0);
    libResult = rTempPointMat2.GetValue(sceneY, 1, 0, 0);
    libResult = rTempPointMat2.GetValue(sceneZ, 2, 0, 0);

    // guard against division by zero
    if(sceneZ != 0.0)
    {
      cpScenePoint[i]->x = (sceneX / sceneZ);
      cpScenePoint[i]->y = (sceneY / sceneZ);
    }
    else
    {
      cpScenePoint[i]->x = 0.0;
      cpScenePoint[i]->y = 0.0;
    }
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::ConvertSceneToBirdEyeMeters(const ICP_Mat< float >& cSceneToBirdEyeMat,
                                                      const ICP_Mat< float >& cBirdEyePixelsToMetersMat,
                                                      ICP_Mat< float >&       rTempPointMat1,
                                                      ICP_Mat< float >&       rTempPointMat2,
                                                      const float             cVehiclePositionY,
                                                      float&                  rVehiclePositionMeters)
{
  LIB_RESULT libResult = LIB_FAILURE;

  // set coordinate on the source image
  libResult = rTempPointMat1.SetValue(1.0, 0, 0, 0);
  libResult = rTempPointMat1.SetValue(cVehiclePositionY, 1, 0, 0);
  libResult = rTempPointMat1.SetValue(1.0, 2, 0, 0); // z == 1

  // multiply
  libResult = cSceneToBirdEyeMat.Multiply(rTempPointMat1, rTempPointMat2);
  SHOW_ERROR(libResult == LIB_SUCCESS && rTempPointMat2(2, 0) != 0.0);

  libResult = rTempPointMat2.Scale((1.0 / rTempPointMat2(2, 0)), rTempPointMat2);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  libResult = cBirdEyePixelsToMetersMat.Multiply(rTempPointMat2, rTempPointMat1);
  SHOW_ERROR(libResult == LIB_SUCCESS && rTempPointMat1(2, 0) != 0.0);

  libResult = rTempPointMat1.Scale((1.0 / rTempPointMat1(2, 0)), rTempPointMat1);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  rVehiclePositionMeters = -(rTempPointMat1(1, 0));

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::ShouldWeKeepTracking(const ICP_Rectangle& rTrackingRectangle,
                                               const ICP_Line&      rSceneLeftLane,
                                               const ICP_Line&      rSceneRightLane,
                                               const float          cVehicleToLaneWidthRatio,
                                               const int32_t        cMinRectangleHeight,
                                               bool&                rWeKeepTracking)
{
  LIB_RESULT libResult = LIB_SUCCESS;

  rWeKeepTracking = true;

  // Get intersection point of both lanes
  const float xR  = rSceneRightLane.point1.x;
  const float yR  = rSceneRightLane.point1.y;
  const float xR2 = rSceneRightLane.point2.x;
  const float yR2 = rSceneRightLane.point2.y;
  const float xL  = rSceneLeftLane.point1.x;
  const float yL  = rSceneLeftLane.point1.y;
  const float xL2 = rSceneLeftLane.point2.x;
  const float yL2 = rSceneLeftLane.point2.y;

  //slope and intersect for the right line
  const float m1 = (yR2 - yR) / (xR2 - xR);
  const float b1 = yR - m1 * xR;
  //                   for the left line
  const float m2 = (yL2 - yL) / (xL2 - xL);
  const float b2 = yL - m2 * xL;

  const int32_t cRectangleCenterX             = rTrackingRectangle.x + rTrackingRectangle.width / 2;
  const int32_t cRectangleCenterY             = rTrackingRectangle.y + rTrackingRectangle.height;
  const int32_t cLeftLaneRectangleXIntersect  = (cRectangleCenterY - b2) / m2;
  const int32_t cRightLaneRectangleXIntersect = (cRectangleCenterY - b1) / m1;

  const float xposLn = (rTrackingRectangle.y + rTrackingRectangle.height - b2) / m2;
  const float xposRn = (rTrackingRectangle.y + rTrackingRectangle.height - b1) / m1;

  if(cRectangleCenterX > cRightLaneRectangleXIntersect || cRectangleCenterX < cLeftLaneRectangleXIntersect)
  {
    rWeKeepTracking = false;
  }
  else if(((rTrackingRectangle.width / (xposRn - xposLn)) < cVehicleToLaneWidthRatio) ||
          (rTrackingRectangle.height <= cMinRectangleHeight))
  {
    rWeKeepTracking = false;
  }
  else if((rTrackingRectangle.width == 0) || (rTrackingRectangle.height == 0))
  {
    rWeKeepTracking = false;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::Sobel3x3_RowSum(void* const                cpSceneRoiCached,
                                          const icp::DataDescriptor& rcSceneRoiCdd,
                                          void* const                cpGradientsCached,
                                          icp::DataDescriptor& /*rGradientsCdd*/,
                                          const int16_t cDelta)
{
  LIB_RESULT libResult = LIB_SUCCESS;

  //printf("%p, %p\n", cpSceneRoiCached, cpGradientsCached);
  //OAL_TaskSleep(100000);

  const uint32_t cSpanInBytes = rcSceneRoiCdd.GetSpan();
  const uint8_t* pcRowCurr    = (uint8_t*)cpSceneRoiCached;
  const uint8_t* pcRowPrev    = pcRowCurr - cSpanInBytes;
  const uint8_t* pcRowNext    = pcRowCurr + cSpanInBytes;

  uint16_t* const cpGradient = (uint16_t*)cpGradientsCached;

  int32_t        rowSum     = 0;
  const uint32_t cRoiHeight = rcSceneRoiCdd.GetHeight();
  const uint32_t cRoiWidth  = rcSceneRoiCdd.GetWidth();

  for(uint32_t y = 0; y < cRoiHeight; ++y)
  {
    // MIDDLE PIXEL
    for(uint32_t x = 0; x < cRoiWidth; ++x)
    {
      // DX/DY: [ -1020, +1020 ] ((1+2+1)*255)
      // DY: 0, 1, 3
      const int16_t dy = ((pcRowNext[x - 1] - pcRowPrev[x - 1])) + ((pcRowNext[x] - pcRowPrev[x]) * 2) +
                         ((pcRowNext[x + 1] - pcRowPrev[x + 1])) + cDelta;

      rowSum += (dy < 0) ? (0) : ((dy > 255) ? (255) : (dy)); // Limit return values to 8 bits
    }

    cpGradient[y] = fabs((float)(rowSum - 1024)); // SASHBY: Reference code does -1024 but I can't explain why

    pcRowPrev += cSpanInBytes;
    pcRowNext += cSpanInBytes;
    rowSum = 0;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::Median5x1_HRep(void* const cpGradientCached,
                                         const icp::DataDescriptor& /*rcCcdOfGradient*/,
                                         void* const          cpMedianCached,
                                         icp::DataDescriptor& rcCcdOfMedian)
{
  LIB_RESULT libResult = LIB_SUCCESS;

  const uint16_t* const cpcGradient = (uint16_t*)cpGradientCached;
  uint16_t* const       pcMedian    = (uint16_t*)cpMedianCached;

  const int32_t cHeight = rcCcdOfMedian.GetHeight();

  uint16_t temp[5] = {0};

  // FIRST PIXEL
  // REP: -1 -> 0
  // REP: -2 -> 0
  temp[0] = temp[1] = temp[2] = cpcGradient[0];
  temp[3]                     = cpcGradient[1];
  temp[4]                     = cpcGradient[2];
  GdcLdw_Helper::Median5(temp);
  pcMedian[0] = temp[0]; // Index 0 is the result of Median5

  // SECOND PIXEL
  // REP: -1 -> 0
  temp[0] = temp[1] = cpcGradient[0];
  temp[2]           = cpcGradient[1];
  temp[3]           = cpcGradient[2];
  temp[4]           = cpcGradient[3];
  GdcLdw_Helper::Median5(temp);
  pcMedian[1] = temp[0]; // Index 0 is the result of Median5

  // MIDDLE PIXEL
  for(int32_t y = 2; y < (cHeight - 2); ++y)
  {
    temp[0] = cpcGradient[y - 2];
    temp[1] = cpcGradient[y - 1];
    temp[2] = cpcGradient[y];
    temp[3] = cpcGradient[y + 1];
    temp[4] = cpcGradient[y + 2];
    GdcLdw_Helper::Median5(temp);
    pcMedian[y] = temp[0]; // Index 0 is the result of Median5
  }

  // SECOND LAST PIXEL
  temp[0] = cpcGradient[cHeight - 4];
  temp[1] = cpcGradient[cHeight - 3];
  temp[2] = cpcGradient[cHeight - 2];
  temp[3] = temp[4] = cpcGradient[cHeight - 1];
  GdcLdw_Helper::Median5(temp);
  pcMedian[cHeight - 2] = temp[0]; // Index 0 is the result of Median5

  // LAST PIXEL
  temp[0] = cpcGradient[cHeight - 3];
  temp[1] = cpcGradient[cHeight - 2];
  temp[2] = temp[3] = temp[4] = cpcGradient[cHeight - 1];
  GdcLdw_Helper::Median5(temp);
  pcMedian[cHeight - 1] = temp[0]; // Index 0 is the result of Median5

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::Median5(uint16_t cpArray[5])
{
  LIB_RESULT libResult = LIB_SUCCESS;

  // starting from first 4 values
  // cpArray[0]: MIN, cpArray[3]: MAX
  uint16_t temp = 0;

  if(cpArray[3] < cpArray[0])
  {
    temp       = cpArray[3]; // Swap 3 and 0
    cpArray[3] = cpArray[0];
    cpArray[0] = temp;
  }

  if(cpArray[1] < cpArray[0])
  {
    temp       = cpArray[1]; // Swap 1 and 0
    cpArray[1] = cpArray[0];
    cpArray[0] = temp;
  }
  else if(cpArray[1] > cpArray[3])
  {
    temp       = cpArray[1]; // Swap 1 and 3
    cpArray[1] = cpArray[3];
    cpArray[3] = temp;
  }

  if(cpArray[2] < cpArray[0])
  {
    temp       = cpArray[2]; // Swap 2 and 0
    cpArray[2] = cpArray[0];
    cpArray[0] = temp;
  }
  else if(cpArray[2] > cpArray[3])
  {
    temp       = cpArray[2]; // Swap 2 and 3
    cpArray[2] = cpArray[3];
    cpArray[3] = temp;
  }

  if(cpArray[1] > cpArray[2])
  {
    temp       = cpArray[2]; // Swap 2 and 1
    cpArray[2] = cpArray[1];
    cpArray[1] = temp;
  }

  // [1], [2] + last value
  if(cpArray[4] > cpArray[2])
  {
    cpArray[0] = cpArray[2];
  }
  else if(cpArray[4] < cpArray[1])
  {
    cpArray[0] = cpArray[1];
  }
  else
  {
    cpArray[0] = cpArray[4];
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::SendRequest(const int32_t cMessageId,
                                      const int32_t cSenderQueueId,
                                      const int32_t cReceiverQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  MessageBase* pRequest = (MessageBase*)malloc(sizeof(*pRequest));
  SHOW_ERROR(pRequest != 0);

  if(pRequest != 0)
  {
    new((void*)pRequest) MessageBase; // Placement new

    pRequest->mMessageId     = cMessageId;
    pRequest->mSenderId      = cSenderQueueId; // not used, because no reply is expected.
    pRequest->mCorrelationId = 0;              // not used, because no reply is expected.

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pRequest, sizeof(*pRequest), 1);

    if(libResult != LIB_SUCCESS)
    {
      free((void*)pRequest);
      pRequest = 0;
    }
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::SendImageReply(const int32_t  cReceiverQueueId,
                                         vsdk::UMat&    rSceneRgb24_umat,
                                         const void*    pcSceneRgb24_umat_data,
                                         vsdk::UMat&    rScene_umat,
                                         const void*    pcScene_umat_data,
                                         const uint32_t cSceneCounter,
                                         const uint64_t (&cTicks)[2])
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcLdw_ImageReply* pReply = (GdcLdw_ImageReply*)malloc(sizeof(GdcLdw_ImageReply));

  if(pReply != 0)
  {
    new(pReply) GdcLdw_ImageReply(); // Placement new

    pReply->mMessageId              = MESSAGE_ID_GDC_LDW_IMAGE_REPLY;
    pReply->mSenderId               = 0; // not used, because no reply is expected.
    pReply->mCorrelationId          = 0; // not used, because no reply is expected.
    pReply->mSceneRgb24_umat        = rSceneRgb24_umat;
    pReply->mpcSceneRgb24_umat_data = pcSceneRgb24_umat_data;
    pReply->mScene_umat             = rScene_umat;
    pReply->mpcScene_umat_data      = pcScene_umat_data;
    pReply->mSceneCounter           = cSceneCounter;
    std::copy(cTicks, cTicks + 2, pReply->mTicks);

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pReply, sizeof(*pReply), 1);
    if(libResult != LIB_SUCCESS)
    {
      pReply->~GdcLdw_ImageReply();
      free((void*)pReply);
      pReply = 0;
    }
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::SendDetectionData(const int32_t   cReceiverQueueId,
                                            vsdk::UMat&     rSceneRgb24_umat,
                                            const void*     pcSceneRgb24_umat_data,
                                            vsdk::UMat&     rScene_umat,
                                            const void*     pcScene_umat_data,
                                            const uint32_t  cSceneCounter,
                                            void*           cpTopViewImage,
                                            const ICP_Line& rcLeftLane,
                                            const ICP_Line& rcRightLane,
                                            const bool      cNoLanesFound,
                                            const bool      cWeDepartedLeft,
                                            const bool      cWeDepartedRight,
                                            const uint64_t (&cTicks)[gcApexCvEndTick])
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcLdw_DetectionData* pRequest = (GdcLdw_DetectionData*)malloc(sizeof(GdcLdw_DetectionData));

  if(pRequest != 0)
  {
    new(pRequest) GdcLdw_DetectionData(); // Placement new

    pRequest->mMessageId     = MESSAGE_ID_GDC_LDW_DETECTION_DATA;
    pRequest->mSenderId      = 0; // not used, because no reply is expected.
    pRequest->mCorrelationId = 0; // not used, because no reply is expected.

    pRequest->mSceneRgb24_umat        = rSceneRgb24_umat;
    pRequest->mpcSceneRgb24_umat_data = pcSceneRgb24_umat_data;
    pRequest->mScene_umat             = rScene_umat;
    pRequest->mpcScene_umat_data      = pcScene_umat_data;
    pRequest->mSceneCounter           = cSceneCounter;
    pRequest->mhTopViewImage          = cpTopViewImage;
    pRequest->mLeftLane               = rcLeftLane;
    pRequest->mRightLane              = rcRightLane;
    pRequest->mNoLanesFound           = cNoLanesFound;
    pRequest->mWeDepartedLeft         = cWeDepartedLeft;
    pRequest->mWeDepartedRight        = cWeDepartedRight;

    std::copy(cTicks, cTicks + gcApexCvEndTick, pRequest->mTicks);

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pRequest, sizeof(*pRequest), 1);
    if(libResult != LIB_SUCCESS)
    {
      pRequest->~GdcLdw_DetectionData();
      free((void*)pRequest);
      pRequest = 0;
    }
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::SendCompositionData(const int32_t cReceiverQueueId,
                                              vsdk::UMat&   rSceneRgb24_umat,
                                              const void*   pcSceneRgb24_umat_data)
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcLdw_CompositionData* pData = (GdcLdw_CompositionData*)malloc(sizeof(GdcLdw_CompositionData));

  if(pData != 0)
  {
    new(pData) GdcLdw_CompositionData(); // Placement new

    pData->mMessageId              = MESSAGE_ID_GDC_LDW_COMPOSITION_DATA;
    pData->mSenderId               = 0; // not used, because no reply is expected.
    pData->mCorrelationId          = 0; // not used, because no reply is expected.
    pData->mSceneRgb24_umat        = rSceneRgb24_umat;
    pData->mpcSceneRgb24_umat_data = pcSceneRgb24_umat_data;

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pData, sizeof(*pData), 1);

    if(libResult != LIB_SUCCESS)
    {
      pData->~GdcLdw_CompositionData();
      free((void*)pData);
      pData = 0;
    }
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::FreeImageReply(GdcLdw_ImageReply* const cpReply)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(cpReply != 0)
  {
    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::FreeDetectionData(GdcLdw_DetectionData* const cpRequest)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(cpRequest != 0)
  {
    if(cpRequest->mhTopViewImage != 0)
    {
      OAL_MemoryFree(cpRequest->mhTopViewImage);
    }

    cpRequest->mhTopViewImage = 0;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::FreeCompositionData(GdcLdw_CompositionData* const cpData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(cpData != 0)
  {
    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
void GdcLdw_Helper::RenderTextToRgb24Buffer(int32_t const cWidthInPixels,
                                            int32_t const /*cHeightInPixels*/,
                                            uint32_t const       xOffset_text,
                                            uint32_t const       yOffset_text,
                                            const uint8_t* const aString,
                                            uint32_t const       cTextColor,
                                            uint8_t* const       cpConvertedImage,
                                            uint32_t const       cScale)
{
  const uint8_t redColor   = (cTextColor >> 24) & 0xff;
  const uint8_t greenColor = (cTextColor >> 16) & 0xff;
  const uint8_t blueColor  = (cTextColor >> 8) & 0xff;
  //const uint8_t redColorInvert   = 0xFF - redColor;
  //const uint8_t greenColorInvert = 0xFF - greenColor;
  //const uint8_t blueColorInvert  = 0xFF - blueColor;

  //const uint8_t transColor  = (cTextColor) & 0xff;     // unused
  uint8_t*       pCurrentFontMaskLine = 0;
  uint8_t*       pCurrentPixel        = 0;
  const uint8_t* pCurrentCharacter    = aString;
  const uint32_t cPixelSizeInBytes    = 3;
  const uint32_t cLineSpanInBytes     = ((cWidthInPixels - (TEXT_WIDTH_7x13 * cScale)) * cPixelSizeInBytes);

  //
  // Compute start of first character
  // Destination buffer is broken down into character cells of 7 lines of 13 pixels
  //
  uint8_t* pCurrentPixelCell =
      (uint8_t*)(cpConvertedImage + ((yOffset_text * cWidthInPixels) + xOffset_text) * cPixelSizeInBytes);

  if(cScale == 1 || cScale == 2 || cScale == 3 || cScale == 4)
  {
    //
    // Render string
    //
    pCurrentPixel = pCurrentPixelCell;
    do
    {
      pCurrentFontMaskLine = (uint8_t*)&Font7x13[(uint8_t)(*pCurrentCharacter)][0];
      //
      // Render character a line at a time, pixel by pixel
      //
      for(uint32_t y = 0; y < TEXT_HEIGHT_7x13 * cScale; ++y) // Height should be 13
      {
        //
        // Do each pixel in character scanline.
        //
        for(uint32_t x = 0; x < (TEXT_WIDTH_7x13)*cScale; ++x) // Width should be 7
        {
          if((*pCurrentFontMaskLine << (x / cScale)) & 0x80) // 0x80 is there to isolate pixels in the mask line.
          {
            pCurrentPixel[0] = blueColor;
            pCurrentPixel[1] = greenColor;
            pCurrentPixel[2] = redColor;
          }
          pCurrentPixel += cPixelSizeInBytes;
        }
        if((y % cScale) == 1)
        {
          ++pCurrentFontMaskLine;
        }

        //
        // Compute next test scanline address.
        //
        pCurrentPixel += cLineSpanInBytes;
      }
      //
      // Compute offset to start of next character and remember it
      //
      pCurrentPixelCell += ((TEXT_WIDTH_7x13 - 1) * cScale) * cPixelSizeInBytes; // Each cell is 7 pixel wide
      pCurrentPixel = pCurrentPixelCell;
      ++pCurrentCharacter;
    } while(*pCurrentCharacter != '\0');
  }
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
int32_t GdcLdw_Helper::ConvertToBitsPerPixel(const icp::DATATYPE cColorModel)
{
  int32_t bitsPerPixel = 0;

  switch(cColorModel)
  {
    case icp::DATATYPE_32U:
    {
      bitsPerPixel = 32;
      break;
    }
    case icp::DATATYPE_16U:
    {
      bitsPerPixel = 16;
      break;
    }
    case icp::DATATYPE_08U:
    {
      bitsPerPixel = 8;
      break;
    }
    default:
    {
      bitsPerPixel = 0;
      break;
    }
  }

  return bitsPerPixel;
}

/***************************************************************************/
/**
 * \par Description
 * Function used to convert a set of RGB24 pixels to the Y pixel format
 * It is required since apexcv::ColorConverter does not support RGB24 to Y conversion
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::CropAndConvertRgb24ToY(vsdk::UMat&    rSensorRgb24_umat,
                                                 vsdk::UMat&    rSceneRgb24_umat,
                                                 vsdk::UMat&    rSceneY_umat,
                                                 uint32_t const cR2Y,
                                                 uint32_t const cG2Y,
                                                 uint32_t const cB2Y)
{
  LIB_RESULT libResult = LIB_FAILURE;

  const uint32_t cSensorRgb24_WidthInPixels  = rSensorRgb24_umat.cols;
  const uint32_t cSensorRgb24_HeightInPixels = rSensorRgb24_umat.rows;

  const uint32_t cSceneRgb24_WidthInPixels  = rSceneRgb24_umat.cols;
  const uint32_t cSceneRgb24_HeightInPixels = rSceneRgb24_umat.rows;

  const uint32_t cSceneY_WidthInPixels  = rSceneY_umat.cols;
  const uint32_t cSceneY_HeightInPixels = rSceneY_umat.rows;

  if(cSensorRgb24_WidthInPixels == cSceneRgb24_WidthInPixels && cSensorRgb24_WidthInPixels == cSceneY_WidthInPixels &&
     cSensorRgb24_HeightInPixels == cSceneRgb24_HeightInPixels && cSensorRgb24_HeightInPixels == cSceneY_HeightInPixels)
  {
    const uint32_t cRnd = 128;
    const uint32_t cScl = 8;

    const uint32_t cSensorRgb24_PixelSizeInBytes = rSensorRgb24_umat.elemSize();
    const uint32_t cSensorRgb24_SpanInBytes      = rSensorRgb24_umat.step[0];
    const uint32_t cSceneRgb24_PixelSizeInBytes  = rSceneRgb24_umat.elemSize();
    const uint32_t cSceneRgb24_SpanInBytes       = rSceneRgb24_umat.step[0];
    const uint32_t cSceneY_PixelSizeInBytes      = rSceneY_umat.elemSize();
    const uint32_t cSceneY_SpanInBytes           = rSceneY_umat.step[0];

    vsdk::Mat lSensorRgb24_mat = rSensorRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);
    vsdk::Mat lSceneRgb24_mat  = rSceneRgb24_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
    vsdk::Mat lSceneY_mat      = rSceneY_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);

    uint8_t* pSensorRgb24_Line  = (uint8_t*)(lSensorRgb24_mat.data);
    uint8_t* pSensorRgb24_Pixel = pSensorRgb24_Line;

    uint8_t* pSceneRgb24_Line  = (uint8_t*)(lSceneRgb24_mat.data);
    uint8_t* pSceneRgb24_Pixel = pSceneRgb24_Line;

    uint8_t* pSceneY_Line  = (uint8_t*)(lSceneY_mat.data);
    uint8_t* pSceneY_Pixel = pSceneY_Line;

    for(uint32_t y = 0; y < cSensorRgb24_HeightInPixels; ++y) // For every line
    {
      for(uint32_t x = 0; x < cSensorRgb24_WidthInPixels; ++x) // For every pixel on that line
      {
        pSceneRgb24_Pixel[0] = pSensorRgb24_Pixel[0];
        pSceneRgb24_Pixel[1] = pSensorRgb24_Pixel[1];
        pSceneRgb24_Pixel[2] = pSensorRgb24_Pixel[2];

        const uint16_t cY =
            (cRnd + (pSensorRgb24_Pixel[0] * cR2Y) + (pSensorRgb24_Pixel[1] * cG2Y) + (pSensorRgb24_Pixel[2] * cB2Y)) >>
            cScl;

        *pSceneY_Pixel = (cY > 255) ? 255 : (uint8_t)cY;

        pSensorRgb24_Pixel += cSensorRgb24_PixelSizeInBytes;
        pSceneRgb24_Pixel += cSceneRgb24_PixelSizeInBytes;
        pSceneY_Pixel += cSceneY_PixelSizeInBytes;
      }

      pSensorRgb24_Line += cSensorRgb24_SpanInBytes;
      pSceneRgb24_Line += cSceneRgb24_SpanInBytes;
      pSceneY_Line += cSceneY_SpanInBytes;

      pSensorRgb24_Pixel = pSensorRgb24_Line;
      pSceneRgb24_Pixel  = pSceneRgb24_Line;
      pSceneY_Pixel      = pSceneY_Line;
    }

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * Function used to convert a set of Y pixels to the RGB24 pixel format
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::ConvertYToRgb24(vsdk::UMat& rSource_umat, vsdk::UMat& rDestination_umat)
{
  LIB_RESULT libResult = LIB_FAILURE;

  const uint32_t cSourceWidthInPixels  = rSource_umat.cols;
  const uint32_t cSourceHeightInPixels = rSource_umat.rows;

  vsdk::Mat            lSource_mat    = rSource_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);
  const uint8_t* const cpcSourceFrame = (const uint8_t* const)lSource_mat.data;

  if(cpcSourceFrame != 0 && rDestination_umat.empty() == false)
  {
    const uint32_t cPixelInSizeInBytes  = 1; // Y
    const uint32_t cPixelOutSizeInBytes = 3; // RGB24

    const uint32_t cSourceSpanInBytes = rSource_umat.step[0];

    const uint8_t* pcLineIn = (const uint8_t*)cpcSourceFrame;

    const uint8_t* pcPixelIn = pcLineIn;

    vsdk::Mat lDestination_mat = rDestination_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
    uint8_t*  pPixelOut        = (uint8_t*)lDestination_mat.data;

    for(uint32_t y = 0; y < cSourceHeightInPixels; ++y)
    {
      for(uint32_t x = 0; x < cSourceWidthInPixels; ++x)
      {
        pPixelOut[0] = *pcPixelIn;
        pPixelOut[1] = *pcPixelIn;
        pPixelOut[2] = *pcPixelIn;

        pcPixelIn += cPixelInSizeInBytes;
        pPixelOut += cPixelOutSizeInBytes;
      }

      pcLineIn += cSourceSpanInBytes;
      pcPixelIn = pcLineIn;
    }

    libResult = LIB_SUCCESS;
  }
  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * Function used to convert a set of RGB24 pixels to the Y pixel format
 * It is required since apexcv::ColorConverter does not support RGB24 to Y conversion
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::ConvertRgb24ToY(uint32_t const cSourceWidthInPixels,
                                          uint32_t const cSourceHeightInLines,
                                          uint32_t const (*const cpcSourceRoi)[4], // Left, Top, Width, Height
                                          const uint8_t* const cpcSourceFrame,
                                          icp::DataDescriptor& rDestinationDd,
                                          uint32_t const       cR2Y,
                                          uint32_t const       cG2Y,
                                          uint32_t const       cB2Y)
{
  LIB_RESULT libResult = LIB_SUCCESS;

  const uint32_t cSrcPixelSizeInBytes = 3; // RGB24
  const uint32_t cDesPixelSizeInBytes = 1; // Y

  const uint32_t cRnd = 128;
  const uint32_t cScl = 8;

  const uint32_t cDstLineSpanInBytes = rDestinationDd.GetSpan();

  uint8_t* pDstLine  = (uint8_t*)(rDestinationDd.GetDataPtr());
  uint8_t* pDstPixel = pDstLine;

  if(cpcSourceRoi == 0)
  {
    const uint8_t* pcSrcPixel = (const uint8_t*)cpcSourceFrame;

    for(uint32_t y = 0; y < cSourceHeightInLines; ++y) // For every line
    {
      for(uint32_t x = 0; x < cSourceWidthInPixels; ++x) // For every pixel on that line
      {
        const uint16_t cY = (cRnd + (pcSrcPixel[0] * cR2Y) + (pcSrcPixel[1] * cG2Y) + (pcSrcPixel[2] * cB2Y)) >> cScl;

        *pDstPixel = (cY > 255) ? 255 : (uint8_t)cY;

        pcSrcPixel += cSrcPixelSizeInBytes;
        pDstPixel += cDesPixelSizeInBytes;
      }

      pDstLine += cDstLineSpanInBytes;
      pDstPixel = pDstLine;
    }
  }
  else if((((*cpcSourceRoi)[0] + (*cpcSourceRoi)[2]) <= cSourceWidthInPixels) &&
          (((*cpcSourceRoi)[1] + (*cpcSourceRoi)[3]) <= cSourceHeightInLines))
  {
    const uint32_t cSrcLineSpanInBytes = cSourceWidthInPixels * cSrcPixelSizeInBytes;

    const uint8_t* pcSrcLine = ((const uint8_t*)cpcSourceFrame) + ((*cpcSourceRoi)[0] * cSrcPixelSizeInBytes) +
                               ((*cpcSourceRoi)[1] * cSrcLineSpanInBytes);
    const uint8_t* pcSrcPixel = pcSrcLine;

    for(uint32_t y = 0; y < (*cpcSourceRoi)[3]; ++y) // For every line
    {
      for(uint32_t x = 0; x < (*cpcSourceRoi)[2]; ++x) // For every pixel on that line
      {
        const uint16_t cY = (cRnd + (pcSrcPixel[0] * cR2Y) + (pcSrcPixel[1] * cG2Y) + (pcSrcPixel[2] * cB2Y)) >> cScl;

        *pDstPixel = (cY > 255) ? 255 : (uint8_t)cY;

        pcSrcPixel += cSrcPixelSizeInBytes;
        pDstPixel += cDesPixelSizeInBytes;
      }

      pcSrcLine += cSrcLineSpanInBytes;
      pcSrcPixel = pcSrcLine;

      pDstLine += cDstLineSpanInBytes;
      pDstPixel = pDstLine;
    }
  }
  else
  {
    libResult = LIB_FAILURE;
  }
  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * Function used to crop a RGB24 image
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::CropRgb24(uint32_t const cSourceWidthInPixels,
                                    uint32_t const cSourceHeightInLines,
                                    uint32_t const (*const cpcSourceRoi)[4], // Left, Top, Width, Height
                                    const uint8_t* const cpcSourceFrame,
                                    uint8_t* const       cpcDestinationFrame)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(cpcSourceRoi != 0 && (((*cpcSourceRoi)[0] + (*cpcSourceRoi)[2]) <= cSourceWidthInPixels) &&
     (((*cpcSourceRoi)[1] + (*cpcSourceRoi)[3]) <= cSourceHeightInLines))
  {
    const uint32_t cRgb24BytesPerPixel = 3;

    const uint32_t cSrcLineSpanInBytes = cSourceWidthInPixels * cRgb24BytesPerPixel;

    const uint8_t* pcSrcLine = ((const uint8_t*)cpcSourceFrame) + ((*cpcSourceRoi)[0] * cRgb24BytesPerPixel) +
                               ((*cpcSourceRoi)[1] * cSrcLineSpanInBytes);

    const uint32_t cDstLineWidthInBytes = (*cpcSourceRoi)[2] * cRgb24BytesPerPixel;
    const uint32_t cDstLineSpanInBytes  = (*cpcSourceRoi)[2] * cRgb24BytesPerPixel;

    uint8_t* pDstLine = cpcDestinationFrame;

    for(uint32_t y = 0; y < (*cpcSourceRoi)[3]; ++y) // For every line
    {
      memcpy(pDstLine, pcSrcLine, cDstLineWidthInBytes);

      pcSrcLine += cSrcLineSpanInBytes;
      pDstLine += cDstLineSpanInBytes;
    }

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * Calculates the optimal sensor region of interest(ROI), based on the scene dimensions
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::CalculateSensorRoi(const uint32_t      cSensorWidthInPixels,
                                             const uint32_t      cSensorHeightInPixels,
                                             const uint32_t      cSceneWidthInPixels,
                                             const uint32_t      cSceneHeightInPixels,
                                             /*out*/ vsdk::Rect& rRect)
{
  LIB_RESULT libResult = LIB_FAILURE;

  // 1 - Find the aspect ratio of the output
  uint32_t swapTemp            = 0;
  uint32_t originalNumerator   = cSceneWidthInPixels;
  uint32_t originalDenominator = cSceneHeightInPixels;

  while(originalDenominator != 0)
  {
    swapTemp            = originalDenominator;
    originalDenominator = originalNumerator % originalDenominator;
    originalNumerator   = swapTemp;
  }
  const uint32_t cGreatestCommonDivisor  = originalNumerator;
  const uint32_t cAspectRatioNumerator   = cSceneWidthInPixels / cGreatestCommonDivisor;
  const uint32_t cAspectRatioDenominator = cSceneHeightInPixels / cGreatestCommonDivisor;

  // 2 - Find the largest mask with that ratio which will fit in the sensor resolution
  const uint32_t cSensorWidthAndAspectRatioNumeratorQuotient    = cSensorWidthInPixels / cAspectRatioNumerator;
  const uint32_t cSensorHeightAndAspectRatioDenominatorQuotient = cSensorHeightInPixels / cAspectRatioDenominator;

  rRect.width = cSensorWidthAndAspectRatioNumeratorQuotient <= cSensorHeightAndAspectRatioDenominatorQuotient
                    ? cSensorWidthAndAspectRatioNumeratorQuotient * cAspectRatioNumerator
                    : cSensorHeightAndAspectRatioDenominatorQuotient * cAspectRatioNumerator;

  rRect.height = rRect.width * cAspectRatioDenominator / cAspectRatioNumerator;

  rRect.x = (cSensorWidthInPixels - rRect.width) / 2;

  rRect.y = (cSensorHeightInPixels - rRect.height) / 2;

  libResult = LIB_SUCCESS;

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::DrawIcon(const icp::DATATYPE /*cColorModel*/,
                                   int32_t const cDotX,
                                   int32_t const cDotY,
                                   int32_t const cDotWidth,
                                   int32_t const cDotHeight,
                                   int32_t const cFrameWidth,
                                   int32_t const cFrameHeight,
                                   char const    cpSourceIcon[],
                                   void* const   cpDestinationFrame)
{
  LIB_RESULT libResult = LIB_FAILURE;

  int32_t rotatedDotX = cDotX;
  int32_t rotatedDotY = cDotY;

  const int32_t cAdjustedDotX = rotatedDotX <= cFrameWidth ? rotatedDotX : cFrameWidth;
  const int32_t cAdjustedDotY = rotatedDotY <= cFrameHeight ? rotatedDotY : cFrameHeight;
  const int32_t cAdjustedDotWidth =
      (cFrameWidth - cAdjustedDotX) <= cDotWidth ? (cFrameWidth - cAdjustedDotX) : cDotWidth;
  const int32_t cAdjustedDotHeight =
      (cFrameHeight - cAdjustedDotY) <= cDotHeight ? (cFrameHeight - cAdjustedDotY) : cDotHeight;

  if(cAdjustedDotWidth * cAdjustedDotHeight > 0)
  {
    int32_t const  cPixelSizeInBytes  = 3; //GdcLdw_Helper::ConvertToBitsPerPixel(cColorModel) / 8;
    int32_t const  cDestinationOffset = ((cAdjustedDotY * cFrameWidth) + cAdjustedDotX) * cPixelSizeInBytes;
    uint8_t* const cpDestinationStart = (uint8_t*)((intptr_t)cpDestinationFrame + cDestinationOffset);

    if(cPixelSizeInBytes == 3)
    {
      int32_t j = cAdjustedDotHeight;

      uint8_t* pDestLineStart = (uint8_t*)cpDestinationStart;
      uint8_t* pSourceStart   = (uint8_t*)cpSourceIcon;

      do
      {
        for(uint8_t* i = pDestLineStart; i < pDestLineStart + (cAdjustedDotWidth * cPixelSizeInBytes);
            i += cPixelSizeInBytes)
        {
          if(pSourceStart[3] != 0x0)
          {
            i[0] = ((pSourceStart[2] * pSourceStart[3]) + (i[0] * (0xff - pSourceStart[3]))) >> 8;
            i[1] = ((pSourceStart[1] * pSourceStart[3]) + (i[0] * (0xff - pSourceStart[3]))) >> 8;
            i[2] = ((pSourceStart[0] * pSourceStart[3]) + (i[0] * (0xff - pSourceStart[3]))) >> 8;
          }
          pSourceStart = ((uint8_t*)pSourceStart + 4);
        }

        pDestLineStart = ((uint8_t*)pDestLineStart) + (cFrameWidth * cPixelSizeInBytes);

        j--;
      } while(j > 0);

      libResult = LIB_SUCCESS;
    }
  }
  else
  {
    libResult = LIB_SUCCESS;
  }

  return libResult;
}

LIB_RESULT GdcLdw_Helper::DrawTopView(const icp::DATATYPE cColorModel,
                                      int32_t const       cDotX,
                                      int32_t const       cDotY,
                                      int32_t const       cDotWidth,
                                      int32_t const       cDotHeight,
                                      int32_t const       cFrameWidth,
                                      int32_t const       cFrameHeight,
                                      void* const         cpSourceFrame,
                                      void* const         cpDestinationFrame)
{
  LIB_RESULT libResult = LIB_FAILURE;

  const int32_t cAdjustedDotX = cDotX <= cFrameWidth ? cDotX : cFrameWidth;
  const int32_t cAdjustedDotY = cDotY <= cFrameHeight ? cDotY : cFrameHeight;
  const int32_t cAdjustedDotWidth =
      (cFrameWidth - cAdjustedDotX) <= cDotWidth ? (cFrameWidth - cAdjustedDotX) : cDotWidth;
  const int32_t cAdjustedDotHeight =
      (cFrameHeight - cAdjustedDotY) <= cDotHeight ? (cFrameHeight - cAdjustedDotY) : cDotHeight;

  if(cAdjustedDotWidth * cAdjustedDotHeight > 0)
  {
    int32_t const  cPixelSizeInBytes  = GdcLdw_Helper::ConvertToBitsPerPixel(cColorModel) / 8;
    int32_t const  cDestinationOffset = ((cAdjustedDotY * cFrameWidth) + cAdjustedDotX) * cPixelSizeInBytes;
    uint8_t* const cpDestinationStart = (uint8_t*)((intptr_t)cpDestinationFrame + cDestinationOffset);

    if(cPixelSizeInBytes == 1)
    {
      int32_t j = cAdjustedDotHeight;

      uint8_t* pDestLineStart = (uint8_t*)cpDestinationStart;
      uint8_t* pSrcLineStart  = (uint8_t*)cpSourceFrame;

      do
      {
        for(uint8_t *i = pDestLineStart; i < pDestLineStart + cAdjustedDotWidth; i++, pSrcLineStart++)
        { //white
          *i = *pSrcLineStart;
        }
        pDestLineStart = ((uint8_t*)pDestLineStart) + cFrameWidth;

        j--;
      } while(j > 0);

      libResult = LIB_SUCCESS;
    }
  }
  else
  {
    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::DrawLineToRgb24Buffer(const int32_t  cImageWidthInPixels,
                                                const int32_t  cImageHeightInPixels,
                                                const int32_t  cLineX1,
                                                const int32_t  cLineY1,
                                                const int32_t  cLineX2,
                                                const int32_t  cLineY2,
                                                uint32_t const cColorValue,
                                                void* const    cpImageBuffer)
{
  LIB_RESULT libResult = LIB_SUCCESS;

  if(cpImageBuffer != 0)
  {
    const uint32_t cBitsPerPixel     = 24;
    const uint32_t cImageSpanInBytes = cImageWidthInPixels * cBitsPerPixel / 8;

    int32_t x0 = cLineX1;
    int32_t y0 = cLineY1;
    int32_t x1 = cLineX2;
    int32_t y1 = cLineY2;

    int dy = y1 - y0;
    int dx = x1 - x0;
    int stepx, stepy;

    if(dy < 0)
    {
      dy    = -dy;
      stepy = -1;
    }
    else
    {
      stepy = 1;
    }
    if(dx < 0)
    {
      dx    = -dx;
      stepx = -1;
    }
    else
    {
      stepx = 1;
    }

    GdcLdw_Helper::PutPixel(x0, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue,
                            cImageHeightInPixels);
    GdcLdw_Helper::PutPixel(x1, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue,
                            cImageHeightInPixels);
    if(dx > dy)
    {
      int length = (dx - 1) >> 2;
      int extras = (dx - 1) & 3;
      int incr2  = (dy << 2) - (dx << 1);
      if(incr2 < 0)
      {
        int c     = dy << 1;
        int incr1 = c << 1;
        int d     = incr1 - dx;
        for(int i = 0; i < length; i++)
        {
          x0 += stepx;
          x1 -= stepx;

          if(d < 0)
          { // Pattern:
            GdcLdw_Helper::PutPixel(x0, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue,
                                    cImageHeightInPixels); //
            GdcLdw_Helper::PutPixel(x0 += stepx, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels); //  x o o
            GdcLdw_Helper::PutPixel(x1, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue,
                                    cImageHeightInPixels); //
            GdcLdw_Helper::PutPixel(x1 -= stepx, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            d += incr1;
          }
          else
          {
            if(d < c)
            { // Pattern:
              GdcLdw_Helper::PutPixel(x0, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue,
                                      cImageHeightInPixels); //      o
              GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels); //  x o
              GdcLdw_Helper::PutPixel(x1, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue,
                                      cImageHeightInPixels); //
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
            }
            else
            {
              GdcLdw_Helper::PutPixel(x0, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels); // Pattern:
              GdcLdw_Helper::PutPixel(x0 += stepx, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels); //    o o
              GdcLdw_Helper::PutPixel(x1, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels); //  x
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels); //
            }
            d += incr2;
          }
        }
        if(extras > 0)
        {
          if(d < 0)
          {
            GdcLdw_Helper::PutPixel(x0 += stepx, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            if(extras > 1)
              GdcLdw_Helper::PutPixel(x0 += stepx, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
            if(extras > 2)
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
          }
          else if(d < c)
          {
            GdcLdw_Helper::PutPixel(x0 += stepx, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            if(extras > 1)
              GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
            if(extras > 2)
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
          }
          else
          {
            GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            if(extras > 1)
              GdcLdw_Helper::PutPixel(x0 += stepx, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
            if(extras > 2)
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
          }
        }
      }
      else
      {
        int c     = (dy - dx) << 1;
        int incr1 = c << 1;
        int d     = incr1 + dx;
        for(int i = 0; i < length; i++)
        {
          x0 += stepx;
          x1 -= stepx;
          if(d > 0)
          {
            GdcLdw_Helper::PutPixel(x0, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels); // Pattern:
            GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels); //      o
            GdcLdw_Helper::PutPixel(x1, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels); //    o
            GdcLdw_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels); //  x
            d += incr1;
          }
          else
          {
            if(d < c)
            {
              GdcLdw_Helper::PutPixel(x0, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue,
                                      cImageHeightInPixels); // Pattern:
              GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels); //      o
              GdcLdw_Helper::PutPixel(x1, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue,
                                      cImageHeightInPixels); //  x o
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels); //
            }
            else
            {
              GdcLdw_Helper::PutPixel(x0, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels); // Pattern:
              GdcLdw_Helper::PutPixel(x0 += stepx, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels); //    o o
              GdcLdw_Helper::PutPixel(x1, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels); //  x
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels); //
            }
            d += incr2;
          }
        }
        if(extras > 0)
        {
          if(d > 0)
          {
            GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            if(extras > 1)
              GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
            if(extras > 2)
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
          }
          else if(d < c)
          {
            GdcLdw_Helper::PutPixel(x0 += stepx, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            if(extras > 1)
              GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
            if(extras > 2)
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
          }
          else
          {
            GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            if(extras > 1)
              GdcLdw_Helper::PutPixel(x0 += stepx, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
            if(extras > 2)
            {
              if(d > c)
                GdcLdw_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel,
                                        (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
              else
                GdcLdw_Helper::PutPixel(x1 -= stepx, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                        cColorValue, cImageHeightInPixels);
            }
          }
        }
      }
    }
    else
    {
      int length = (dy - 1) >> 2;
      int extras = (dy - 1) & 3;
      int incr2  = (dx << 2) - (dy << 1);
      if(incr2 < 0)
      {
        int c     = dx << 1;
        int incr1 = c << 1;
        int d     = incr1 - dy;
        for(int i = 0; i < length; i++)
        {
          y0 += stepy;
          y1 -= stepy;
          if(d < 0)
          {
            GdcLdw_Helper::PutPixel(x0, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue,
                                    cImageHeightInPixels);
            GdcLdw_Helper::PutPixel(x0, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            GdcLdw_Helper::PutPixel(x1, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue,
                                    cImageHeightInPixels);
            GdcLdw_Helper::PutPixel(x1, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            d += incr1;
          }
          else
          {
            if(d < c)
            {
              GdcLdw_Helper::PutPixel(x0, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue,
                                      cImageHeightInPixels);
              GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
              GdcLdw_Helper::PutPixel(x1, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue,
                                      cImageHeightInPixels);
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
            }
            else
            {
              GdcLdw_Helper::PutPixel(x0 += stepx, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
              GdcLdw_Helper::PutPixel(x0, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
              GdcLdw_Helper::PutPixel(x1, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
            }
            d += incr2;
          }
        }
        if(extras > 0)
        {
          if(d < 0)
          {
            GdcLdw_Helper::PutPixel(x0, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            if(extras > 1)
              GdcLdw_Helper::PutPixel(x0, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
            if(extras > 2)
              GdcLdw_Helper::PutPixel(x1, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
          }
          else if(d < c)
          {
            GdcLdw_Helper::PutPixel(stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            if(extras > 1)
              GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
            if(extras > 2)
              GdcLdw_Helper::PutPixel(x1, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
          }
          else
          {
            GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            if(extras > 1)
              GdcLdw_Helper::PutPixel(x0, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
            if(extras > 2)
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
          }
        }
      }
      else
      {
        int c     = (dx - dy) << 1;
        int incr1 = c << 1;
        int d     = incr1 + dy;
        for(int i = 0; i < length; i++)
        {
          y0 += stepy;
          y1 -= stepy;
          if(d > 0)
          {
            GdcLdw_Helper::PutPixel(x0 += stepx, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            GdcLdw_Helper::PutPixel(x1 -= stepx, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            GdcLdw_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            d += incr1;
          }
          else
          {
            if(d < c)
            {
              GdcLdw_Helper::PutPixel(x0, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue,
                                      cImageHeightInPixels);
              GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
              GdcLdw_Helper::PutPixel(x1, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue,
                                      cImageHeightInPixels);
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
            }
            else
            {
              GdcLdw_Helper::PutPixel(x0 += stepx, y0, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
              GdcLdw_Helper::PutPixel(x0, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
              GdcLdw_Helper::PutPixel(x1, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
            }
            d += incr2;
          }
        }
        if(extras > 0)
        {
          if(d > 0)
          {
            GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            if(extras > 1)
              GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
            if(extras > 2)
              GdcLdw_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
          }
          else if(d < c)
          {
            GdcLdw_Helper::PutPixel(x0, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            if(extras > 1)
              GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel,
                                      (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
            if(extras > 2)
              GdcLdw_Helper::PutPixel(x1, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
          }
          else
          {
            GdcLdw_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                    cColorValue, cImageHeightInPixels);
            if(extras > 1)
              GdcLdw_Helper::PutPixel(x0, y0 += stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                      cColorValue, cImageHeightInPixels);
            if(extras > 2)
            {
              if(d > c)
                GdcLdw_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel,
                                        (uint8_t*)cpImageBuffer, cColorValue, cImageHeightInPixels);
              else
                GdcLdw_Helper::PutPixel(x1, y1 -= stepy, cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer,
                                        cColorValue, cImageHeightInPixels);
            }
          }
        }
      }
    }
  }
  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
void GdcLdw_Helper::PutPixel(const uint32_t cX,
                             const uint32_t cY,
                             const uint32_t cSpanInBytes,
                             const uint32_t cBitsPerPixel,
                             uint8_t*       cpImageInuint8_t,
                             const uint32_t cColorValue,
                             const uint32_t cMaxHeight)
{
  const uint32_t cSpanInPixels = cSpanInBytes / (cBitsPerPixel / 8);
  const uint32_t maxsize       = cSpanInBytes * cMaxHeight;

  switch(cBitsPerPixel)
  {
    case 8:
    {
      if(cY * cSpanInPixels + cX < maxsize)
        cpImageInuint8_t[cY * cSpanInPixels + cX] = cColorValue; // Use low 8 bits
      break;
    }
    case 16:
    {
      uint16_t* cpImageInuint16_t = (uint16_t*)cpImageInuint8_t;

      if(cY * cSpanInPixels + cX < maxsize)
        cpImageInuint16_t[cY * cSpanInPixels + cX] = cColorValue; // Use low 16 bits
      break;
    }
    case 24:
    {
      uint8_t* lpPixelU8 = cpImageInuint8_t + (cSpanInBytes * cY) + (cX * (cBitsPerPixel / 8));
      uint32_t lColor    = cColorValue;

      if(lpPixelU8 < cpImageInuint8_t + maxsize)
      {
        (*lpPixelU8) = (uint8_t)(lColor & 0xFF);
        lpPixelU8++;
        lColor       = lColor >> 8;
        (*lpPixelU8) = (uint8_t)(lColor & 0xFF);
        lpPixelU8++;
        lColor       = lColor >> 8;
        (*lpPixelU8) = (uint8_t)(lColor & 0xFF); //lpPixelU8++; lColor = lColor >> 8;
      }

      break;
    }
    case 32:
    {
      uint32_t* cpImageInUint32 = (uint32_t*)cpImageInuint8_t;

      if(cY * cSpanInPixels + cX < maxsize)
        cpImageInUint32[cY * cSpanInPixels + cX] = cColorValue; // Use 32 bits
      break;
    }
    default:
    {
      break;
    }
  };
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcLdw_Helper::readRawImage(const char* pString, char* pImageBuffer, const uint32_t cSceneBufferSizeInBytes)
{
  LIB_RESULT libResult = LIB_FAILURE;

  FILE* pFile = fopen(pString, "rb");
  SHOW_ERROR(pFile != 0);
  uint32_t bytesRead = 0;
  if(pFile == 0)
  {
    fprintf(stderr, "%s: error %s: No such file\n", __PRETTY_FUNCTION__, pString);
  }
  else
  {
    bytesRead = fread(pImageBuffer, 1, cSceneBufferSizeInBytes, pFile);

    if(cSceneBufferSizeInBytes > 0 && bytesRead == cSceneBufferSizeInBytes)
    {
      libResult = LIB_SUCCESS;
    }
    fclose(pFile);
  }
  return libResult;
}

/*!*********************************************************************************
 *  @brief Function used to render a string of RGB24 formast buffer
 *
 */
LIB_RESULT GdcLdw_Helper::ReverseCopyRgb24(void* const    cpDestination,
                                           void* const    cpSource,
                                           uint32_t const cWidthInPixels,
                                           uint32_t const cHeightInPixels)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(cpDestination != 0 && cpSource != 0)
  {
    const uint32_t cSizeInPixels   = cWidthInPixels * cHeightInPixels;
    const uint32_t cBytesPerPixels = 3; // RGB24

    uint8_t* pDst_Pixel = (uint8_t*)((uintptr_t)cpDestination + (cSizeInPixels * cBytesPerPixels) -
                                     cBytesPerPixels); // Position to first pixel

    uint8_t* pSrc_Pixel = (uint8_t*)cpSource; // Position to first pixel

    for(uint32_t i = 0; i < cSizeInPixels; ++i) // For every pixel
    {
      pDst_Pixel[0] = pSrc_Pixel[0];
      pDst_Pixel[1] = pSrc_Pixel[1];
      pDst_Pixel[2] = pSrc_Pixel[2];

      pDst_Pixel -= cBytesPerPixels;
      pSrc_Pixel += cBytesPerPixels;
    }

    libResult = LIB_SUCCESS;
  }

  return libResult;
}
