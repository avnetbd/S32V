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

/*!*********************************************************************************
*  @file
*  @brief Message definitions used in inter-task communications.
*
*
*  This file contains the message types that this application's tasks are capable of handling.
*
***********************************************************************************/

#ifndef GDCFEATURETRACKINGMESSAGESHPP
#define GDCFEATURETRACKINGMESSAGESHPP

/***************************************************************************/
/* Include files. */
#include "gdc_ft_configuration.hpp"
#include "gdc_ft_types.hpp"
#include "icp_feature.h"
#include "umat.hpp"
#include <communications.hpp>
#include "global_errors.h"
#include <stdint.h>


//**************************************************************************/
// Literals & definitions
const int32_t MESSAGE_ID_BASE = 0;



const int32_t MESSAGE_ID_GDC_FT_IMAGE_REQUEST = MESSAGE_ID_BASE; /* struct GdcFt_ImageRequest */

/**
 * \brief Message requesting an image from the input task, sent to FileReaderTask or ImageSensorTask
 *
 */
struct GdcFt_ImageRequest : public MessageBase
{
};


const int32_t MESSAGE_ID_GDC_FT_IMAGE_REPLY = MESSAGE_ID_BASE + 1; /* struct GdcFt_ImageReply */

/**
 * \brief Message providing an image to a processing task, sent to BsdTask
 *
 */
struct GdcFt_ImageReply : public MessageBase
{
   vsdk::UMat  mSceneRgb24_umat;
   const void* mpcSceneRgb24_umat_data;
   vsdk::UMat  mScene_umat;
   const void* mpcScene_umat_data;
   uint32_t    mSceneCounter;
   uint64_t    mTicks[2];  // [0] Entry, [1] Return
};


const int32_t MESSAGE_ID_GDC_FT_RESET_REQUEST = MESSAGE_ID_BASE +2 ; /* struct GdcFt_ResetRequest */

/**
 * \brief Message requesting a reset of the application.
 *
 */
struct GdcFt_ResetRequest : public MessageBase
{
};


const int32_t MESSAGE_ID_GDC_FT_EXIT_REQUEST = MESSAGE_ID_BASE + 3; /* struct GdcFt_ExitRequest */

/**
 * \brief Event indicating that the input task is ready to receive requests.
 *
 */
struct GdcFt_ExitRequest : public MessageBase
{
};


const int32_t MESSAGE_ID_GDC_FT_RETURN_CODE = MESSAGE_ID_BASE + 4; /* struct GdcFt_ReturnCode */

/**
 * \brief Data containing a return code.
 *
 */
struct GdcFt_ReturnCode : public MessageBase
{
   LIB_RESULT mLibResult;
};


const int32_t MESSAGE_ID_GDC_FT_DETECTOR_DATA = MESSAGE_ID_BASE + 5; /* struct GdcFt_DetectorData */

/**
 * \brief Data containing output of detector step.
 *
 */
struct GdcFt_DetectorData : public MessageBase
{

   vsdk::UMat             mSceneRgb24_umat;
   const void*            mpcSceneRgb24_umat_data;
   vsdk::UMat             mScene_umat;
   const void*            mpcScene_umat_data;
   uint32_t               mSceneCounter;
   void*                  mhFeatureDescriptorOal;
   icp::FeatureDescriptor mFeatureDescriptor;
   bool                   mWeDetectedFeatures;
   uint64_t               mTicks[gcApexCvEndTick];    // [0] Entry, [1] before apex, [2] after apex, [3] Return
};


const int32_t MESSAGE_ID_GDC_FT_PYRAMID_DATA = MESSAGE_ID_BASE + 6; /* struct GdcFt_PyramidData */

/**
 * \brief Data containing output of pyramid step.
 *
 */
struct GdcFt_PyramidData : public MessageBase
{

   vsdk::UMat             mSceneRgb24_umat;
   const void*            mpcSceneRgb24_umat_data;
   vsdk::UMat             mScene_umat;
   const void*            mpcScene_umat_data;
   uint32_t               mSceneCounter;
   void*                  mhFeatureDescriptorOal;
   icp::FeatureDescriptor mFeatureDescriptor;
   bool                   mWeDetectedFeatures;
   vsdk::UMat             mImagePyramid_umat[gcDepthOfImagePyramid];
   uint64_t               mTicks[gcApexCvEndTick];    // [0] Entry, [1] before apex, [2] after apex, [3] Return
};


const int32_t MESSAGE_ID_GDC_FT_TRACKER_DATA = MESSAGE_ID_BASE + 7; /* struct GdcFt_TrackerData */

/**
 * \brief Data containing output of klt step.
 *
 */
struct GdcFt_TrackerData : public MessageBase
{

   vsdk::UMat           mSceneRgb24_umat;
   const void*          mpcSceneRgb24_umat_data;
   vsdk::UMat           mScene_umat;
   const void*          mpcScene_umat_data;
   uint32_t             mSceneCounter;
   GdcFt_TrackedFeature mTrackedFeatures[gcMaxNumberOfTrackedFeatures];
   uint32_t             mNumberOfTrackedFeatures;
   uint32_t             mNumberOfDetectedFeatures;
   uint64_t             mTicks[gcApexCvEndTick];    // [0] Entry, [1] before apex, [2] after apex, [3] Return
};


const int32_t MESSAGE_ID_GDC_FT_COMPOSITION_DATA = MESSAGE_ID_BASE + 8; /* struct GdcFt_CompositionData */

/**
 * \brief Data containing output of composition step.
 *
 */
struct GdcFt_CompositionData : public MessageBase
{
   vsdk::UMat  mSceneRgb24_umat;
   const void* mpcSceneRgb24_umat_data;
   uint32_t    mSceneCounter;
   uint64_t    mTicks[gcApexCvEndTick];    // [0] Entry, [1] before apex, [2] after apex, [3] Return
};


#endif /* GDCFEATURETRACKINGMESSAGESHPP */



