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
#ifndef ANALYSISGRAPH_HPP
#define ANALYSISGRAPH_HPP

#include <acf_graph.hpp>
#include <column_sum_acf.h>
#include <reduction_for_clmn_acf.h>

class analysis_graph : public ACF_Graph
{
public:
        const char* m_graphName;
public:
        analysis_graph()
                : ACF_Graph()
                , m_graphName("ANALYSIS")
        {
#ifdef APEX2_EMULATE
                XREGISTER_ACF_KERNEL(apu_columns_sum); // this works only if the corresponding KERNEL_INFO structure has been defined with: extKernelInfoDecl(ADD_K);
                XREGISTER_ACF_KERNEL(apu_reduction_for_clmn); // this works only if the corresponding KERNEL_INFO structure has been defined with: extKernelInfoDecl(ADD_K);
#else
        GET_KERNEL_HANDLE(&kernelInfoConcat(apu_columns_sum),XSTR(apu_columns_sum));
        GET_KERNEL_HANDLE(&kernelInfoConcat(apu_reduction_for_clmn),XSTR(apu_reduction_for_clmn));
#endif
        }

   void Create()
   {
      //set identifier for graph
      SetIdentifier("ANALYSIS");
      
      AddKernel("columns_sum", "apu_columns_sum");
      AddKernel("columns_reduc", "apu_reduction_for_clmn");
      
      AddInputPort("InputBirdView");
      AddInputPort("Prior_down");
      AddInputPort("Prior_up");
      AddInputPort("Start_tile");
      
      AddOutputPort("Columns_sum_down");
      AddOutputPort("Columns_sum_up");
      
      Connect(GraphPort("InputBirdView"), KernelPort("columns_sum", "INPUT_0"));
      Connect(GraphPort("Prior_down"), KernelPort("columns_sum", "INPUT_1"));
      Connect(GraphPort("Prior_up"), KernelPort("columns_sum", "INPUT_2"));
      Connect(GraphPort("Start_tile"), KernelPort("columns_sum", "INPUT_3"));
      
      Connect(KernelPort("columns_sum", "OUTPUT_0"), KernelPort("columns_reduc", "INPUT_0"));
      Connect(KernelPort("columns_sum", "OUTPUT_1"), KernelPort("columns_reduc", "INPUT_1"));
      
      Connect(KernelPort("columns_reduc", "OUTPUT_0"), GraphPort("Columns_sum_down"));
      Connect(KernelPort("columns_reduc", "OUTPUT_1"), GraphPort("Columns_sum_up"));
      
      /*Connect(KernelPort("columns_sum", "OUTPUT_0"), GraphPort("Columns_sum_down"));
      Connect(KernelPort("columns_sum", "OUTPUT_1"), GraphPort("Columns_sum_down"));*/
   }
};

#endif /* ANALYSISGRAPH_HPP */
