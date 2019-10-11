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

ARM_APP = gdc_cnn

ISP_GRAPH = mipi_simple
SEQ_MODE = dynamic
SEQ_FW = $(SEQ_MODE)_$(ISP_GRAPH)
##############################################################################
# ARM_APP
##############################################################################

VPATH = ../src/lib

ARM_DEFS += -DVSDK_UMAT_USE_OPENCV

ARM_APP_SRCS +=                                                               \
    gdc_cnn_main.cpp                                                          \
    gdc_cnn_app.cpp                                                           \
    gdc_cnn_task_classifier.cpp                                               \
    gdc_cnn_task_composition.cpp                                              \
    gdc_cnn_task_display.cpp                                                  \
    gdc_cnn_task_file_reader.cpp                                              \
    gdc_cnn_task_image_sensor.cpp                                             \
    gdc_cnn_task_ip_camera.cpp                                                \
    gdc_cnn_task_resize.cpp                                                   \
    gdc_cnn_task_stdin.cpp                                                    \
    gdc_cnn_types.cpp                                                         \
    gdc_cnn_types_composition.cpp                                             \
    gdc_cnn_types_display.cpp


ARM_INCS +=                                                                  \
    -I../include                                                             \
    -I$(OPENCV_ROOT)/include                                                 \
    -I$(SDK_ROOT)/include                                                    \
	-I$(SDK_ROOT)/libs/utils/umat/include                                    \
	-I$(SDK_ROOT)/libs/utils/oal/user/include                                \
	-I$(SDK_ROOT)/libs/utils/oal/kernel/include                              \
    -I$(SDK_ROOT)/libs/io/frame_io/include                                   \
    -I$(SDK_ROOT)/libs/utils/communications/src                              \
    -I$(SDK_ROOT)/platform/s32_v234                                          \
    -I$(SDK_ROOT)/isp/inc                                                    \
    -I$(SDK_ROOT)/libs/dnn/squeeze_net_v10/include         \
    -I$(SDK_ROOT)/libs/dnn/squeeze_net_v10/graphs/build-apu-$(APU_COMP)-sa-d \
    -I$(SDK_ROOT)/libs/io/dcu/include                                        \
    -I$(SDK_ROOT)/libs/utils/communications/include                          \
    -I$(SDK_ROOT)/3rdparty/iniparser4/src                                    \
    -I$(SDK_ROOT)/libs/io/semihost/include                                   \
    -I$(SDK_ROOT)/isp/graphs/$(ISP_GRAPH)/inc                                \


ARM_APP_LIBS +=                                                              \
    $(SDK_ROOT)/3rdparty/iniparser4/$(ODIR)/libiniparser4.a                  \
    $(SDK_ROOT)/isp/graphs/$(ISP_GRAPH)/$(SEQ_FW)/$(ODIR)/lib$(SEQ_FW).a      \
    $(SDK_ROOT)/libs/apex/acf/$(ODIR)/libacf.a                               \
    $(SDK_ROOT)/libs/apex/drivers/user/$(ODIR)/libapexdrv.a                  \
    $(SDK_ROOT)/libs/apex/icp/$(ODIR)/libicp.a                               \
    $(SDK_ROOT)/libs/dnn/apexdnn_sn/$(ODIR)/apexdnn_sn.a                     \
    $(SDK_ROOT)/libs/dnn/squeeze_net_v10/$(ODIR)/dnn_squeeze_net_v10.a      \
    $(SDK_ROOT)/libs/dnn/squeeze_net_v11/$(ODIR)/dnn_squeeze_net_v11.a      \
    $(SDK_ROOT)/libs/io/frame_io/$(ODIR)/libframe_io.a                       \
    $(SDK_ROOT)/libs/io/gdi/$(ODIR)/libgdi.a                                 \
    $(SDK_ROOT)/libs/io/sdi/$(ODIR)/libsdi.a                                 \
    $(SDK_ROOT)/libs/isp/cam_generic/user/$(ODIR)/libcamdrv.a                \
    $(SDK_ROOT)/libs/isp/csi/user/$(ODIR)/libcsidrv.a                        \
    $(SDK_ROOT)/libs/isp/fdma/user/$(ODIR)/libfdmadrv.a                      \
    $(SDK_ROOT)/libs/isp/sequencer/user/$(ODIR)/libseqdrv.a                  \
    $(SDK_ROOT)/libs/utils/common/$(ODIR)/libcommon.a                        \
    $(SDK_ROOT)/libs/utils/umat/$(ODIR)/libumat.a                            \
    $(SDK_ROOT)/libs/utils/communications/$(ODIR)/lib_communications.a       \
    $(SDK_ROOT)/libs/utils/log/$(ODIR)/liblog.a                              \
    $(SDK_ROOT)/libs/utils/oal/user/$(ODIR)/liboal.a                         \

ARM_LDOPTS +=                                                                \
    -lopencv_imgcodecs                                                       \
    -lopencv_imgproc                                                         \
    -lopencv_core                                                            \
    -lopencv_videoio                                                         \
    -lavcodec                                                                \
    -lavformat                                                               \
    -lavutil                                                                 \
    -lswscale                                                                \
    -lswresample
    

##############################################################################
# STANDALONE SPECIFIC INCLUDES
##############################################################################

ifneq (,$(findstring gnu-sa,$(ODIR))) 

ARM_APP_LIBS +=                                                              \

endif

##############################################################################
# LINUX SPECIFIC INCLUDES
##############################################################################

ifneq (,$(findstring gnu-linux,$(ODIR))) 

ARM_APP_LIBS +=                                                              \
    $(SDK_ROOT)/libs/isp/jpegdec/user/$(ODIR)/libjpegdecdrv.a                \
    $(SDK_ROOT)/libs/isp/h264enc/user/$(ODIR)/libh264encdrv.a                \
    $(SDK_ROOT)/libs/isp/h264dec/user/$(ODIR)/libh264decdrv.a                \
    $(SDK_ROOT)/libs/isp/viu/user/$(ODIR)/libviudrv.a                        \

endif
