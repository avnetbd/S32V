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

#ifndef GDCPEDESTRIANDETECTIONAPP_HPP
#define GDCPEDESTRIANDETECTIONAPP_HPP

/***************************************************************************/
/* Include files. */
#if !defined(__CYGWIN__) && !defined(_WIN32)
#include "gdc_pd_image_sensor_task.hpp"
#else
#include "gdc_pd_opencv_task.hpp"
#endif
#include "gdc_pd_file_reader_task.hpp"
#include "gdc_pd_resize_task.hpp"
#include "gdc_pd_hog_task.hpp"
#include "gdc_pd_composition_task.hpp"
#include "gdc_pd_display_task.hpp"
#include "global_errors.h"
#include <stdint.h>
#include "gdc_pd_task_stdin.hpp"

//**************************************************************************/
// Literals & definitions

/**
 * \brief Class that configures, connects and spawns this application's tasks.
 *
 */
class GdcPd_App
{
public:
   GdcPd_App();
   virtual ~GdcPd_App();

   LIB_RESULT Start();   

private:
   GdcPd_ImageSensorTask mImageSensorTask;
   GdcPd_FileReaderTask  mFileReaderTask;
#if defined(__CYGWIN__) || defined(_WIN32)
   GdcPd_OpenCvTask      mOpenCvTask;
#endif
   GdcPd_ResizeTask      mResizeTask;
   GdcPd_HogTask         mHogTask;
   GdcPd_CompositionTask mCompositionTask;
   GdcPd_DisplayTask     mDisplayTask;

   GdcPd_StdinTask            mStdinTask;
   bool mWeUseApexResize;
   bool mIsStarted;

private:
   GdcPd_App( const GdcPd_App& );            // Do not implement
   GdcPd_App& operator=( const GdcPd_App& ); // Do not implement
};

#endif /* GDCPEDESTRIANDETECTIONAPP_HPP */
