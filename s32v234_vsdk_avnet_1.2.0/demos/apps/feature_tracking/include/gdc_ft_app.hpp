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

#ifndef GDCFEATURETRACKINGAPP_HPP
#define GDCFEATURETRACKINGAPP_HPP

/***************************************************************************/
/* Include files. */
#include "gdc_ft_image_sensor_task.hpp"
#include "gdc_ft_file_reader_task.hpp"
#include "gdc_ft_detector_task.hpp"
#include "gdc_ft_pyramid_task.hpp"
#include "gdc_ft_tracker_task.hpp"
#include "gdc_ft_composition_task.hpp"
#include "gdc_ft_display_task.hpp"
#include "gdc_ft_task_stdin.hpp"
#include "global_errors.h"
#include <stdint.h>


//**************************************************************************/
// Literals & definitions

/**
 * \brief Class that configures, connects and spawns this application's tasks.
 *
 */
class GdcFt_App
{
public:
   GdcFt_App();
   virtual ~GdcFt_App();

   LIB_RESULT Start();   

private:
   GdcFt_ImageSensorTask mImageSensorTask;
   GdcFt_FileReaderTask  mFileReaderTask;
   GdcFt_DetectorTask    mDetectorTask;
   GdcFt_PyramidTask     mPyramidTask;
   GdcFt_TrackerTask     mTrackerTask;
   GdcFt_CompositionTask mCompositionTask;
   GdcFt_DisplayTask     mDisplayTask;
   GdcFt_StdinTask       mStdinTask;

   bool mIsStarted;

private:
   GdcFt_App( const GdcFt_App& );            // Do not implement
   GdcFt_App& operator=( const GdcFt_App& ); // Do not implement
};

#endif /* GDCFEATURETRACKINGAPP_HPP */
