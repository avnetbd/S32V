##############################################################################
#
# NXP Confidential Proprietary
#
# Copyright (c) 2016 Freescale Semiconductor;
# Copyright 2017 NXP;
# All Rights Reserved
#
##############################################################################
#
# THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
##############################################################################

SDK_ROOT := $(call path_relative_to,$(CURDIR),$(CURR_SDK_ROOT))
KERNELS  := $(SDK_ROOT)/kernels/apu

##############################################################################
# acf kernels and graphs
##############################################################################
APU_GRAPH_LIBS =                                                             \
   $(KERNELS)/apexcv_base_image_filters/$(ODIR)/apexcv_base_image_filters.a  \
   $(KERNELS)/apexcv_pro_remap/$(ODIR)/apexcv_pro_remap.a                    \
   $(KERNELS)/apexcv_pro_hough/$(ODIR)/apexcv_pro_hough.a                    \
   $(KERNELS)/apexcv_gdc_ldw2/$(ODIR)/apexcv_gdc_ldw2.a
   
APU_GRAPH_INCS =                                                             \
   -I..                                                                      \
   -I$(SDK_ROOT)/include                                                     \
   -I$(SDK_ROOT)/libs/utils/common/include                                   \
   -I$(SDK_ROOT)/libs/apex/icp/include                                       \
   -I$(SDK_ROOT)/kernels/apu/apexcv_pro_remap/src                            \
   -I$(SDK_ROOT)/kernels/apu/apexcv_pro_hough/src                            \
   -I$(SDK_ROOT)/kernels/apu/apexcv_base_image_filters/src                   \
   -I$(SDK_ROOT)/kernels/apu/apexcv_gdc_ldw2/src                             \

APU_GRAPHS =                                                                 \
   ../ACF_filters_apu_process_desc.hpp                                       \
   ../ACF_filters_no_reg_apu_process_desc.hpp
