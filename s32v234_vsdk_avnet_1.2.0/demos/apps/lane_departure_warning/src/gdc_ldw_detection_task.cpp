/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
 *
 * Copyright 2013-2017 NXP 
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
 **************************************************************************************************/

/***************************************************************************/
/* Include files. */
#include <gdc_ldw_configuration.hpp>
#include <gdc_ldw_messages.hpp>
#include <gdc_ldw_detection_task.hpp>
#include <gdc_ldw_types.hpp>
//#include "ICL_Dewarp.hpp"
#include <communications.hpp>
#include <oal.h>
#include <algorithm>
#include <stdint.h>
//#include <new>
//#include "ICL_Dewarp.hpp"
#include <apexcv_pro_remap.h>
#include <Kernels_Configuration.h>

//#define __APEXCV_PROFILE__
#include "swt_arm_report.h"
#include <common_time_measure.h>

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/* Externally defined global data */

/***************************************************************************/
/* Locally defined global data */

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
GdcLdw_DetectionTask::GdcLdw_DetectionTask()
    : mReceiveMessageQueueId(0), mDisplayMessageQueueId(0), mSceneWidthInPixels(0), mSceneHeightInPixels(0),
      mSceneColorModel(icp::DATATYPE_08U), mIsTaskClosing(false), mIsConfigured(false),
      //GdcLdw_LdwConfig          mLdwConfig;
      //icp::DataDescriptor         mVoteThresholdCdd;
      //icp::DataDescriptor         mPreviousThresholdCdd;
      //icp::DataDescriptor         mBirdEyeSobelMedianCdd;
      //icp::DataDescriptor         mLaneDataCdd;
      //icp::DataDescriptor  mLeftMaxCdd;
      //icp::DataDescriptor  mRightMaxCdd;
      //icp::DataDescriptor  mPointsCdd;
      //apexcv::GdcLdw_Remap mRemap;
      //icp::DataDescriptor   mBirdEyeLutDeltaCdd;
      //icp::DataDescriptor   mBirdEyeLutOffsetCdd;
      //icp::DataDescriptor   mBirdEyeLutBlockOffsetCdd;
      //ACF_FILTERS_REG           mAcfRegProcess;
      //ACF_FILTERS_NO_REG        mAcfNoRegProcess;
      mpLaneDetectionAccumulator(0), mLaneDetectionAccumulatorSize(0),
      //GdcLdw_ConstantValueFilter mLeftAngleKalman;
      //GdcLdw_ConstantValueFilter mRightAngleKalman;
      //GdcLdw_ConstantValueFilter mCentreKalman;
      //GdcLdw_ConstantValueFilter mLaneGapKalman;
      //GdcLdw_ConstantValueFilter mCentreKalman2;
      //GdcLdw_ConstantValueFilter mLaneGapKalman2;
      //ICP_Mat<float>             mBirdEyeToSceneMat;
      //ICP_Mat<float>             mTempPointMat1;      // Stored here instead of reallocating in conversion function
      //ICP_Mat<float>             mTempPointMat2;      // Stored here instead of reallocating in conversion function
      mLdwMaxWarningFrameCount(0), mLdwWarningWidthRangeInLane(0), mLdwWarningWidthRangeInWarning(0),
      //char                       mCalibLoaderFileName[gcStringMaxSize];
      mIsLeftWarning(false), mIsRightWarning(false), mLeftWarningCounter(0), mRightWarningCounter(0),
      mNoLanesCounter(0),
      //TurnningState mOldState;
      mReadMismatchCounter(0)
{
  memset(&mLdwConfig, 0, sizeof(mLdwConfig));
  memset(&mCalibLoaderFileName, 0, sizeof(mCalibLoaderFileName));
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
GdcLdw_DetectionTask::~GdcLdw_DetectionTask()
{
}

/***************************************************************************/
/**
 * \par Description
 * This function needs to be called prior to "spawning" the task.
 * It allows the application to configure the task.
 *
 **************************************************************************/
LIB_RESULT GdcLdw_DetectionTask::Configure(int32_t const       cSceneWidthInPixels,
                                           int32_t const       cSceneHeightInPixels,
                                           icp::DATATYPE const cSceneColorModel,
                                           int32_t const       cReceiveMessageQueueId,
                                           int32_t const       cDisplayMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mSceneWidthInPixels  = cSceneWidthInPixels;
    mSceneHeightInPixels = cSceneHeightInPixels;
    mSceneColorModel     = cSceneColorModel;

    mReceiveMessageQueueId = cReceiveMessageQueueId;
    mDisplayMessageQueueId = cDisplayMessageQueueId;
    mIsConfigured          = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * This function is used to initialize and reserve any required resources.
 * It is called automatically in a new execution content when the task objects
 * "spawn" function is called.
 **************************************************************************/
LIB_RESULT GdcLdw_DetectionTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_SUCCESS;

  SWT_ARM_LOG_MODULE_FILENAME_REG("GdcLdw_DetectionTask", "laneDetection");

  libResult = GdcLdw_Helper::ReadLdwConfigFromIniFile(gcIniFileName, mLdwConfig);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  mLdwWarningWidthRangeInWarning =
      mLdwConfig.mWarningWidthFromCenter * ((double)mLdwConfig.mWarningHysteresisPct / 100 + 1);
  mLdwWarningWidthRangeInLane = mLdwConfig.mWarningWidthFromCenter;
  mLdwMaxWarningFrameCount    = mLdwConfig.mMaxWarningFrameCount;

  sprintf(mCalibLoaderFileName, "%s", gcFileNameCalibLoad);
  //
  // needs Calibration loader and create LUTs and mapper for groundToWarp
  //

  // Calibration data format
  //
  //        cameraParams.fx         cameraParams.cx        cameraParams.fy         cameraParams.cy
  //       distortParams.p1        distortParams.p2       distortParams.k1        distortParams.k2         distortParams.k3        distortParams.k4        distortParams.k5        distortParams.k6
  //    rotationAngles.tilt    rotationAngles.swing     rotationAngles.pan            cameraHeight
  //  dewarpCameraParams.fx   dewarpCameraParams.cx  dewarpCameraParams.fy   dewarpCameraParams.cy
  //  dewarpImageSize.width  dewarpImageSize.Height
  //         topGroundROI.x          topGroundROI.y     topGroundROI.width     topGroundROI.height
  //     topImageSize.width     topImageSize.height
  // splitViewAngles.theta1  splitViewAngles.theta2   splitViewAngles.phi1    splitViewAngles.phi2
  //printf("\nLDW test (%dx%d)-->(%dx%d)\n\n",gcSceneWidthInPixels, gcSceneHeightInPixels, mLdwConfig.mBirdEyeWidth, mLdwConfig.mBirdEyeHeight);

  libResult |=
      mRemap.GenerateLUTFromCalibLoader(mCalibLoaderFileName, mLdwConfig.mBirdEyeWidth, mLdwConfig.mBirdEyeHeight,
                                        gcSceneWidthInPixels, gcSceneHeightInPixels, gcRemapDestBlockWidth,
                                        gcRemapDestBlockHeight, gcRemapSrcBlockWidth, gcRemapSrcBlockHeight);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  // Generate LUT for remap algorithm.
  if(libResult == LIB_SUCCESS)
  {
    mRemap.RetLUTs(mBirdEyeLutDelta_umat, mBirdEyeLutOffset_umat, mBirdEyeLutBlockOffset_umat);
  }
  //printf("offset:%d, %d, %d\n", mBirdEyeLutOffsetCdd.GetElementDataType(), mBirdEyeLutOffsetCdd.GetElementDimX(),  mBirdEyeLutOffsetCdd.GetElementDimY());
  //printf("delta:%d, %d, %d\n", mBirdEyeLutDeltaCdd.GetElementDataType(), mBirdEyeLutDeltaCdd.GetElementDimX(),  mBirdEyeLutDeltaCdd.GetElementDimY());

  //printf ("chunk (%d,%d)-->(%d,%d)\n", mRemap.valid_config.src_width, mRemap.valid_config.src_height, mRemap.valid_config.dst_width, mRemap.valid_config.dst_height);

  // Allocating memory for graph IO
  mPreviousThreshold_umat.create(1, 1, VSDK_CV_8UC1);

  {
    vsdk::Mat TempMat  = mPreviousThreshold_umat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
    uint8_t*  pPointU8 = (uint8_t*)TempMat.data;
    *pPointU8          = 0;
  }

  mVoteThreshold_umat.create(1, 1, VSDK_CV_8UC1);

  // TODO removed magic number 6, this is after linear regression
  mLaneData_umat.create(2, 6,
                        VSDK_CV_32SC1); // This is supposed to be equivalent to icp::DATATYPE_32U

  mBirdEyeSobelMedian_umat.create(mLdwConfig.mBirdEyeHeight, mLdwConfig.mBirdEyeWidth, gcLdwBirdEyeDataType);

  // Used for additional IO if we are doing linear regression on the ARM.
#ifndef _TOGGLE_REG_GRAPH_
  mLeftMax_umat.create(1, 1, VSDK_CV_8UC1);

  mRightMax_umat.create(1, 1, VSDK_CV_8UC1);

  mPoints_umat.create(gcNumOfAccumalators, gcDataOutStride,
                      VSDK_CV_32SC1); // This is supposed to be equivalent to icp::DATATYPE_32U

  memset(mPoints_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, 0x00,
         gcNumOfAccumalators * gcDataOutStride * sizeof(uint32_t));
#endif // end: #ifndef _TOGGLE_REG_GRAPH_

  // Reference Code
  //   mhLaneDetectionAccumulatorOal = (int32_t*)OAL_MemoryAllocFlag(mBirdEyeSobelMedianCdd.RetWidth() * gcNumOfAccumalators * sizeof(mpLaneDetectionAccumulator[0]),
  //                                                                 OAL_MEMORY_FLAG_CONTIGUOUS |
  //                                                                 OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) |
  //                                                                 OAL_MEMORY_FLAG_ZERO); SHOW_ERROR(mhLaneDetectionAccumulatorOal != 0);
  //
  //   mLaneDetectionAccumulatorSize = mBirdEyeSobelMedianCdd.RetWidth() * gcNumOfAccumalators;

  //initialize kalman filters (angles)
  libResult = mLeftAngleKalman.Initialize(gcLdwKalmanMeasurementNoiseVar, gcLdwKalmanPostErrorVar,
                                          gcLdwAngleKalmanInitialValue);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  libResult = mRightAngleKalman.Initialize(gcLdwKalmanMeasurementNoiseVar, gcLdwKalmanPostErrorVar,
                                           gcLdwAngleKalmanInitialValue);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  libResult =
      mLaneGapKalman.Initialize(gcLdwKalmanMeasurementNoiseVar, gcLdwKalmanPostErrorVar, mLdwConfig.mInitialKLGap);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  libResult =
      mCentreKalman.Initialize(gcLdwKalmanMeasurementNoiseVar, gcLdwKalmanPostErrorVar, mLdwConfig.mInitialKCentre);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  libResult =
      mLaneGapKalman2.Initialize(gcLdwKalmanMeasurementNoiseVar, gcLdwKalmanPostErrorVar, mLdwConfig.mInitialKLGap);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  libResult =
      mCentreKalman2.Initialize(gcLdwKalmanMeasurementNoiseVar, gcLdwKalmanPostErrorVar, mLdwConfig.mInitialKCentre);
  SHOW_ERROR(libResult == LIB_SUCCESS);

  GDC_TRACE;

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 * See ICP_SDK_Platform_Software_UserGuide document for further details about:
 *    1. OAL Task State machine
 *    2. Communication infrastructure.
 *    3. File System Abstraction
 *    4. Memory Abstraction
 *
 * See Image_Cognition_Library_User_Guide document for further details about:
 *    1. ICP Image Framework
 *    2. ICP_Image_Descriptor
 *    3. ICP_Image
 *
 * They can be found in the /Documentation directory of the ICP SDK installation location.
 *
 **************************************************************************/
LIB_RESULT GdcLdw_DetectionTask::TaskService()
{
  LIB_RESULT libResult = LIB_SUCCESS;

  bool isTaskClosing = false;

  MessageBase* pMessage           = 0;
  int32_t      messageSizeInBytes = 0;

  while(isTaskClosing == false)
  {
    // This task uses a message handler pattern and will only run
    // if a message is sent to its queue.
    // It spends the rest of its time suspended on the message queue semaphore.
    libResult = MsgMgr_MessageReceive(mReceiveMessageQueueId, pMessage, messageSizeInBytes, 0xFFFFFFFF);
    //0x00989680);
    if(libResult == LIB_SUCCESS && pMessage != 0)
    {
      switch(pMessage->mMessageId)
      {
        case MESSAGE_ID_GDC_LDW_IMAGE_REPLY:
        {
          GdcLdw_ImageReply* pReply = (GdcLdw_ImageReply*)pMessage;

          libResult = GdcLdw_DetectionTask::HandleImageReply(*pReply);

          pReply->~GdcLdw_ImageReply();

          break;
        }
        case MESSAGE_ID_GDC_LDW_EXIT_REQUEST:
        {
          isTaskClosing = true;

          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult = GdcLdw_Helper::SendRequest(MESSAGE_ID_GDC_LDW_EXIT_REQUEST, mReceiveMessageQueueId,
                                                 mDisplayMessageQueueId);
          SHOW_ERROR(libResult == LIB_SUCCESS);
          break;
        }
        case MESSAGE_ID_GDC_LDW_RESET_REQUEST: // this is the case of reading image data from file and repeat
        {
          //TODO:
          break;
        }
        default:
        {
          SHOW_ERROR(false); // This should NEVER happen!
          break;
        }
      }

      free(pMessage); // Buffer received by MsgMgr_MessageReceive is freed
      pMessage = 0;
    }
    else
    {
      SHOW_ERROR(false); // This should NEVER happen!
      break;             // This will lead to task entering "finished" state after "TaskClose" is called
    }

    if(gcIsRunningWithThreads == false)
    {
      break;
    }
  } // while(isTaskClosing == false)

  if(libResult == LIB_SUCCESS && mIsTaskClosing == true)
  {
    libResult = LIB_APP_IS_CLOSING;
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 *
 * See ICP_SDK_Platform_Software_UserGuide document for further details about:
 *    1. OAL Task State machine
 *    2. Communication infrastructure.
 *    3. File System Abstraction
 *    4. Memory Abstraction
 *
 * It can be found in the /Documentation directory of the ICP SDK installation location.
 *
 **************************************************************************/
LIB_RESULT GdcLdw_DetectionTask::TaskClose()
{
  LIB_RESULT libResult = LIB_SUCCESS;

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * Runs the incoming frame buffer into the algorithm and forwards the result to the displays
 *
 **************************************************************************/
LIB_RESULT GdcLdw_DetectionTask::HandleImageReply(GdcLdw_ImageReply& rReply)
{
  GDC_TRACE;

  uint64_t ticks[gcApexCvEndTick] = {0};

  ticks[gcSceneTick]     = rReply.mTicks[gcSceneTick];     // copy time for scene task
  ticks[gcSceneTick + 1] = rReply.mTicks[gcSceneTick + 1]; // copy time for scene task
  ticks[gcFiltersTick]   = FSL_Ticks();

  LIB_RESULT libResult = LIB_FAILURE;
  //int64_t startFreeMemory = 0;

  if(rReply.mScene_umat.empty() == false)
  {
    //
    // STEP: Convert to birdeye
    //
    // Change the perspective of the scene to bird eye.
    // From this perspective the lanes will be parallel.
    //
    //
    // STEP:  Apply SOBEL filter
    //
    // Produce an edge image which highlight the lanes.
    //
    // STEP:  Apply MEDIAN filter
    //
    // 3x3 median filter was added to reduce the impact of noise in the edge image.
    // This greatly clean the edge and avoid inaccurate voting during the lane detection
    //
    // STEP: Find Lanes
    //
    // Apply pseudo Hough transform to find the left and right lane.
    // This function also does thresholding, so pixels with
    // a value less than mLdsConfiguration.mMinGrayScaleValue are not considered.
    //
    // Find the best candidate for the left lane and right lane
    // We are looking for the highest number of votes for each lanes.
    // Those lines correlates to the birdeye image
    //
    // Find where the (rho,theta) Polar line intersects with the edge of the image
    //
    //

    //      clock_gettime(CLOCK_REALTIME, &t1);
    vsdk::Mat lVoteThreshold_mat = mVoteThreshold_umat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);

    uint8_t* vote = (uint8_t*)lVoteThreshold_mat.data;
    vote[0]       = 100 - mLdwConfig.mLaneVoteThreshold;

    SWT_ARM_LOG_IMAGE_SIZE_FUNCTION_REG(mSceneWidthInPixels, mSceneHeightInPixels, mBirdEyeSobelMedian_umat.cols,
                                        mBirdEyeSobelMedian_umat.rows, 0, *vote, 0, 0, 0, 0);

#ifdef _TOGGLE_REG_GRAPH_
    libResult =
        GdcLdw_Helper::AcfFilters_reg(mAcfRegProcess, lSceneCdd, mBirdEyeLutDelta_umat, F mBirdEyeLutOffset_umat,
                                      mBirdEyeLutBlockOffset_umat, mVoteThreshold_umat,
                                      /*out*/ mLaneData_umat,
                                      /*in/out*/ mPreviousThreshold_umat,
                                      /*out*/ mBirdEyeSobelMedian_umat);
    SHOW_ERROR(libResult == LIB_SUCCESS);
#endif // _TOGGLE_REG_GRAPH_

#ifndef _TOGGLE_REG_GRAPH_
    libResult =
        GdcLdw_Helper::AcfFilters_no_reg(mAcfNoRegProcess, rReply.mScene_umat, mBirdEyeLutDelta_umat,
                                         mBirdEyeLutOffset_umat, mBirdEyeLutBlockOffset_umat, mVoteThreshold_umat,
                                         /*out*/ mLaneData_umat,
                                         /*in/out*/ mPreviousThreshold_umat,
                                         /*out*/ mBirdEyeSobelMedian_umat,
                                         /* for no_reg only */ mLeftMax_umat,
                                         /* for no_reg only */ mRightMax_umat,
                                         /* for no_reg only */ mPoints_umat);
    SHOW_ERROR(libResult == LIB_SUCCESS);
#endif

    SWT_ARM_LOG_FUNCTION_RVAL(libResult);
    SWT_ARM_LOG_IMAGE_SIZE_FUNCTION_REG(mSceneWidthInPixels, mSceneHeightInPixels, mBirdEyeSobelMedian_umat.cols,
                                        mBirdEyeSobelMedian_umat.rows, 0, 0, 0, 0, 0, 1);

    //char* pleftdata  = (char*)mLeftMaxCdd.GetDataPtr();
    //char* pRightData = (char*)mRightMaxCdd.GetDataPtr();
    //int32_t *pPointData = (int32_t *)mPointsCdd.GetDataPtr();
    //int32_t *pLaneData = (int32_t *)mLaneDataCdd.GetDataPtr();
    //char *PreviousThreshold = (char *)mPreviousThresholdCdd.GetDataPtr();

    //printf("point:%d,%d,%d,%d,%d,%d\n", pPointData[0], pPointData[1], pPointData[2], pPointData[3], pPointData[4], pPointData[5]);

    //printf("Lane:%d,%d,%d,%d,%d,%d\n\n", pLaneData[0], pLaneData[1], pLaneData[2], pLaneData[3], pLaneData[4], pLaneData[5]);
    //printf("thres:%d\n", (int32_t) *PreviousThreshold);

    //printf("L:%d, R:%d\n",(int32_t) *pleftdata, (int32_t) *pRightData);
    //printf("f:%d, rc:%d\n",rReply.mSceneCounter, libResult);

    GdcLdw_LinePolar birdEyeLeftLanePolar;
    GdcLdw_LinePolar birdEyeRightLanePolar;
    ICP_Line         birdEyeLeftLaneCartesian;
    ICP_Line         birdEyeRightLaneCartesian;

    libResult = GdcLdw_Helper::UpdateLaneRawData(mLaneData_umat,
                                                 /*out*/ birdEyeLeftLanePolar,
                                                 /*out*/ birdEyeRightLanePolar,
                                                 /*out*/ birdEyeLeftLaneCartesian,
                                                 /*out*/ birdEyeRightLaneCartesian);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    ticks[gcFiltersTick + 1] = FSL_Ticks();
    ticks[gcDetectionTick]   = ticks[gcFiltersTick + 1];

    //void* displayImage = mBirdEyeSobelMedianCdd.GetDataPtr();

    //
    // STEP: Kalman filter
    //
    // Kalman filter was added to model the output of the lane detector and reduce the impact of false positives
    // Once the boundary lines have been found, Kalman filters are used to track them in order to manage various
    // unwanted noise such as cracks in the road, shadows from other vehicles and bridges, and also in situations
    // when the road lines are not very visible.
    // In order to be certain that the filters do not track detected lines that are not actually the lane boundaries,
    // each filter is updated only if certain conditions are met.
    const int32_t cLeftAngleKalman  = mLeftAngleKalman.GetValue();
    const int32_t cRightAngleKalman = mRightAngleKalman.GetValue();
    //if there is only one lane detected and it is close to the predicted centre, the vehicle is changing lanes

    //reset the flags
    bool mLeftLineHasCorrectAngle  = false;
    bool mRightLineHasCorrectAngle = false;
    //if the angles are relatively parallel (within 5 degrees of each other), and they are "good", update their kalman filters
    //Compare the angles of the newly detected lines with the Kalman values
    if(birdEyeLeftLanePolar.mVotes != 0 && birdEyeRightLanePolar.mVotes != 0)
    {
      if(abs(birdEyeLeftLanePolar.mAngleInDegrees - birdEyeRightLanePolar.mAngleInDegrees) <
         mLdwConfig.mMaxLRAngleDiffInDegrees)
      {
        if(abs(birdEyeLeftLanePolar.mAngleInDegrees - cLeftAngleKalman) < mLdwConfig.mMaxAngleDeltaInDegrees)
        {
          mLeftLineHasCorrectAngle = true;
        }
        if(abs(birdEyeRightLanePolar.mAngleInDegrees - cRightAngleKalman) < mLdwConfig.mMaxAngleDeltaInDegrees)
        {
          mRightLineHasCorrectAngle = true;
        }
      }
    }
    else
    {
      if(birdEyeLeftLanePolar.mVotes != 0)
      {
        if(abs(birdEyeLeftLanePolar.mAngleInDegrees - cRightAngleKalman) < mLdwConfig.mMaxLRAngleDiffInDegrees)
        {
          if(abs(birdEyeLeftLanePolar.mAngleInDegrees - cLeftAngleKalman) < mLdwConfig.mMaxAngleDeltaInDegrees)
          {
            mLeftLineHasCorrectAngle = true;
          }
        }
      }

      if(birdEyeRightLanePolar.mVotes != 0)
      {
        if(abs(birdEyeRightLanePolar.mAngleInDegrees - cLeftAngleKalman) < mLdwConfig.mMaxLRAngleDiffInDegrees)
        {
          if(abs(birdEyeRightLanePolar.mAngleInDegrees - cRightAngleKalman) < mLdwConfig.mMaxAngleDeltaInDegrees)
          {
            mRightLineHasCorrectAngle = true;
          }
        }
      }
    }
    // Find line gaps
    // This is using the information from the birdeye image
    const int32_t cLaneGap      = birdEyeRightLaneCartesian.point1.x - birdEyeLeftLaneCartesian.point1.x;
    const int32_t cCentreLanePt = (cLaneGap / 2) + birdEyeLeftLaneCartesian.point1.x;

    //Added second point kalman values
    const int32_t cLaneGap2      = birdEyeRightLaneCartesian.point2.x - birdEyeLeftLaneCartesian.point2.x;
    const int32_t cCentreLanePt2 = (cLaneGap2 / 2) + birdEyeLeftLaneCartesian.point2.x;

    const int32_t cLaneGapKalman2 = mLaneGapKalman2.GetValue();
    const int32_t cCentreKalman2  = mCentreKalman2.GetValue();

    const int32_t cLaneGapKalman = mLaneGapKalman.GetValue();
    const int32_t cCentreKalman  = mCentreKalman.GetValue();

    //Variable to Covert Bird-eye View to Tio View
    ICP_Line birdEyeLeftLane;
    ICP_Line birdEyeRightLane;
    ICP_Line sceneLeftLane;
    ICP_Line sceneRightLane;
    bool     mNoLanesFound = false;

    //Parse the lanes detected to verify if they are actual lanes or irrelevant flutter in the data.
    if(mLeftLineHasCorrectAngle && mRightLineHasCorrectAngle && (cLaneGap < mLdwConfig.mMaxLaneWidth) &&
       (cLaneGap > mLdwConfig.mMinLaneWidth) && (cLaneGap2 < mLdwConfig.mMaxLaneWidth) &&
       (cLaneGap2 > mLdwConfig.mMinLaneWidth))
    {
      // The Kalman filter reset occurs during lane changes since the information is completely different
      // otherwise the lane lines would gradually slide to the new location causing the departure detection to falsely trigger
      if(abs(cCentreLanePt - cCentreKalman) > mLdwConfig.mMaxLaneCenterDelta ||
         abs(cCentreLanePt2 - cCentreKalman2) > mLdwConfig.mMaxLaneCenterDelta)
      {
        libResult = mLaneGapKalman.Reset(gcLdwKalmanMeasurementNoiseVar, gcLdwKalmanPostErrorVar, cLaneGap);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        libResult = mCentreKalman.Reset(gcLdwKalmanMeasurementNoiseVar, gcLdwKalmanPostErrorVar, cCentreLanePt);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        libResult = mLaneGapKalman2.Reset(gcLdwKalmanMeasurementNoiseVar, gcLdwKalmanPostErrorVar, cLaneGap2);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        libResult = mCentreKalman2.Reset(gcLdwKalmanMeasurementNoiseVar, gcLdwKalmanPostErrorVar, cCentreLanePt2);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        libResult = mLeftAngleKalman.Reset(gcLdwKalmanMeasurementNoiseVar, gcLdwKalmanPostErrorVar,
                                           birdEyeLeftLanePolar.mAngleInDegrees);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        libResult = mRightAngleKalman.Reset(gcLdwKalmanMeasurementNoiseVar, gcLdwKalmanPostErrorVar,
                                            birdEyeRightLanePolar.mAngleInDegrees);
        SHOW_ERROR(libResult == LIB_SUCCESS);
      }
      else
      {
        libResult = mLaneGapKalman.NextMeasurement(cLaneGap);
        SHOW_ERROR(libResult == LIB_SUCCESS);
        libResult = mCentreKalman.NextMeasurement(cCentreLanePt);
        SHOW_ERROR(libResult == LIB_SUCCESS);
        libResult = mLaneGapKalman2.NextMeasurement(cLaneGap2);
        libResult = mCentreKalman2.NextMeasurement(cCentreLanePt2);
        libResult = mLeftAngleKalman.NextMeasurement(birdEyeLeftLanePolar.mAngleInDegrees);
        SHOW_ERROR(libResult == LIB_SUCCESS);
        libResult = mRightAngleKalman.NextMeasurement(birdEyeRightLanePolar.mAngleInDegrees);
        SHOW_ERROR(libResult == LIB_SUCCESS);
      }
    }
    else if(mLeftLineHasCorrectAngle && //Estimate the position of the right lane if the left is a valid lane
            abs(birdEyeLeftLaneCartesian.point1.x + cLaneGapKalman / 2 - cCentreKalman) <
                mLdwConfig.mMaxLaneCenterDelta &&
            abs(birdEyeLeftLaneCartesian.point2.x + cLaneGapKalman2 / 2 - cCentreKalman2) <
                mLdwConfig.mMaxLaneCenterDelta)
    {
      libResult = mCentreKalman.NextMeasurement(birdEyeLeftLaneCartesian.point1.x + cLaneGapKalman / 2);
      SHOW_ERROR(libResult == LIB_SUCCESS);
      libResult = mCentreKalman2.NextMeasurement(birdEyeLeftLaneCartesian.point2.x + cLaneGapKalman2 / 2);
      SHOW_ERROR(libResult == LIB_SUCCESS);
      libResult = mLeftAngleKalman.NextMeasurement(birdEyeLeftLanePolar.mAngleInDegrees);
      SHOW_ERROR(libResult == LIB_SUCCESS);
    }
    else if(mRightLineHasCorrectAngle && //Estimate the position of the left lane if the right is a valid lane
            abs(birdEyeRightLaneCartesian.point1.x - cLaneGapKalman / 2 - cCentreKalman) <
                mLdwConfig.mMaxLaneCenterDelta &&
            abs(birdEyeRightLaneCartesian.point2.x - cLaneGapKalman2 / 2 - cCentreKalman2) <
                mLdwConfig.mMaxLaneCenterDelta)
    {
      libResult = mCentreKalman.NextMeasurement(birdEyeRightLaneCartesian.point1.x - (cLaneGapKalman / 2));
      SHOW_ERROR(libResult == LIB_SUCCESS);
      libResult = mCentreKalman2.NextMeasurement(birdEyeRightLaneCartesian.point2.x - (cLaneGapKalman2 / 2));
      SHOW_ERROR(libResult == LIB_SUCCESS);
      libResult = mRightAngleKalman.NextMeasurement(birdEyeRightLanePolar.mAngleInDegrees);
      SHOW_ERROR(libResult == LIB_SUCCESS);
    }
    else
    {
      // Getting Garbage Lines, simply keep the kalman values you have
      mNoLanesFound = true;
    }

    if(mNoLanesFound)
    { //Don't show lines, but use the values to check lane change
      birdEyeLeftLane.point1.x = cCentreKalman2 - (cLaneGapKalman2 / 2L);
      birdEyeLeftLane.point1.y = mLdwConfig.mBirdEyeHeight - 1;
      birdEyeLeftLane.point2.x = cCentreKalman2 - (cLaneGapKalman2 / 2L);
      birdEyeLeftLane.point2.y = mLdwConfig.mBirdEyeHeight - 1;

      birdEyeRightLane.point1.x = cCentreKalman2 + (cLaneGapKalman2 / 2L);
      birdEyeRightLane.point1.y = mLdwConfig.mBirdEyeHeight - 1;
      birdEyeRightLane.point2.x = cCentreKalman2 + (cLaneGapKalman2 / 2L);
      birdEyeRightLane.point2.y = mLdwConfig.mBirdEyeHeight - 1;
    }
    else
    {
      birdEyeLeftLane.point1.x = std::max(cCentreKalman - (cLaneGapKalman / 2L), 0L);
      birdEyeLeftLane.point1.y = 0;
      birdEyeLeftLane.point2.x = std::max(cCentreKalman2 - (cLaneGapKalman2 / 2L), 0L);
      birdEyeLeftLane.point2.y = mLdwConfig.mBirdEyeHeight - 1;

      birdEyeRightLane.point1.x = cCentreKalman + (cLaneGapKalman / 2L);
      birdEyeRightLane.point1.y = 0;
      birdEyeRightLane.point2.x = cCentreKalman2 + (cLaneGapKalman2 / 2L);
      birdEyeRightLane.point2.y = mLdwConfig.mBirdEyeHeight - 1;
    }

    //
    // STEP: Covert Bird-eye View to Tio View
    //

    if(cLaneGapKalman != 0 && cCentreKalman != 0)
    {
      mRemap.convertBirdEyeLinesToScene(mLdwConfig.mBirdEyeWidth, birdEyeLeftLane, birdEyeRightLane, sceneLeftLane,
                                        sceneRightLane);
    }

    if(mIsLeftWarning)
    {
      if(sceneLeftLane.point2.x > (int32_t)(mSceneWidthInPixels / 2 - mLdwWarningWidthRangeInWarning))
      {
        mLeftWarningCounter = mLdwMaxWarningFrameCount << 1;
      }
      else
      {
        mLeftWarningCounter = mLeftWarningCounter - 1 < mLdwMaxWarningFrameCount ? 0 : mLeftWarningCounter - 1;
      }
    }
    else
    {
      if(sceneLeftLane.point2.x > (int32_t)(mSceneWidthInPixels / 2 - mLdwWarningWidthRangeInLane))
      {
        mLeftWarningCounter = mLeftWarningCounter + 1 >= mLdwMaxWarningFrameCount ? mLdwMaxWarningFrameCount << 1
                                                                                  : mLeftWarningCounter + 1;
      }
      else
      {
        mLeftWarningCounter = 0;
      }
    }

    if(mIsRightWarning)
    {
      if(sceneRightLane.point2.x < (int32_t)(mSceneWidthInPixels / 2 + mLdwWarningWidthRangeInWarning))
      {
        mRightWarningCounter = mLdwMaxWarningFrameCount << 1;
      }
      else
      {
        mRightWarningCounter = mRightWarningCounter - 1 < mLdwMaxWarningFrameCount ? 0 : mRightWarningCounter - 1;
      }
    }
    else
    {
      if(sceneRightLane.point2.x < (int32_t)(mSceneWidthInPixels / 2 + mLdwWarningWidthRangeInLane))
      {
        mRightWarningCounter = mRightWarningCounter + 1 >= mLdwMaxWarningFrameCount ? mLdwMaxWarningFrameCount << 1
                                                                                    : mRightWarningCounter + 1;
      }
      else
      {
        mRightWarningCounter = 0;
      }
    }

    mIsLeftWarning  = mLeftWarningCounter >= mLdwMaxWarningFrameCount;
    mIsRightWarning = mRightWarningCounter >= mLdwMaxWarningFrameCount;

    if(!mIsRightWarning && !mIsLeftWarning)
    {
      mOldState = MID_LANE;
    }
    else if(mIsRightWarning && (mOldState == MID_LANE || mOldState == NO_LANES))
    {
      mOldState = TURNING_RIGHT;
    }
    else if(mIsLeftWarning && (mOldState == MID_LANE || mOldState == NO_LANES))
    {
      mOldState = TURNING_LEFT;
    }

    if((mNoLanesCounter >= mLdwMaxWarningFrameCount) && mOldState == MID_LANE)
    {
      mOldState = NO_LANES;
    }

    //
    // STEP: Send results out so they can reach the user.
    //
    bool lNoLanesFound    = false;
    bool lWeDepartedLeft  = false;
    bool lWeDepartedRight = false;

    switch(mOldState)
    {
      case TURNING_RIGHT:
      {
        lNoLanesFound    = false;
        lWeDepartedLeft  = false;
        lWeDepartedRight = true;
        break;
      }
      case TURNING_LEFT:
      {
        lNoLanesFound    = false;
        lWeDepartedLeft  = true;
        lWeDepartedRight = false;
        break;
      }
      case MID_LANE:
      {
        lNoLanesFound    = false;
        lWeDepartedLeft  = false;
        lWeDepartedRight = false;
        break;
      }
      default:
      {
        lNoLanesFound    = true;
        lWeDepartedLeft  = mIsLeftWarning;
        lWeDepartedRight = mIsRightWarning;
        break;
      }
    }
    SWT_ARM_LOG_FUNCTION_RVAL(libResult);

    ticks[gcDetectionTick + 1] = FSL_Ticks();
    libResult                  = GdcLdw_Helper::SendDetectionData(
        mDisplayMessageQueueId, rReply.mSceneRgb24_umat,
        rReply.mSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
        rReply.mScene_umat,
        rReply.mScene_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
        rReply.mSceneCounter, 0, sceneLeftLane, sceneRightLane, lNoLanesFound, lWeDepartedLeft, lWeDepartedRight,
        ticks);
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }

  GDC_TRACE;

  return libResult;
}
