/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2015 Freescale Semiconductor;
* Copyright 2017 NXP
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

/***************************************************************************/
/* Include files. */
#include "gdc_ldw_configuration.hpp"
#include "gdc_ldw_composition_task.hpp"
#include "communications.hpp"
#include "iniparser.h"
#include "Font7x13.h"
#include "nxp_logo.h"
#include "global_errors.h"
#include <stdint.h>

#include <apexcv_pro_resize.h>
#include "resize_ref.h"

#include "common_time_measure.h"

/***************************************************************************/
/* Local Literals & definitions */

/***************************************************************************/
/***************************************************************************/
GdcLdw_CompositionTask::GdcLdw_CompositionTask()
    : mReceiveMessageQueueId(0), mDisplayTaskMessageQueueId(0), mIsTaskClosing(false), mIsConfigured(false),
      //GdcLdw_MovingAverage mMovingAverage_FramePeriod;
      mFrameTicks(0), mWeDrawLanes(true), mWeDrawAlarmIcon(true), mWeDrawTopView(false), mpAlarmIcon_NoLane(0),
      mpAlarmIcon_InLane(0), mpAlarmIcon_TurningRight(0), mpAlarmIcon_TurningLeft(0), mDisplayTicks(0)
{
}

/***************************************************************************/
/***************************************************************************/
GdcLdw_CompositionTask::~GdcLdw_CompositionTask()
{
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcLdw_CompositionTask::Configure(const int32_t cReceiveMessageQueueId,
                                             const int32_t cDisplayTaskMessageQueueId)
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsConfigured == false)
  {
    mReceiveMessageQueueId     = cReceiveMessageQueueId;
    mDisplayTaskMessageQueueId = cDisplayTaskMessageQueueId;

    mIsConfigured = true;

    libResult = LIB_SUCCESS;
  }

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcLdw_CompositionTask::TaskOpen()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  mMovingAverage_FramePeriod.Initialize(16); // Magic number bad!!!

  mWeDrawLanes             = gcWeDrawLanes;
  mWeDrawAlarmIcon         = gcWeDrawAlarmIcon;
  mWeDrawTopView           = gcWeDrawTopView;
  mAlarmIconWidthInPixels  = gcAlarmIconWidthInPixels;
  mAlarmIconHeightInPixels = gcAlarmIconHeightInPixels;

  const char* lpAlarmIcon_NoLane       = gcAlarmIcon_NoLane;
  const char* lpAlarmIcon_InLane       = gcAlarmIcon_InLane;
  const char* lpAlarmIcon_TurningRight = gcAlarmIcon_TurningRight;
  const char* lpAlarmIcon_TurningLeft  = gcAlarmIcon_TurningLeft;

  dictionary* pDictionary = 0;

  pDictionary = iniparser_load(gcIniFileName);

  if(pDictionary != 0)
  {
    mWeDrawLanes     = iniparser_getint(pDictionary, "CompositionTask:WeDrawLanes", gcWeDrawLanes);
    mWeDrawAlarmIcon = iniparser_getint(pDictionary, "CompositionTask:WeDrawAlarmIcon", gcWeDrawAlarmIcon);
    mWeDrawTopView   = iniparser_getint(pDictionary, "CompositionTask:WeDrawTopView", gcWeDrawTopView);
    mAlarmIconWidthInPixels =
        iniparser_getint(pDictionary, "CompositionTask:AlarmIconWidthInPixels", gcAlarmIconWidthInPixels);
    mAlarmIconHeightInPixels =
        iniparser_getint(pDictionary, "CompositionTask:AlarmIconHeightInPixels", gcAlarmIconHeightInPixels);

    lpAlarmIcon_NoLane = iniparser_getstring_const(pDictionary, "CompositionTask:AlarmIcon_NoLane", gcAlarmIcon_NoLane);
    lpAlarmIcon_InLane = iniparser_getstring_const(pDictionary, "CompositionTask:AlarmIcon_InLane", gcAlarmIcon_InLane);
    lpAlarmIcon_TurningRight =
        iniparser_getstring_const(pDictionary, "CompositionTask:AlarmIcon_TurningRight", gcAlarmIcon_TurningRight);
    lpAlarmIcon_TurningLeft =
        iniparser_getstring_const(pDictionary, "CompositionTask:AlarmIcon_TurningLeft", gcAlarmIcon_TurningLeft);

    //
    // Need to keep the dictionary alive for the calls to GdcLdw_Helper::readRawImage
    // Will free it at the end of this function
    //
  }

  const uint32_t cAlarmIconSizeInBytes = mAlarmIconWidthInPixels * mAlarmIconHeightInPixels * 4; // Magic Number Bad!!!!

  mpAlarmIcon_NoLane       = (char*)malloc(cAlarmIconSizeInBytes);
  mpAlarmIcon_InLane       = (char*)malloc(cAlarmIconSizeInBytes);
  mpAlarmIcon_TurningRight = (char*)malloc(cAlarmIconSizeInBytes);
  mpAlarmIcon_TurningLeft  = (char*)malloc(cAlarmIconSizeInBytes);

  if(mpAlarmIcon_NoLane != 0 && mpAlarmIcon_InLane != 0 && mpAlarmIcon_TurningRight != 0 &&
     mpAlarmIcon_TurningLeft != 0)
  {
    libResult = GdcLdw_Helper::readRawImage(lpAlarmIcon_NoLane, mpAlarmIcon_NoLane, cAlarmIconSizeInBytes);

    libResult = GdcLdw_Helper::readRawImage(lpAlarmIcon_InLane, mpAlarmIcon_InLane, cAlarmIconSizeInBytes);

    libResult = GdcLdw_Helper::readRawImage(lpAlarmIcon_TurningRight, mpAlarmIcon_TurningRight, cAlarmIconSizeInBytes);

    libResult = GdcLdw_Helper::readRawImage(lpAlarmIcon_TurningLeft, mpAlarmIcon_TurningLeft, cAlarmIconSizeInBytes);
  }
  else
  {
    mIsTaskClosing = true;
    // Resources will be freed in TaskClose
  }

  if(pDictionary != 0)
  {
    iniparser_freedict(pDictionary);
    pDictionary = 0;
  }

  mDisplayTicks = FSL_Ticks();

  GDC_TRACE;

  return libResult;
}

/***************************************************************************/
/***************************************************************************/
LIB_RESULT GdcLdw_CompositionTask::TaskService()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  MessageBase* pMessage           = 0;
  int32_t      messageSizeInBytes = 0;

  while(mIsTaskClosing == false) // Keep going as long as there are more images in the file.
  {
    // This task uses a message handler pattern and will only run
    // if a message is sent to its queue.
    // It spends the rest of its time suspended on the message queue semaphore.
    libResult = MsgMgr_MessageReceive(mReceiveMessageQueueId, pMessage, messageSizeInBytes,
                                      0xFFFFFFFF); // 0xFFFFFFFF wait forever
    if(libResult == LIB_SUCCESS && pMessage != 0)
    {
      switch(pMessage->mMessageId)
      {
        case MESSAGE_ID_GDC_LDW_DETECTION_DATA:
        { //
          // Request to send image to the display
          //
          GdcLdw_DetectionData* pData = (GdcLdw_DetectionData*)pMessage; // Placement new

          libResult = GdcLdw_CompositionTask::HandleDetectionData(*pData);

          pData->~GdcLdw_DetectionData();
          break;
        }
        case MESSAGE_ID_GDC_LDW_EXIT_REQUEST:
        {
          libResult = MsgMgr_MessageQueueSetFlowControl(mReceiveMessageQueueId,
                                                        /*cIsSendingAllowed*/ false,
                                                        /*cIsReceivingAllowed*/ true);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          libResult = GdcLdw_Helper::SendRequest(MESSAGE_ID_GDC_LDW_EXIT_REQUEST, mReceiveMessageQueueId,
                                                 mDisplayTaskMessageQueueId);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          mIsTaskClosing = true;

          break;
        }
        case MESSAGE_ID_GDC_LDW_RESET_REQUEST:
        {
          // Do Nothing
          break;
        }
        default:
        {
          SHOW_ERROR(false); // This should NEVER happen!
          break;
        }
      }
      free(pMessage);
      pMessage = 0;
    }

    if(gcIsRunningWithThreads == false)
    {
      break;
    }
  }

  if(libResult == LIB_SUCCESS && mIsTaskClosing == true)
  {
    libResult = LIB_APP_IS_CLOSING;
  }

  return libResult;
}

/***************************************************************************
 **************************************************************************/
LIB_RESULT GdcLdw_CompositionTask::TaskClose()
{
  GDC_TRACE;
  LIB_RESULT libResult = LIB_FAILURE;

  //
  // Free resources owned by the task
  //
  if(mpAlarmIcon_NoLane != 0)
  {
    free(mpAlarmIcon_NoLane);
    mpAlarmIcon_NoLane = 0;
  }
  if(mpAlarmIcon_InLane != 0)
  {
    free(mpAlarmIcon_InLane);
    mpAlarmIcon_InLane = 0;
  }
  if(mpAlarmIcon_TurningRight != 0)
  {
    free(mpAlarmIcon_TurningRight);
    mpAlarmIcon_TurningRight = 0;
  }
  if(mpAlarmIcon_TurningLeft != 0)
  {
    free(mpAlarmIcon_TurningLeft);
    mpAlarmIcon_TurningLeft = 0;
  }

  //
  // Free messages still in the tasks queue
  //
  MessageBase* pMessage           = 0;
  int32_t      messageSizeInBytes = 0;

  while(1)
  {
    libResult = MsgMgr_MessageReceive(mReceiveMessageQueueId, pMessage, messageSizeInBytes, 0);
    if(libResult == LIB_SUCCESS && pMessage != 0)
    {
      switch(pMessage->mMessageId)
      {
        case MESSAGE_ID_GDC_LDW_DETECTION_DATA:
        {
          //
          // Request to send image to the display
          //
          GdcLdw_DetectionData* pData = (GdcLdw_DetectionData*)pMessage; // Placement new

          libResult = GdcLdw_Helper::FreeDetectionData(pData);
          SHOW_ERROR(libResult == LIB_SUCCESS);

          pData->~GdcLdw_DetectionData();
          break;
        }
        default:
        {
          SHOW_ERROR(false); // This should NEVER happen!
          break;
        }
      }
      free(pMessage);
      pMessage = 0;
    }
    else
    {
      break;
    }
  }

  return libResult;
}

/***************************************************************************/
/**
 * \par Description
 * Displays the result of the LDW algorithm
 *
 **************************************************************************/
LIB_RESULT GdcLdw_CompositionTask::HandleDetectionData(GdcLdw_DetectionData& rData)
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  const uint32_t cSceneWidthInPixels  = rData.mScene_umat.cols;
  const uint32_t cSceneHeightInPixels = rData.mScene_umat.rows;

  bool weNeedToConvertYToRgb24 = false;
  rData.mTicks[gcCompTick]     = FSL_Ticks();
  if(rData.mSceneRgb24_umat.empty() == true)
  {
    rData.mSceneRgb24_umat.create(cSceneHeightInPixels, cSceneWidthInPixels, CV_8UC3);
    SHOW_ERROR(rData.mSceneRgb24_umat.empty() == false);

    weNeedToConvertYToRgb24 = true;
  }

  if(rData.mSceneRgb24_umat.empty() == false)
  {
    vsdk::Mat lSceneRgb24_mat = rData.mSceneRgb24_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);

    void* lpSceneRgb24 = (void*)(lSceneRgb24_mat.data);
    SHOW_ERROR(lpSceneRgb24 != 0);

    if(weNeedToConvertYToRgb24 == true)
    {
      libResult = GdcLdw_Helper::ConvertYToRgb24(rData.mScene_umat, rData.mSceneRgb24_umat);
      SHOW_ERROR(libResult == LIB_SUCCESS);
    }
    else
    {
      libResult = LIB_SUCCESS;
    }

    if(libResult == LIB_SUCCESS)
    {
      const uint32_t cScale             = 2;
      const uint32_t cTextColor         = 0x00ff0000;
      const uint32_t cLineSpace         = 0;
      const int32_t  cTextOffsetY_delta = (TEXT_HEIGHT_7x13 * cScale + cLineSpace);

      int32_t textOffsetX = 16;
      int32_t textOffsetY = cTextOffsetY_delta;

      char lString[128] = {0};
      sprintf(lString, "Frame       [%d]", rData.mSceneCounter);
      GdcLdw_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, textOffsetX, textOffsetY,
                                             (uint8_t*)lString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

      //
      // Draw Frames Per Second
      //
      if(mFrameTicks != 0)
      {
        const uint64_t cFrameTicks = FSL_Ticks();
        mMovingAverage_FramePeriod.Update(cFrameTicks - mFrameTicks);
        mFrameTicks = cFrameTicks;
      }
      else
      {
        mFrameTicks = FSL_Ticks();
      }

      const double lFramesPerSecond = (1000.0 * 1000.0) / (double)(mMovingAverage_FramePeriod.Get() / 1000);

      lString[0] = 0;
      textOffsetY += cTextOffsetY_delta;
      sprintf(lString, "Frame rate  [%d] fps", (int)lFramesPerSecond);
      GdcLdw_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, textOffsetX, textOffsetY,
                                             (uint8_t*)lString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

      lString[0] = 0;
      textOffsetY += cTextOffsetY_delta;
      sprintf(lString, "Resolution  [%dx%d]", cSceneWidthInPixels, cSceneHeightInPixels);
      GdcLdw_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, textOffsetX, textOffsetY,
                                             (uint8_t*)lString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);
      lString[0] = 0;
      textOffsetY += cTextOffsetY_delta;
      int32_t processTime = (rData.mTicks[1] - rData.mTicks[0]) / 1000;
      sprintf(lString, "APEX        [%d]us", processTime);
      GdcLdw_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, textOffsetX, textOffsetY,
                                             (uint8_t*)lString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

      lString[0] = 0;
      textOffsetY += cTextOffsetY_delta;
      processTime = (rData.mTicks[3] - rData.mTicks[2]) / 1000;
      sprintf(lString, "Kalman      [%d]us", processTime);
      GdcLdw_Helper::RenderTextToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels, textOffsetX, textOffsetY,
                                             (uint8_t*)lString, cTextColor, (uint8_t* const)lpSceneRgb24, cScale);

      if(mWeDrawLanes == true)
      {
        if(rData.mLeftLane.point1.y != rData.mLeftLane.point2.y)
        {
          libResult = GdcLdw_Helper::DrawLineToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels,
                                                           rData.mLeftLane.point1.x, rData.mLeftLane.point1.y,
                                                           rData.mLeftLane.point2.x, rData.mLeftLane.point2.y,
                                                           0xFFFFFF, // Magic Number Bad!!!
                                                           lpSceneRgb24);
        }

        if(rData.mRightLane.point1.y != rData.mRightLane.point2.y)
        {
          libResult = GdcLdw_Helper::DrawLineToRgb24Buffer(cSceneWidthInPixels, cSceneHeightInPixels,
                                                           rData.mRightLane.point1.x, rData.mRightLane.point1.y,
                                                           rData.mRightLane.point2.x, rData.mRightLane.point2.y,
                                                           0xFFFFFF, // Magic Number Bad!!!
                                                           lpSceneRgb24);
        }
      }

      if(mWeDrawTopView == true)
      {
        GdcLdw_Helper::DrawTopView(gcSceneColorModel_DataType, 50, 50,
                                   256, // this should be get from gdcldw.ini file
                                   320, // this should be get from gdcldw.ini file
                                   cSceneWidthInPixels, cSceneHeightInPixels, rData.mhTopViewImage, lpSceneRgb24);
      }

      if(mWeDrawAlarmIcon == true)
      {
        char* icon =
            rData.mNoLanesFound
                ? mpAlarmIcon_NoLane
                : (rData.mWeDepartedLeft ? mpAlarmIcon_TurningLeft
                                         : (rData.mWeDepartedRight ? mpAlarmIcon_TurningRight : mpAlarmIcon_InLane));

        if(icon)
        {
          GdcLdw_Helper::DrawIcon(gcSceneColorModel_DataType, (cSceneWidthInPixels - mAlarmIconWidthInPixels) / 2,
                                  (cSceneHeightInPixels - mAlarmIconHeightInPixels * 1.5), mAlarmIconWidthInPixels,
                                  mAlarmIconHeightInPixels, cSceneWidthInPixels, cSceneHeightInPixels, (char*)icon,
                                  lpSceneRgb24);
        }
        else
        {
          printf("err: icon not selected\n");
        }
      }

      GdcLdw_Helper::SetLogo(cSceneWidthInPixels, cSceneHeightInPixels, cSceneWidthInPixels - NXP_LOGO_WIDTH - 16,
                             cSceneHeightInPixels - NXP_LOGO_HEIGHT - 16, (uint8_t* const)lpSceneRgb24);

      rData.mTicks[gcCompTick + 1] = FSL_Ticks();

      libResult = GdcLdw_Helper::SendDetectionData(
          mDisplayTaskMessageQueueId, rData.mSceneRgb24_umat,
          rData.mSceneRgb24_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
          rData.mScene_umat,
          rData.mScene_umat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, // Debug only
          rData.mSceneCounter, 0, rData.mLeftLane, rData.mRightLane, rData.mNoLanesFound, rData.mWeDepartedLeft,
          rData.mWeDepartedRight, rData.mTicks);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      libResult = GdcLdw_Helper::FreeDetectionData(&rData);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      libResult = LIB_SUCCESS;
    }
  }

  GDC_TRACE;

  return libResult;
}
