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

/*!*****************************************************************************
*  \file
*  \brief Apex HOG Object Detector based on:
*  N. Dalal and B. Triggs. Histograms of Oriented Gradients for Human Detection. 
*  INRIA, 2005.  https://lear.inrialpes.fr/people/triggs/pubs/Dalal-cvpr05.pdf 
*
*  The HOG parameters are as follows:
*  histogram bins = 8
*  cell size = block size = 8x8 pixels
*  block stride = (8,8) pixels
*  detection window size = 8x16 blocks = 64x128 pixels
*  detection window stride = (4,4) pixels
*  The resulting descriptor size is:
*  Nd = (detection window blocks)*(histogram bins) = (8*16)*8 = 1024
*
*******************************************************************************/

#ifndef APEXCVPROHOG_H
#define APEXCVPROHOG_H

#include <apexcv_error_codes.hpp>
#include <acf_process_apu.h>
#include <umat.hpp>
#include <apexcv_host_base_class.h>

namespace apexcv
{
/*!*****************************************************************************
*  \brief Apex HOG class.
*
*  apexcv::Hog is the host-ACF interface for creating, initializing,
*  executing and releasing the \ref pagHOG on Apex.
*******************************************************************************/
  class Hog: public ApexcvHostBaseBaseClass
  {
public:

  enum class HogType
  {
    CELLS /*< 8-bin normalized block histogram computed every 4x4 pixels */,
    DETECT /*< SVM detection result for each detection window */
  };

  /****************************************************************************
  *  Default constructor.
  *****************************************************************************/
  Hog();

  /****************************************************************************
  *  Destructor, undocumented
  *****************************************************************************/
  ~Hog();

  /*!***************************************************************************
  *  \brief Initialization for the APU process
  *  \return Error code for ACF process initialization, APEXCV_SUCCESS on
  *  success.
  *  
  *  Allocates and initializes ACF process, connects input and output buffers
  *  and allocates output buffer.
  *****************************************************************************/
   APEXCV_LIB_RESULT Initialize(const vsdk::UMat &aSrc    /*!< 8-bit grayscale source image */,
                                      vsdk::UMat &aDst    /*!< Destination image, format depends on operation, buffer allocated by function */,
                                         HogType  aOutputType=HogType::DETECT /*!< The type of processing to be performed */);

   /*!****************************************************************************
    *  \brief Reconnect IO
    *
    *  Use this to reconnect the input and output buffers and allocate the output. 
    *  This only needs to be done if the connected Input/Outputs are changed. 
    *  If only the data within (no size, data pointer, or type changes), then this 
    *  does not need to be called.
    ******************************************************************************/
   APEXCV_LIB_RESULT ReconnectIO(const vsdk::UMat &aSrc /*!< 8-bit grayscale source image */,
                                       vsdk::UMat &aDst /*!< Destination image, format depends on operation, buffer allocated by function */);

   /*!***************************************************************************
    *  \brief Convert and initialize the SVM port.
    *  \return Error code or APEXCV_SUCCESS on success.
    *
    *  Specify the decision function used by the linear SVM classifier.  The
    *  decision function comes from training the detector on a class of objects
    *  (e.g. pedestrians), by specifying the decision function you specify what
    *  class of objects you wish to detect.  The size of the decision function
    *  is (descriptorSize + 1) for (SVM weights + offset).  This function is only
    *  valid for HogType::DETECT.  Must be called after Initialize().
    *****************************************************************************/
   APEXCV_LIB_RESULT SetSVM(const vsdk::UMat &aSVM /*!< Vector of descriptorSize + 1 elements of type VSDK_CV_64FC1 (double) */);

   /*!***************************************************************************
    *  \brief Select APEX Core for processing
    *  \return Error code for the ACF configuration (APEXCV_SUCCESS on success).
    *
    *  Select which APEX core (0 or 1) to be selected to run the processing.
    *  This function has to be called after Initialize() and can be executed per
    *  frame base.
    *****************************************************************************/
   APEXCV_LIB_RESULT SelectApexCore( int32_t aApexId /*!< The ID of the desired APEX (e.g if there are 2 APEXs,
                                                        valid values for aApexId would be 0 and 1).*/);

   /*!***************************************************************************
    *  \brief Run the HOG process
    *  \return Error code for the ACF execution (APEXCV_SUCCESS on success).
    *
    *  For all processing types the detector takes an 8-bit grayscale image and 
    *  computes the 8-bin normalized block histogram every 4x4 pixels.  The size and
    *  contents of the output image depend on the HogType passed to the Initialize() 
    *  method.
    *  
    *  For HogType::CELLS each pixel in the output is the 8  histogram bins packed as 
    *  an 8-bit unsigned 8-channel image. 
    *  If the input image size is not an integer multiple of the block size, partial 
    *  blocks at the right and bottom edges are not computed.  For a (WinxHin) 
    *  sized image and wxh sized block window the output size is: 
    *  (Wout, Hout) =  (floor((Win-w)/4)*8, floor((Hin-h)/4))
    *
    *  For HogType::DETECT for each pixel on a 4x4 lattice  of the image, the HOG 
    *  descriptor is computed and the trained linear SVM  classifier is applied to each 
    *  descriptor to produce a 16-bit \em SVM \em score. 
    *  If the input image  size is not an integer mutiple of the window size and stride, 
    *  partial blocks at the right and bottom edges are not computed.
    *  For a (WinxHin) sized image and wxh sized block window the output size is: 
    *  (Wout, Hout) = (floor((Win-w)/4), floor((Hin-h)/4))
    *****************************************************************************/
   APEXCV_LIB_RESULT Process();

   /*!***************************************************************************
    *  \brief Get fixed point SVM decision function
    *  \return Error code for memory allocation (APEXCV_SUCCESS on success).
    *
    *  This function has to be called after SetSVM().
    *****************************************************************************/
   APEXCV_LIB_RESULT GetSVM(vsdk::UMat &aSVM /*!< Vector to be populated with descriptorSize linear SVM weights in Q0.7 format */ ,
                               int16_t &aOffset /*!< Linear SVM offset in Q4.11 */);

   /*!***************************************************************************
    *  \brief Get the size of HOG descriptor for a single detection window
    *****************************************************************************/
   static size_t GetDescriptorSize();

   /*!***************************************************************************
    *  \brief Concatenate blocks to form HOG descriptor
    *  \return Error code for the ACF execution (APEXCV_SUCCESS on success).
    *
    *  The HOG descriptor is a column-wise concatenation of the block histograms 
    *  in a detection window.  If the input image size is not an integer mutiple 
    *  of the window size and stride, partial blocks at the right and bottom edges 
    *  are not computed.  The length-Nd descriptors are packed as an 8-bit unsigned 
    *  Nd-channel image.
    *  For a (WinxHin) sized image and wxh sized block window the output size
    *  is: (Wout, Hout) = (floor((Win-w)/4)*Nd, floor((Hin-h)/4)) 
    *  Note that the memory requirement of the output descriptor image is large:
    *  Nd/16 = 64 times the input image.
    *****************************************************************************/
   static APEXCV_LIB_RESULT GetDescriptors(const vsdk::UMat &aInput /*!< HOG cells image, output of Process() called with HogType::CELLS . */,
                                                 vsdk::UMat &aOutput /*!< Output, populated on successful return. */);

 private:
   /*!***************************************************************************
    *  \brief Resets state and frees memory
    *  \return APEXCV_SUCCESS on success
    *****************************************************************************/
   APEXCV_LIB_RESULT Release();

   vsdk::UMat mSVM; /*!< SVM decision function memory buffer */

   void *mpScoreProcess;      /*!< HOG detection ACF Graph */

   void *mpDescriptorProcess; /*!< HOG descriptor ACF Graph */

   int32_t mApexId; /*!< Index of the APEX core selected to run the algorithm */

   bool mInit; /*!< Initialization state. */

   HogType mType; /*!< Type of computation. */
  };
}  /* namespace apex */

#endif /* APEXCVPROHOG_H */
