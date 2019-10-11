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

#ifndef GDCLDWFILEREADERTASK_HPP
#define GDCLDWFILEREADERTASK_HPP

#include "gdc_ldw_types.hpp"
#include "gdc_ldw_messages.hpp"
#include "global_errors.h"
#include <stdint.h>



//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.

/*
 *  \brief GdcLdw_FileReaderTask
 *
 */
class GdcLdw_FileReaderTask : public GdcLdw_Task
{
public:
   GdcLdw_FileReaderTask();
   virtual ~GdcLdw_FileReaderTask();

   LIB_RESULT Configure(const int32_t cReceiveMessageQueueId,
                        const int32_t cDetectorTaskMessageQueueId);

private:
   int32_t mReceiveMessageQueueId;
   int32_t mDetectorTaskMessageQueueId;
   int32_t mSceneCounter;
   char    mFileName[gcFileNameSize+1];
   FILE*   mpFile;
   bool    mWeRunVideoInALoop;
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
   LIB_RESULT HandleImageRequest(const GdcLdw_ImageRequest& rcRequest);

private:
   GdcLdw_FileReaderTask( const GdcLdw_FileReaderTask& );            // Do not implement
   GdcLdw_FileReaderTask& operator=( const GdcLdw_FileReaderTask& ); // Do not implement
};

#endif /* GDCLDWFILEREADERTASK_HPP */


