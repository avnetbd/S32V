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


/******************************************************************************
 * NOTE: multiple graphs
 * There are two graphs for the LDW2 implementation. The key difference is
 * the location of the linear regression line code.
 *
 * (1) no_reg: does NOT contain the regression line kernel. Instead, this graph outputs
 *             set of points and linear regression must be enabled on the ARM.
 * (2) reg:    contains the linear regression line kernel as part of the graph.
 * 
 * To toggle: edit toggle_regression.h and comment or uncomment the #define.
 *
 * This is done to reduce the size of the program so it can be used by some FPGAs.
 * On systems with less than 32 kB program memory, use option (1). On systems with
 * more than this, use option (2).
 ******************************************************************************/
#ifndef ACFFILTERSNOREGAPUPROCESSDESC_HPP
#define ACFFILTERSNOREGAPUPROCESSDESC_HPP

#include <acf_process_desc_apu.hpp>
#include <ACF_filters_no_reg_graph.hpp>

class ACF_filters_no_reg_apu_process_desc : public ACF_Process_Desc_APU
{
public:

   void Create()
   {
      Initialize(m_GdcLdw_graph, "ACF_FILTERS_NO_REG");
      FlagInputAsChunkBasedIndirect("SRC_IMG");
      //FlagInputAsChunkBasedIndirect("THRESHOLD_PREVIOUS");
      // SetInputChunkSize("SRC_IMG", 4, 4);
      // SetInputChunkSize("THRESHOLD_PREVIOUS", 8, 8);
   }

   ACF_filters_no_reg_graph m_GdcLdw_graph;
};

#endif /* ACFFILTERSNOREGAPUPROCESSDESC_HPP */

