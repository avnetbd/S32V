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

#ifndef APUREMAPBMGRAPH_HPP
#define APUREMAPBMGRAPH_HPP

#include <acf_graph.hpp>
#include <remap_nearest_acf.h>

class apu_remap_bm_graph : public ACF_Graph
{
public:
        const char* m_graphName;
public:
        apu_remap_bm_graph()
                : ACF_Graph()
                , m_graphName("apu_remap_bm_graph")
        {
#ifdef APEX2_EMULATE
                XREGISTER_ACF_KERNEL(remap_nearest_grayscale); // this works only if the corresponding KERNEL_INFO structure has been defined with: extKernelInfoDecl(ADD_K);
#else
        GET_KERNEL_HANDLE(&kernelInfoConcat(remap_nearest_grayscale),XSTR(remap_nearest_grayscale));
#endif
        }

   void Create()
   {
      //set identifier for graph
      //SetIdentifier("apu_rotate_graph");

      //add kernels
      //AddKernel("rotate_180_0", "apu_rotate_180");

      //add graph ports
      //AddInputPort("INPUT_0");
      //AddOutputPort("OUTPUT_0");

      //specify connections
      //Connect(GraphPort("INPUT_0"), KernelPort("rotate_180_0", "INPUT"));
      //Connect(KernelPort("rotate_180_0", "OUTPUT"), GraphPort("OUTPUT_0"));
      
      //set identifier for graph
      SetIdentifier("apu_remap_bm_graph");

      //add kernels
      AddKernel("nn_remap", "remap_nearest_grayscale");

      //add graph ports
      AddInputPort("INPUT");
      AddInputPort("OFFSET");
      AddOutputPort("OUTPUT");

      //specify connections
      Connect(GraphPort("INPUT"), KernelPort("nn_remap", "SRC"));
      Connect(GraphPort("OFFSET"), KernelPort("nn_remap", "OFFSET"));
      Connect(KernelPort("nn_remap", "DST"), GraphPort("OUTPUT"));
   }
};

#endif /* APUREMAPBMGRAPH_HPP */

