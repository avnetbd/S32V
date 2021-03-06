/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
* Copyright 2017-2018 NXP 
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
 *  \brief ApexCV Image Remap
 ***********************************************************************************/

#ifndef APEXCVPROREMAP_H
#define APEXCVPROREMAP_H

#include <umat.hpp>
#include "icp_data.h"
#include "icp_types.h"
#include <apexcv_host_base_class.h>
#include <apexcv_error_codes.hpp>

namespace apexcv{

/*!*********************************************************************************
 *  \brief Interpolation options.
 ***********************************************************************************/
enum INTER_TYPE
{
   INTER_NN        = 0   /*!< Nearest neighbour interpolation. */,
   INTER_LINEAR    = 1   /*!< Bilinear interpolation. */,
   INTER_CUBIC     = 2   /*!< Bicubic interpolation over 4x4 pixel neighborhood. */,
   INTER_AREA      = 3   /*!< Resampling using pixel area relation. It may be a preferred method for image decimation, as it gives moire�-free results. But when the image is zoomed, it is similar to the INTER_NEAREST method. */,
   INTER_LANCZOS4  = 4   /*!< Lanczos interpolation over 8x8 pixel neighborhood. */
};

/*!*********************************************************************************
 *  \brief Border options.
 ***********************************************************************************/
enum BORDER_TYPE
{
   BORDER_CONSTANT   = 0   /*!< Set the border to a specified value. */,
   BORDER_REPLICATE  = 1   /*!< Replicate the border. */,
   BORDER_REFLECT    = 2   /*!< Reflect the border. */,
   BORDER_WRAP       = 3   /*!< Wrap the border. */
};


/*!*********************************************************************************
 *  \brief Apex Remap.
 *
 *  apexcv::Remap is the host-ACF interface for creating, initializing,
 *  executing and releasing \ref pagRemap "image remap" on Apex.
 ***********************************************************************************/
class Remap: public ApexcvHostBaseBaseClass
{
public:

   /**********************************************************************************
    *  Default constructor.
    *  Allocate resources for the ACF process.
    ***********************************************************************************/
   Remap();

   /**********************************************************************************
    *  Destructor.
    *  Release resources for the ACF process.
    ***********************************************************************************/
   ~Remap();

   /*!*********************************************************************************
    *  \brief Initialize the remap ACF process.
    *
    *  This method is used to initialize the remap processing.  This processing will
    *  calculate the required source block size, and generate the internal offset and
    *  delta tables required for the APU implementation of resize.
    *
    *  This method must be called before process() if using a new float map. Subsequent
    *  calls to process() with the same float map can bypass this function.
    *
    *  Currently only INTERP_LINEAR and BORDER_CONSTANT with a value of 0 are supported.
    *
    *  \return Error code for the execution (zero on success).
    ***********************************************************************************/
    APEXCV_LIB_RESULT Initialize(float* aMap,
                  uint32_t aMapWidth,
                  uint32_t aMapHeight,
                  uint32_t aSrcWidth,
                  apexcv::INTER_TYPE aInterp,
                  apexcv::BORDER_TYPE aBorder,
                  uint32_t aBorderValue = 0);

   /*!*********************************************************************************
    *  \brief Execute the remap ACF process.
    *
    *  This method executes the image remap which was configured by initialize().
    *
    *  \return Error code for the execution (zero on success).
    ***********************************************************************************/
   APEXCV_LIB_RESULT Process(vsdk::UMat &aSrc,
               vsdk::UMat &aDst);

   /*!*********************************************************************************
    *  \brief Generate the look-up table for top view perspective.
    *
    *  \return Error code for the execution (zero on success).
    ***********************************************************************************/
   APEXCV_LIB_RESULT GenerateLUTFromCalibLoader(const char* apFilename,
                                  uint32_t aDstWidth, uint32_t aDstHeight,
                                  uint32_t aSrcWidth, uint32_t aSrcHeight,
                                  uint32_t aDestBlockWidth, uint32_t aDestBlockHeight,
                                  uint32_t aRefSrcBlockWidth, uint32_t aRefSrcBlockHeight);

   /*!*********************************************************************************
    *  \brief Returns the LUTs needed for the remap ACF process.
    *
    *  This method returns the LUTs which was configured by initialize().
    *
    *  \return Error code for the execution (zero on success).
    ***********************************************************************************/
   int RetLUTs(vsdk::UMat   &aDeltaDesc,
               vsdk::UMat   &aLocalOffsetDesc,
               vsdk::UMat   &aBlockOffsetDesc);

   /*!*********************************************************************************
    *  \brief Generate random float map.
    *
    *  This method generates a random float map based off the input parameter's dimensions.
    *  map must be a pointer to an appropriate sized buffer.
    *  max_offset_per_dim specifies the maximum radius of the dst pixel from the source,
    *  ie. a value of 3 means the dst pixel can be chosen from a 3x3 window centered at
    *  the x, y location of the current pixel.
    *
    ***********************************************************************************/
   void GenerateFloatMap(vsdk::UMat& aInput,
                         vsdk::UMat& aOutput,
                         float* apMap,
                         float aMaxOffsetPerDim,
                         int aSeed);

protected:
   // From old stuff
   void*  mpParamLoader;
   void*  mpGroundToDewarp;
   void*  mpGroundToWarp;
   float  mTopViewPixelToMeterRatioX;
   float  mTopViewPixelToMeterRatioY;

private:

   void Release();

   /*!*********************************************************************************
    *  \brief ACF scenario block descriptor struct
    *
    *  This struct describes a potential scenario to be used with ACF.
    *
    ***********************************************************************************/
   struct blockDesc
   {
      uint16_t srcWidth;     /*!< source chunk width */
      uint16_t srcHeight;    /*!< source chunk height */
      uint16_t dstWidth;     /*!< destination chunk width */
      uint16_t dstHeight;    /*!< destination chunk height */
      float ratio;            /*!< destination to source size ratio */
      int32_t memSize;       /*!< CMEM usage in bytes */
   };

   /*!*********************************************************************************
    *  \brief Chosen valid ACF scenario
    *
    *  This contains a valid configuration to be used to set the ACF scenario.
    *
    ***********************************************************************************/
   blockDesc validConfig;

   // Containers for offeset/delta
   vsdk::UMat moOffset;     /*!< Motion comp offset buffer. */
   vsdk::UMat mLocalOffset; /*!< CU local offset buffer. */
   vsdk::UMat mDelta;       /*!< Interpolation delta buffer. */

   // Pointers for the above containers
   void* mpLocalDstOal;

   void   *mpRemapBlockProc;
   void   *mpRemapProc;

   int32_t mInitState;

   int32_t FloatToFixedOffsets(const float *apMapRGB, vsdk::UMat &aFixedOffset, const int32_t acDstStride, const int32_t acSrcStride);
   int32_t InitMoOffsets(vsdk::UMat &aFixedOffset, const int32_t acSrcStride);
   int32_t FixedToLocalOffsets(vsdk::UMat &aFixedOffset, const int32_t acSrcStride);
   void PopulateBlockDesc(vsdk::UMat aData, blockDesc *apdesc);
   int32_t AdjustImageWidth(int32_t aWidth);
   int32_t AdjustNumberOfCU(int32_t aWidth);
};
}  /* namespace apexcv */

#endif   /* APEXCVPROREMAP_H */
