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

#ifndef GDCCNNFILEREADERTASK_HPP
#define GDCCNNFILEREADERTASK_HPP

#include "gdc_cnn_types.hpp"
#include "gdc_cnn_messages.hpp"
#include "apexcv_pro_resize.h"
#include "global_errors.h"
#include <stdint.h>



//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.

/*
 *  \brief GdcCnn_FileReaderTask
 *
 */
class GdcCnn_FileReaderTask : public GdcCnn_Task
{
public:
   GdcCnn_FileReaderTask();
   virtual ~GdcCnn_FileReaderTask();

   LIB_RESULT Configure(const int32_t cReceiveMessageQueueId,
                        const int32_t cResizeTaskMessageQueueId);

private:
   int32_t  mReceiveMessageQueueId;
   int32_t  mResizeTaskMessageQueueId;

   std::string                         mDatasetDirectory;
   std::vector<std::string>            mDatasetList;
   std::vector<std::string>::iterator  mDatasetList_Iterator;
   uint32_t                            mSceneCounter;

   uint32_t mFrameDurationInUs;
   timespec mTimeSpecLastSend;
   timespec mTimeSpecNow;
   timespec mTimeSpecDiff;
   timespec mTimeSpecSleep;

   bool     mWeLoop;
   bool     mIsTaskClosing;
   bool     mIsConfigured;

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
   LIB_RESULT HandleImageRequest(const GdcCnn_ImageRequest& rcRequest);

private:
   GdcCnn_FileReaderTask( const GdcCnn_FileReaderTask& );            // Do not implement
   GdcCnn_FileReaderTask& operator=( const GdcCnn_FileReaderTask& ); // Do not implement
};

#endif /* GDCCNNFILEREADERTASK_HPP */


