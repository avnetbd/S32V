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
*  \file
*  \brief APU Kernel and ACF configuration definitions for HOG.
***********************************************************************************/
#ifndef HOGCONFIG_H
#define HOGCONFIG_H

#define N_BINS 8

struct Size
{
  int width;
  int height;
};

struct HogParam
{
  Size cell;
  Size blockCells;
  Size block;
  Size blockStride;
  Size winBlocks;
  Size winPixels;
  Size winStride;
  Size winStridePerBlock;
  Size svm;
};

static HogParam DefaultParams = {{   8,   8},   /* cell size in pixels */
                                 {   1,   1},   /* cells per block */
                                 {   8,   8},   /* block size in pixels */
                                 {   8,   8},   /* blockStride in pixels*/
                                 {   8,  16},   /* blocks per window */
                                 {  64, 128},   /* window size in pixels */
                                 {   4,   4},   /* window stride in pixels */ 
                                 {   2,   2},   /* window strides per block */
                                 {1024,   1}};  /* svm */

#endif   /* HOGCONFIG_H */

