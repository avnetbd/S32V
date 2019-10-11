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

namespace user_utils
{


  /// *************************************************************************
  ///  @brief          User Processing class to compute image intensity.
  ///  @details        Object of this class computes the intensity of the second
  ///                  channel (green) of a 3-channel image (BGR).
  /// *************************************************************************
  class UserChannelIntensity
  {
  private:
    apexcv::ExtractChannel mExtractChannel2;///< object to extract channel
    apexcv::Histogram mHistogram; ///< object to compute histogram
    vsdk::UMat mHist;///< internal image buffer to contain the histogram results
    vsdk::UMat mInter;///< internal image buffer containing the extracted channel
    bool mbInitialized;///< object initialization status


    /// *************************************************************************
    ///  @brief          Initialize the object.
    ///  @details        Initialize the variables and object for processing.
    /// *************************************************************************
    int_fast32_t Initialize(
      vsdk::UMat& aSrc) ///< image source, type is VSDK_CV_8UC3.
    {
      // Initialize variables and objects
      int_fast32_t lRetVal = 0;
      mInter = vsdk::UMat(aSrc.rows, aSrc.cols, VSDK_CV_8UC1);
      mHist = vsdk::UMat(1, 256, VSDK_CV_32SC1);
      lRetVal |= mExtractChannel2.Initialize(aSrc, 2, mInter);
      lRetVal |= mHistogram.Initialize(mInter, mHist);
      if (0 != lRetVal)
      {
        printf("Error %d, at %s (line %d)\n", lRetVal, __FILE__, __LINE__);
      }
      else
      {
        mbInitialized = true;
        printf("UserChannelIntensity Initialized\n");
      }
      return lRetVal;
    }

  public:
    UserChannelIntensity() :mbInitialized(false)
    {
      printf("UserChannelIntensity Created\n");
    };

    /// *************************************************************************
    ///  @brief          Preform the processing.
    ///  @details        Process the image source and returns the average intensity.
    /// *************************************************************************
    int_fast32_t Process(vsdk::UMat& aSrc, uint_fast8_t& aIntensity)
    {
      int_fast32_t lRetVal = 0;
      // Initialize if not done already
      if (!mbInitialized)
      {
        lRetVal |= Initialize(aSrc);
      }
      if (0 != lRetVal)
      {
        goto EXIT;
      }

      // Extract channel
      lRetVal |= mExtractChannel2.ReconnectIO(aSrc, mInter);
      lRetVal |= mExtractChannel2.Process();
      // Compute histogram
      lRetVal |= mHistogram.Process();
      if (0 != lRetVal)
      {
        goto EXIT;
      }
      // compute intensity
      {
        vsdk::Mat mat = mHist.getMat(OAL_USAGE_CACHED | vsdk::ACCESS_RW);
        uint_fast32_t lSum = 0;
        uint_fast32_t lWAcc = 0;
        for (int i = 0; i < mHist.cols; i++)
        {
          uint_fast32_t lVal = mat.at<uint32_t>(i);
          lSum += lVal;
          lWAcc += i*lVal;
        }
        uint_fast32_t lTemp = lWAcc / lSum;
        aIntensity = (uint_fast8_t)lTemp;
      }
      EXIT:
      return lRetVal;
    }

  };



  /// *************************************************************************
  ///  @brief          User Processing class to invert a channel.
  ///  @details        Object of this class inverts the intensity of the channel
  ///                  selected in arguments.
  /// *************************************************************************
  class UserChannelInvert
  {
  private:
    apexcv::SplitChannel mSplitChannel;///< object to split the channels
    apexcv::MergeChannel mMergeChannel;///< object to merge the channels
    apexcv::BitwiseNOT mBitwiseNOT;///< object to invert the intensity
    vsdk::UMat mInter1;///< internal image buffer of a single channel
    vsdk::UMat mInter2;///< internal image buffer of a single channel
    vsdk::UMat mInter3;///< internal image buffer of a single channel
    vsdk::UMat mInter4;///< internal image buffer holding the inverted channel
    bool mbInitialized;///< Object initialization status


    /// *************************************************************************
    ///  @brief          Initialize the object.
    ///  @details        Initialize the variables and object for processing.
    /// *************************************************************************
    int_fast32_t Initialize(vsdk::UMat& aSrc, int_fast8_t aChannelIndex, vsdk::UMat& aDst)
    {
      int_fast32_t lRetVal = 0;
      mInter1 = vsdk::UMat(aSrc.rows, aSrc.cols, VSDK_CV_8UC1);
      mInter2 = vsdk::UMat(aSrc.rows, aSrc.cols, VSDK_CV_8UC1);
      mInter3 = vsdk::UMat(aSrc.rows, aSrc.cols, VSDK_CV_8UC1);
      mInter4 = vsdk::UMat(aSrc.rows, aSrc.cols, VSDK_CV_8UC1);
      lRetVal |= mSplitChannel.Initialize(aSrc, mInter1, mInter2, mInter3);
      lRetVal |= mBitwiseNOT.Initialize(mInter1, mInter4);
      lRetVal |= mMergeChannel.Initialize(mInter1, mInter2, mInter3, aDst);
      if (0 != lRetVal)
      {
        printf("Error %d, at %s (line %d)\n", lRetVal, __FILE__, __LINE__);
      }
      else
      {
        mbInitialized = true;
        printf("UserChannelInvert Initialized\n");
      }
      return lRetVal;
    }


  public:
    // *************************************************************************
    // Constructor. (not documented)
    // *************************************************************************
    UserChannelInvert() :mbInitialized(false)
    {
      printf("UserChannelInvert Created\n");
    };


    /// *************************************************************************
    ///  @brief          Preform the processing.
    ///  @details        Process the image source and returns an image
    ///                  with inverted channel.
    /// *************************************************************************
    int_fast32_t Process(
      vsdk::UMat& aSrc, ///< image source to process, type VSDK_CV_8UC3
      int_fast8_t aChannelIndex, ///< index of the channel to inver, 1, 2 or 3
      vsdk::UMat& aDst) ///< image destination with the inverted channel, type VSDK_CV_8UC3
    {
      int_fast32_t lRetVal = 0;
      // Initialize if not done already
      if (!mbInitialized)
      {
        lRetVal |= Initialize(aSrc, aChannelIndex, aDst);
      }
      if (0 != lRetVal)
      {
        goto EXIT;
      }

      // Prepare split channel with new source data
      lRetVal |= mSplitChannel.ReconnectIO(aSrc, mInter1, mInter2, mInter3);
      switch (aChannelIndex)
      {
        // invert first channel
      case 1:
      {
        // Prepares image content invert and merge 
        lRetVal |= mBitwiseNOT.ReconnectIO(mInter1, mInter4);
        lRetVal |= mMergeChannel.ReconnectIO(mInter4, mInter2, mInter3, aDst);;
        if (0 != lRetVal)
        {
          printf("Error %d, at %s (line %d)\n", lRetVal, __FILE__, __LINE__);
        }
      }break;
      case 2:
      {
        // Prepares image content invert and merge 
        lRetVal |= mBitwiseNOT.ReconnectIO(mInter2, mInter4);
        lRetVal |= mMergeChannel.ReconnectIO(mInter1, mInter4, mInter3, aDst);;
        if (0 != lRetVal)
        {
          printf("Error %d, at %s (line %d)\n", lRetVal, __FILE__, __LINE__);
        }
      }break;
      case 3:
      {
        // Prepares image content invert and merge 
        lRetVal |= mBitwiseNOT.ReconnectIO(mInter3, mInter4);
        lRetVal |= mMergeChannel.ReconnectIO(mInter1, mInter2, mInter4, aDst);;
        if (0 != lRetVal)
        {
          printf("Error %d, at %s (line %d)\n", lRetVal, __FILE__, __LINE__);
        }
      }break;
      default:
        printf("Shouldn't be here, at %s (line %d)\n", aChannelIndex, __FILE__, __LINE__);
        break;
      }
      // Performs the operations, split, invert and merge
      lRetVal |= mSplitChannel.Process();
      lRetVal |= mBitwiseNOT.Process();
      lRetVal |= mMergeChannel.Process();

      EXIT:
      return lRetVal;
    }

  };







}

#endif
