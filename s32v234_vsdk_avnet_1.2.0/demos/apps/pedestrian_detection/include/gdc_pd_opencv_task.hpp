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

#ifndef GDCPEDESTRIANDETECTIONOPENCVTASK_HPP
#define GDCPEDESTRIANDETECTIONOPENCVTASK_HPP

#include "gdc_pd_types.hpp"
#include "global_errors.h"
#include <opencv2/opencv.hpp>
#include <stdint.h>


//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.

/*
 *  \brief GdcPd_OpenCvTask
 *
 */
class GdcPd_OpenCvTask : public GdcPd_Task
{
public:
   GdcPd_OpenCvTask();
   virtual ~GdcPd_OpenCvTask();

   LIB_RESULT Configure(const int32_t cReceiveMessageQueueId);

private:
   int32_t mReceiveMessageQueueId;

   cv::HOGDescriptor mHogDescriptor;

   bool    mIsTaskClosing;
   bool    mIsConfigured;

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
   LIB_RESULT HandleImageReply(GdcPd_ImageReply& rImageReply);


private:
   GdcPd_OpenCvTask( const GdcPd_OpenCvTask& );            // Do not implement
   GdcPd_OpenCvTask& operator=( const GdcPd_OpenCvTask& ); // Do not implement
};

#endif /* GDCPEDESTRIANDETECTIONOPENCVTASK_HPP */


