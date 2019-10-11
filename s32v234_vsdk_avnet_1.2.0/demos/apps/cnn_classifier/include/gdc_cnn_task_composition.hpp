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

#ifndef GDCCNNCOMPOSITIONTASK_HPP
#define GDCCNNCOMPOSITIONTASK_HPP

#include "gdc_cnn_messages.hpp"
#include "gdc_cnn_types.hpp"
#include "global_errors.h"
#include <stdint.h>



//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.

/*
 *  \brief GdcCnn_CompositionTask
 *
 */
class GdcCnn_CompositionTask : public GdcCnn_Task
{
public:
   GdcCnn_CompositionTask();
   virtual ~GdcCnn_CompositionTask();

   LIB_RESULT Configure(const int32_t cReceiveMessageQueueId,
                        const int32_t cDisplayMessageQueueId,
                        const bool cWeCompareWithReference);

private:
   int32_t mReceiveMessageQueueId;
   int32_t mDisplayMessageQueueId;
   bool    mWeCompareWithReference;
   bool    mIsTaskClosing;
   bool    mIsConfigured;

   GdcCnn_Composition mComposition;


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
    *  @brief Method called in the context of the spawned task, contains the deinitialization code.
    */
   LIB_RESULT TaskClose();

private:
   LIB_RESULT HandleClassifierData(GdcCnn_ClassifierData& rData);

private:
   GdcCnn_CompositionTask( const GdcCnn_CompositionTask& );            // Do not implement
   GdcCnn_CompositionTask& operator=( const GdcCnn_CompositionTask& ); // Do not implement
};

#endif /* GDCCNNCOMPOSITIONTASK_HPP */


