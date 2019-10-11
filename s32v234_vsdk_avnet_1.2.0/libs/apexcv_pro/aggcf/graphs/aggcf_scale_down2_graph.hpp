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
*  \brief ACF graph definition for the aggcf pyramid implementation.
***********************************************************************************/
#ifndef __AGGCF_SCALE_DOWN2_GRAPH_HPP__
#define __AGGCF_SCALE_DOWN2_GRAPH_HPP__



#include <acf_graph.hpp>
#include "aggcf_acf.h"

class aggcf_scale_down2_graph : public ACF_Graph
{
public:

   aggcf_scale_down2_graph() : ACF_Graph()
   {
      GET_KERNEL_HANDLE(&_kernel_info_agg_acf_biresize_verti_r1, "agg_acf_biresize_verti_r1");
      GET_KERNEL_HANDLE(&_kernel_info_agg_acf_biresize_hori_r1,  "agg_acf_biresize_hori_r1");
   }
  
   void Create()
   {
      //set identifier for graph
      SetIdentifier("aggcf_scale_down2_graph");
      
      //add kernels
      AddKernel("biresize_verti_r1", "agg_acf_biresize_verti_r1");
      AddKernel("biresize_hori_r1",  "agg_acf_biresize_hori_r1");

      //add graph ports
      AddInputPort("SRC");
      AddOutputPort("DST");

      // connect graph ports
      Connect(GraphPort("SRC"),                             KernelPort("biresize_hori_r1",    "INPUT" ));
      Connect(KernelPort("biresize_hori_r1",  "OUTPUT"),    KernelPort("biresize_verti_r1",   "INPUT" ));
      Connect(KernelPort("biresize_verti_r1", "OUTPUT"),    GraphPort("DST" ));
   }
};
     
#endif
