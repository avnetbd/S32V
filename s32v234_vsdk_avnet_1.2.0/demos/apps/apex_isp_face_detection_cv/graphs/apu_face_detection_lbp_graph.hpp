/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2016 Freescale Semiconductor
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
#ifndef APUFACEDETECTIONLBPGRAPH_HPP
#define APUFACEDETECTIONLBPGRAPH_HPP

#include <acf_graph.hpp>

#ifdef APEX2_EMULATE
#pragma once
using namespace APEX2;
#endif

#include "apu_face_detection_lbp_graph.h"

//Kernel includes
#include "lbp_cascade_acf.h"
#include "sat_acf.h"


class apu_face_detection_lbp_graph : public ACF_Graph
{
public:
	apu_face_detection_lbp_graph()
		: ACF_Graph()
	{
#ifdef APEX2_EMULATE
		XREGISTER_ACF_KERNEL(LBP_CASC_K);
		XREGISTER_ACF_KERNEL(SAT_K);
#else
        GET_KERNEL_HANDLE(&kernelInfoConcat(LBP_CASC_K),XSTR(LBP_CASC_K));
        GET_KERNEL_HANDLE(&kernelInfoConcat(SAT_K),XSTR(SAT_K));
#endif
	}

  void Create()
  {
    //set identifier for graph
    SetIdentifier("apu_face_detection_lbp_graph");

    //add kernels
    AddKernel("lbp_cascade_0", LBP_CASC_KN);
	AddKernel("sat_0", SAT_KN);

    //add graph ports
    AddInputPort(FACE_DET_GR_IN_IMG);
    AddInputPort(FACE_DET_GR_IN_CASC_SZ);
    AddInputPort(FACE_DET_GR_IN_CASC_FEAT);
    AddInputPort(FACE_DET_GR_IN_CASC_STAGES);
    AddInputPort(FACE_DET_GR_IN_PIX_SHFT);
	AddInputPort(FACE_DET_GR_IN_PIX_OFFS);
    AddOutputPort("OUTPUT");
	AddOutputPort(FACE_DET_GR_OUT_INTEGR_IMG);

    //specify connections
    Connect(GraphPort(FACE_DET_GR_IN_IMG), KernelPort("sat_0", SAT_KN_IN));
	Connect(KernelPort("sat_0", SAT_KN_OUT), KernelPort("lbp_cascade_0", LBP_CASC_IN_INTEGR_IMG));
	Connect(KernelPort("sat_0", SAT_KN_OUT), GraphPort(FACE_DET_GR_OUT_INTEGR_IMG));
    Connect(GraphPort(FACE_DET_GR_IN_CASC_SZ), KernelPort("lbp_cascade_0", LBP_CASC_IN_CASC_SZ_AND_SKIP));
	Connect(GraphPort(FACE_DET_GR_IN_CASC_FEAT), KernelPort("lbp_cascade_0", LBP_CASC_IN_CASC_FEAT));
    Connect(GraphPort(FACE_DET_GR_IN_CASC_STAGES), KernelPort("lbp_cascade_0", LBP_CASC_IN_CASC_STAGES));
	Connect(GraphPort(FACE_DET_GR_IN_PIX_SHFT), KernelPort("lbp_cascade_0", LBP_CASC_IN_PIX_SHFT));
	Connect(GraphPort(FACE_DET_GR_IN_PIX_OFFS), KernelPort("lbp_cascade_0", LBP_CASC_IN_PIX_OFFS));
    Connect(KernelPort("lbp_cascade_0", LBP_CASC_OUT), GraphPort(FACE_DET_GR_OUT));
  }
};

#endif /* APUFACEDETECTIONLBPGRAPH_HPP */
