/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
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

#ifndef GDCFEATURETRACKINGPYRAMIDTASK_HPP
#define GDCFEATURETRACKINGPYRAMIDTASK_HPP

#include "gdc_ft_types.hpp"
#include "gdc_ft_types_verification.hpp"
#include "apexcv_pro_pyramid.h"
#include "apexcv_pro_pyramid_multi.h"
#include "image_pyramid_multi_ref.h"
#include <apexcv_pro_resize.h>
#include "global_errors.h"
#include <stdint.h>



//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.

/*
 *  \brief GdcFt_PyramidTask
 *
 */
class GdcFt_PyramidTask : public GdcFt_Task
{
public:
   GdcFt_PyramidTask();
   virtual ~GdcFt_PyramidTask();

   LIB_RESULT Configure(const int32_t cReceiveMessageQueueId,
                        const int32_t cTrackerTaskMessageQueueId);

private:
   int32_t mReceiveMessageQueueId;
   int32_t mTrackerTaskMessageQueueId;

   uint32_t mImagePyramidSizes[gcDepthOfImagePyramid][5]; // [0] Width, [1] Height, [2] Padded Width, [3] Padded Height, [4] Offset


   apexcv::PyramidMultiCreation mPyramidMultiCreation;
   ref::PyramidMultiCreation    mPyramidMultiCreation_ref;
   bool                         mIsPyramidMultiCreationInitialized;
   bool                         mWeUseApexPyramidMultiCreation;

   // Testing
   GdcFt_OutputFile_PyramidMultiCreation  mOutputFile;

   char    mOutputReadFilename[gcFileNameSize+1];
   char    mOutputWriteFilename[gcFileNameSize+1];
   bool    mWeReadOutputFile;
   bool    mWeWriteOutputFile;

   int32_t mReadMismatchCounter;

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
   LIB_RESULT HandleDetectorData(GdcFt_DetectorData& rData);

private:
   GdcFt_PyramidTask( const GdcFt_PyramidTask& );            // Do not implement
   GdcFt_PyramidTask& operator=( const GdcFt_PyramidTask& ); // Do not implement
};

#endif /* GDCFEATURETRACKINGPYRAMIDTASK_HPP */


