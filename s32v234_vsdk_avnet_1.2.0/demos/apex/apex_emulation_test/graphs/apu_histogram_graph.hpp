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
#ifndef HISTOGRAMGRAPH_HPP
#define HISTOGRAMGRAPH_HPP

#include <acf_graph.hpp>

#ifdef APEX2_EMULATE
using namespace APEX2;
#endif

#include "apu_histogram_graph_names.h"

// Kernels
#include "reduction_acf.h"
#include "histogram_acf.h"


class apu_histogram_graph : public ACF_Graph
{
public:
	const char* m_graphName;
public:
	apu_histogram_graph()
		: ACF_Graph()
		, m_graphName(HISTOGRAM_GN)
	{
		XREGISTER_ACF_KERNEL(HISTOGRAM_K)
		XREGISTER_ACF_KERNEL(REDUCTION_K)
	}
	
   virtual void Create()
   {
      //set identifier for graph
      SetIdentifier("apu_histogram_graph");

      //add kernels
	  std::string histInst("histogram_0");
      AddKernel(histInst, HISTOGRAM_KN);

	  std::string redInst("reduction_0");
      AddKernel(redInst, REDUCTION_KN);

      //add graph ports
      AddInputPort(HISTOGRAM_GRAPH_IN);
      AddOutputPort(HISTOGRAM_GRAPH_OUT);

      //specify connections
      Connect(GraphPort(HISTOGRAM_GRAPH_IN), KernelPort(histInst, HISTOGRAM_KN_IN));
	  Connect(KernelPort(histInst, HISTOGRAM_KN_OUT), KernelPort(redInst, REDUCTION_KN_IN));
	  Connect(KernelPort(redInst, REDUCTION_KN_OUT), GraphPort(HISTOGRAM_GRAPH_OUT));
   }
};


#endif /* HISTOGRAMGRAPH_HPP */
