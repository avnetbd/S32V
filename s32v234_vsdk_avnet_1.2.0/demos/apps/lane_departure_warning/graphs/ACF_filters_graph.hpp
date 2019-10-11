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

#ifndef ACFFILTERSGRAPH_HPP
#define ACFFILTERSGRAPH_HPP

#include <acf_graph.hpp>
// Reference the kernels here
#include "image_filters_acf.h"
#include "remap_bilinear_acf.h"
#include "thresh_val_histogram_acf.h"
#include "threshold_acf.h"
#include "lane_detection_acf.h"

class ACF_filters_graph : public ACF_Graph
{
public:
   ACF_filters_graph() : ACF_Graph()
   {
      GET_KERNEL_HANDLE(&_CgvLdw2_remap_bilinear_greyscale, "CgvLdw2_remap_bilinear_grayscale");
      GET_KERNEL_HANDLE(&_kernel_info_sobel_3x3_08u, "sobel_3x3_08u");
	    GET_KERNEL_HANDLE(&_kernel_info_median_3x3_08u, "median_3x3_08u");
      GET_KERNEL_HANDLE(&_thresh_val_histogram, "thresh_val_histogram");
      GET_KERNEL_HANDLE(&_CgvLdw2_grayscale_threshold, "CgvLdw2_grayscale_threshold");
      GET_KERNEL_HANDLE(&_lane_detection, "lane_detection");
   }

   void Create()
   {
      //set identifier for graph
      SetIdentifier("ACF_FILTERS");

      //add kernels
      AddKernel("myRemap",  "CgvLdw2_remap_bilinear_grayscale"); // yuv once implemented
      AddKernel("mySobel",  "sobel_3x3_08u");
      AddKernel("myMedian", "median_3x3_08u");
      AddKernel("myHistogram", "thresh_val_histogram");
      AddKernel("myThreshold", "CgvLdw2_grayscale_threshold");
      AddKernel("myLaneDetection", "lane_detection");

      //add graph ports
      AddInputPort("SRC_IMG");
      AddInputPort("LOCAL_OFFSET");
      AddInputPort("DELTA");
      AddInputPort("THRESHOLD_PREVIOUS");
      AddInputPort("VOTE_THRESHOLD");
      AddOutputPort("DST_IMG");
      AddOutputPort("THRESHOLD");
      AddOutputPort("LANE_DATA");

      //specify connections
      Connect(GraphPort("SRC_IMG"), KernelPort("myRemap", "SRC"));
      Connect(GraphPort("LOCAL_OFFSET"), KernelPort("myRemap", "OFFSET"));
      Connect(GraphPort("DELTA"), KernelPort("myRemap", "DELTA"));
      Connect(GraphPort("THRESHOLD_PREVIOUS"), KernelPort("myThreshold", "THRESH"));
      Connect(KernelPort("myRemap", "DST"), KernelPort("myThreshold", "SRC"));
      Connect(KernelPort("myRemap", "DST"), KernelPort("myHistogram", "SRC"));
      Connect(KernelPort("myThreshold", "DST"), KernelPort("mySobel", "INPUT_0"));
      Connect(GraphPort("THRESHOLD_PREVIOUS"), KernelPort("myHistogram", "THRESH_IN"));
      Connect(GraphPort("VOTE_THRESHOLD"), KernelPort("myLaneDetection", "VOTE_THRESHOLD"));
      Connect(KernelPort("mySobel", "OUTPUT_0"), KernelPort("myMedian", "INPUT_0"));
      Connect(KernelPort("myMedian", "OUTPUT_0"), KernelPort("myLaneDetection", "SRC"));
      Connect(KernelPort("myLaneDetection", "POINTS_DATA"), GraphPort("LANE_DATA"));
      Connect(KernelPort("myHistogram", "THRESH_OUT"), GraphPort("THRESHOLD"));
      Connect(KernelPort("myMedian", "OUTPUT_0"), GraphPort("DST_IMG"));
   }
};

#endif /* ACFFILTERSGRAPH_HPP */

