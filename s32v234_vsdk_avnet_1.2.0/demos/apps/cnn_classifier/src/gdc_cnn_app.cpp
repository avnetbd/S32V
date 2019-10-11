/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2015 Freescale Semiconductor;
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
#include "gdc_cnn_configuration.hpp"
#include "gdc_cnn_app.hpp"
#include "gdc_cnn_messages.hpp"
#include "GDC_OAL_Semaphore.hpp"
#include "communications.hpp"
#include <stdint.h>
#include "iniparser.h"

// OS Ressources
const char     gcSceneTaskName[]    = "Scene";
const uint32_t gcSceneTaskStackSize = 16 * 4096;
const uint32_t gcSceneTaskPriority  = 28;

const char     gcResizeTaskName[]    = "Resize";
const uint32_t gcResizeTaskStackSize = 16 * 4096;
const uint32_t gcResizeTaskPriority  = 30;

const char     gcClassifierTaskName[]    = "Classifier";
const uint32_t gcClassifierTaskStackSize = 16 * 4096;
const uint32_t gcClassifierTaskPriority  = 30;

const char     gcCompositionTaskName[]    = "Composition";
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
 * GdcCnn_Application Class Constructor Definition
 *
 * \warning 
 * \todo    
 * \bug  
 **************************************************************************/
GdcCnn_App::GdcCnn_App()
    : //GdcCnn_ImageSensorTask  mImageSensorTask; // Scene Task
      //GdcCnn_FileReaderTask   mFileReaderTask;  // Scene Task
      //GdcCnn_IpCameraTask     mIpCameraTask;    // Scene Task
      //GdcCnn_ResizeTask       mResizeTask;
      //GdcCnn_ClassifierTask   mClassifierTask;
      //GdcCnn_CompositionTask  mCompositionTask;
      //GdcCnn_CompositionTask  mDisplayTask;
      //GdcCnn_StdinTask        mStdinTask;
      mIsStarted(false)
{
}

/***************************************************************************/
/**
 * \par Description
 * GdcCnn_Application Class Destructor Definition
 *
 **************************************************************************/
GdcCnn_App::~GdcCnn_App()
{
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcCnn_App::Start()
{
  GDC_TRACE;

  LIB_RESULT libResult = LIB_FAILURE;

  GdcCnn_SceneTask_Enum lSceneTaskEnum = gcSceneTaskEnum;

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
          GdcCnn_Helper::ConvertToSceneTaskEnum(iniparser_getint(pDictionary, "App:SceneTaskEnum", gcSceneTaskEnum));

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }

    int32_t stdinTaskQueueId       = 0;
    int32_t displayTaskQueueId     = 0;
    int32_t compositionTaskQueueId = 0;
    int32_t classifierTaskQueueId  = 0;
    int32_t resizeTaskQueueId      = 0;
    int32_t sceneTaskQueueId       = 0;

    libResult = MsgMgr_MessageQueueCreate(stdinTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(displayTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(compositionTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(classifierTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(resizeTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(sceneTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    //
    //
    //
    bool lWeCompareWithReference = (lSceneTaskEnum == GDC_CNN_SCENE_TASK_FILE_READER);

    libResult = mDisplayTask.Configure(displayTaskQueueId, sceneTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mDisplayTask.Spawn(gcDisplayTaskStackSize, gcDisplayTaskPriority, gcDisplayTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    libResult = mCompositionTask.Configure(compositionTaskQueueId, displayTaskQueueId, lWeCompareWithReference);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mCompositionTask.Spawn(gcCompositionTaskStackSize, gcCompositionTaskPriority, gcCompositionTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    libResult = mClassifierTask.Configure(classifierTaskQueueId, compositionTaskQueueId, lWeCompareWithReference);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mClassifierTask.Spawn(gcClassifierTaskStackSize, gcClassifierTaskPriority, gcClassifierTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    libResult = mResizeTask.Configure(resizeTaskQueueId, classifierTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mResizeTask.Spawn(gcResizeTaskStackSize, gcResizeTaskPriority, gcResizeTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    switch(lSceneTaskEnum)
    {
      case GDC_CNN_SCENE_TASK_FILE_READER:
      {
        libResult = mFileReaderTask.Configure(sceneTaskQueueId, resizeTaskQueueId);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        libResult = mFileReaderTask.Spawn(gcSceneTaskStackSize, gcSceneTaskPriority, gcSceneTaskName);
        SHOW_ERROR(libResult == LIB_SUCCESS);
        GDC_TRACE;
        break;
      }
      case GDC_CNN_SCENE_TASK_IMAGE_SENSOR:
      {
        libResult = mImageSensorTask.Configure(sceneTaskQueueId, resizeTaskQueueId);
        SHOW_ERROR(libResult == LIB_SUCCESS);

        libResult = mImageSensorTask.Spawn(gcSceneTaskStackSize, gcSceneTaskPriority, gcSceneTaskName);
        SHOW_ERROR(libResult == LIB_SUCCESS);
        GDC_TRACE;
        break;
      }
      case GDC_CNN_SCENE_TASK_IP_CAMERA:
      {
        libResult = mIpCameraTask.Configure(sceneTaskQueueId, resizeTaskQueueId);
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
          case GDC_CNN_SCENE_TASK_FILE_READER:
          {
            libResult = mFileReaderTask.TaskService();
            SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
            break;
          }
          case GDC_CNN_SCENE_TASK_IMAGE_SENSOR:
          {
            libResult = mImageSensorTask.TaskService();
            SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
            break;
          }
          case GDC_CNN_SCENE_TASK_IP_CAMERA:
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

        libResult = mResizeTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);

        libResult = mClassifierTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);

        libResult = mCompositionTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);

        libResult = mDisplayTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
      }
    }

    switch(lSceneTaskEnum)
    {
      case GDC_CNN_SCENE_TASK_FILE_READER:
      {
        libResult = mFileReaderTask.Join();
        SHOW_ERROR(libResult == LIB_SUCCESS);
        GDC_TRACE;
        break;
      }
      case GDC_CNN_SCENE_TASK_IMAGE_SENSOR:
      {
        libResult = mImageSensorTask.Join();
        SHOW_ERROR(libResult == LIB_SUCCESS);
        GDC_TRACE;
        break;
      }
      case GDC_CNN_SCENE_TASK_IP_CAMERA:
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

    libResult = mResizeTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    libResult = mClassifierTask.Join();
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
      libResult = mStdinTask.Delete();
      SHOW_ERROR(libResult == LIB_SUCCESS);
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
    libResult = MsgMgr_MessageQueueDestroy(classifierTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(resizeTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(sceneTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    mIsStarted = false;
  }

  GDC_TRACE;

  return libResult;
}
