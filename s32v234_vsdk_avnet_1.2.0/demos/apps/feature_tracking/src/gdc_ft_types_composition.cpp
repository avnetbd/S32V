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
#include "oal.h"
#include "global_errors.h"
#include "iniparser.h"
#include <stdlib.h>
#include <stdint.h>
#include <new>
#include <algorithm>
#include <cmath>

#include "nxp_logo.h"
#include "Font7x13.h"
#include "t32term.h"

#include <apexcv_pro_resize.h>
#include "resize_ref.h"
#include "oal_protected.h"

const bool gcRemoveSafetyChecksFromThisFile = false;

/**********************************************************************************************************/
// GdcFt_Composition function definitions
/**********************************************************************************************************/

/***************************************************************************/
/**
 * \par Description
 * Constructor should set values to default state
 * Resource allocation should happen outside the constructor since it cannot
 * return any values indicating success or failure.
 *
 **************************************************************************/
GdcFt_Composition::GdcFt_Composition()
    : mWeDrawFullPaths(gcComposition_WeDrawFullPaths),
      //GdcFt_MovingAverage mMovingAverage_FramePeriod;
      mFrameTicks(0), mDisplayTicks(0), mIsInitialized(false), mAvgIndex(0)
{
  memset((char*)mAvgPerformance, 0, sizeof(mAvgPerformance));
}

/***************************************************************************/
/**
 * \par Description
 * Destructor should handle freeing of any leftover resources
 * Resource deallocation should happen outside the destructor since it cannot
 * return any values indicating success or failure.
 *
 **************************************************************************/
GdcFt_Composition::~GdcFt_Composition()
{
}

/***************************************************************************/
/**
 * \par Description
 * Initialization function prepares this object for use.
 * Resources are allocated and members variables are set.
 *
 **************************************************************************/
LIB_RESULT GdcFt_Composition::Initialize(const bool cWeDrawFullPaths)
{
  LIB_RESULT libResult = LIB_SUCCESS;

  if(mIsInitialized == false)
  {
    mWeDrawFullPaths = cWeDrawFullPaths;

    mMovingAverage_FramePeriod.Initialize(16); // Magic number bad!!!

    mDisplayTicks = FSL_Ticks();

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
LIB_RESULT GdcFt_Composition::Deinitialize()
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
LIB_RESULT GdcFt_Composition::PutFrame(GdcFt_TrackerData& rData)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsInitialized == true)
  {
    const uint32_t cSceneWidthInPixels  = rData.mScene_umat.cols;
    const uint32_t cSceneHeightInPixels = rData.mScene_umat.rows;

    bool weNeedToConvertYToRgb24 = false;

    if(rData.mSceneRgb24_umat.empty() == true)
    {
      rData.mSceneRgb24_umat.create(cSceneHeightInPixels, cSceneWidthInPixels, CV_8UC3);
      SHOW_ERROR(rData.mSceneRgb24_umat.empty() == false);

      weNeedToConvertYToRgb24 = true;
    }

    if(rData.mSceneRgb24_umat.empty() == false)
    {
      vsdk::Mat lSceneRgb24_mat = rData.mSceneRgb24_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);

      void* lpSceneRgb24 = (void*)(lSceneRgb24_mat.data);
      SHOW_ERROR(lpSceneRgb24 != 0);

      if(weNeedToConvertYToRgb24 == true)
      {
        libResult = GdcFt_Helper::ConvertYToRgb24(rData.mScene_umat, rData.mSceneRgb24_umat);
        SHOW_ERROR(libResult == LIB_SUCCESS);
      }
      else
      {
        libResult = LIB_SUCCESS;
      }

      if(libResult == LIB_SUCCESS)
      {
        uint64_t difDisplayTicks = (FSL_Ticks() - mDisplayTicks) / 1000;

        mDisplayTicks = FSL_Ticks();

        if(rData.mNumberOfTrackedFeatures > 0)
        {
          if(mWeDrawFullPaths == false)
          {
            for(uint32_t i = 0; i < rData.mNumberOfTrackedFeatures; ++i)
            {
              ICP_Point positionBack;
              ICP_Point positionFront;

              libResult = rData.mTrackedFeatures[i].GetBack(positionBack);
              SHOW_ERROR(libResult == LIB_SUCCESS);
              libResult = rData.mTrackedFeatures[i].GetFront(positionFront);
              SHOW_ERROR(libResult == LIB_SUCCESS);

              uint32_t colorRgb24 = 0;

              libResult = GdcFt_Helper::GetLineColorFromSlopeRgb24(positionBack.x, positionBack.y, positionFront.x,
                                                                   positionFront.y,
                                                                   /* out */ colorRgb24);
              SHOW_ERROR(libResult == LIB_SUCCESS);

              libResult = GdcFt_Helper::DrawFeatureRgb24((uint8_t* const)lpSceneRgb24, cSceneWidthInPixels,
                                                         cSceneHeightInPixels, rData.mTrackedFeatures[i].mCurrent, 4,
                                                         colorRgb24);
              SHOW_ERROR(libResult == LIB_SUCCESS);

              libResult =
                  GdcFt_Helper::DrawLineRgb24(cSceneWidthInPixels, cSceneHeightInPixels, positionBack.x, positionBack.y,
                                              positionFront.x, positionFront.y, colorRgb24, lpSceneRgb24);
              SHOW_ERROR(libResult == LIB_SUCCESS);
            }
          }
          else
          {
            for(uint32_t i = 0; i < rData.mNumberOfTrackedFeatures; ++i)
            {
              GdcFt_TrackedFeatureIterator iterator;

              libResult = rData.mTrackedFeatures[i].Iterator(iterator);

              if(libResult == LIB_SUCCESS)
              {
                ICP_Point positionBack;
                ICP_Point positionFront;

                libResult = rData.mTrackedFeatures[i].GetBack(positionBack);
                SHOW_ERROR(libResult == LIB_SUCCESS);
                libResult = rData.mTrackedFeatures[i].GetFront(positionFront);
                SHOW_ERROR(libResult == LIB_SUCCESS);

                uint32_t colorRgb24 = 0;

                libResult = GdcFt_Helper::GetLineColorFromSlopeRgb24(positionBack.x, positionBack.y, positionFront.x,
                                                                     positionFront.y,
                                                                     /* out */ colorRgb24);
                SHOW_ERROR(libResult == LIB_SUCCESS);

                const ICP_Point* pPosition0 = 0;
                const ICP_Point* pPosition1 = 0;

                if(iterator.HasNext())
                {
                  pPosition0 = iterator.Next();

                  while(iterator.HasNext())
                  {
                    pPosition1 = iterator.Next();

                    libResult = GdcFt_Helper::DrawFeatureRgb24((uint8_t* const)lpSceneRgb24, cSceneWidthInPixels,
                                                               cSceneHeightInPixels, rData.mTrackedFeatures[i].mCurrent,
                                                               4, colorRgb24);
                    SHOW_ERROR(libResult == LIB_SUCCESS);

                    libResult = GdcFt_Helper::DrawLineRgb24(cSceneWidthInPixels, cSceneHeightInPixels, pPosition0->x,
                                                            pPosition0->y, pPosition1->x, pPosition1->y, colorRgb24,
                                                            lpSceneRgb24);
                    SHOW_ERROR(libResult == LIB_SUCCESS);
                    pPosition0 = pPosition1;
                  }
                }
              }
            }
          }
        }

        GdcFt_Helper::SetLogo(cSceneWidthInPixels, cSceneHeightInPixels,
                              cSceneWidthInPixels - NXP_LOGO_WIDTH - (2 * gcScenePaddingInBytes),
                              cSceneHeightInPixels - NXP_LOGO_HEIGHT - (2 * gcScenePaddingInBytes),
                              (uint8_t* const)lpSceneRgb24);

        // end of draw NXP logo on image
        const uint32_t cMovingWindowSize = 10; // set mFeatureTrackingTicks < 30
        const uint32_t pyramid_ticks     = 0;
        const uint32_t harris_ticks      = 1;
        const uint32_t KLT_ticks         = 2;
        const uint32_t total_ticks       = 30;
        const uint32_t average_ticks     = 31;

        // avarage performance
        mAvgPerformance[pyramid_ticks][mAvgIndex] =
            (rData.mTicks[gcPyramidTick + 2] - rData.mTicks[gcPyramidTick + 1]) / gcTimeDivisor;
        mAvgPerformance[harris_ticks][mAvgIndex] =
            (rData.mTicks[gcDetectorTick + 2] - rData.mTicks[gcDetectorTick + 1]) / gcTimeDivisor;
        mAvgPerformance[KLT_ticks][mAvgIndex] =
            (rData.mTicks[gcTrackerTick + 2] - rData.mTicks[gcTrackerTick + 1]) / gcTimeDivisor;

        mAvgPerformance[pyramid_ticks][total_ticks] = mAvgPerformance[pyramid_ticks][0];
        mAvgPerformance[harris_ticks][total_ticks]  = mAvgPerformance[harris_ticks][0];
        mAvgPerformance[KLT_ticks][total_ticks]     = mAvgPerformance[KLT_ticks][0];

        for(unsigned int i = 1; i < cMovingWindowSize; i++)
        {
          mAvgPerformance[pyramid_ticks][total_ticks] += mAvgPerformance[pyramid_ticks][i];
          mAvgPerformance[harris_ticks][total_ticks] += mAvgPerformance[harris_ticks][i];
          mAvgPerformance[KLT_ticks][total_ticks] += mAvgPerformance[KLT_ticks][i];
        }

        mAvgPerformance[pyramid_ticks][average_ticks] = mAvgPerformance[pyramid_ticks][total_ticks] / cMovingWindowSize;
        mAvgPerformance[harris_ticks][average_ticks]  = mAvgPerformance[harris_ticks][total_ticks] / cMovingWindowSize;
        mAvgPerformance[KLT_ticks][average_ticks]     = mAvgPerformance[KLT_ticks][total_ticks] / cMovingWindowSize;

        if(++mAvgIndex >= cMovingWindowSize)
          mAvgIndex = 0;

        // rendering text on image
        const uint32_t cScale            = 2;
        const uint32_t cTextColor        = 0xFFFFFF00;
        const uint32_t cLineSpace        = 0;
        char           pString[64]       = {0};
        uint32_t       xOffset_text      = 24;
        uint32_t       yOffset_text      = TEXT_HEIGHT_7x13 * 2 - 8;
        const int32_t  textOffsetY_delta = (TEXT_HEIGHT_7x13 * cScale + cLineSpace);

        // perfomance display
        pString[0] = 0;
        sprintf(pString, "Frame      [%d]", rData.mSceneCounter);
        GdcFt_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, xOffset_text, yOffset_text,
                                              (uint8_t*)pString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

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

        const double lFramesPerSecond = (1000.0 * 1000.0) / (double)(mMovingAverage_FramePeriod.Get() / gcTimeDivisor);

        yOffset_text += textOffsetY_delta;
        pString[0] = 0;
        sprintf(pString, "Frame rate [%d]", (int)lFramesPerSecond);
        GdcFt_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, xOffset_text, yOffset_text,
                                              (uint8_t*)pString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

        yOffset_text += textOffsetY_delta;
        //mpString[0] = 0;
        sprintf(pString, "Resolution [%dx%d]", cSceneWidthInPixels, cSceneHeightInPixels);
        GdcFt_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, xOffset_text, yOffset_text,
                                              (uint8_t*)pString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

        yOffset_text += textOffsetY_delta;
        //mpString[0] = 0;
        sprintf(pString, "Detected      [%d]", rData.mNumberOfDetectedFeatures);
        GdcFt_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, xOffset_text, yOffset_text,
                                              (uint8_t*)pString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

        yOffset_text += textOffsetY_delta;
        pString[0] = 0;
        sprintf(pString, "Tracked       [%d]", rData.mNumberOfTrackedFeatures);
        GdcFt_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, xOffset_text, yOffset_text,
                                              (uint8_t*)pString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

        yOffset_text += textOffsetY_delta;
        pString[0] = 0;
        sprintf(pString, "[%d] Pyramid: %5d us", gcApexCvPyramidMultiCreationApexId,
                (int)mAvgPerformance[pyramid_ticks][average_ticks]);
        GdcFt_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, xOffset_text, yOffset_text,
                                              (uint8_t*)pString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

        yOffset_text += textOffsetY_delta;
        pString[0] = 0;
        sprintf(pString, "[%d] Harris : %5d us", gcApexCvGfttCornersApexId,
                (int)mAvgPerformance[harris_ticks][average_ticks]);
        GdcFt_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, xOffset_text, yOffset_text,
                                              (uint8_t*)pString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

        yOffset_text += textOffsetY_delta;
        pString[0] = 0;
        sprintf(pString, "[%d] KLT    : %5d us", gcApexCvLKTrackerOpticalFlowApexId,
                (int)mAvgPerformance[KLT_ticks][average_ticks]);
        GdcFt_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, xOffset_text, yOffset_text,
                                              (uint8_t*)pString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

        yOffset_text += textOffsetY_delta;
        uint64_t totalApexUs[2] = {0, 0}; // 0 is Apex0 and 1 is Apex1

        SHOW_ERROR((gcApexCvGfttCornersApexId >= 0 && gcApexCvGfttCornersApexId < 2) &&
                   (gcApexCvPyramidMultiCreationApexId >= 0 && gcApexCvPyramidMultiCreationApexId < 2) &&
                   (gcApexCvLKTrackerOpticalFlowApexId >= 0 && gcApexCvLKTrackerOpticalFlowApexId < 2));

        totalApexUs[gcApexCvGfttCornersApexId] += mAvgPerformance[harris_ticks][31];
        totalApexUs[gcApexCvPyramidMultiCreationApexId] += mAvgPerformance[pyramid_ticks][31];
        totalApexUs[gcApexCvLKTrackerOpticalFlowApexId] += mAvgPerformance[KLT_ticks][31];

        pString[0] = 0;
        sprintf(pString, "    Total  : %5lu us", *(std::max_element(&(totalApexUs[0]), &(totalApexUs[2]))));
        GdcFt_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, xOffset_text, yOffset_text,
                                              (uint8_t*)pString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

        libResult = LIB_SUCCESS;
      }
    }
  }

  return libResult;
}

/**********************************************************************************************************/
// GdcFt_Helper function definitions
/**********************************************************************************************************/

void GdcFt_Helper::SetLogo(int32_t const cWidthInPixels,
                           int32_t const /*cHeightInPixels*/,
                           uint32_t const cHorizontalOffsetInPixels,
                           uint32_t const cVerticalOffsetInPixels,
                           uint8_t* const cpBuffer)
{
  const uint32_t cPixelSizeInBytes = 3;
  const uint32_t cLineSpanInBytes  = (cWidthInPixels * cPixelSizeInBytes);

  uint8_t* pLine      = (uint8_t*)(cpBuffer + ((cVerticalOffsetInPixels * cLineSpanInBytes) +
                                          cHorizontalOffsetInPixels * cPixelSizeInBytes));
  uint32_t lLogoPixel = 0;
  uint8_t* pPixel     = pLine;

  for(int i = 0; i < NXP_LOGO_HEIGHT; i++)
  {
    for(int j = 0; j < (NXP_LOGO_WIDTH); j++)
    {
      if((nxp_logo[lLogoPixel + 2] != 0xff) || (nxp_logo[lLogoPixel + 1] != 0xff) || (nxp_logo[lLogoPixel] != 0xff))
      {
        pPixel[0] = nxp_logo[lLogoPixel + 2];
        pPixel[1] = nxp_logo[lLogoPixel + 1];
        pPixel[2] = nxp_logo[lLogoPixel];
      }
      pPixel += cPixelSizeInBytes;
      lLogoPixel += cPixelSizeInBytes;
    }
    pLine += cLineSpanInBytes;
    pPixel = pLine;
  }
}

void GdcFt_Helper::RenderTextToRgb24Buffer(int32_t const cWidthInPixels,
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

  const uint8_t redColorInvert   = 0xFF - redColor;
  const uint8_t greenColorInvert = 0xFF - greenColor;
  const uint8_t blueColorInvert  = 0xFF - blueColor;

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
          else
          {
            pCurrentPixel[0] = redColorInvert;
            pCurrentPixel[1] = greenColorInvert;
            pCurrentPixel[2] = blueColorInvert;
          }
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

/*!*********************************************************************************
 *  @brief Function used to generate different colors based on the orientation
 */
LIB_RESULT GdcFt_Helper::GetLineColorFromSlopeRgb24(const int32_t cLineX1,
                                                    const int32_t cLineY1,
                                                    const int32_t cLineX2,
                                                    const int32_t cLineY2,
                                                    uint32_t&     rReturnedColorValueRgb24)
{
  LIB_RESULT libResult = LIB_SUCCESS;

  rReturnedColorValueRgb24 = 0xFFFFFF; // White

  const double cY = (cLineY2 - cLineY1);
  const double cX = (cLineX2 - cLineX1);

  if(!(cX == 0.0 && cY == 0.0))
  {
    const double   cRadians         = atan2(cY, cX);
    const int32_t  cDegrees         = (int32_t)(cRadians * 180.0 / M_PI);
    const uint32_t cDegreesPositive = ((cDegrees >= 0) ? cDegrees : 360 + cDegrees);

    const uint32_t cNumberOfColors    = 12;
    const int32_t  cDegreesPerSection = 360 / cNumberOfColors;

    const uint32_t cColorRgb24[cNumberOfColors] = {
        0xFF0000, // Red            [  0, 30[
        0xFF7D00, // Orange         [ 30, 60[
        0xFFFF00, // Yellow         [ 60, 90[
        0x7DFF00, // Spring green   [ 90,120[
        0x00FF00, // Green          [120,150[
        0x00FF7D, // Turquoise      [150,180[
        0x00FFFF, // Cyan           [180,210[
        0x007DFF, // Ocean          [210,240[
        0x0000FF, // Blue           [240,270[
        0x7D00FF, // Violet         [270,300[
        0xFF00FF, // Magenta        [300,330[
        0xFF007D, // Raspberry      [330,360[
    };
    uint32_t limitLow  = 0;
    uint32_t limitHigh = cDegreesPerSection;

    for(uint32_t i = 0; i < cNumberOfColors; ++i)
    {
      if(cDegreesPositive >= limitLow && // Inclusive
         cDegreesPositive < limitHigh)   // Non-inclusive
      {
        rReturnedColorValueRgb24 = cColorRgb24[i];
        break;
      }

      limitLow += cDegreesPerSection;
      limitHigh += cDegreesPerSection;

      SHOW_ERROR(limitHigh <= 360 && // 360 degrees
                 limitLow < limitHigh);
    }
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcFt_Helper::DrawLineRgb24(const int32_t  cImageWidthInPixels,
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
    const uint32_t cBitsPerPixel     = 24; // RGB24
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

    GdcFt_Helper::PutPixel(x0, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes, cBitsPerPixel,
                           (uint8_t*)cpImageBuffer, cColorValue);
    GdcFt_Helper::PutPixel(x1, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes, cBitsPerPixel,
                           (uint8_t*)cpImageBuffer, cColorValue);
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
            GdcFt_Helper::PutPixel(x0, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes, cBitsPerPixel,
                                   (uint8_t*)cpImageBuffer, cColorValue); //
            GdcFt_Helper::PutPixel(x0 += stepx, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                   cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //  x o o
            GdcFt_Helper::PutPixel(x1, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes, cBitsPerPixel,
                                   (uint8_t*)cpImageBuffer, cColorValue); //
            GdcFt_Helper::PutPixel(x1 -= stepx, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                   cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            d += incr1;
          }
          else
          {
            if(d < c)
            { // Pattern:
              GdcFt_Helper::PutPixel(x0, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //      o
              GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //  x o
              GdcFt_Helper::PutPixel(x1, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //
              GdcFt_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            }
            else
            {
              GdcFt_Helper::PutPixel(x0, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); // Pattern:
              GdcFt_Helper::PutPixel(x0 += stepx, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //    o o
              GdcFt_Helper::PutPixel(x1, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //  x
              GdcFt_Helper::PutPixel(x1 -= stepx, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //
            }
            d += incr2;
          }
        }
        if(extras > 0)
        {
          if(d < 0)
          {
            GdcFt_Helper::PutPixel(x0 += stepx, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                   cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 1)
              GdcFt_Helper::PutPixel(x0 += stepx, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 2)
              GdcFt_Helper::PutPixel(x1 -= stepx, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
          }
          else if(d < c)
          {
            GdcFt_Helper::PutPixel(x0 += stepx, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                   cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 1)
              GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 2)
              GdcFt_Helper::PutPixel(x1 -= stepx, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
          }
          else
          {
            GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                   cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 1)
              GdcFt_Helper::PutPixel(x0 += stepx, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 2)
              GdcFt_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
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
            GdcFt_Helper::PutPixel(x0, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                   cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); // Pattern:
            GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                   cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //      o
            GdcFt_Helper::PutPixel(x1, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                   cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //    o
            GdcFt_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels,
                                   cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //  x
            d += incr1;
          }
          else
          {
            if(d < c)
            {
              GdcFt_Helper::PutPixel(x0, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); // Pattern:
              GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //      o
              GdcFt_Helper::PutPixel(x1, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //  x o
              GdcFt_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //
            }
            else
            {
              GdcFt_Helper::PutPixel(x0, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); // Pattern:
              GdcFt_Helper::PutPixel(x0 += stepx, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //    o o
              GdcFt_Helper::PutPixel(x1, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //  x
              GdcFt_Helper::PutPixel(x1 -= stepx, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue); //
            }
            d += incr2;
          }
        }
        if(extras > 0)
        {
          if(d > 0)
          {
            GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                   cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 1)
              GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 2)
              GdcFt_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
          }
          else if(d < c)
          {
            GdcFt_Helper::PutPixel(x0 += stepx, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                   cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 1)
              GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 2)
              GdcFt_Helper::PutPixel(x1 -= stepx, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
          }
          else
          {
            GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                   cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 1)
              GdcFt_Helper::PutPixel(x0 += stepx, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 2)
            {
              if(d > c)
                GdcFt_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels,
                                       cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
              else
                GdcFt_Helper::PutPixel(x1 -= stepx, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                       cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
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
            GdcFt_Helper::PutPixel(x0, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes, cBitsPerPixel,
                                   (uint8_t*)cpImageBuffer, cColorValue);
            GdcFt_Helper::PutPixel(x0, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                   cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            GdcFt_Helper::PutPixel(x1, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes, cBitsPerPixel,
                                   (uint8_t*)cpImageBuffer, cColorValue);
            GdcFt_Helper::PutPixel(x1, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                   cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            d += incr1;
          }
          else
          {
            if(d < c)
            {
              GdcFt_Helper::PutPixel(x0, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
              GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
              GdcFt_Helper::PutPixel(x1, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
              GdcFt_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            }
            else
            {
              GdcFt_Helper::PutPixel(x0 += stepx, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
              GdcFt_Helper::PutPixel(x0, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
              GdcFt_Helper::PutPixel(x1 -= stepx, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
              GdcFt_Helper::PutPixel(x1, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            }
            d += incr2;
          }
        }
        if(extras > 0)
        {
          if(d < 0)
          {
            GdcFt_Helper::PutPixel(x0, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                   cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 1)
              GdcFt_Helper::PutPixel(x0, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 2)
              GdcFt_Helper::PutPixel(x1, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
          }
          else if(d < c)
          {
            GdcFt_Helper::PutPixel(stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                   cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 1)
              GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 2)
              GdcFt_Helper::PutPixel(x1, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
          }
          else
          {
            GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                   cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 1)
              GdcFt_Helper::PutPixel(x0, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 2)
              GdcFt_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
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
            GdcFt_Helper::PutPixel(x0 += stepx, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                   cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                   cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            GdcFt_Helper::PutPixel(x1 -= stepx, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                   cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            GdcFt_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels,
                                   cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            d += incr1;
          }
          else
          {
            if(d < c)
            {
              GdcFt_Helper::PutPixel(x0, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
              GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
              GdcFt_Helper::PutPixel(x1, y1, cImageSpanInBytes, cImageWidthInPixels, cImageHeightInPixels,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
              GdcFt_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            }
            else
            {
              GdcFt_Helper::PutPixel(x0 += stepx, y0, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
              GdcFt_Helper::PutPixel(x0, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
              GdcFt_Helper::PutPixel(x1 -= stepx, y1, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
              GdcFt_Helper::PutPixel(x1, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            }
            d += incr2;
          }
        }
        if(extras > 0)
        {
          if(d > 0)
          {
            GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                   cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 1)
              GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 2)
              GdcFt_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
          }
          else if(d < c)
          {
            GdcFt_Helper::PutPixel(x0, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                   cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 1)
              GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                     cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 2)
              GdcFt_Helper::PutPixel(x1, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
          }
          else
          {
            GdcFt_Helper::PutPixel(x0 += stepx, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels,
                                   cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 1)
              GdcFt_Helper::PutPixel(x0, y0 += stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                     cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
            if(extras > 2)
            {
              if(d > c)
                GdcFt_Helper::PutPixel(x1 -= stepx, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels,
                                       cImageSpanInBytes, cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
              else
                GdcFt_Helper::PutPixel(x1, y1 -= stepy, cImageWidthInPixels, cImageHeightInPixels, cImageSpanInBytes,
                                       cBitsPerPixel, (uint8_t*)cpImageBuffer, cColorValue);
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
void GdcFt_Helper::PutPixel(const uint32_t cX,
                            const uint32_t cY,
                            const uint32_t cImageWidthInPixels,
                            const uint32_t cImageHeightInPixels,
                            const uint32_t cSpanInBytes,
                            const uint32_t cBitsPerPixel,
                            uint8_t* const cpImageInUint8,
                            const uint32_t cColorValue)
{
  if(cX < cImageWidthInPixels && cY < cImageHeightInPixels)
  {
    const uint32_t cSpanInPixels = cSpanInBytes / (cBitsPerPixel / 8);

    switch(cBitsPerPixel)
    {
      case 8:
      {
        cpImageInUint8[cY * cSpanInPixels + cX] = cColorValue; // Use low 8 bits
        break;
      }
      case 16:
      {
        uint16_t* cpImageInUint16 = (uint16_t*)cpImageInUint8;

        cpImageInUint16[cY * cSpanInPixels + cX] = cColorValue; // Use low 16 bits
        break;
      }
      case 24:
      {
        const uint32_t cIndex0 = cY * cSpanInBytes + ((cX * cBitsPerPixel) / 8);

        cpImageInUint8[cIndex0]     = (cColorValue & 0x000000FF);
        cpImageInUint8[cIndex0 + 1] = (cColorValue & 0x0000FF00) >> 8;
        cpImageInUint8[cIndex0 + 2] = (cColorValue & 0x00FF0000) >> 16;
        break;
      }
      case 32:
      {
        uint32_t* cpImageInUint32 = (uint32_t*)cpImageInUint8;

        cpImageInUint32[cY * cSpanInPixels + cX] = cColorValue; // Use 32 bits
        break;
      }
      default:
      {
        break;
      }
    };
  }
}

/***************************************************************************/
/**
 * \par Description
 * Function used to draw a rectangle in an RGB24 image
 *
 **************************************************************************/
LIB_RESULT GdcFt_Helper::DrawFeatureRgb24(uint8_t* const   cpFrame,
                                          int32_t const    cFrameWidthInPixels,
                                          int32_t const    cFrameHeightInPixels,
                                          icp::Feature32S& rFeature,
                                          const uint32_t   cSizeInPixels,
                                          const uint32_t   cRgb24Color)
{
  LIB_RESULT libResult = LIB_FAILURE;

  const int32_t lRoi[4] = {rFeature.position.x - (cSizeInPixels / 2), rFeature.position.y - (cSizeInPixels / 2),
                           cSizeInPixels, cSizeInPixels};

  if(cpFrame != 0 && cFrameWidthInPixels > 0 && cFrameHeightInPixels > 0 && cSizeInPixels > 0 && lRoi[0] >= 0 &&
     lRoi[1] >= 0 && lRoi[2] > 0 && lRoi[3] > 0 && lRoi[0] + lRoi[2] < cFrameWidthInPixels &&
     lRoi[1] + lRoi[3] < cFrameHeightInPixels)
  {
    const uint32_t cPixelSizeInBytes = 3; // RGB24

    const uint8_t cRed   = cRgb24Color & 0xFF;
    const uint8_t cGreen = (cRgb24Color & 0xFF00) >> 8;
    const uint8_t cBlue  = (cRgb24Color & 0xFF0000) >> 16;

    const uint32_t cLineSpanInBytes = cFrameWidthInPixels * cPixelSizeInBytes;

    uint8_t* pLine  = cpFrame + (lRoi[0] * cPixelSizeInBytes) + (lRoi[1] * cLineSpanInBytes);
    uint8_t* pPixel = pLine;

    for(int32_t y = 0; y < lRoi[3]; ++y)
    {
      for(int32_t x = 0; x < lRoi[2]; ++x)
      {
        pPixel[0] = cRed;
        pPixel[1] = cGreen;
        pPixel[2] = cBlue;
        pPixel += cPixelSizeInBytes;
      }
      pLine += cLineSpanInBytes;
      pPixel = pLine;
    }

    libResult = LIB_SUCCESS;
  }
  else
  {
#if DO_DEBUG
    printf("cpFrame[%p], cFrameWidthInPixels[%d], cFrameHeightInPixels[%d], cSizeInPixels[%d], lRoi[%d,%d,%d,%d]\n",
           cpFrame, cFrameWidthInPixels, cFrameHeightInPixels, cSizeInPixels, lRoi[0], lRoi[1], lRoi[2], lRoi[3]);
#endif
    libResult = LIB_SUCCESS;
  }

  return libResult;
}
