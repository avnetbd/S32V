/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2016 Freescale Semiconductor
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
#ifndef APUSEGMENTATIONIMPL_H
#define APUSEGMENTATIONIMPL_H

/*!
   Calculate local k-means algorithm.
   \brief local k-means algorithm.   
   \param klabels  - [Output] output labels 
   \param m_Yvec  -  [Input]  grayscale picture
   \param kseedsY -  [Input]  grayscale seeds
   \param index -    [Input]  index of seeds array
   \param bw   -     [Input]  block width
   \param bh   -     [Input]  block height
   \param pw   -     [Input]  unused
   \param ph   -     [Input]  unused
 */

void k_means_kernel_impl(vec16u* klabels, vec08u* m_Yvec, vec16u* kseedsY, vec16u* index, int bw, int bh, int pw, int ph);

/*!
   Draw contours around segments.
   \brief Draw contours around segments.   
   \param klabels  -       [Input]  output labels 
   \param inimg  -         [Output] input picture
   \param outimg -         [Input]  output picture   
   \param bw   -           [Input]  block width
   \param bh   -           [Input]  block height
   \param klabelstride   - [Input]  klabelstride
   \param instride   -     [Input]  instride
   \param outstride   -    [Input]  outstride
 */

void draw_contours_around_segments_kernel_impl(vec16u* klabels, vec08u* inimg, vec08u* outimg, int bw, int bh, int klabelstride, int instride,  int outstride);

/*!
   Draw contours around segments.
   \brief Draw contours around segments.   
   \param klabels  -       [Input]  output labels 
   \param inimg  -         [Output] input picture
   \param outimg -         [Input]  output picture   
   \param bw   -           [Input]  block width
   \param bh   -           [Input]  block height
   \param klabelstride   - [Input]  klabelstride
   \param instride   -     [Input]  instride
   \param outstride   -    [Input]  outstride
 */

void draw_contours_around_segments_RGB_kernel_impl(vec16u* klabels, vec08u* inimg, vec08u* outimg, int bw, int bh, int klabelstride, int instride,  int outstride);

#endif /* APUSEGMENTATIONIMPL_H */

/*! @} */
