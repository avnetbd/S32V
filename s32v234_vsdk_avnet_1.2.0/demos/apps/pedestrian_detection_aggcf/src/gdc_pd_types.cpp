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
#include "gdc_pd_types.hpp"
#include "gdc_pd_types_dummy.hpp"
#include "gdc_pd_messages.hpp"
#include "nxp_logo.h"
#include "Font7x13.h"
#if !defined(__CYGWIN__) && !defined(_WIN32)
#include <apexcv_pro_resize.h>
#else
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#endif
#include <resize_ref.h>
#include <umat.hpp>
#include <communications.hpp>
#include <oal.h>
#include <global_errors.h>
#include <pthread.h>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdint.h>
#include <cstdlib>
#include <new>
#include "common_time_measure.h"

/***************************************************************************/
/* Local Literals & definitions */

/**********************************************************************************************************/
// GdcPd_Display function definitions
/**********************************************************************************************************/

GdcPd_MovingAverage::GdcPd_MovingAverage()
    : mSum(0), mCount_Current(0), mCount_Max(1) // Must be positive otherwise might lead to division by zero
{
}

GdcPd_MovingAverage::~GdcPd_MovingAverage()
{
}

void GdcPd_MovingAverage::Initialize(const uint32_t cCount_Max)
{
  mCount_Max = cCount_Max;
}

uint64_t GdcPd_MovingAverage::Update(const uint64_t cNewValue)
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

uint64_t GdcPd_MovingAverage::Get() const
{
  return (mSum / mCount_Current);
}

/**********************************************************************************************************/
// GdcPd_FrameSize function definitions
/**********************************************************************************************************/

GdcPd_FrameSize::GdcPd_FrameSize() : mWidth(0), mHeight(0), mSpan(0)
{
}

GdcPd_FrameSize::GdcPd_FrameSize(const uint32_t cWidth, const uint32_t cHeight)
    : mWidth(cWidth), mHeight(cHeight), mSpan(cWidth)
{
}

GdcPd_FrameSize::~GdcPd_FrameSize()
{
}

uint32_t GdcPd_FrameSize::Area() const
{
  return mSpan * mHeight;
}

/**********************************************************************************************************/
// GdcPd_Display function definitions
/**********************************************************************************************************/

/***************************************************************************/
/**
 * \par Description
 * Constructor should set values to default state
 * Resource allocation should happen outside the constructor since it cannot
 * return any values indicating success or failure.
 *
 **************************************************************************/
GdcPd_Composition::GdcPd_Composition()
    : //GdcPd_MovingAverage mMovingAverage_ResizeApexCv;
      //GdcPd_MovingAverage mMovingAverage_ResizeApex;
      //GdcPd_MovingAverage mMovingAverage_HogApexCv;
      //GdcPd_MovingAverage mMovingAverage_HogApex;
      //GdcPd_MovingAverage mMovingAverage_FramePeriod;
      mFrameTicks(0), mIsInitialized(false)
{
}

/***************************************************************************/
/**
 * \par Description
 * Destructor should handle freeing of any leftover resources
 * Resource deallocation should happen outside the destructor since it cannot
 * return any values indicating success or failure.
 *
 **************************************************************************/
GdcPd_Composition::~GdcPd_Composition()
{
}

/***************************************************************************/
/**
 * \par Description
 * Initialization function prepares this object for use.
 * Resources are allocated and members variables are set.
 *
 **************************************************************************/
LIB_RESULT GdcPd_Composition::Initialize()
{
  LIB_RESULT libResult = LIB_SUCCESS;

  if(mIsInitialized == false)
  {
    mMovingAverage_LuvApexCv.Initialize(16);       // Magic number bad!!!
    mMovingAverage_1stOctaveApexCv.Initialize(16); // Magic number bad!!!
    mMovingAverage_NthOctaveApexCv.Initialize(16); // Magic number bad!!!
    mMovingAverage_detector.Initialize(16);        // Magic number bad!!!
    mMovingAverage_FramePeriod.Initialize(32);     // Magic number bad!!!
    mIsInitialized = true;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * Deinitialization function releases any leftover resources owned by the object
 *
 **************************************************************************/
LIB_RESULT GdcPd_Composition::Deinitialize()
{
  LIB_RESULT libResult = LIB_SUCCESS;

  if(mIsInitialized == true)
  {
    mIsInitialized = false;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * Deinitialization function releases any leftover resources owned by the object
 *
 **************************************************************************/
LIB_RESULT GdcPd_Composition::PutFrame(GdcPd_DetectorData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsInitialized == true)
  {
    SHOW_ERROR(rData.mSceneRgb24_umat.empty() == false);

    const uint32_t cSceneWidthInPixels  = rData.mSceneRgb24_umat.cols;
    const uint32_t cSceneHeightInPixels = rData.mSceneRgb24_umat.rows;

    vsdk::Mat lSceneRgb24_mat = rData.mSceneRgb24_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
    void*     lpSceneRgb24    = (void*)(lSceneRgb24_mat.data);
    SHOW_ERROR(lpSceneRgb24 != 0);

    if(lpSceneRgb24 != 0)
    {
      for(uint32_t i = 0; i < rData.mPedestrianRois.size(); ++i)
      {
        libResult = GdcPd_Helper::DrawRectangleRgb24((uint8_t* const)lpSceneRgb24, cSceneWidthInPixels,
                                                     cSceneHeightInPixels, rData.mPedestrianRois[i], 0x00FF0064UL);
      }

      GdcPd_Helper::SetLogo(cSceneWidthInPixels, cSceneHeightInPixels, cSceneWidthInPixels - NXP_LOGO_WIDTH - 16,
                            cSceneHeightInPixels - NXP_LOGO_HEIGHT - 16, (uint8_t* const)lpSceneRgb24);

      const uint32_t cScale     = 1;
      const uint32_t cTextColor = 0x00ff0000;
      const uint32_t cLineSpace = 0;

      int32_t textOffsetX = (TEXT_WIDTH_7x13 * 2);
      int32_t textOffsetY = (TEXT_HEIGHT_7x13 * cScale + TEXT_HEIGHT_7x13);

      const int32_t textOffsetY_delta = (TEXT_HEIGHT_7x13 * cScale + cLineSpace);

      char lString[128] = {0};
      sprintf(lString, "Frame       [%d]", rData.mSceneCounter);
      GdcPd_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, textOffsetX, textOffsetY,
                                            (uint8_t*)lString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

      lString[0] = 0;
      textOffsetY += textOffsetY_delta;
      sprintf(lString, "Resolution  [%dx%d]", cSceneWidthInPixels, cSceneHeightInPixels);
      GdcPd_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, textOffsetX, textOffsetY,
                                            (uint8_t*)lString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

      /*
         lString[0] = 0;
         textOffsetY += textOffsetY_delta;
         sprintf(lString, "Scales      [%d]", gcTotalNumberOfApproxScales+gcNumberOfRealScales);
         GdcPd_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels,
                                               cSceneHeightInPixels,
                                               textOffsetX,
                                               textOffsetY,
                                               (uint8_t*)lString,
                                               cTextColor,
                                               (uint8_t* const)lpSceneRgb24,
                                               cScale);
*/
      lString[0] = 0;
      textOffsetY += textOffsetY_delta;
      sprintf(lString, "Pedestrians [%zu]", rData.mPedestrianRois.size());
      GdcPd_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, textOffsetX, textOffsetY,
                                            (uint8_t*)lString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

      const uint32_t cTimeDivisor = 1000; // In linux values are in nanoseconds

      //
      // Draw Frames Per Second
      //
      if(mFrameTicks != 0)
      {
        const uint64_t cFrameTicks = FSL_Ticks();
        mMovingAverage_FramePeriod.Update(cFrameTicks - mFrameTicks);
        mFrameTicks = cFrameTicks;
      }
      else
      {
        mFrameTicks = FSL_Ticks();
      }

      const double lFramesPerSecond = (1000.0 * 1000.0) / (double)(mMovingAverage_FramePeriod.Get() / cTimeDivisor);

      lString[0] = 0;
      textOffsetY += textOffsetY_delta;
      sprintf(lString, "Frame rate  [%d fps]", (int)lFramesPerSecond);
      GdcPd_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, textOffsetX, textOffsetY,
                                            (uint8_t*)lString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

      uint64_t totalArmUs[gcApexCvEndTick];
      uint64_t totalApexCvUs[gcApexCvEndTick];
      for(uint32_t i = 0; i < gcApexCvEndTick; i++)
      {
        totalArmUs[i]    = 0;
        totalApexCvUs[i] = 0;
      }

      SHOW_ERROR(gcApexCvApproxScaleApexId < 2 && gcApexCvRealScaleApexId < 2);

      //uint64_t totalDetector = 0;

      //totalDetector = rData.mPerDetectorAcfProfilingInfo[0].apu_total;
      //totalApexUs[gcApexCvRealScaleApexId] += mMovingAverage_ResizeApex.Update(totalDetector);
      totalApexCvUs[gcSceneTick] +=
          mMovingAverage_Scene.Update(rData.mTicks[gcSceneTick + 1] - rData.mTicks[gcSceneTick]) / cTimeDivisor;
      totalApexCvUs[gcApexCvLUVTick] +=
          mMovingAverage_LuvApexCv.Update(rData.mTicks[gcApexCvLUVTick + 1] - rData.mTicks[gcApexCvLUVTick]) /
          cTimeDivisor;
      totalApexCvUs[gcApexCv1stOctaveTick] +=
          mMovingAverage_1stOctaveApexCv.Update(rData.mTicks[gcApexCv1stOctaveTick + 1] -
                                                rData.mTicks[gcApexCv1stOctaveTick]) /
          cTimeDivisor;
      totalApexCvUs[gcApexCvNthOctaveTick] +=
          mMovingAverage_NthOctaveApexCv.Update(rData.mTicks[gcApexCvNthOctaveTick + 1] -
                                                rData.mTicks[gcApexCvNthOctaveTick]) /
          cTimeDivisor;

      totalArmUs[0] += mMovingAverage_detector.Update(rData.mTicks[gcDetectorTick + 1] - rData.mTicks[gcDetectorTick]) /
                       cTimeDivisor;

      lString[0] = 0;
      textOffsetY += textOffsetY_delta;
      sprintf(lString, "Scene       [%zu us]", totalApexCvUs[gcSceneTick]);
      GdcPd_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, textOffsetX, textOffsetY,
                                            (uint8_t*)lString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);
      /*
         lString[0] = 0;
         textOffsetY += textOffsetY_delta;
         sprintf(lString, "A%1d:LUV      [%zu us]",gcApexCvLuvApexId, totalApexCvUs[gcApexCvLUVTick]);
         GdcPd_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels,
                                               cSceneHeightInPixels,
                                               textOffsetX,
                                               textOffsetY,
                                               (uint8_t*)lString,
                                               cTextColor,
                                               (uint8_t* const)lpSceneRgb24,
                                               cScale);
         lString[0] = 0;
         textOffsetY += textOffsetY_delta;
         sprintf(lString, "A%1d:Octave1  [%zu us]", gcApexCvRealScaleApexId, totalApexCvUs[gcApexCv1stOctaveTick]);
         GdcPd_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels,
                                               cSceneHeightInPixels,
                                               textOffsetX,
                                               textOffsetY,
                                               (uint8_t*)lString,
                                               cTextColor,
                                               (uint8_t* const)lpSceneRgb24,
                                               cScale);
         lString[0] = 0;
         textOffsetY += textOffsetY_delta;
         sprintf(lString, "A%1d:OctaveN  [%zu us]",gcApexCvApproxScaleApexId, totalApexCvUs[gcApexCvNthOctaveTick]);
         GdcPd_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels,
                                               cSceneHeightInPixels,
                                               textOffsetX,
                                               textOffsetY,
                                               (uint8_t*)lString,
                                               cTextColor,
                                               (uint8_t* const)lpSceneRgb24,
                                               cScale);
*/
      lString[0] = 0;
      textOffsetY += textOffsetY_delta;
      sprintf(lString, "Detector    [%zu us]", totalArmUs[0]);
      GdcPd_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, textOffsetX, textOffsetY,
                                            (uint8_t*)lString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);
      /* apex perforamance info on screen
         lString[0] = 0;
         textOffsetY += textOffsetY_delta;
         sprintf(lString, "Apex0       [%zu us]", totalApexUs[0]);
         GdcPd_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels,
                                               cSceneHeightInPixels,
                                               textOffsetX,
                                               textOffsetY,
                                               (uint8_t*)lString,
                                               cTextColor,
                                               (uint8_t* const)lpSceneRgb24,
                                               cScale);
         lString[0] = 0;
         textOffsetY += textOffsetY_delta;
         sprintf(lString, "Apex1       [%zu us]", totalApexUs[1]);
         GdcPd_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels,
                                               cSceneHeightInPixels,
                                               textOffsetX,
                                               textOffsetY,
                                               (uint8_t*)lString,
                                               cTextColor,
                                               (uint8_t* const)lpSceneRgb24,
                                               cScale);
*/
      libResult = LIB_SUCCESS;
    }
  }

  return libResult;
}

/***************************************************************************/
/**
 * \details
 * This function ...
 *
 **************************************************************************/
GdcPd_Task::GdcPd_Task()
    : mStackSizeInBytes(0), mPriority(0), mIsSemaphoreThreadSafetyCreated(false), mIsTaskSpawned(false),
      mIsTaskFinished(false)

{
  LIB_RESULT libResult = LIB_FAILURE;

  mName[0] = 0;

  // mSemaphoreThreadSafety: Starting count should be 1;
  libResult = GDC_OAL_SemaphoreCreate(&mSemaphoreThreadSafety, (char*)gcGdcPdTaskSemaphoreThreadSafetyName,
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
GdcPd_Task::~GdcPd_Task()
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsTaskSpawned == true && gcIsRunningWithThreads == true)
  {
    libResult = GdcPd_Task::Delete();
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
LIB_RESULT GdcPd_Task::Spawn(const uint32_t cStackSizeInBytes, const uint32_t cTaskPriority, const char cTaskName[])
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
            intResult = pthread_create(pThread, NULL, GdcPd_Task::TaskEntryRoutine, this);
            SHOW_ERROR(intResult == 0);
            if(intResult == 0)
            {
#if !defined(__CYGWIN__) && !defined(_WIN32)
              intResult = pthread_setname_np(*pThread, cTaskName);
              SHOW_ERROR(intResult == 0);
#endif
              if(intResult == 0)
              {
                mTaskControlBlock = (GdcPd_TCB)pThread;
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
          strncpy(mName, cTaskName, gcGdcPdTaskNameMaximumSizeInChar);
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
LIB_RESULT GdcPd_Task::Join()
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
        mIsTaskSpawned  = false;
        mIsTaskFinished = true;
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
LIB_RESULT GdcPd_Task::Delete()
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
void* GdcPd_Task::TaskEntryRoutine(void* pVoid)
{
  if(gcIsRunningWithThreads == true)
  {
    SHOW_ERROR(pVoid != 0);

    GdcPd_Task* const cpTask = (GdcPd_Task*)pVoid;

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

void GdcPd_Helper::SetLogo(int32_t const cWidthInPixels,
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
  for(int i = 0; i < NXP_LOGO_HEIGHT; i++)
  {
    for(int j = 0; j < (NXP_LOGO_WIDTH); j++)
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

/**********************************************************************************************************/
// GdcPd_Helper function definitions
/**********************************************************************************************************/

void GdcPd_Helper::RenderTextToRgb24Buffer(int32_t const cWidthInPixels,
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
          /*else
            {
               pCurrentPixel[0] = redColorInvert;
               pCurrentPixel[1] = greenColorInvert;
               pCurrentPixel[2] = blueColorInvert;
            }*/
          pCurrentPixel += cPixelSizeInBytes;
        }

        if((y % cScale) == 1 || cScale == 1)
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
 * Function used to draw a rectangle in an RGB24 image
 *
 **************************************************************************/
LIB_RESULT GdcPd_Helper::DrawRectangleRgb24(uint8_t* const     cpFrame,
                                            int32_t const      cFrameWidthInPixels,
                                            int32_t const      cFrameHeightInPixels,
                                            const apexcv::bbs& rcRoi,
                                            const uint32_t     cRgb24Color)
{
  LIB_RESULT libResult = LIB_FAILURE;

  int32_t boxWidth  = rcRoi.w;
  int32_t boxHeight = rcRoi.h;

  // make sure box is within image
  if(rcRoi.x + boxWidth >= cFrameWidthInPixels)
  {
    boxWidth = cFrameWidthInPixels - rcRoi.x - 1;
  }
  if(rcRoi.y + boxHeight >= cFrameHeightInPixels)
  {
    boxHeight = cFrameHeightInPixels - rcRoi.y - 1;
  }

  if(cpFrame != 0 && cFrameWidthInPixels > 0 && cFrameHeightInPixels > 0 && rcRoi.x >= 0 && rcRoi.y >= 0 &&
     rcRoi.w > 0 && rcRoi.h > 0 && rcRoi.x + boxWidth < cFrameWidthInPixels &&
     rcRoi.y + boxHeight < cFrameHeightInPixels)
  {
    const uint32_t cPixelSizeInBytes = 3; // RGB24

    const uint8_t cRed   = cRgb24Color & 0xFF;
    const uint8_t cBlue  = (cRgb24Color & 0xFF00) >> 8;
    const uint8_t cGreen = (cRgb24Color & 0xFF0000) >> 16;

    const uint32_t cLineSpanInBytes = cFrameWidthInPixels * cPixelSizeInBytes;

    uint8_t* pLine = cpFrame + (rcRoi.x * cPixelSizeInBytes) + (rcRoi.y * cLineSpanInBytes);

    uint8_t* pPixel = pLine;

    //
    // Draw top edge
    //
    for(int32_t x = 0; x < boxWidth; ++x)
    {
      pPixel[0] = cRed;
      pPixel[1] = cBlue;
      pPixel[2] = cGreen;

      // pPixel[0 - cLineSpanInBytes] = cRed;
      // pPixel[1 - cLineSpanInBytes] = cBlue;
      // pPixel[2 - cLineSpanInBytes] = cGreen;

      // pPixel[0 + cLineSpanInBytes] = cRed;
      // pPixel[1 + cLineSpanInBytes] = cBlue;
      // pPixel[2 + cLineSpanInBytes] = cGreen;

      pPixel += cPixelSizeInBytes;
    }
    pLine += cLineSpanInBytes;
    pPixel = pLine;
    //
    // Draw left and right edges
    //
    for(int32_t y = 0; y < (boxHeight - 4); ++y)
    {
      pPixel[0] = cRed;
      pPixel[1] = cBlue;
      pPixel[2] = cGreen;
      // // pPixel[0 + cPixelSizeInBytes] = cRed;
      // // pPixel[1 + cPixelSizeInBytes] = cBlue;
      // // pPixel[2 + cPixelSizeInBytes] = cGreen;
      // // pPixel[0 - cPixelSizeInBytes] = cRed;
      // // pPixel[1 - cPixelSizeInBytes] = cBlue;
      // // pPixel[2 - cPixelSizeInBytes] = cGreen;

      pPixel += ((boxWidth - 1) * cPixelSizeInBytes) - cPixelSizeInBytes;
      pPixel[0] = cRed;
      pPixel[1] = cBlue;
      pPixel[2] = cGreen;
      // pPixel[0 + cPixelSizeInBytes] = cRed;
      // pPixel[1 + cPixelSizeInBytes] = cBlue;
      // pPixel[2 + cPixelSizeInBytes] = cGreen;
      // pPixel[0 - cPixelSizeInBytes] = cRed;
      // pPixel[1 - cPixelSizeInBytes] = cBlue;
      // pPixel[2 - cPixelSizeInBytes] = cGreen;

      pLine += cLineSpanInBytes;
      pPixel = pLine;
    }

    //
    // Draw bottom edge
    //
    for(int32_t x = 0; x < boxWidth; ++x)
    {
      pPixel[0] = cRed;
      pPixel[1] = cBlue;
      pPixel[2] = cGreen;

      // pPixel[0 - cLineSpanInBytes] = cRed;
      // pPixel[1 - cLineSpanInBytes] = cBlue;
      // pPixel[2 - cLineSpanInBytes] = cGreen;

      // pPixel[0 + cLineSpanInBytes] = cRed;
      // pPixel[1 + cLineSpanInBytes] = cBlue;
      // pPixel[2 + cLineSpanInBytes] = cGreen;

      pPixel += cPixelSizeInBytes;
    }
    pLine += cLineSpanInBytes;
    pPixel = pLine;

    libResult = LIB_SUCCESS;
  }
  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * Function used to convert a set of RGB24 pixels to the RGB32 (BGRX) pixel format
 * It is required since apexcv::ColorConverter does not support RGB24 to RGB32 (BGRX) conversion
 *
 **************************************************************************/
LIB_RESULT GdcPd_Helper::CropAndConvertRgb24ToRgb32(vsdk::UMat& rSensorRgb24_umat,
                                                    vsdk::UMat& rSceneRgb24_umat,
                                                    vsdk::UMat& rSceneRgb32_umat)
{
  LIB_RESULT libResult = LIB_FAILURE;

  const uint32_t cSensorRgb24_WidthInPixels  = rSensorRgb24_umat.cols;
  const uint32_t cSensorRgb24_HeightInPixels = rSensorRgb24_umat.rows;

  const uint32_t cSceneRgb24_WidthInPixels  = rSceneRgb24_umat.cols;
  const uint32_t cSceneRgb24_HeightInPixels = rSceneRgb24_umat.rows;

  const uint32_t cSceneRgb32_WidthInPixels  = rSceneRgb32_umat.cols;
  const uint32_t cSceneRgb32_HeightInPixels = rSceneRgb32_umat.rows;

  const uint32_t cSensorRgb24_PixelSizeInBytes = rSensorRgb24_umat.elemSize();
  const uint32_t cSensorRgb24_SpanInBytes      = rSensorRgb24_umat.step[0];
  const uint32_t cSceneRgb24_PixelSizeInBytes  = rSceneRgb24_umat.elemSize();
  const uint32_t cSceneRgb24_SpanInBytes       = rSceneRgb24_umat.step[0];
  const uint32_t cSceneRgb32_PixelSizeInBytes  = rSceneRgb32_umat.elemSize();
  const uint32_t cSceneRgb32_SpanInBytes       = rSceneRgb32_umat.step[0];

  if(cSensorRgb24_WidthInPixels == cSceneRgb24_WidthInPixels &&
     cSensorRgb24_WidthInPixels == cSceneRgb32_WidthInPixels &&
     cSensorRgb24_HeightInPixels == cSceneRgb24_HeightInPixels &&
     cSensorRgb24_HeightInPixels == cSceneRgb32_HeightInPixels)
  {
    vsdk::Mat lSensorRgb24_mat = rSensorRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);
    vsdk::Mat lSceneRgb24_mat  = rSceneRgb24_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
    vsdk::Mat lSceneRgb32_mat  = rSceneRgb32_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);

    uint8_t* pSensorRgb24_Line  = (uint8_t*)(lSensorRgb24_mat.data);
    uint8_t* pSensorRgb24_Pixel = pSensorRgb24_Line;

    uint8_t* pSceneRgb24_Line  = (uint8_t*)(lSceneRgb24_mat.data);
    uint8_t* pSceneRgb24_Pixel = pSceneRgb24_Line;

    uint8_t* pSceneRgb32_Line  = (uint8_t*)(lSceneRgb32_mat.data);
    uint8_t* pSceneRgb32_Pixel = pSceneRgb32_Line;

    for(uint32_t y = 0; y < cSceneRgb32_HeightInPixels; ++y) // For every line
    {
      for(uint32_t x = 0; x < cSceneRgb32_WidthInPixels; ++x) // For every pixel on that line
      {
        pSceneRgb24_Pixel[0] = pSensorRgb24_Pixel[0];
        pSceneRgb24_Pixel[1] = pSensorRgb24_Pixel[1];
        pSceneRgb24_Pixel[2] = pSensorRgb24_Pixel[2];

        pSceneRgb32_Pixel[0] = 0;
        pSceneRgb32_Pixel[1] = pSensorRgb24_Pixel[2];
        pSceneRgb32_Pixel[2] = pSensorRgb24_Pixel[1];
        pSceneRgb32_Pixel[3] = pSensorRgb24_Pixel[0];

        pSensorRgb24_Pixel += cSensorRgb24_PixelSizeInBytes;
        pSceneRgb24_Pixel += cSceneRgb24_PixelSizeInBytes;
        pSceneRgb32_Pixel += cSceneRgb32_PixelSizeInBytes;
      }
      pSensorRgb24_Line += cSensorRgb24_SpanInBytes;
      pSceneRgb24_Line += cSceneRgb24_SpanInBytes;
      pSceneRgb32_Line += cSceneRgb32_SpanInBytes;

      pSensorRgb24_Pixel = pSensorRgb24_Line;
      pSceneRgb24_Pixel  = pSceneRgb24_Line;
      pSceneRgb32_Pixel  = pSceneRgb32_Line;
    }
    libResult = LIB_SUCCESS;
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
 * Function used to convert a set of RGB24 pixels to the RGB32 (BGRX) pixel format
 * It is required since apexcv::ColorConverter does not support RGB24 to RGB32 (BGRX) conversion
 *
 **************************************************************************/
LIB_RESULT GdcPd_Helper::CropAndConvertBgr24ToRgb32(vsdk::UMat& rSensorRgb24_umat,
                                                    vsdk::UMat& rSceneRgb24_umat,
                                                    vsdk::UMat& rSceneRgb32_umat)
{
  LIB_RESULT libResult = LIB_FAILURE;

  const uint32_t cSensorRgb24_WidthInPixels  = rSensorRgb24_umat.cols;
  const uint32_t cSensorRgb24_HeightInPixels = rSensorRgb24_umat.rows;

  const uint32_t cSceneRgb24_WidthInPixels  = rSceneRgb24_umat.cols;
  const uint32_t cSceneRgb24_HeightInPixels = rSceneRgb24_umat.rows;

  const uint32_t cSceneRgb32_WidthInPixels  = rSceneRgb32_umat.cols;
  const uint32_t cSceneRgb32_HeightInPixels = rSceneRgb32_umat.rows;

  const uint32_t cSensorRgb24_PixelSizeInBytes = rSensorRgb24_umat.elemSize();
  const uint32_t cSensorRgb24_SpanInBytes      = rSensorRgb24_umat.step[0];
  const uint32_t cSceneRgb24_PixelSizeInBytes  = rSceneRgb24_umat.elemSize();
  const uint32_t cSceneRgb24_SpanInBytes       = rSceneRgb24_umat.step[0];
  const uint32_t cSceneRgb32_PixelSizeInBytes  = rSceneRgb32_umat.elemSize();
  const uint32_t cSceneRgb32_SpanInBytes       = rSceneRgb32_umat.step[0];

  if(cSensorRgb24_WidthInPixels == cSceneRgb24_WidthInPixels &&
     cSensorRgb24_WidthInPixels == cSceneRgb32_WidthInPixels &&
     cSensorRgb24_HeightInPixels == cSceneRgb24_HeightInPixels &&
     cSensorRgb24_HeightInPixels == cSceneRgb32_HeightInPixels)
  {
    vsdk::Mat lSensorRgb24_mat = rSensorRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);
    vsdk::Mat lSceneRgb24_mat  = rSceneRgb24_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
    vsdk::Mat lSceneRgb32_mat  = rSceneRgb32_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);

    uint8_t* pSensorRgb24_Line  = (uint8_t*)(lSensorRgb24_mat.data);
    uint8_t* pSensorRgb24_Pixel = pSensorRgb24_Line;

    uint8_t* pSceneRgb24_Line  = (uint8_t*)(lSceneRgb24_mat.data);
    uint8_t* pSceneRgb24_Pixel = pSceneRgb24_Line;

    uint8_t* pSceneRgb32_Line  = (uint8_t*)(lSceneRgb32_mat.data);
    uint8_t* pSceneRgb32_Pixel = pSceneRgb32_Line;

    for(uint32_t y = 0; y < cSceneRgb32_HeightInPixels; ++y) // For every line
    {
      for(uint32_t x = 0; x < cSceneRgb32_WidthInPixels; ++x) // For every pixel on that line
      {
        pSceneRgb24_Pixel[0] = pSensorRgb24_Pixel[0];
        pSceneRgb24_Pixel[1] = pSensorRgb24_Pixel[1];
        pSceneRgb24_Pixel[2] = pSensorRgb24_Pixel[2];

        pSceneRgb32_Pixel[0] = 0;
        pSceneRgb32_Pixel[1] = pSensorRgb24_Pixel[0];
        pSceneRgb32_Pixel[2] = pSensorRgb24_Pixel[1];
        pSceneRgb32_Pixel[3] = pSensorRgb24_Pixel[2];

        pSensorRgb24_Pixel += cSensorRgb24_PixelSizeInBytes;
        pSceneRgb24_Pixel += cSceneRgb24_PixelSizeInBytes;
        pSceneRgb32_Pixel += cSceneRgb32_PixelSizeInBytes;
      }
      pSensorRgb24_Line += cSensorRgb24_SpanInBytes;
      pSceneRgb24_Line += cSceneRgb24_SpanInBytes;
      pSceneRgb32_Line += cSceneRgb32_SpanInBytes;

      pSensorRgb24_Pixel = pSensorRgb24_Line;
      pSceneRgb24_Pixel  = pSceneRgb24_Line;
      pSceneRgb32_Pixel  = pSceneRgb32_Line;
    }
    libResult = LIB_SUCCESS;
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
 * Function used to convert a set of RGB32 (XRGB) pixels to the RGB24 (RGB888) pixel format
 *
 **************************************************************************/
LIB_RESULT GdcPd_Helper::ConvertRgb32ToRgb24(vsdk::UMat& rSource_umat, vsdk::UMat& rDestination_umat)
{
  LIB_RESULT libResult = LIB_FAILURE;

  const uint32_t cSourceWidthInPixels  = rSource_umat.cols;
  const uint32_t cSourceHeightInPixels = rSource_umat.rows;

  vsdk::Mat            lSource_mat    = rSource_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);
  const uint8_t* const cpcSourceFrame = (const uint8_t* const)lSource_mat.data;

  if(cpcSourceFrame != 0 && rDestination_umat.empty() == false)
  {
    //const uint32_t cSourceSpanInBytes = rSource_umat.step[0];

    const uint32_t cPixelInSizeInBytes  = 4; // RGB32
    const uint32_t cPixelOutSizeInBytes = 3; // RGB24

    const uint8_t* pcPixelIn = (const uint8_t*)(lSource_mat.data);

    vsdk::Mat      lDestination_mat = rDestination_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
    uint8_t*       pPixelOut        = (uint8_t*)(lDestination_mat.data);
    const uint8_t* cpcPixelInEnd =
        (const uint8_t*)&pcPixelIn[cSourceWidthInPixels * cSourceHeightInPixels * cPixelInSizeInBytes];

    for(; pcPixelIn < cpcPixelInEnd; pcPixelIn += cPixelInSizeInBytes)
    {
      pPixelOut[0] = pcPixelIn[1];
      pPixelOut[1] = pcPixelIn[2];
      pPixelOut[2] = pcPixelIn[3];

      pPixelOut += cPixelOutSizeInBytes;
    }
    libResult = LIB_SUCCESS;
  }
  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * Function used to convert a set of RGB32 (RGBX) pixels to the RGB24 (RGB888) pixel format
 *
 **************************************************************************/
LIB_RESULT GdcPd_Helper::ConvertRgb24ToRgb32(uint32_t const cFrameWidthInPixels,
                                             uint32_t const cFrameHeightInPixels,
                                             uint8_t* const cpSourceFrame,
                                             uint8_t* const cpDestinationFrame)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(cpSourceFrame != 0 && cpDestinationFrame != 0)
  {
    const uint32_t cFrameSizeInPixels   = cFrameWidthInPixels * cFrameHeightInPixels;
    const uint32_t cPixelInSizeInBytes  = 3; // RGB24
    const uint32_t cPixelOutSizeInBytes = 4; // RGB32

    const uint8_t* pcPixelIn = (const uint8_t*)cpSourceFrame;
    uint8_t*       pPixelOut = (uint8_t*)cpDestinationFrame;

    const uint8_t* cpcPixelInEnd =
        (const uint8_t*)&(pcPixelIn[cFrameSizeInPixels * cPixelInSizeInBytes]); // Address of first non-valid pixel

    for(; pcPixelIn < cpcPixelInEnd; pcPixelIn += cPixelInSizeInBytes)
    {
      pPixelOut[0] = 0;
      pPixelOut[1] = pcPixelIn[2];
      pPixelOut[2] = pcPixelIn[1];
      pPixelOut[3] = pcPixelIn[0];
      pPixelOut += cPixelOutSizeInBytes;
    }
    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/*!*********************************************************************************
 *  @brief Function used to render a string of RGB24 formast buffer
 *
 */
LIB_RESULT GdcPd_Helper::ReverseCopyRgb24(void* const    cpDestination,
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

/***************************************************************************/
/**
 * \par Description
 * Function used to crop a RGB24 image
 *
 **************************************************************************/
LIB_RESULT GdcPd_Helper::CropRgb24(uint32_t const cSourceWidthInPixels,
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
LIB_RESULT GdcPd_Helper::CalculateSensorRoi(const uint32_t      cSensorWidthInPixels,
                                            const uint32_t      cSensorHeightInPixels,
                                            const uint32_t      cSceneWidthInPixels,
                                            const uint32_t      cSceneHeightInPixels,
                                            /*out*/ vsdk::Rect& rRect)
{
  LIB_RESULT libResult = LIB_FAILURE;

  rRect.width = cSceneWidthInPixels;

  rRect.height = cSceneHeightInPixels;

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
LIB_RESULT GdcPd_Helper::SendRequest(const int32_t cMessageId,
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
LIB_RESULT GdcPd_Helper::SendRequest(const int32_t cMessageId,
                                     const int32_t cSenderQueueId,
                                     const int32_t cReceiverQueueId,
                                     const int32_t cPriority)
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

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pRequest, sizeof(*pRequest), cPriority);
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
LIB_RESULT GdcPd_Helper::SendImageReply(const int32_t  cReceiverQueueId,
                                        vsdk::UMat&    rSceneRgb24_umat,
                                        const void*    pcSceneRgb24_umat_data,
                                        vsdk::UMat&    rSceneRgb32_umat,
                                        const void*    pcSceneRgb32_umat_data,
                                        const uint32_t cSceneCounter,
                                        const uint64_t (&cTicks)[2])
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcPd_ImageReply* pReply = (GdcPd_ImageReply*)malloc(sizeof(GdcPd_ImageReply));

  if(pReply != 0)
  {
    new(pReply) GdcPd_ImageReply(); // Placement new

    pReply->mMessageId              = MESSAGE_ID_GDC_PD_IMAGE_REPLY;
    pReply->mSenderId               = 0; // not used, because no reply is expected.
    pReply->mCorrelationId          = 0; // not used, because no reply is expected.
    pReply->mSceneRgb24_umat        = rSceneRgb24_umat;
    pReply->mpcSceneRgb24_umat_data = pcSceneRgb24_umat_data;
    pReply->mSceneRgb32_umat        = rSceneRgb32_umat;
    pReply->mpcSceneRgb32_umat_data = pcSceneRgb32_umat_data;
    pReply->mSceneCounter           = cSceneCounter;

    std::copy(cTicks, cTicks + 2, pReply->mTicks);

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pReply, sizeof(*pReply), 1);
    if(libResult != LIB_SUCCESS)
    {
      pReply->~GdcPd_ImageReply();
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
LIB_RESULT GdcPd_Helper::SendDetectorData(const int32_t               cReceiverQueueId,
                                          vsdk::UMat&                 rSceneRgb24_umat,
                                          const void*                 pcSceneRgb24_umat_data,
                                          const uint32_t              cSceneCounter,
                                          std::vector< apexcv::bbs >& rPedestrianRois,
                                          const uint64_t (&mFeatureTicks)[gcApexCvEndTick],
                                          const AcfProfilingInfo (&/*cPerAcfProfilingInfo*/)[gcNumberOfRealScales])
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcPd_DetectorData* pData = (GdcPd_DetectorData*)malloc(sizeof(GdcPd_DetectorData));

  if(pData != 0)
  {
    new(pData) GdcPd_DetectorData(); // Placement new

    pData->mMessageId     = MESSAGE_ID_GDC_PD_AGGCF_DATA;
    pData->mSenderId      = 0; // not used, because no reply is expected.
    pData->mCorrelationId = 0; // not used, because no reply is expected.

    pData->mSceneRgb24_umat        = rSceneRgb24_umat;
    pData->mpcSceneRgb24_umat_data = pcSceneRgb24_umat_data;
    pData->mSceneCounter           = cSceneCounter;
    pData->mPedestrianRois         = rPedestrianRois;

    std::copy(mFeatureTicks, mFeatureTicks + (gcApexCvEndTick), pData->mTicks);
    //std::copy(&(cPerAcfProfilingInfo[0]), &(cPerAcfProfilingInfo[0])+(gcNumberOfRealScales), &(pData->mPerDetectorAcfProfilingInfo[0]));

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pData, sizeof(*pData), 1);
    if(libResult != LIB_SUCCESS)
    {
      pData->~GdcPd_DetectorData();
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
LIB_RESULT GdcPd_Helper::SendFeatureData(const int32_t  cReceiverQueueId,
                                         vsdk::UMat&    rSceneRgb24_umat,
                                         const void*    pcSceneRgb24_umat_data,
                                         const uint32_t cSceneCounter,
                                         vsdk::UMat*    rOutput_py_umat,
                                         const uint64_t (&mFeatureTicks)[gcApexCvEndTick],
                                         const AcfProfilingInfo (&/*cPerAcfProfilingInfo*/)[gcNumberOfRealScales])
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcPd_FeatureData* pData = (GdcPd_FeatureData*)malloc(sizeof(GdcPd_FeatureData));

  if(pData != 0)
  {
    new(pData) GdcPd_FeatureData(); // Placement new

    pData->mMessageId     = MESSAGE_ID_GDC_PD_FEATURE_DATA;
    pData->mSenderId      = 0; // not used, because no reply is expected.
    pData->mCorrelationId = 0; // not used, because no reply is expected.

    pData->mSceneRgb24_umat        = rSceneRgb24_umat;
    pData->mpcSceneRgb24_umat_data = pcSceneRgb24_umat_data;
    pData->mSceneCounter           = cSceneCounter;
    pData->mpOutput_Py_umat        = rOutput_py_umat;

    std::copy(mFeatureTicks, mFeatureTicks + (gcApexCvEndTick), pData->mTicks);
    //std::copy(&(cPerAcfProfilingInfo[0]), &(cPerAcfProfilingInfo[0])+(gcNumberOfRealScales), &(pData->mPerDetectorAcfProfilingInfo[0]));

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pData, sizeof(*pData), 1);
    if(libResult != LIB_SUCCESS)
    {
      pData->~GdcPd_FeatureData();
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
LIB_RESULT GdcPd_Helper::SendLUVData(const int32_t  cReceiverQueueId,
                                     vsdk::UMat&    rSceneRgb24_umat,
                                     const void*    pcSceneRgb24_umat_data,
                                     const uint32_t cSceneCounter,
                                     vsdk::UMat&    rSceneLUV_umat,
                                     vsdk::UMat*    rOutput_py_umat,
                                     const uint64_t (&cAggcfTicks)[gcApexCvEndTick],
                                     const AcfProfilingInfo (&/*cPerAcfProfilingInfo*/)[gcNumberOfRealScales])
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcPd_LuvData* pData = (GdcPd_LuvData*)malloc(sizeof(GdcPd_LuvData));

  if(pData != 0)
  {
    new(pData) GdcPd_LuvData(); // Placement new

    pData->mMessageId     = MESSAGE_ID_GDC_PD_LUV_DATA;
    pData->mSenderId      = 0; // not used, because no reply is expected.
    pData->mCorrelationId = 0; // not used, because no reply is expected.

    pData->mSceneRgb24_umat        = rSceneRgb24_umat;
    pData->mpcSceneRgb24_umat_data = pcSceneRgb24_umat_data;
    pData->mSceneCounter           = cSceneCounter;
    pData->mSceneLuv_umat          = rSceneLUV_umat;
    pData->mpOutput_Py_umat        = rOutput_py_umat;

    std::copy(cAggcfTicks, cAggcfTicks + (gcApexCvEndTick), pData->mTicks);

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pData, sizeof(*pData), 1);
    if(libResult != LIB_SUCCESS)
    {
      pData->~GdcPd_LuvData();
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
LIB_RESULT GdcPd_Helper::SendCompositionData(const int32_t cReceiverQueueId,
                                             vsdk::UMat&   rSceneRgb24_umat,
                                             const void*   pcSceneRgb24_umat_data)
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcPd_CompositionData* pData = (GdcPd_CompositionData*)malloc(sizeof(GdcPd_CompositionData));

  if(pData != 0)
  {
    new(pData) GdcPd_CompositionData(); // Placement new

    pData->mMessageId              = MESSAGE_ID_GDC_PD_COMPOSITION_DATA;
    pData->mSenderId               = 0; // not used, because no reply is expected.
    pData->mCorrelationId          = 0; // not used, because no reply is expected.
    pData->mSceneRgb24_umat        = rSceneRgb24_umat;
    pData->mpcSceneRgb24_umat_data = pcSceneRgb24_umat_data;

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pData, sizeof(*pData), 1);
    if(libResult != LIB_SUCCESS)
    {
      pData->~GdcPd_CompositionData();
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
LIB_RESULT GdcPd_Helper::FreeImageReply(GdcPd_ImageReply* const cpReply)
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
LIB_RESULT GdcPd_Helper::FreeDetectorData(GdcPd_DetectorData* const cpData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(cpData != 0)
  {
    cpData->mPedestrianRois.clear();

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcPd_Helper::FreeCompositionData(GdcPd_CompositionData* const cpData)
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
 **************************************************************************/
GdcPd_SceneTask_Enum GdcPd_Helper::ConvertToSceneTaskEnum(const uint32_t cValue)
{
  GdcPd_SceneTask_Enum lSceneTask_Enum = GDC_PD_SCENE_TASK_INVALID;

  switch(cValue)
  {
    case 1:
    {
      lSceneTask_Enum = GDC_PD_SCENE_TASK_FILE_READER;
      break;
    }
    case 2:
    {
      lSceneTask_Enum = GDC_PD_SCENE_TASK_IMAGE_SENSOR;
      break;
    }
    case 3:
    {
      lSceneTask_Enum = GDC_PD_SCENE_TASK_IP_CAMERA;
      break;
    }
    default:
    {
      SHOW_ERROR(false);
      lSceneTask_Enum = GDC_PD_SCENE_TASK_INVALID;
      break;
    }
  };

  return lSceneTask_Enum;
}
