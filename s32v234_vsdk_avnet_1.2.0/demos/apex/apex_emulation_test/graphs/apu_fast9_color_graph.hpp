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
#ifndef APUFAST9COLORGRAPH_HPP
#define APUFAST9COLORGRAPH_HPP

#include <acf_graph.hpp>

#ifdef APEX2_EMULATE
using namespace APEX2;
#endif

#include "apu_fast9_graph_names.h"

// Kernel includes
#include "rgb_to_grayscale_acf.h"
#include "gauss_3x3_acf.h"
#include "fast9_acf.h"
#include "nms_acf.h"
#include "dilate_diamond_acf.h"
#include "saturate_nonzero_acf.h"
#include "mark_color_channel_acf.h"

class apu_fast9_color_graph : public ACF_Graph
{
public:
	const char* m_graphName;

public:
	apu_fast9_color_graph()
		: ACF_Graph()
		, m_graphName(FAST9_COLOR_GN)
	{
		XREGISTER_ACF_KERNEL(DILATE_DIAMOND_K);
		XREGISTER_ACF_KERNEL(NMS_K);		
		XREGISTER_ACF_KERNEL(FAST9_K);
		XREGISTER_ACF_KERNEL(RGB_TO_GRAY_K);
		XREGISTER_ACF_KERNEL(GAUSS_3x3_K);
		XREGISTER_ACF_KERNEL(SAT_NONZERO_K);
		XREGISTER_ACF_KERNEL(MARK_COL_K);
	}
	
   virtual void Create()
   {
      //set identifier for graph
      SetIdentifier("apu_fast9_color_graph");

      //add kernels
      AddKernel("rgb_to_grayscale_0", RGB_TO_GRAY_KN);
      AddKernel("gauss_3x3_0", GAUSS_3x3_KN);
      AddKernel("fast9_0", FAST9_KN);
      AddKernel("nms_0", NMS_KN);
      AddKernel("dilate_diamond_0", DILATE_DIAMOND_KN);
	  //AddKernel("dilate_diamond_1", DILATE_DIAMOND_KN);
	  AddKernel("saturate_nonzero_0", SAT_NONZERO_KN);
      AddKernel("mark_color_channel_0", MARK_COL_KN);

      //add graph ports
	  AddInputPort(FAST9_GRAPH_IN0);
	  AddInputPort(FAST9_GRAPH_THR);
	  AddInputPort(FAST9_GRAPH_MARK);
      AddOutputPort(FAST9_GRAPH_OUT);

      //specify connections
      Connect(GraphPort(FAST9_GRAPH_IN0), KernelPort("rgb_to_grayscale_0", RGB_TO_GRAY_KN_IN));
	  Connect(KernelPort("rgb_to_grayscale_0", RGB_TO_GRAY_KN_OUT), KernelPort("gauss_3x3_0", GAUSS_3x3_KN_IN));
	  Connect(KernelPort("gauss_3x3_0", GAUSS_3x3_KN_OUT), KernelPort("fast9_0", FAST9_KN_IN_IMG));
	  Connect(GraphPort(FAST9_GRAPH_THR), KernelPort("fast9_0", FAST9_KN_IN_THR));
	  Connect(KernelPort("fast9_0", FAST9_KN_OUT), KernelPort("nms_0", NMS_KN_IN));
	  Connect(KernelPort("nms_0", NMS_KN_OUT), KernelPort("dilate_diamond_0", DILATE_DIAMOND_KN_IN));
	  //Connect(KernelPort("dilate_diamond_0", DILATE_DIAMOND_KN_OUT), KernelPort("dilate_diamond_1", DILATE_DIAMOND_KN_IN));
	  //Connect(KernelPort("dilate_diamond_1", DILATE_DIAMOND_KN_OUT), KernelPort("saturate_nonzero_0", SAT_NONZERO_IN));
	  Connect(KernelPort("dilate_diamond_0", DILATE_DIAMOND_KN_OUT), KernelPort("saturate_nonzero_0", SAT_NONZERO_IN));
	  Connect(KernelPort("saturate_nonzero_0", SAT_NONZERO_OUT), KernelPort("mark_color_channel_0", MARK_COL_KN_MARKER));
	  Connect(GraphPort(FAST9_GRAPH_MARK), KernelPort("mark_color_channel_0", MARK_COL_KN_CHN_IDX));
	  Connect(GraphPort(FAST9_GRAPH_IN0), KernelPort("mark_color_channel_0", MARK_COL_KN_IN));
	  Connect(KernelPort("mark_color_channel_0", MARK_COL_KN_OUT), GraphPort(FAST9_GRAPH_OUT));
   }
};
#endif /* APUFAST9COLORGRAPH_HPP */
