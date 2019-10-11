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
#ifndef APUDOWNSAMPLEUPSAMPLEGRAPH_HPP
#define APUDOWNSAMPLEUPSAMPLEGRAPH_HPP

#include <acf_graph.hpp>

#include <downsample_acf.h>
#include <upsample_acf.h>

class apu_downsample_upsample_graph : public ACF_Graph
{
public:
        const char* m_graphName;

        apu_downsample_upsample_graph() : ACF_Graph(), m_graphName("apu_downsample_upsample_graph")
        {
          GET_KERNEL_HANDLE(&kernelInfoConcat(apu_downsample),"apu_downsample");
          GET_KERNEL_HANDLE(&kernelInfoConcat(apu_upsample),"apu_upsample");
        }
        
   void Create()
   {
      //set identifier for graph
      SetIdentifier("apu_downsample_upsample_graph");

      //add kernels
      AddKernel("downsample_0", "apu_downsample");
      AddKernel("upsample_0", "apu_upsample");

      //add graph ports
      AddInputPort("INPUT");
      AddOutputPort("OUTPUT_DOWN");
      AddOutputPort("OUTPUT_UP");

      //specify connections
      Connect(GraphPort("INPUT"), KernelPort("downsample_0", "INPUT_0"));
      Connect(KernelPort("downsample_0", "OUTPUT_0"), GraphPort("OUTPUT_DOWN"));
      Connect(GraphPort("INPUT"), KernelPort("upsample_0", "INPUT_0"));
      Connect(KernelPort("upsample_0", "OUTPUT_0"), GraphPort("OUTPUT_UP"));
   }
};

#endif /* APUDOWNSAMPLEUPSAMPLEGRAPH_HPP */
