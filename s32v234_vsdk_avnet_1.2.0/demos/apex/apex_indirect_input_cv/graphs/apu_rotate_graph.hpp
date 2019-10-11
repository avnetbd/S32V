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
#ifndef APUROTATEGRAPH_HPP
#define APUROTATEGRAPH_HPP

#include <acf_graph.hpp>
#include <rotate_180_acf.h>

class apu_rotate_graph : public ACF_Graph
{
  public:
        const char* m_graphName;

        apu_rotate_graph()
                : ACF_Graph()
                , m_graphName("apu_rotate_graph")
        {
#ifdef APEX2_EMULATE
           XREGISTER_ACF_KERNEL(apu_rotate_180); // this works only if the corresponding KERNEL_INFO structure has been defined with: extKernelInfoDecl(ADD_K);
#else
           GET_KERNEL_HANDLE(&kernelInfoConcat(apu_rotate_180),XSTR(apu_rotate_180));
#endif
        }
        
   void Create()
   {
      //set identifier for graph
      SetIdentifier("apu_rotate_graph");

      //add kernels
      AddKernel("rotate_180_0", "apu_rotate_180");

      //add graph ports
      AddInputPort("INPUT_0");
      AddOutputPort("OUTPUT_0");

      //specify connections
      Connect(GraphPort("INPUT_0"), KernelPort("rotate_180_0", "INPUT"));
      Connect(KernelPort("rotate_180_0", "OUTPUT"), GraphPort("OUTPUT_0"));
   }
};

#endif /* APUROTATEGRAPH_HPP */