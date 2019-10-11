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

#ifdef APEX2_EMULATE
using namespace APEX2;
#endif

#include "apu_rotate_graph_names.h"

// Kernel includes
#include "rotate_180_acf.h"


class apu_rotate_graph : public ACF_Graph
{
public:
	const char* m_graphName;
public:
	apu_rotate_graph()
		: ACF_Graph()
		, m_graphName(ROTATE_GN)
	{
		XREGISTER_ACF_KERNEL(ROTATE_K);
	}
	
	
   virtual void Create()
   {
      //set identifier for graph
      SetIdentifier("apu_rotate_graph");

      //add kernels
	  std::string rotateInst("rotate_180_0");
      AddKernel(rotateInst, ROTATE_KN);

      //add graph ports
	  AddInputPort(ROTATE_GRAPH_IN);
      AddOutputPort(ROTATE_GRAPH_OUT);

      //specify connections
	  Connect(GraphPort(ROTATE_GRAPH_IN), KernelPort(rotateInst, ROTATE_KN_IN));
	  Connect(KernelPort(rotateInst, ROTATE_KN_OUT), GraphPort(ROTATE_GRAPH_OUT));
   }
};

#endif /* APUROTATEGRAPH_HPP */
