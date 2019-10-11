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

##############################################################################
# ARM_APP
##############################################################################
ARM_APP = apex_add

ARM_DEFS += -DVSDK_UMAT_USE_OPENCV

ARM_APP_SRCS +=                                                                  \
    main.cpp                                                                 \

ARM_INCS +=                                                                  \
    -I$(SDK_ROOT)/libs/apex/icp/include                                      \
    -I$(SDK_ROOT)/libs/apex/drivers/user/include                             \
    -I$(SDK_ROOT)/include                                                    \
	-I$(SDK_ROOT)/libs/utils/umat/include                                    \
	-I$(SDK_ROOT)/libs/utils/oal/user/include                                \
	-I$(SDK_ROOT)/libs/utils/oal/kernel/include                              \
    -I$(OPENCV_ROOT)/include                                                 \
    -I../graphs                                                              \
    -I../graphs/build-apu-$(APU_COMP)-sa-d

ARM_APP_LIBS =                                                               \
    $(SDK_ROOT)/libs/apex/acf/$(ODIR)/libacf.a                               \
    $(SDK_ROOT)/libs/apex/icp/$(ODIR)/libicp.a                               \
    $(SDK_ROOT)/libs/utils/common/$(ODIR)/libcommon.a                        \
    $(SDK_ROOT)/libs/utils/umat/$(ODIR)/libumat.a                            \
    $(SDK_ROOT)/libs/utils/umat_helper/$(ODIR)/libumat_helper.a              \
    $(SDK_ROOT)/libs/apex/drivers/user/$(ODIR)/libapexdrv.a                  \
    $(SDK_ROOT)/libs/utils/oal/user/$(ODIR)/liboal.a                         \
    $(SDK_ROOT)/libs/utils/communications/$(ODIR)/lib_communications.a       \
    $(SDK_ROOT)/libs/utils/log/$(ODIR)/liblog.a

ARM_LDOPTS +=                                                                \
    -lopencv_core 
    
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
    
##############################################################################
# INTEGRITY SPECIFIC INCLUDES
##############################################################################	
else
ifneq (,$(findstring -integrity,$(ODIR)))

ARM_LDOPTS +=                                                                \
    -lposix                                                                  \
    -livfs                                                                   \
    -lIlmImf                                                                 \
    -lzlib                                                                   \
    --exceptions                                                             \
     
##############################################################################
# LINUX SPECIFIC INCLUDES
##############################################################################			
else

ARM_LDOPTS +=                                                                \
    
endif
endif


