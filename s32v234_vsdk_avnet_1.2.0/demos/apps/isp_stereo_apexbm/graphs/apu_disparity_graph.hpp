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

#ifndef DISPARITYGRAPH_HPP
#define DISPARITYGRAPH_HPP

#include <acf_graph.hpp>

#ifdef APEX2_EMULATE
using namespace APEX2;
#endif

#include "apu_disparity_graph.h"
#include "disparity_acf.h"
#include "gauss_3x1_acf.h"

class apu_disparity_graph : public ACF_Graph
{
public:
	apu_disparity_graph()
		: ACF_Graph() {
#ifdef APEX2_EMULATE
	XREGISTER_ACF_KERNEL(DISPARITY_K)
        XREGISTER_ACF_KERNEL(GAUSS3X1_K)
#else
        GET_KERNEL_HANDLE(&kernelInfoConcat(DISPARITY_K),XSTR(DISPARITY_K));
        GET_KERNEL_HANDLE(&kernelInfoConcat(GAUSS3X1_K),XSTR(GAUSS3X1_K));
#endif
	}

	void Create()
	{
		//set identifier for graph
		SetIdentifier("disparity_graph");

		//add kernels
		AddKernel("My_DISPARITY", DISPARITY_KN);
		AddKernel("Filter1", GAUSS3X1_KN);
		AddKernel("Filter2", GAUSS3X1_KN);
		
		//add graph ports
		AddInputPort("IN0");
		AddInputPort("IN1");
		AddOutputPort("OUT");

		//specify connections
		Connect(GraphPort("IN0"), KernelPort("Filter1", "INPUT_0"));
		Connect(GraphPort("IN1"), KernelPort("Filter2", "INPUT_0"));

		Connect(KernelPort("Filter1", "OUTPUT_0"), KernelPort("My_DISPARITY", "INPUT_0"));
		Connect(KernelPort("Filter2", "OUTPUT_0"), KernelPort("My_DISPARITY", "INPUT_1"));

		Connect(KernelPort("My_DISPARITY", "OUTPUT_0"), GraphPort("OUT"));

	}
};

#endif /* DISPARITYGRAPH_HPP */
