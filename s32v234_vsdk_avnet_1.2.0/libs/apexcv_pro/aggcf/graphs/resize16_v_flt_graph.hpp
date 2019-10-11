/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2016-2017 NXP
* All Rights Reserved
*
*****************************************************************************
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

/*!*********************************************************************************
*  \file
*  \brief ACF graph definition for the resize implementation.
***********************************************************************************/
#ifndef __RESIZE16_V_FLT_GRAPH_HPP__
#define __RESIZE16_V_FLT_GRAPH_HPP__



#include <acf_graph.hpp>
#include "resize_acf.h"
#include "aggcf_acf.h"

class resize16_v_flt_graph : public ACF_Graph
{
public:

   resize16_v_flt_graph() : ACF_Graph()
   {
      GET_KERNEL_HANDLE(&_kernel_info_vrsz16, "apu_resize16_vertical");
      GET_KERNEL_HANDLE(&_kernel_info_agg_acf_flt, "agg_acf_flt");
   }
  
   void Create()
   {

      //set identifier for graph
      SetIdentifier("resize16_v_flt_graph");
      
      //add kernels
      AddKernel("resize16_vertical", "apu_resize16_vertical");
      AddKernel("aggcf_flt", "agg_acf_flt");

      //add graph ports
      AddInputPort("SRC");
      AddInputPort("SRC_HEIGHT");
      AddOutputPort("DST");
      AddInputPort("DST_HEIGHT");
      AddInputPort("PARAMS");

      // connect graph ports
      Connect(GraphPort("SRC"),                         KernelPort("resize16_vertical",   "SRC" )      );
      Connect(GraphPort("SRC_HEIGHT"),                  KernelPort("resize16_vertical",   "SRC_HEIGHT"));
      Connect(GraphPort("DST_HEIGHT"),                  KernelPort("resize16_vertical",   "DST_HEIGHT"));
      Connect(KernelPort("resize16_vertical", "DST"),   KernelPort("aggcf_flt",           "INPUT")     );
      Connect(GraphPort("PARAMS"),                      KernelPort("aggcf_flt",           "PARAMS")    );
      Connect(KernelPort("aggcf_flt", "OUTPUT"),        GraphPort("DST")                               );
   }
};
     
#endif
