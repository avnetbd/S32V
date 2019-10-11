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
#include "gdc_ft_configuration.hpp"
#include "gdc_ft_app.hpp"
#include "gdc_ft_messages.hpp"
#include "GDC_OAL_Semaphore.hpp"
#include "communications.hpp"
#include <stdint.h>
#include "iniparser.h"

// OS Ressources
const char     gcInputImageTaskName[]    = "Scene";
const uint32_t gcInputImageTaskStackSize = 16 * 4096;
const uint32_t gcInputImageTaskPriority  = 28;

const char     gcDetectorTaskName[]    = "Detector";
const uint32_t gcDetectorTaskStackSize = 16 * 4096;
const uint32_t gcDetectorTaskPriority  = 30;

const char     gcPyramidTaskName[]    = "Pyramid";
const uint32_t gcPyramidTaskStackSize = 16 * 4096;
const uint32_t gcPyramidTaskPriority  = 30;

const char     gcTrackerTaskName[]    = "Tracker";
const uint32_t gcTrackerTaskStackSize = 16 * 4096;
const uint32_t gcTrackerTaskPriority  = 30;

const char     gcCompositionTaskName[]    = "Composition";
const uint32_t gcCompositionTaskStackSize = 16 * 4096;
const uint32_t gcCompositionTaskPriority  = 29;

const char     gcDisplayTaskName[]    = "Display";
const uint32_t gcDisplayTaskStackSize = 32 * 4096;
const uint32_t gcDisplayTaskPriority  = 29;

const char     gcStdinTaskName[]    = "Stdin";
const uint32_t gcStdinTaskStackSize = 16 * 4096;
const uint32_t gcStdinTaskPriority  = 27;

/***************************************************************************/
/* Locally defined global data */

/***************************************************************************/
/**
 * \par Description
 * GdcFt_Application Class Constructor Definition
 *
 * \warning 
 * \todo    
 * \bug  
 **************************************************************************/
GdcFt_App::GdcFt_App()
    : //GdcFt_ImageSensorTask mImageSensorTask;
      //GdcFt_FileReaderTask  mFileReaderTask;
      //GdcFt_DetectorTask    mDetectorTask;
      //GdcFt_PyramidTask     mPyramidTask;
      //GdcFt_TrackerTask     mTrackerTask;
      //GdcFt_DisplayTask     mDisplayTask;
      mIsStarted(false)
{
}

/***************************************************************************/
/**
 * \par Description
 * GdcFt_Application Class Destructor Definition
 *
 **************************************************************************/
GdcFt_App::~GdcFt_App()
{
}

/***************************************************************************/
/**
 * \par Description
 *
 **************************************************************************/
LIB_RESULT GdcFt_App::Start()
{
  GDC_TRACE;

  LIB_RESULT libResult        = LIB_FAILURE;
  bool       lWeUseFileReader = gcWeUseFileReaderTask;

  int32_t  lSceneWidthInPixels  = gcSceneWidthInPixels;
  int32_t  lSceneHeightInPixels = gcSceneHeightInPixels;
  uint32_t lSceneBitsPerPixel   = gcSceneBitsPerPixel;
  uint32_t lScenePaddingInBytes = gcScenePaddingInBytes;

  if(mIsStarted == false)
  {
    libResult = MsgMgr_Initialize();
    SHOW_ERROR(libResult == LIB_SUCCESS);

    dictionary* pDictionary = 0;

    pDictionary = iniparser_load(gcIniFileName);

    if(pDictionary != 0)
    {
      lWeUseFileReader     = (iniparser_getint(pDictionary, "App:WeUseFileReader", lWeUseFileReader) == 1);
      lSceneWidthInPixels  = iniparser_getint(pDictionary, "App:SceneWidthInPixels", gcSceneWidthInPixels);
      lSceneHeightInPixels = iniparser_getint(pDictionary, "App:SceneHeightInPixels", gcSceneHeightInPixels);
      lSceneBitsPerPixel   = iniparser_getint(pDictionary, "App:SceneBitsPerPixel", gcSceneBitsPerPixel);
      lScenePaddingInBytes = iniparser_getint(pDictionary, "App:ScenePaddingInBytes", gcScenePaddingInBytes);

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }

    int32_t sceneTaskQueueId       = 0;
    int32_t detectorTaskQueueId    = 0;
    int32_t pyramidTaskQueueId     = 0;
    int32_t trackerTaskQueueId     = 0;
    int32_t compositionTaskQueueId = 0;
    int32_t displayTaskQueueId     = 0;
    int32_t stdinTaskQueueId       = 0;

    libResult = MsgMgr_MessageQueueCreate(sceneTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(detectorTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(pyramidTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(trackerTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(compositionTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(displayTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(stdinTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    if(lWeUseFileReader == true)
    {
      //
      // File Reader Task
      //
      libResult = mFileReaderTask.Configure(sceneTaskQueueId, detectorTaskQueueId);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      libResult = mFileReaderTask.Spawn(gcInputImageTaskStackSize, gcInputImageTaskPriority, gcInputImageTaskName);
      SHOW_ERROR(libResult == LIB_SUCCESS);
      GDC_TRACE;
    }
    else
    {
      //
      // Image Sensor Task
      //
      libResult = mImageSensorTask.Configure(sceneTaskQueueId, detectorTaskQueueId, lSceneWidthInPixels,
                                             lSceneHeightInPixels, lSceneBitsPerPixel, lScenePaddingInBytes);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      libResult = mImageSensorTask.Spawn(gcInputImageTaskStackSize, gcInputImageTaskPriority, gcInputImageTaskName);
      SHOW_ERROR(libResult == LIB_SUCCESS);
      GDC_TRACE;
    }

    libResult = mDetectorTask.Configure(detectorTaskQueueId, pyramidTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mDetectorTask.Spawn(gcDetectorTaskStackSize, gcDetectorTaskPriority, gcDetectorTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    libResult = mPyramidTask.Configure(pyramidTaskQueueId, trackerTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mPyramidTask.Spawn(gcPyramidTaskStackSize, gcPyramidTaskPriority, gcPyramidTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    libResult = mTrackerTask.Configure(trackerTaskQueueId, compositionTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mTrackerTask.Spawn(gcTrackerTaskStackSize, gcTrackerTaskPriority, gcTrackerTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    libResult = mCompositionTask.Configure(compositionTaskQueueId, displayTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mCompositionTask.Spawn(gcCompositionTaskStackSize, gcCompositionTaskPriority, gcCompositionTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

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
        if(lWeUseFileReader == true)
        {
          libResult = mFileReaderTask.TaskService();
          SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
        }
        else
        {
          libResult = mImageSensorTask.TaskService();
          SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
        }

        libResult = mDetectorTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);

        libResult = mPyramidTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);

        libResult = mTrackerTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);

        libResult = mCompositionTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);

        libResult = mDisplayTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
      }
    }

    if(lWeUseFileReader == true)
    {
      libResult = mFileReaderTask.Join();
      SHOW_ERROR(libResult == LIB_SUCCESS);
      GDC_TRACE;
    }
    else
    {
      libResult = mImageSensorTask.Join();
      SHOW_ERROR(libResult == LIB_SUCCESS);
      GDC_TRACE;
    }
    libResult = mDetectorTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    libResult = mPyramidTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    libResult = mTrackerTask.Join();
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
      libResult = GdcFt_Helper::SendRequest(MESSAGE_ID_GDC_FT_EXIT_REQUEST, 0, stdinTaskQueueId);
      SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_SENDING_NOT_ALLOWED);

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
    libResult = MsgMgr_MessageQueueDestroy(trackerTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(pyramidTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(detectorTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(sceneTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    if(mTrackerTask.MismatchCounter_Get() > 0)
    {
      libResult = LIB_ERROR_OUTPUT_DOES_NOT_MATCH_REFERENCE;
    }

    mIsStarted = false;
  }

  GDC_TRACE;

  return libResult;
}
