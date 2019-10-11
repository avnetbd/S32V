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

#ifndef APUDISPARITYAPUPROCESSDESC_HPP
#define APUDISPARITYAPUPROCESSDESC_HPP


#include <acf_process_desc_apu.hpp>
#include "apu_disparity_graph.hpp"

#ifdef APEX2_EMULATE
using namespace APEX2;
#endif

class apu_disparity_apu_process_desc : public ACF_Process_Desc_APU
{     
public:

	void Create()
	{
		Initialize(mGraph, "APU_DISPARITY");

		SetInputChunkSize("IN0", 64, 2);
		SetInputChunkSize("IN1", 64, 2);

	}

	apu_disparity_graph mGraph;

};

#endif /* APUDISPARITYAPUPROCESSDESC_HPP */

