/*******************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2017 NXP
* All Rights Reserved
*
********************************************************************************
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
*******************************************************************************/

#include <oal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <global_hal.h>
#include <cmem_if_hal.h>

#include <apexcv_pro_hog.h>
#include <hog_config.h>

// GENERATED ACF PROCESS
#include <HOG_SCORE.hpp>
#include <HOG_DESCRIPTOR.hpp>


namespace apexcv
{

/***********************
 * Default constructor
 **********************/
Hog::Hog()
  : 
  mpScoreProcess(nullptr),
  mpDescriptorProcess(nullptr),
  mApexId(0),
  mInit(false),
  mType(HogType::DETECT)
{
}


/**************
 * Destructor
 *************/
Hog::~Hog()
{
  Release();
}


/**************
 * Initialize
 *************/
APEXCV_LIB_RESULT Hog::Initialize(const vsdk::UMat& aSrc, vsdk::UMat& aDst, HogType aType)
{
  APEXCV_LIB_RESULT lStatus = APEXCV_SUCCESS;

  if(mInit)
  {
    lStatus = APEXCV_ERROR_OBJECT_ALREADY_INITIALIZED;
  }
  else
  {
    mType = aType;

    switch(mType)
    {
      case HogType::DETECT:
      {
        HOG_SCORE *pScore = new HOG_SCORE;

        if(nullptr == pScore)
        {
          lStatus = APEXCV_ERROR_MEMORY_ALLOCATION_FAILED;
        }
        else
        {
          mpScoreProcess = pScore;

          if(pScore->Initialize() != 0)
          {
            lStatus = APEXCV_ERROR_ACF_PROC_INITIALIZE;
          }
          else
          {
            ApexcvHostBaseBaseClass::InfoCluster lInfo;
            lInfo.set_ACF("HOG_SCORE", pScore);
            lInfo.push_PortName("IMAGE");
            mvInfoClusters.push_back(lInfo);
          }
        }
        if(APEXCV_SUCCESS == lStatus)
        {
          // Allocate svm.width for 8-bit weights + 2 for 16-bit offset
          mSVM = vsdk::UMat(DefaultParams.svm.height, DefaultParams.svm.width + 2, VSDK_CV_8SC1);

          vsdk::Mat SVM_mat = mSVM.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);

          if(nullptr == SVM_mat.data)
          {
            lStatus = APEXCV_ERROR_MEMORY_ALLOCATION_FAILED;
          }
          else
          {
            memset(SVM_mat.data, 0, DefaultParams.svm.height * (DefaultParams.svm.width + 2));
          }
        }
      }
      break;

      case HogType::CELLS:
      {
        HOG_DESCRIPTOR *pDescriptor = new HOG_DESCRIPTOR;

        if(nullptr == pDescriptor)
        {
          lStatus = APEXCV_ERROR_MEMORY_ALLOCATION_FAILED;
        }
        else
        {
          mpDescriptorProcess = pDescriptor;

          if(pDescriptor->Initialize())
          {
            lStatus = APEXCV_ERROR_ACF_PROC_INITIALIZE;
          }
          else
          {
            ApexcvHostBaseBaseClass::InfoCluster lInfo;
            lInfo.set_ACF("HOG_DESCRIPTOR", pDescriptor);
            lInfo.push_PortName("IMAGE");
            mvInfoClusters.push_back(lInfo);
          }
        }
      }
    }

    if(APEXCV_SUCCESS == lStatus)
    {
      lStatus = ReconnectIO(aSrc, aDst);
    }

  }

  if(APEXCV_SUCCESS == lStatus)
  {
    mInit = true;
  }

  return lStatus;
}


/*******************************************************
 * Release allocated buffers and reset member variables
 ******************************************************/
APEXCV_LIB_RESULT Hog::Release()
{
  if(nullptr != mpScoreProcess)
  {
    delete static_cast<HOG_SCORE*>(mpScoreProcess);
  }

  if(nullptr != mpDescriptorProcess)
  {
    delete static_cast<HOG_DESCRIPTOR*>(mpDescriptorProcess);
  }

  // Reset member variables
  mInit = false;
  mpScoreProcess = nullptr;
  mpDescriptorProcess = nullptr;

  return APEXCV_SUCCESS;
}

/*********************************
 * Connect input and output ports
 ********************************/
APEXCV_LIB_RESULT Hog::ReconnectIO(const vsdk::UMat &aSrc, vsdk::UMat &aDst)
{
  APEXCV_LIB_RESULT lStatus = APEXCV_SUCCESS;

  HOG_SCORE* pScoreProcess = (HOG_SCORE*) mpScoreProcess;
  HOG_DESCRIPTOR* pDescriptorProcess = (HOG_DESCRIPTOR*) mpDescriptorProcess;

  ACF_Process_APU* pProcess = (HogType::DETECT == mType) ?
    static_cast<ACF_Process_APU*>(pScoreProcess) : static_cast<ACF_Process_APU*>(pDescriptorProcess);

  int32_t numCU = pProcess->RetCuArrayWidth();
  /*
   *  The chunk width must be a multiple of the minimum unit size which is (winStrideX, winStrideY).
   *  Here we determine the smallest chunk width that can still contain the image in the full CU array and
   *  inform ACF of this chunk size through SelectScenario.
   */
  int32_t chunkWidth = (aSrc.cols + numCU - 1) / numCU;
  chunkWidth = (chunkWidth + DefaultParams.winStride.width - 1) & ~(DefaultParams.winStride.width - 1);
  chunkWidth = chunkWidth > 8 ? chunkWidth : 8;

  while(chunkWidth <= 32)
  {
    if((aSrc.cols % chunkWidth) == 0)
    {
      if(pProcess->SelectScenario("IMAGE", chunkWidth, DefaultParams.winStride.height) == 0)
      {
        break;
      }
    }

    chunkWidth += DefaultParams.winStride.width;
  }

  // Verify that selected chunkWidth is valid
  if(pProcess->SelectScenario("IMAGE", chunkWidth, DefaultParams.winStride.height))
  {
    return APEXCV_ERROR_ACF_PROC_SELECTSCENARIO;
  }

  if(HogType::DETECT == mType)
  {
    int roiRight = ((DefaultParams.winPixels.width - DefaultParams.winStride.width) / chunkWidth) * chunkWidth;
    int roiTop = ((DefaultParams.winPixels.height - DefaultParams.block.height) / DefaultParams.winStride.height) * DefaultParams.winStride.height;
    int roiBottom = DefaultParams.winStride.height;

    vsdk::UMat adjusted(aSrc,
          vsdk::Rect(0, roiTop, aSrc.cols - roiRight, aSrc.rows - roiTop - roiBottom));

    int outWidth = adjusted.cols / DefaultParams.winStride.width;
    int outHeight = adjusted.rows / DefaultParams.winStride.height;
    int outTrimWidth = (aSrc.cols - (DefaultParams.winPixels.width - DefaultParams.winStride.width)) / DefaultParams.winStride.width;
    int outTrimHeight = (aSrc.rows - (DefaultParams.winPixels.height - DefaultParams.winStride.height)) / DefaultParams.winStride.height;

    bool allocateNew = true;
    if(!aDst.empty())
    {
      cv::UMat cvDst = aDst;
      cv::Size outBufferSize;
      cv::Point outBufferOffset;
      cvDst.locateROI(outBufferSize, outBufferOffset);
      if((outBufferSize.width >= outWidth) && (outBufferSize.height >= outHeight))
      {
        // Use existing buffer, connect to (0,0)
        aDst = vsdk::UMat(aDst, vsdk::Rect(-outBufferOffset.x, -outBufferOffset.y, outWidth, outHeight));
        allocateNew = false;
      }
    }
    if(allocateNew)
    {
      // Allocate new buffer
      aDst = vsdk::UMat(outHeight, outWidth, VSDK_CV_16SC1);
    }

    if(nullptr == aDst.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data)
    {
      return APEXCV_ERROR_MEMORY_ALLOCATION_FAILED;
    }

    if( pScoreProcess->SetRoiInfo(0, roiRight, roiTop, roiBottom)
        || pScoreProcess->ConnectIO("IMAGE", adjusted)
        || pScoreProcess->ConnectIO("SVM_FUNCTION", mSVM)
        || pScoreProcess->ConnectIO("SCORES", aDst))
    {
      return APEXCV_ERROR_ACF_PROC_CONNECTIO;
    }

    // Trim output to size in case chunkWidth restrictions produced partial blocks
    aDst = vsdk::UMat(aDst, vsdk::Rect(0, 0, outTrimWidth, outTrimHeight));
  }
  else
  {
    // Compute all blocks in steps of (stride.width, stride.height)
    int roiRight = ((DefaultParams.block.width - DefaultParams.winStride.width) / chunkWidth) * chunkWidth;
    int roiTop = 0;
    int roiBottom = ((DefaultParams.block.height - DefaultParams.winStride.height) / DefaultParams.winStride.height) * DefaultParams.winStride.height;

    vsdk::UMat adjusted(aSrc,
        vsdk::Rect(0, roiTop, aSrc.cols - roiRight, aSrc.rows - roiTop - roiBottom));

    int outWidth = adjusted.cols / DefaultParams.winStride.width;
    int outHeight = adjusted.rows / DefaultParams.winStride.height;
    int trimOutWidth =  ((aSrc.cols - (DefaultParams.block.width - DefaultParams.winStride.width)) / DefaultParams.winStride.width) * N_BINS;
    int trimOutHeight =  (aSrc.rows - (DefaultParams.block.height - DefaultParams.winStride.height)) / DefaultParams.winStride.height;

    bool allocateNew = true;
    if(!aDst.empty())
    {
      cv::UMat cvDst = aDst;
      cv::Size outBufferSize;
      cv::Point outBufferOffset;
      cvDst.locateROI(outBufferSize, outBufferOffset);
      if((outBufferSize.width >= (outWidth*N_BINS)) && (outBufferSize.height >= outHeight))
      {
        // Use existing buffer, connect to (0,0)
        aDst = vsdk::UMat(aDst, vsdk::Rect(-outBufferOffset.x, -outBufferOffset.y, outWidth*N_BINS, outHeight));
        allocateNew = false;
      }
    }
    if(allocateNew)
    {
      // Output is an image with N_BINS channels
      aDst = vsdk::UMat(outHeight, outWidth*N_BINS, VSDK_CV_8UC1);
    }

    if(nullptr == aDst.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data)
    {
      return APEXCV_ERROR_MEMORY_ALLOCATION_FAILED;
    }

    if(pDescriptorProcess->SetRoiInfo(0, roiRight, roiTop, roiBottom))
    {
      return APEXCV_ERROR_ACF_PROC_CONNECTIO;
    }

    if(pDescriptorProcess->ConnectIO("IMAGE", adjusted) ||
        pDescriptorProcess->ConnectIO("BLOCK_HISTOGRAMS", aDst))
    {
      return APEXCV_ERROR_ACF_PROC_CONNECTIO;
    }

    // Trim output to size in case chunkWidth restrictions produced partial blocks
    aDst = vsdk::UMat(aDst, vsdk::Rect(0, 0, trimOutWidth, trimOutHeight));
  }
  return lStatus;
}

APEXCV_LIB_RESULT Hog::Process()
{
  APEXCV_LIB_RESULT lStatus = APEXCV_SUCCESS;

  if(!mInit)
  {
    lStatus = APEXCV_ERROR_OBJECT_ISNOT_INITIALIZED;
  }
  else
  {
    switch(mType)
    {
      case HogType::CELLS:
      {
        HOG_DESCRIPTOR* pDescriptorProcess = static_cast<HOG_DESCRIPTOR*>(mpDescriptorProcess);

        lStatus = pDescriptorProcess->Start();
        lStatus |= pDescriptorProcess->Wait();

      }
      break;

      case HogType::DETECT:
      {
        HOG_SCORE* pScoreProcess = static_cast<HOG_SCORE*>(mpScoreProcess);

        lStatus = pScoreProcess->Start();
        lStatus |= pScoreProcess->Wait();
      }
      break;
    }
    if(APEXCV_SUCCESS != lStatus)
    {
      lStatus = APEXCV_ERROR_ACF_PROC_EXEC;
    }
  }

  return lStatus;
}

/**********************************
 * APU HOG Descriptors Computation
 *********************************/
APEXCV_LIB_RESULT Hog::GetDescriptors(const vsdk::UMat &acSrc, vsdk::UMat &aDst)
{
  int inRows = (acSrc.rows + 1) * DefaultParams.winStride.width;
  int inCols = (acSrc.cols / N_BINS + 1) * DefaultParams.winStride.height;
  int outRows = (inRows - (DefaultParams.winPixels.height - DefaultParams.winStride.height)) / DefaultParams.winStride.height;
  int outCols = (inCols - (DefaultParams.winPixels.width - DefaultParams.winStride.width)) / DefaultParams.winStride.width;
  int descriptorSize = GetDescriptorSize();

  // Allocate output
  aDst = vsdk::UMat(outRows, outCols * descriptorSize, VSDK_CV_8UC1);

  vsdk::Mat outMat = aDst.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
  vsdk::Mat blockMat = acSrc.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
  if(nullptr == blockMat.data)
  {
    return APEXCV_ERROR_INVALID_ARGUMENT;
  }
  if(nullptr == outMat.data)
  {
    return APEXCV_ERROR_MEMORY_ALLOCATION_FAILED;
  }

  // Build descriptor by concatenating block histograms column-wise
  for(int row = 0; row < outRows; ++row)
  {
    for(int col = 0; col < outCols; ++col)
    {
      // Pointer to output descriptor
      uint8_t* pOut = outMat.data + row * outMat.step[0] + col * descriptorSize;

      // Pointer to block histogram in upper left corner of detection window
      uint8_t* pBlock = blockMat.data + row * blockMat.step[0] + col * N_BINS;

      // Iterate over histograms in the detection window,
      // Assume block stride == block size
      for(int x = 0; x < DefaultParams.winBlocks.width; ++x)
      {
        int colOffset = x * DefaultParams.winStridePerBlock.width * N_BINS;

        for(int y = 0; y < DefaultParams.winBlocks.height; ++y)
        {
          int rowOffset = y * DefaultParams.winStridePerBlock.height * blockMat.step[0];

          memcpy(pOut, pBlock + colOffset + rowOffset, N_BINS);
          pOut += N_BINS;
        }
      }
    }
  }

  return APEXCV_SUCCESS;
}


/*********************
 * Select APEX to run
 ********************/
APEXCV_LIB_RESULT Hog::SelectApexCore(int32_t aApexId)
{
  APEXCV_LIB_RESULT lResult = APEXCV_SUCCESS;
  if(!mInit)
  {
    lResult = APEXCV_ERROR_OBJECT_ISNOT_INITIALIZED;
  }
  else
  {
    HOG_SCORE* pScoreProcess = (HOG_SCORE*) mpScoreProcess;
    HOG_DESCRIPTOR* pDescriptorProcess = (HOG_DESCRIPTOR*) mpDescriptorProcess;

    ACF_Process_APU* pProcess = (HogType::DETECT == mType) ?
      static_cast<ACF_Process_APU*>(pScoreProcess) : static_cast<ACF_Process_APU*>(pDescriptorProcess);

    if(pProcess->SelectApuConfiguration(ACF_APU_CFG__DEFAULT, aApexId))
    {
       lResult = APEXCV_ERROR_ACF_PROC_SELECTAPUCONFIG;
    }
    else
    {
      mApexId = aApexId;
    }
  }

  return lResult;
}


/**************************************************
 * Set the linear SVM classifier decision function
 *************************************************/
APEXCV_LIB_RESULT Hog::SetSVM(const vsdk::UMat& acSVM)
{
  APEXCV_LIB_RESULT lStatus = APEXCV_SUCCESS;
  const int cSvmSize = GetDescriptorSize();

  if(!mInit)
  {
    lStatus = APEXCV_ERROR_OBJECT_ISNOT_INITIALIZED;
  }
  else if((HogType::DETECT != mType) 
        || size_t(cSvmSize + 1) != acSVM.total()
        || VSDK_CV_64FC1 != acSVM.type())

  {
    lStatus = APEXCV_ERROR_INVALID_ARGUMENT;
  }
  else
  {

    vsdk::Mat doubleSVM_Mat = acSVM.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
    double *pDoubleSVM = (double*)doubleSVM_Mat.data;

    if(nullptr == pDoubleSVM)
    {
      lStatus = APEXCV_ERROR_INTERNAL_ERROR;
    }
    else
    {
      vsdk::Mat fixSVM_Mat = mSVM.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
      int8_t *pFixSVM = (int8_t*)fixSVM_Mat.data;

      for(int i = 0; i < cSvmSize; ++i)
      {
        // Convert weights from double to Q0.7
        pFixSVM[i] = (int8_t)(pDoubleSVM[i] * 128.0 - 0.5);
      }

      // Convert offset from double to Q4.11
      const int16_t cOffset = (int16_t)(pDoubleSVM[cSvmSize] * 2048.0 - 0.5);
      memcpy(&pFixSVM[cSvmSize], &cOffset, 2);
    }
  }

  return lStatus;
}

/***************************************
 * Get fixed-point SVM decision function
 ***************************************/
APEXCV_LIB_RESULT Hog::GetSVM(vsdk::UMat& aSVM, int16_t& aOffset)
{
  if(nullptr == mSVM.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data)
  {
    return APEXCV_ERROR_OBJECT_ISNOT_INITIALIZED;
  }

  size_t svmSize = GetDescriptorSize();

  aSVM = vsdk::UMat(mSVM,
      vsdk::Rect(0, 0, DefaultParams.svm.height, DefaultParams.svm.width));

  if(nullptr == aSVM.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data)
  {
    return APEXCV_ERROR_MEMORY_ALLOCATION_FAILED;
  }

  // Copy 16-bit offset
  vsdk::Mat SVM_Mat = mSVM.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
  int8_t *pSVM = (int8_t*)SVM_Mat.data;
  memcpy(&aOffset, &pSVM[svmSize], 2);

  return APEXCV_SUCCESS;
}

/**************************************************
 * Get the number of elements in the HOG descriptor
 **************************************************/
size_t Hog::GetDescriptorSize()
{
  return DefaultParams.svm.width * DefaultParams.svm.height;
}

}  /* namespace apex */
