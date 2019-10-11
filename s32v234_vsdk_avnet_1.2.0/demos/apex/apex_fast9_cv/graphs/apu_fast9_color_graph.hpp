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

#include "mark_color_channel_acf.h"
#include "saturate_nonzero_acf.h"
#include "dilate_diamond_acf.h"
#include "rgb_to_grayscale_acf.h"
#include "fast9_acf.h"
#include "gauss_3x3_acf.h"
#include "nms_acf.h"
#include "GraphNames.h"

class apu_fast9_color_graph : public ACF_Graph
{
public:
	const char* m_graphName;

	apu_fast9_color_graph()
    : ACF_Graph()
    , m_graphName(FAST9_COLOR_GN)
    {
    	GET_KERNEL_HANDLE(&kernelInfoConcat(RGB_TO_GRAY_K),XSTR(RGB_TO_GRAY_K));
    	GET_KERNEL_HANDLE(&kernelInfoConcat(GAUSS_3x3_K),XSTR(GAUSS_3x3_K));
    	GET_KERNEL_HANDLE(&kernelInfoConcat(FAST9_K),XSTR(FAST9_K));
    	GET_KERNEL_HANDLE(&kernelInfoConcat(NMS_K),XSTR(NMS_K));
    	GET_KERNEL_HANDLE(&kernelInfoConcat(DILATE_DIAMOND_K),XSTR(DILATE_DIAMOND_K));
    	GET_KERNEL_HANDLE(&kernelInfoConcat(SAT_NONZERO_K),XSTR(SAT_NONZERO_K));
    	GET_KERNEL_HANDLE(&kernelInfoConcat(MARK_COL_K),XSTR(MARK_COL_K));
    }

    void Create()
   {
      //set identifier for graph
      SetIdentifier("apu_fast9_color_graph");
      
      //add kernels
      AddKernel("rgb_to_grayscale_0", "apu_rgb_to_grayscale");
      AddKernel("gauss_3x3_0", "apu_gauss_3x3");
      AddKernel("fast9_0", "apu_fast9");
      AddKernel("nms_0", "apu_nms");
      AddKernel("dilate_diamond_0", "apu_dilate_diamond");
      AddKernel("dilate_diamond_1", "apu_dilate_diamond");
      AddKernel("saturate_nonzero_0", "apu_saturate_nonzero");
      AddKernel("mark_color_channel_0", "apu_mark_color_channel");

      //add graph ports
      AddInputPort("INPUT");
      AddInputPort("THRESHOLD");
      AddInputPort("MARK_COLOR_CHANNEL");
      AddOutputPort("OUTPUT");

      //specify connections
      Connect(GraphPort("INPUT"), KernelPort("rgb_to_grayscale_0", "INPUT_0"));
      Connect(KernelPort("rgb_to_grayscale_0", "OUTPUT_0"), KernelPort("gauss_3x3_0", "INPUT_0"));
      Connect(KernelPort("gauss_3x3_0", "OUTPUT_0"), KernelPort("fast9_0", "IN_Img"));
      Connect(GraphPort("THRESHOLD"), KernelPort("fast9_0", "IN_Thr"));
      Connect(KernelPort("fast9_0", "OUT_Img"), KernelPort("nms_0", "INPUT_0"));
      Connect(KernelPort("nms_0", "OUTPUT_0"), KernelPort("dilate_diamond_0", "INPUT_0"));
      Connect(KernelPort("dilate_diamond_0", "OUTPUT_0"), KernelPort("dilate_diamond_1", "INPUT_0"));
      Connect(KernelPort("dilate_diamond_1", "OUTPUT_0"), KernelPort("saturate_nonzero_0", "INPUT_0"));
      Connect(KernelPort("saturate_nonzero_0", "OUTPUT_0"), KernelPort("mark_color_channel_0", "INPUT_MARKER"));
      Connect(GraphPort("MARK_COLOR_CHANNEL"), KernelPort("mark_color_channel_0", "INPUT_CHANNEL_INDEX"));
      Connect(GraphPort("INPUT"), KernelPort("mark_color_channel_0", "INPUT_IMAGE"));
      Connect(KernelPort("mark_color_channel_0", "OUTPUT"), GraphPort("OUTPUT"));
   }
};


#endif /* APUFAST9COLORGRAPH_HPP */
