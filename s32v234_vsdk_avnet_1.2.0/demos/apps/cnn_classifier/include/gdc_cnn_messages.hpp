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

#ifndef GDCCNNMESSAGES_HPP
#define GDCCNNMESSAGES_HPP

/***************************************************************************/
/* Include files. */
#include "gdc_cnn_configuration.hpp"
#include "gdc_cnn_types.hpp"
#include <communications.hpp>
#include "global_errors.h"
#include <string>
#include <stdint.h>


//**************************************************************************/
// Literals & definitions
const int32_t MESSAGE_ID_BASE = 0;



const int32_t MESSAGE_ID_GDC_CNN_IMAGE_REQUEST = MESSAGE_ID_BASE; /* struct GdcCnn_ImageRequest */

/**
 * \brief Message requesting an image from the input task, sent to FileReaderTask or ImageSensorTask
 *
 */
struct GdcCnn_ImageRequest : public MessageBase
{
};


const int32_t MESSAGE_ID_GDC_CNN_IMAGE_REPLY = MESSAGE_ID_BASE + 1; /* struct GdcCnn_ImageReply */

/**
 * \brief Message providing an image to a processing task, sent to BsdTask
 *
 */
struct GdcCnn_ImageReply : public MessageBase
{
   cv::Mat             mSceneMat;
   uint32_t            mSceneCounter;
   std::string         mSceneName;
   uint64_t            mTicks[gcApexCvEndTick];      // [0] Entry, [1] Return
};


const int32_t MESSAGE_ID_GDC_RESIZE_DATA = MESSAGE_ID_BASE + 2; /* struct GdcCnn_ResizeData */

/**
 * \brief Message providing an image to a processing task, sent to BsdTask
 *
 */
struct GdcCnn_ResizeData : public MessageBase
{
   cv::Mat             mSceneMat;
   uint32_t            mSceneCounter;
   std::string         mSceneName;
   uint64_t            mSceneTicks[2];      // [0] Entry, [1] Return
   vsdk::UMat          mPaddedScene_umat;
   const void*         mpcPaddedScene_umat_data;
   uint64_t            mTicks[gcApexCvEndTick];      // [0] Entry, [1] After Resize [2] Return

};


const int32_t MESSAGE_ID_GDC_CNN_CLASSIFIER_DATA = MESSAGE_ID_BASE + 3; /* struct GdcCnn_ClassifierData */

/**
 * \brief Data containing output of detector step.
 *
 */
struct GdcCnn_ClassifierData : public MessageBase
{
   cv::Mat             mSceneMat;
   uint32_t            mSceneCounter;
   uint64_t            mSceneTicks[2];       // [0] Entry, [1] Return
   uint64_t            mResizeTicks[3];      // [0] Entry, [1] After Resize [2] Return
   uint16_t            mClassificationIndexes_Local[gcSqueezeNet_NumberOfClassifications];
   std::string         mClassificationLabels_Local[gcSqueezeNet_NumberOfClassifications];
   float               mClassificationScores_Local[gcSqueezeNet_NumberOfClassifications];
   int32_t             mClassificationIndex_Reference;
   std::string         mClassificationLabel_Reference;
   uint64_t            mTicks[gcApexCvEndTick];      // [0] Entry, [1] Before SqueezeNet, [2] After SqueezeNet, [3] Return
};


const int32_t MESSAGE_ID_GDC_CNN_COMPOSITION_DATA = MESSAGE_ID_BASE + 4; /* struct GdcCnn_ClassifierData */

/**
 * \brief Data containing output of detector step.
 *
 */
struct GdcCnn_CompositionData : public MessageBase
{
   void*               mpComposition;
   icp::DataDescriptor mCompositionDd;
   uint32_t            mRightY;
   uint32_t            mSceneCounter;
   uint64_t            mTicks[gcApexCvEndTick];      // [0] Entry, [1] Return
};


const int32_t MESSAGE_ID_GDC_CNN_RESET_REQUEST = MESSAGE_ID_BASE + 5 ; /* struct GdcCnn_ResetRequest */

/**
 * \brief Message requesting a reset of the application.
 *
 */
struct GdcCnn_ResetRequest : public MessageBase
{
};


const int32_t MESSAGE_ID_GDC_CNN_EXIT_REQUEST = MESSAGE_ID_BASE + 6; /* struct GdcCnn_ExitRequest */

/**
 * \brief Event indicating that the input task is ready to receive requests.
 *
 */
struct GdcCnn_ExitRequest : public MessageBase
{
};


const int32_t MESSAGE_ID_GDC_CNN_RETURN_CODE = MESSAGE_ID_BASE + 7; /* struct GdcCnn_ReturnCode */

/**
 * \brief Data containing a return code.
 *
 */
struct GdcCnn_ReturnCode : public MessageBase
{
   LIB_RESULT mLibResult;
};


#endif /* GDCCNNMESSAGES_HPP */



