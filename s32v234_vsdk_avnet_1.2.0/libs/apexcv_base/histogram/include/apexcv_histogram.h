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
/*!*********************************************************************************
*  \file apexcv_histogram.h
*  \brief APEX-CV interface for histogram
***********************************************************************************/

#ifndef APEXCVHISTOGRAM_H
#define APEXCVHISTOGRAM_H


#include <umat.hpp>
#include <apexcv_host_base_class.h>


namespace apexcv{

  /*!*********************************************************************************
  *  \brief          Histogram
  *  \details        Object of this class computes the histogram of the image.<br>
  *                  Output dimensions are 256x1 VSDK_CV_32SC1.<br>
  *                  Supported input type: VSDK_CV_8UC1.<br>
  *                  Supported width: 128 to 2048 pixels.
  ***********************************************************************************/
  class Histogram : public ApexcvHostBaseClass
  {
  public:

    /***********************************************************************************
    *  Destructor, not documented.
    ***********************************************************************************/
    ~Histogram();

    /*!*********************************************************************************
    *  \brief        Initialize object (required).
    *  \details      This function initializes the object.  The function Process() can
    *                be called to execute the processing on the APEX core.  To process
    *                another image buffer, use ReconnectIO(...).
    *  \return       APEXCV_LIB_RESULT Error code.
    ***********************************************************************************/
    APEXCV_LIB_RESULT Initialize(
      vsdk::UMat&    aSrc /*!< [in] Source image buffer (VSDK_CV_8UC1). */,
      vsdk::UMat&    aDst /*!< [in,out] Destination image buffer 256x1 (VSDK_CV_32SC1). */);

    /*!*********************************************************************************
    *  \brief        Reconnect IO (optional).
    *  \details      This function allows to change the Input and Output images without re-initializing.
    *  \return       APEXCV_LIB_RESULT Error code.
    ***********************************************************************************/
    APEXCV_LIB_RESULT ReconnectIO(
      vsdk::UMat&    aSrc /*!< [in] Source image buffer (VSDK_CV_8UC1). */,
      vsdk::UMat&    aDst /*!< [in,out] Destination image buffer 256x1 (VSDK_CV_32SC1). */);


  };
}
#endif /* APEXCVHISTOGRAM_H */
