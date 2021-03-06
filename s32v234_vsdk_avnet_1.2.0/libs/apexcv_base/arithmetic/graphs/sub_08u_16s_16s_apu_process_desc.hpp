/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2016 Freescale Semiconductor
* Copyright 2018 NXP 
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
***************************************************************************
  
/*!*********************************************************************************
*  @file sub_08u_16s_16s_proc_desc.hpp
*  @brief Process description for arithmetic subtraction (signed 16 bit)
***********************************************************************************/
#ifndef SUB08U16S16SAPUPROCESSDESC_HPP
#define SUB08U16S16SAPUPROCESSDESC_HPP

#include <acf_process_desc_apu.hpp>


#include "arithmetic_acf.h"
#include <acf_graph.hpp>
#include "graph_templates.h"

GRAPH_CLASS_DEF_3In_1Out(SUB_08u_16s_16s_K);

#define SUB_08U_16S_16S_P_ID    SUB_08U_16S_16S
#define SUB_08U_16S_16S_P_STR  XSTR(SUB_08U_16S_16S_P_ID)

class sub_08u_16s_16s_apu_process_desc : public ACF_Process_Desc_APU
{
public:

  void Create()
  {
    Initialize(mGraph, SUB_08U_16S_16S_P_STR);
  }
  graphClassName(SUB_08u_16s_16s_K) mGraph;
};


#ifdef APEX2_EMULATE
REGISTER_PROCESS_TYPE(SUB_08U_16S_16S_P_ID, sub_08u_16s_16s_apu_process_desc)
#endif
#endif /* SUB08U16S16SAPUPROCESSDESC_HPP */
