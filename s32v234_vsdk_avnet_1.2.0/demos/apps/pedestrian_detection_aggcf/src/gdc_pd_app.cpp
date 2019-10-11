/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2015 Freescale Semiconductor;
* Copyright 2017-2018 NXP
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
#include "gdc_pd_configuration.hpp"
#include "gdc_pd_app.hpp"
#include "gdc_pd_messages.hpp"
#include "iniparser.h"
#include "GDC_OAL_Semaphore.hpp"
#include "communications.hpp"
#include <stdint.h>

// OS Ressources
const char     gcSceneTaskName[]    = "Scene";
const uint32_t gcSceneTaskStackSize = 16 * 4096;
const uint32_t gcSceneTaskPriority  = 28;

const char     gcAggcfLuvTaskName[]    = "Aggcf_L";
const uint32_t gcAggcfLuvTaskStackSize = 16 * 4096;
const uint32_t gcAggcfLuvTaskPriority  = 30;

const char     gcAggcfApproxScaleTaskName[]    = "Aggcf_AS";
const uint32_t gcAggcfApproxScaleTaskStackSize = 16 * 4096;
const uint32_t gcAggcfApproxScaleTaskPriority  = 30;

const char     gcAggcfRealScaleTaskName[]    = "Aggcf_RS";
const uint32_t gcAggcfRealScaleTaskStackSize = 16 * 4096;
const uint32_t gcAggcfRealScaleTaskPriority  = 30;

const char     gcAggcfDetectorTaskName[]    = "Aggcf_D";
const uint32_t gcAggcfDetectorTaskStackSize = 16 * 4096;
const uint32_t gcAggcfDetectorTaskPriority  = 30;

const char     gcCompositionTaskName[]    = "Comp";
const uint32_t gcCompositionTaskStackSize = 16 * 4096;
const uint32_t gcCompositionTaskPriority  = 29;

const char     gcDisplayTaskName[]    = "Display";
const uint32_t gcDisplayTaskStackSize = 16 * 4096;
const uint32_t gcDisplayTaskPriority  = 29;

const char     gcStdinTaskName[]    = "Stdin";
const uint32_t gcStdinTaskStackSize = 16 * 4096;
const uint32_t gcStdinTaskPriority  = 27;

/***************************************************************************/
/* Locally defined global data */

/***************************************************************************/
/**
 * \par Description
 * GdcPd_Application Class Constructor Definition
 *
 * \warning
 * \todo
 * \bug
 **************************************************************************/
GdcPd_App::GdcPd_App()
    : //GdcPd_ImageSensorTask     mImageSensorTask;
      //GdcPd_FileReaderTask      mFileReaderTask;
      //GdcPd_IpCameraTask        mIpCameraTask;
      //GdcPd_CompsitionTask      mCompsitionTask
      //GdcPd_AggcfDetectorTask   mAggcfDetectorTask;
      //GdcPd_AggcfLuvTask        mAggcfLuvTask;
      //GdcPd_DisplayTask         mDisplayTask;
      //GdcPd_StdinTask           mStdinTask;
      mIsStarted(false)
{
}

/***************************************************************************/
/**
 * \par Description
 * GdcPd_Application Class Destructor Definition
 *
 **************************************************************************/
GdcPd_App::~GdcPd_App()
{
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcPd_App::Start()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  GdcPd_SceneTask_Enum lSceneTaskEnum = gcSceneTaskEnum;

  int32_t  lSceneWidthInPixels  = gcSceneWidthInPixels;
  int32_t  lSceneHeightInPixels = gcSceneHeightInPixels;
  uint32_t lSceneBitsPerPixel   = gcSceneBitsPerPixel;
  uint32_t lScenePaddingInBytes = gcScenePaddingInBytes;

  if(mIsStarted == false)
  {
    libResult = MsgMgr_Initialize();
    SHOW_ERROR(libResult == LIB_SUCCESS);

    dictionary* pDictionary = 0;
    ;
    pDictionary = iniparser_load(gcIniFileName);

    if(pDictionary != 0)
    {
      lSceneTaskEnum =
          GdcPd_Helper::ConvertToSceneTaskEnum(iniparser_getint(pDictionary, "App:SceneTaskEnum", gcSceneTaskEnum));

      lSceneWidthInPixels  = iniparser_getint(pDictionary, "App:SceneWidthInPixels", gcSceneWidthInPixels);
      lSceneHeightInPixels = iniparser_getint(pDictionary, "App:SceneHeightInPixels", gcSceneHeightInPixels);
      lSceneBitsPerPixel   = iniparser_getint(pDictionary, "App:SceneBitsPerPixel", gcSceneBitsPerPixel);
      lScenePaddingInBytes = iniparser_getint(pDictionary, "App:ScenePaddingInBytes", gcScenePaddingInBytes);

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }

    int32_t stdinTaskQueueId            = 0;
    int32_t displayTaskQueueId          = 0;
    int32_t compositionTaskQueueId      = 0;
    int32_t aggcfDetectorTaskQueueId    = 0;
    int32_t aggcfLuvTaskQueueId         = 0;
    int32_t aggcfRealScaleTaskQueueId   = 0;
    int32_t aggcfApproxScaleTaskQueueId = 0;
    int32_t sceneTaskQueueId            = 0;

    libResult = MsgMgr_MessageQueueCreate(stdinTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(displayTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(compositionTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(aggcfDetectorTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(aggcfApproxScaleTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(aggcfRealScaleTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(aggcfLuvTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(sceneTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    GdcPd_FrameSize sceneSize(gcSceneWidthInPixels, gcSceneHeightInPixels);

    switch(lSceneTaskEnum)
    {
      case GDC_PD_SCENE_TASK_FILE_READER:
      {
        libResult = mFileReaderTask.Configure(sceneTaskQueueId, aggcfLuvTaskQueueId);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        libResult = mFileReaderTask.Spawn(gcSceneTaskStackSize, gcSceneTaskPriority, gcSceneTaskName);
        SHOW_ERROR(libResult == LIB_SUCCESS);
        GDC_TRACE;
        break;
      }
      case GDC_PD_SCENE_TASK_IMAGE_SENSOR:
      {
        libResult = mImageSensorTask.Configure(sceneTaskQueueId, aggcfLuvTaskQueueId, lSceneWidthInPixels,
                                               lSceneHeightInPixels, lSceneBitsPerPixel, lScenePaddingInBytes);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        libResult = mImageSensorTask.Spawn(gcSceneTaskStackSize, gcSceneTaskPriority, gcSceneTaskName);
        SHOW_ERROR(libResult == LIB_SUCCESS);
        GDC_TRACE;
        break;
      }
      case GDC_PD_SCENE_TASK_IP_CAMERA:
      {
        libResult = mIpCameraTask.Configure(sceneTaskQueueId, aggcfLuvTaskQueueId);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        libResult = mIpCameraTask.Spawn(gcSceneTaskStackSize, gcSceneTaskPriority, gcSceneTaskName);
        SHOW_ERROR(libResult == LIB_SUCCESS);
        GDC_TRACE;
        break;
      }
      default:
      {
        SHOW_ERROR(false);
        break;
      }
    };

    //
    // aggcf feature Task
    //

    libResult = mAggcfLuvTask.Configure(aggcfLuvTaskQueueId, aggcfRealScaleTaskQueueId, sceneSize);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mAggcfLuvTask.Spawn(gcAggcfLuvTaskStackSize, gcAggcfLuvTaskPriority, gcAggcfLuvTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    // feature extractor sub task for real scale
    libResult = mAggcfRealScaleTask.Configure(aggcfRealScaleTaskQueueId, aggcfApproxScaleTaskQueueId, sceneSize);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mAggcfRealScaleTask.Spawn(gcAggcfRealScaleTaskStackSize, gcAggcfRealScaleTaskPriority,
                                          gcAggcfRealScaleTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    // feature extractor sub task for approximate scale
    libResult = mAggcfApproxScaleTask.Configure(aggcfApproxScaleTaskQueueId, aggcfDetectorTaskQueueId, sceneSize);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mAggcfApproxScaleTask.Spawn(gcAggcfApproxScaleTaskStackSize, gcAggcfApproxScaleTaskPriority,
                                            gcAggcfApproxScaleTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    //
    // aggcf detector Task
    //
    libResult = mAggcfDetectorTask.Configure(aggcfDetectorTaskQueueId, compositionTaskQueueId, sceneSize);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult =
        mAggcfDetectorTask.Spawn(gcAggcfDetectorTaskStackSize, gcAggcfDetectorTaskPriority, gcAggcfDetectorTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    //
    // Composition Task
    //
    libResult = mCompositionTask.Configure(compositionTaskQueueId, displayTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mCompositionTask.Spawn(gcCompositionTaskStackSize, gcCompositionTaskPriority, gcCompositionTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    //
    // Display Task
    //
    libResult = mDisplayTask.Configure(displayTaskQueueId, sceneTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mDisplayTask.Spawn(gcDisplayTaskStackSize, gcDisplayTaskPriority, gcDisplayTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    if(gcIsRunningWithThreads == true)
    {
      libResult = mStdinTask.Configure(stdinTaskQueueId, sceneTaskQueueId);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      libResult = mStdinTask.Spawn(gcStdinTaskStackSize, gcStdinTaskPriority, gcStdinTaskName);
      SHOW_ERROR(libResult == LIB_SUCCESS);
      GDC_TRACE;
    }

    mIsStarted = true;

    if(gcIsRunningWithThreads == false)
    {
      while(libResult == LIB_SUCCESS)
      {
        switch(lSceneTaskEnum)
        {
          case GDC_PD_SCENE_TASK_FILE_READER:
          {
            libResult = mFileReaderTask.TaskService();
            SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
            break;
          }
          case GDC_PD_SCENE_TASK_IMAGE_SENSOR:
          {
            libResult = mImageSensorTask.TaskService();
            SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
            break;
          }
          case GDC_PD_SCENE_TASK_IP_CAMERA:
          {
            libResult = mIpCameraTask.TaskService();
            SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
            break;
          }
          default:
          {
            SHOW_ERROR(false);
            libResult = LIB_FAILURE;
            break;
          }
        };

        libResult = mAggcfLuvTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
        libResult = mAggcfRealScaleTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
        libResult = mAggcfApproxScaleTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
        libResult = mAggcfDetectorTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);

        libResult = mCompositionTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);

        libResult = mDisplayTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
      }
    }

    sleep(2);

    if(mDisplayTask.mOpenedSuccessfully == false)
    {
      libResult = mStdinTask.Delete();
      SHOW_ERROR(libResult == LIB_SUCCESS);
      GDC_TRACE;

      libResult = mStdinTask.Join();
      SHOW_ERROR(libResult == LIB_SUCCESS);
      GDC_TRACE;
    }

    switch(lSceneTaskEnum)
    {
      case GDC_PD_SCENE_TASK_FILE_READER:
      {
        libResult = mFileReaderTask.Join();
        SHOW_ERROR(libResult == LIB_SUCCESS);
        GDC_TRACE;
        break;
      }
      case GDC_PD_SCENE_TASK_IMAGE_SENSOR:
      {
        libResult = mImageSensorTask.Join();
        SHOW_ERROR(libResult == LIB_SUCCESS);
        GDC_TRACE;
        break;
      }
      case GDC_PD_SCENE_TASK_IP_CAMERA:
      {
        libResult = mIpCameraTask.Join();
        SHOW_ERROR(libResult == LIB_SUCCESS);
        GDC_TRACE;
        break;
      }
      default:
      {
        SHOW_ERROR(false);
        libResult = LIB_FAILURE;
        break;
      }
    };

    libResult = mAggcfLuvTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;
    libResult = mAggcfRealScaleTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;
    libResult = mAggcfApproxScaleTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;
    libResult = mAggcfDetectorTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;
    libResult = mCompositionTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;
    libResult = mDisplayTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    if(gcIsRunningWithThreads == true)
    {
      libResult = GdcPd_Helper::SendRequest(MESSAGE_ID_GDC_PD_EXIT_REQUEST, 0, stdinTaskQueueId);
      SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_SENDING_NOT_ALLOWED);
      GDC_TRACE;

      libResult = mStdinTask.Join();
      SHOW_ERROR(libResult == LIB_SUCCESS);
      GDC_TRACE;
    }

    libResult = MsgMgr_MessageQueueDestroy(stdinTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(displayTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(compositionTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(aggcfDetectorTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(aggcfApproxScaleTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(aggcfRealScaleTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(aggcfLuvTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(sceneTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    mIsStarted = false;
  }
  GDC_TRACE;

  return libResult;
}
