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
#ifndef APUGAUSS5X5GRAPH_HPP
#define APUGAUSS5X5GRAPH_HPP

#include <acf_graph.hpp>

#include <gauss_5x5_acf.h>

class apu_gauss_5x5_graph : public ACF_Graph
{
public:
        const char* m_graphName;

        apu_gauss_5x5_graph()
    : ACF_Graph()
    , m_graphName("apu_gauss_5x5_graph")
    {
        GET_KERNEL_HANDLE(&kernelInfoConcat(apu_gauss_5x5),"apu_gauss_5x5");
    }
    
   void Create()
   {
      //set identifier for graph
      SetIdentifier("apu_gauss_5x5_graph");

      //add kernels
      AddKernel("gauss_5x5_instance", "apu_gauss_5x5");

      //add graph ports
      AddInputPort("INPUT_0");
      AddOutputPort("OUTPUT_0");

      //specify connections
      Connect(GraphPort("INPUT_0"), KernelPort("gauss_5x5_instance", "INPUT_0"));
      Connect(KernelPort("gauss_5x5_instance", "OUTPUT_0"), GraphPort("OUTPUT_0"));
   }
};

#endif /* APUGAUSS5X5GRAPH_HPP */