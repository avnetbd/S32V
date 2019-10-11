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
 *  @brief Private API of a processing task which forms the Lane Departure Warning algorithm.
 *
 *  This task receives images from an input task and processes them.
 *
 ***********************************************************************************/

#ifndef GDCLDWDETECTIONTASK_HPP
#define GDCLDWDETECTIONTASK_HPP

#include <gdc_ldw_types.hpp>
#include <icp_types.h>
#include <icp_data.h>
#include <icp_datatype.h>
#include <oal.h>
#include "global_errors.h"
#include <stdint.h>
#include "toggle_regression.h"

/* 
 * Place these lines after #include "toggle_regression.h"
 * Used to toggle between two separate graphs
 * See toggle_regression.h for details
 */
#ifdef _TOGGLE_REG_GRAPH_
#include "ACF_FILTERS_REG.hpp"
#endif // end: #ifdef _TOGGLE_REG_GRAPH_
#ifndef _TOGGLE_REG_GRAPH_
#include "ACF_FILTERS_NO_REG.hpp"
#endif // end: #ifndef _TOGGLE_REG_GRAPH_


//**************************************************************************/
// Literals & definitions

enum TurnningState { TURNING_RIGHT, TURNING_LEFT, MID_LANE, NO_LANES };

//**************************************************************************/
// Class declaration.

/**
 * \brief Class that configures and controls this task.
 *
 */
class GdcLdw_DetectionTask : public GdcLdw_Task
{
public:
   GdcLdw_DetectionTask();
   virtual ~GdcLdw_DetectionTask();

   /*!*********************************************************************************
    *  @brief Method called by the application to configure the task.
    */
   LIB_RESULT Configure(int32_t const cSceneWidthInPixels,
                        int32_t const cSceneHeightInPixels,
                        //icp::DataDescriptor const cSceneColorModel,
                        icp::DATATYPE const cSceneColorModel,
                        int32_t const cReceiveMessageQueueId,
                        int32_t const cDisplayMessageQueueId);

private:
   int32_t                    mReceiveMessageQueueId;
   int32_t                    mDisplayMessageQueueId;

   int32_t                    mSceneWidthInPixels;
   int32_t                    mSceneHeightInPixels;
   icp::DATATYPE              mSceneColorModel;

   bool                       mIsTaskClosing;
   bool                       mIsConfigured;
   bool                       mIsRunningWithThreads;

   GdcLdw_LdwConfig          mLdwConfig;

   // Convert to birdeye variables
   vsdk::UMat mVoteThreshold_umat;
   vsdk::UMat mPreviousThreshold_umat;
   vsdk::UMat mBirdEyeSobelMedian_umat;
   vsdk::UMat mLaneData_umat;

   // Used for linear regression
#ifndef _TOGGLE_REG_GRAPH_
   vsdk::UMat mLeftMax_umat;
   vsdk::UMat mRightMax_umat;
   vsdk::UMat mPoints_umat;
#endif


   apexcv::GdcLdw_Remap mRemap;
   vsdk::UMat mBirdEyeLutDelta_umat;
   vsdk::UMat mBirdEyeLutOffset_umat;
   vsdk::UMat mBirdEyeLutBlockOffset_umat;

   // Apply Remap, SOBEL and MEDIAN filter variables

#ifdef _TOGGLE_REG_GRAPH_  // Regression in graph
   ACF_FILTERS_REG           mAcfRegProcess;
#endif // end: #ifdef _TOGGLE_REG_GRAPH_

#ifndef _TOGGLE_REG_GRAPH_ // No regression in graph
   ACF_FILTERS_NO_REG        mAcfNoRegProcess;
#endif // end: #ifndef _TOGGLE_REG_GRAPH_


   // Find lanes variables
   int32_t*                   mpLaneDetectionAccumulator;
   int32_t                    mLaneDetectionAccumulatorSize;

   // Kalman filter variables
   GdcLdw_ConstantValueFilter mLeftAngleKalman;
   GdcLdw_ConstantValueFilter mRightAngleKalman;
   GdcLdw_ConstantValueFilter mCentreKalman;
   GdcLdw_ConstantValueFilter mLaneGapKalman;
   GdcLdw_ConstantValueFilter mCentreKalman2;
   GdcLdw_ConstantValueFilter mLaneGapKalman2;

   // Detect vehicle variables
   ICP_Mat<float>             mBirdEyeToSceneMat;
   ICP_Mat<float>             mTempPointMat1;      // Stored here instead of reallocating in conversion function
   ICP_Mat<float>             mTempPointMat2;      // Stored here instead of reallocating in conversion function

   // Lane Departure thresholds
   int32_t mLdwMaxWarningFrameCount;
   int32_t mLdwWarningWidthRangeInLane;
   int32_t mLdwWarningWidthRangeInWarning;

   // Mapping Variables
   char                       mCalibLoaderFileName[gcStringMaxSize];

   // LDW Status Variables
   bool          mIsLeftWarning;
   bool          mIsRightWarning;
   int           mLeftWarningCounter;
   int           mRightWarningCounter;
   int           mNoLanesCounter;
   TurnningState mOldState;

   int                mReadMismatchCounter;

public:
   /*!*********************************************************************************
    *  @brief Method called in the context of the spawned task, contains initialization code.
    */
   virtual LIB_RESULT TaskOpen();

   /*!*********************************************************************************
    *  @brief Method called in the context of the spawned task, contains the main body.
    */
   virtual LIB_RESULT TaskService();

   /*!*********************************************************************************
    *  @brief Method called in the context of the spawned task, contains the uninitialization code.
    */
   virtual LIB_RESULT TaskClose();

private:
   /*!*********************************************************************************
    *  @brief Method called in TaskService which handle a message type
    */
   LIB_RESULT HandleImageReply(GdcLdw_ImageReply& rReply);

private:
   GdcLdw_DetectionTask( const GdcLdw_DetectionTask& );            // Do not implement
   GdcLdw_DetectionTask& operator=( const GdcLdw_DetectionTask& ); // Do not implement
};

#endif /* GDCLDWDETECTIONTASK_HPP */

