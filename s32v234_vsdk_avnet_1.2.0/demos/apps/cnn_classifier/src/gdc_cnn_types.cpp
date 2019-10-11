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
#include "gdc_cnn_types.hpp"
#include "gdc_cnn_messages.hpp"
#include <apexcv_pro_resize.h>
#include "icp_data.h"
#include "communications.hpp"
#include "oal.h"
#include "global_errors.h"
#include "iniparser.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdint.h>
#include <new>
#include <algorithm>
#include <cmath>

#include "nxp_logo.h"
#include "t32term.h"

/**********************************************************************************************************/
// GdcCnn_MovingAverage function definitions
/**********************************************************************************************************/

GdcCnn_MovingAverage::GdcCnn_MovingAverage()
    : mSum(0), mCount_Current(0), mCount_Max(1) // Must be positive otherwise might lead to division by zero
{
}

GdcCnn_MovingAverage::~GdcCnn_MovingAverage()
{
}

void GdcCnn_MovingAverage::Initialize(const uint32_t cCount_Max)
{
  mCount_Max = cCount_Max;
}

uint64_t GdcCnn_MovingAverage::Update(const uint64_t cNewValue)
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

uint64_t GdcCnn_MovingAverage::Get() const
{
  return (mSum / mCount_Current);
}

#ifndef __STANDALONE__
/***************************************************************************/
/**
 * \details
 * This function ...
 *
 **************************************************************************/
GdcCnn_Task::GdcCnn_Task()
    : mStackSizeInBytes(0), mPriority(0), mIsSemaphoreThreadSafetyCreated(false), mIsTaskSpawned(false),
      mIsTaskFinished(false)

{
  LIB_RESULT libResult = LIB_FAILURE;

  mName[0] = 0;

  // mSemaphoreThreadSafety: Starting count should be 1;
  libResult = GDC_OAL_SemaphoreCreate(&mSemaphoreThreadSafety, (char*)gcGdcCnnTaskSemaphoreThreadSafetyName,
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
GdcCnn_Task::~GdcCnn_Task()
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsTaskSpawned == true && gcIsRunningWithThreads == true)
  {
    libResult = GdcCnn_Task::Delete();
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
LIB_RESULT GdcCnn_Task::Spawn(const uint32_t cStackSizeInBytes, const uint32_t cTaskPriority, const char cTaskName[])
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
            intResult = pthread_create(pThread, NULL, GdcCnn_Task::TaskEntryRoutine, this);
            SHOW_ERROR(intResult == 0);
            if(intResult == 0)
            {
#if !defined(__CYGWIN__) && !defined(_WIN32)
              intResult = pthread_setname_np(*pThread, cTaskName);
              SHOW_ERROR(intResult == 0);
#endif
              if(intResult == 0)
              {
                mTaskControlBlock = (GdcCnn_TCB)pThread;
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
          strncpy(mName, cTaskName, gcGdcCnnTaskNameMaximumSizeInChar);
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
LIB_RESULT GdcCnn_Task::Join()
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
        libResult       = LIB_SUCCESS;
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
LIB_RESULT GdcCnn_Task::Delete()
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
void* GdcCnn_Task::TaskEntryRoutine(void* pVoid)
{
  if(gcIsRunningWithThreads == true)
  {
    SHOW_ERROR(pVoid != 0);

    GdcCnn_Task* const cpTask = (GdcCnn_Task*)pVoid;

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

/**********************************************************************************************************/
// GdcCnn_Helper function definitions
/**********************************************************************************************************/

/***************************************************************************/
/**
 *
 **************************************************************************/
LIB_RESULT GdcCnn_Helper::ReadDatasetListFromFile(const char*                 pcDatasetList_Filename,
                                                  std::vector< std::string >& rDatasetList)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(pcDatasetList_Filename != 0)
  {
    std::ifstream lIfstream(pcDatasetList_Filename);

    if(lIfstream.is_open() == true)
    {
      std::string lString;

      while(lIfstream.good() == true)
      {
        std::getline(lIfstream, lString);

        if(lString.size() > 0)
        {
          if(lString[0] != '#')
          {
            GdcCnn_Helper::trim(lString);

            if(lString.size() > 0)
            {
              rDatasetList.push_back(lString);
            }
          }
          lString.clear();
        }
        else
        {
          break;
        }
      }

      lIfstream.close();

      libResult = LIB_SUCCESS;
    }
    else
    {
      SHOW_ERROR(false);
      fprintf(stderr, "GdcCnn_ClassifierTask: Failed to open [%s]\n", pcDatasetList_Filename);
    }
  }

  return libResult;
}

/***************************************************************************/
/**
 *
 **************************************************************************/
LIB_RESULT GdcCnn_Helper::ReadWeightModelFromFile(const char* pcSqueezeNetModel_Filename,
                                                  int8_t*&    rpSqueezeNet_WeightModel)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(pcSqueezeNetModel_Filename != 0 && rpSqueezeNet_WeightModel == 0)
  {
    FILE* pFile = fopen(pcSqueezeNetModel_Filename, "rb");

    if(pFile != 0)
    {
      fseek(pFile, 0, SEEK_END);
      int32_t lFileSizeInBytes = ftell(pFile);

      if(lFileSizeInBytes == gcSqueezeNetModel_Filesize)
      {
        fseek(pFile, 0, SEEK_SET);

        rpSqueezeNet_WeightModel = (int8_t*)malloc(lFileSizeInBytes);
        SHOW_ERROR(rpSqueezeNet_WeightModel != 0);

        if(rpSqueezeNet_WeightModel != 0)
        {
          const size_t cElementsRead = fread(rpSqueezeNet_WeightModel, lFileSizeInBytes, 1, pFile);
          SHOW_ERROR(cElementsRead == 1);

          if(cElementsRead == 1)
          {
            libResult = LIB_SUCCESS;
          }
        }
      }
      else
      {
        SHOW_ERROR(false);
        fprintf(stderr, "GdcCnn_ClassifierTask: Invalid file size [%s], %d != %d\n", pcSqueezeNetModel_Filename,
                lFileSizeInBytes, gcSqueezeNetModel_Filesize);
      }

      fclose(pFile);
      pFile = 0;
    }
    else
    {
      SHOW_ERROR(false);
      fprintf(stderr, "GdcCnn_ClassifierTask: Failed to open [%s]\n", pcSqueezeNetModel_Filename);
    }
  }

  return libResult;
}

/***************************************************************************/
/**
  *
 **************************************************************************/
LIB_RESULT GdcCnn_Helper::ReadSynonymSetFromFile(const char*                 pcSqueezeNetSynonymSet_Filename,
                                                 std::vector< std::string >& rSqueezeNet_SynonymSet)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(pcSqueezeNetSynonymSet_Filename != 0)
  {
    std::ifstream lIfstream(pcSqueezeNetSynonymSet_Filename);

    if(lIfstream.is_open() == true)
    {
      std::string lString;

      while(lIfstream.good() == true)
      {
        std::getline(lIfstream, lString);

        if(lString.size() > 0)
        {
          const size_t cPosition_Space = lString.find_first_of(" \n");
          const size_t cPosition_Colon = lString.find_first_of(",\n");

          rSqueezeNet_SynonymSet.push_back(
              lString.substr(cPosition_Space + 1, cPosition_Colon - (cPosition_Space + 1)));

          lString.clear();
        }
        else
        {
          break;
        }
      }

      libResult = LIB_SUCCESS;

      lIfstream.close();
    }
    else
    {
      SHOW_ERROR(false);
      fprintf(stderr, "GdcCnn_ClassifierTask: Failed to open [%s]\n", pcSqueezeNetSynonymSet_Filename);
    }
  }

  return libResult;
}

/***************************************************************************/
/**
  *
 **************************************************************************/
LIB_RESULT GdcCnn_Helper::ReadReferenceResultsFromFile(
    const char*                                   pcSqueezeNetReferenceResults_Filename,
    std::map< std::string, std::set< int32_t > >& rSqueezeNet_ReferenceResults)
{
  LIB_RESULT libResult = LIB_SUCCESS;

  if(pcSqueezeNetReferenceResults_Filename != 0)
  {
    std::ifstream lIfstream(pcSqueezeNetReferenceResults_Filename);

    if(lIfstream.is_open() == true)
    {
      //
      // Expected format is space separated values: <SceneName> <SynonymSet_Index0> <SynonymSet_Index1> .. <SynonymSet_IndexN>
      // Lines starting with '#' are ignored
      //
      std::string                lString;
      const std::string          cDelimiters(" \t");
      std::vector< std::string > lTokens;

      while(lIfstream.good() == true)
      {
        std::getline(lIfstream, lString);

        if(lString.size() > 0 && lString[0] != '#')
        {
          GdcCnn_Helper::tokenize(lString, cDelimiters, lTokens);

          if(lTokens.size() > 1)
          {
            std::set< int32_t > lSet;

            std::pair< std::map< std::string, std::set< int32_t > >::iterator, bool > lPair;

            lPair = rSqueezeNet_ReferenceResults.insert(std::make_pair(lTokens[0], lSet));

            for(uint32_t i = 1; i < lTokens.size(); ++i)
            {
              lPair.first->second.insert(std::stoi(lTokens[i]));
            }
          }
          else
          {
            // Need at least filename and one index...
            SHOW_ERROR(false);
            //libResult = LIB_FAILURE;
          }
        }
        lString.clear();
        lTokens.clear();
      }

      lIfstream.close();
    }
    else
    {
      SHOW_ERROR(false);
      fprintf(stderr, "GdcCnn_ClassifierTask: Failed to open [%s]\n", pcSqueezeNetReferenceResults_Filename);
      libResult = LIB_FAILURE;
    }
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * Function used to 2D copy data into composition buffer.
 *
 **************************************************************************/
LIB_RESULT GdcCnn_Helper::MemCopy2D(void* const    cpDestination,
                                    void* const    cpSource,
                                    uint32_t const cDestinationSpanInBytes,
                                    uint32_t const cDestinationHorizontalOffsetInPixels,
                                    uint32_t const cDestinationVerticalOffsetInPixels,
                                    uint32_t const cDestinationBytesPerPixel,
                                    uint32_t const cSourceBytesPerPixel,
                                    uint32_t const cSourceWidthInPixels,
                                    uint32_t const cSourceHeightInPixels)
{
  LIB_RESULT libResult = LIB_FAILURE;

  SHOW_ERROR(cDestinationBytesPerPixel == 3); // Only support RGB24

  if(cpDestination != 0 && cpSource != 0 && cDestinationBytesPerPixel == 3 && // Only support RGB24
     (cSourceBytesPerPixel == 1 || cSourceBytesPerPixel == 3))                // Support RGB24 or GS8
  {
    uint8_t* pDst_Line =
        (uint8_t*)(((uintptr_t)cpDestination) + (cDestinationVerticalOffsetInPixels * cDestinationSpanInBytes) +
                   (cDestinationHorizontalOffsetInPixels *
                    cDestinationBytesPerPixel)); // Position to first pixel in first line

    uint8_t* pDst_Pixel = pDst_Line;

    uint8_t* pSrc_Pixel = (uint8_t*)cpSource;

    for(uint32_t h = 0; h < cSourceHeightInPixels; ++h) // For every line
    {
      for(uint32_t w = 0; w < cSourceWidthInPixels; ++w) // For pixel on that line
      {
        if(cSourceBytesPerPixel == 1) // GS8
        {
          pDst_Pixel[0] = pSrc_Pixel[0];
          pDst_Pixel[1] = pSrc_Pixel[0];
          pDst_Pixel[2] = pSrc_Pixel[0];
        }
        else if(cSourceBytesPerPixel == 3) // RGB24
        {
          pDst_Pixel[0] = pSrc_Pixel[0];
          pDst_Pixel[1] = pSrc_Pixel[1];
          pDst_Pixel[2] = pSrc_Pixel[2];
        }
        else
        {
          SHOW_ERROR(false);
        }

        pDst_Pixel += cDestinationBytesPerPixel;
        pSrc_Pixel += cSourceBytesPerPixel;
      }

      pDst_Line += cDestinationSpanInBytes;
      pDst_Pixel = pDst_Line;
    }

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/*!*********************************************************************************
 *  @brief Function used to render a string of RGB24 formast buffer
 *
 */
LIB_RESULT GdcCnn_Helper::ReverseCopyRgb24(void* const    cpDestination,
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

void GdcCnn_Helper::SetLogo(int32_t const  cWidthInPixels,
                            uint32_t const cHorizontalOffsetInPixels,
                            uint32_t const cVerticalOffsetInPixels,
                            uint8_t* const cpBuffer)
{
  const uint32_t cPixelSizeInBytes = 3;
  const uint32_t cLineSpanInBytes  = (cWidthInPixels * cPixelSizeInBytes);

  uint8_t* pLine      = (uint8_t*)(cpBuffer + ((cVerticalOffsetInPixels * cLineSpanInBytes) +
                                          cHorizontalOffsetInPixels * cPixelSizeInBytes));
  uint32_t lLogoIndex = 0;
  uint8_t* pPixel     = pLine;

  for(int i = 0; i < NXP_LOGO_HEIGHT; i++)
  {
    for(int j = 0; j < (NXP_LOGO_WIDTH); j++)
    {
      if((nxp_logo[lLogoIndex] != 0xff) || (nxp_logo[lLogoIndex + 1] != 0xff) || (nxp_logo[lLogoIndex + 2] != 0xff))
      {
        pPixel[2] = nxp_logo[lLogoIndex];
        pPixel[1] = nxp_logo[lLogoIndex + 1];
        pPixel[0] = nxp_logo[lLogoIndex + 2];
      }
      pPixel += cPixelSizeInBytes;
      lLogoIndex += cPixelSizeInBytes;
    }
    pLine += cLineSpanInBytes;
    pPixel = pLine;
  }
}

/***************************************************************************/
/**
 * \par Description
 * Function used to crop a RGB24 image
 *
 **************************************************************************/
LIB_RESULT GdcCnn_Helper::CropRgb24(uint32_t const cSourceWidthInPixels,
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

#ifndef __STANDALONE__
/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcCnn_Helper::SendRequest(const int32_t cMessageId,
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
LIB_RESULT GdcCnn_Helper::SendRequest(const int32_t cMessageId,
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
LIB_RESULT GdcCnn_Helper::SendImageReply(const int32_t      cReceiverQueueId,
                                         cv::Mat&           rSceneMat,
                                         const uint32_t     cSceneCounter,
                                         const std::string& rcSceneName,
                                         const uint64_t (&cTicks)[gcApexCvEndTick])
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcCnn_ImageReply* pReply = (GdcCnn_ImageReply*)malloc(sizeof(*pReply));

  if(pReply != 0)
  {
    new(pReply) GdcCnn_ImageReply(); // Placement new

    pReply->mMessageId     = MESSAGE_ID_GDC_CNN_IMAGE_REPLY;
    pReply->mSenderId      = 0; // not used, because no reply is expected.
    pReply->mCorrelationId = 0; // not used, because no reply is expected.
    pReply->mSceneMat      = rSceneMat;
    pReply->mSceneCounter  = cSceneCounter;
    pReply->mSceneName     = rcSceneName;

    std::copy(cTicks, cTicks + gcApexCvEndTick, pReply->mTicks);

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pReply, sizeof(*pReply), 1);
    if(libResult != LIB_SUCCESS)
    {
      pReply->~GdcCnn_ImageReply();
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
LIB_RESULT GdcCnn_Helper::SendResizeData(const int32_t      cReceiverQueueId,
                                         const cv::Mat&     rSceneMat,
                                         const uint32_t     cSceneCounter,
                                         const std::string& rcSceneName,
                                         vsdk::UMat&        rPaddedScene_umat,
                                         const void*        pcPaddedScene_umat_data,
                                         const uint64_t (&cResizeTicks)[gcApexCvEndTick])
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(rPaddedScene_umat.empty() == false)
  {
    GdcCnn_ResizeData* pData = (GdcCnn_ResizeData*)malloc(sizeof(*pData));

    if(pData != 0)
    {
      new(pData) GdcCnn_ResizeData(); // Placement new

      pData->mMessageId     = MESSAGE_ID_GDC_RESIZE_DATA;
      pData->mSenderId      = 0; // not used, because no reply is expected.
      pData->mCorrelationId = 0; // not used, because no reply is expected.
      pData->mSceneMat      = rSceneMat;
      pData->mSceneCounter  = cSceneCounter;
      pData->mSceneName     = rcSceneName;

      pData->mPaddedScene_umat        = rPaddedScene_umat;
      pData->mpcPaddedScene_umat_data = pcPaddedScene_umat_data;

      std::copy(cResizeTicks, cResizeTicks + gcApexCvEndTick, pData->mTicks);

      libResult = MsgMgr_MessageSend(cReceiverQueueId, pData, sizeof(*pData), 1);
      if(libResult != LIB_SUCCESS)
      {
        pData->~GdcCnn_ResizeData();
        free((void*)pData);
        pData = 0;
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
LIB_RESULT GdcCnn_Helper::SendClassifierData(
    const int32_t  cReceiverQueueId,
    const cv::Mat& rcSceneMat,
    const uint32_t cSceneCounter,
    const uint16_t (&rcClassificationIndexes_Local)[gcSqueezeNet_NumberOfClassifications],
    const std::string (&rcClassificationLabels_Local)[gcSqueezeNet_NumberOfClassifications],
    const float (&rcClassificationScores_Local)[gcSqueezeNet_NumberOfClassifications],
    const int32_t      rcClassificationIndex_Reference,
    const std::string& rcClassificationLabel_Reference,
    const uint64_t (&rcClassificationTicks)[gcApexCvEndTick])
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcCnn_ClassifierData* pData = (GdcCnn_ClassifierData*)malloc(sizeof(*pData));

  if(pData != 0)
  {
    new(pData) GdcCnn_ClassifierData(); // Placement new

    pData->mMessageId     = MESSAGE_ID_GDC_CNN_CLASSIFIER_DATA;
    pData->mSenderId      = 0; // not used, because no reply is expected.
    pData->mCorrelationId = 0; // not used, because no reply is expected.
    pData->mSceneMat      = rcSceneMat;
    pData->mSceneCounter  = cSceneCounter;

    for(uint32_t i = 0; i < gcSqueezeNet_NumberOfClassifications; ++i)
    {
      pData->mClassificationIndexes_Local[i] = rcClassificationIndexes_Local[i];
      pData->mClassificationLabels_Local[i]  = rcClassificationLabels_Local[i];
      pData->mClassificationScores_Local[i]  = rcClassificationScores_Local[i];
    }

    pData->mClassificationIndex_Reference = rcClassificationIndex_Reference;
    pData->mClassificationLabel_Reference = rcClassificationLabel_Reference;

    std::copy(rcClassificationTicks, rcClassificationTicks + gcApexCvEndTick, pData->mTicks);

    libResult = MsgMgr_MessageSend(cReceiverQueueId, pData, sizeof(*pData), 1);
    if(libResult != LIB_SUCCESS)
    {
      pData->~GdcCnn_ClassifierData();
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
LIB_RESULT GdcCnn_Helper::SendCompositionData(const int32_t                  cReceiverQueueId,
                                              void* const                    cpComposition,
                                              /*const*/ icp::DataDescriptor& rcCompositionDd,
                                              const uint32_t                 cRightY,
                                              const uint32_t                 cSceneCounter,
                                              const uint64_t (&rcTicks)[gcApexCvEndTick])
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(cpComposition != 0)
  {
    GdcCnn_CompositionData* pData = (GdcCnn_CompositionData*)malloc(sizeof(*pData));

    if(pData != 0)
    {
      new(pData) GdcCnn_CompositionData(); // Placement new

      pData->mMessageId     = MESSAGE_ID_GDC_CNN_COMPOSITION_DATA;
      pData->mSenderId      = 0; // not used, because no reply is expected.
      pData->mCorrelationId = 0; // not used, because no reply is expected.

      pData->mpComposition = cpComposition;
      pData->mRightY       = cRightY;
      pData->mSceneCounter = cSceneCounter;
      std::copy(rcTicks, rcTicks + gcApexCvEndTick, pData->mTicks);

      pData->mCompositionDd.InitManual(rcCompositionDd.GetWidth(), rcCompositionDd.GetHeight(),
                                       rcCompositionDd.GetDataPtr(), rcCompositionDd.GetDataPtrPhys(),
                                       rcCompositionDd.GetElementDataType(), rcCompositionDd.GetElementDimX(),
                                       rcCompositionDd.GetElementDimY());

      pData->mCompositionDd.SetSpan(rcCompositionDd.GetSpan());

      libResult = MsgMgr_MessageSend(cReceiverQueueId, pData, sizeof(*pData), 1);
      if(libResult != LIB_SUCCESS)
      {
        pData->mpComposition = 0; // Ownership of the buffer is still the callers.
        pData->mCompositionDd.UpdatePtrs(0, 0);
        pData->~GdcCnn_CompositionData();
        free((void*)pData);
        pData = 0;
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
LIB_RESULT GdcCnn_Helper::FreeImageReply(GdcCnn_ImageReply* const cpReply)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(cpReply != 0)
  {
    // Nothing to do

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcCnn_Helper::FreeResizeData(GdcCnn_ResizeData* const cpData)
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
LIB_RESULT GdcCnn_Helper::FreeClassifierData(GdcCnn_ClassifierData* const /*cpData*/)
{
  LIB_RESULT libResult = LIB_SUCCESS;

  // Nothing to do

  return libResult;
}

/***************************************************************************/
/**
 **************************************************************************/
timespec GdcCnn_Helper::timespec_diff(timespec const cMinuend, timespec const cSubtrahend)
{
  timespec temp;

  if((cMinuend.tv_nsec - cSubtrahend.tv_nsec) < 0)
  {
    temp.tv_sec  = cMinuend.tv_sec - cSubtrahend.tv_sec - 1;
    temp.tv_nsec = cMinuend.tv_nsec - cSubtrahend.tv_nsec + 1E9;
  }
  else
  {
    temp.tv_sec  = cMinuend.tv_sec - cSubtrahend.tv_sec;
    temp.tv_nsec = cMinuend.tv_nsec - cSubtrahend.tv_nsec;
  }

  return temp;
}

/***************************************************************************/
/**
 **************************************************************************/
GdcCnn_SceneTask_Enum GdcCnn_Helper::ConvertToSceneTaskEnum(const uint32_t cValue)
{
  GdcCnn_SceneTask_Enum lSceneTask_Enum = GDC_CNN_SCENE_TASK_INVALID;

  switch(cValue)
  {
    case 1:
    {
      lSceneTask_Enum = GDC_CNN_SCENE_TASK_FILE_READER;
      break;
    }
    case 2:
    {
      lSceneTask_Enum = GDC_CNN_SCENE_TASK_IMAGE_SENSOR;
      break;
    }
    case 3:
    {
      lSceneTask_Enum = GDC_CNN_SCENE_TASK_IP_CAMERA;
      break;
    }
    default:
    {
      SHOW_ERROR(false);
      lSceneTask_Enum = GDC_CNN_SCENE_TASK_INVALID;
      break;
    }
  };

  return lSceneTask_Enum;
}

/***************************************************************************/
/**
 **************************************************************************/
void GdcCnn_Helper::trim(std::string& rString)
{
  const std::string& cWhitespaces = " \t";
  const size_t       cPosBegin    = rString.find_first_not_of(cWhitespaces);

  if(cPosBegin != std::string::npos)
  {
    const size_t cPosEnd = rString.find_last_not_of(cWhitespaces);

    const size_t cLength = cPosEnd - cPosBegin + 1;

    rString = rString.substr(cPosBegin, cLength);
  }
  else
  {
    rString.clear();
  }
}

/***************************************************************************/
/**
 **************************************************************************/
void GdcCnn_Helper::tokenize(const std::string&          rcString,
                             const std::string&          rcDelimiters,
                             std::vector< std::string >& rTokens)
{
  rTokens.clear();

  size_t lPosBegin = 0;
  size_t lPosEnd   = 0;

  while(1)
  {
    lPosBegin = rcString.find_first_not_of(rcDelimiters, lPosBegin);

    lPosEnd = rcString.find_first_of(rcDelimiters, lPosBegin);

    if(lPosBegin != std::string::npos)
    {
      const size_t cPosEnd = (lPosEnd != std::string::npos) ? lPosEnd : rcString.length();

      rTokens.push_back(rcString.substr(lPosBegin, (cPosEnd - lPosBegin)));

      lPosBegin = cPosEnd + 1;
    }
    else
    {
      break;
    }
  }
}

LIB_RESULT GdcCnn_Helper::transformUMat(vsdk::UMat& InputImage, apexdnnTensorDescriptor* Tensor)
{
  const int WIDTH_STRIDE = 700;
  const int HEIGHT       = 227;

  vsdk::Mat tempMat = InputImage.getMat(cv::ACCESS_RW | OAL_USAGE_CACHED);
  int8_t*   pInput  = (int8_t*)tempMat.data;
  if(pInput == NULL || Tensor == NULL)
  {
    return LIB_FAILURE;
  }
  int8_t* tensorData = (int8_t*)apexdnnRetTensorDataPtr(Tensor);

  for(int h = 0; h < HEIGHT; h++)
  {
    memcpy(tensorData, pInput, WIDTH_STRIDE);
    tensorData += WIDTH_STRIDE;
    pInput += (InputImage.step[0] / InputImage.elemSize());
  }

  return LIB_SUCCESS;
}
#endif
