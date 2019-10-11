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

#ifndef GDCAGGCFFEATURETASK_HPP
#define GDCAGGCFFEATURETASK_HPP

#include "gdc_pd_types.hpp"
#include "gdc_pd_types_dummy.hpp"
#include "gdc_pd_types_verification.hpp"
#include "global_errors.h"
#include <stdint.h>



//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.

/*
 *  \brief GdcPd_AggCFTask
 *
 */
class GdcPd_AggcfLuvTask : public GdcPd_Task
{
public:
   GdcPd_AggcfLuvTask();
   virtual ~GdcPd_AggcfLuvTask();

   LIB_RESULT Configure(const int32_t          cReceiveMessageQueueId,
                        const int32_t          cRealScaleTaskMessageQueueId,
                        const GdcPd_FrameSize  cSceneSize);

   int32_t MismatchCounter_Get() const;

private:
   int32_t mReceiveMessageQueueId;
   int32_t mRealScaleTaskMessageQueueId;
   int32_t mApproxScaleTaskMessageQueueId;

   GdcPd_FrameSize     mSceneSize;

   bool mWeUseApexAggcf;
   int32_t mIsScaledataReady;

   bool    mIsTaskClosing;
   bool    mIsConfigured;
   vsdk::UMat* mpOut_py_umat[gcInputImageQueueSize];
   vsdk::UMat  mScene_umat;
   apexcv::AggCFDetector mAcf_detector;
   uint32_t mProcessNum;

   uint64_t mfeatureTicks[2];

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
   LIB_RESULT HandleImageReply(GdcPd_ImageReply& rResizeData);

private:
   GdcPd_AggcfLuvTask( const GdcPd_AggcfLuvTask& );            // Do not implement
   GdcPd_AggcfLuvTask& operator=( const GdcPd_AggcfLuvTask& ); // Do not implement
};

#endif /* GDCAGGCFFEATURETASK_HPP */


