/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2016 Freescale Semiconductor
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
*  \section fcw_desc Description
*
*  This file contains the message types that this application's tasks are capable of handling.
*
***********************************************************************************/

#ifndef GDCLDWMESSAGES_HPP
#define GDCLDWMESSAGES_HPP

/***************************************************************************/
/* Include files. */
#include <icp_data.h>
#include <icp_types.h>
#include <communications.hpp>
#include <umat.hpp>
#include <oal.h>
#include <ShowMessage.h>
#include <global_errors.h>

//**************************************************************************/
// Literals & definitions
const int32_t MESSAGE_ID_BASE = 0;

const int32_t MESSAGE_ID_GDC_LDW_IMAGE_REQUEST = MESSAGE_ID_BASE; /* struct GdcLdw2ImageReply */

/**
 * \brief Message requesting an image from the input task, sent to FileReaderTask or ImageSensorTask
 *
 */
struct GdcLdw_ImageRequest : public MessageBase
{
   int32_t mDetectionTaskQueueId;
};


const int32_t MESSAGE_ID_GDC_LDW_IMAGE_REPLY = MESSAGE_ID_BASE + 1; /* struct GdcLdw2ImageReply */

/**
 * \brief Message providing an image to a processing task, sent to DetectionTask
 *
 */
struct GdcLdw_ImageReply : public MessageBase
{
   vsdk::UMat  mSceneRgb24_umat;
   const void* mpcSceneRgb24_umat_data;
   vsdk::UMat  mScene_umat;
   const void* mpcScene_umat_data;
   uint32_t    mSceneCounter;
   uint64_t    mTicks[2];  // [0] Entry, [1] Return
};


const int32_t MESSAGE_ID_GDC_LDW_RESET_REQUEST = MESSAGE_ID_BASE +2 ; /* struct GdcLdw2ResetRequest */

/**
 * \brief Message requesting a reset of the application.
 *
 */
struct GdcLdw_ResetRequest : public MessageBase
{
};


const int32_t MESSAGE_ID_GDC_LDW_DETECTION_DATA = MESSAGE_ID_BASE + 3; /* struct GdcLdw2DisplayRequest */

/**
 * \brief Message requesting content be displayed to output device, sent to the DisplayTask
 *
 */
struct GdcLdw_DetectionData : public MessageBase
{
   vsdk::UMat  mSceneRgb24_umat;
   const void* mpcSceneRgb24_umat_data;
   vsdk::UMat  mScene_umat;
   const void* mpcScene_umat_data;
   uint32_t    mSceneCounter;
   void*       mhTopViewImage;
   ICP_Line    mLeftLane;
   ICP_Line    mRightLane;
   bool        mWeDepartedLeft;
   bool        mWeDepartedRight;
   bool        mNoLanesFound;
   uint64_t    mTicks[gcApexCvEndTick];  // [0] Entry, [1] Return

};


const int32_t MESSAGE_ID_GDC_LDW_COMPOSITION_DATA = MESSAGE_ID_BASE + 8; /* struct GdcFt_CompositionData */

/**
 * \brief Data containing output of composition step.
 *
 */
struct GdcLdw_CompositionData : public MessageBase
{
   vsdk::UMat  mSceneRgb24_umat;
   const void* mpcSceneRgb24_umat_data;
};


const int32_t MESSAGE_ID_GDC_LDW_EXIT_REQUEST = MESSAGE_ID_BASE + 5; /* struct GdcLdw2ExitRequest */

/**
 * \brief Message requesting that the task should close/exit
 *
 */
struct GdcLdw_ExitRequest : public MessageBase
{
};


#endif /* GDCLDWMESSAGES_HPP */



