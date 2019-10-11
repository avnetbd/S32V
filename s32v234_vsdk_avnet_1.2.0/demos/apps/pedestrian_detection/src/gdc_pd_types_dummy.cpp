/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2015 Freescale Semiconductor;
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

/***************************************************************************/
/* Include files. */
#if defined(__CYGWIN__) || defined(_WIN32)

#include "gdc_pd_types_dummy.hpp"
#include <vector>

int ACF_Init()
{
}

int ACF_Deinit()
{
}

void io_FrameOutput_t::Init(uint32_t /*aWidth*/,
                            uint32_t /*aHeight*/,
                            io::IO_DATA_DEPTH /*aDepth*/,
                            io::IO_DATA_CHANNELS /*aChannels*/,
                            uint32_t /*aFormat*/)
{
}

void io_FrameOutput_t::PutFrame(void* /*frame*/, bool /*aInvalidateCache*/)
{
}

namespace apexcv
{
int Resize::process_acf(icp::DataDescriptor& /*src*/, icp::DataDescriptor& /*dst*/, void*& /*dst_pad_oal*/)
{
  return 0;
}

int Resize::SelectApuConfiguration(ACF_APU_CFG lApuConfig, int32_t lApexId)
{
  return 0;
}

Hog::Hog(hog_param /*p*/)
{
}

int Hog::set_svm(double* /*svm_double*/)
{
  return 0;
}

int Hog::detect(icp::DataDescriptor& /*input*/, icp::DataDescriptor& /*output*/, void*& /*p_output*/)
{
  return -1;
}

int Hog::SelectApuConfiguration(ACF_APU_CFG /*lApuConfig*/, int32_t /*lApexId*/)
{
  return 0;
}
}

GdcPd_ImageSensorTask::GdcPd_ImageSensorTask()
{
}

GdcPd_ImageSensorTask::~GdcPd_ImageSensorTask()
{
}

LIB_RESULT GdcPd_ImageSensorTask::Configure(const int32_t cReceiveMessageQueueId,
                                            const int32_t cResizeTaskMessageQueueId)
{
  return LIB_FAILURE;
}

LIB_RESULT GdcPd_ImageSensorTask::TaskOpen()
{
}

LIB_RESULT GdcPd_ImageSensorTask::TaskService()
{
}

LIB_RESULT GdcPd_ImageSensorTask::TaskClose()
{
}

namespace opencv
{
Hog::Hog(hog_param lHogParams)
{
  mHogDescriptor.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
  memcpy(&mHogParam, &lHogParams, sizeof(mHogParam));
}

int Hog::detect(icp::DataDescriptor& input, icp::DataDescriptor& output, void*& p_output)
{
  int ret = -1;

  const uint32_t cOutputCapacity = output.GetWidth() * output.GetHeight();

  if(output.GetDataPtr() == 0)
  {
    p_output = OAL_MemoryAllocFlag(cOutputCapacity * 2, OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) |
                                                            OAL_MEMORY_FLAG_CONTIGUOUS | OAL_MEMORY_FLAG_ZERO);
    output.UpdatePtrs(p_output, OAL_MemoryReturnAddress(p_output, ACCESS_PHY));
  }

  if(output.GetDataPtr() != 0 && input.GetDataPtr() != 0 && input.GetSpan() > 0 && input.GetHeight() > 0 &&
     input.GetWidth() > 0)
  {
    cv::Mat lImage_Mat(input.GetWidth(), input.GetHeight(), CV_8UC1, input.GetDataPtr());

    std::vector< cv::Point > lFoundLocations_VectorPoint;
    std::vector< double >    lWeights_VectorDouble;
    double                   lHitThreshold = 0.0;
    cv::Size                 lWinStride;
    cv::Size                 lPadding;
    std::vector< cv::Point > lSearchLocations;

    mHogDescriptor.detect(lImage_Mat, lFoundLocations_VectorPoint, lWeights_VectorDouble, lHitThreshold, lWinStride,
                          lPadding, lSearchLocations);

    if(lFoundLocations_VectorPoint.size() != 0)
    {
      printf("lFoundLocations_VectorPoint.size() [%d]\n", lFoundLocations_VectorPoint.size());
    }
    if(lWeights_VectorDouble.size() != 0)
    {
      printf("lWeights_VectorDouble.size() [%d]\n", lWeights_VectorDouble.size());
    }

    ret = 0;
  }

  return ret;
}
}
#endif
