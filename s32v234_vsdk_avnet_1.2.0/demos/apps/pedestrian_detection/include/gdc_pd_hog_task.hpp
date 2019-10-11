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

#ifndef GDCPEDESTRIANDETECTIONHOGTASK_HPP
#define GDCPEDESTRIANDETECTIONHOGTASK_HPP

#include "gdc_pd_types.hpp"
#include "gdc_pd_types_dummy.hpp"
#include "gdc_pd_types_verification.hpp"
#if !defined(__CYGWIN__) && !defined(_WIN32)
#include "apexcv_pro_hog.h"
#endif
#include "hog_ref.h"
#include "global_errors.h"
#include <stdint.h>



//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.

/*
 *  \brief GdcPd_HogTask
 *
 */
class GdcPd_HogTask : public GdcPd_Task
{
public:
   GdcPd_HogTask();
   virtual ~GdcPd_HogTask();

   LIB_RESULT Configure(const int32_t                cReceiveMessageQueueId,
                        const int32_t                cCompositionTaskMessageQueueId,
                        const uint32_t               cNumberOfSceneResizes,
                        const GdcPd_FrameSize* const cpcSceneResizes);

   int32_t MismatchCounter_Get() const;

private:
   int32_t mReceiveMessageQueueId;
   int32_t mCompositionTaskMessageQueueId;

   uint32_t            mNumberOfSceneResizes;
   GdcPd_FrameSize     mSceneResizes[gcNumberOfSceneResizes];

   vsdk::UMat mSvmDouble;

   uint32_t mHogScoresWidth[gcNumberOfSceneResizes];
   uint32_t mHogScoresHeight[gcNumberOfSceneResizes];

   vsdk::UMat mHogScores_umat;
   uint32_t   mSceneResizesHogCounters[gcNumberOfSceneResizes];

   apexcv::Hog mHog;
   ref::Hog mHogRef;

   std::vector<ROI> mPedestrianRois;

   bool mWeUseApexHog;
   bool mIsApexHogInitialized;

   // Testing
   GdcPd_OutputFile_PedestrianDetection  mOutputFile;

   char    mOutputReadFilename[gcFileNameSize+1];
   char    mOutputWriteFilename[gcFileNameSize+1];
   bool    mWeReadOutputFile;
   bool    mWeWriteOutputFile;

   bool    mWeDumpDetectionImages;

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
   LIB_RESULT HandleResizeData(GdcPd_ResizeData& rResizeData);

   LIB_RESULT VerifyOutput_PedestrianDetection(const uint32_t        cSceneCounter,
                                               std::vector<ROI>&     rPedestrianRois,
                                               icp::DataDescriptor (&rSceneResizeDd)[gcNumberOfSceneResizes]);

private:
   GdcPd_HogTask( const GdcPd_HogTask& );            // Do not implement
   GdcPd_HogTask& operator=( const GdcPd_HogTask& ); // Do not implement
};

#endif /* GDCPEDESTRIANDETECTIONHOGTASK_HPP */


