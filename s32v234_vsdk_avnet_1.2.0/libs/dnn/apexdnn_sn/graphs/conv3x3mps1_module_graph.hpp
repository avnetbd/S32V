/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2016 Freescale Semiconductor
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
 *  @file conv3x3mps1_module_graph.hpp
 *  @brief Graph for CONV3X3MPS1_MODULE
 * ***********************************************************************************/
#ifndef CONV3X3MPS1MODULEGRAPH_HPP
#define CONV3X3MPS1MODULEGRAPH_HPP

#include <acf_graph.hpp>
#include "apexdnn_sn_acf.h"

class conv3x3mps1_module_graph : public ACF_Graph
{
public:
   conv3x3mps1_module_graph() : ACF_Graph()
   {
      GET_KERNEL_HANDLE(&_kernel_info_apu_conv3x3mps1_module_nhcw_forward, "_kernel_info_apu_conv3x3mps1_module_nhcw_forward");
   }

   void Create()
   {
      /* set identifier for graph */
      SetIdentifier("conv3x3mps1_module_graph");

      /* add kernels */
      AddKernel("APU_CONV3X3MPS1_MODULE", "apu_conv3x3mps1_module_nhcw_forward");
      SetKernelPortOutputDelay("APU_CONV3X3MPS1_MODULE", "OUTPUT_S1", 1);

      /* add graph port */
      AddInputPort ("CONV3X3MPS1_MODULE_INPUT_IMAGE");
      AddInputPort ("CONV3X3MPS1_MODULE_INPUT_WEIGHT");
      AddInputPort ("CONV3X3MPS1_MODULE_INPUT_PARAMS");
      AddOutputPort("CONV3X3MPS1_MODULE_OUTPUT_S1");

      /* specify connection */
      Connect(GraphPort("CONV3X3MPS1_MODULE_INPUT_IMAGE"),   KernelPort("APU_CONV3X3MPS1_MODULE", "INPUT_IMAGE"));
      Connect(GraphPort("CONV3X3MPS1_MODULE_INPUT_WEIGHT"),  KernelPort("APU_CONV3X3MPS1_MODULE", "INPUT_WEIGHT"));
      Connect(GraphPort("CONV3X3MPS1_MODULE_INPUT_PARAMS"),  KernelPort("APU_CONV3X3MPS1_MODULE", "INPUT_PARAMS"));

      Connect(KernelPort("APU_CONV3X3MPS1_MODULE", "OUTPUT_S1"), GraphPort("CONV3X3MPS1_MODULE_OUTPUT_S1"));
   }
};

#endif /* CONV3X3MPS1MODULEGRAPH_HPP */

