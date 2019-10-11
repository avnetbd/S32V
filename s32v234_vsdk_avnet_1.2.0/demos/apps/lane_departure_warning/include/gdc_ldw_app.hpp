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
*  \section fcw_desc Description
*
*  This file contains a class which configures, spawns and connects tasks which constitute the application.
*
***********************************************************************************/

#ifndef GDCLDWAPPLICATION_HPP
#define GDCLDWAPPLICATION_HPP


/***************************************************************************/
/* Include files. */
#include <gdc_ldw_file_reader_task.hpp>
#include <gdc_ldw_image_sensor_task.hpp>
#include <gdc_ldw_detection_task.hpp>
#include <gdc_ldw_composition_task.hpp>
#include <gdc_ldw_display_task.hpp>
#include <icp_data.h>      // replacement of ICP_Image_Descriptor.hpp
#include "global_errors.h"
#include <stdint.h>
#include "gdc_ldw_task_stdin.hpp"

//**************************************************************************/
// Literals & definitions

/**
 * \brief Class that configures, connects and spawns this application's tasks.
 *
 */
class GdcLdw_App
{
public:
   GdcLdw_App();
   virtual ~GdcLdw_App();

   LIB_RESULT Start();


private:
   int32_t                mSceneWidthInPixels;
   int32_t                mSceneHeightInPixels;
   icp::DATATYPE          mSceneColorModel;

   GdcLdw_FileReaderTask  mFileReaderTask;      // Input task
   GdcLdw_ImageSensorTask mImageSensorTask;     // Input task
   GdcLdw_DetectionTask   mDetectionTask;       // Processing task
   GdcLdw_CompositionTask mCompositionTask;     // Processing task
   GdcLdw_DisplayTask     mDisplayTask;         // Output task
   GdcLdw_StdinTask       mStdinTask;

   bool  mIsStarted;

private:
   GdcLdw_App( const GdcLdw_App& );            // Do not implement
   GdcLdw_App& operator=( const GdcLdw_App& ); // Do not implement
};

#endif /* GDCLDWAPPLICATION_HPP */
