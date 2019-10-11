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

#ifndef GDCPEDESTRIANDETECTIONRESIZETASK_HPP
#define GDCPEDESTRIANDETECTIONRESIZETASK_HPP

#include "gdc_pd_types.hpp"
#include "gdc_pd_types_dummy.hpp"
#if !defined(__CYGWIN__) && !defined(_WIN32)
#include "apexcv_pro_resize.h"
#include "apexcv_pro_pyramid.h"

#endif
#include "resize_ref.h"
#include "global_errors.h"
#include <stdint.h>

#define PADDING 64



//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.

/*
 *  \brief GdcPd_ResizeTask
 *
 */
class GdcPd_ResizeTask : public GdcPd_Task
{
public:
   GdcPd_ResizeTask();
   virtual ~GdcPd_ResizeTask();

   LIB_RESULT Configure(const int32_t                cReceiveMessageQueueId,
                        const int32_t                cHogTaskMessageQueueId,
                        const uint32_t               cNumberOfSceneResizes,
                        const GdcPd_FrameSize* const cpcSceneResizes,
                        uint32_t                     pyramidScales []);

private:
   int32_t mReceiveMessageQueueId;
   int32_t mHogTaskMessageQueueId;

   uint32_t        mNumberOfSceneResizes;
   GdcPd_FrameSize mSceneResizes[gcNumberOfSceneResizes];

   apexcv::PyramidCreation mPyramidCreation;
   apexcv::Resize          mResize;
   ref::Resize         mResizeRef;
   uint32_t            mIsResizeInitialized;

   vsdk::UMat  mSceneResize_umat[gcNumberOfSceneResizes];
   const void* mpSceneResize_umat_data[gcNumberOfSceneResizes];
   uint32_t    mPyramidScales[gcNumberOfMajorScales];


   bool    mWeUseApexResize;
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
   GdcPd_ResizeTask( const GdcPd_ResizeTask& );            // Do not implement
   GdcPd_ResizeTask& operator=( const GdcPd_ResizeTask& ); // Do not implement
};


#endif /* GDCPEDESTRIANDETECTIONRESIZETASK_HPP */


