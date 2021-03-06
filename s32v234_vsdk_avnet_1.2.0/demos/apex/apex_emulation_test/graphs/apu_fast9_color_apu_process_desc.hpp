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
#ifndef APUFAST9COLORAPUPROCESSDESC_HPP
#define APUFAST9COLORAPUPROCESSDESC_HPP

#ifndef APEX2_EMULATE
#include <acf_process_desc_apu.hpp>
#endif

#include "apu_fast9_graph_names.h"
#include "apu_fast9_color_graph.hpp"


class apu_fast9_color_apu_process_desc : public ACF_Process_Desc_APU
{
public:

   void Create()
   {
      Initialize(mGraph, FAST9_COLOR_GN);

      SetInputChunkSize(FAST9_GRAPH_IN0, 8, 8);
	  //SetInputChunkSize(FAST9_GRAPH_THR, 1, 1);
	  //SetInputChunkSize(FAST9_GRAPH_MARK, 1, 1);
   }

   apu_fast9_color_graph mGraph;
};


#endif /* APUFAST9COLORAPUPROCESSDESC_HPP */

