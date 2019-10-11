/**************************************************************************************************
 *
 * NXP Confidential Proprietary
 *
 * Copyright 2013-2018 NXP
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
#include <gdc_ldw_app.hpp>
#include "iniparser.h"
#include "communications.hpp"
#include <ShowMessage.h>

// OS Resources
const char     gcFileReaderTaskName[]    = "LdwInput";
const uint32_t gcFileReaderTaskStackSize = 4096;
const uint32_t gcFileReaderTaskPriority  = 19;

//const char   gcImageResizeTaskName[]    = "ImgResize";
//const uint32_t gcImageResizeTaskStackSize = 4096;
//const uint32_t gcImageResizeTaskPriority  = 16;

const char     gcDetectionTaskName[]    = "LdwDetect";
const uint32_t gcDetectionTaskStackSize = 4096;
const uint32_t gcDetectionTaskPriority  = 17;

const char     gcCompositionTaskName[]    = "LdwComp";
const uint32_t gcCompositionTaskStackSize = 4096;
const uint32_t gcCompositionTaskPriority  = 18;

const char     gcDisplayTaskName[]    = "LdwDisp";
const uint32_t gcDisplayTaskStackSize = 4096;
const uint32_t gcDisplayTaskPriority  = 18;

const char     gcStdinTaskName[]    = "Stdin";
const uint32_t gcStdinTaskStackSize = 16 * 4096;
const uint32_t gcStdinTaskPriority  = 15;

/***************************************************************************/
/* Locally defined global data */

/***************************************************************************/
/**
 * \par Description
 * GdcLdw_App Class Constructor Definition
 *
 **************************************************************************/
GdcLdw_App::GdcLdw_App()
    : mSceneWidthInPixels(0), mSceneHeightInPixels(0), mSceneColorModel(icp::DATATYPE_08U),
      //GdcLdw_FileReaderTask  mFileReaderTask;      // Input task
      //GdcLdw_DetectionTask   mDetectionTask;       // Processing task
      //GdcLdw_ResizeTask      mResizeTask;          // Resize Output image task
      //GdcLdw_DisplayTask     mDisplayTask;         // Output task
      mIsStarted(false)
{
}

/***************************************************************************/
/**
 * \par Description
 * GdcLdw_App Class Destructor Definition
 *
 **************************************************************************/
GdcLdw_App::~GdcLdw_App()
{
}

/***************************************************************************/
/**
 * \par Description
 * This function configures, creates and starts the various tasks involved
 * in the forward collision warning application.
 *
 * See ICP_SDK_Platform_Software_UserGuide document for further details
 * about the OAL Task State machine.
 *
 * It can be found in the /Documentation directory of the ICP SDK installation location.
 *
 * \return ::LIB_SUCCESS
 *
 **************************************************************************/
LIB_RESULT GdcLdw_App::Start()
{
  LIB_RESULT libResult = LIB_FAILURE;

  if(mIsStarted == false)
  {
    libResult = MsgMgr_Initialize();
    SHOW_ERROR(libResult == LIB_SUCCESS);

    int32_t stdinTaskQueueId       = 0;
    int32_t sceneTaskQueueId       = 0;
    int32_t detectionTaskQueueId   = 0;
    int32_t compositionTaskQueueId = 0;
    int32_t displayTaskQueueId     = 0;

    libResult = MsgMgr_MessageQueueCreate(stdinTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(sceneTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(detectionTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(compositionTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueCreate(displayTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    mSceneWidthInPixels  = gcSceneWidthInPixels;
    mSceneHeightInPixels = gcSceneHeightInPixels;
    mSceneColorModel     = gcSceneColorModel_DataType;

    bool lWeUseFileReader = gcWeUseFileReaderTask;

    //
    // Get info from .ini file
    //
    dictionary* pDictionary = 0;

    pDictionary = iniparser_load(gcIniFileName);

    if(pDictionary != 0)
    {
      mSceneWidthInPixels  = iniparser_getint(pDictionary, "App:SceneWidthInPixels", gcSceneWidthInPixels);
      mSceneHeightInPixels = iniparser_getint(pDictionary, "App:SceneHeightInPixels", gcSceneHeightInPixels);
      lWeUseFileReader     = (iniparser_getint(pDictionary, "App:WeUseFileReader", gcWeUseFileReaderTask) == 1);

      iniparser_freedict(pDictionary);
      pDictionary = 0;
    }

    if(lWeUseFileReader == true)
    {
      libResult = mFileReaderTask.Configure(sceneTaskQueueId, detectionTaskQueueId);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      libResult = mFileReaderTask.Spawn(gcFileReaderTaskStackSize, gcFileReaderTaskPriority, gcFileReaderTaskName);
      SHOW_ERROR(libResult == LIB_SUCCESS);
    }
    else
    {
      libResult = mImageSensorTask.Configure(sceneTaskQueueId, detectionTaskQueueId, gcSceneWidthInPixels,
                                             gcSceneHeightInPixels, gcSceneBitsPerPixel, gcScenePaddingInBytes);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      libResult = mImageSensorTask.Spawn(gcFileReaderTaskStackSize, gcFileReaderTaskPriority, gcFileReaderTaskName);
      SHOW_ERROR(libResult == LIB_SUCCESS);
    }

    libResult = mDetectionTask.Configure(mSceneWidthInPixels, mSceneHeightInPixels, mSceneColorModel,
                                         detectionTaskQueueId, compositionTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mDetectionTask.Spawn(gcDetectionTaskStackSize, gcDetectionTaskPriority, gcDetectionTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mCompositionTask.Configure(compositionTaskQueueId, displayTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mCompositionTask.Spawn(gcCompositionTaskStackSize, gcCompositionTaskPriority, gcCompositionTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mDisplayTask.Configure(displayTaskQueueId, sceneTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    libResult = mDisplayTask.Spawn(gcDisplayTaskStackSize, gcDisplayTaskPriority, gcDisplayTaskName);
    SHOW_ERROR(libResult == LIB_SUCCESS);

    if(gcIsRunningWithThreads == true)
    {
      libResult = mStdinTask.Configure(stdinTaskQueueId, sceneTaskQueueId);
      SHOW_ERROR(libResult == LIB_SUCCESS);

      libResult = mStdinTask.Spawn(gcStdinTaskStackSize, gcStdinTaskPriority, gcStdinTaskName);
      SHOW_ERROR(libResult == LIB_SUCCESS);
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
        libResult = mDetectionTask.TaskService();
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
    }
    else
    {
      libResult = mImageSensorTask.Join();
      SHOW_ERROR(libResult == LIB_SUCCESS);
    }

    libResult = mDetectionTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = mCompositionTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = mDisplayTask.Join();
    SHOW_ERROR(libResult == LIB_SUCCESS);

    if(gcIsRunningWithThreads == true)
    {
      libResult = GdcLdw_Helper::SendRequest(MESSAGE_ID_GDC_LDW_EXIT_REQUEST, 0, stdinTaskQueueId);
      SHOW_ERROR(libResult == LIB_SUCCESS || libResult == LIB_SENDING_NOT_ALLOWED);
      libResult = mStdinTask.Join();
      SHOW_ERROR(libResult == LIB_SUCCESS);
    }

    libResult = MsgMgr_MessageQueueDestroy(stdinTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(sceneTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(detectionTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(compositionTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
    libResult = MsgMgr_MessageQueueDestroy(displayTaskQueueId);
    SHOW_ERROR(libResult == LIB_SUCCESS);
  }
  else
  {
    SHOW_ERROR(false);
  }

  return libResult;
}
