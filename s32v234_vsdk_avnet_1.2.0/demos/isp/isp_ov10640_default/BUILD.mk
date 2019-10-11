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
#    Basic OV10640 CSI functionality demo.
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

ARM_APP = isp_ov10640_default
ISP_GRAPH = ov10640_default
SEQ_MODE = static
SEQ_FW = $(SEQ_MODE)_$(ISP_GRAPH)

##############################################################################
# ARM_APP
##############################################################################

VPATH = ../src/lib

ARM_DEFS += -DVSDK_UMAT_USE_OPENCV

ARM_APP_SRCS += \
    main.cpp \
    exposure.cpp \
    yuv420torgb.cpp \
    mymemcpy.S \

ARM_DEFS += \
    -DOV9716_QUAD_GRAPH=$(OV9716_QUAD_GRAPH)

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
    $(SDK_ROOT)/isp/graphs/$(ISP_GRAPH)/$(SEQ_FW)/$(ODIR)/lib$(SEQ_FW).a

ARM_LDOPTS +=                                                             \
    -lopencv_core   

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
