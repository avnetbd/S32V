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

#ifndef GDCPEDESTRIANDETECTIONCOMPOSITIONTASK_HPP
#define GDCPEDESTRIANDETECTIONCOMPOSITIONTASK_HPP

#include "gdc_pd_messages.hpp"
#include "gdc_pd_types.hpp"
#include "global_errors.h"
#include <stdint.h>



//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.

/*
 *  \brief GdcPd Task
 *
 */
class GdcPd_CompositionTask : public GdcPd_Task
{
public:
   GdcPd_CompositionTask();
   virtual ~GdcPd_CompositionTask();

   LIB_RESULT Configure(const int32_t cReceiveMessageQueueId,
                        const int32_t cDisplayTaskMessageQueueId);

private:
   int32_t mReceiveMessageQueueId;
   int32_t mDisplayTaskMessageQueueId;
   bool    mIsTaskClosing;
   bool    mIsConfigured;

   //
   // Used to display image on device
   //
   GdcPd_Composition mComposition;

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
   LIB_RESULT HandleHogData(GdcPd_HogData& rHogData);

private:
   GdcPd_CompositionTask( const GdcPd_CompositionTask& );            // Do not implement
   GdcPd_CompositionTask& operator=( const GdcPd_CompositionTask& ); // Do not implement
};

#endif /* GDCPEDESTRIANDETECTIONCOMPOSITIONTASK_HPP */


