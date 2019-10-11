##############################################################################
#
# Freescale Confidential Proprietary
#
# Copyright (c) 2016 Freescale Semiconductor;
# All Rights Reserved
#
##############################################################################
#
# THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
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
    $(KERNELS)/sample_arithmetic_kernels/$(ODIR)/arithmetic_kernels.a               \
    $(KERNELS)/sample_filtering_kernels/$(ODIR)/filtering_kernels.a                 \
    $(KERNELS)/sample_statistics_kernels/$(ODIR)/statistics_kernels.a               \
    $(KERNELS)/sample_geometry_kernels/$(ODIR)/geometry_kernels.a

APU_GRAPH_INCS =                                                             \
    -I..                                                                     \
    -I$(SDK_ROOT)/include                                                    \
	-I$(SDK_ROOT)/libs/utils/umat/include                                    \
	-I$(SDK_ROOT)/libs/utils/oal/user/include                                \
	-I$(SDK_ROOT)/libs/utils/oal/kernel/include                              \
    -I$(KERNELS)/sample_arithmetic_kernels/src                                      \
    -I$(KERNELS)/sample_filtering_kernels/src                                       \
    -I$(KERNELS)/sample_statistics_kernels/src                                      \
    -I$(KERNELS)/sample_geometry_kernels/src

APU_GRAPHS =                                                                 \
    ../ldws_remap_sobel_median_apu_process_desc.hpp                          \
    ../history_maximum_apu_process_desc.hpp                                  \
    ../analysis_apu_process_desc.hpp                                         \
    