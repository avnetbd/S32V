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
#ifndef LDWSREMAPSOBELMEDIANGRAPH_HPP
#define LDWSREMAPSOBELMEDIANGRAPH_HPP

#include <acf_graph.hpp>
#include <remap_bilinear_acf.h>
#include <filtering_sobel_3x3_acf.h>
#include <filter_median_3x3_acf.h>

class ldws_graph : public ACF_Graph
{
public:
        const char* m_graphName;
public:
        ldws_graph()
                : ACF_Graph()
                , m_graphName("ldws_remap_sobel_median")
        {
#ifdef APEX2_EMULATE
                XREGISTER_ACF_KERNEL(remap_bilinear_grayscale); // this works only if the corresponding KERNEL_INFO structure has been defined with: extKernelInfoDecl(ADD_K);
                XREGISTER_ACF_KERNEL(sobel_3x3_8bpp); // this works only if the corresponding KERNEL_INFO structure has been defined with: extKernelInfoDecl(ADD_K);
                XREGISTER_ACF_KERNEL(median_3x3_8bpp); // this works only if the corresponding KERNEL_INFO structure has been defined with: extKernelInfoDecl(ADD_K);
#else
        GET_KERNEL_HANDLE(&kernelInfoConcat(remap_bilinear_grayscale),XSTR(remap_bilinear_grayscale));
        GET_KERNEL_HANDLE(&kernelInfoConcat(sobel_3x3_8bpp),XSTR(sobel_3x3_8bpp));
        GET_KERNEL_HANDLE(&kernelInfoConcat(median_3x3_8bpp),XSTR(median_3x3_8bpp));
#endif
        }

   void Create()
   {
      //set identifier for graph
      SetIdentifier("ldws_remap_sobel_median");

      //add kernels
      AddKernel("myRemap",  "remap_bilinear_grayscale"); // yuv once implemented
      AddKernel("mySobel",  "sobel_3x3_8bpp");
      AddKernel("myMedian", "median_3x3_8bpp");

      //add graph ports
      AddInputPort("SRC_IMG");
      AddInputPort("LOCAL_OFFSET");
      AddInputPort("DELTA");
      AddOutputPort("DST_IMG");

      //specify connections
      Connect(GraphPort("SRC_IMG"), KernelPort("myRemap", "SRC"));
      Connect(GraphPort("LOCAL_OFFSET"), KernelPort("myRemap", "OFFSET"));
      Connect(GraphPort("DELTA"), KernelPort("myRemap", "DELTA"));
      Connect(KernelPort("myRemap", "DST"), KernelPort("mySobel", "SRC"));
      Connect(KernelPort("mySobel", "DST"), KernelPort("myMedian", "SRC"));
      Connect(KernelPort("myMedian", "DST"), GraphPort("DST_IMG"));
   }
};

#endif /* LDWSREMAPSOBELMEDIANGRAPH_HPP */