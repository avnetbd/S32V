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

/*!*********************************************************************************
*  @file
*
***********************************************************************************/
#ifndef GDCPEDESTRIANDETECTIONTYPESDUMMY_HPP
#define GDCPEDESTRIANDETECTIONTYPESDUMMY_HPP


#if defined(__CYGWIN__) || defined(_WIN32)
/***************************************************************************/
/* Include files. */
#include "gdc_pd_types.hpp"
#include "frame_io_types.h"
#include "hog_config.h"
#include "icp_data.h"
#include "acf_process_apu.h"
#include <opencv2/objdetect.hpp>
#include <stdint.h>



class sdi_grabber
{
};

//class io_FrameOutput_t
//{
//public:
//   void Init( uint32_t aWidth,
//              uint32_t aHeight,
//              io::IO_DATA_DEPTH aDepth,
//              io::IO_DATA_CHANNELS aChannels,
//              uint32_t aFormat = 0);
//
//   void PutFrame(void *frame, bool aInvalidateCache);
//};

namespace apexcv
{
class Resize
{
public:
   int process_acf(icp::DataDescriptor& src, icp::DataDescriptor& dst, void *&dst_pad_oal);

   int SelectApuConfiguration(ACF_APU_CFG lApuConfig,
                              int32_t lApexId);
};

class Hog
{
public:
   Hog(hog_param p);

   int set_svm(double *svm_double);

   int detect(icp::DataDescriptor &input, icp::DataDescriptor &output, void* &p_output);

   int SelectApuConfiguration(ACF_APU_CFG lApuConfig,
                              int32_t lApexId);
};
}

class GdcPd_ImageSensorTask : public GdcPd_Task
{
public:
   GdcPd_ImageSensorTask();
   virtual ~GdcPd_ImageSensorTask();

   LIB_RESULT Configure(const int32_t cReceiveMessageQueueId,
                        const int32_t cResizeTaskMessageQueueId);
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
   GdcPd_ImageSensorTask( const GdcPd_ImageSensorTask& );            // Do not implement
   GdcPd_ImageSensorTask& operator=( const GdcPd_ImageSensorTask& ); // Do not implement
};


namespace opencv
{
   class Hog
   {
   public:
      Hog(hog_param lHogParams);

      int detect(icp::DataDescriptor &input,
                 icp::DataDescriptor &output,
                 void* &p_output);

      cv::HOGDescriptor mHogDescriptor;
      hog_param mHogParam;
   };
}
#endif

#endif /* GDCPEDESTRIANDETECTIONTYPESDUMMY_HPP */
