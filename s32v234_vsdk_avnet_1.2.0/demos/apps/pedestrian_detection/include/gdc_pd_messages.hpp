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

#ifndef GDCPEDESTRIANDETECTIONMESSAGES_HPP
#define GDCPEDESTRIANDETECTIONMESSAGES_HPP

/***************************************************************************/
/* Include files. */
#include "gdc_pd_configuration.hpp"
#include "gdc_pd_types.hpp"
#include "acf_process_apu.h"
#include <communications.hpp>
#include "global_errors.h"
#include <stdint.h>


//**************************************************************************/
// Literals & definitions
const int32_t MESSAGE_ID_BASE = 0;



const int32_t MESSAGE_ID_GDC_PD_IMAGE_REQUEST = MESSAGE_ID_BASE; /* struct GdcPd_ImageReply */

/**
 * \brief Message requesting an image from the input task, sent to FileReaderTask or ImageSensorTask
 *
 */
struct GdcPd_ImageRequest : public MessageBase
{
};


const int32_t MESSAGE_ID_GDC_PD_IMAGE_REPLY = MESSAGE_ID_BASE + 1; /* struct GdcPd_ImageReply */

/**
 * \brief Message providing an image to a processing task, sent to BsdTask
 *
 */
struct GdcPd_ImageReply : public MessageBase
{
   vsdk::UMat  mSceneRgb24_umat;
   const void* mpcSceneRgb24_umat_data;
   vsdk::UMat  mSceneRgb32_umat;
   const void* mpcSceneRgb32_umat_data;
   uint32_t    mSceneCounter;
   uint64_t    mTicks[gcApexCvEndTick];  // [0] Entry, [1] Return
};


const int32_t MESSAGE_ID_GDC_PD_DISPLAY_REQUEST = MESSAGE_ID_BASE + 2; /* struct GdcPd_DisplayRequest */

/**
 * \brief Message requesting content be displayed to output device, sent to the DisplayTask
 *
 */
struct GdcPd_DisplayRequest : public MessageBase
{
   void* mpCurrentImage;
};


const int32_t MESSAGE_ID_GDC_PD_RESIZE_DATA = MESSAGE_ID_BASE + 3; /* struct GdcPd_ResizeData */

/**
 * \brief Data containing output of resize step.
 *
 */
struct GdcPd_ResizeData : public MessageBase
{
   vsdk::UMat       mSceneRgb24_umat;
   const void*      mpcSceneRgb24_umat_data;
   vsdk::UMat       mScene_umat;
   const void*      mpcScene_umat_data;
   uint32_t         mSceneCounter;
   uint32_t         mNumberOfSceneResizes;
   vsdk::UMat       mSceneResizes_umat[gcNumberOfSceneResizes];
   const void*      mhSceneResizes_umat_data[gcNumberOfSceneResizes];
   uint64_t         mResizeTicks[gcApexCvEndTick];  // [0] Entry, [1] Return
   uint64_t         mPerResizeTicks[gcNumberOfSceneResizes][2];  // [0] Entry, [1] Return
   AcfProfilingInfo mPerResizeAcfProfilingInfo[gcNumberOfSceneResizes];
};


const int32_t MESSAGE_ID_GDC_PD_HOG_DATA = MESSAGE_ID_BASE + 4; /* struct GdcPd_HogData */

/**
 * \brief Data containing output of hog step.
 *
 */
struct GdcPd_HogData : public MessageBase
{
   vsdk::UMat          mSceneRgb24_umat;
   const void*         mpcSceneRgb24_umat_data;
   vsdk::UMat          mScene_umat;
   const void*         mpcScene_umat_data;
   uint32_t            mSceneCounter;
   std::vector<ROI>    mPedestrianRois;
   uint64_t            mPerResizeTicks[gcNumberOfSceneResizes][2];  // [0] Entry, [1] Return
   AcfProfilingInfo    mPerResizeAcfProfilingInfo[gcNumberOfSceneResizes];
   uint64_t            mHogTicks[gcApexCvEndTick];     // [0] Entry, [1] Return
   uint64_t            mPerHogTicks[gcNumberOfSceneResizes][2];     // [0] Entry, [1] Return
   AcfProfilingInfo    mPerHogAcfProfilingInfo[gcNumberOfSceneResizes];
};


const int32_t MESSAGE_ID_GDC_PD_COMPOSITION_DATA = MESSAGE_ID_BASE + 5; /* struct GdcPd_CompositionData */

/**
 * \brief Data containing output of composition step.
 *
 */
struct GdcPd_CompositionData : public MessageBase
{
   vsdk::UMat  mSceneRgb24_umat;
   const void* mpcSceneRgb24_umat_data;
};


const int32_t MESSAGE_ID_GDC_PD_EXIT_REQUEST = MESSAGE_ID_BASE + 6; /* struct GdcPd_ExitRequest */

/**
 * \brief Request for the tasks to close and exit.
 *
 */
struct GdcPd_ExitRequest : public MessageBase
{
};


const int32_t MESSAGE_ID_GDC_PD_RESET_REQUEST = MESSAGE_ID_BASE + 7 ; /* struct GdcPd_ResetRequest */

/**
 * \brief Message requesting a reset of the application.
 *
 */
struct GdcPd_ResetRequest : public MessageBase
{
};


#endif /* GDCPEDESTRIANDETECTIONMESSAGES_HPP */



