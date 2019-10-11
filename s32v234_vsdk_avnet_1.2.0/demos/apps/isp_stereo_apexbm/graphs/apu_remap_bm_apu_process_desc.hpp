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

#ifndef APUREMAPBMAPUPROCESSDESC_HPP
#define APUREMAPBMAPUPROCESSDESC_HPP

#include <acf_process_desc_apu.hpp>
#include "apu_remap_bm_graph.hpp"

const int TRANSFER_TILE_SIZE_X = 20;
const int TRANSFER_TILE_SIZE_Y = 4;

class apu_remap_bm_apu_process_desc : public ACF_Process_Desc_APU
{
public:

   void Create()
   {
      Initialize(mGraph, "APU_REMAP_BM");

      SetInputChunkSize("INPUT", TRANSFER_TILE_SIZE_X, TRANSFER_TILE_SIZE_Y);
      SetInputChunkSize("OFFSET", TRANSFER_TILE_SIZE_X, TRANSFER_TILE_SIZE_Y);
      FlagInputAsChunkBasedIndirect("INPUT");
   }

   apu_remap_bm_graph mGraph;
};

#endif /* APUREMAPBMAPUPROCESSDESC_HPP */

