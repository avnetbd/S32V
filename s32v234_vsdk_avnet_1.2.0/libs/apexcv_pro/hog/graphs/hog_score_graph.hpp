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
#ifndef HOGSCOREGRAPH_HPP
#define HOGSCOREGRAPH_HPP



#include <acf_graph.hpp>
#include "hog_acf.h"

class hog_score_graph : public ACF_Graph
{
public:
   hog_score_graph() : ACF_Graph()
   {
      GET_KERNEL_HANDLE(&_kernel_info_hog_get_gradient, "HOG_getGradient");
      GET_KERNEL_HANDLE(&_kernel_info_hog_get_cell_histograms, "HOG_getCellHistograms");
      GET_KERNEL_HANDLE(&_kernel_info_hog_compute_block_histograms, "HOG_computeBlockHistograms");
      GET_KERNEL_HANDLE(&_kernel_info_hog_get_scores, "HOG_getScores");
   }

   void Create()
   {
      //set identifier for graph
      SetIdentifier("hog_score_graph");
      
      //add kernels
      AddKernel("getGradient", "HOG_getGradient");
      AddKernel("getCellHistograms", "HOG_getCellHistograms");
      AddKernel("computeBlockHistograms", "HOG_computeBlockHistograms");
      AddKernel("getScores", "HOG_getScores");

      //add graph ports
      AddInputPort("IMAGE");
      AddInputPort("SVM_FUNCTION");
      AddOutputPort("SCORES");

      // connect graph ports
      Connect(GraphPort("IMAGE"),        KernelPort("getGradient", "IMAGE"));
      Connect(GraphPort("SVM_FUNCTION"), KernelPort("getScores",   "SVM_FUNCTION"));

      // connect kernel ports
      Connect(KernelPort("getGradient", "GRADIENT_MAGNITUDE"), KernelPort("getCellHistograms", "GRADIENT_MAGNITUDE"));
      Connect(KernelPort("getGradient", "GRADIENT_ANGLE_BIN"), KernelPort("getCellHistograms", "GRADIENT_ANGLE_BIN"));  
      
      Connect(KernelPort("getCellHistograms", "CELL_HISTOGRAMS"), KernelPort("computeBlockHistograms", "CELL_HISTOGRAMS"));
            
      Connect(KernelPort("computeBlockHistograms", "BLOCK_HISTOGRAMS"), KernelPort("getScores", "BLOCK_HISTOGRAMS"));
      
      Connect(KernelPort("getScores", "SCORES"), GraphPort("SCORES"));
   }
};

#endif /* HOGSCOREGRAPH_HPP */

