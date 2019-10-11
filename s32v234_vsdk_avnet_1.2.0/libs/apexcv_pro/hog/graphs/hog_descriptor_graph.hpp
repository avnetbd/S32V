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
*  \brief ACF graph definition for the HOG implementation.
***********************************************************************************/
#ifndef HOGDESCRIPTORGRAPH_HPP
#define HOGDESCRIPTORGRAPH_HPP



#include <acf_graph.hpp>
#include "hog_acf.h"

class hog_descriptor_graph : public ACF_Graph
{
public:
   hog_descriptor_graph() : ACF_Graph()
   {
      GET_KERNEL_HANDLE(&_kernel_info_hog_get_gradient, "HOG_getGradient");
      GET_KERNEL_HANDLE(&_kernel_info_hog_get_cell_histograms, "HOG_getCellHistograms");
      GET_KERNEL_HANDLE(&_kernel_info_hog_get_block_histograms, "HOG_getBlockHistograms");
   }

   void Create()
   {
      //set identifier for graph
      SetIdentifier("hog_descriptor_graph");
      
      //add kernels
      AddKernel("getGradient", "HOG_getGradient");
      AddKernel("getCellHistograms", "HOG_getCellHistograms");
      AddKernel("getBlockHistograms", "HOG_getBlockHistograms");

      //add graph ports
      AddInputPort("IMAGE");
      AddOutputPort("BLOCK_HISTOGRAMS");

      // connect graph ports
      Connect(GraphPort("IMAGE"), KernelPort("getGradient", "IMAGE"));

      // connect kernel ports
      Connect(KernelPort("getGradient", "GRADIENT_MAGNITUDE"), KernelPort("getCellHistograms", "GRADIENT_MAGNITUDE"));
      Connect(KernelPort("getGradient", "GRADIENT_ANGLE_BIN"), KernelPort("getCellHistograms", "GRADIENT_ANGLE_BIN"));  
      
      Connect(KernelPort("getCellHistograms", "CELL_HISTOGRAMS"), KernelPort("getBlockHistograms", "CELL_HISTOGRAMS"));
            
      Connect(KernelPort("getBlockHistograms", "BLOCK_HISTOGRAMS"), GraphPort("BLOCK_HISTOGRAMS"));
   }
};

#endif /* HOGDESCRIPTORGRAPH_HPP */

