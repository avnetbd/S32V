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

#ifndef GDCCNNCLASSIFIERTASK_HPP
#define GDCCNNCLASSIFIERTASK_HPP

#include "gdc_cnn_types.hpp"
//#include "gdc_cnn_types_verification.hpp"
#include <squeeze_net_v10.hpp>
#include <squeeze_net_v11.hpp>
#include "global_errors.h"
#include <stdint.h>



//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.

/*
 *  \brief GdcCnn_ClassifierTask
 *
 */
class GdcCnn_ClassifierTask : public GdcCnn_Task
{
public:
   GdcCnn_ClassifierTask();
   virtual ~GdcCnn_ClassifierTask();

   LIB_RESULT Configure(const int32_t cReceiveMessageQueueId,
                        const int32_t cCompositionTaskMessageQueueId,
                        const bool cWeCompareWithReference);

private:
   int32_t mReceiveMessageQueueId;
   int32_t mCompositionTaskMessageQueueId;

   int8_t* mpSqueezeNet_WeightModel; // Classifier input

   bool mIsSqueezeNetInitialized;
#if SQUEEZENET11
   apexcv::SqueezeV11Net mSqueezeNet;	// Classifier itself
#else
   apexcv::SqueezeV10Net mSqueezeNet;    // Classifier itself
#endif

   std::vector<std::string> mSqueezeNet_SynonymSet; // Mapping between classifier index and string

   std::map<std::string, std::set<int32_t>> mReferenceResults; // Mapping between filename and SynonymSet indexes

   bool    mWeCompareWithReference;
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
   LIB_RESULT HandleResizeData(GdcCnn_ResizeData& rData);

private:
   GdcCnn_ClassifierTask( const GdcCnn_ClassifierTask& );            // Do not implement
   GdcCnn_ClassifierTask& operator=( const GdcCnn_ClassifierTask& ); // Do not implement
};

#endif /* GDCCNNCLASSIFIERTASK_HPP */


