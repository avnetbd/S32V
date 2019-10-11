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
#ifndef APUFAST9GRAPH_HPP
#define APUFAST9GRAPH_HPP

#include <acf_graph.hpp>

#ifdef APEX2_EMULATE
using namespace APEX2;
#endif

#include "apu_fast9_graph_names.h"
#include "fast9_acf.h"
#include "gauss_3x3_acf.h"
#include "nms_acf.h"
#include "dilate_diamond_acf.h"
#include "add_acf.h"
#include "cvt_16low_to_8_acf.h"

class apu_fast9_graph : public ACF_Graph
{
public:
	apu_fast9_graph()
		: ACF_Graph() 
	{
		XREGISTER_ACF_KERNEL(GAUSS_3x3_K);
		XREGISTER_ACF_KERNEL(FAST9_K);
		XREGISTER_ACF_KERNEL(NMS_K);
		XREGISTER_ACF_KERNEL(DILATE_DIAMOND_K);
		XREGISTER_ACF_KERNEL(ADD_K);
		XREGISTER_ACF_KERNEL(_16_LOW_TO_8_K);
	}
   void Create()
   {
      //set identifier for graph
      SetIdentifier("apu_fast9_graph");

      //add kernels
      AddKernel("gauss_3x3_0", GAUSS_3x3_KN);
	  AddKernel("fast9_0", FAST9_KN);
      AddKernel("nms_0", NMS_KN);
	  AddKernel("dilate_diamond_0", DILATE_DIAMOND_KN);
	  AddKernel("dilate_diamond_1", DILATE_DIAMOND_KN);
	  AddKernel("dilate_diamond_2", DILATE_DIAMOND_KN);
	  //AddKernel("dilate_diamond_3", DILATE_DIAMOND_KN);
	  //AddKernel("saturate_nonzero_0", "apu_saturate_nonzero");
	  //AddKernel("mark_0", "apu_mark");
	  AddKernel("16low_to_8_0", _16_LOW_TO_8_KN);
	  AddKernel("add_0", ADD_KN);

      //add graph ports
	  AddInputPort(FAST9_GRAPH_IN0);
	  AddInputPort(FAST9_GRAPH_IN1);
	  AddOutputPort(FAST9_GRAPH_OUT);

      //specify connections
      Connect(GraphPort("INPUT_0"), KernelPort("gauss_3x3_0", GAUSS_3x3_KN_IN));
      Connect(GraphPort("INPUT_1"), KernelPort("fast9_0", FAST9_KN_IN_THR));
	  Connect(KernelPort("gauss_3x3_0", GAUSS_3x3_KN_OUT), KernelPort("fast9_0", FAST9_KN_IN_IMG));
      Connect(KernelPort("fast9_0", FAST9_KN_OUT), KernelPort("nms_0", NMS16_KN_IN));
 
	  Connect(KernelPort("nms_0", NMS_KN_OUT), KernelPort("dilate_diamond_0", DILATE_DIAMOND_KN_IN));
	  Connect(KernelPort("dilate_diamond_0", DILATE_DIAMOND_KN_OUT), KernelPort("dilate_diamond_1", DILATE_DIAMOND_KN_IN));
	  Connect(KernelPort("dilate_diamond_1", DILATE_DIAMOND_KN_OUT), KernelPort("dilate_diamond_2", DILATE_DIAMOND_KN_IN));
	  //Connect(KernelPort("dilate_diamond_2", DILATE_DIAMOND_KN_OUT), KernelPort("dilate_diamond_3", DILATE_DIAMOND_KN_IN));
	  //Connect(KernelPort("dilate_diamond_3", DILATE_DIAMOND_KN_OUT), GraphPort(FAST9_GRAPH_OUT));
	  Connect(KernelPort("dilate_diamond_2", DILATE_DIAMOND_KN_OUT), KernelPort("add_0", ADD_KN_INA));
	  Connect(GraphPort(FAST9_GRAPH_IN0), KernelPort("add_0", ADD_KN_INB));
	  Connect(KernelPort("add_0", ADD_KN_OUT), KernelPort("16low_to_8_0", _16_LOW_TO_8_KN_IN));
	  Connect(KernelPort("16low_to_8_0", _16_LOW_TO_8_KN_OUT), GraphPort(FAST9_GRAPH_OUT));
   }
};

#endif /* APUFAST9GRAPH_HPP */
