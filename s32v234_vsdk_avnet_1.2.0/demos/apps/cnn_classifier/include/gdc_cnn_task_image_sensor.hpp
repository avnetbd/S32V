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

#ifndef GDCCNNIMAGESENSORTASK_HPP
#define GDCCNNIMAGESENSORTASK_HPP

#include "gdc_cnn_types.hpp"
#include "global_errors.h"
#include <stdint.h>



//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.

/*
 *  \brief GdcCnn_ImageSensorTask
 *
 */
class GdcCnn_ImageSensorTask : public GdcCnn_Task
{
public:
   GdcCnn_ImageSensorTask();
   virtual ~GdcCnn_ImageSensorTask();

   LIB_RESULT Configure(const int32_t cReceiveMessageQueueId,
                        const int32_t cFeatureDetectorTaskMessageQueueId);

private:
   int32_t mReceiveMessageQueueId;
   int32_t mFeatureDetectorTaskMessageQueueId;
   bool    mIsTaskClosing;
   bool    mIsConfigured;

   //
   // Used to capture images from the sensor
   //
   sdi_grabber mSdiGrabber;

   //
   // Used to convert captured image into the resolution and image format needed by the algorithm
   //
   uint32_t mCropRoi[4];                          // Region of Interest: left, top, width, height
   uint32_t mCroppedAndConvertedImageSpanInBytes;
   uint32_t mCroppedAndConvertedImageBufferSizeInBytes;
   uint32_t mCroppedAndConvertedImageOffsetInBytes;

   uint32_t            mSceneCounter;
   uint32_t            mImageSensorWidthInPixels;
   uint32_t            mImageSensorHeightInPixels;
   
public:
   /*!*********************************************************************************
    *  @brief Method called in the context of the spawned task, contains initialization code.
    */
   LIB_RESULT TaskOpen();

   /*!*********************************************************************************
    *  @brief Method called in the context of the spawned task, contains the main body.
    */
   LIB_RESULT TaskService();

   /*!*********************************************************************************
    *  @brief Method called in the context of the spawned task, contains the uninitialization code.
    */
   LIB_RESULT TaskClose();

private:
   LIB_RESULT HandleImageRequest();

private:
   GdcCnn_ImageSensorTask( const GdcCnn_ImageSensorTask& );            // Do not implement
   GdcCnn_ImageSensorTask& operator=( const GdcCnn_ImageSensorTask& ); // Do not implement
};

#endif /* GDCCNNIMAGESENSORTASK_HPP */


