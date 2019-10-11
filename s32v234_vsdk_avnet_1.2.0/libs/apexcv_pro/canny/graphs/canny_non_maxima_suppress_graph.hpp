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
/*!*********************************************************************************
*  \file canny_non_maxima_suppress_graph.hpp
*  \brief Graph for canny non-maxima suppression
***********************************************************************************/
#ifndef CANNYNONMAXIMASUPPRESSGRAPH_HPP
#define CANNYNONMAXIMASUPPRESSGRAPH_HPP


#include <acf_graph.hpp>
#include "canny_acf.h"

class canny_non_maxima_suppress_graph : public ACF_Graph
{
public:
   canny_non_maxima_suppress_graph() : ACF_Graph()
   {
      GET_KERNEL_HANDLE(&kernel_info_canny_non_maxima_suppress, "canny_non_maxima_suppress");
   }

   void Create()
   {
      SetIdentifier("canny_non_maxima_suppress_graph");
      AddKernel("SUPPRESS",   "canny_non_maxima_suppress");
      
      AddInputPort("INPUT");
      AddInputPort("LOW_THRESH");
      AddInputPort("HIGH_THRESH");
      AddOutputPort("OUTPUT");
      
      Connect( GraphPort("LOW_THRESH"),            KernelPort("SUPPRESS",  "LOW_THRESH"));
      Connect( GraphPort("HIGH_THRESH"),           KernelPort("SUPPRESS",  "HIGH_THRESH"));
      Connect( GraphPort("INPUT"),                 KernelPort("SUPPRESS",  "INPUT"));
      
      Connect( KernelPort("SUPPRESS",  "OUTPUT"),  GraphPort("OUTPUT"));
   }
};

#endif /* CANNYNONMAXIMASUPPRESSGRAPH_HPP */

