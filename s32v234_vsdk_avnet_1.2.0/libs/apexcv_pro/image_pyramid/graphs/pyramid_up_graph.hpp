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
*  \file pyramid_graph.hpp
*  \brief ACF graph for the \ref secImagePyramid "image pyramid creation".
***********************************************************************************/
#ifndef PYRAMIDUPGRAPH_HPP
#define PYRAMIDUPGRAPH_HPP

#include <acf_graph.hpp>
#include "pyramid_acf.h"

class pyramid_up_graph : public ACF_Graph
{
public:
   pyramid_up_graph() : ACF_Graph()
   {
      GET_KERNEL_HANDLE(&kernel_info_horizontal_gaus_and_expand, "horizontal_gaus_and_expand");
      GET_KERNEL_HANDLE(&kernel_info_vertical_gaus, "vertical_gaus");
   }

   void Create()
   {
      // set identifier for graph
      SetIdentifier("pyramid_up_graph");
      
      // add kernels
      AddKernel("GAUS_X",     "horizontal_gaus_and_expand");
      AddKernel("GAUS_Y",     "vertical_gaus");
      
      // add graph ports
      AddInputPort("INPUT");
      AddOutputPort("OUTPUT");
      
      // specify connections
      Connect( GraphPort("INPUT"),              KernelPort("GAUS_X", "INPUT"));
      Connect( KernelPort("GAUS_X", "OUTPUT"),  KernelPort("GAUS_Y", "INPUT"));
      Connect( KernelPort("GAUS_Y", "OUTPUT"),  GraphPort("OUTPUT"));
   }
   
};


#endif /* PYRAMIDUPGRAPH_HPP */


