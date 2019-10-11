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

#ifndef AGGCFLUVGRAPH_HPP
#define AGGCFLUVGRAPH_HPP

#include <acf_graph.hpp>
#include "aggcf_acf.h"

class aggcf_luv_graph : public ACF_Graph
{
public:
   aggcf_luv_graph() : ACF_Graph()
   {
      GET_KERNEL_HANDLE(&_kernel_info_agg_acf_luv, "agg_acf_luv");
   }

   void Create()
   {
      // Set identifier for graph
      SetIdentifier("aggcf_luv_graph");

      // add kernels
      AddKernel("AGGCF_LUV_C", "agg_acf_luv");

      // add graph port
      AddInputPort("INPUT");
      AddOutputPort("OUTPUTL");
      AddOutputPort("OUTPUTU");
      AddOutputPort("OUTPUTV");
      

      // specify connections
      Connect(  GraphPort("INPUT"),                  KernelPort("AGGCF_LUV_C", "INPUT"));
      Connect(  KernelPort("AGGCF_LUV_C", "OUTPUTL"),  GraphPort("OUTPUTL"));
      Connect(  KernelPort("AGGCF_LUV_C", "OUTPUTU"),  GraphPort("OUTPUTU"));
      Connect(  KernelPort("AGGCF_LUV_C", "OUTPUTV"),  GraphPort("OUTPUTV"));

   }
};

#endif

