#
#  Copyright (c) 2016, Freescale Semiconductor, Inc.
#  Copyright (c) 2016-2017  NXP
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#  o Redistributions of source code must retain the above copyright notice, this
#  list of conditions and the following disclaimer.
#
#  o Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
#  o Neither the name of Freescale Semiconductor, Inc. nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.
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
#
#  CONTENT
#    OV9716 Camera ISP.
#
#  AUTHOR
#
#
#  DATE
#
#
#  LANGUAGE
#    make
#
##############################################################################

SDK_ROOT := $(call path_relative_to,$(CURDIR),$(CURR_SDK_ROOT))

ARM_APP = isp_ov9716_mipi_s
ISP_GRAPH = ov9716_mipi_s
SEQ_MODE = static
SEQ_FW = $(SEQ_MODE)_$(ISP_GRAPH)

# options for OV9716_QUAD_GRAPH are:
# <empty>  = set to 4 as default
# 1        = single camera (no sync)
# 2        = dual exposure mode only
# 3        = 3 outputs (no camera dump)
# 4        = full steam quad cameras
# MIPI     = for MIPI camera
OV9716_QUAD_GRAPH ?= MIPI
UDP264 ?= y

##############################################################################
# ARM_APP
##############################################################################

VPATH += ../lib/linux

ARM_DEFS += -DVSDK_UMAT_USE_OPENCV

ARM_APP_SRCS += \
    exposure.cpp \
    interactive_control.cpp \
    main.cpp \
    app_config.cpp \
    Ipu_coherent_reg.cpp                                       \
    mymemcpy.S                                               \
    yuv420torgb.cpp\
    ctl_tonemap.cpp\

ifeq ($(UDP264),y)
ARM_APP_SRCS +=                                                         \
    udp264.cpp
endif

ARM_DEFS += \
    -DOV9716_QUAD_GRAPH=$(OV9716_QUAD_GRAPH)

ifeq ($(UDP264),y)
ARM_DEFS += \
    -DUDP264=1
endif

ARM_INCS +=                                                               \
    -I../include                                                          \
    -I$(SDK_ROOT)/platform/s32_v234                                       \
    -I$(OPENCV_ROOT)/include                                              \
    -I$(SDK_ROOT)/include                                                 \
    -I$(SDK_ROOT)/libs/io/sdi/include                                     \
    -I$(SDK_ROOT)/libs/utils/common/include                               \
    -I$(SDK_ROOT)/libs/apex/common/include                                \
    -I$(SDK_ROOT)/isp/inc                                                 \
    -I$(SDK_ROOT)/libs/io/framebuffer/user/include/linux                  \
    -I$(SDK_ROOT)/libs/io/image_camera_io/include                         \
    -I$(SDK_ROOT)/libs/io/frame_io/include                                \
    -I$(SDK_ROOT)/isp/graphs/$(ISP_GRAPH)/inc                             \
    -I$(SDK_ROOT)/libs/io/dcu/include                                     \
    -I$(SDK_ROOT)/libs/isp/sequencer/kernel/include                       \
    -I$(SDK_ROOT)/3rdparty/iniparser4/src                                 \
    -I$(SDK_ROOT)/3rdparty/boost_1_62_0                                   \


ARM_APP_LIBS +=                                                           \
    $(SDK_ROOT)/libs/io/frame_io/$(ODIR)/libframe_io.a                    \
    $(SDK_ROOT)/libs/io/sdi/$(ODIR)/libsdi.a                              \
    $(SDK_ROOT)/libs/isp/cam_generic/user/$(ODIR)/libcamdrv.a             \
    $(SDK_ROOT)/libs/isp/csi/user/$(ODIR)/libcsidrv.a                     \
    $(SDK_ROOT)/libs/utils/log/$(ODIR)/liblog.a                           \
    $(SDK_ROOT)/libs/isp/sequencer/user/$(ODIR)/libseqdrv.a               \
    $(SDK_ROOT)/libs/isp/fdma/user/$(ODIR)/libfdmadrv.a                   \
    $(SDK_ROOT)/libs/utils/oal/user/$(ODIR)/liboal.a                      \
    $(SDK_ROOT)/libs/utils/umat/$(ODIR)/libumat.a                         \
    $(SDK_ROOT)/libs/utils/common/$(ODIR)/libcommon.a                     \
    $(SDK_ROOT)/3rdparty/iniparser4/$(ODIR)/libiniparser4.a               \
    $(SDK_ROOT)/isp/graphs/$(ISP_GRAPH)/$(SEQ_FW)/$(ODIR)/lib$(SEQ_FW).a  \
    $(SDK_ROOT)/libs/utils/h264encoder/$(ODIR)/h264encoder.a              \

ARM_LDOPTS +=                                                             \
    -lopencv_core                                                         \
    -L$(SDK_ROOT)/3rdparty/boost_1_62_0/stage/lib                         \
    -lboost_system                                                        \
    -lboost_thread

ifneq (,$(findstring gnu-sa,$(ODIR)))
  ARM_APP_LIBS +=                                                         \
    $(SDK_ROOT)/libs/io/i2c/$(ODIR)/libi2c.a                              \
    $(SDK_ROOT)/libs/io/dcu/$(ODIR)/libdcu.a                              \

endif

ifneq (,$(findstring gnu-linux,$(ODIR)))
  ARM_APP_LIBS +=                                                         \
    $(SDK_ROOT)/libs/isp/jpegdec/user/$(ODIR)/libjpegdecdrv.a             \
    $(SDK_ROOT)/libs/isp/h264enc/user/$(ODIR)/libh264encdrv.a             \
    $(SDK_ROOT)/libs/isp/h264dec/user/$(ODIR)/libh264decdrv.a             \
    $(SDK_ROOT)/libs/isp/viu/user/$(ODIR)/libviudrv.a                     \
		# ../lib/linux/ctl_tonemap.o\
    # ../lib/linux/Ipu_coherent_reg.o                                       \
    # ../lib/linux/mymemcpy.o                                               \
    # ../lib/linux/yuv420torgb.o                                            \

# ifeq ($(UDP264),y)
#   ARM_APP_LIBS +=                                                         \
#     ../lib/linux/udp264.o                                                 \

# endif

ARM_LDOPTS += \
    $(SDK_ROOT)/3rdparty/iniparser4/$(ODIR)/libiniparser4.a

endif

##############################################################################
# STANDALONE SPECIFIC INCLUDES
##############################################################################
ifneq (,$(findstring -sa,$(ODIR)))

ARM_APP_LIBS +=                                                              \
    $(SDK_ROOT)/libs/startup/v234ce_standalone/$(ODIR)/libv234ce.a           \
    $(SDK_ROOT)/libs/io/i2c/$(ODIR)/libi2c.a                                 \
    $(SDK_ROOT)/libs/io/semihost/$(ODIR)/libSemihost.a                       \
    $(SDK_ROOT)/libs/io/uartlinflex_io/$(ODIR)/liblinflex.a

ARM_LDOPTS +=                                                                \
    -lzlib

endif
