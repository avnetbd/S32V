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

#ifndef GDCCNNDISPLAYTASK_HPP
#define GDCCNNDISPLAYTASK_HPP

#include "gdc_cnn_messages.hpp"
#include "gdc_cnn_types.hpp"
#include "global_errors.h"
#include <stdint.h>



//**************************************************************************/
// Literals & definitions

struct GdcCnn_ProfilingData
{
   uint32_t                 mSceneCounter;
   uint64_t                 mTicks[gcApexCvEndTick];     // even number Entry, odd number Return
};


//**************************************************************************/
// Class declaration.

/*
 *  \brief GdcCnn_DisplayTask
 *
 */
class GdcCnn_DisplayTask : public GdcCnn_Task
{
public:
   GdcCnn_DisplayTask();
   virtual ~GdcCnn_DisplayTask();

   LIB_RESULT Configure(const int32_t cReceiveMessageQueueId,
                        const int32_t cSceneMessageQueueId);

private:
   int32_t mReceiveMessageQueueId;
   int32_t mSceneMessageQueueId;
   bool    mIsTaskClosing;
   bool    mIsConfigured;

   GdcCnn_Display mDisplay;

   /**********************************************************************************
    *  for application profiling
    */
   bool       mEnableProfile;
   uint32_t   mSamplingDuration;
   uint32_t   mStartingFrame;
   char       mOutputFilename[gcFileNameSize];
   char       mOutputFileFormart[4];
   vsdk::UMat mOutputDataUMat;
   vsdk::Mat  mOutputDataMat;



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
   LIB_RESULT DumpProfilingInBuffer(uint8_t* profilingOutputBuffer, GdcCnn_CompositionData& rData);
   LIB_RESULT DumpProfilingInFile(char* profilingoutputFilename, uint8_t* profilingData);
   LIB_RESULT HandleCompositionData(GdcCnn_CompositionData& rData);

private:
   GdcCnn_DisplayTask( const GdcCnn_DisplayTask& );            // Do not implement
   GdcCnn_DisplayTask& operator=( const GdcCnn_DisplayTask& ); // Do not implement
};

#endif /* GDCCNNDISPLAYTASK_HPP */


