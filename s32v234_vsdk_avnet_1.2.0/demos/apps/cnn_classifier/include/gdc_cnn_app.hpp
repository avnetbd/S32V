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
*  @brief Public API provided to users to instantiate this application.
*
*
*  This file contains a class which configures, spawns and connects tasks which constitute the application.
*
***********************************************************************************/

#ifndef GDCCNNAPP_HPP
#define GDCCNNAPP_HPP

/***************************************************************************/
/* Include files. */
#include "gdc_cnn_task_image_sensor.hpp"
#include "gdc_cnn_task_file_reader.hpp"
#include "gdc_cnn_task_ip_camera.hpp"
#include "gdc_cnn_task_resize.hpp"
#include "gdc_cnn_task_classifier.hpp"
#include "gdc_cnn_task_composition.hpp" 
#include "gdc_cnn_task_display.hpp"
#include "gdc_cnn_task_stdin.hpp"
#include "global_errors.h"
#include <stdint.h>


//**************************************************************************/
// Literals & definitions

/**
 * \brief Class that configures, connects and spawns this application's tasks.
 *
 */
class GdcCnn_App
{
public:
   GdcCnn_App();
   virtual ~GdcCnn_App();

   LIB_RESULT Start();

private:
   GdcCnn_ImageSensorTask  mImageSensorTask; // Scene Task
   GdcCnn_FileReaderTask   mFileReaderTask;  // Scene Task
   GdcCnn_IpCameraTask     mIpCameraTask;    // Scene Task

   GdcCnn_ResizeTask       mResizeTask;
   GdcCnn_ClassifierTask   mClassifierTask;
   GdcCnn_CompositionTask  mCompositionTask;
   GdcCnn_DisplayTask      mDisplayTask;

   GdcCnn_StdinTask        mStdinTask;

   bool mIsStarted;

private:
   GdcCnn_App( const GdcCnn_App& );            // Do not implement
   GdcCnn_App& operator=( const GdcCnn_App& ); // Do not implement
};

#endif /* GDCCNNAPP_HPP */
