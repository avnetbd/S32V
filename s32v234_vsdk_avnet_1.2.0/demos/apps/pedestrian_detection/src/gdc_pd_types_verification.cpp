/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2015 Freescale Semiconductor;
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
#include "gdc_pd_types_verification.hpp"
#include <limits>
#include <math.h>

/***************************************************************************/
/* Local Literals & definitions */

/**********************************************************************************************************/
// GdcPd_OutputFile function definitions
/**********************************************************************************************************/

GdcPd_OutputFile::GdcPd_OutputFile() : mpFileWrite(0), mpFileRead(0)
{
}

GdcPd_OutputFile::~GdcPd_OutputFile()
{
}

LIB_RESULT GdcPd_OutputFile::Open(const char* const cpcFilenameWrite, const char* const cpcFilenameRead)
{
  // Write file
  //--------------------------------------
  LIB_RESULT libResultWrite = LIB_FAILURE;

  if(cpcFilenameWrite != 0 && mpFileWrite == 0)
  {
    mpFileWrite = fopen(cpcFilenameWrite,
                        "wb"); // Hmm should I use binary in this case?

    if(mpFileWrite != 0)
    {
      libResultWrite = LIB_SUCCESS;
    }
  }
  else if(cpcFilenameWrite == 0)
  {
    libResultWrite = LIB_SUCCESS;
  }

  // Read file
  //--------------------------------------
  LIB_RESULT libResultRead = LIB_FAILURE;

  if(cpcFilenameRead != 0 && mpFileRead == 0)
  {
    mpFileRead = fopen(cpcFilenameRead,
                       "rb"); // Hmm should I use binary in this case?

    if(mpFileRead != 0)
    {
      libResultRead = LIB_SUCCESS;
    }
  }
  else if(cpcFilenameRead == 0)
  {
    libResultRead = LIB_SUCCESS;
  }

  return ((libResultWrite == LIB_SUCCESS) && (libResultRead == LIB_SUCCESS)) ? LIB_SUCCESS : LIB_FAILURE;
}

LIB_RESULT GdcPd_OutputFile::Close()
{
  // Write file
  //--------------------------------------
  int intResultWrite = 0;

  if(mpFileWrite != 0)
  {
    intResultWrite = fclose(mpFileWrite);
    mpFileWrite    = 0;
  }

  // Read file
  //--------------------------------------
  int intResultRead = 0;

  if(mpFileRead != 0)
  {
    intResultRead = fclose(mpFileRead);
    mpFileRead    = 0;
  }

  return ((intResultWrite == 0) && (intResultRead == 0)) ? LIB_SUCCESS : LIB_FAILURE;
}

/**********************************************************************************************************/
// GdcPd_Output_PedestrianDetection function definitions
/**********************************************************************************************************/

GdcPd_Output_Hog::GdcPd_Output_Hog()
    : mSceneCounter(0),
      //void*                  mpHogScoresOal[gcNumberOfSceneResizes];
      //icp::DataDescriptor    mHogScoresDd[gcNumberOfSceneResizes];
      mWeFreeBuffers(false)
{
  memset(mhHogScoresOal, 0, sizeof(mhHogScoresOal));
}

GdcPd_Output_Hog::~GdcPd_Output_Hog()
{
  if(mWeFreeBuffers == true)
  {
    for(uint32_t i = 0; i < gcNumberOfSceneResizes; ++i)
    {
      if(mhHogScoresOal[i] != 0)
      {
        OAL_MemoryFree(mhHogScoresOal[i]);
      }
      mhHogScoresOal[i] = 0;
      mHogScoresDd[i].UpdatePtrs(0, 0);
    }
  }
}

bool operator==(const GdcPd_Output_Hog& rcLeft, const GdcPd_Output_Hog& rcRight)
{
  bool isEqual = ((rcLeft.mSceneCounter == rcRight.mSceneCounter));

  if(isEqual == true)
  {
    for(uint32_t i = 0; i < gcNumberOfSceneResizes; ++i)
    {
      if(!(rcLeft.mHogScoresDd[i].GetWidth() == rcRight.mHogScoresDd[i].GetWidth() &&
           rcLeft.mHogScoresDd[i].GetHeight() == rcRight.mHogScoresDd[i].GetHeight() &&
           rcLeft.mHogScoresDd[i].GetElementDataType() == rcRight.mHogScoresDd[i].GetElementDataType() &&
           rcLeft.mHogScoresDd[i].GetElementDimX() == rcRight.mHogScoresDd[i].GetElementDimX() &&
           rcLeft.mHogScoresDd[i].GetElementDimY() == rcRight.mHogScoresDd[i].GetElementDimY() &&
           rcLeft.mHogScoresDd[i] == rcRight.mHogScoresDd[i]))
      {
        isEqual = false;
        break;
      }
    }
  }

  return isEqual;
}

LIB_RESULT GdcPd_OutputFile_Hog::Write(GdcPd_Output_Hog& rOutput)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mpFileWrite != 0)
  {
    size_t elementsWritten = 0;

    // Serializing the output for an image
    elementsWritten = fwrite(&(rOutput.mSceneCounter), sizeof(rOutput.mSceneCounter), 1, mpFileWrite);

    elementsWritten = fwrite(&(gcNumberOfSceneResizes), sizeof(gcNumberOfSceneResizes), 1, mpFileWrite);

    for(uint32_t i = 0; i < gcNumberOfSceneResizes; ++i)
    {
      elementsWritten = 0;

      const uint32_t      cWidth           = rOutput.mHogScoresDd[i].GetWidth();
      const uint32_t      cHeight          = rOutput.mHogScoresDd[i].GetHeight();
      const icp::DATATYPE cElementDataType = rOutput.mHogScoresDd[i].GetElementDataType();
      const uint32_t      cElementDimX     = rOutput.mHogScoresDd[i].GetElementDimX();
      const uint32_t      cElementDimY     = rOutput.mHogScoresDd[i].GetElementDimY();
      const uint32_t      cSpan            = rOutput.mHogScoresDd[i].GetSpan();
      void*               pDataPtr         = rOutput.mHogScoresDd[i].GetDataPtr();

      elementsWritten += fwrite(&(cWidth), sizeof(cWidth), 1, mpFileWrite);

      elementsWritten += fwrite(&(cHeight), sizeof(cHeight), 1, mpFileWrite);

      elementsWritten += fwrite(&(cElementDataType), sizeof(cElementDataType), 1, mpFileWrite);

      elementsWritten += fwrite(&(cElementDimX), sizeof(cElementDimX), 1, mpFileWrite);

      elementsWritten += fwrite(&(cElementDimY), sizeof(cElementDimY), 1, mpFileWrite);

      elementsWritten += fwrite(&(cSpan), sizeof(cSpan), 1, mpFileWrite);

      elementsWritten += fwrite(pDataPtr, cSpan * cHeight, 1, mpFileWrite);

      if(elementsWritten != 7) // 7 matches the number of fwrites.
      {
        libResult = LIB_FAILURE;
        break;
      }
    }
  }

  return libResult;
}

LIB_RESULT GdcPd_OutputFile_Hog::Read(GdcPd_Output_Hog& /*rOutput*/)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mpFileRead != 0)
  {
  }

  return libResult;
}

/**********************************************************************************************************/
// GdcPd_Output_PedestrianDetection function definitions
/**********************************************************************************************************/

GdcPd_Output_PedestrianDetection::GdcPd_Output_PedestrianDetection() : mWeFreeBuffers(false)
{
  memset(mhSceneResizesOal, 0, sizeof(mhSceneResizesOal));
}

GdcPd_Output_PedestrianDetection::~GdcPd_Output_PedestrianDetection()
{
  if(mWeFreeBuffers == true)
  {
    for(uint32_t i = 0; i < gcNumberOfSceneResizes; ++i)
    {
      if(mhSceneResizesOal[i] != 0)
      {
        OAL_MemoryFree(mhSceneResizesOal[i]);
      }
      mhSceneResizesOal[i] = 0;
      mSceneResizeDd[i].UpdatePtrs(0, 0);
    }
  }
}

bool operator==(const ROI& rcLeft, const ROI& rcRight)
{
  bool isEqual = ((rcLeft.x == rcRight.x) && (rcLeft.y == rcRight.y) && (rcLeft.width == rcRight.width) &&
                  (rcLeft.height == rcRight.height) && (rcLeft.score_low == rcRight.score_low) &&
                  (rcLeft.score_high == rcRight.score_high) &&
                  (fabs(rcLeft.h_scale - rcRight.h_scale) <= std::numeric_limits< double >::epsilon()) &&
                  (fabs(rcLeft.v_scale - rcRight.v_scale) <= std::numeric_limits< double >::epsilon()) &&
                  (rcLeft.s_width == rcRight.s_width) && (rcLeft.s_height == rcRight.s_height));

  return isEqual;
}

bool operator==(const GdcPd_Output_PedestrianDetection& rcLeft, const GdcPd_Output_PedestrianDetection& rcRight)
{
  bool isEqual = ((rcLeft.mImageNumber == rcRight.mImageNumber) && (rcLeft.mPedestrianRois == rcRight.mPedestrianRois));

  if(isEqual == true)
  {
    for(uint32_t i = 0; i < gcNumberOfSceneResizes; ++i)
    {
      if(!(rcLeft.mSceneResizeDd[i].GetWidth() == rcRight.mSceneResizeDd[i].GetWidth() &&
           rcLeft.mSceneResizeDd[i].GetHeight() == rcRight.mSceneResizeDd[i].GetHeight() &&
           rcLeft.mSceneResizeDd[i].GetElementDataType() == rcRight.mSceneResizeDd[i].GetElementDataType() &&
           rcLeft.mSceneResizeDd[i].GetElementDimX() == rcRight.mSceneResizeDd[i].GetElementDimX() &&
           rcLeft.mSceneResizeDd[i].GetElementDimY() == rcRight.mSceneResizeDd[i].GetElementDimY() &&
           rcLeft.mSceneResizeDd[i] == rcRight.mSceneResizeDd[i]))
      {
        isEqual = false;
        break;
      }
    }
  }

  return isEqual;
}

LIB_RESULT GdcPd_OutputFile_PedestrianDetection::Write(GdcPd_Output_PedestrianDetection& rOutput)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mpFileWrite != 0)
  {
    size_t elementsWritten = 0;

    // Serializing the output for an image
    elementsWritten = fwrite(&(rOutput.mImageNumber), sizeof(rOutput.mImageNumber), 1, mpFileWrite);

    if(elementsWritten == 1)
    {
      libResult = LIB_SUCCESS;

      const uint32_t cPedestrianRoisSize = rOutput.mPedestrianRois.size();

      elementsWritten = fwrite(&(cPedestrianRoisSize), sizeof(cPedestrianRoisSize), 1, mpFileWrite);

      for(std::vector< ROI >::size_type i = 0; i < cPedestrianRoisSize; ++i)
      {
        elementsWritten = 0;

        const int32_t  cX                = rOutput.mPedestrianRois[i].x;
        const int32_t  cY                = rOutput.mPedestrianRois[i].y;
        const int32_t  cWidth            = rOutput.mPedestrianRois[i].width;
        const int32_t  cHeight           = rOutput.mPedestrianRois[i].height;
        const int32_t  cScore_low        = rOutput.mPedestrianRois[i].score_low;
        const int32_t  cScore_high       = rOutput.mPedestrianRois[i].score_high;
        const double   cH_scale          = rOutput.mPedestrianRois[i].h_scale;
        const double   cV_scale          = rOutput.mPedestrianRois[i].v_scale;
        const int32_t  cS_width          = rOutput.mPedestrianRois[i].s_width;
        const int32_t  cS_height         = rOutput.mPedestrianRois[i].s_height;
        const uint32_t cDetectionCounter = rOutput.mPedestrianRois[i].mDetectionCounter;

        elementsWritten += fwrite(&(cX), sizeof(cX), 1, mpFileWrite);

        elementsWritten += fwrite(&(cY), sizeof(cY), 1, mpFileWrite);

        elementsWritten += fwrite(&(cWidth), sizeof(cWidth), 1, mpFileWrite);

        elementsWritten += fwrite(&(cHeight), sizeof(cHeight), 1, mpFileWrite);

        elementsWritten += fwrite(&(cScore_low), sizeof(cScore_low), 1, mpFileWrite);

        elementsWritten += fwrite(&(cScore_high), sizeof(cScore_high), 1, mpFileWrite);

        elementsWritten += fwrite(&(cH_scale), sizeof(cH_scale), 1, mpFileWrite);

        elementsWritten += fwrite(&(cV_scale), sizeof(cV_scale), 1, mpFileWrite);

        elementsWritten += fwrite(&(cS_width), sizeof(cS_width), 1, mpFileWrite);

        elementsWritten += fwrite(&(cS_height), sizeof(cS_height), 1, mpFileWrite);

        elementsWritten += fwrite(&(cDetectionCounter), sizeof(cDetectionCounter), 1, mpFileWrite);

        for(uint32_t j = 0; j < gcNumberOfSceneResizes; ++j)
        {
          elementsWritten += fwrite(&(rOutput.mPedestrianRois[i].mResolutionDetectionCounters[j]),
                                    sizeof(rOutput.mPedestrianRois[j].mResolutionDetectionCounters[j]), 1, mpFileWrite);
        }

        if(elementsWritten != (11 + gcNumberOfSceneResizes))
        {
          libResult = LIB_FAILURE;
          SHOW_ERROR(false);
          break;
        }
      }
    }
    else
    {
      SHOW_ERROR(false);
    }

    if(libResult == LIB_SUCCESS)
    {
      const char cRoiEnder[] = "ROI_END";

      elementsWritten = fwrite(&(cRoiEnder), sizeof(cRoiEnder), 1,
                               mpFileWrite); // Writing this to be able to check alignment on the read

      elementsWritten = fwrite(&(gcNumberOfSceneResizes), sizeof(gcNumberOfSceneResizes), 1,
                               mpFileWrite); // Writing this to be able to check value on the read

      for(uint32_t i = 0; i < gcNumberOfSceneResizes; ++i)
      {
        elementsWritten = 0;

        const uint32_t      cWidth           = rOutput.mSceneResizeDd[i].GetWidth();
        const uint32_t      cHeight          = rOutput.mSceneResizeDd[i].GetHeight();
        const icp::DATATYPE cElementDataType = rOutput.mSceneResizeDd[i].GetElementDataType();
        const uint32_t      cElementDimX     = rOutput.mSceneResizeDd[i].GetElementDimX();
        const uint32_t      cElementDimY     = rOutput.mSceneResizeDd[i].GetElementDimY();
        const uint32_t      cSpan            = rOutput.mSceneResizeDd[i].GetSpan();
        void*               pDataPtr         = rOutput.mSceneResizeDd[i].GetDataPtr();

        elementsWritten += fwrite(&(cWidth), sizeof(cWidth), 1, mpFileWrite);

        elementsWritten += fwrite(&(cHeight), sizeof(cHeight), 1, mpFileWrite);

        elementsWritten += fwrite(&(cElementDataType), sizeof(cElementDataType), 1, mpFileWrite);

        elementsWritten += fwrite(&(cElementDimX), sizeof(cElementDimX), 1, mpFileWrite);

        elementsWritten += fwrite(&(cElementDimY), sizeof(cElementDimY), 1, mpFileWrite);

        elementsWritten += fwrite(&(cSpan), sizeof(cSpan), 1, mpFileWrite);

        elementsWritten += fwrite(pDataPtr, cSpan * cHeight, 1, mpFileWrite);

        if(elementsWritten != 7) // 7 matches the number of fwrites.
        {
          libResult = LIB_FAILURE;
          SHOW_ERROR(false);
          break;
        }
      }
    }
  }

  return libResult;
}

LIB_RESULT GdcPd_OutputFile_PedestrianDetection::Read(GdcPd_Output_PedestrianDetection& rOutput)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mpFileRead != 0)
  {
    size_t elementsRead = 0;

    // Deserializing the input for an image
    elementsRead = fread(&(rOutput.mImageNumber), sizeof(rOutput.mImageNumber), 1, mpFileRead);

    if(elementsRead == 1)
    {
      libResult = LIB_SUCCESS;

      uint32_t lPedestrianRoisSize = 0;

      elementsRead = fread(&(lPedestrianRoisSize), sizeof(lPedestrianRoisSize), 1,
                           mpFileRead); // 4 bytes

      for(std::vector< ROI >::size_type i = 0; i < lPedestrianRoisSize; ++i)
      {
        ROI lRoi;

        elementsRead = 0;

        elementsRead += fread(&(lRoi.x), sizeof(lRoi.x), 1,
                              mpFileRead); // 4 byte

        elementsRead += fread(&(lRoi.y), sizeof(lRoi.y), 1,
                              mpFileRead); // 4 bytes

        elementsRead += fread(&(lRoi.width), sizeof(lRoi.width), 1,
                              mpFileRead); // 4 bytes

        elementsRead += fread(&(lRoi.height), sizeof(lRoi.height), 1,
                              mpFileRead); // 4 bytes

        elementsRead += fread(&(lRoi.score_low), sizeof(lRoi.score_low), 1,
                              mpFileRead); // 4 bytes

        elementsRead += fread(&(lRoi.score_high), sizeof(lRoi.score_high), 1,
                              mpFileRead); // 4 bytes

        elementsRead += fread(&(lRoi.h_scale), sizeof(lRoi.h_scale), 1,
                              mpFileRead); // 8 bytes

        elementsRead += fread(&(lRoi.v_scale), sizeof(lRoi.v_scale), 1,
                              mpFileRead); // 8 bytes

        elementsRead += fread(&(lRoi.s_width), sizeof(lRoi.s_width), 1,
                              mpFileRead); // 4 bytes

        elementsRead += fread(&(lRoi.s_height), sizeof(lRoi.s_height), 1,
                              mpFileRead); // 4 bytes

        elementsRead += fread(&(lRoi.mDetectionCounter), sizeof(lRoi.mDetectionCounter), 1,
                              mpFileRead); // 4 bytes

        for(uint32_t j = 0; j < gcNumberOfSceneResizes; ++j)
        {
          elementsRead += fread(&(lRoi.mResolutionDetectionCounters[j]),
                                sizeof(rOutput.mPedestrianRois[j].mResolutionDetectionCounters[j]), 1, mpFileRead);
        }

        if(elementsRead != (11 + gcNumberOfSceneResizes))
        {
          libResult = LIB_FAILURE;
          SHOW_ERROR(false);
          break;
        }
        else
        {
          rOutput.mPedestrianRois.push_back(lRoi);
        }
      }

      if(libResult == LIB_SUCCESS)
      {
        const char cRoiEnder[] = "ROI_END";
        char       lRoiEnder[] = "DNE_IOR";

        elementsRead = fread(&(lRoiEnder), sizeof(lRoiEnder), 1, mpFileRead);
        SHOW_ERROR(strcmp(cRoiEnder, lRoiEnder) == 0);

        uint32_t lNumberOfSceneResizes = 0;

        elementsRead = fread(&(lNumberOfSceneResizes), sizeof(lNumberOfSceneResizes), 1, mpFileRead);
        SHOW_ERROR(lNumberOfSceneResizes == gcNumberOfSceneResizes);

        bool isReadSuccess = false;

        for(uint32_t i = 0; i < lNumberOfSceneResizes; ++i)
        {
          isReadSuccess = false;

          elementsRead = 0;

          uint32_t      lWidth           = 0;
          uint32_t      lHeight          = 0;
          icp::DATATYPE lElementDataType = icp::DATATYPE_08U;
          uint32_t      lElementDimX     = 0;
          uint32_t      lElementDimY     = 0;
          uint32_t      lSpan            = 0;

          elementsRead += fread(&(lWidth), sizeof(lWidth), 1, mpFileRead);
          SHOW_ERROR(elementsRead == 1);

          elementsRead += fread(&(lHeight), sizeof(lHeight), 1, mpFileRead);
          SHOW_ERROR(elementsRead == 2);

          elementsRead += fread(&(lElementDataType), sizeof(lElementDataType), 1, mpFileRead);
          SHOW_ERROR(elementsRead == 3);

          elementsRead += fread(&(lElementDimX), sizeof(lElementDimX), 1, mpFileRead);
          SHOW_ERROR(elementsRead == 4);

          elementsRead += fread(&(lElementDimY), sizeof(lElementDimY), 1, mpFileRead);
          SHOW_ERROR(elementsRead == 5);

          elementsRead += fread(&(lSpan), sizeof(lSpan), 1, mpFileRead);
          SHOW_ERROR(elementsRead == 6);

          if(elementsRead == 6) // 6 matches the number of freads.
          {
            SHOW_ERROR(rOutput.mhSceneResizesOal[i] == 0);

            rOutput.mhSceneResizesOal[i] = OAL_MemoryAllocFlag(
                lSpan * lHeight, OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_MEMORY_FLAG_CONTIGUOUS);
            SHOW_ERROR(rOutput.mhSceneResizesOal[i] != 0);
            if(rOutput.mhSceneResizesOal[i] != 0)
            {
              void* lpSceneResizesOal = OAL_MemoryReturnAddress(rOutput.mhSceneResizesOal[i], ACCESS_CH_WB);

              elementsRead = 0;

              elementsRead = fread(lpSceneResizesOal, lSpan * lHeight, 1, mpFileRead);
              SHOW_ERROR(elementsRead == 1);
              if(elementsRead == 1)
              {
                rOutput.mSceneResizeDd[i].InitManual(lWidth, lHeight, lpSceneResizesOal,
                                                     OAL_MemoryReturnAddress(rOutput.mhSceneResizesOal[i], ACCESS_PHY),
                                                     lElementDataType, lElementDimX, lElementDimY);

                rOutput.mSceneResizeDd[i].SetSpan(lSpan);

                isReadSuccess = true;
              }
            }
          }

          if(isReadSuccess == false)
          {
            libResult = LIB_FAILURE;
            SHOW_ERROR(false);
            break;
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
 **************************************************************************/
void GdcPd_Helper::PrintMismatch_Hog(FILE* const             cpFile,
                                     const GdcPd_Output_Hog& rcLocalOutput,
                                     const GdcPd_Output_Hog& rcReferenceOutput)
{
  fprintf(cpFile, "GdcPd_HogTask: mismatch at frame %d\n", rcLocalOutput.mSceneCounter);

  if(rcLocalOutput.mSceneCounter != rcReferenceOutput.mSceneCounter)
  {
    fprintf(cpFile, "   mImageNumber %d != %d", rcLocalOutput.mSceneCounter, rcReferenceOutput.mSceneCounter);
  }

  for(uint32_t i = 0; i < gcNumberOfSceneResizes; ++i)
  {
    if(rcLocalOutput.mHogScoresDd[i].GetWidth() != rcReferenceOutput.mHogScoresDd[i].GetWidth())
    {
      fprintf(cpFile, "   mHogScoresDd[%d].GetWidth() %d != %d\n", (int)i, rcLocalOutput.mHogScoresDd[i].GetWidth(),
              rcReferenceOutput.mHogScoresDd[i].GetWidth());
    }
    if(rcLocalOutput.mHogScoresDd[i].GetHeight() != rcReferenceOutput.mHogScoresDd[i].GetHeight())
    {
      fprintf(cpFile, "   mHogScoresDd[%d].GetHeight() %d != %d\n", (int)i, rcLocalOutput.mHogScoresDd[i].GetHeight(),
              rcReferenceOutput.mHogScoresDd[i].GetHeight());
    }
    if(rcLocalOutput.mHogScoresDd[i].GetElementDataType() != rcReferenceOutput.mHogScoresDd[i].GetElementDataType())
    {
      fprintf(cpFile, "   mHogScoresDd[%d].GetElementDataType() %d != %d\n", (int)i,
              (int)(rcLocalOutput.mHogScoresDd[i].GetElementDataType()),
              (int)(rcReferenceOutput.mHogScoresDd[i].GetElementDataType()));
    }
    if(rcLocalOutput.mHogScoresDd[i].GetElementDimX() != rcReferenceOutput.mHogScoresDd[i].GetElementDimX())
    {
      fprintf(cpFile, "   mHogScoresDd[%d].GetElementDimX() %d != %d\n", (int)i,
              rcLocalOutput.mHogScoresDd[i].GetElementDimX(), rcReferenceOutput.mHogScoresDd[i].GetElementDimX());
    }
    if(rcLocalOutput.mHogScoresDd[i].GetElementDimY() != rcReferenceOutput.mHogScoresDd[i].GetElementDimY())
    {
      fprintf(cpFile, "   mHogScoresDd[%d].GetElementDimY() %d != %d\n", (int)i,
              rcLocalOutput.mHogScoresDd[i].GetElementDimY(), rcReferenceOutput.mHogScoresDd[i].GetElementDimY());
    }
    if(rcLocalOutput.mHogScoresDd[i].GetSpan() != rcReferenceOutput.mHogScoresDd[i].GetSpan())
    {
      fprintf(cpFile, "   mHogScoresDd[%d].GetSpan() %d != %d\n", (int)i, rcLocalOutput.mHogScoresDd[i].GetSpan(),
              rcReferenceOutput.mHogScoresDd[i].GetSpan());
    }

    if(rcLocalOutput.mHogScoresDd[i].GetSpan() == rcReferenceOutput.mHogScoresDd[i].GetSpan() &&
       rcLocalOutput.mHogScoresDd[i].GetHeight() == rcReferenceOutput.mHogScoresDd[i].GetHeight())
    {
      const uint32_t cWidth  = rcLocalOutput.mHogScoresDd[i].GetWidth();
      const uint32_t cHeight = rcLocalOutput.mHogScoresDd[i].GetHeight();
      const uint32_t cSpan   = rcLocalOutput.mHogScoresDd[i].GetSpan();

      const uint8_t* const cpcLocalBegin = (uint8_t*)(rcLocalOutput.mHogScoresDd[i].GetDataPtr());
      const uint8_t* const cpcLocalEnd   = (uint8_t*)(rcLocalOutput.mHogScoresDd[i].GetDataPtr()) + (cSpan * cHeight);
      const uint8_t* const cpcReferenceBegin = (uint8_t*)(rcReferenceOutput.mHogScoresDd[i].GetDataPtr());
      const uint8_t* const cpcReferenceEnd =
          (uint8_t*)(rcReferenceOutput.mHogScoresDd[i].GetDataPtr()) + (cSpan * cHeight);

      const std::pair< const uint8_t*, const uint8_t* > cPairResult =
          std::mismatch(cpcLocalBegin, cpcLocalEnd, cpcReferenceBegin);
      if(cPairResult.first != cpcLocalEnd || cPairResult.second != cpcReferenceEnd)
      {
        SHOW_ERROR(cPairResult.first != cpcLocalEnd && cPairResult.second != cpcReferenceEnd);

        const uintptr_t cBadPixel = ((uintptr_t)cPairResult.first - (uintptr_t)cpcLocalBegin);
        const uint32_t  cBadX     = (cBadPixel % cSpan);
        const uint32_t  cBadY     = (cBadPixel / cSpan);

        fprintf(cpFile, "   mHogScoresDd[%2d] (%3d,%3d,%3d): pixel (%3d,%3d): %X != %X\n", (int)i, cSpan, cWidth,
                cHeight, cBadX, cBadY, *(cPairResult.first), *(cPairResult.second));
      }
    }
  }
}

/***************************************************************************/
/**
 * \par Description
 **************************************************************************/
void GdcPd_Helper::PrintMismatch_PedestrianDetection(FILE* const                             cpFile,
                                                     const GdcPd_Output_PedestrianDetection& rcLocalOutput,
                                                     const GdcPd_Output_PedestrianDetection& rcReferenceOutput)
{
  bool weKeepGoing = true;

  fprintf(cpFile, "GdcPd_HogTask: mismatch at frame %d\n", rcLocalOutput.mImageNumber);

  if(rcLocalOutput.mImageNumber != rcReferenceOutput.mImageNumber)
  {
    fprintf(cpFile, "   mImageNumber %d != %d", rcLocalOutput.mImageNumber, rcReferenceOutput.mImageNumber);
  }

  for(uint32_t i = 0; i < gcNumberOfSceneResizes; ++i)
  {
    if(rcLocalOutput.mSceneResizeDd[i].GetWidth() != rcReferenceOutput.mSceneResizeDd[i].GetWidth())
    {
      fprintf(cpFile, "   mSceneResizeDd[%d].GetWidth() %d != %d\n", (int)i, rcLocalOutput.mSceneResizeDd[i].GetWidth(),
              rcReferenceOutput.mSceneResizeDd[i].GetWidth());
    }
    if(rcLocalOutput.mSceneResizeDd[i].GetHeight() != rcReferenceOutput.mSceneResizeDd[i].GetHeight())
    {
      fprintf(cpFile, "   mSceneResizeDd[%d].GetHeight() %d != %d\n", (int)i,
              rcLocalOutput.mSceneResizeDd[i].GetHeight(), rcReferenceOutput.mSceneResizeDd[i].GetHeight());
    }
    if(rcLocalOutput.mSceneResizeDd[i].GetElementDataType() != rcReferenceOutput.mSceneResizeDd[i].GetElementDataType())
    {
      fprintf(cpFile, "   mSceneResizeDd[%d].GetElementDataType() %d != %d\n", (int)i,
              (int)(rcLocalOutput.mSceneResizeDd[i].GetElementDataType()),
              (int)(rcReferenceOutput.mSceneResizeDd[i].GetElementDataType()));
    }
    if(rcLocalOutput.mSceneResizeDd[i].GetElementDimX() != rcReferenceOutput.mSceneResizeDd[i].GetElementDimX())
    {
      fprintf(cpFile, "   mSceneResizeDd[%d].GetElementDimX() %d != %d\n", (int)i,
              rcLocalOutput.mSceneResizeDd[i].GetElementDimX(), rcReferenceOutput.mSceneResizeDd[i].GetElementDimX());
    }
    if(rcLocalOutput.mSceneResizeDd[i].GetElementDimY() != rcReferenceOutput.mSceneResizeDd[i].GetElementDimY())
    {
      fprintf(cpFile, "   mSceneResizeDd[%d].GetElementDimY() %d != %d\n", (int)i,
              rcLocalOutput.mSceneResizeDd[i].GetElementDimY(), rcReferenceOutput.mSceneResizeDd[i].GetElementDimY());
    }
    /*if(rcLocalOutput.mSceneResizeDd[i].GetSpan() != rcReferenceOutput.mSceneResizeDd[i].GetSpan())
      {
         fprintf(cpFile, "   mSceneResizeDd[%d].GetSpan() %d != %d\n",
                 (int)i, rcLocalOutput.mSceneResizeDd[i].GetSpan(), rcReferenceOutput.mSceneResizeDd[i].GetSpan());
      }*/

    if(rcLocalOutput.mSceneResizeDd[i].GetSpan() == rcReferenceOutput.mSceneResizeDd[i].GetSpan() &&
       rcLocalOutput.mSceneResizeDd[i].GetHeight() == rcReferenceOutput.mSceneResizeDd[i].GetHeight())
    {
      const uint32_t cWidth  = rcLocalOutput.mSceneResizeDd[i].GetWidth();
      const uint32_t cHeight = rcLocalOutput.mSceneResizeDd[i].GetHeight();
      const uint32_t cSpan   = rcLocalOutput.mSceneResizeDd[i].GetSpan();

      const uint8_t* const cpcLocalBegin = (uint8_t*)(rcLocalOutput.mSceneResizeDd[i].GetDataPtr());
      const uint8_t* const cpcLocalEnd   = (uint8_t*)(rcLocalOutput.mSceneResizeDd[i].GetDataPtr()) + (cSpan * cHeight);
      const uint8_t* const cpcReferenceBegin = (uint8_t*)(rcReferenceOutput.mSceneResizeDd[i].GetDataPtr());
      const uint8_t* const cpcReferenceEnd =
          (uint8_t*)(rcReferenceOutput.mSceneResizeDd[i].GetDataPtr()) + (cSpan * cHeight);

      const std::pair< const uint8_t*, const uint8_t* > cPairResult =
          std::mismatch(cpcLocalBegin, cpcLocalEnd, cpcReferenceBegin);
      if(cPairResult.first != cpcLocalEnd || cPairResult.second != cpcReferenceEnd)
      {
        SHOW_ERROR(cPairResult.first != cpcLocalEnd && cPairResult.second != cpcReferenceEnd);

        const uintptr_t cBadPixel = ((uintptr_t)cPairResult.first - (uintptr_t)cpcLocalBegin);
        const uint32_t  cBadX     = (cBadPixel % cSpan);
        const uint32_t  cBadY     = (cBadPixel / cSpan);

        fprintf(cpFile, "   mSceneResizeDd[%2d] (%3d,%3d,%3d): pixel (%3d,%3d): 0x%X != 0x%X, (%p, %p)\n", (int)i,
                cSpan, cWidth, cHeight, cBadX, cBadY, *(cPairResult.first), *(cPairResult.second),
                &(*(cPairResult.first)), &(*(cPairResult.second)));

        weKeepGoing = false;
      }
    }
  }

  if(weKeepGoing == true)
  {
    const std::vector< ROI >::size_type cLocalSize     = rcLocalOutput.mPedestrianRois.size();
    const std::vector< ROI >::size_type cReferenceSize = rcReferenceOutput.mPedestrianRois.size();

    if(cLocalSize != cReferenceSize)
    {
      fprintf(cpFile, "   mPedestrianRois.size %d != %d\n", (int)cLocalSize, (int)cReferenceSize);
    }
    else
    {
      const std::vector< ROI >::size_type cSmallestSize = cLocalSize < cReferenceSize ? cLocalSize : cReferenceSize;

      for(std::vector< ROI >::size_type i = 0; i < cSmallestSize; ++i)
      {
        if(rcLocalOutput.mPedestrianRois[i].x != rcReferenceOutput.mPedestrianRois[i].x)
        {
          fprintf(cpFile, "   mPedestrianRois[%d].x %d != %d,\n", (int)i, rcLocalOutput.mPedestrianRois[i].x,
                  rcReferenceOutput.mPedestrianRois[i].x);
        }
        if(rcLocalOutput.mPedestrianRois[i].y != rcReferenceOutput.mPedestrianRois[i].y)
        {
          fprintf(cpFile, "   mPedestrianRois[%d].y %d != %d\n", (int)i, rcLocalOutput.mPedestrianRois[i].y,
                  rcReferenceOutput.mPedestrianRois[i].y);
        }
        if(rcLocalOutput.mPedestrianRois[i].width != rcReferenceOutput.mPedestrianRois[i].width)
        {
          fprintf(cpFile, "   mPedestrianRois[%d].width %d != %d\n", (int)i, rcLocalOutput.mPedestrianRois[i].width,
                  rcReferenceOutput.mPedestrianRois[i].width);
        }
        if(rcLocalOutput.mPedestrianRois[i].height != rcReferenceOutput.mPedestrianRois[i].height)
        {
          fprintf(cpFile, "   mPedestrianRois[%d].height %d != %d\n", (int)i, rcLocalOutput.mPedestrianRois[i].height,
                  rcReferenceOutput.mPedestrianRois[i].height);
        }
        if(rcLocalOutput.mPedestrianRois[i].score_low != rcReferenceOutput.mPedestrianRois[i].score_low)
        {
          fprintf(cpFile, "   mPedestrianRois[%d].score_low %d != %d\n", (int)i,
                  rcLocalOutput.mPedestrianRois[i].score_low, rcReferenceOutput.mPedestrianRois[i].score_low);
        }
        if(rcLocalOutput.mPedestrianRois[i].score_high != rcReferenceOutput.mPedestrianRois[i].score_high)
        {
          fprintf(cpFile, "   mPedestrianRois[%d].score_high %d != %d\n", (int)i,
                  rcLocalOutput.mPedestrianRois[i].score_high, rcReferenceOutput.mPedestrianRois[i].score_high);
        }
        if(fabs(rcLocalOutput.mPedestrianRois[i].h_scale - rcReferenceOutput.mPedestrianRois[i].h_scale) >
           std::numeric_limits< double >::epsilon())
        {
          fprintf(cpFile, "   mPedestrianRois[%d].h_scale %f != %f\n", (int)i, rcLocalOutput.mPedestrianRois[i].h_scale,
                  rcReferenceOutput.mPedestrianRois[i].h_scale);
        }
        if(fabs(rcLocalOutput.mPedestrianRois[i].v_scale - rcReferenceOutput.mPedestrianRois[i].v_scale) >
           std::numeric_limits< double >::epsilon())
        {
          fprintf(cpFile, "   mPedestrianRois[%d].v_scale %f != %f\n", (int)i, rcLocalOutput.mPedestrianRois[i].v_scale,
                  rcReferenceOutput.mPedestrianRois[i].v_scale);
        }
        if(rcLocalOutput.mPedestrianRois[i].s_width != rcReferenceOutput.mPedestrianRois[i].s_width)
        {
          fprintf(cpFile, "   mPedestrianRois[%d].s_width %d != %d\n", (int)i, rcLocalOutput.mPedestrianRois[i].s_width,
                  rcReferenceOutput.mPedestrianRois[i].s_width);
        }
        if(rcLocalOutput.mPedestrianRois[i].s_height != rcReferenceOutput.mPedestrianRois[i].s_height)
        {
          fprintf(cpFile, "   mPedestrianRois[%d].s_height %d != %d\n", (int)i,
                  rcLocalOutput.mPedestrianRois[i].s_height, rcReferenceOutput.mPedestrianRois[i].s_height);
        }
        if(rcLocalOutput.mPedestrianRois[i].mDetectionCounter != rcReferenceOutput.mPedestrianRois[i].mDetectionCounter)
        {
          fprintf(cpFile, "   mPedestrianRois[%d].mDetectionCounter %d != %d\n", (int)i,
                  rcLocalOutput.mPedestrianRois[i].mDetectionCounter,
                  rcReferenceOutput.mPedestrianRois[i].mDetectionCounter);
        }
      }
    }
  }
} /***************************************************************************/
/**
 * \par Description
 **************************************************************************/
void GdcPd_Helper::DumpDetectionImages(const uint32_t             cSceneId,
                                       const uint32_t             cScaleId,
                                       const uint32_t             cScoreX,
                                       const uint32_t             cScoreY,
                                       const icp::DataDescriptor& rcDd,
                                       const ROI&                 rcRoi)
{
  // cSceneId_cScaleId_Width_Height_cScoreX_cScoreY
  char filename[1024] = {0};

  snprintf(filename, 1024, "%04d_%02d_%03dx%03d_%03dx%03d.y", cSceneId, cScaleId, rcRoi.width, rcRoi.height, cScoreX,
           cScoreY);

  FILE* pFile = fopen(filename, "wb");

  if(pFile != 0)
  {
    const uint32_t cSpan   = rcDd.GetSpan();
    const uint8_t* pcPixel = (uint8_t*)((((uintptr_t)rcDd.GetDataPtr()) + rcRoi.x) + (cSpan * rcRoi.y));

    size_t elementsWritten = 0;

    for(int32_t i = 0; i < rcRoi.height; ++i)
    {
      elementsWritten = fwrite(pcPixel, rcRoi.width, 1, pFile);
      SHOW_ERROR(elementsWritten == 1);
      pcPixel += cSpan;
    }

    fclose(pFile);
    pFile = 0;
  }
}
