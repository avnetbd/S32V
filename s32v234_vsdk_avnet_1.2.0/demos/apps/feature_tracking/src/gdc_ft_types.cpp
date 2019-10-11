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
#include "gdc_ft_types.hpp"
#include "gdc_ft_messages.hpp"
#include <apexcv_pro_resize.h>
#include "icp_data.h"
#include "communications.hpp"
#include "oal.h"
#include "global_errors.h"
#include "iniparser.h"
#include <stdlib.h>
#include <stdint.h>
#include <new>
#include <algorithm>
#include <cmath>

#include "t32term.h"

#include <apexcv_pro_resize.h>
#include "resize_ref.h"
#include "oal_protected.h"

const bool gcRemoveSafetyChecksFromThisFile = false;

/***************************************************************************/
/* Local Literals & definitions */
static int32_t sqrt32(int32_t value);

/**********************************************************************************************************/
// GdcFt_MovingAverage function definitions
/**********************************************************************************************************/

GdcFt_MovingAverage::GdcFt_MovingAverage()
    : mSum(0), mCount_Current(0), mCount_Max(1) // Must be positive otherwise might lead to division by zero
{
}

GdcFt_MovingAverage::~GdcFt_MovingAverage()
{
}

void GdcFt_MovingAverage::Initialize(const uint32_t cCount_Max)
{
  mCount_Max = cCount_Max;
}

uint64_t GdcFt_MovingAverage::Update(const uint64_t cNewValue)
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

uint64_t GdcFt_MovingAverage::Get() const
{
  return (mSum / mCount_Current);
}

/**********************************************************************************************************/
// GdcFt_MinMax function definitions
/**********************************************************************************************************/

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
GdcFt_MinMax::GdcFt_MinMax(const int32_t cMin, const int32_t cMax) : mMin(cMin), mMax(cMax)
{
}

/**********************************************************************************************************/
// GdcFt_TrackedFeature function definitions
/**********************************************************************************************************/

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
GdcFt_TrackedFeature::GdcFt_TrackedFeature()
    : //icp::Feature  mCurrent;
      mAgeInFramesSinceDetection(0),
      //ICP_Point mPositions[gcTrackedFeatureHistoryLength];
      mPositionsIndex_Current(0), mPositionsSize(0)
{
  mCurrent.position.x = 0;
  mCurrent.position.y = 0;
  mCurrent.strength   = 0;
  mCurrent.reserve    = 0;

  memset(mPositions, 0, sizeof(mPositions));
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
GdcFt_TrackedFeature::~GdcFt_TrackedFeature()
{
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcFt_TrackedFeature::Update(/*const*/ icp::Feature32S& rcFeature)
{
  LIB_RESULT libResult = LIB_SUCCESS;

  SHOW_ERROR(mPositionsIndex_Current < gcTrackedFeatureHistoryLength && mPositionsSize < gcTrackedFeatureHistoryLength);

  if(mPositionsIndex_Current >= gcTrackedFeatureHistoryLength || mPositionsSize >= gcTrackedFeatureHistoryLength)
  {
    printf("BAD\n"); // SASHBY
  }

  mCurrent = rcFeature;

  if(mPositionsSize > 0)
  {
    ++mAgeInFramesSinceDetection;
  }

  if(mPositionsSize < (gcTrackedFeatureHistoryLength - 1))
  {
    mPositions[mPositionsIndex_Current] = rcFeature.position;
    ++mPositionsIndex_Current;
    ++mPositionsSize;
  }
  else
  {
    mPositions[mPositionsIndex_Current] = rcFeature.position;
    mPositionsIndex_Current             = (mPositionsIndex_Current + 1) % gcTrackedFeatureHistoryLength;
  }

  SHOW_ERROR(mPositionsIndex_Current < gcTrackedFeatureHistoryLength && mPositionsSize < gcTrackedFeatureHistoryLength);

  if(mPositionsIndex_Current >= gcTrackedFeatureHistoryLength || mPositionsSize >= gcTrackedFeatureHistoryLength)
  {
    printf("BAD\n"); // SASHBY
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcFt_TrackedFeature::GetFront(ICP_Point& rPositionFront) const
{
  LIB_RESULT libResult = LIB_SUCCESS;

  SHOW_ERROR(mPositionsIndex_Current < gcTrackedFeatureHistoryLength && mPositionsSize < gcTrackedFeatureHistoryLength);

  if(mPositionsIndex_Current >= gcTrackedFeatureHistoryLength || mPositionsSize >= gcTrackedFeatureHistoryLength)
  {
    printf("BAD\n"); // SASHBY
  }

  const uint32_t cFrontIndex =
      (mPositionsIndex_Current != 0) ? (mPositionsIndex_Current - 1) : (gcTrackedFeatureHistoryLength - 1);

  rPositionFront = mPositions[cFrontIndex];

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcFt_TrackedFeature::GetBack(ICP_Point& rPositionBack) const
{
  LIB_RESULT libResult = LIB_SUCCESS;

  SHOW_ERROR(mPositionsIndex_Current < gcTrackedFeatureHistoryLength && mPositionsSize < gcTrackedFeatureHistoryLength);

  if(mPositionsIndex_Current >= gcTrackedFeatureHistoryLength || mPositionsSize >= gcTrackedFeatureHistoryLength)
  {
    printf("BAD\n"); // SASHBY
  }

  const uint32_t cBackIndex = (mPositionsSize < gcTrackedFeatureHistoryLength) ? (0) : (mPositionsIndex_Current);

  rPositionBack = mPositions[cBackIndex];

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
int32_t GdcFt_TrackedFeature::DisplacementAbs() const
{
  SHOW_ERROR(mPositionsIndex_Current < gcTrackedFeatureHistoryLength && mPositionsSize < gcTrackedFeatureHistoryLength);

  if(mPositionsIndex_Current >= gcTrackedFeatureHistoryLength || mPositionsSize >= gcTrackedFeatureHistoryLength)
  {
    printf("BAD\n"); // SASHBY
  }

  ICP_Point position1;
  ICP_Point position2;

  GdcFt_TrackedFeature::GetBack(position1);
  GdcFt_TrackedFeature::GetFront(position2);

  return GdcFt_Helper::DisplacementAbs(position1, position2);
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
int32_t GdcFt_TrackedFeature::DisplacementHistoryAbs()
{
  LIB_RESULT libResult = LIB_FAILURE;

  SHOW_ERROR(mPositionsIndex_Current < gcTrackedFeatureHistoryLength && mPositionsSize < gcTrackedFeatureHistoryLength);

  if(mPositionsIndex_Current >= gcTrackedFeatureHistoryLength || mPositionsSize >= gcTrackedFeatureHistoryLength)
  {
    printf("BAD\n"); // SASHBY
  }

  int32_t displacement = 0;

  GdcFt_TrackedFeatureIterator iterator;

  libResult = GdcFt_TrackedFeature::Iterator(iterator);

  if(libResult == LIB_SUCCESS)
  {
    const ICP_Point* pPosition0 = 0;
    const ICP_Point* pPosition1 = 0;

    if(iterator.HasNext())
    {
      pPosition0 = iterator.Next();

      while(iterator.HasNext())
      {
        pPosition1 = iterator.Next();

        displacement += GdcFt_Helper::DisplacementAbs(*pPosition0, *pPosition1);
        pPosition0 = pPosition1;
      }
    }
  }

  return displacement;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcFt_TrackedFeature::Iterator(GdcFt_TrackedFeatureIterator& rReturnedIterator) const
{
  LIB_RESULT lLibResult = LIB_FAILURE;

  SHOW_ERROR(mPositionsIndex_Current < gcTrackedFeatureHistoryLength && mPositionsSize < gcTrackedFeatureHistoryLength);

  if(mPositionsIndex_Current >= gcTrackedFeatureHistoryLength || mPositionsSize >= gcTrackedFeatureHistoryLength)
  {
    printf("BAD\n"); // SASHBY
  }

  rReturnedIterator.mpPositions = mPositions;
  rReturnedIterator.mPositionsIndex_Current =
      (mPositionsSize < gcTrackedFeatureHistoryLength) ? (0) : (mPositionsIndex_Current);
  rReturnedIterator.mPositionsCount = 0;
  rReturnedIterator.mPositionsSize  = mPositionsSize;

  lLibResult = LIB_SUCCESS; // Always succeed?

  return lLibResult;
}

/**********************************************************************************************************/
// GdcFt_TrackedFeatureAllocator function definitions
/**********************************************************************************************************/

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
GdcFt_TrackedFeatureIterator::GdcFt_TrackedFeatureIterator()
    : mpPositions(0), mPositionsIndex_Current(0), mPositionsCount(0), mPositionsSize(0)
{
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
GdcFt_TrackedFeatureIterator::~GdcFt_TrackedFeatureIterator()
{
}

/***************************************************************************/
/**
 * \par Returns true if the iteration has more elements.
 * In other words, returns true if Next() would return an element rather.
 *
 * \return true   There is another element in the container
 * \return false  There is no more elements in the container
 *
 * \warning Only safe to call after Iterator has been initialized by container
 **************************************************************************/
bool GdcFt_TrackedFeatureIterator::HasNext()
{
  SHOW_ERROR(mPositionsIndex_Current < gcTrackedFeatureHistoryLength && mPositionsSize < gcTrackedFeatureHistoryLength);

  if(mPositionsIndex_Current >= gcTrackedFeatureHistoryLength || mPositionsSize >= gcTrackedFeatureHistoryLength)
  {
    printf("BAD\n"); // SASHBY
  }

  bool hasNext = false;

  if(mpPositions != 0 && mPositionsCount < mPositionsSize)
  {
    hasNext = true;
  }

  return hasNext;
}

/***************************************************************************/
/**
 * \par Returns the next element in the iteration.
 * Calling this method repeatedly until the HasNext() method returns false
 * will return each element in the container exactly once.
 *
 * \return Name*   Pointer to the next element in the container
 *
 * \warning Only safe to call if HasNext returned true
 **************************************************************************/
const ICP_Point* GdcFt_TrackedFeatureIterator::Next()
{
  SHOW_ERROR(mpPositions != 0 && mPositionsIndex_Current < gcTrackedFeatureHistoryLength &&
             mPositionsCount < mPositionsSize && mPositionsSize <= gcTrackedFeatureHistoryLength);

  if(!(mpPositions != 0 && mPositionsIndex_Current < gcTrackedFeatureHistoryLength &&
       mPositionsCount < mPositionsSize && mPositionsSize <= gcTrackedFeatureHistoryLength))
  {
    printf("BAD!\n"); // SASHBY
  }

  const uint32_t cIndex = mPositionsIndex_Current;

  if(mPositionsSize < (gcTrackedFeatureHistoryLength - 1))
  {
    ++mPositionsIndex_Current;
  }
  else
  {
    mPositionsIndex_Current = (mPositionsIndex_Current + 1) % gcTrackedFeatureHistoryLength;
  }
  ++mPositionsCount;

  return &(mpPositions[cIndex]);
}

/**********************************************************************************************************/
// GdcFt_TrackedFeatureAllocator function definitions
/**********************************************************************************************************/

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
GdcFt_TrackedFeatureAllocator::GdcFt_TrackedFeatureAllocator()
    : mpObjectArray(0), mpFreeObjects(0), mNumberOfFreeObjects(0), mMaxNumberOfObjects(0), mIsInitialized(false)
{
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
GdcFt_TrackedFeatureAllocator::~GdcFt_TrackedFeatureAllocator()
{
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcFt_TrackedFeatureAllocator::Initialize(void* const    cpBuffer,
                                                     uint32_t const cBufferSizeInBytes,
                                                     uint32_t const cMaxNumberOfObjects,
                                                     uint32_t&      rReturnedUsedSizeInBytes)
{
  LIB_RESULT libResult = LIB_SUCCESS;

  // Aligning buffer adds to the used bytes
  const uintptr_t cAlignedStartingAddress =
      ((((uintptr_t)cpBuffer) + 3) & ~(3UL)); // Start should be aligned to 4 bytes

  const uint32_t cObjectSizeInBytes = ((sizeof(mpObjectArray[0]) + 3) & ~(3UL));

  rReturnedUsedSizeInBytes = (cAlignedStartingAddress - (uintptr_t)cpBuffer) +
                             (cMaxNumberOfObjects * (cObjectSizeInBytes + // Storage for Feature
                                                     sizeof(uint32_t)));  // Storage for free object list
  if(cpBuffer != 0)
  {
    if(mpObjectArray == 0 && cBufferSizeInBytes >= rReturnedUsedSizeInBytes)
    {
      uint8_t* pCurrentBufferLocation   = (uint8_t*)cAlignedStartingAddress;
      int32_t  currentBufferSizeInBytes = cBufferSizeInBytes;

      mpFreeObjects = (uint32_t*)pCurrentBufferLocation;
      pCurrentBufferLocation += (cMaxNumberOfObjects * sizeof(uint32_t));
      currentBufferSizeInBytes -= (cMaxNumberOfObjects * sizeof(uint32_t));

      mpObjectArray = (GdcFt_TrackedFeature*)pCurrentBufferLocation;
      pCurrentBufferLocation += (cMaxNumberOfObjects * cObjectSizeInBytes);
      currentBufferSizeInBytes -= (cMaxNumberOfObjects * cObjectSizeInBytes);

      mNumberOfFreeObjects = cMaxNumberOfObjects;
      mMaxNumberOfObjects  = cMaxNumberOfObjects; // After this assignment this value should NEVER change!

      const uint32_t cLastValidIndex = cMaxNumberOfObjects - 1;

      for(uint32_t i = 0; i <= cLastValidIndex; ++i)
      {
        mpFreeObjects[i] = i;
      }

      mIsInitialized = true;

      libResult = LIB_SUCCESS;
    }
    else
    {
      rReturnedUsedSizeInBytes = 0;
      libResult                = LIB_FAILURE;
    }
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcFt_TrackedFeatureAllocator::Allocate(GdcFt_TrackedFeature*& rpReturnedObject)
{
  LIB_RESULT libResult = LIB_FAILURE;

  rpReturnedObject = 0;

  if((gcRemoveSafetyChecksFromThisFile == true) ||
     (mNumberOfFreeObjects >= 1 && mNumberOfFreeObjects <= mMaxNumberOfObjects &&
      mpFreeObjects[mNumberOfFreeObjects - 1] < mMaxNumberOfObjects)) // Using mMaxNumberOfObjects as an invalid entry
  {
    rpReturnedObject = &(mpObjectArray[mpFreeObjects[mNumberOfFreeObjects - 1]]);

    mpFreeObjects[mNumberOfFreeObjects - 1] = mMaxNumberOfObjects;

    --mNumberOfFreeObjects;

    new(rpReturnedObject) GdcFt_TrackedFeature; // Placement new

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcFt_TrackedFeatureAllocator::Deallocate(GdcFt_TrackedFeature*& rpObjectToBeDeallocated)
{
  LIB_RESULT libResult = LIB_FAILURE;

  const uint32_t cLastValidIndex = mMaxNumberOfObjects - 1;

  /* Little bit of code to find double Deallocation
   const uint32_t cObjectSizeInBytes = ((sizeof(mpObjectArray[0]) + 3) & ~(3UL));
   const uint32_t cRelativeObjectAddress = ((uint32_t)rpObjectToBeDeallocated) - ((uint32_t)mpObjectArray);
   const uint32_t cObjectIndex = cRelativeObjectAddress / cObjectSizeInBytes;

   SHOW_ERROR(std::find(&(mpFreeObjects[0]),
                             &(mpFreeObjects[mNumberOfFreeObjects]),
                             cObjectIndex) == &(mpFreeObjects[mNumberOfFreeObjects]));*/

  if((gcRemoveSafetyChecksFromThisFile == true) ||
     ((rpObjectToBeDeallocated >= &(mpObjectArray[0])) &&
      (rpObjectToBeDeallocated <= &(mpObjectArray[cLastValidIndex])) && (mNumberOfFreeObjects < mMaxNumberOfObjects)))
  {
    const uint32_t  cObjectSizeInBytes     = ((sizeof(mpObjectArray[0]) + 3) & ~(3UL));
    const uintptr_t cRelativeObjectAddress = ((uintptr_t)rpObjectToBeDeallocated) - ((uintptr_t)mpObjectArray);
    const uint32_t  cObjectIndex           = cRelativeObjectAddress / cObjectSizeInBytes;

    if((&(mpObjectArray[cObjectIndex])) == rpObjectToBeDeallocated)
    {
      ++mNumberOfFreeObjects;

      mpFreeObjects[mNumberOfFreeObjects - 1] = cObjectIndex;

      rpObjectToBeDeallocated->~GdcFt_TrackedFeature();

      libResult = LIB_SUCCESS;
    }
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
uint32_t GdcFt_TrackedFeatureAllocator::Capacity() const
{
  return mMaxNumberOfObjects;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
uint32_t GdcFt_TrackedFeatureAllocator::Size() const
{
  return mNumberOfFreeObjects;
}

#ifndef __STANDALONE__
/***************************************************************************/
/**
 * \details
 * This function ...
 *
 **************************************************************************/
GdcFt_Task::GdcFt_Task()
    : mStackSizeInBytes(0), mPriority(0), mIsSemaphoreThreadSafetyCreated(false), mIsTaskSpawned(false),
      mIsTaskFinished(false)

{
  LIB_RESULT libResult = LIB_FAILURE;

  mName[0] = 0;

  // mSemaphoreThreadSafety: Starting count should be 1;
  libResult = GDC_OAL_SemaphoreCreate(&mSemaphoreThreadSafety, (char*)gcGdcFtTaskSemaphoreThreadSafetyName,
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
GdcFt_Task::~GdcFt_Task()
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsTaskSpawned == true && gcIsRunningWithThreads == true)
  {
    libResult = GdcFt_Task::Delete();
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
LIB_RESULT GdcFt_Task::Spawn(const uint32_t cStackSizeInBytes, const uint32_t cTaskPriority, const char cTaskName[])
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
            intResult = pthread_create(pThread, NULL, GdcFt_Task::TaskEntryRoutine, this);
            SHOW_ERROR(intResult == 0);
            if(intResult == 0)
            {
#if !defined(__CYGWIN__) && !defined(_WIN32)
              intResult = pthread_setname_np(*pThread, cTaskName);
              SHOW_ERROR(intResult == 0);
#endif
              if(intResult == 0)
              {
                mTaskControlBlock = (GdcFt_TCB)pThread;
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
          strncpy(mName, cTaskName, gcGdcFtTaskNameMaximumSizeInChar);
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
LIB_RESULT GdcFt_Task::Join()
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
LIB_RESULT GdcFt_Task::Delete()
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
void* GdcFt_Task::TaskEntryRoutine(void* pVoid)
{
  if(gcIsRunningWithThreads == true)
  {
    SHOW_ERROR(pVoid != 0);

    GdcFt_Task* const cpTask = (GdcFt_Task*)pVoid;

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
#endif

/***************************************************************************/
/**
 * \par Description
 * Function used to convert a set of RGB24 pixels to the Y pixel format
 * It is required since apexcv::ColorConverter does not support RGB24 to Y conversion
 *
 **************************************************************************/
LIB_RESULT GdcFt_Helper::CropAndConvertRgb24ToY(vsdk::UMat&    rSensorRgb24_umat,
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
LIB_RESULT GdcFt_Helper::ConvertYToRgb24(vsdk::UMat& rSource_umat, vsdk::UMat& rDestination_umat)
{
  LIB_RESULT libResult = LIB_FAILURE;

  const uint32_t cSourceWidthInPixels  = rSource_umat.cols;
  const uint32_t cSourceHeightInPixels = rSource_umat.rows;

  vsdk::Mat            lSource_mat      = rSource_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);
  vsdk::Mat            lDestination_mat = rDestination_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
  const uint8_t* const cpcSourceFrame   = (const uint8_t* const)lSource_mat.data;

  if(cpcSourceFrame != 0 && rDestination_umat.empty() == false)
  {
    const uint32_t cPixelInSizeInBytes  = 1; // Y
    const uint32_t cPixelOutSizeInBytes = 3; // RGB24

    const uint32_t cSourceSpanInBytes = rSource_umat.step[0];

    const uint8_t* pcLineIn = (const uint8_t*)cpcSourceFrame;

    const uint8_t* pcPixelIn = pcLineIn;
    uint8_t*       pPixelOut = (uint8_t*)lDestination_mat.data;

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
 * Function used to crop a RGB24 image
 *
 **************************************************************************/
LIB_RESULT GdcFt_Helper::CropRgb24(uint32_t const cSourceWidthInPixels,
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
LIB_RESULT GdcFt_Helper::CalculateSensorRoi(const uint32_t      cSensorWidthInPixels,
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
LIB_RESULT GdcFt_Helper::FilterFeatures(GdcFt_TrackedFeatureAllocator&    rTrackedFeatureAllocator,
                                        const uint32_t                    cTrackedFeatureFilterGridWidth,
                                        const uint32_t                    cTrackedFeatureFilterGridHeight,
                                        GdcFt_TrackedFeature** const      cppTrackedFeatureFilter,
                                        GdcFt_TrackedFeature** const      cppTrackedFeatures,
                                        int32_t&                          rNumberOfTrackedFeatures,
                                        /*const*/ icp::FeatureDescriptor& rcFeatureDescriptor,
                                        const int16_t                     cTrackedFeatureStrength_Threshold,
                                        const int16_t                     cDetectedFeatureStrength_Threshold)
{
  LIB_RESULT libResult = LIB_SUCCESS;

  const uint32_t cLastValidGridIndex = (gcFilterGridWidth * gcFilterGridHeight) - 1;

  /*
    *  1. Distribute tracked features in grid
    *    a. Grid represents ROI divided into AxB pixels; look at configuration for values of A and B.
    *    b. Discard features too close to each other.
    */
  int32_t conflictCounter = 0;

  for(int32_t i = 0; i < rNumberOfTrackedFeatures; ++i)
  {
    SHOW_ERROR(cppTrackedFeatures[i] != 0);

    bool weShouldKeepTracking = true; // meaning we want to keep tracking it.

    if(cppTrackedFeatures[i]->mAgeInFramesSinceDetection > 0 &&
       (cppTrackedFeatures[i]->mCurrent.strength < 0 ||
        cppTrackedFeatures[i]->mCurrent.strength > cTrackedFeatureStrength_Threshold))
    {
      weShouldKeepTracking = false;
    }

    // Past this point features are good enough for tracking
    if(weShouldKeepTracking == true)
    {
      const uint32_t cFilterGridIndex =
          (cppTrackedFeatures[i]->mCurrent.position.x / cTrackedFeatureFilterGridWidth) +
          ((cppTrackedFeatures[i]->mCurrent.position.y / cTrackedFeatureFilterGridHeight) * gcFilterGridWidth);

      SHOW_ERROR(cFilterGridIndex <= cLastValidGridIndex);

      if(cppTrackedFeatureFilter[cFilterGridIndex] == 0)
      {
        // If cell is empty then add the tracked feature
        cppTrackedFeatureFilter[cFilterGridIndex] = cppTrackedFeatures[i];
      }
      else
      {
        if(cppTrackedFeatures[i]->mAgeInFramesSinceDetection >=
           cppTrackedFeatureFilter[cFilterGridIndex]->mAgeInFramesSinceDetection)
        {
          libResult = rTrackedFeatureAllocator.Deallocate(cppTrackedFeatureFilter[cFilterGridIndex]);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          cppTrackedFeatureFilter[cFilterGridIndex] = cppTrackedFeatures[i];
        }
        else
        {
          libResult = rTrackedFeatureAllocator.Deallocate(cppTrackedFeatures[i]);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          ++conflictCounter;
        }
      }
    }
    else
    {
      libResult = rTrackedFeatureAllocator.Deallocate(cppTrackedFeatures[i]);
      SHOW_ERROR(libResult == LIB_SUCCESS);
    }
  }

  /*
    * 2. Distribute detected feature in grid
    *    a. if grid element is empty create new tracked feature.
    */
  int32_t cNumberOfDetectedFeatures = rcFeatureDescriptor.GetCount();

#if DO_DEBUG
  int16_t* lpDetectedFeatureStrengths = (int16_t*)calloc(cNumberOfDetectedFeatures, sizeof(int16_t)); // For debug
#endif

  for(int32_t i = 0; i < cNumberOfDetectedFeatures; ++i)
  {
    SHOW_ERROR(rcFeatureDescriptor[i].strength >= 0);

#if DO_DEBUG
    lpDetectedFeatureStrengths[i] = rcFeatureDescriptor[i].strength; // For debug
#endif
    const uint32_t cFilterGridIndex =
        (rcFeatureDescriptor[i].position.x / cTrackedFeatureFilterGridWidth) +
        ((rcFeatureDescriptor[i].position.y / cTrackedFeatureFilterGridHeight) * gcFilterGridWidth);

    if(!(rcFeatureDescriptor[i].position.x < 0 || rcFeatureDescriptor[i].position.y < 0 ||
         rcFeatureDescriptor[i].position.x >= gcSceneWidthInPixels ||
         rcFeatureDescriptor[i].position.y >= gcSceneHeightInPixels) && // Must be within bounds of the image
       (cppTrackedFeatureFilter[cFilterGridIndex] == 0 ||               // If empty fill it
        (cppTrackedFeatureFilter[cFilterGridIndex]->mAgeInFramesSinceDetection == 0 &&
         rcFeatureDescriptor[i].strength >
             cppTrackedFeatureFilter[cFilterGridIndex]
                 ->mCurrent.strength)) && // If choice between two detected, then pick the strongest one
       rcFeatureDescriptor[i].strength > cDetectedFeatureStrength_Threshold)
    {
      libResult = rTrackedFeatureAllocator.Allocate(cppTrackedFeatureFilter[cFilterGridIndex]);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      icp::Feature32S lFeature32S;

      lFeature32S.position.x = rcFeatureDescriptor[i].position.x;
      lFeature32S.position.y = rcFeatureDescriptor[i].position.y;
      lFeature32S.strength   = rcFeatureDescriptor[i].strength;
      lFeature32S.reserve    = rcFeatureDescriptor[i].reserve;

      libResult = cppTrackedFeatureFilter[cFilterGridIndex]->Update(lFeature32S);
      SHOW_ERROR(libResult == LIB_SUCCESS);
    }
  }

#if DO_DEBUG
  std::sort(lpDetectedFeatureStrengths, lpDetectedFeatureStrengths + cNumberOfDetectedFeatures); // For debug
  free(lpDetectedFeatureStrengths);                                                              // For debug
  lpDetectedFeatureStrengths = 0;                                                                // For debug
#endif

  /*
    * 3. Re-populate the tracked feature array with content of the grid.
    */
  rNumberOfTrackedFeatures = 0;

  for(uint32_t i = 0; i <= cLastValidGridIndex; ++i)
  {
    if(cppTrackedFeatureFilter[i] != 0)
    {
      cppTrackedFeatures[rNumberOfTrackedFeatures] = cppTrackedFeatureFilter[i];

      ++(cppTrackedFeatures[rNumberOfTrackedFeatures]->mAgeInFramesSinceDetection);

      ++rNumberOfTrackedFeatures;
    }
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
int32_t GdcFt_Helper::DisplacementAbs(ICP_Point const cPoint1, ICP_Point const cPoint2)
{
  const int32_t cStartingX = cPoint1.x;
  const int32_t cStartingY = cPoint1.y;
  const int32_t cCurrentX  = cPoint2.x;
  const int32_t cCurrentY  = cPoint2.y;

  const GdcFt_MinMax cMinMaxX =
      (cStartingX < cCurrentX) ? GdcFt_MinMax(cStartingX, cCurrentX) : GdcFt_MinMax(cCurrentX, cStartingX);
  const GdcFt_MinMax cMinMaxY =
      (cStartingY < cCurrentY) ? GdcFt_MinMax(cStartingY, cCurrentY) : GdcFt_MinMax(cCurrentY, cStartingY);

  const int32_t cAbsoluteDisplacementX = abs(cMinMaxX.mMax - cMinMaxX.mMin);
  const int32_t cAbsoluteDisplacementY = abs(cMinMaxY.mMax - cMinMaxY.mMin);

  return sqrt32((cAbsoluteDisplacementX * cAbsoluteDisplacementX) + (cAbsoluteDisplacementY * cAbsoluteDisplacementY));
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcFt_Helper::SendRequest(const int32_t cMessageId,
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
LIB_RESULT GdcFt_Helper::SendImageReply(const int32_t  cReceiverQueueId,
                                        vsdk::UMat&    rSceneRgb24_umat,
                                        const void*    pcSceneRgb24_umat_data,
                                        vsdk::UMat&    rScene_umat,
                                        const void*    pcScene_umat_data,
                                        const uint32_t cSceneCounter,
                                        const uint64_t (&cTicks)[2])
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcFt_ImageReply* pReply = (GdcFt_ImageReply*)malloc(sizeof(GdcFt_ImageReply));

  if(pReply != 0)
  {
    new(pReply) GdcFt_ImageReply(); // Placement new

    pReply->mMessageId              = MESSAGE_ID_GDC_FT_IMAGE_REPLY;
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
      pReply->~GdcFt_ImageReply();
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
LIB_RESULT GdcFt_Helper::SendDetectorData(const int32_t           cReceiverQueueId,
                                          vsdk::UMat&             rSceneRgb24_umat,
                                          const void*             pcSceneRgb24_umat_data,
                                          vsdk::UMat&             rScene_umat,
                                          const void*             pcScene_umat_data,
                                          const uint32_t          cSceneCounter,
                                          void* const             chFeatureDescriptorOal,
                                          icp::FeatureDescriptor& rFeatureDescriptor,
                                          const bool              cWeDetectedFeatures,
                                          const uint64_t (&cDetectorTicks)[gcApexCvEndTick])
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcFt_DetectorData* pData = (GdcFt_DetectorData*)malloc(sizeof(GdcFt_DetectorData));

  if(pData != 0)
  {
    new(pData) GdcFt_DetectorData(); // Placement new

    pData->mMessageId              = MESSAGE_ID_GDC_FT_DETECTOR_DATA;
    pData->mSenderId               = 0; // not used, because no reply is expected.
    pData->mCorrelationId          = 0; // not used, because no reply is expected.
    pData->mSceneRgb24_umat        = rSceneRgb24_umat;
    pData->mpcSceneRgb24_umat_data = pcSceneRgb24_umat_data;
    pData->mScene_umat             = rScene_umat;
    pData->mpcScene_umat_data      = pcScene_umat_data;
    pData->mSceneCounter           = cSceneCounter;

    pData->mhFeatureDescriptorOal = chFeatureDescriptorOal;

    pData->mFeatureDescriptor.Init(rFeatureDescriptor.GetDataPtr(), rFeatureDescriptor.GetDataPtrPhys(),
                                   rFeatureDescriptor.GetSize());

    pData->mFeatureDescriptor.SetCount(rFeatureDescriptor.GetCount());

    pData->mWeDetectedFeatures = cWeDetectedFeatures;

    std::copy(cDetectorTicks, cDetectorTicks + gcApexCvEndTick, pData->mTicks);

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pData, sizeof(*pData), 1);
    if(libResult != LIB_SUCCESS)
    {
      pData->mFeatureDescriptor.SetCount(0);
      pData->mFeatureDescriptor.Init(0, 0, 0);
      pData->~GdcFt_DetectorData();
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
LIB_RESULT GdcFt_Helper::SendPyramidData(const int32_t           cReceiverQueueId,
                                         vsdk::UMat&             rSceneRgb24_umat,
                                         const void*             pcSceneRgb24_umat_data,
                                         vsdk::UMat&             rScene_umat,
                                         const void*             pcScene_umat_data,
                                         const uint32_t          cSceneCounter,
                                         void* const             chFeatureDescriptorOal,
                                         icp::FeatureDescriptor& rFeatureDescriptor,
                                         const bool              cWeDetectedFeatures,
                                         vsdk::UMat (&rImagePyramid_umat)[gcDepthOfImagePyramid],
                                         const uint64_t (&cPyramidTicks)[gcApexCvEndTick])
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcFt_PyramidData* pData = (GdcFt_PyramidData*)malloc(sizeof(GdcFt_PyramidData));

  if(pData != 0)
  {
    new(pData) GdcFt_PyramidData(); // Placement new

    pData->mMessageId              = MESSAGE_ID_GDC_FT_PYRAMID_DATA;
    pData->mSenderId               = 0; // not used, because no reply is expected.
    pData->mCorrelationId          = 0; // not used, because no reply is expected.
    pData->mSceneRgb24_umat        = rSceneRgb24_umat;
    pData->mpcSceneRgb24_umat_data = pcSceneRgb24_umat_data;
    pData->mScene_umat             = rScene_umat;
    pData->mpcScene_umat_data      = pcScene_umat_data;
    pData->mSceneCounter           = cSceneCounter;

    pData->mhFeatureDescriptorOal = chFeatureDescriptorOal;

    pData->mFeatureDescriptor.Init(rFeatureDescriptor.GetDataPtr(), rFeatureDescriptor.GetDataPtrPhys(),
                                   rFeatureDescriptor.GetSize());

    pData->mFeatureDescriptor.SetCount(rFeatureDescriptor.GetCount());

    pData->mWeDetectedFeatures = cWeDetectedFeatures;

    for(uint32_t i = 0; i < gcDepthOfImagePyramid; ++i)
    {
      pData->mImagePyramid_umat[i] = rImagePyramid_umat[i];
    }

    std::copy(cPyramidTicks, cPyramidTicks + gcApexCvEndTick, pData->mTicks);

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pData, sizeof(*pData), 1);
    if(libResult != LIB_SUCCESS)
    {
      pData->mFeatureDescriptor.SetCount(0);
      pData->mFeatureDescriptor.Init(0, 0, 0);
      pData->~GdcFt_PyramidData();
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
LIB_RESULT GdcFt_Helper::SendTrackerData(const int32_t                          cReceiverQueueId,
                                         vsdk::UMat&                            rSceneRgb24_umat,
                                         const void*                            pcSceneRgb24_umat_data,
                                         vsdk::UMat&                            rScene_umat,
                                         const void*                            pcScene_umat_data,
                                         const uint32_t                         cSceneCounter,
                                         /*const*/ GdcFt_TrackedFeature** const ppcTrackedFeatures,
                                         const uint32_t                         cNumberOfTrackedFeatures,
                                         const uint32_t                         cNumberOfDetectedFeatures,
                                         const uint64_t (&cTrackerTicks)[gcApexCvEndTick])
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcFt_TrackerData* pData = (GdcFt_TrackerData*)malloc(sizeof(GdcFt_TrackerData));

  if(pData != 0)
  {
    new(pData) GdcFt_TrackerData(); // Placement new

    pData->mMessageId              = MESSAGE_ID_GDC_FT_TRACKER_DATA;
    pData->mSenderId               = 0; // not used, because no reply is expected.
    pData->mCorrelationId          = 0; // not used, because no reply is expected.
    pData->mSceneRgb24_umat        = rSceneRgb24_umat;
    pData->mpcSceneRgb24_umat_data = pcSceneRgb24_umat_data;
    pData->mScene_umat             = rScene_umat;
    pData->mpcScene_umat_data      = pcScene_umat_data;
    pData->mSceneCounter           = cSceneCounter;

    for(uint32_t i = 0; i < cNumberOfTrackedFeatures; ++i)
    {
      pData->mTrackedFeatures[i] = *(ppcTrackedFeatures[i]);
    }
    pData->mNumberOfDetectedFeatures = cNumberOfDetectedFeatures;
    pData->mNumberOfTrackedFeatures  = cNumberOfTrackedFeatures;

    std::copy(cTrackerTicks, cTrackerTicks + gcApexCvEndTick, pData->mTicks);

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pData, sizeof(*pData), 1);

    if(libResult != LIB_SUCCESS)
    {
      pData->~GdcFt_TrackerData();
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
LIB_RESULT GdcFt_Helper::SendCompositionData(const int32_t  cReceiverQueueId,
                                             vsdk::UMat&    rSceneRgb24_umat,
                                             const void*    pcSceneRgb24_umat_data,
                                             const uint32_t cSceneCounter,
                                             const uint64_t (&cTrackerTicks)[gcApexCvEndTick])
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcFt_CompositionData* pData = (GdcFt_CompositionData*)malloc(sizeof(GdcFt_CompositionData));

  if(pData != 0)
  {
    new(pData) GdcFt_CompositionData(); // Placement new

    pData->mMessageId              = MESSAGE_ID_GDC_FT_COMPOSITION_DATA;
    pData->mSenderId               = 0; // not used, because no reply is expected.
    pData->mCorrelationId          = 0; // not used, because no reply is expected.
    pData->mSceneRgb24_umat        = rSceneRgb24_umat;
    pData->mpcSceneRgb24_umat_data = pcSceneRgb24_umat_data;
    pData->mSceneCounter           = cSceneCounter;

    std::copy(cTrackerTicks, cTrackerTicks + gcApexCvEndTick, pData->mTicks);

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pData, sizeof(*pData), 1);

    if(libResult != LIB_SUCCESS)
    {
      pData->~GdcFt_CompositionData();
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
LIB_RESULT GdcFt_Helper::FreeImageReply(GdcFt_ImageReply* const cpReply)
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
LIB_RESULT GdcFt_Helper::FreeDetectorData(GdcFt_DetectorData* const cpData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(cpData != 0)
  {
    if(cpData->mhFeatureDescriptorOal != 0)
    {
      OAL_MemoryFree(cpData->mhFeatureDescriptorOal);
      cpData->mhFeatureDescriptorOal = 0;
    }
    cpData->mFeatureDescriptor.SetCount(0);
    cpData->mFeatureDescriptor.Init(0, 0, 0);

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcFt_Helper::FreePyramidData(GdcFt_PyramidData* const cpData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(cpData != 0)
  {
    if(cpData->mhFeatureDescriptorOal != 0)
    {
      OAL_MemoryFree(cpData->mhFeatureDescriptorOal);
      cpData->mhFeatureDescriptorOal = 0;
    }
    cpData->mFeatureDescriptor.SetCount(0);
    cpData->mFeatureDescriptor.Init(0, 0, 0);

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcFt_Helper::FreeTrackerData(GdcFt_TrackerData* const cpData)
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
LIB_RESULT GdcFt_Helper::FreeCompositionData(GdcFt_CompositionData* const cpData)
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
#define SQRT_STEP                                                                                                      \
  tmp >>= 2;                                                                                                           \
  testVal = tmp + root;                                                                                                \
  if(testVal <= value)                                                                                                 \
  {                                                                                                                    \
    value -= testVal;                                                                                                  \
    root = (root >> 1) | tmp;                                                                                          \
  }                                                                                                                    \
  else                                                                                                                 \
  {                                                                                                                    \
    root >>= 1;                                                                                                        \
  }

static int32_t sqrt32(int32_t value)
{
  //ASSERT(value >= 0);         // must be non-negative number!

  int32_t root = 0, tmp = 0x40000000l, testVal;

  // bit 0
  if(tmp <= value)
  {
    value -= tmp;
    root = tmp;
  }

  SQRT_STEP; // bit 2
  SQRT_STEP; // bit 4
  SQRT_STEP; // bit 6
  SQRT_STEP; // bit 8
  SQRT_STEP; // bit 10
  SQRT_STEP; // bit 12
  SQRT_STEP; // bit 14
  SQRT_STEP; // bit 16
  SQRT_STEP; // bit 18
  SQRT_STEP; // bit 20
  SQRT_STEP; // bit 22
  SQRT_STEP; // bit 24
  SQRT_STEP; // bit 26
  SQRT_STEP; // bit 28
  SQRT_STEP; // bit 30

  // round to the nearest integer, cuts max error in half
  if(root < value)
    ++root;

  return root;
}
