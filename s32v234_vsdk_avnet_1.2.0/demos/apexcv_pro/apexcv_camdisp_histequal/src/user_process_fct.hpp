/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2017 NXP
* All Rights Reserved
*
*****************************************************************************
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
****************************************************************************/
#ifndef USERPROCESSFCT_H
#define USERPROCESSFCT_H

#include <cstdint>

#include <apexcv_base.h>
#include <apexcv_pro_histogram_equalization.h>

namespace user_utils
{


  /// *************************************************************************
  ///  @brief          User Processing class to compute histogram equalization.
  ///  @details        Object of this class computes the histogram equalization
  ///                  on all 3 channels.
  /// *************************************************************************
  class UserHistEqual
  {
  private:
    apexcv::SplitChannel mSplitChannel;///< object to Split channel
    apexcv::MergeChannel mMergeChannel;///< object to Merge channel
    apexcv::HistogramEqualization mHistEqual[3]; ///< object to compute histogram
    vsdk::UMat mInter[6];///< internal image buffer 
    bool mbInitialized;///< object initialization status


    /// *************************************************************************
    ///  @brief          Initialize the object.
    ///  @details        Initialize the variables and object for processing.
    /// *************************************************************************
    int_fast32_t Initialize(
      vsdk::UMat& aSrc, ///< image source, type is VSDK_CV_8UC3.
      vsdk::UMat& aDst) ///< image source, type is VSDK_CV_8UC3.
    {
      // Initialize variables and objects
      int_fast32_t lRetVal;
      // create the intermediate buffers
      for (int i = 0; i < 6; i++)
      {
        mInter[i] = vsdk::UMat(aSrc.rows, aSrc.cols, VSDK_CV_8UC1);
        (cv::UMat)mInter[i] = cv::Scalar(80);
      }
      // Initialize the objects
      lRetVal = mSplitChannel.Initialize(aSrc, mInter[0], mInter[1], mInter[2]);
      if (APEXCV_SUCCESS != lRetVal)
      {
        printf("Error %d, at %s (line %d)\n", lRetVal, __FILE__, __LINE__);
        goto END;
      }
      for (int i = 0; i < 3; i++)
      {
        lRetVal = mHistEqual[i].Initialize(mInter[i], mInter[i + 3]);
        if (APEXCV_SUCCESS != lRetVal)
        {
          printf("Error %d, at %s (line %d)\n", lRetVal, __FILE__, __LINE__);
          goto END;
        }
      }
      lRetVal = mMergeChannel.Initialize(mInter[3], mInter[4], mInter[5], aDst);
      if (APEXCV_SUCCESS != lRetVal)
      {
        printf("Error %d, at %s (line %d)\n", lRetVal, __FILE__, __LINE__);
        goto END;
      }

      
      mbInitialized = true;
      printf("UserHistEqual Initialized\n");
      END:
      return lRetVal;
    }

  public:
    UserHistEqual() : mbInitialized(false)
    {
      printf("UserHistEqual Created\n");
    };

    /// *************************************************************************
    ///  @brief          Preform the processing.
    ///  @details        Process the image source and returns the average intensity.
    /// *************************************************************************
    int_fast32_t Process(vsdk::UMat& aSrc, vsdk::UMat& aDst)
    {
      APEXCV_LIB_RESULT lRetVal;
      // Initialize if not done already
      if (!mbInitialized)
      {
        lRetVal = Initialize(aSrc, aDst);
        if (APEXCV_SUCCESS != lRetVal)
        {
          goto EXIT;
        }
      }

      // Split channels
      lRetVal = mSplitChannel.ReconnectIO(aSrc, mInter[0], mInter[1], mInter[2]);
      if (APEXCV_SUCCESS != lRetVal)
      {
        printf("Error %d, at %s (line %d)\n", lRetVal, __FILE__, __LINE__);
        goto EXIT;
      }
      lRetVal = mSplitChannel.Process();
      if (APEXCV_SUCCESS != lRetVal)
      {
        printf("Error %d, at %s (line %d)\n", lRetVal, __FILE__, __LINE__);
        goto EXIT;
      }
      // Compute histogram equalization
      for (int i = 0; i < 3; i++)
      {
        lRetVal = mHistEqual[i].Process();
        if (APEXCV_SUCCESS != lRetVal)
        {
          printf("Error %d, at %s (line %d)\n", lRetVal, __FILE__, __LINE__);
          goto EXIT;
        }
      }
      // Merge channels
      lRetVal = mMergeChannel.ReconnectIO(mInter[3], mInter[4], mInter[5], aDst);
      if (APEXCV_SUCCESS != lRetVal)
      {
        printf("Error %d, at %s (line %d)\n", lRetVal, __FILE__, __LINE__);
        goto EXIT;
      }
      lRetVal = mMergeChannel.Process();
      if (APEXCV_SUCCESS != lRetVal)
      {
        printf("Error %d, at %s (line %d)\n", lRetVal, __FILE__, __LINE__);
        goto EXIT;
      }
      EXIT:
      return lRetVal;
    }

  };







}

#endif
