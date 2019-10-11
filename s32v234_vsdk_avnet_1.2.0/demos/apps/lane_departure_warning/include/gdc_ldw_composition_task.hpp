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

#ifndef GDCLDWCOMPOSITIONTASK_HPP
#define GDCLDWCOMPOSITIONTASK_HPP

#include <gdc_ldw_types.hpp>
#include "global_errors.h"
#include <stdint.h>



//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.

/**
 * \brief Class that configures and controls this task.
 *
 */
class GdcLdw_CompositionTask : public GdcLdw_Task
{
public:
   GdcLdw_CompositionTask();
   virtual ~GdcLdw_CompositionTask();

   /*!*********************************************************************************
    *  @brief Method called by the application to configure the task.
    */
   LIB_RESULT Configure(const int32_t cReceiveMessageQueueId,
                        const int32_t cDisplayTaskMessageQueueId);

private:
   int32_t  mReceiveMessageQueueId;
   int32_t  mDisplayTaskMessageQueueId;
   bool     mIsTaskClosing;
   bool     mIsConfigured;

   GdcLdw_MovingAverage mMovingAverage_FramePeriod;
   uint64_t mFrameTicks;

   // Values read in from the .ini file
   bool             mWeDrawLanes;
   bool             mWeDrawAlarmIcon;
   bool             mWeDrawTopView;

   uint32_t         mAlarmIconWidthInPixels;
   uint32_t         mAlarmIconHeightInPixels;
   char*            mpAlarmIcon_NoLane;
   char*            mpAlarmIcon_InLane;
   char*            mpAlarmIcon_TurningRight;
   char*            mpAlarmIcon_TurningLeft;
   uint64_t         mDisplayTicks;

public:
   /*!*********************************************************************************
    *  @brief Method called in the context of the spawned task, contains initialization code.
    */
   virtual LIB_RESULT TaskOpen();

   /*!*********************************************************************************
    *  @brief Method called in the context of the spawned task, contains the main body.
    */
   virtual LIB_RESULT TaskService();

   /*!*********************************************************************************
    *  @brief Method called in the context of the spawned task, contains the uninitialization code.
    */
   virtual LIB_RESULT TaskClose();

private:
   LIB_RESULT HandleDetectionData(GdcLdw_DetectionData& rRequest);

private:
   GdcLdw_CompositionTask( const GdcLdw_CompositionTask& );            // Do not implement
   GdcLdw_CompositionTask& operator=( const GdcLdw_CompositionTask& ); // Do not implement
};

#endif /* GDCLDWCOMPOSITIONTASK_HPP */

