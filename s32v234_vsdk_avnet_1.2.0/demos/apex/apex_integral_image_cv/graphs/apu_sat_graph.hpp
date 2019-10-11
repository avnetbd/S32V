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
#ifndef APUSATGRAPH_HPP
#define APUSATGRAPH_HPP

#include <acf_graph.hpp>
#include <sat_acf.h>
#include <sat_box_filter_acf.h>

class apu_sat_graph : public ACF_Graph
{
public:
        const char* m_graphName;
public:
        apu_sat_graph()
                : ACF_Graph()
                , m_graphName("apu_sat_graph")
        {
#ifdef APEX2_EMULATE
                XREGISTER_ACF_KERNEL(apu_sat); // this works only if the corresponding KERNEL_INFO structure has been defined with: extKernelInfoDecl(ADD_K);
                XREGISTER_ACF_KERNEL(apu_sat_box_filter); // this works only if the corresponding KERNEL_INFO structure has been defined with: extKernelInfoDecl(ADD_K);
#else
        GET_KERNEL_HANDLE(&kernelInfoConcat(apu_sat),XSTR(apu_sat));
        GET_KERNEL_HANDLE(&kernelInfoConcat(apu_sat_box_filter),XSTR(apu_sat_box_filter));
#endif
        }

   void Create()
   {
      //set identifier for graph
      SetIdentifier("apu_sat_graph");

      //add kernels
      AddKernel("apu_sat_0", "apu_sat");
      AddKernel("apu_sat_box_filter_0", "apu_sat_box_filter");

      //add graph ports
      AddInputPort("INPUT_0");
      AddOutputPort("OUTPUT_0");
      AddOutputPort("OUTPUT_1");

      //specify connections
      Connect(GraphPort("INPUT_0"), KernelPort("apu_sat_0", "INPUT"));
      Connect(KernelPort("apu_sat_0", "OUTPUT"), KernelPort("apu_sat_box_filter_0", "INPUT"));
      Connect(KernelPort("apu_sat_0", "OUTPUT"), GraphPort("OUTPUT_0"));
      Connect(KernelPort("apu_sat_box_filter_0", "OUTPUT"), GraphPort("OUTPUT_1"));
   }
};

#endif /* APUSATGRAPH_HPP */