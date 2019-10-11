/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
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

#ifndef GDCFEATURETRACKINGTRACKERTASK_HPP
#define GDCFEATURETRACKINGTRACKERTASK_HPP

#include "gdc_ft_types.hpp"
#include "gdc_ft_types_verification.hpp"
#include "apexcv_pro_lktracker.h"
#include "apexcv_pro_resize.h" // TO USE: replace with the header you need.
#include "lktracker_ref.h"
#include "global_errors.h"
#include <stdint.h>



//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.

/*
 *  \brief GdcFt_TrackerTask
 *
 */
class GdcFt_TrackerTask : public GdcFt_Task
{
public:
   GdcFt_TrackerTask();
   virtual ~GdcFt_TrackerTask();

   LIB_RESULT Configure(const int32_t cReceiveMessageQueueId,
                        const int32_t cCompositionTaskMessageQueueId);

   int32_t MismatchCounter_Get() const;

private:
   int32_t mReceiveMessageQueueId;
   int32_t mCompositionTaskMessageQueueId;

   apexcv::LKTrackerOpticalFlow mLktTrackerOpticalFlow;
   //LKTrackerRef                 mLkTrackerRef;
   bool                         mIsLktTrackerOpticalFlowInitialized;

   uint32_t mImagePyramidSizes[gcDepthOfImagePyramid][5]; // [0] Width, [1] Height, [2] Padded Width, [3] Padded Height, [4] Offset

   vsdk::UMat                mImagePyramid_umat_Current[gcDepthOfImagePyramid];
   vsdk::UMat                mImagePyramid_umat_Previous[gcDepthOfImagePyramid];

   void*                     mhFeatures32S_PreviousOal_Temp;
   icp::Feature32SDescriptor mFeatures32S_Previous_Temp;
   void*                     mhFeatures32S_CurrentOal_Temp;
   icp::Feature32SDescriptor mFeatures32S_Current_Temp;

   void*                         mpTrackedFeatureAllocator;
   GdcFt_TrackedFeatureAllocator mTrackedFeatureAllocator;
   GdcFt_TrackedFeature**        mppTrackedFeatureTemp;          // Temporary buffer used for filtering the currently tracked features
   GdcFt_TrackedFeature**        mppTrackedFeatures;             // Stores the currently tracked features
   int32_t                       mNumberOfTrackedFeatures;       // Number of tracked features in mppTrackedFeatures

   int16_t  mTrackedFeatureStrength_Threshold;
   int16_t  mDetectedFeatureStrength_Threshold;

   bool mWeUseApexLKTrackerOpticalFlow;

   // Testing
   GdcFt_OutputFile_LKTrackerOpticalFlow  mOutputFile;

   char    mOutputReadFilename[gcFileNameSize+1];
   char    mOutputWriteFilename[gcFileNameSize+1];
   bool    mWeReadOutputFile;
   bool    mWeWriteOutputFile;

   int32_t mReadMismatchCounter;

   bool    mIsTaskClosing;
   bool    mIsConfigured;

public:
   /*!*********************************************************************************
    *  @brief Method called in the context of the spawned task, contains initialization code.
    */
   LIB_RESULT TaskOpen();

   /*!*********************************************************************************
    *  @brief Method called in the context of the spawned task, contains the main body.
    */
   LIB_RESULT TaskService();

   /*!*********************************************************************************
    *  @brief Method called in the context of the spawned task, contains the uninitialization code.
    */
   LIB_RESULT TaskClose();

private:
   LIB_RESULT HandlePyramidData(GdcFt_PyramidData& rData);
   LIB_RESULT VerifyOutput_LKTrackerOpticalFlow(const uint32_t cSceneCounter,
                                                icp::Feature32SDescriptor& rFeature32SDescriptor);

private:
   GdcFt_TrackerTask( const GdcFt_TrackerTask& );            // Do not implement
   GdcFt_TrackerTask& operator=( const GdcFt_TrackerTask& ); // Do not implement
};

#endif /* GDCFEATURETRACKINGTRACKERTASK_HPP */


