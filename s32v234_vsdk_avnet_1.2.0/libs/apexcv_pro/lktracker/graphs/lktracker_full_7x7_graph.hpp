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
 * *  @file lktracker_full_7x7_graph.hpp
 * *  @brief Graph for L-K tracker full graph 7x7 window
 * ***********************************************************************************/
#ifndef LKTRACKERFULL7X7GRAPH_HPP
#define LKTRACKERFULL7X7GRAPH_HPP

#include <acf_graph.hpp>
#include "lktracker_acf.h"

class lktracker_full_7x7_graph : public ACF_Graph
{
public:
   lktracker_full_7x7_graph() : ACF_Graph()
   {
       GET_KERNEL_HANDLE(&_kernel_info_lktracker_templ_bilinear_08u_7x7, "lktracker_templ_bilinear_08u_7x7");
       GET_KERNEL_HANDLE(&_kernel_info_lktracker_ht_centraldxdy_7x7, "lktracker_ht_centraldxdy_7x7");
       GET_KERNEL_HANDLE(&_kernel_info_lktracker_core_7x7, "lktracker_core_7x7");
   }

   void Create()
   {
      /* set identifier for graph */
      SetIdentifier("lktracker_full_7x7_graph");

		/* add kernels */
		AddKernel("TEMPLATE_BILINEAR_7X7", "lktracker_templ_bilinear_08u_7x7");
		AddKernel("TEMPLATE_DXDY_7X7",     "lktracker_ht_centraldxdy_7x7");
		AddKernel("CORE_7X7",              "lktracker_core_7x7");

		/* add graph port */
		AddInputPort("INPUT_TEMPLATE");
		AddInputPort("INPUT_PATCH");
		AddInputPort("INPUT_COOR_XY0");
		AddInputPort("INPUT_COOR_XY1");
		AddInputPort("PARAMS");            // Bytes [0-3]: MAX_X; [4-7]: MAX_Y; [8]: NUM_ITER; [9-11]: K_0, K_1, K_2
		AddOutputPort("OUTPUT_COOR_XY1");

		/* specify connection */
		Connect(GraphPort("INPUT_TEMPLATE"),                     KernelPort("TEMPLATE_BILINEAR_7X7", "INPUT_0"));
		Connect(GraphPort("INPUT_COOR_XY0"),                     KernelPort("TEMPLATE_BILINEAR_7X7", "INPUT_1"));
		Connect(KernelPort("TEMPLATE_BILINEAR_7X7", "OUTPUT_0"), KernelPort("TEMPLATE_DXDY_7X7", "INPUT_0"));
      Connect(GraphPort("PARAMS"),                             KernelPort("TEMPLATE_DXDY_7X7", "INPUT_1"));

		Connect(GraphPort("INPUT_PATCH"),                        KernelPort("CORE_7X7", "INPUT_0"));
		Connect(KernelPort("TEMPLATE_BILINEAR_7X7", "OUTPUT_0"), KernelPort("CORE_7X7", "INPUT_1"));
		Connect(KernelPort("TEMPLATE_DXDY_7X7", "OUTPUT_0"),     KernelPort("CORE_7X7", "INPUT_2"));
		Connect(KernelPort("TEMPLATE_DXDY_7X7", "OUTPUT_1"),     KernelPort("CORE_7X7", "INPUT_3"));
		Connect(GraphPort("INPUT_COOR_XY1"),                     KernelPort("CORE_7X7", "INPUT_4"));
      Connect(GraphPort("PARAMS"),                             KernelPort("CORE_7X7", "INPUT_5"));
		Connect(KernelPort("CORE_7X7", "OUTPUT_0"), GraphPort("OUTPUT_COOR_XY1"));
   }
};


#endif /* LKTRACKERFULL7X7GRAPH_HPP */
