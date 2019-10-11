/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2011-2016 Freescale Semiconductor
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
*  @brief Implements a Kalman filter.
*
*  \section ldw_desc Description
*
*  This file contains the implementation of a Kalman filter
*
***********************************************************************************/

 
#ifndef ICLKALMAN_HPP
#define ICLKALMAN_HPP

/////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////

#include "ICP_Types.hpp"

/////////////////////////////////////////////////////////////////////////
/// \par Descriptions:
/// This function...
///
/// \param edg - [Output] reference of destination edge image

///
/// \return
/// \return ICP_R_OK - if edge detection process success.
/// \return ICP_R_UNKNOWN_ERR - if image descriptor is not valid.
/////////////////////////////////////////////////////////////////////////




class ICP_KalmanFilter
{
public:
   ICP_KalmanFilter();
   ICP_KalmanFilter(int const cDynamParams,
                    int const cMeasureParams,
                    int const cControlParams);

   ~ICP_KalmanFilter();

   LIB_RESULT Initialize(int const cDynamParams,
                         int const cMeasureParams,
                         int const cControlParams);

   LIB_RESULT Predict();
   LIB_RESULT Predict(const ICP_Mat<float>& rcControl);
   LIB_RESULT Correct(const ICP_Mat<float>& rcMeasurement);


public:
   ICP_Mat<float> mPredictedState;
   ICP_Mat<float> mCorrectedState;
   ICP_Mat<float> mStateTransitionMatrix;
   ICP_Mat<float> mExternalControlMatrix;
   ICP_Mat<float> mMeasurementMatrix;
   ICP_Mat<float> mProcessNoiseCovariance;
   ICP_Mat<float> mMeasurementNoiseCovariance;
   ICP_Mat<float> mPrioriErrorCovariance;      // Associated with predicted state
   ICP_Mat<float> mKalmanGain;
   ICP_Mat<float> mPosterioriErrorCovariance;  // Associated with corrected state


private:
   ICP_Mat<float> mTemp1;
   ICP_Mat<float> mTemp2;
   ICP_Mat<float> mTemp3;
   ICP_Mat<float> mTemp4;
   ICP_Mat<float> mTemp5;

};


#endif /* ICLKALMAN_HPP */

