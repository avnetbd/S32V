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
const char     gcInputImageTaskName[]    = "ImgSensor";
const uint32_t gcInputImageTaskStackSize = 16 * 4096;
const uint32_t gcInputImageTaskPriority  = 28;

const char     gcOpenCvTaskName[]    = "OpenCv";
const uint32_t gcOpenCvTaskStackSize = 16 * 4096;
const uint32_t gcOpenCvTaskPriority  = 30;

const char     gcResizeTaskName[]    = "Resize";
const uint32_t gcResizeTaskStackSize = 16 * 4096;
const uint32_t gcResizeTaskPriority  = 30;

const char     gcHogTaskName[]    = "Hog";
const uint32_t gcHogTaskStackSize = 16 * 4096;
const uint32_t gcHogTaskPriority  = 30;

const char     gcCompositionTaskName[]    = "Compose";
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
    : //GdcPd_ImageSensorTask mImageSensorTask;
      //GdcPd_FileReaderTask  mFileReaderTask;
      //GdcPd_OpenCvTask      mOpenCvTask;
      //GdcPd_ResizeTask      mResizeTask;
      //GdcPd_HogTask         mHogTask;
      //GdcPd_DisplayTask     mDisplayTask;
      mWeUseApexResize(true), mIsStarted(false)
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

  bool lWeUseFileReader = gcWeUseFileReaderTask;

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
      mWeUseApexResize     = (iniparser_getint(pDictionary, "App:WeUseApexResize", 1) == 1);
      lWeUseFileReader     = (iniparser_getint(pDictionary, "App:WeUseFileReader", 0) == 1);
      lSceneWidthInPixels  = iniparser_getint(pDictionary, "App:SceneWidthInPixels", gcSceneWidthInPixels);
      lSceneHeightInPixels = iniparser_getint(pDictionary, "App:SceneHeightInPixels", gcSceneHeightInPixels);
      lSceneBitsPerPixel   = iniparser_getint(pDictionary, "App:SceneBitsPerPixel", gcSceneBitsPerPixel);

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }

    int32_t stdinTaskQueueId = 0;
    int32_t sceneTaskQueueId = 0;
#if defined(__CYGWIN__) || defined(_WIN32)
    int32_t opencvTaskQueueId = 0;
#endif
    int32_t resizeTaskQueueId      = 0;
    int32_t hogTaskQueueId         = 0;
    int32_t compositionTaskQueueId = 0;
    int32_t displayTaskQueueId     = 0;

    libResult = MsgMgr_MessageQueueCreate(stdinTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(sceneTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
#if defined(__CYGWIN__) || defined(_WIN32)
    libResult = MsgMgr_MessageQueueCreate(opencvTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
#endif
    libResult = MsgMgr_MessageQueueCreate(resizeTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(hogTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(compositionTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(displayTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    GdcPd_FrameSize sceneResizes[gcNumberOfSceneResizes];
    uint32_t        pyramidScales[gcNumberOfSceneResizes];

    GdcPd_Helper::ComputeNumOfMinorScales(gcMajorSceneResizes, pyramidScales, gcNumberOfMajorScales);

    libResult = GdcPd_Helper::GetResizeFrameSizes(gcSceneWidthInPixels, gcSceneHeightInPixels, gcNumberOfSceneResizes,
                                                  gcMajorSceneResizes, sceneResizes, pyramidScales, mWeUseApexResize);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    if(lWeUseFileReader == true)
    {
      //
      // File Reader Task
      //

      libResult = mFileReaderTask.Configure(sceneTaskQueueId, resizeTaskQueueId);
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
      libResult = mImageSensorTask.Configure(sceneTaskQueueId, resizeTaskQueueId, lSceneWidthInPixels,
                                             lSceneHeightInPixels, lSceneBitsPerPixel, lScenePaddingInBytes);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      libResult = mImageSensorTask.Spawn(gcInputImageTaskStackSize, gcInputImageTaskPriority, gcInputImageTaskName);
      SHOW_ERROR(libResult == LIB_SUCCESS);
      GDC_TRACE;
    }

#if defined(__CYGWIN__) || defined(_WIN32)
    //
    // OpenCv Task
    //
    libResult = mOpenCvTask.Configure(opencvTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mOpenCvTask.Spawn(gcOpenCvTaskStackSize, gcOpenCvTaskPriority, gcOpenCvTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;
#endif

    //
    // Resize Task
    //
    libResult =
        mResizeTask.Configure(resizeTaskQueueId, hogTaskQueueId, gcNumberOfSceneResizes, sceneResizes, pyramidScales);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mResizeTask.Spawn(gcResizeTaskStackSize, gcResizeTaskPriority, gcResizeTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    //
    // Hog Task
    //
    libResult = mHogTask.Configure(hogTaskQueueId, compositionTaskQueueId, gcNumberOfSceneResizes, sceneResizes);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mHogTask.Spawn(gcHogTaskStackSize, gcHogTaskPriority, gcHogTaskName);
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
#if defined(__CYGWIN__) || defined(_WIN32)
        libResult = mOpenCvTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);
#endif

        libResult = mResizeTask.TaskService();
        SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_APP_IS_CLOSING);

        libResult = mHogTask.TaskService();
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
#if defined(__CYGWIN__) || defined(_WIN32)
    libResult = mOpenCvTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;
#endif
    libResult = mResizeTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;
    libResult = mHogTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;
    libResult = mCompositionTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;
    libResult = mDisplayTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    GDC_TRACE;

    if(mHogTask.MismatchCounter_Get() > 0)
    {
      libResult = LIB_ERROR_OUTPUT_DOES_NOT_MATCH_REFERENCE;
    }

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
    libResult = MsgMgr_MessageQueueDestroy(sceneTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
#if defined(__CYGWIN__) || defined(_WIN32)
    libResult = MsgMgr_MessageQueueDestroy(opencvTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
#endif
    libResult = MsgMgr_MessageQueueDestroy(resizeTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(hogTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(compositionTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(displayTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    mIsStarted = false;
  }

  GDC_TRACE;

  return libResult;
}
