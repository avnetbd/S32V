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
#ifndef APUAPUINPUTSGRAPHHPP
#define APUAPUINPUTSGRAPHHPP

#include <acf_graph.hpp>
#include <apu_segmentation.h>

class apu_inputs_graph : public ACF_Graph
{
  public:
        const char* m_graphName;

        apu_inputs_graph()
    : ACF_Graph()
    , m_graphName("apu_k_means_graph")
    {
        GET_KERNEL_HANDLE(&kernelInfoConcat(k_means_kernel),"k_means_kernel");        
    }
    
public:

   void Create()
   {
      //set identifier for graph
      SetIdentifier("apu_inputs_graph");

      //add kernels
      AddKernel("k_means_kernel_instance", "k_means_kernel");

      //add graph ports
      AddInputPort("INPUT_0");
      AddInputPort("INPUT_1");
      AddInputPort("INPUT_2");
      AddOutputPort("OUTPUT_0");

      //specify connections
      Connect(GraphPort("INPUT_0"), KernelPort("k_means_kernel_instance", "INPUT_0"));
      Connect(GraphPort("INPUT_1"), KernelPort("k_means_kernel_instance", "INPUT_1"));
      Connect(GraphPort("INPUT_2"), KernelPort("k_means_kernel_instance", "INPUT_2"));
      Connect(KernelPort("k_means_kernel_instance", "OUTPUT_0"), GraphPort("OUTPUT_0"));
   }
};

#endif /* APUAPUINPUTSGRAPHHPP */