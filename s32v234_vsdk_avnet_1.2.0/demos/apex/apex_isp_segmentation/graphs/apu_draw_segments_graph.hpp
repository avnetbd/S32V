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
#ifndef APUAPUDRAWSEGMENTSGRAPH_HPP
#define APUAPUDRAWSEGMENTSGRAPH_HPP

#include <acf_graph.hpp>
#include <apu_segmentation.h>

class apu_draw_segments_graph : public ACF_Graph
{
  public:
        const char* m_graphName;

        apu_draw_segments_graph()
    : ACF_Graph()
    , m_graphName("apu_draw_segments_graph")
    {
        GET_KERNEL_HANDLE(&kernelInfoConcat(draw_contours_around_segments_kernel),"draw_contours_around_segments_kernel");        
    }

   void Create()
   {
      //set identifier for graph
      SetIdentifier("apu_draw_segments_graph");

      //add kernels
      AddKernel("draw_contours_around_segments_instance", "draw_contours_around_segments_kernel");

      //add graph ports
      AddInputPort("INPUT_0");
      AddInputPort("INPUT_1");
      AddOutputPort("OUTPUT_0");

      //specify connections
      Connect(GraphPort("INPUT_0"), KernelPort("draw_contours_around_segments_instance", "INPUT_0"));
      Connect(GraphPort("INPUT_1"), KernelPort("draw_contours_around_segments_instance", "INPUT_1"));
      Connect(KernelPort("draw_contours_around_segments_instance", "OUTPUT_0"), GraphPort("OUTPUT_0"));
   }
};

#endif /* APUAPUDRAWSEGMENTSGRAPH_HPP */