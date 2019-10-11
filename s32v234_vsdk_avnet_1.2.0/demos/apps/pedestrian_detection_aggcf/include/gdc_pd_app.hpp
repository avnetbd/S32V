/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
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
#include "gdc_pd_task_image_sensor.hpp"
#include "gdc_pd_task_file_reader.hpp"
#include "gdc_pd_task_ip_camera.hpp"
#include "gdc_pd_task_aggcf_luv.hpp"
#include "gdc_pd_task_aggcf_real_scale.hpp"
#include "gdc_pd_task_aggcf_approx_scale.hpp"
#include "gdc_pd_task_composition.hpp"
#include "gdc_pd_task_aggcf_detector.hpp"
#include "gdc_pd_task_display.hpp"
#include "gdc_pd_task_stdin.hpp"
#include "global_errors.h"
#include <stdint.h>


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
   GdcPd_ImageSensorTask      mImageSensorTask;
   GdcPd_FileReaderTask       mFileReaderTask;
   GdcPd_IpCameraTask         mIpCameraTask;
   GdcPd_AggcfDetectorTask    mAggcfDetectorTask;
   GdcPd_AggcfLuvTask         mAggcfLuvTask;
   GdcPd_AggcfApproxScaleTask mAggcfApproxScaleTask;
   GdcPd_AggcfRealScaleTask   mAggcfRealScaleTask;
   GdcPd_CompositionTask      mCompositionTask;
   GdcPd_DisplayTask          mDisplayTask;

   GdcPd_StdinTask            mStdinTask;

   bool mIsStarted;

private:
   GdcPd_App( const GdcPd_App& );            // Do not implement
   GdcPd_App& operator=( const GdcPd_App& ); // Do not implement
};

#endif /* GDCPEDESTRIANDETECTIONAPP_HPP */
