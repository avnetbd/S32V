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

#ifndef GDCPDDISPLAYTASK_HPP
#define GDCPDDISPLAYTASK_HPP

#include <gdc_ldw_types.hpp>
#include "global_errors.h"
#include <stdint.h>



//**************************************************************************/
// Literals & definitions

struct GdcLdw_ProfilingData
{
   uint32_t                 mSceneCounter;
   uint64_t                 mTicks[gcApexCvEndTick];     // even number Entry, odd number Return
};


//**************************************************************************/
// Class declaration.

/**
 * \brief Class that configures and controls this task.
 *
 */
class GdcLdw_DisplayTask : public GdcLdw_Task
{
public:
   GdcLdw_DisplayTask();
   virtual ~GdcLdw_DisplayTask();

   /*!*********************************************************************************
    *  @brief Method called by the application to configure the task.
    */
   LIB_RESULT Configure(const int32_t cReceiveMessageQueueId,
                        const int32_t cSceneTaskMessageQueueId);

private:
   int32_t mReceiveMessageQueueId;
   int32_t mSceneTaskMessageQueueId;
   bool    mIsTaskClosing;
   bool    mIsConfigured;

   uint32_t mDisplayWidthInPixels;
   uint32_t mDisplayHeightInPixels;
   uint32_t mDisplayBitsPerPixel;

   bool     mWeRotateDisplay;

   void*    mpSceneRgb24_Rotated;
   uint32_t mSceneRgb24_Rotated_SizeInBytes;

   void*    mpSceneRgb24_Resized;
   uint32_t mSceneRgb24_Resized_SizeInBytes;

   io_FrameOutput_t mFrameOutput; // Used to display image on device

   /**********************************************************************************
    *  for application profiling
    */
   bool       mEnableProfile;
   uint32_t   mSamplingDuration;
   uint32_t   mStartingFrame;
   char       mOutputFilename[gcFileNameSize];
   char       mOutputfileFormat[4];
   vsdk::UMat mOutputDataUMat;
   vsdk::Mat  mOutputDataMat;

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
   LIB_RESULT DumpProfilingInBuffer(uint8_t* profilingOutputBuffer, GdcLdw_DetectionData& rData);
   LIB_RESULT DumpProfilingInFile(char* profilingoutputFilename, uint8_t* profilingData);
   LIB_RESULT HandleCompositionData(GdcLdw_DetectionData& rData);

private:
   GdcLdw_DisplayTask( const GdcLdw_DisplayTask& );            // Do not implement
   GdcLdw_DisplayTask& operator=( const GdcLdw_DisplayTask& ); // Do not implement
};

#endif /* GDCPDDISPLAYTASK_HPP */

