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
#ifndef APUHISTOGRAMGRAPH_HPP
#define APUHISTOGRAMGRAPH_HPP

#include <acf_graph.hpp>

#include "apu_histogram_graph.h"
#include "histogram_acf.h"
#include "reduction_acf.h"

class apu_histogram_graph : public ACF_Graph
{
public:
	const char* m_graphName;
public:
	apu_histogram_graph() 
		: ACF_Graph()
		, m_graphName(HISTOGRAM_GN)
	{
#ifdef APEX2_EMULATE
		XREGISTER_ACF_KERNEL(HISTOGRAM_KN)
		XREGISTER_ACF_KERNEL(REDUCTION_KN)
#else
        GET_KERNEL_HANDLE(&kernelInfoConcat(HISTOGRAM_K),XSTR(HISTOGRAM_K));
        GET_KERNEL_HANDLE(&kernelInfoConcat(REDUCTION_K),XSTR(REDUCTION_K));
#endif
	}

   void Create()
   {
      //set identifier for graph
      SetIdentifier("apu_histogram_graph");

      //add kernels
	  AddKernel("histogram_0", HISTOGRAM_KN);
      AddKernel("reduction_0", REDUCTION_KN);

      //add graph ports
      AddInputPort(HISTOGRAM_IN);
	  AddOutputPort(HISTOGRAM_OUT);

      //specify connections
	  Connect(GraphPort(HISTOGRAM_IN), KernelPort("histogram_0", HISTOGRAM_KN_IN));
	  Connect(KernelPort("histogram_0", HISTOGRAM_KN_OUT), KernelPort("reduction_0", REDUCTION_KN_IN));
	  Connect(KernelPort("reduction_0", REDUCTION_KN_OUT), GraphPort(HISTOGRAM_OUT));
   }
};

#endif /* APUHISTOGRAMGRAPH_HPP */