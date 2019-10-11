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
#ifndef ACFDOWNSAMPLEUPSAMPLEGRAPH_HPP
#define ACFDOWNSAMPLEUPSAMPLEGRAPH_HPP

#include "acf_graph.hpp"

#ifdef APEX2_EMULATE
using namespace APEX2;
#endif

#include "apu_downsample_upsample_graph_names.h"

//Kernels
#include "downsample_acf.h"
#include "upsample_acf.h"

class apu_downsample_upsample_graph : public ACF_Graph
{
public:
	const char* m_graphName;
public:
	apu_downsample_upsample_graph()
		: ACF_Graph()
		, m_graphName(DOWNSAMPLE_UPSAMPLE_GN)
	{
		XREGISTER_ACF_KERNEL(UPSAMPLE_K)
		XREGISTER_ACF_KERNEL(DOWNSAMPLE_K)
    }
	
   virtual void Create()
   {
      //set identifier for graph
      SetIdentifier("apu_downsample_upsample_graph");

      //add kernels
	  std::string downsampleInst("downsample_0");
	  AddKernel(downsampleInst, DOWNSAMPLE_KN);

	  std::string upsampleInst("upsample_0");
      AddKernel(upsampleInst, UPSAMPLE_KN);

      //add graph ports
	  AddInputPort(DOWNSAMPLE_UPSAMPLE_IN);
	  AddOutputPort(DOWNSAMPLE_UPSAMPLE_OUT_DOWN);
	  AddOutputPort(DOWNSAMPLE_UPSAMPLE_OUT_UP);

      //specify connections
	  Connect(GraphPort(DOWNSAMPLE_UPSAMPLE_IN), KernelPort(downsampleInst, DOWNSAMPLE_KN_IN));
	  Connect(KernelPort(downsampleInst, DOWNSAMPLE_KN_OUT), GraphPort(DOWNSAMPLE_UPSAMPLE_OUT_DOWN));
	  Connect(GraphPort(DOWNSAMPLE_UPSAMPLE_IN), KernelPort(upsampleInst, UPSAMPLE_KN_IN));
	  Connect(KernelPort(upsampleInst, UPSAMPLE_KN_OUT), GraphPort(DOWNSAMPLE_UPSAMPLE_OUT_UP));
   }
};

#endif /* ACFDOWNSAMPLEUPSAMPLEGRAPH_HPP */
