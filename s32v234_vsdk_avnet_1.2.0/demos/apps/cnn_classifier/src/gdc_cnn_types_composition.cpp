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

#define NXP_LOGO_WIDTH (196)
#define NXP_LOGO_HEIGHT (110)

/**********************************************************************************************************/
// GdcCnn_Composition function definitions
/**********************************************************************************************************/

/***************************************************************************/
/**
 * \par Description
 * Constructor should set values to default state
 * Resource allocation should happen outside the constructor since it cannot
 * return any values indicating success or failure.
 *
 **************************************************************************/
GdcCnn_Composition::GdcCnn_Composition()
    : mClassificationCounter_Top1(0), mClassificationCounter_Top5(0), mClassificationCounter_Total(0),
      //GdcCnn_MovingAverage mMovingAverage_Scene;
      //GdcCnn_MovingAverage mMovingAverage_Resize;
      //GdcCnn_MovingAverage mMovingAverage_Padding;
      //GdcCnn_MovingAverage mMovingAverage_Classifier;
      //GdcCnn_MovingAverage mMovingAverage_SqueezeNet;
      //GdcCnn_MovingAverage mMovingAverage_Composition;
      //GdcCnn_MovingAverage mMovingAverage_SqueezeNetApex;
      //GdcCnn_MovingAverage mMovingAverage_Display;
      //GdcCnn_MovingAverage mMovingAverage_FramePeriod;
      mFrameTicks(0), mWidthInPixels(0), mHeightInPixels(0), mBitsPerPixel(0), mScaleNumerator(1), mScaleDenominator(1),
      mCompositionBufferSizeInBytes(0), mWeCompareWithReference(false), mIsInitialized(false)
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
GdcCnn_Composition::~GdcCnn_Composition()
{
}

/***************************************************************************/
/**
 * \par Description
 * Initialization function prepares this object for use.
 * Resources are allocated and members variables are set.
 *
 **************************************************************************/
LIB_RESULT GdcCnn_Composition::Initialize(const uint32_t cWidthInPixels,
                                          const uint32_t cHeightInPixels,
                                          const uint32_t cBitsPerPixel,
                                          const uint32_t cScaleNumerator,
                                          const uint32_t cScaleDenominator,
                                          const bool     cWeCompareWithReference)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsInitialized == false)
  {
    mWidthInPixels  = (cWidthInPixels * cScaleNumerator) / cScaleDenominator;
    mHeightInPixels = (cHeightInPixels * cScaleNumerator) / cScaleDenominator;
    mBitsPerPixel   = cBitsPerPixel;

    mScaleNumerator   = cScaleNumerator;
    mScaleDenominator = cScaleDenominator;

    mCompositionBufferSizeInBytes = mWidthInPixels * mHeightInPixels * (mBitsPerPixel / 8); // 8 bits per bytes

    mMovingAverage_Scene.Initialize(16);          // Magic number bad!!!
    mMovingAverage_Resize.Initialize(16);         // Magic number bad!!!
    mMovingAverage_Padding.Initialize(16);        // Magic number bad!!!
    mMovingAverage_Classifier.Initialize(16);     // Magic number bad!!!
    mMovingAverage_SqueezeNet.Initialize(16);     // Magic number bad!!!
    mMovingAverage_SqueezeNetApex.Initialize(16); // Magic number bad!!!
    mMovingAverage_Composition.Initialize(16);    // Magic number bad!!!
    mMovingAverage_FramePeriod.Initialize(16);    // Magic number bad!!!

    mWeCompareWithReference = cWeCompareWithReference;

    mIsInitialized = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * Deinitialization function releases any leftover resources owned by the object
 *
 **************************************************************************/
LIB_RESULT GdcCnn_Composition::Deinitialize()
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
 *
 **************************************************************************/
LIB_RESULT GdcCnn_Composition::Compose(GdcCnn_ClassifierData& rData,
                                       void* const            cpCompositionBuffer,
                                       uint32_t const         cCompositionBufferSizeInBytes,
                                       icp::DataDescriptor&   rCompositionDd,
                                       uint32_t&              rRightY)
{
  LIB_RESULT libResult = LIB_FAILURE;

  uint64_t ticks[2] = {0};

  ticks[0] = FSL_Ticks();

  if(mIsInitialized == true && cCompositionBufferSizeInBytes >= mCompositionBufferSizeInBytes)
  {
    rCompositionDd.InitManual(mWidthInPixels, mHeightInPixels, cpCompositionBuffer, 0, icp::DATATYPE_08U,
                              mBitsPerPixel / 8, 1);

    rCompositionDd.SetSpan((mWidthInPixels * mBitsPerPixel) / 8);

    memset(cpCompositionBuffer, 0, mCompositionBufferSizeInBytes);

    cv::Mat lMatCompositionBuffer(mHeightInPixels, mWidthInPixels, CV_8UC3, cpCompositionBuffer,
                                  mWidthInPixels * (mBitsPerPixel / 8)); // 8 bits per byte
    //
    // Prepare labels
    //
    const int    cFontFace            = cv::FONT_HERSHEY_SIMPLEX;
    const double cFontScale_Label     = (1.75 * mScaleNumerator) / mScaleDenominator;
    const int    cFontThickness_Label = (3.75 * mScaleNumerator) / mScaleDenominator;

    bool    lWeHaveAMatch = false;
    int32_t lMatchIndex   = -1;

    cv::Size lSizeFonts(0, 0);
    cv::Size lSizeFont[gcSqueezeNet_NumberOfClassifications];

    cv::Scalar lScalarColors[gcSqueezeNet_NumberOfClassifications + 1];

    const cv::Scalar lScalarColor_Pass(0, 255, 0); // Green
    const cv::Scalar lScalarColor_Fail(0xCC, 0xCC, 0);
    const cv::Scalar lScalarColor_Ref(0, 0, 255); // Red

    std::ostringstream lOStringStream[gcSqueezeNet_NumberOfClassifications];
    std::string        lStringDebug;
    int                baseline = 0;

    for(uint32_t i = 0; i < gcSqueezeNet_NumberOfClassifications; ++i)
    {
      const int32_t cPercentage = (int32_t)(rData.mClassificationScores_Local[i] * 100.0);

      SHOW_ERROR(cPercentage >= 0 && cPercentage <= 100);

      lOStringStream[i] << "[" << std::right << std::setw(3) << cPercentage
                        << "%] : " << rData.mClassificationLabels_Local[i];

      lStringDebug = lOStringStream[i].str();

      lSizeFont[i] = cv::getTextSize(lStringDebug, cFontFace, cFontScale_Label, cFontThickness_Label, &baseline);

      if(lSizeFont[i].width > lSizeFonts.width)
      {
        lSizeFonts.width = (lSizeFont[i].width * 6) / 5;
      } // Magic Number Bad!!!!
      lSizeFonts.height += (lSizeFont[i].height + ((20 * mScaleNumerator) / mScaleDenominator));
      lSizeFont[i].height += (24 * mScaleNumerator) / mScaleDenominator; // Magic Number Bad!!!!

      if(rData.mClassificationIndexes_Local[i] == rData.mClassificationIndex_Reference)
      {
        lScalarColors[i] = lScalarColor_Pass;
        lWeHaveAMatch    = true;
        if(lMatchIndex == -1)
        {
          lMatchIndex = i;
        }
      }
      else
      {
        lScalarColors[i] = lScalarColor_Fail;
      }
    }

    if(mWeCompareWithReference == true && lWeHaveAMatch == false)
    {
      lOStringStream[0] << " (" << rData.mClassificationLabel_Reference << ")";

      lScalarColors[0] = lScalarColor_Ref;
    }

    //
    // Draw labels
    //
    uint32_t lLabelY = mHeightInPixels - lSizeFonts.height;

    for(uint32_t i = 0; i < gcSqueezeNet_NumberOfClassifications; ++i)
    {
      cv::putText(lMatCompositionBuffer, lOStringStream[i].str(), cv::Point(0, lLabelY), cFontFace, cFontScale_Label,
                  lScalarColors[i], cFontThickness_Label, cv::LINE_AA, false);

      lLabelY += lSizeFont[i].height;

      lOStringStream[i].clear();
      lOStringStream[i].str("");
    }

    cv::Mat lResizedSceneMat;

    //
    // Find largest ROI in destination which has aspect ratio of the source
    //
    const uint32_t cDstWidth  = mWidthInPixels / 2;
    const uint32_t cDstHeight = mHeightInPixels - lSizeFonts.height - ((48 * mScaleNumerator) / mScaleDenominator);

    const double cSrcWidth      = rData.mSceneMat.cols;
    const double cSrcHeight     = rData.mSceneMat.rows;
    const double cSrcWidthRatio = cSrcWidth / cSrcHeight;

    uint32_t lResizeWidth  = 0;
    uint32_t lResizeHeight = 0;

    if(cSrcWidthRatio >= 1.0)
    {
      //
      // width greater than height
      //
      lResizeWidth  = cDstWidth;
      lResizeHeight = (uint32_t)(cDstWidth * (1.0 / cSrcWidthRatio));
    }
    else
    {
      //
      // height greater than width
      //
      lResizeWidth  = (uint32_t)(cDstHeight * cSrcWidthRatio);
      lResizeHeight = cDstHeight;
    }

    if(lResizeWidth > cDstWidth)
    {
      lResizeWidth = cDstWidth;
    }
    if(lResizeHeight > cDstHeight)
    {
      lResizeHeight = cDstHeight;
    }

    cv::Size lResizedSceneSize(lResizeWidth, lResizeHeight);

    cv::resize(rData.mSceneMat, lResizedSceneMat, lResizedSceneSize);

    libResult =
        GdcCnn_Helper::MemCopy2D(cpCompositionBuffer, lResizedSceneMat.data,
                                 mWidthInPixels * (mBitsPerPixel / 8), // 8 bits per byte
                                 (cDstWidth - lResizedSceneMat.cols) / 2, (cDstHeight - lResizedSceneMat.rows) / 2,
                                 mBitsPerPixel / 8, // 8 bits per byte
                                 lResizedSceneMat.elemSize(), lResizedSceneMat.cols, lResizedSceneMat.rows);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    //
    // Right screen side information
    //
    double lRightSide_FontScale     = (1.75 * mScaleNumerator) / mScaleDenominator;
    int    lRightSide_FontThickness = (3.75 * mScaleNumerator) / mScaleDenominator;

    std::ostringstream& rOStringStream = lOStringStream[0];
    cv::Size&           rSizeFont      = lSizeFont[0];

    const uint32_t cRightSide_X = mWidthInPixels / 2;

    uint32_t lRightSide_Y = (16 * mScaleNumerator) / mScaleDenominator;

    rOStringStream << "[" << std::right << std::setw(5) << rData.mSceneCounter << "] : Image";

    lStringDebug = rOStringStream.str();

    rSizeFont = cv::getTextSize(lStringDebug, cFontFace, lRightSide_FontScale, lRightSide_FontThickness, &baseline);

    cv::putText(lMatCompositionBuffer, lStringDebug, cv::Point(cRightSide_X, lRightSide_Y + rSizeFont.height),
                cFontFace, lRightSide_FontScale, cv::Scalar(0, 255, 0), lRightSide_FontThickness, cv::LINE_AA, false);

    lRightSide_Y += (rSizeFont.height + ((28 * mScaleNumerator) / mScaleDenominator)); // MagicNumber Bad!!!

    rOStringStream.clear();
    rOStringStream.str("");

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

    rOStringStream << "[" << std::fixed << std::right << std::setw(5) << std::setprecision(2) << lFramesPerSecond
                   << "] : Frames per Second";

    lStringDebug = rOStringStream.str();

    rSizeFont = cv::getTextSize(lStringDebug, cFontFace, lRightSide_FontScale, lRightSide_FontThickness, &baseline);

    cv::putText(lMatCompositionBuffer, lStringDebug, cv::Point(cRightSide_X, lRightSide_Y + rSizeFont.height),
                cFontFace, lRightSide_FontScale, cv::Scalar(0, 255, 0), lRightSide_FontThickness, cv::LINE_AA, false);

    lRightSide_Y += (rSizeFont.height + ((28 * mScaleNumerator) / mScaleDenominator)); // MagicNumber Bad!!!

    rOStringStream.clear();
    rOStringStream.str("");

    //
    // Update stats
    //
    if(lWeHaveAMatch == true)
    {
      if(lMatchIndex == 0)
      {
        ++mClassificationCounter_Top1;
      }
      if(lMatchIndex >= 0 && (uint32_t)lMatchIndex < gcSqueezeNet_NumberOfClassifications)
      {
        ++mClassificationCounter_Top5;
      }
    }

    ++mClassificationCounter_Total;

    //
    // Draw stats
    //
    lRightSide_FontScale     = (1.50 * mScaleNumerator) / mScaleDenominator;
    lRightSide_FontThickness = (3.5 * mScaleNumerator) / mScaleDenominator;

    lRightSide_Y += (4 * rSizeFont.height); // MagicNumber Bad!!!
    //
    // Top 1
    //
    double lClassificationPercentage = mClassificationCounter_Top1;

    lClassificationPercentage = (lClassificationPercentage * 100.0) / (double)mClassificationCounter_Total;

    rOStringStream << "[" << std::fixed << std::right << std::setw(3) << std::setprecision(2)
                   << lClassificationPercentage << "%] : Top 1";

    lStringDebug = rOStringStream.str();

    rSizeFont = cv::getTextSize(lStringDebug, cFontFace, lRightSide_FontScale, lRightSide_FontThickness, &baseline);
    if(mWeCompareWithReference == true)
    {
      cv::putText(lMatCompositionBuffer, lStringDebug, cv::Point(cRightSide_X, lRightSide_Y + rSizeFont.height),
                  cFontFace, lRightSide_FontScale, cv::Scalar(0, 255, 0), lRightSide_FontThickness, cv::LINE_AA, false);
    }

    lRightSide_Y += (rSizeFont.height + ((28 * mScaleNumerator) / mScaleDenominator)); // MagicNumber Bad!!!

    rOStringStream.clear();
    rOStringStream.str("");

    //
    // Top 5
    //
    lClassificationPercentage = mClassificationCounter_Top5;

    lClassificationPercentage = (lClassificationPercentage * 100.0) / (double)mClassificationCounter_Total;

    rOStringStream << "[" << std::fixed << std::right << std::setw(3) << std::setprecision(2)
                   << lClassificationPercentage << "%] : Top 5";

    lStringDebug = rOStringStream.str();

    rSizeFont = cv::getTextSize(lStringDebug, cFontFace, lRightSide_FontScale, lRightSide_FontThickness, &baseline);

    if(mWeCompareWithReference == true)
    {
      cv::putText(lMatCompositionBuffer, lStringDebug, cv::Point(cRightSide_X, lRightSide_Y + rSizeFont.height),
                  cFontFace, lRightSide_FontScale, cv::Scalar(0, 255, 0), lRightSide_FontThickness, cv::LINE_AA, false);
    }

    lRightSide_Y += (rSizeFont.height + ((28 * mScaleNumerator) / mScaleDenominator)); // MagicNumber Bad!!!

    rOStringStream.clear();
    rOStringStream.str("");

    //
    // Update ticks / moving averages
    //
    mMovingAverage_Scene.Update(rData.mTicks[gcSceneTick + 1] - rData.mTicks[gcSceneTick]);
    mMovingAverage_Resize.Update(rData.mTicks[gcResizeTick + 1] - rData.mTicks[gcResizeTick]);
    mMovingAverage_Padding.Update(rData.mTicks[gcResizeTick + 2] - rData.mTicks[gcResizeTick + 1]);
    mMovingAverage_Classifier.Update(rData.mTicks[gcClassifierTick + 3] - rData.mTicks[gcClassifierTick]);
    mMovingAverage_SqueezeNet.Update(rData.mTicks[gcClassifierTick + 2] - rData.mTicks[gcClassifierTick + 1]);

    uint32_t lSqueezeNet_ApuTotal = 0;

    mMovingAverage_SqueezeNetApex.Update(lSqueezeNet_ApuTotal);

    //
    // Draw periods in us
    //
    lRightSide_FontScale     = (1.2 * mScaleNumerator) / mScaleDenominator;
    lRightSide_FontThickness = (3.0 * mScaleNumerator) / mScaleDenominator;

    const uint32_t cNumberOfTicks                  = 2;
    uint64_t       lMovingAverages[cNumberOfTicks] = {(mMovingAverage_Scene.Get() / cTimeDivisor),
                                                //(mMovingAverage_Resize.Get()/cTimeDivisor),
                                                //(mMovingAverage_Padding.Get()/cTimeDivisor),
                                                //(mMovingAverage_Classifier.Get()/cTimeDivisor),
                                                //(mMovingAverage_SqueezeNet.Get()/cTimeDivisor),
                                                (mMovingAverage_Composition.Get() / cTimeDivisor)};

    std::string lMovingAverage_String[cNumberOfTicks] = {"Input",
                                                         //"Resize",
                                                         //"Padding"
                                                         //"Classifier",
                                                         //"SqueezeNet",
                                                         "Composition"};

    lRightSide_Y += (3 * rSizeFont.height); // MagicNumber Bad!!!

    for(uint32_t i = 0; i < cNumberOfTicks; ++i)
    {
      rOStringStream << "[" << std::right << std::setw(6) << lMovingAverages[i]
                     << " us] : " << lMovingAverage_String[i];

      lStringDebug = rOStringStream.str();

      rSizeFont = cv::getTextSize(lStringDebug, cFontFace, lRightSide_FontScale, lRightSide_FontThickness, &baseline);

      cv::putText(lMatCompositionBuffer, lStringDebug, cv::Point(cRightSide_X, lRightSide_Y + rSizeFont.height),
                  cFontFace, lRightSide_FontScale, cv::Scalar(0, 255, 0), lRightSide_FontThickness, cv::LINE_AA, false);

      lRightSide_Y += (rSizeFont.height + ((28 * mScaleNumerator) / mScaleDenominator)); // MagicNumber Bad!!!

      rOStringStream.clear();
      rOStringStream.str("");
    }

    //
    // Draw logo
    //
    GdcCnn_Helper::SetLogo(mWidthInPixels,
                           mWidthInPixels - NXP_LOGO_WIDTH - ((376 * mScaleNumerator) / mScaleDenominator),
                           mHeightInPixels - NXP_LOGO_HEIGHT - ((96 * mScaleNumerator) / mScaleDenominator),
                           (uint8_t*)cpCompositionBuffer);

    ticks[1] = FSL_Ticks();
    mMovingAverage_Composition.Update(ticks[1] - ticks[0]);

    rRightY = lRightSide_Y;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcCnn_Composition::BufferSizeInBytes_Get(uint32_t& rBufferSizeInBytes)
{
  LIB_RESULT libResult = LIB_SUCCESS;

  rBufferSizeInBytes = mCompositionBufferSizeInBytes;

  return libResult;
}
