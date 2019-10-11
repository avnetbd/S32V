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
#ifndef ACFFILTERSNOREGGRAPH_HPP
#define ACFFILTERSNOREGGRAPH_HPP

#include <acf_graph.hpp>
// Reference the kernels here
#include "image_filters_acf.h"
#include "remap_bilinear_acf.h"
#include "thresh_val_histogram_acf.h"
#include "threshold_acf.h"
#include "lane_detection_acf.h"

class ACF_filters_no_reg_graph : public ACF_Graph
{
public:
   ACF_filters_no_reg_graph() : ACF_Graph()
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
      // Set identifier for graph
      SetIdentifier("ACF_FILTERS_NO_REG");

      // Add kernels
      AddKernel("myRemap",  "CgvLdw2_remap_bilinear_grayscale"); // yuv once implemented
      AddKernel("mySobel",  "sobel_3x3_08u");
      AddKernel("myMedian", "median_3x3_08u");
      AddKernel("myHistogram", "thresh_val_histogram");
      AddKernel("myThreshold", "CgvLdw2_grayscale_threshold");
      AddKernel("myLaneDetection", "lane_detection");

      // Add graph input ports
      AddInputPort("SRC_IMG");
      AddInputPort("LOCAL_OFFSET");
      AddInputPort("DELTA");
      AddInputPort("THRESHOLD_PREVIOUS");
      AddInputPort("VOTE_THRESHOLD");

      // Add graph output ports
      AddOutputPort("DST_IMG");    // out: median
      AddOutputPort("THRESHOLD");  // out: histogram
      AddOutputPort("MAX_LEFT");   // out: lane_detection
      AddOutputPort("MAX_RIGHT");  // out: lane_detection
      AddOutputPort("POINTS_DATA");// out: lane_detection

      // Specify connections
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
      Connect(KernelPort("myLaneDetection", "POINTS_DATA"), GraphPort("POINTS_DATA"));   // out: lane detection
      Connect(KernelPort("myLaneDetection", "MAX_LEFT"), GraphPort("MAX_LEFT"));         // out: lane_detection
      Connect(KernelPort("myLaneDetection", "MAX_RIGHT"), GraphPort("MAX_RIGHT"));       // out: lane_detection
      Connect(KernelPort("myHistogram", "THRESH_OUT"), GraphPort("THRESHOLD"));          // out: threshold
      Connect(KernelPort("myMedian", "OUTPUT_0"), GraphPort("DST_IMG"));                 // out: median
   }
};

#endif /* ACFFILTERSNOREGGRAPH_HPP */

