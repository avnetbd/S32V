/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
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

#include "apu_segmentation.h"


#ifdef ACF_KERNEL_METADATA

/*! 
*   k-means kernel metadata.
*   
*   Calculate local k-means algorithm.
*   \brief local k-means algorithm.   
*   \param INPUT_0   grayscale picture
*   \param INPUT_1   luminance seeds
*   \param INPUT_2   index
*   \param OUTPUT_0  k-means labels
*/

KERNEL_INFO kernelInfoConcat(k_means_kernel)
(
   "k_means_kernel",
   4,
   __port(__index(0),
          __identifier("INPUT_0"),
          __attributes(ACF_ATTR_VEC_IN_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(20, 20)),
   __port(__index(1),
          __identifier("INPUT_1"),
          __attributes(ACF_ATTR_VEC_IN_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(3, 3)),
    __port(__index(2),
          __identifier("INPUT_2"),
          __attributes(ACF_ATTR_VEC_IN_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(3, 3)),
     __port(__index(3),
          __identifier("OUTPUT_0"),
          __attributes(ACF_ATTR_VEC_OUT_FIXED),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(20, 20))
);

/*! 
*   k-means kernel metadata.
*   
*   Calculate local k-means algorithm.
*   \brief local k-means algorithm.   
*   \param INPUT_0   k-means labels
*   \param INPUT_1   picture
*   \param OUTPUT_0  output picture
*/

KERNEL_INFO kernelInfoConcat(draw_contours_around_segments_kernel)
(
   "draw_contours_around_segments_kernel",
   3,
   __port(__index(0),
   __identifier("INPUT_0"),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(1,1,1,1),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
   __identifier("INPUT_1"),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(1,1,1,1),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(2),
   __identifier("OUTPUT_0"),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(1, 1),
          __ek_size(1, 1))
);

/*! 
*   k-means kernel metadata.
*   
*   Calculate local k-means algorithm.
*   \brief local k-means algorithm.   
*   \param INPUT_0   k-means labels
*   \param INPUT_1   picture
*   \param OUTPUT_0  output picture
*/

KERNEL_INFO kernelInfoConcat(draw_contours_around_segments_RGB_kernel)
(
   "draw_contours_around_segments_RGB_kernel",
   3,
   __port(__index(0),
   __identifier("INPUT_0"),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(1,1,1,1),
          __e0_data_type(d16u),
          __e0_size(1, 1),
          __ek_size(1, 1)),
   __port(__index(1),
   __identifier("INPUT_1"),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(3, 1),
          __ek_size(1, 1)),
   __port(__index(2),
   __identifier("OUTPUT_0"),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d08u),
          __e0_size(3, 1),
          __ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "apu_segmentation_impl.h"

void k_means_kernel(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc in2, kernel_io_desc out0)
{
	vec08u*        lIn0 = (vec08u*)in0.pMem;
	vec16u*        lIn1 = (vec16u*)in1.pMem;
        vec16u*        lIn2 = (vec16u*)in2.pMem;
	vec16u*        lOut0 = (vec16u*)out0.pMem;

	k_means_kernel_impl(lOut0, lIn0, lIn1, lIn2, in0.chunkWidth, in0.chunkHeight, in1.chunkWidth, in1.chunkHeight);
}

void draw_contours_around_segments_kernel(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0)
{
        vec16u*        lIn0 = (vec16u*)in0.pMem;
        vec08u*        lIn1 = (vec08u*)in1.pMem;       
        vec08u*        lOut0 = (vec08u*)out0.pMem;

        draw_contours_around_segments_kernel_impl(lIn0, lIn1, lOut0, in0.chunkWidth, in0.chunkHeight, in0.chunkSpan / 2, in1.chunkSpan, out0.chunkSpan);
} 

void draw_contours_around_segments_RGB_kernel(kernel_io_desc in0, kernel_io_desc in1, kernel_io_desc out0)
{
        vec16u*        lIn0 = (vec16u*)in0.pMem;
        vec08u*        lIn1 = (vec08u*)in1.pMem;       
        vec08u*        lOut0 = (vec08u*)out0.pMem;

        draw_contours_around_segments_RGB_kernel_impl(lIn0, lIn1, lOut0, in0.chunkWidth, in0.chunkHeight, in0.chunkSpan / 2, in1.chunkSpan, out0.chunkSpan);
} 

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
