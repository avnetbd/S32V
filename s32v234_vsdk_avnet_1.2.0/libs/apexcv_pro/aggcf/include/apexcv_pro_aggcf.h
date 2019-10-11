/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor
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
*  \file
*  \brief Apex aggregated channel feature mag/ori calculation.
***********************************************************************************/

#ifndef APEXCVAGGCF_H
#define APEXCVAGGCF_H

#include <stdint.h>
#include "aggcf_ref_calc.h"

// ACF HOST LIB
#include <icp_data.h>
#include <icp_feature.h>
#include <apexcv_error_codes.hpp>
#include <apexcv_host_base_class.h>

// GENERATED ACF PROCESS
#include <AGGCF.hpp>
#include <AGGCF_HIST_NT1.hpp>
#include <AGGCF_HIST_NT2.hpp>
#include <AGGCF_HIST_NT3.hpp>
#include <AGGCF_HIST_NT4.hpp>
#include <AGGCF_HIST_TI2.hpp>
#include <AGGCF_HIST_TI3.hpp>
#include <AGGCF_HIST_TI4.hpp>
#include <AGGCF_FLT.hpp>
#include <AGGCF_FLT_NORM.hpp>
#include <AGGCF_LUV.hpp>
#include <AGGCF_TRIFLT.hpp>
#include <AGGCF_SCALE_DOWN2.hpp>
#include <AGGCF_SCALE_DOWN4.hpp>
#include <AGGCF_SCALE_DOWN8.hpp>
namespace apexcv{
    
class AGGCFScaleRef
{
public:
    AGGCFScaleRef();

    ~AGGCFScaleRef();

    int Process(vsdk::UMat& in, vsdk::UMat& out, int scale );
};

class AGGCFScaleDown2: public ApexcvHostBaseBaseClass
{
public:
    AGGCFScaleDown2();

    ~AGGCFScaleDown2();

    APEXCV_LIB_RESULT Initialize(vsdk::UMat &src, vsdk::UMat &dst);

    APEXCV_LIB_RESULT ReconnectIO(vsdk::UMat &src, vsdk::UMat &dst);

    APEXCV_LIB_RESULT Process();

    APEXCV_LIB_RESULT SetApuConfiguration(ACF_APU_CFG lApuConfig, int32_t lApexId);

private:
 
    int         mInitState;
    ACF_APU_CFG mApuConfig;
    int32_t     mApexId;
    bool        mSelectedApuConfiguration;
    
    AGGCF_SCALE_DOWN2 mProcessAggScaleDown2;
    APEXCV_LIB_RESULT ScaleDown2();
};

class AGGCFScaleDown4: public ApexcvHostBaseBaseClass
{
public:
    AGGCFScaleDown4();

    ~AGGCFScaleDown4();

    APEXCV_LIB_RESULT Initialize(vsdk::UMat &src, vsdk::UMat &dst);

    APEXCV_LIB_RESULT ReconnectIO(vsdk::UMat &src, vsdk::UMat &dst);

    APEXCV_LIB_RESULT Process();

    APEXCV_LIB_RESULT SetApuConfiguration(ACF_APU_CFG lApuConfig, int32_t lApexId);

private:
 
    int         mInitState;
    ACF_APU_CFG mApuConfig;
    int32_t     mApexId;
    bool        mSelectedApuConfiguration;
    
    AGGCF_SCALE_DOWN4 mProcessAggScaleDown4;
    APEXCV_LIB_RESULT ScaleDown4();
};

class AGGCFScaleDown8: public ApexcvHostBaseBaseClass
{
public:
    AGGCFScaleDown8();

    ~AGGCFScaleDown8();

    APEXCV_LIB_RESULT Initialize(vsdk::UMat &src, vsdk::UMat &dst);

    APEXCV_LIB_RESULT ReconnectIO(vsdk::UMat &src, vsdk::UMat &dst);

    APEXCV_LIB_RESULT Process();

    APEXCV_LIB_RESULT SetApuConfiguration(ACF_APU_CFG lApuConfig, int32_t lApexId);

private:
 
    int         mInitState;
    ACF_APU_CFG mApuConfig;
    int32_t     mApexId;
    bool        mSelectedApuConfiguration;
    
    AGGCF_SCALE_DOWN8 mProcessAggScaleDown8;
    APEXCV_LIB_RESULT ScaleDown8();
};




class AGGCFResizeFlt: public ApexcvHostBaseBaseClass
{
public:
    AGGCFResizeFlt();

    ~AGGCFResizeFlt();

    APEXCV_LIB_RESULT Initialize(vsdk::UMat &lSrcImage, vsdk::UMat &lDestImage, int p);

    APEXCV_LIB_RESULT ReconnectIO(vsdk::UMat &lSrcImage, vsdk::UMat &lDestImage, int p);

    APEXCV_LIB_RESULT Process(vsdk::UMat &src, vsdk::UMat &dst);

    APEXCV_LIB_RESULT SelectApuConfiguration(ACF_APU_CFG lApuConfig, int32_t lApexId);

private:
 
    int         mInitState;
    ACF_APU_CFG mApuConfig;
    int32_t     mApexId;
    bool        mWeSelectedApuConfiguration;
    vsdk::UMat  mParams;
    vsdk::UMat mSrcImage_umat;
    vsdk::UMat mDstImage_umat;

    APEXCV_LIB_RESULT copy16(vsdk::UMat& src, vsdk::UMat& dst);
    APEXCV_LIB_RESULT ACFHrszFlt16(vsdk::UMat& src, vsdk::UMat& dst);
    APEXCV_LIB_RESULT ACFVrszFlt16(vsdk::UMat& src, vsdk::UMat& dst);
    APEXCV_LIB_RESULT AcfResizeFlt16(vsdk::UMat& src, vsdk::UMat& dst);

    APEXCV_LIB_RESULT InitParams(int p);
};

class AGGCFluvRef
{
public:
   AGGCFluvRef();

   ~AGGCFluvRef();

   int Process(
          vsdk::UMat &in,
          vsdk::UMat &outl,
          vsdk::UMat &outu,
          vsdk::UMat &outv );

};


class AGGCFluv: public ApexcvHostBaseBaseClass
{
public:

/*!*********************************************************************************
*  \brief Default constructor.
*  Allocate resources for the ACF processes.
***********************************************************************************/
   AGGCFluv();

/*!*********************************************************************************
*  \brief Destructor.
*   Release resources for the ACF processes.
***********************************************************************************/
   ~AGGCFluv();

   APEXCV_LIB_RESULT Initialize( vsdk::UMat &in,
                   vsdk::UMat &outl,
                   vsdk::UMat &outu,
                   vsdk::UMat &outv
               );

   APEXCV_LIB_RESULT ReconnectIO( vsdk::UMat &in,
                    vsdk::UMat &outl,
                    vsdk::UMat &outu,
                    vsdk::UMat &outv
               );

   APEXCV_LIB_RESULT Process();
   APEXCV_LIB_RESULT SetApuConfiguration(ACF_APU_CFG lApuConfig, int32_t lApexId);

private:

   void Release();
   APEXCV_LIB_RESULT Computeluv();

   int     mInitState;                     /*!< Initialization state for all ACF processes. */
   int32_t mImageWidth;                    /*!< The image width used in the process */
   int32_t mImageHeight;                   /*!< The image height used in the process */

   AGGCF_LUV mProcessAggLuv;
   
   ACF_APU_CFG mApuConfig;
   int32_t     mApexId;
   int32_t     mSelectedApuConfiguration; 
};



class AGGCFflt3x3: public ApexcvHostBaseBaseClass
{
public:

/*!*********************************************************************************
*  \brief Default constructor.
*  Allocate resources for the ACF processes.
***********************************************************************************/
   AGGCFflt3x3();

/*!*********************************************************************************
*  \brief Destructor.
*   Release resources for the ACF processes.
***********************************************************************************/
   ~AGGCFflt3x3();

   APEXCV_LIB_RESULT Initialize( vsdk::UMat &in,
                   vsdk::UMat &out,
                   uint32_t p, uint32_t normconst);

   APEXCV_LIB_RESULT ReconnectIO( vsdk::UMat &in,
                    vsdk::UMat &out,
                    uint32_t p, uint32_t normConst);

   APEXCV_LIB_RESULT SetParameters(uint32_t p, uint32_t normConst);
   APEXCV_LIB_RESULT Process();
   APEXCV_LIB_RESULT SetApuConfiguration(ACF_APU_CFG lApuConfig, int32_t lApexId);


private:

   APEXCV_LIB_RESULT InitParams(uint32_t p, uint32_t normconst);
   void Release();   
   APEXCV_LIB_RESULT ComputeFlt();

   int     mInitState;                     /*!< Initialization state for all ACF processes. */
   int32_t mImageWidth;                    /*!< The image width used in the process */
   int32_t mImageHeight;                   /*!< The image height used in the process */

   AGGCF_FLT mProcessAggFlt;
   AGGCF_FLT_NORM mProcessAggFltNorm;
   
   vsdk::UMat mParams;
   
   uint32_t    mFilterP;
   uint32_t    mNormConst; 
   ACF_APU_CFG mApuConfig;
   int32_t     mApexId;
   int32_t     mSelectedApuConfiguration; 
};

class AGGCFfltRef
{
public:
   AGGCFfltRef();

   ~AGGCFfltRef();

   int Process(
          vsdk::UMat &in,
          vsdk::UMat &out,
          uint32_t p);

};


class AGGCFtriflt: public ApexcvHostBaseBaseClass
{
public:

/*!*********************************************************************************
*  \brief Default constructor.
*  Allocate resources for the ACF processes.
***********************************************************************************/
   AGGCFtriflt();

/*!*********************************************************************************
*  \brief Destructor.
*   Release resources for the ACF processes.
***********************************************************************************/
   ~AGGCFtriflt();

   APEXCV_LIB_RESULT Initialize( vsdk::UMat &in,
                   vsdk::UMat &out,
                   int r);

   APEXCV_LIB_RESULT ReconnectIO( vsdk::UMat &in,
                    vsdk::UMat &out,
                    int r );

   APEXCV_LIB_RESULT SetParameters(int r);
   APEXCV_LIB_RESULT Process();
   APEXCV_LIB_RESULT SetApuConfiguration(ACF_APU_CFG lApuConfig, int32_t lApexId);

private:

   APEXCV_LIB_RESULT InitParams(int r);
   void Release();   
   APEXCV_LIB_RESULT ComputeFlt();

   int     mInitState;                     /*!< Initialization state for all ACF processes. */
   int32_t mImageWidth;                    /*!< The image width used in the process */
   int32_t mImageHeight;                   /*!< The image height used in the process */

   AGGCF_TRIFLT mProcessAggFlt;

   vsdk::UMat  mParams;
   
   int32_t     mFilterRadius;
   ACF_APU_CFG mApuConfig;
   int32_t     mApexId;
   int32_t     mSelectedApuConfiguration; 
};


class AGGCFMagOriRef
{
public:
   AGGCFMagOriRef();
  
   ~AGGCFMagOriRef();

   int Process( vsdk::UMat &srcl,
                vsdk::UMat &srcu,
                vsdk::UMat &srcv,
                vsdk::UMat &mag,
                vsdk::UMat &ori,
                int full );   
 
   
};


class AGGCFMagOri: public ApexcvHostBaseBaseClass
{
public:

/*!*********************************************************************************
*  \brief Default constructor.
*  Allocate resources for the ACF processes.
***********************************************************************************/
   AGGCFMagOri();

/*!*********************************************************************************
*  \brief Destructor.
*   Release resources for the ACF processes.
***********************************************************************************/
   ~AGGCFMagOri();

   APEXCV_LIB_RESULT Initialize(vsdk::UMat &srcl,
                  vsdk::UMat &srcu,
                  vsdk::UMat &srcv,
                  vsdk::UMat &mag,
                  vsdk::UMat &ori,
                  int full  );

   APEXCV_LIB_RESULT ReconnectIO( vsdk::UMat &srcl,
                    vsdk::UMat &srcu,
                    vsdk::UMat &srcv,
                    vsdk::UMat &mag,
                    vsdk::UMat &ori,
                    int full );

   APEXCV_LIB_RESULT SetParameters(int full);
   APEXCV_LIB_RESULT Process();
   APEXCV_LIB_RESULT SetApuConfiguration(ACF_APU_CFG lApuConfig, int32_t lApexId);

private:

   APEXCV_LIB_RESULT InitParams(int full);
   void Release();   
   APEXCV_LIB_RESULT ComputeMagOri();

   int     mInitState;                     /*!< Initialization state for all ACF processes. */
   int32_t mImageWidth;                    /*!< The image width used in the process */
   int32_t mImageHeight;                   /*!< The image height used in the process */

   AGGCF mProcessAggCF;
   vsdk::UMat mParams;
   
   int32_t     mFullrange;
   ACF_APU_CFG mApuConfig;
   int32_t     mApexId;
   int32_t     mSelectedApuConfiguration; 
};


class AGGCFhist: public ApexcvHostBaseBaseClass
{
public:

   AGGCFhist();
   ~AGGCFhist();

   APEXCV_LIB_RESULT Initialize(vsdk::UMat &srcM,
                  vsdk::UMat &srcO,
	          vsdk::UMat &Hist1,
	          vsdk::UMat &Hist2,
	          vsdk::UMat &Hist3,
	          vsdk::UMat &Hist4,
	          vsdk::UMat &Hist5,
	          vsdk::UMat &Hist6,
	          int full, int numOrientations, int bin, int softbin );

   APEXCV_LIB_RESULT SetParameters( int full, int numOrientations, int bin, int softbin);
   APEXCV_LIB_RESULT Process();
   APEXCV_LIB_RESULT SetApuConfiguration(ACF_APU_CFG lApuConfig, int32_t lApexId);

   APEXCV_LIB_RESULT ReconnectIO(
          vsdk::UMat &srcM,
          vsdk::UMat &srcO,
          vsdk::UMat &dstH1,
          vsdk::UMat &dstH2,
          vsdk::UMat &dstH3,
          vsdk::UMat &dstH4,
          vsdk::UMat &dstH5,
          vsdk::UMat &dstH6,
          int full, int numOrientations, int bin, int softbin);

private:

   APEXCV_LIB_RESULT InitBuffer(int bin);
   APEXCV_LIB_RESULT InitParams(int full, int numOrientations, int bin, int softbin);
   void Release();
   APEXCV_LIB_RESULT ComputeHist();

   int mFullrange;
   int mNumOrientations;
   int mBin;
   int mSoftbin;

   int     mInitState;                     /*!< Initialization state for all ACF processes. */
   int32_t mImageWidth;                    /*!< The image width used in the process */
   int32_t mImageHeight;                   /*!< The image height used in the process */

   vsdk::UMat offsetRow;
   vsdk::UMat mParams;
 
   AGGCF_HIST_NT1 mProcessHistnt1;
   AGGCF_HIST_NT2 mProcessHistnt2;
   AGGCF_HIST_NT3 mProcessHistnt3;
   AGGCF_HIST_NT4 mProcessHistnt4;
   
   AGGCF_HIST_TI2 mProcessHistti2;
   AGGCF_HIST_TI3 mProcessHistti3;
   AGGCF_HIST_TI4 mProcessHistti4;


   ACF_APU_CFG mApuConfig;
   int32_t     mApexId;
   int32_t     mSelectedApuConfiguration;  
};


class AGGCFhistRef
{
public:
   AGGCFhistRef();

   ~AGGCFhistRef();

   int Process( 
          vsdk::UMat &srcM,
          vsdk::UMat &srcO,
          vsdk::UMat &dstH1,
          vsdk::UMat &dstH2,
          vsdk::UMat &dstH3,
          vsdk::UMat &dstH4,
          vsdk::UMat &dstH5,
          vsdk::UMat &dstH6,
          int full, int numOrientations, int bin, int softbin);

};




}
#endif /* APEXCVAGGCF_H */
