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

#include "t32term.h"

/**********************************************************************************************************/
// GdcFt_Display function definitions
/**********************************************************************************************************/

/***************************************************************************/
/**
 * \par Description
 * Constructor should set values to default state
 * Resource allocation should happen outside the constructor since it cannot
 * return any values indicating success or failure.
 *
 **************************************************************************/
GdcFt_Display::GdcFt_Display()
    : mDisplayWidthInPixels(0), mDisplayHeightInPixels(0), mDisplayBitsPerPixel(0), mWeRotateDisplay(false),
      mpSceneRgb24_Rotated(0), mSceneRgb24_Rotated_SizeInBytes(0), mpSceneRgb24_Resized(0),
      mSceneRgb24_Resized_SizeInBytes(0),
      //io_FrameOutput_t mFrameOutput;
      mIsInitialized(false)
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
GdcFt_Display::~GdcFt_Display()
{
  if(mpSceneRgb24_Rotated != 0)
  {
    free(mpSceneRgb24_Rotated);
    mpSceneRgb24_Rotated            = 0;
    mSceneRgb24_Rotated_SizeInBytes = 0;
  }

  if(mpSceneRgb24_Resized != 0)
  {
    free(mpSceneRgb24_Resized);
    mpSceneRgb24_Resized            = 0;
    mSceneRgb24_Resized_SizeInBytes = 0;
  }
}

/***************************************************************************/
/**
 * \par Description
 * Initialization function prepares this object for use.
 * Resources are allocated and members variables are set.
 *
 **************************************************************************/
LIB_RESULT GdcFt_Display::Initialize(const uint32_t cDisplayWidthInPixels,
                                     const uint32_t cDisplayHeightInPixels,
                                     const uint32_t cDisplayBitsPerPixel,
                                     const bool     cWeRotateDisplay)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsInitialized == false)
  {
    mDisplayWidthInPixels  = cDisplayWidthInPixels;
    mDisplayHeightInPixels = cDisplayHeightInPixels;
    mDisplayBitsPerPixel   = cDisplayBitsPerPixel;

    mWeRotateDisplay = cWeRotateDisplay;

    mSceneRgb24_Resized_SizeInBytes =
        (mDisplayWidthInPixels * mDisplayHeightInPixels * mDisplayBitsPerPixel) / 8; // bits per byte

    mpSceneRgb24_Resized = malloc(mSceneRgb24_Resized_SizeInBytes);

    if(mpSceneRgb24_Resized != 0)
    {
      //
      // io::FrameOutputV234Fb
      //
      mFrameOutput.Init(mDisplayWidthInPixels, mDisplayHeightInPixels, gcDisplayIoDataDepth, gcDisplayIoDataChannels);

      mIsInitialized = true;

      libResult = LIB_SUCCESS;
    }
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * Deinitialization function releases any leftover resources owned by the object
 *
 **************************************************************************/
LIB_RESULT GdcFt_Display::Deinitialize()
{
  LIB_RESULT libResult = LIB_SUCCESS;

  if(mIsInitialized == true)
  {
    // io::FrameOutputV234Fb does give me a means of deinitializing it other than the destructor!!!

    if(mpSceneRgb24_Rotated != 0)
    {
      free(mpSceneRgb24_Rotated);
      mpSceneRgb24_Rotated            = 0;
      mSceneRgb24_Rotated_SizeInBytes = 0;
    }

    if(mpSceneRgb24_Resized != 0)
    {
      free(mpSceneRgb24_Resized);
      mpSceneRgb24_Resized            = 0;
      mSceneRgb24_Resized_SizeInBytes = 0;
    }

    mIsInitialized = false;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcFt_Display::PutFrame(GdcFt_CompositionData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsInitialized == true)
  {
    const uint32_t lcSceneWidthInPixels  = rData.mSceneRgb24_umat.cols;
    const uint32_t lcSceneHeightInPixels = rData.mSceneRgb24_umat.rows;
    const uint32_t lcSceneSizeInBytes = lcSceneWidthInPixels * lcSceneHeightInPixels * 3; // 3 bytes per pixel for RGB24

    if(mSceneRgb24_Rotated_SizeInBytes < lcSceneSizeInBytes)
    {
      if(mpSceneRgb24_Rotated != 0)
      {
        free(mpSceneRgb24_Rotated);
      }

      mpSceneRgb24_Rotated = malloc(lcSceneSizeInBytes);

      mSceneRgb24_Rotated_SizeInBytes = lcSceneSizeInBytes;
    }

    if(mpSceneRgb24_Rotated != 0)
    {
      vsdk::Mat lSceneRgb24_mat = rData.mSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);
      void*     lpSceneRgb24    = (void*)(lSceneRgb24_mat.data);
      SHOW_ERROR(lpSceneRgb24 != 0);
      void* lpDisplayRgb24 = 0;

      if(mWeRotateDisplay == false && lcSceneWidthInPixels == mDisplayWidthInPixels &&
         lcSceneHeightInPixels == mDisplayHeightInPixels)
      {
        lpDisplayRgb24 = lpSceneRgb24;
      }
      else if(mWeRotateDisplay == true && lcSceneWidthInPixels == mDisplayWidthInPixels &&
              lcSceneHeightInPixels == mDisplayHeightInPixels)
      {
        libResult = GdcFt_Helper::ReverseCopyRgb24(mpSceneRgb24_Rotated, lpSceneRgb24, lcSceneWidthInPixels,
                                                   lcSceneHeightInPixels);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        lpDisplayRgb24 = mpSceneRgb24_Rotated;
      }
      else if(mWeRotateDisplay == false &&
              (lcSceneWidthInPixels != mDisplayWidthInPixels || lcSceneHeightInPixels != mDisplayHeightInPixels))
      {
        cv::Mat lSceneMat(
            lcSceneHeightInPixels, lcSceneWidthInPixels, CV_8UC3,
            lpSceneRgb24); // The external data is not automatically deallocated, so you should take care of it.

        cv::Mat lDisplayMat(
            mDisplayHeightInPixels, mDisplayWidthInPixels, CV_8UC3,
            mpSceneRgb24_Resized); // The external data is not automatically deallocated, so you should take care of it.

        cv::Size lDisplaySize(mDisplayWidthInPixels, mDisplayHeightInPixels);

        cv::resize(lSceneMat, lDisplayMat, lDisplaySize);

        lpDisplayRgb24 = mpSceneRgb24_Resized;
      }
      else if(mWeRotateDisplay == true &&
              (lcSceneWidthInPixels != mDisplayWidthInPixels || lcSceneHeightInPixels != mDisplayHeightInPixels))
      {
        libResult = GdcFt_Helper::ReverseCopyRgb24(mpSceneRgb24_Rotated, lpSceneRgb24, lcSceneWidthInPixels,
                                                   lcSceneHeightInPixels);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        cv::Mat lSceneMat(
            lcSceneHeightInPixels, lcSceneWidthInPixels, CV_8UC3,
            mpSceneRgb24_Rotated); // The external data is not automatically deallocated, so you should take care of it.

        cv::Mat lDisplayMat(
            mDisplayHeightInPixels, mDisplayWidthInPixels, CV_8UC3,
            mpSceneRgb24_Resized); // The external data is not automatically deallocated, so you should take care of it.

        cv::Size lDisplaySize(mDisplayWidthInPixels, mDisplayHeightInPixels);

        cv::resize(lSceneMat, lDisplayMat, lDisplaySize);

        lpDisplayRgb24 = mpSceneRgb24_Resized;
      }

      if(lpDisplayRgb24 != 0)
      {
        mFrameOutput.PutFrame(lpDisplayRgb24, false);

        libResult = LIB_SUCCESS;
      }
      else
      {
        SHOW_ERROR(false);
      }
    }
  }

  return libResult;
}

/*!*********************************************************************************
 *  @brief Function used to render a string of RGB24 formast buffer
 *
 */
LIB_RESULT GdcFt_Helper::ReverseCopyRgb24(void* const    cpDestination,
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
