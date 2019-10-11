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

#ifndef GDCFEATURETRACKINGDETECTORTASK_HPP
#define GDCFEATURETRACKINGDETECTORTASK_HPP

#include "gdc_ft_types.hpp"
#include "gdc_ft_types_verification.hpp"
#include <apexcv_pro_gftt_corners.h>
#include "apexcv_pro_resize.h" // TO USE: replace with the header you need.
#include <gftt_corners_ref.h>
#include "global_errors.h"
#include <stdint.h>



//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.

/*
 *  \brief GdcFt_DetectorTask
 *
 */
class GdcFt_DetectorTask : public GdcFt_Task
{
public:
   GdcFt_DetectorTask();
   virtual ~GdcFt_DetectorTask();

   LIB_RESULT Configure(const int32_t cReceiveMessageQueueId,
                        const int32_t cPyramidTaskMessageQueueId);

private:
   int32_t mReceiveMessageQueueId;
   int32_t mPyramidTaskMessageQueueId;

   apexcv::GFTTCorners    mGfttCorners;
   ref::GFTTCorners       mGfttCornersRef;
   bool                   mIsGfttCornersInitialized;

   void*                  mhFeatureDescriptorOal;
   icp::FeatureDescriptor mFeatureDescriptor;

   int                    mGfttCorners_UseHarris;
   int                    mHarrisCornersQualityLevel;
   int                    mHarrisCornersMinDistance;
   int                    mHarrisCornersCornerCoef;
   int                    mHarrisCornersThreshold;
   int                    mHarrisCornersBoxSize;
   int                    mHarrisCornersNmsRadius;
   int                    mHarrisCornersScaleFactor;
   int                    mHarrisCornersMinCorners;
   int                    mHarrisCornersMaxCorners;

   bool    mWeUseApexGfttCorners;

   // Testing
   GdcFt_OutputFile_HarrisCorners  mOutputFile;

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
   LIB_RESULT HandleImageReply(GdcFt_ImageReply& rReply);

private:
   GdcFt_DetectorTask( const GdcFt_DetectorTask& );            // Do not implement
   GdcFt_DetectorTask& operator=( const GdcFt_DetectorTask& ); // Do not implement
};

#endif /* GDCFEATURETRACKINGDETECTORTASK_HPP */


