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
#ifndef ACFGAUSS3X3GRAPH_HPP
#define ACFGAUSS3X3GRAPH_HPP

#include <acf_graph.hpp>


#ifdef APEX2_EMULATE
#include "acf_lib.hpp"
using namespace APEX2;
#endif

#include "gauss_3x3_acf.h"
#include "apu_gauss_3x3_graph_names.h"

class apu_gauss_3x3_graph : public ACF_Graph
{
public:
	const char* m_graphName;
public:
	apu_gauss_3x3_graph()
		: ACF_Graph()
		, m_graphName(GAUSS_3x3_GN)
	{
		XREGISTER_ACF_KERNEL(GAUSS_3x3_K);
	}


   void Create()
   {
      //set identifier for graph
      SetIdentifier(m_graphName);

      //add kernels
	  std::string gaussInst("gauss_3x3_instance");
      AddKernel(gaussInst, GAUSS_3x3_KN);

      //add graph ports
	  AddInputPort(GAUSS_3x3_GRAPH_IN);
	  AddOutputPort(GAUSS_3x3_GRAPH_OUT);

      //specify connections
	  Connect(GraphPort(GAUSS_3x3_GRAPH_IN), KernelPort(gaussInst, GAUSS_3x3_KN_IN));
	  Connect(KernelPort(gaussInst, GAUSS_3x3_KN_OUT), GraphPort(GAUSS_3x3_GRAPH_OUT));
   }
};

#endif /* ACFGAUSS3X3GRAPH_HPP */
