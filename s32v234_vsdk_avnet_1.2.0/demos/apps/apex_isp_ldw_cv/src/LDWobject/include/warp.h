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
#ifndef WARP_H
#define WARP_H

#include <opencv2/opencv.hpp>
#include "transform_lut.h"

#include <apex.h>
#include <umat.hpp>
#include "LDWS_REMAP_SOBEL_MEDIAN.hpp"

/**
 * Class consists of function which warps an image into bird eye view
 */
class Warp
{
  public:
    /**
     * Constructor
     */
    Warp();
    
    /**
     * Constructor apex init
     */
    Warp(int apuldx);
    
    /**
     * Function warps the input image into the output using pre-computed 
     * transformation.
     * Uses APEX subsystem.
     * @param in Input image
     * @param out Output bird eye image
     * @param transform Transformation LUTs to be used 
     */
    void Warp_BirdEye(vsdk::UMat in, vsdk::UMat out, Transform_LUT &transform);
    
    /*
     * Warp reinint
     */
    void set_warp_init(bool value);
    
  private:
    
     /**
     * APU index
     */
    int m_apuldx;   
    
//     /**
//      * Source image
//      */
//     vsdk::UMat src;
//     
//     /**
//      * Destination image
//      */
//     vsdk::UMat dst;
    
    /**
     * Blocks indices
     */
    vsdk::UMat blocks;
    
    /**
     * Offsets indices
     */
    vsdk::UMat offsets;
    
    /**
     * Deltas values
     */
    vsdk::UMat deltas;
    
    /**
     * APEX remap process
     */
    LDWS_REMAP_SOBEL_MEDIAN apex_remap;
    
    /**
     * Warp initialised
     */
    bool warp_init;
};

#endif /* WARP_H */

