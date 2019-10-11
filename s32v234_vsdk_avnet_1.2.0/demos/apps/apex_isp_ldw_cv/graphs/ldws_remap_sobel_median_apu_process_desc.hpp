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
#ifndef LDWSREMAPSOBELMEDIANAPUPROCESSDESC_HPP
#define LDWSREMAPSOBELMEDIANAPUPROCESSDESC_HPP

#include <acf_process_desc_apu.hpp>
#include "ldws_remap_sobel_median_graph.hpp"

class ldws_process_desc : public ACF_Process_Desc_APU
{
public:

   void Create()
   {
      Initialize(m_ldws_graph, "LDWS_REMAP_SOBEL_MEDIAN");
      FlagInputAsChunkBasedIndirect("SRC_IMG");
      SetInputChunkSize("LOCAL_OFFSET", 6, 8);
      SetInputChunkSize("DELTA", 6, 8);
   }

   ldws_graph m_ldws_graph;
};

#endif /* LDWSREMAPSOBELMEDIANAPUPROCESSDESC_HPP */
