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
#include "gdc_ft_types_verification.hpp"
#include <algorithm>

/**********************************************************************************************************/
// GdcFt_OutputFile function definitions
/**********************************************************************************************************/

GdcFt_OutputFile::GdcFt_OutputFile() : mpFileWrite(0), mpFileRead(0)
{
}

GdcFt_OutputFile::~GdcFt_OutputFile()
{
}

LIB_RESULT GdcFt_OutputFile::Open(const char* const cpcFilenameWrite, const char* const cpcFilenameRead)
{
  // Write file
  //--------------------------------------
  LIB_RESULT libResultWrite = LIB_FAILURE;

  if(cpcFilenameWrite != 0 && mpFileWrite == 0)
  {
    mpFileWrite = fopen(cpcFilenameWrite,
                        "w"); // Hmm should I use binary in this case?

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
                       "r"); // Hmm should I use binary in this case?

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

LIB_RESULT GdcFt_OutputFile::Close()
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
// GdcFt_Output_HarrisCorners function definitions
/**********************************************************************************************************/

GdcFt_Output_HarrisCorners::GdcFt_Output_HarrisCorners()
    : mSceneCounter(0), mhFeatureDescriptorOal(0),
      //mFeatureDescriptor;
      mWeFreeBuffers(false)
{
}

GdcFt_Output_HarrisCorners::~GdcFt_Output_HarrisCorners()
{
  if(mWeFreeBuffers == true)
  {
    if(mhFeatureDescriptorOal != 0)
    {
      OAL_MemoryFree(mhFeatureDescriptorOal);
      mhFeatureDescriptorOal = 0;
    }
    mFeatureDescriptor.SetCount(0);
    mFeatureDescriptor.Init(0, 0, 0);
  }
}

bool operator==(const icp::Feature& rcLeft, const icp::Feature& rcRight)
{
  bool isEqual = false;

  if(rcLeft.position.x == rcRight.position.x && rcLeft.position.y == rcRight.position.y &&
     rcLeft.strength == rcRight.strength && rcLeft.reserve == rcRight.reserve)
  {
    isEqual = true;
  }

  return isEqual;
}

bool operator==(const GdcFt_Output_HarrisCorners& rcLeft, const GdcFt_Output_HarrisCorners& rcRight)
{
  bool isEqual = false;

  SHOW_ERROR(rcLeft.mFeatureDescriptor.GetCount() >= 0 && rcRight.mFeatureDescriptor.GetCount() >= 0);

  if(rcLeft.mSceneCounter == rcRight.mSceneCounter &&
     rcLeft.mFeatureDescriptor.GetCount() == rcRight.mFeatureDescriptor.GetCount())
  {
    const int32_t cCount = rcLeft.mFeatureDescriptor.GetCount();

    if(cCount >= 0 && rcLeft.mFeatureDescriptor.GetDataPtr() != 0 && rcRight.mFeatureDescriptor.GetDataPtr() != 0)
    {
      const int32_t cLeftCount  = rcLeft.mFeatureDescriptor.GetCount();
      const int32_t cRightCount = rcRight.mFeatureDescriptor.GetCount();

      icp::Feature* pLeftFeaturesSorted  = (icp::Feature*)malloc(cLeftCount * sizeof(pLeftFeaturesSorted[0]));
      icp::Feature* pRightFeaturesSorted = (icp::Feature*)malloc(cRightCount * sizeof(pRightFeaturesSorted[0]));

      if(pLeftFeaturesSorted != 0 && pRightFeaturesSorted != 0)
      {
        memcpy(pLeftFeaturesSorted, rcLeft.mFeatureDescriptor.GetDataPtr(),
               cLeftCount * sizeof(pLeftFeaturesSorted[0]));

        memcpy(pRightFeaturesSorted, rcRight.mFeatureDescriptor.GetDataPtr(),
               cRightCount * sizeof(pRightFeaturesSorted[0]));

        std::sort(pLeftFeaturesSorted, pLeftFeaturesSorted + cLeftCount, GdcFt_Helper::LessThan);

        std::sort(pRightFeaturesSorted, pRightFeaturesSorted + cRightCount, GdcFt_Helper::LessThan);

        std::vector< icp::Feature > setDifference;

        std::set_difference(pLeftFeaturesSorted, pLeftFeaturesSorted + cLeftCount, pRightFeaturesSorted,
                            pRightFeaturesSorted + cRightCount, std::inserter(setDifference, setDifference.begin()),
                            GdcFt_Helper::LessThan);

        if(setDifference.size() == 0)
        {
          setDifference.clear();

          std::set_difference(pRightFeaturesSorted, pRightFeaturesSorted + cRightCount, pLeftFeaturesSorted,
                              pLeftFeaturesSorted + cLeftCount, std::inserter(setDifference, setDifference.begin()),
                              GdcFt_Helper::LessThan);

          if(setDifference.size() == 0)
          {
            isEqual = true;
          }
        }
      }

      if(pLeftFeaturesSorted != 0)
      {
        free(pLeftFeaturesSorted);
        pLeftFeaturesSorted = 0;
      }
      if(pRightFeaturesSorted != 0)
      {
        free(pRightFeaturesSorted);
        pRightFeaturesSorted = 0;
      }
    }
  }

  return isEqual;
}

GdcFt_OutputFile_HarrisCorners::GdcFt_OutputFile_HarrisCorners()
{
}

GdcFt_OutputFile_HarrisCorners::~GdcFt_OutputFile_HarrisCorners()
{
}

LIB_RESULT GdcFt_OutputFile_HarrisCorners::Write(GdcFt_Output_HarrisCorners& rOutput)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mpFileWrite != 0)
  {
    size_t elementsWritten = 0;

    // Serializing the output for an image
    elementsWritten = fwrite(&(rOutput.mSceneCounter), sizeof(rOutput.mSceneCounter), 1, mpFileWrite);

    const int32_t cCount = rOutput.mFeatureDescriptor.GetCount();

    if(elementsWritten == 1)
    {
      elementsWritten = fwrite(&cCount, sizeof(cCount), 1, mpFileWrite);
    }

    if(elementsWritten == 1 && rOutput.mFeatureDescriptor.GetDataPtr() != 0)
    {
      libResult = LIB_SUCCESS;

      for(int32_t i = 0; i < rOutput.mFeatureDescriptor.GetCount(); ++i)
      {
        icp::Feature& rFeature = rOutput.mFeatureDescriptor[i];

        if((&rFeature) != 0)
        {
          elementsWritten = 0;

          elementsWritten += fwrite(&(rFeature.position.x), sizeof(rFeature.position.x), 1, mpFileWrite);

          elementsWritten += fwrite(&(rFeature.position.y), sizeof(rFeature.position.y), 1, mpFileWrite);

          elementsWritten += fwrite(&(rFeature.strength), sizeof(rFeature.strength), 1, mpFileWrite);

          elementsWritten += fwrite(&(rFeature.reserve), sizeof(rFeature.reserve), 1, mpFileWrite);

          if(elementsWritten != 4)
          {
            libResult = LIB_FAILURE;
            SHOW_ERROR(false);
            break;
          }
        }
        else
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

LIB_RESULT GdcFt_OutputFile_HarrisCorners::Read(GdcFt_Output_HarrisCorners& rOutput)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mpFileRead != 0)
  {
    size_t elementsRead = 0;

    // Serializing the output for an image
    elementsRead = fread(&(rOutput.mSceneCounter), sizeof(rOutput.mSceneCounter), 1, mpFileRead);

    int32_t lCount = 0;

    if(elementsRead == 1)
    {
      elementsRead = fread(&lCount, sizeof(lCount), 1, mpFileRead);
    }

    if(elementsRead == 1 && lCount >= 0 && rOutput.mFeatureDescriptor.GetDataPtr() == 0)
    {
      if(lCount != 0)
      {
        void* hOal = OAL_MemoryAllocFlag(lCount * sizeof(icp::Feature),
                                         OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_MEMORY_FLAG_CONTIGUOUS);
        SHOW_ERROR(hOal != 0);

        if(hOal != 0)
        {
          rOutput.mWeFreeBuffers = true;

          libResult = LIB_SUCCESS;

          rOutput.mFeatureDescriptor.Init(OAL_MemoryReturnAddress(hOal, ACCESS_CH_WB),
                                          OAL_MemoryReturnAddress(hOal, ACCESS_PHY), lCount);

          rOutput.mFeatureDescriptor.SetCount(lCount);

          for(int32_t i = 0; i < rOutput.mFeatureDescriptor.GetCount(); ++i)
          {
            icp::Feature& rFeature = rOutput.mFeatureDescriptor[i];

            if((&rFeature) != 0)
            {
              elementsRead = 0;

              elementsRead += fread(&(rFeature.position.x), sizeof(rFeature.position.x), 1, mpFileRead);

              elementsRead += fread(&(rFeature.position.y), sizeof(rFeature.position.y), 1, mpFileRead);

              elementsRead += fread(&(rFeature.strength), sizeof(rFeature.strength), 1, mpFileRead);

              elementsRead += fread(&(rFeature.reserve), sizeof(rFeature.reserve), 1, mpFileRead);

              if(elementsRead != 4)
              {
                libResult = LIB_FAILURE;
                SHOW_ERROR(false);
                break;
              }
            }
            else
            {
              libResult = LIB_FAILURE;
              SHOW_ERROR(false);
              break;
            }
          }
        }
      }
      else
      {
        libResult = LIB_SUCCESS;
      }
    }
  }

  return libResult;
}

/**********************************************************************************************************/
// GdcFt_OutputFile_PyramidMultiCreation function definitions
/**********************************************************************************************************/

GdcFt_Output_PyramidMultiCreation::GdcFt_Output_PyramidMultiCreation()
    : mSceneCounter(0),
      //void*               mhImagePyramidOal[gcDepthOfImagePyramid];
      //icp::DataDescriptor mImagePyramidDd[gcDepthOfImagePyramid];
      mWeFreeBuffers(false)
{
  memset(mhImagePyramidOal, 0, sizeof(mhImagePyramidOal));
}

GdcFt_Output_PyramidMultiCreation::~GdcFt_Output_PyramidMultiCreation()
{
  if(mWeFreeBuffers == true)
  {
    const uint32_t cNumberOfDd = sizeof(mImagePyramidDd) / sizeof(mImagePyramidDd[0]);

    for(uint32_t i = 0; i < cNumberOfDd; ++i)
    {
      if(mhImagePyramidOal[i] != 0)
      {
        OAL_MemoryFree(mhImagePyramidOal[i]);
      }
      mhImagePyramidOal[i] = 0;
      mImagePyramidDd[i].UpdatePtrs(0, 0);
    }
  }
}

bool operator==(const GdcFt_Output_PyramidMultiCreation& rcLeft, const GdcFt_Output_PyramidMultiCreation& rcRight)
{
  bool isEqual = ((rcLeft.mSceneCounter == rcRight.mSceneCounter));

  if(isEqual == true)
  {
    const uint32_t cNumberOfDd = sizeof(rcLeft.mImagePyramidDd) / sizeof(rcLeft.mImagePyramidDd[0]);

    for(uint32_t i = 0; i < cNumberOfDd; ++i)
    {
      if(!(rcLeft.mImagePyramidDd[i].GetWidth() == rcRight.mImagePyramidDd[i].GetWidth() &&
           rcLeft.mImagePyramidDd[i].GetHeight() == rcRight.mImagePyramidDd[i].GetHeight() &&
           rcLeft.mImagePyramidDd[i].GetElementDataType() == rcRight.mImagePyramidDd[i].GetElementDataType() &&
           rcLeft.mImagePyramidDd[i].GetElementDimX() == rcRight.mImagePyramidDd[i].GetElementDimX() &&
           rcLeft.mImagePyramidDd[i].GetElementDimY() == rcRight.mImagePyramidDd[i].GetElementDimY() &&
           rcLeft.mImagePyramidDd[i] == rcRight.mImagePyramidDd[i]))
      {
        isEqual = false;
        break;
      }
    }
  }

  return isEqual;
}

GdcFt_OutputFile_PyramidMultiCreation::GdcFt_OutputFile_PyramidMultiCreation()
{
}

GdcFt_OutputFile_PyramidMultiCreation::~GdcFt_OutputFile_PyramidMultiCreation()
{
}

LIB_RESULT GdcFt_OutputFile_PyramidMultiCreation::Write(GdcFt_Output_PyramidMultiCreation& rOutput)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mpFileWrite != 0)
  {
    size_t elementsWritten = 0;

    // Serializing the output for an image
    elementsWritten = fwrite(&(rOutput.mSceneCounter), sizeof(rOutput.mSceneCounter), 1, mpFileWrite);

    if(elementsWritten == 1)
    {
      libResult = LIB_SUCCESS;

      const uint32_t cNumberOfDd = sizeof(rOutput.mImagePyramidDd) / sizeof(rOutput.mImagePyramidDd[0]);

      for(uint32_t i = 0; i < cNumberOfDd; ++i)
      {
        elementsWritten = 0;

        const uint32_t      cWidth           = rOutput.mImagePyramidDd[i].GetWidth();
        const uint32_t      cHeight          = rOutput.mImagePyramidDd[i].GetHeight();
        const icp::DATATYPE cElementDataType = rOutput.mImagePyramidDd[i].GetElementDataType();
        const uint32_t      cElementDimX     = rOutput.mImagePyramidDd[i].GetElementDimX();
        const uint32_t      cElementDimY     = rOutput.mImagePyramidDd[i].GetElementDimY();
        const uint32_t      cSpan            = rOutput.mImagePyramidDd[i].GetSpan();
        void*               pDataPtr         = rOutput.mImagePyramidDd[i].GetDataPtr();

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
  }

  return libResult;
}

LIB_RESULT GdcFt_OutputFile_PyramidMultiCreation::Read(GdcFt_Output_PyramidMultiCreation& rOutput)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mpFileRead != 0)
  {
    size_t elementsRead = 0;

    // Serializing the output for an image
    elementsRead = fread(&(rOutput.mSceneCounter), sizeof(rOutput.mSceneCounter), 1, mpFileRead);
    if(elementsRead == 1)
    {
      libResult = LIB_SUCCESS;

      bool isReadSuccess = false;

      const uint32_t cNumberOfDd = sizeof(rOutput.mImagePyramidDd) / sizeof(rOutput.mImagePyramidDd[0]);

      for(uint32_t i = 0; i < cNumberOfDd; ++i)
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

        elementsRead += fread(&(lHeight), sizeof(lHeight), 1, mpFileRead);

        elementsRead += fread(&(lElementDataType), sizeof(lElementDataType), 1, mpFileRead);

        elementsRead += fread(&(lElementDimX), sizeof(lElementDimX), 1, mpFileRead);

        elementsRead += fread(&(lElementDimY), sizeof(lElementDimY), 1, mpFileRead);

        elementsRead += fread(&(lSpan), sizeof(lSpan), 1, mpFileRead);

        if(elementsRead == 6) // 6 matches the number of freads.
        {
          SHOW_ERROR(rOutput.mhImagePyramidOal[i] == 0);

          rOutput.mhImagePyramidOal[i] = OAL_MemoryAllocFlag(lSpan * lHeight, OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) |
                                                                                  OAL_MEMORY_FLAG_CONTIGUOUS);
          SHOW_ERROR(rOutput.mhImagePyramidOal[i] != 0);
          if(rOutput.mhImagePyramidOal[i] != 0)
          {
            elementsRead = 0;

            void* lpImagePyramidOal = OAL_MemoryReturnAddress(rOutput.mhImagePyramidOal[i], ACCESS_CH_WB);

            elementsRead = fread(lpImagePyramidOal, lSpan * lHeight, 1, mpFileRead);

            if(elementsRead == 1)
            {
              rOutput.mImagePyramidDd[i].InitManual(lWidth, lHeight, lpImagePyramidOal,
                                                    OAL_MemoryReturnAddress(rOutput.mhImagePyramidOal[i], ACCESS_PHY),
                                                    lElementDataType, lElementDimX, lElementDimY);

              rOutput.mImagePyramidDd[i].SetSpan(lSpan);

              isReadSuccess = true;
            }
          }
        }

        if(isReadSuccess == false)
        {
          libResult = LIB_FAILURE;
          break;
        }
      }
    }
  }

  return libResult;
}

/**********************************************************************************************************/
// GdcFt_Output_LKTrackerOpticalFlow function definitions
/**********************************************************************************************************/

GdcFt_Output_LKTrackerOpticalFlow::GdcFt_Output_LKTrackerOpticalFlow()
    : mSceneCounter(0), mhFeature32SDescriptorOal(0),
      //Feature32SDescriptor   mFeatureDescriptor;
      mWeFreeBuffers(false)
{
}

GdcFt_Output_LKTrackerOpticalFlow::~GdcFt_Output_LKTrackerOpticalFlow()
{
  if(mWeFreeBuffers == true)
  {
    if(mhFeature32SDescriptorOal != 0)
    {
      OAL_MemoryFree(mhFeature32SDescriptorOal);
      mhFeature32SDescriptorOal = 0;

      mFeature32SDescriptor.SetCount(0);
      mFeature32SDescriptor.Init(0, 0, 0);
    }
  }
}

bool operator==(const icp::Feature32S& rcLeft, const icp::Feature32S& rcRight)
{
  bool isEqual = false;

  if(rcLeft.position.x == rcRight.position.x && rcLeft.position.y == rcRight.position.y &&
     rcLeft.strength == rcRight.strength && rcLeft.reserve == rcRight.reserve)
  {
    isEqual = true;
  }

  return isEqual;
}

bool operator==(const GdcFt_Output_LKTrackerOpticalFlow& rcLeft, const GdcFt_Output_LKTrackerOpticalFlow& rcRight)
{
  bool isEqual = false;

  SHOW_ERROR(rcLeft.mFeature32SDescriptor.GetCount() >= 0 && rcRight.mFeature32SDescriptor.GetCount() >= 0);

  if(rcLeft.mSceneCounter == rcRight.mSceneCounter &&
     rcLeft.mFeature32SDescriptor.GetCount() == rcRight.mFeature32SDescriptor.GetCount())
  {
    const int32_t cCount = rcLeft.mFeature32SDescriptor.GetCount();

    if(cCount >= 0 && rcLeft.mFeature32SDescriptor.GetDataPtr() != 0 && rcRight.mFeature32SDescriptor.GetDataPtr() != 0)
    {
      const int32_t cLeftCount  = rcLeft.mFeature32SDescriptor.GetCount();
      const int32_t cRightCount = rcRight.mFeature32SDescriptor.GetCount();

      icp::Feature32S* pLeftFeaturesSorted  = (icp::Feature32S*)malloc(cLeftCount * sizeof(pLeftFeaturesSorted[0]));
      icp::Feature32S* pRightFeaturesSorted = (icp::Feature32S*)malloc(cRightCount * sizeof(pRightFeaturesSorted[0]));

      if(pLeftFeaturesSorted != 0 && pRightFeaturesSorted != 0)
      {
        memcpy(pLeftFeaturesSorted, rcLeft.mFeature32SDescriptor.GetDataPtr(),
               cLeftCount * sizeof(pLeftFeaturesSorted[0]));

        memcpy(pRightFeaturesSorted, rcRight.mFeature32SDescriptor.GetDataPtr(),
               cRightCount * sizeof(pRightFeaturesSorted[0]));

        std::sort(pLeftFeaturesSorted, pLeftFeaturesSorted + cLeftCount, GdcFt_Helper::LessThan32S);

        std::sort(pRightFeaturesSorted, pRightFeaturesSorted + cRightCount, GdcFt_Helper::LessThan32S);

        std::vector< icp::Feature32S > setDifference;

        std::set_difference(pLeftFeaturesSorted, pLeftFeaturesSorted + cLeftCount, pRightFeaturesSorted,
                            pRightFeaturesSorted + cRightCount, std::inserter(setDifference, setDifference.begin()),
                            GdcFt_Helper::LessThan32S);

        if(setDifference.size() == 0)
        {
          setDifference.clear();

          std::set_difference(pRightFeaturesSorted, pRightFeaturesSorted + cRightCount, pLeftFeaturesSorted,
                              pLeftFeaturesSorted + cLeftCount, std::inserter(setDifference, setDifference.begin()),
                              GdcFt_Helper::LessThan32S);

          if(setDifference.size() == 0)
          {
            isEqual = true;
          }
        }
      }

      if(pLeftFeaturesSorted != 0)
      {
        free(pLeftFeaturesSorted);
        pLeftFeaturesSorted = 0;
      }
      if(pRightFeaturesSorted != 0)
      {
        free(pRightFeaturesSorted);
        pRightFeaturesSorted = 0;
      }
    }
  }

  return isEqual;
}

/**********************************************************************************************************/
// GdcFt_OutputFile_LKTrackerOpticalFlow function definitions
/**********************************************************************************************************/

GdcFt_OutputFile_LKTrackerOpticalFlow::GdcFt_OutputFile_LKTrackerOpticalFlow()
{
}

GdcFt_OutputFile_LKTrackerOpticalFlow::~GdcFt_OutputFile_LKTrackerOpticalFlow()
{
}

LIB_RESULT GdcFt_OutputFile_LKTrackerOpticalFlow::Write(GdcFt_Output_LKTrackerOpticalFlow& rOutput)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mpFileWrite != 0)
  {
    size_t elementsWritten = 0;

    // Serializing the output for an image
    elementsWritten += fwrite(&(rOutput.mSceneCounter), sizeof(rOutput.mSceneCounter), 1, mpFileWrite);

    const int32_t cSize  = rOutput.mFeature32SDescriptor.GetSize();  // Max number of elements
    const int32_t cCount = rOutput.mFeature32SDescriptor.GetCount(); // Current number of elements

    elementsWritten += fwrite(&cSize, sizeof(cSize), 1, mpFileWrite);

    elementsWritten += fwrite(&cCount, sizeof(cCount), 1, mpFileWrite);

    if(elementsWritten == 3 && rOutput.mFeature32SDescriptor.GetDataPtr() != 0 && cSize >= cCount &&
       rOutput.mFeature32SDescriptor.GetSpan() == sizeof(icp::Feature32S))
    {
      libResult = LIB_SUCCESS;

      const icp::Feature32S* const cpcFeatureBegin = (icp::Feature32S*)rOutput.mFeature32SDescriptor.GetDataPtr();
      const icp::Feature32S* const cpcFeatureEnd   = cpcFeatureBegin + cSize;

      for(int32_t i = 0; i < cCount; ++i)
      {
        icp::Feature32S& rCurrentFeature = rOutput.mFeature32SDescriptor[i];

        if((&rCurrentFeature) >= cpcFeatureBegin && (&rCurrentFeature) < cpcFeatureEnd)
        {
          elementsWritten = 0;

          elementsWritten += fwrite(&(rCurrentFeature.position.x), sizeof(rCurrentFeature.position.x), 1, mpFileWrite);

          elementsWritten += fwrite(&(rCurrentFeature.position.y), sizeof(rCurrentFeature.position.y), 1, mpFileWrite);

          elementsWritten += fwrite(&(rCurrentFeature.strength), sizeof(rCurrentFeature.strength), 1, mpFileWrite);

          elementsWritten += fwrite(&(rCurrentFeature.reserve), sizeof(rCurrentFeature.reserve), 1, mpFileWrite);

          if(elementsWritten != 4)
          {
            libResult = LIB_FAILURE;
            SHOW_ERROR(false);
            break;
          }
        }
        else
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

LIB_RESULT GdcFt_OutputFile_LKTrackerOpticalFlow::Read(GdcFt_Output_LKTrackerOpticalFlow& rOutput)
{
  LIB_RESULT libResult = LIB_FAILURE;

  SHOW_ERROR(rOutput.mFeature32SDescriptor.GetDataPtr() == 0);

  if(mpFileRead != 0)
  {
    size_t elementsRead = 0;

    // Deserializing the input for an image
    elementsRead += fread(&(rOutput.mSceneCounter), sizeof(rOutput.mSceneCounter), 1, mpFileRead);

    int32_t lSize  = 0; // Max number of elements
    int32_t lCount = 0; // Current number of elements

    elementsRead += fread(&lSize, sizeof(lSize), 1, mpFileRead);

    elementsRead += fread(&lCount, sizeof(lCount), 1, mpFileRead);

    if(elementsRead == 3 && lSize >= lCount && rOutput.mFeature32SDescriptor.GetSpan() == sizeof(icp::Feature32S))
    {
      rOutput.mhFeature32SDescriptorOal =
          OAL_MemoryAllocFlag(lSize * sizeof(icp::Feature32S), OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) |
                                                                   OAL_MEMORY_FLAG_CONTIGUOUS | OAL_MEMORY_FLAG_ZERO);
      SHOW_ERROR(rOutput.mhFeature32SDescriptorOal != 0);
      if(rOutput.mhFeature32SDescriptorOal != 0)
      {
        libResult = LIB_SUCCESS;

        rOutput.mFeature32SDescriptor.Init(OAL_MemoryReturnAddress(rOutput.mhFeature32SDescriptorOal, ACCESS_CH_WB),
                                           OAL_MemoryReturnAddress(rOutput.mhFeature32SDescriptorOal, ACCESS_PHY),
                                           lSize);

        rOutput.mFeature32SDescriptor.SetCount(lCount);

        const icp::Feature32S* const cpcFeatureBegin = (icp::Feature32S*)rOutput.mFeature32SDescriptor.GetDataPtr();
        const icp::Feature32S* const cpcFeatureEnd   = cpcFeatureBegin + lSize;

        for(int32_t i = 0; i < lCount; ++i)
        {
          icp::Feature32S& rCurrentFeature = rOutput.mFeature32SDescriptor[i];

          if((&rCurrentFeature) >= cpcFeatureBegin && (&rCurrentFeature) < cpcFeatureEnd)
          {
            elementsRead = 0;

            elementsRead += fread(&(rCurrentFeature.position.x), sizeof(rCurrentFeature.position.x), 1, mpFileRead);

            elementsRead += fread(&(rCurrentFeature.position.y), sizeof(rCurrentFeature.position.y), 1, mpFileRead);

            elementsRead += fread(&(rCurrentFeature.strength), sizeof(rCurrentFeature.strength), 1, mpFileRead);

            elementsRead += fread(&(rCurrentFeature.reserve), sizeof(rCurrentFeature.reserve), 1, mpFileRead);

            if(elementsRead != 4)
            {
              libResult = LIB_FAILURE;
              SHOW_ERROR(false);
              break;
            }
          }
          else
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

/**********************************************************************************************************/
// GdcFt_Output_FeatureTracking function definitions
/**********************************************************************************************************/

GdcFt_Output_FeatureTracking::GdcFt_Output_FeatureTracking()
    : mSceneCounter(0), mNumberOfDetectedFeatures(0), mppTrackedFeatures(0), mWeFreeBuffers(false)
{
}

GdcFt_Output_FeatureTracking::~GdcFt_Output_FeatureTracking()
{
  if(mWeFreeBuffers == true)
  {
    if(mppTrackedFeatures != 0)
    {
      for(uint32_t i = 0; i < mNumberOfDetectedFeatures; ++i)
      {
        SHOW_ERROR(mppTrackedFeatures[i] != 0);

        if(mppTrackedFeatures[i] != 0)
        {
          free(mppTrackedFeatures[i]);
          mppTrackedFeatures[i] = 0;
        }
      }

      free(mppTrackedFeatures);
    }
    mppTrackedFeatures = 0;
  }
}

bool operator==(const GdcFt_TrackedFeature& rcLeft, const GdcFt_TrackedFeature& rcRight)
{
  bool isEqual =
      (rcLeft.mCurrent.position.x == rcRight.mCurrent.position.x &&
       rcLeft.mCurrent.position.y == rcRight.mCurrent.position.y &&
       rcLeft.mCurrent.strength == rcRight.mCurrent.strength && rcLeft.mCurrent.reserve == rcRight.mCurrent.reserve &&
       rcLeft.mAgeInFramesSinceDetection == rcRight.mAgeInFramesSinceDetection &&
       rcLeft.mPositionsIndex_Current == rcRight.mPositionsIndex_Current &&
       rcLeft.mPositionsSize == rcRight.mPositionsSize);

  if(isEqual == true)
  {
    LIB_RESULT libResult_left  = LIB_FAILURE;
    LIB_RESULT libResult_right = LIB_FAILURE;

    GdcFt_TrackedFeatureIterator iterator_left;
    GdcFt_TrackedFeatureIterator iterator_right;

    libResult_left  = rcLeft.Iterator(iterator_left);
    libResult_right = rcRight.Iterator(iterator_right);

    isEqual = false;

    if(libResult_left == LIB_SUCCESS && libResult_right == LIB_SUCCESS)
    {
      isEqual = true;

      while(iterator_left.HasNext() == true && iterator_right.HasNext() == true)
      {
        const ICP_Point& rcPointLeft  = *(iterator_left.Next());
        const ICP_Point& rcPointRight = *(iterator_right.Next());

        if(rcPointLeft.x != rcPointRight.x || rcPointLeft.y != rcPointRight.y)
        {
          isEqual = false;
          break;
        }
      }

      if(iterator_left.HasNext() != iterator_right.HasNext())
      {
        isEqual = false;
      }
    }
  }

  return isEqual;
}

bool operator==(const GdcFt_Output_FeatureTracking& rcLeft, const GdcFt_Output_FeatureTracking& rcRight)
{
  bool isEqual = (rcLeft.mSceneCounter == rcRight.mSceneCounter &&
                  rcLeft.mNumberOfDetectedFeatures == rcRight.mNumberOfDetectedFeatures);

  if(isEqual == true && rcLeft.mNumberOfDetectedFeatures > 0)
  {
    SHOW_ERROR(rcLeft.mppTrackedFeatures != 0 && rcRight.mppTrackedFeatures != 0);

    if(rcLeft.mppTrackedFeatures != 0 && rcRight.mppTrackedFeatures != 0)
    {
      for(uint32_t i = 0; i < rcLeft.mNumberOfDetectedFeatures; ++i)
      {
        SHOW_ERROR(rcLeft.mppTrackedFeatures[i] != 0 && rcRight.mppTrackedFeatures[i] != 0);

        if(!(*(rcLeft.mppTrackedFeatures[i]) == *(rcRight.mppTrackedFeatures[i])))
        {
          isEqual = false;
          break;
        }
      }
    }
    else
    {
      isEqual = false;
    }
  }

  return isEqual;
}

/**********************************************************************************************************/
// GdcFt_OutputFile_FeatureTracking function definitions
/**********************************************************************************************************/

LIB_RESULT GdcFt_OutputFile_FeatureTracking::Write(GdcFt_Output_FeatureTracking& rOutput)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mpFileWrite != 0)
  {
    size_t elementsWritten = 0;

    // Serializing the output for an image
    elementsWritten = fwrite(&(rOutput.mSceneCounter), sizeof(rOutput.mSceneCounter), 1, mpFileWrite);
    if(elementsWritten == 1)
    {
      elementsWritten =
          fwrite(&(rOutput.mNumberOfDetectedFeatures), sizeof(rOutput.mNumberOfDetectedFeatures), 1, mpFileWrite);
    }

    if(elementsWritten == 1 && rOutput.mppTrackedFeatures != 0)
    {
      libResult = LIB_SUCCESS;

      for(uint32_t i = 0; i < rOutput.mNumberOfDetectedFeatures; ++i)
      {
        if(rOutput.mppTrackedFeatures[i] != 0)
        {
          elementsWritten = 0;

          elementsWritten += fwrite(&(rOutput.mppTrackedFeatures[i]->mCurrent.position.x),
                                    sizeof(rOutput.mppTrackedFeatures[i]->mCurrent.position.x), 1, mpFileWrite);

          elementsWritten += fwrite(&(rOutput.mppTrackedFeatures[i]->mCurrent.position.y),
                                    sizeof(rOutput.mppTrackedFeatures[i]->mCurrent.position.y), 1, mpFileWrite);

          elementsWritten += fwrite(&(rOutput.mppTrackedFeatures[i]->mCurrent.strength),
                                    sizeof(rOutput.mppTrackedFeatures[i]->mCurrent.strength), 1, mpFileWrite);

          elementsWritten += fwrite(&(rOutput.mppTrackedFeatures[i]->mCurrent.reserve),
                                    sizeof(rOutput.mppTrackedFeatures[i]->mCurrent.reserve), 1, mpFileWrite);

          elementsWritten += fwrite(&(rOutput.mppTrackedFeatures[i]->mAgeInFramesSinceDetection),
                                    sizeof(rOutput.mppTrackedFeatures[i]->mAgeInFramesSinceDetection), 1, mpFileWrite);

          for(uint32_t j = 0; j < gcTrackedFeatureHistoryLength; ++j)
          {
            elementsWritten += fwrite(&(rOutput.mppTrackedFeatures[i]->mPositions[j].x),
                                      sizeof(rOutput.mppTrackedFeatures[i]->mPositions[j].x), 1, mpFileWrite);

            elementsWritten += fwrite(&(rOutput.mppTrackedFeatures[i]->mPositions[j].y),
                                      sizeof(rOutput.mppTrackedFeatures[i]->mPositions[j].y), 1, mpFileWrite);
          }

          elementsWritten += fwrite(&(rOutput.mppTrackedFeatures[i]->mPositionsIndex_Current),
                                    sizeof(rOutput.mppTrackedFeatures[i]->mPositionsIndex_Current), 1, mpFileWrite);

          elementsWritten += fwrite(&(rOutput.mppTrackedFeatures[i]->mPositionsSize),
                                    sizeof(rOutput.mppTrackedFeatures[i]->mPositionsSize), 1, mpFileWrite);

          if(elementsWritten != (7 + 2 * gcTrackedFeatureHistoryLength))
          {
            libResult = LIB_FAILURE;
            SHOW_ERROR(false);
            break;
          }
        }
        else
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

LIB_RESULT GdcFt_OutputFile_FeatureTracking::Read(GdcFt_Output_FeatureTracking& rOutput)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mpFileRead != 0)
  {
    size_t elementsRead = 0;

    // Deserializing the input for an image
    elementsRead = fread(&(rOutput.mSceneCounter), sizeof(rOutput.mSceneCounter), 1, mpFileRead);

    if(elementsRead == 1)
    {
      elementsRead =
          fread(&(rOutput.mNumberOfDetectedFeatures), sizeof(rOutput.mNumberOfDetectedFeatures), 1, mpFileRead);
    }

    if(elementsRead == 1)
    {
      libResult = LIB_SUCCESS;

      if(rOutput.mNumberOfDetectedFeatures == 0)
      {
        rOutput.mppTrackedFeatures = 0;
      }
      else
      {
        rOutput.mppTrackedFeatures =
            (GdcFt_TrackedFeature**)calloc(1, sizeof(GdcFt_TrackedFeature*) * rOutput.mNumberOfDetectedFeatures);
        SHOW_ERROR(rOutput.mppTrackedFeatures != 0);

        if(rOutput.mppTrackedFeatures != 0)
        {
          rOutput.mWeFreeBuffers =
              true; // Any buffers allocated during read of the reference will be freed in the destructor.

          for(uint32_t i = 0; i < rOutput.mNumberOfDetectedFeatures; ++i)
          {
            rOutput.mppTrackedFeatures[i] = (GdcFt_TrackedFeature*)malloc(sizeof(GdcFt_TrackedFeature));

            new((void*)rOutput.mppTrackedFeatures[i]) GdcFt_TrackedFeature;

            if(rOutput.mppTrackedFeatures[i] != 0)
            {
              elementsRead = 0;

              elementsRead += fread(&(rOutput.mppTrackedFeatures[i]->mCurrent.position.x),
                                    sizeof(rOutput.mppTrackedFeatures[i]->mCurrent.position.x), 1, mpFileRead);

              elementsRead += fread(&(rOutput.mppTrackedFeatures[i]->mCurrent.position.y),
                                    sizeof(rOutput.mppTrackedFeatures[i]->mCurrent.position.y), 1, mpFileRead);

              elementsRead += fread(&(rOutput.mppTrackedFeatures[i]->mCurrent.strength),
                                    sizeof(rOutput.mppTrackedFeatures[i]->mCurrent.strength), 1, mpFileRead);

              elementsRead += fread(&(rOutput.mppTrackedFeatures[i]->mCurrent.reserve),
                                    sizeof(rOutput.mppTrackedFeatures[i]->mCurrent.reserve), 1, mpFileRead);

              elementsRead += fread(&(rOutput.mppTrackedFeatures[i]->mAgeInFramesSinceDetection),
                                    sizeof(rOutput.mppTrackedFeatures[i]->mAgeInFramesSinceDetection), 1, mpFileRead);

              for(uint32_t j = 0; j < gcTrackedFeatureHistoryLength; ++j)
              {
                elementsRead += fread(&(rOutput.mppTrackedFeatures[i]->mPositions[j].x),
                                      sizeof(rOutput.mppTrackedFeatures[i]->mPositions[j].x), 1, mpFileRead);

                elementsRead += fread(&(rOutput.mppTrackedFeatures[i]->mPositions[j].y),
                                      sizeof(rOutput.mppTrackedFeatures[i]->mPositions[j].y), 1, mpFileRead);
              }

              elementsRead += fread(&(rOutput.mppTrackedFeatures[i]->mPositionsIndex_Current),
                                    sizeof(rOutput.mppTrackedFeatures[i]->mPositionsIndex_Current), 1, mpFileRead);

              elementsRead += fread(&(rOutput.mppTrackedFeatures[i]->mPositionsSize),
                                    sizeof(rOutput.mppTrackedFeatures[i]->mPositionsSize), 1, mpFileRead);

              SHOW_ERROR(rOutput.mppTrackedFeatures[i]->mPositionsIndex_Current < gcTrackedFeatureHistoryLength &&
                         rOutput.mppTrackedFeatures[i]->mPositionsSize < gcTrackedFeatureHistoryLength);

              if(elementsRead != (7 + 2 * gcTrackedFeatureHistoryLength))
              {
                libResult = LIB_FAILURE;
                SHOW_ERROR(false);
                break;
              }
            }
            else
            {
              libResult = LIB_FAILURE;
              SHOW_ERROR(false);
              break;
            }
          }
        }
        else
        {
          libResult = LIB_FAILURE;
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
void GdcFt_Helper::PrintMismatch_HarrisCorners(FILE* const                       cpFile,
                                               const GdcFt_Output_HarrisCorners& rcLocalOutput,
                                               const GdcFt_Output_HarrisCorners& rcReferenceOutput)
{
  bool weKeepGoing = true;

  fprintf(cpFile, "GdcFt_TrackerTask: mismatch at frame %u\n", rcLocalOutput.mSceneCounter);

  if(rcLocalOutput.mSceneCounter != rcReferenceOutput.mSceneCounter)
  {
    fprintf(cpFile, "   mSceneCounter %u != %u\n", rcLocalOutput.mSceneCounter, rcReferenceOutput.mSceneCounter);
  }

  const int32_t cLocalCount     = rcLocalOutput.mFeatureDescriptor.GetCount();
  const int32_t cReferenceCount = rcReferenceOutput.mFeatureDescriptor.GetCount();

  if(cLocalCount != cReferenceCount)
  {
    fprintf(cpFile, "   mFeatureDescriptor.GetCount() %d != %d\n", cLocalCount, cReferenceCount);
  }

  SHOW_ERROR(cLocalCount >= 0 && cReferenceCount >= 0);

  if(weKeepGoing == true)
  {
    if((rcLocalOutput.mFeatureDescriptor.GetDataPtr() == 0 && rcReferenceOutput.mFeatureDescriptor.GetDataPtr() != 0) ||
       (rcLocalOutput.mFeatureDescriptor.GetDataPtr() != 0 && rcReferenceOutput.mFeatureDescriptor.GetDataPtr() == 0))
    {
      fprintf(cpFile, "   mFeatureDescriptor.GetDataPtr() %p != %p\n",
              (void*)(rcLocalOutput.mFeatureDescriptor.GetDataPtr()),
              (void*)(rcReferenceOutput.mFeatureDescriptor.GetDataPtr()));
    }
    else
    {
      icp::Feature* pLocalFeaturesSorted = (icp::Feature*)malloc(cLocalCount * sizeof(pLocalFeaturesSorted[0]));
      icp::Feature* pReferenceFeaturesSorted =
          (icp::Feature*)malloc(cReferenceCount * sizeof(pReferenceFeaturesSorted[0]));

      if(pLocalFeaturesSorted != 0 && pReferenceFeaturesSorted != 0)
      {
        memcpy(pLocalFeaturesSorted, rcLocalOutput.mFeatureDescriptor.GetDataPtr(),
               cLocalCount * sizeof(pLocalFeaturesSorted[0]));

        memcpy(pReferenceFeaturesSorted, rcReferenceOutput.mFeatureDescriptor.GetDataPtr(),
               cReferenceCount * sizeof(pReferenceFeaturesSorted[0]));

        std::sort(pLocalFeaturesSorted, pLocalFeaturesSorted + cLocalCount, GdcFt_Helper::LessThan);

        std::sort(pReferenceFeaturesSorted, pReferenceFeaturesSorted + cReferenceCount, GdcFt_Helper::LessThan);

        std::vector< icp::Feature > setDifference;

        std::set_difference(pLocalFeaturesSorted, pLocalFeaturesSorted + cLocalCount, pReferenceFeaturesSorted,
                            pReferenceFeaturesSorted + cReferenceCount,
                            std::inserter(setDifference, setDifference.begin()), GdcFt_Helper::LessThan);

        if(setDifference.size() > 0)
        {
          fprintf(cpFile, "   mFeatureDescriptor Local-Reference\n");
          for(uint32_t i = 0; i < setDifference.size(); ++i)
          {
            fprintf(cpFile, "   (%4d,%4d,%5d,%5d)\n", setDifference[i].position.x, setDifference[i].position.y,
                    setDifference[i].strength, setDifference[i].reserve);
          }
        }

        setDifference.clear();

        std::set_difference(pReferenceFeaturesSorted, pReferenceFeaturesSorted + cReferenceCount, pLocalFeaturesSorted,
                            pLocalFeaturesSorted + cLocalCount, std::inserter(setDifference, setDifference.begin()),
                            GdcFt_Helper::LessThan);

        if(setDifference.size() > 0)
        {
          fprintf(cpFile, "   mFeatureDescriptor Reference-Local\n");
          for(uint32_t i = 0; i < setDifference.size(); ++i)
          {
            fprintf(cpFile, "   (%4d,%4d,%5d,%5d)\n", setDifference[i].position.x, setDifference[i].position.y,
                    setDifference[i].strength, setDifference[i].reserve);
          }
        }
      }

      if(pLocalFeaturesSorted != 0)
      {
        free(pLocalFeaturesSorted);
        pLocalFeaturesSorted = 0;
      }
      if(pReferenceFeaturesSorted != 0)
      {
        free(pReferenceFeaturesSorted);
        pReferenceFeaturesSorted = 0;
      }
    }
  }
}

/***************************************************************************/
/**
 * \par Description
 **************************************************************************/
void GdcFt_Helper::PrintMismatch_PyramidMultiCreation(FILE* const                              cpFile,
                                                      const GdcFt_Output_PyramidMultiCreation& rcLocalOutput,
                                                      const GdcFt_Output_PyramidMultiCreation& rcReferenceOutput)
{
  fprintf(cpFile, "GdcFt_PyramidTask: mismatch at frame %d\n", rcLocalOutput.mSceneCounter);

  if(rcLocalOutput.mSceneCounter != rcReferenceOutput.mSceneCounter)
  {
    fprintf(cpFile, "   mSceneCounter %d != %d\n", rcLocalOutput.mSceneCounter, rcReferenceOutput.mSceneCounter);
  }

  const uint32_t cNumberOfDd = sizeof(rcLocalOutput.mImagePyramidDd) / sizeof(rcLocalOutput.mImagePyramidDd[0]);

  for(uint32_t i = 0; i < cNumberOfDd; ++i)
  {
    if(rcLocalOutput.mImagePyramidDd[i].GetWidth() != rcReferenceOutput.mImagePyramidDd[i].GetWidth())
    {
      fprintf(cpFile, "   mImagePyramidDd[%d].GetWidth() %d != %d\n", (int)i,
              rcLocalOutput.mImagePyramidDd[i].GetWidth(), rcReferenceOutput.mImagePyramidDd[i].GetWidth());
    }
    if(rcLocalOutput.mImagePyramidDd[i].GetHeight() != rcReferenceOutput.mImagePyramidDd[i].GetHeight())
    {
      fprintf(cpFile, "   mImagePyramidDd[%d].GetHeight() %d != %d\n", (int)i,
              rcLocalOutput.mImagePyramidDd[i].GetHeight(), rcReferenceOutput.mImagePyramidDd[i].GetHeight());
    }
    if(rcLocalOutput.mImagePyramidDd[i].GetElementDataType() !=
       rcReferenceOutput.mImagePyramidDd[i].GetElementDataType())
    {
      fprintf(cpFile, "   mImagePyramidDd[%d].GetElementDataType() %d != %d\n", (int)i,
              (int)(rcLocalOutput.mImagePyramidDd[i].GetElementDataType()),
              (int)(rcReferenceOutput.mImagePyramidDd[i].GetElementDataType()));
    }
    if(rcLocalOutput.mImagePyramidDd[i].GetElementDimX() != rcReferenceOutput.mImagePyramidDd[i].GetElementDimX())
    {
      fprintf(cpFile, "   mImagePyramidDd[%d].GetElementDimX() %d != %d\n", (int)i,
              rcLocalOutput.mImagePyramidDd[i].GetElementDimX(), rcReferenceOutput.mImagePyramidDd[i].GetElementDimX());
    }
    if(rcLocalOutput.mImagePyramidDd[i].GetElementDimY() != rcReferenceOutput.mImagePyramidDd[i].GetElementDimY())
    {
      fprintf(cpFile, "   mImagePyramidDd[%d].GetElementDimY() %d != %d\n", (int)i,
              rcLocalOutput.mImagePyramidDd[i].GetElementDimY(), rcReferenceOutput.mImagePyramidDd[i].GetElementDimY());
    }
    if(rcLocalOutput.mImagePyramidDd[i].GetSpan() != rcReferenceOutput.mImagePyramidDd[i].GetSpan())
    {
      fprintf(cpFile, "   mImagePyramidDd[%d].GetSpan() %d != %d\n", (int)i, rcLocalOutput.mImagePyramidDd[i].GetSpan(),
              rcReferenceOutput.mImagePyramidDd[i].GetSpan());
    }

    if(rcLocalOutput.mImagePyramidDd[i].GetSpan() == rcReferenceOutput.mImagePyramidDd[i].GetSpan() &&
       rcLocalOutput.mImagePyramidDd[i].GetHeight() == rcReferenceOutput.mImagePyramidDd[i].GetHeight())
    {
      const uint32_t cWidth  = rcLocalOutput.mImagePyramidDd[i].GetWidth();
      const uint32_t cHeight = rcLocalOutput.mImagePyramidDd[i].GetHeight();
      const uint32_t cSpan   = rcLocalOutput.mImagePyramidDd[i].GetSpan();

      const uint8_t* const cpcLocalBegin = (uint8_t*)(rcLocalOutput.mImagePyramidDd[i].GetDataPtr());
      const uint8_t* const cpcLocalEnd = (uint8_t*)(rcLocalOutput.mImagePyramidDd[i].GetDataPtr()) + (cSpan * cHeight);
      const uint8_t* const cpcReferenceBegin = (uint8_t*)(rcReferenceOutput.mImagePyramidDd[i].GetDataPtr());
      const uint8_t* const cpcReferenceEnd =
          (uint8_t*)(rcReferenceOutput.mImagePyramidDd[i].GetDataPtr()) + (cSpan * cHeight);

      const std::pair< const uint8_t*, const uint8_t* > cPairResult =
          std::mismatch(cpcLocalBegin, cpcLocalEnd, cpcReferenceBegin);
      if(cPairResult.first != cpcLocalEnd || cPairResult.second != cpcReferenceEnd)
      {
        SHOW_ERROR(cPairResult.first != cpcLocalEnd && cPairResult.second != cpcReferenceEnd);

        const uintptr_t cBadPixel = ((uintptr_t)cPairResult.first - (uintptr_t)cpcLocalBegin);
        const uint32_t  cBadX     = (cBadPixel % cSpan);
        const uint32_t  cBadY     = (cBadPixel / cSpan);

        fprintf(cpFile, "   mImagePyramidDd[%2d] (%3d,%3d,%3d): pixel (%3d,%3d): %X != %X\n", (int)i, cSpan, cWidth,
                cHeight, cBadX, cBadY, *(cPairResult.first), *(cPairResult.second));
      }
    }
  }
}

/***************************************************************************/
/**
 * \par Description
 **************************************************************************/
void GdcFt_Helper::PrintMismatch_LKTrackerOpticalFlow(FILE* const                              cpFile,
                                                      const GdcFt_Output_LKTrackerOpticalFlow& rcLocalOutput,
                                                      const GdcFt_Output_LKTrackerOpticalFlow& rcReferenceOutput)
{
  fprintf(cpFile, "GdcFt_TrackerTask: mismatch at frame %u\n", rcLocalOutput.mSceneCounter);

  if(rcLocalOutput.mSceneCounter != rcReferenceOutput.mSceneCounter)
  {
    fprintf(cpFile, "   mSceneCounter %u != %u\n", rcLocalOutput.mSceneCounter, rcReferenceOutput.mSceneCounter);
  }

  const int32_t cLocalCount     = rcLocalOutput.mFeature32SDescriptor.GetCount();
  const int32_t cReferenceCount = rcReferenceOutput.mFeature32SDescriptor.GetCount();

  if(cLocalCount != cReferenceCount)
  {
    fprintf(cpFile, "   mFeature32SDescriptor.GetCount() %d != %d\n", cLocalCount, cReferenceCount);
  }

  SHOW_ERROR(cLocalCount >= 0 && cReferenceCount >= 0);

  if((rcLocalOutput.mFeature32SDescriptor.GetDataPtr() == 0 &&
      rcReferenceOutput.mFeature32SDescriptor.GetDataPtr() != 0) ||
     (rcLocalOutput.mFeature32SDescriptor.GetDataPtr() != 0 &&
      rcReferenceOutput.mFeature32SDescriptor.GetDataPtr() == 0))
  {
    fprintf(cpFile, "   mFeature32SDescriptor.GetDataPtr() %p != %p\n",
            (void*)(rcLocalOutput.mFeature32SDescriptor.GetDataPtr()),
            (void*)(rcReferenceOutput.mFeature32SDescriptor.GetDataPtr()));
  }
  else
  {
    icp::Feature* pLocalFeaturesSorted = (icp::Feature*)malloc(cLocalCount * sizeof(pLocalFeaturesSorted[0]));
    icp::Feature* pReferenceFeaturesSorted =
        (icp::Feature*)malloc(cReferenceCount * sizeof(pReferenceFeaturesSorted[0]));

    if(pLocalFeaturesSorted != 0 && pReferenceFeaturesSorted != 0)
    {
      memcpy(pLocalFeaturesSorted, rcLocalOutput.mFeature32SDescriptor.GetDataPtr(),
             cLocalCount * sizeof(pLocalFeaturesSorted[0]));

      memcpy(pReferenceFeaturesSorted, rcReferenceOutput.mFeature32SDescriptor.GetDataPtr(),
             cReferenceCount * sizeof(pReferenceFeaturesSorted[0]));

      std::sort(pLocalFeaturesSorted, pLocalFeaturesSorted + cLocalCount, GdcFt_Helper::LessThan);

      std::sort(pReferenceFeaturesSorted, pReferenceFeaturesSorted + cReferenceCount, GdcFt_Helper::LessThan);

      std::vector< icp::Feature > setDifference;

      std::set_difference(pLocalFeaturesSorted, pLocalFeaturesSorted + cLocalCount, pReferenceFeaturesSorted,
                          pReferenceFeaturesSorted + cReferenceCount,
                          std::inserter(setDifference, setDifference.begin()), GdcFt_Helper::LessThan);

      if(setDifference.size() > 0)
      {
        fprintf(cpFile, "   mFeature32SDescriptor Local-Reference\n");

        for(uint32_t i = 0; i < setDifference.size(); ++i)
        {
          fprintf(cpFile, "   (%4d,%4d,%5d,%5d)\n", setDifference[i].position.x, setDifference[i].position.y,
                  setDifference[i].strength, setDifference[i].reserve);

          if(i > 6)
          {
            fprintf(cpFile, "   ... %lu others\n", setDifference.size() - i);
            break;
          }
        }
      }

      setDifference.clear();

      std::set_difference(pReferenceFeaturesSorted, pReferenceFeaturesSorted + cReferenceCount, pLocalFeaturesSorted,
                          pLocalFeaturesSorted + cLocalCount, std::inserter(setDifference, setDifference.begin()),
                          GdcFt_Helper::LessThan);

      if(setDifference.size() > 0)
      {
        fprintf(cpFile, "   mFeature32SDescriptor Reference-Local\n");
        for(uint32_t i = 0; i < setDifference.size(); ++i)
        {
          fprintf(cpFile, "   (%4d,%4d,%5d,%5d)\n", setDifference[i].position.x, setDifference[i].position.y,
                  setDifference[i].strength, setDifference[i].reserve);

          if(i > 6)
          {
            fprintf(cpFile, "   ... %lu others\n", setDifference.size() - i);
            break;
          }
        }
      }
    }

    if(pLocalFeaturesSorted != 0)
    {
      free(pLocalFeaturesSorted);
      pLocalFeaturesSorted = 0;
    }
    if(pReferenceFeaturesSorted != 0)
    {
      free(pReferenceFeaturesSorted);
      pReferenceFeaturesSorted = 0;
    }
  }
}

/***************************************************************************/
/**
 * \par Description
 **************************************************************************/
void GdcFt_Helper::PrintMismatch_FeatureTracking(FILE* const                         cpFile,
                                                 const GdcFt_Output_FeatureTracking& rcLocalOutput,
                                                 const GdcFt_Output_FeatureTracking& rcReferenceOutput)
{
  bool weKeepGoing = true;

  fprintf(cpFile, "GdcFt_TrackerTask: mismatch at frame %u\n", rcLocalOutput.mSceneCounter);

  if(rcLocalOutput.mSceneCounter != rcReferenceOutput.mSceneCounter)
  {
    fprintf(cpFile, "   mSceneCounter %u != %u\n", rcLocalOutput.mSceneCounter, rcReferenceOutput.mSceneCounter);
  }

  if(rcLocalOutput.mNumberOfDetectedFeatures != rcReferenceOutput.mNumberOfDetectedFeatures)
  {
    fprintf(cpFile, "   mNumberOfDetectedFeatures %u != %u\n", rcLocalOutput.mNumberOfDetectedFeatures,
            rcReferenceOutput.mNumberOfDetectedFeatures);
  }

  if(rcLocalOutput.mNumberOfDetectedFeatures == rcReferenceOutput.mNumberOfDetectedFeatures &&
     rcLocalOutput.mNumberOfDetectedFeatures > 0 && rcReferenceOutput.mNumberOfDetectedFeatures > 0)
  {
    if((rcLocalOutput.mppTrackedFeatures == 0 && rcReferenceOutput.mppTrackedFeatures != 0) ||
       (rcLocalOutput.mppTrackedFeatures != 0 && rcReferenceOutput.mppTrackedFeatures == 0))
    {
      fprintf(cpFile, "   mppTrackedFeatures %p != %p\n", (void*)(rcLocalOutput.mppTrackedFeatures),
              (void*)(rcReferenceOutput.mppTrackedFeatures));
    }
    else
    {
      for(uint32_t i = 0; i < rcLocalOutput.mNumberOfDetectedFeatures; ++i)
      {
        if(rcLocalOutput.mppTrackedFeatures[i] != 0 && rcReferenceOutput.mppTrackedFeatures[i] != 0)
        {
          if(rcLocalOutput.mppTrackedFeatures[i]->mCurrent.position.x !=
             rcReferenceOutput.mppTrackedFeatures[i]->mCurrent.position.x)
          {
            fprintf(cpFile, "   mppTrackedFeatures[%d].mCurrent.position.x %d != %d\n", i,
                    rcLocalOutput.mppTrackedFeatures[i]->mCurrent.position.x,
                    rcReferenceOutput.mppTrackedFeatures[i]->mCurrent.position.x);

            weKeepGoing = false;
          }

          if(rcLocalOutput.mppTrackedFeatures[i]->mCurrent.position.y !=
             rcReferenceOutput.mppTrackedFeatures[i]->mCurrent.position.y)
          {
            fprintf(cpFile, "   mppTrackedFeatures[%u].mCurrent.position.y %d != %d\n", i,
                    rcLocalOutput.mppTrackedFeatures[i]->mCurrent.position.y,
                    rcReferenceOutput.mppTrackedFeatures[i]->mCurrent.position.y);

            weKeepGoing = false;
          }

          if(rcLocalOutput.mppTrackedFeatures[i]->mCurrent.strength !=
             rcReferenceOutput.mppTrackedFeatures[i]->mCurrent.strength)
          {
            fprintf(cpFile, "   mppTrackedFeatures[%u].mCurrent.strength %d != %d\n", i,
                    rcLocalOutput.mppTrackedFeatures[i]->mCurrent.strength,
                    rcReferenceOutput.mppTrackedFeatures[i]->mCurrent.strength);

            weKeepGoing = false;
          }

          if(rcLocalOutput.mppTrackedFeatures[i]->mCurrent.reserve !=
             rcReferenceOutput.mppTrackedFeatures[i]->mCurrent.reserve)
          {
            fprintf(cpFile, "   mppTrackedFeatures[%u].mCurrent.reserve %d != %d\n", i,
                    rcLocalOutput.mppTrackedFeatures[i]->mCurrent.reserve,
                    rcReferenceOutput.mppTrackedFeatures[i]->mCurrent.reserve);

            weKeepGoing = false;
          }

          if(rcLocalOutput.mppTrackedFeatures[i]->mAgeInFramesSinceDetection !=
             rcReferenceOutput.mppTrackedFeatures[i]->mAgeInFramesSinceDetection)
          {
            fprintf(cpFile, "   mppTrackedFeatures[%u].mAgeInFramesSinceDetection %u != %u\n", i,
                    rcLocalOutput.mppTrackedFeatures[i]->mAgeInFramesSinceDetection,
                    rcReferenceOutput.mppTrackedFeatures[i]->mAgeInFramesSinceDetection);

            weKeepGoing = false;
          }

          if(rcLocalOutput.mppTrackedFeatures[i]->mPositionsIndex_Current !=
             rcReferenceOutput.mppTrackedFeatures[i]->mPositionsIndex_Current)
          {
            fprintf(cpFile, "   mppTrackedFeatures[%u].mPositionsIndex_Current %u != %u\n", i,
                    rcLocalOutput.mppTrackedFeatures[i]->mPositionsIndex_Current,
                    rcReferenceOutput.mppTrackedFeatures[i]->mPositionsIndex_Current);

            weKeepGoing = false;
          }

          if(rcLocalOutput.mppTrackedFeatures[i]->mPositionsSize !=
             rcReferenceOutput.mppTrackedFeatures[i]->mPositionsSize)
          {
            fprintf(cpFile, "   mppTrackedFeatures[%u].mPositionsSize %u != %u\n", i,
                    rcLocalOutput.mppTrackedFeatures[i]->mPositionsSize,
                    rcReferenceOutput.mppTrackedFeatures[i]->mPositionsSize);

            weKeepGoing = false;
          }

          GdcFt_TrackedFeatureIterator iterator_local;
          GdcFt_TrackedFeatureIterator iterator_ref;

          LIB_RESULT libResult_local = rcLocalOutput.mppTrackedFeatures[i]->Iterator(iterator_local);
          LIB_RESULT libResult_ref   = rcReferenceOutput.mppTrackedFeatures[i]->Iterator(iterator_ref);

          if(libResult_local == LIB_SUCCESS && libResult_ref == LIB_SUCCESS)
          {
            uint32_t j = 0;

            while(iterator_local.HasNext() == true && iterator_ref.HasNext() == true)
            {
              const ICP_Point& rcPointLeft  = *(iterator_local.Next());
              const ICP_Point& rcPointRight = *(iterator_ref.Next());

              if(rcPointLeft.x != rcPointRight.x)
              {
                fprintf(cpFile, "   mppTrackedFeatures[%u].Iterator[%d].x %u != %d\n", i, j, rcPointLeft.x,
                        rcPointRight.x);
              }

              if(rcPointLeft.y != rcPointRight.y)
              {
                fprintf(cpFile, "   mppTrackedFeatures[%u].Iterator[%u].y %d != %d\n", i, j, rcPointLeft.y,
                        rcPointRight.y);
              }

              if(rcPointLeft.x != rcPointRight.x || rcPointLeft.y != rcPointRight.y)
              {
                weKeepGoing = false;
                break;
              }
              ++j;
            }

            if(iterator_local.HasNext() != iterator_ref.HasNext())
            {
              fprintf(cpFile, "   mppTrackedFeatures[%u].Iterator[%u].HasNext() %d != %d\n", i, j,
                      iterator_local.HasNext(), iterator_ref.HasNext());

              weKeepGoing = false;
            }
          }
          else
          {
            if(libResult_local != LIB_SUCCESS || libResult_ref != LIB_SUCCESS)
            {
              fprintf(cpFile, "   mppTrackedFeatures[%u].Iterator libResult %ld != %ld\n", i, libResult_local,
                      libResult_ref);

              weKeepGoing = false;
            }
          }

          if(weKeepGoing == false)
          {
            break;
          }
        }
        else
        {
          fprintf(cpFile, "   mppTrackedFeatures[%d] %p != %p\n", i, (void*)(rcLocalOutput.mppTrackedFeatures),
                  (void*)(rcReferenceOutput.mppTrackedFeatures));

          weKeepGoing = false;
          break;
        }
      }
    }
  }
}

/***************************************************************************/
/**
 * \par Description
 **************************************************************************/
bool GdcFt_Helper::LessThan(const icp::Feature& rcLeft, const icp::Feature& rcRight)
{
  bool isLessThan = false;

  if(rcLeft.position.x < rcRight.position.x)
  {
    isLessThan = true;
  }
  else if(rcLeft.position.x == rcRight.position.x)
  {
    if(rcLeft.position.y < rcRight.position.y)
    {
      isLessThan = true;
    }
    else if(rcLeft.position.y == rcRight.position.y)
    {
      if(rcLeft.strength < rcRight.strength)
      {
        isLessThan = true;
      }
      else if(rcLeft.strength == rcRight.strength)
      {
        if(rcLeft.reserve < rcRight.reserve)
        {
          isLessThan = true;
        }
      }
    }
  }

  return isLessThan;
}

/***************************************************************************/
/**
 * \par Description
 **************************************************************************/
bool GdcFt_Helper::LessThan32S(const icp::Feature32S& rcLeft, const icp::Feature32S& rcRight)
{
  bool isLessThan = false;

  if(rcLeft.position.x < rcRight.position.x)
  {
    isLessThan = true;
  }
  else if(rcLeft.position.x == rcRight.position.x)
  {
    if(rcLeft.position.y < rcRight.position.y)
    {
      isLessThan = true;
    }
    else if(rcLeft.position.y == rcRight.position.y)
    {
      if(rcLeft.strength < rcRight.strength)
      {
        isLessThan = true;
      }
      else if(rcLeft.strength == rcRight.strength)
      {
        if(rcLeft.reserve < rcRight.reserve)
        {
          isLessThan = true;
        }
      }
    }
  }

  return isLessThan;
}
