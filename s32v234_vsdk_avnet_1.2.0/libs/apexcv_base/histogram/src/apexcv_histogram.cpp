/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2016-2017 NXP Semiconductor;
* All Rights Reserved
*
*****************************************************************************
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
*  \file apexcv_histogram.cpp
*  \brief APEX-CV interface for histogram
***********************************************************************************/

#include <apexcv_histogram.h>
#include <umat.hpp>

#ifndef APEX2_EMULATE
#include <acf_process_apu.h>
#include <HISTOGRAM.hpp>
#else
#include "apu_lib.hpp"
#include "apu_extras.hpp"
#include "acf_lib.hpp"
using namespace APEX2;
#include "histogram_apu_process_desc.hpp"
REGISTER_PROCESS_TYPE(HISTOGRAM, histogram_apu_process_desc);
#endif


#include "graph_templates.h"

#include <apexcv_error_codes.hpp>
#include <../graphs/histogram_graph_names.h>



namespace apexcv{


  Histogram::~Histogram()
  {
    if (0 != mType){
      delete (HISTOGRAM *)mpProcess;
    }
  }


  APEXCV_LIB_RESULT Histogram::Initialize(
    vsdk::UMat&    aSrc,
    vsdk::UMat&    aDst)
  {

    if (0 != mType){
      return APEXCV_ERROR_OBJECT_ALREADY_INITIALIZED;
    }

    if (aSrc.type() == VSDK_CV_8UC1 &&
      aDst.type() == VSDK_CV_32SC1)
    {

      HISTOGRAM *pf = new HISTOGRAM;
      if (NULL == pf){
        return APEXCV_ERROR_MEMORY_ALLOCATION_FAILED;
      }
      mpProcess = pf;
      mvInfoClusters.push_back(InfoCluster((void*)((ACF_Process_APU*)pf), GR_IN_0));

      int lRetVal = 0;
      lRetVal |= pf->Initialize();
      if (0 != lRetVal)
      {
        delete pf;
        mpProcess = nullptr;
        return APEXCV_ERROR_ACF_PROC_INITIALIZE;
      }
      lRetVal |= pf->ConnectIO(HISTOGR_IN_IMG, aSrc);
      lRetVal |= pf->ConnectIO(HISTOGR_OUT_HISTO, aDst);
      if (0 != lRetVal)
      {
        delete pf;
        mpProcess = nullptr;
        return APEXCV_ERROR_ACF_PROC_CONNECTIO;
      }
      mType = 1;
      return APEXCV_SUCCESS;
    }
    else{
      return APEXCV_ERROR_INVALID_ARGUMENT;
    }
  }


  APEXCV_LIB_RESULT Histogram::ReconnectIO(
    vsdk::UMat&    aSrc,
    vsdk::UMat&    aDst)
  {
    switch (mType)
    {
    case 0:{
      return APEXCV_ERROR_OBJECT_ISNOT_INITIALIZED;
    }break;
    case 1:
      if (aSrc.type() == VSDK_CV_8UC1 &&
        aDst.type() == VSDK_CV_32SC1)
      {
        HISTOGRAM *pf = (HISTOGRAM*)mpProcess;
        int lRetVal = 0;
        lRetVal |= pf->ConnectIO(HISTOGR_IN_IMG, aSrc);
        lRetVal |= pf->ConnectIO(HISTOGR_OUT_HISTO, aDst);
        if (0 != lRetVal)
        {
          return APEXCV_ERROR_ACF_PROC_CONNECTIO;
        }
        return APEXCV_SUCCESS;
      }
      else{
        return APEXCV_ERROR_INVALID_ARGUMENT;
      }
      break;
    default:
      return APEXCV_ERROR_INTERNAL_ERROR;
      break;
    }
  }


}
