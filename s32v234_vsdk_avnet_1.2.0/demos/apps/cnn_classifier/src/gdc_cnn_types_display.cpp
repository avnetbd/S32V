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

#include "t32term.h"

/**********************************************************************************************************/
// GdcCnn_Display function definitions
/**********************************************************************************************************/

/***************************************************************************/
/**
 * \par Description
 * Constructor should set values to default state
 * Resource allocation should happen outside the constructor since it cannot
 * return any values indicating success or failure.
 *
 **************************************************************************/
GdcCnn_Display::GdcCnn_Display()
    : //GdcCnn_MovingAverage mMovingAverage_Display;
      mWidthInPixels(0), mHeightInPixels(0), mBitsPerPixel(0), mScaleNumerator(1), mScaleDenominator(1),
      mWeRotateDisplay(false), mDisplayBufferSizeInBytes(0), mpDisplayBuffer_Rotated(0),
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
GdcCnn_Display::~GdcCnn_Display()
{
  if(mpDisplayBuffer_Rotated != 0)
  {
    free(mpDisplayBuffer_Rotated);
    mpDisplayBuffer_Rotated = 0;
  }
}

/***************************************************************************/
/**
 * \par Description
 * Initialization function prepares this object for use.
 * Resources are allocated and members variables are set.
 *
 **************************************************************************/
LIB_RESULT GdcCnn_Display::Initialize(const uint32_t cWidthInPixels,
                                      const uint32_t cHeightInPixels,
                                      const uint32_t cBitsPerPixel,
                                      const uint32_t cScaleNumerator,
                                      const uint32_t cScaleDenominator,
                                      const bool     cWeRotateDisplay)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsInitialized == false)
  {
    mWidthInPixels  = (cWidthInPixels * cScaleNumerator) / cScaleDenominator;
    mHeightInPixels = (cHeightInPixels * cScaleNumerator) / cScaleDenominator;
    mBitsPerPixel   = cBitsPerPixel;

    mScaleNumerator   = cScaleNumerator;
    mScaleDenominator = cScaleDenominator;

    mWeRotateDisplay = cWeRotateDisplay;

    mDisplayBufferSizeInBytes = mWidthInPixels * mHeightInPixels * (mBitsPerPixel / 8); // 8 bits per bytes

    mpDisplayBuffer_Rotated = malloc(mDisplayBufferSizeInBytes);

    if(mpDisplayBuffer_Rotated != 0)
    {
      //
      // io::FrameOutputV234Fb
      //
      mFrameOutput.Init(mWidthInPixels, mHeightInPixels, gcDisplayIoDataDepth, gcDisplayIoDataChannels);
      mIsInitialized = true;

      libResult = LIB_SUCCESS;
    }
    else
    {
      if(mpDisplayBuffer_Rotated != 0)
      {
        free(mpDisplayBuffer_Rotated);
        mpDisplayBuffer_Rotated = 0;
      }
    }

    mMovingAverage_Display.Initialize(16); // Magic number bad!!!
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * Deinitialization function releases any leftover resources owned by the object
 *
 **************************************************************************/
LIB_RESULT GdcCnn_Display::Deinitialize()
{
  LIB_RESULT libResult = LIB_SUCCESS;

  if(mIsInitialized == true)
  {
    // io::FrameOutputV234Fb does give me a means of deinitializing it other than the destructor!!!

    if(mpDisplayBuffer_Rotated != 0)
    {
      free(mpDisplayBuffer_Rotated);
      mpDisplayBuffer_Rotated = 0;
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
LIB_RESULT GdcCnn_Display::PutFrame(GdcCnn_CompositionData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  uint64_t ticks[3] = {0};

  ticks[0] = FSL_Ticks();

  if(mIsInitialized == true)
  {
    //
    // Rotate and/or Update Display device
    //
    if(mWeRotateDisplay == false)
    {
      mFrameOutput.PutFrame(rData.mpComposition, false);

      libResult = LIB_SUCCESS;
    }
    else if(mWeRotateDisplay == true)
    {
      libResult = GdcCnn_Helper::ReverseCopyRgb24(mpDisplayBuffer_Rotated, rData.mpComposition, mWidthInPixels,
                                                  mHeightInPixels);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      mFrameOutput.PutFrame(mpDisplayBuffer_Rotated, false);
    }

    ticks[2] = FSL_Ticks();
    mMovingAverage_Display.Update(ticks[2] - ticks[1]);
  }

  return libResult;
}
