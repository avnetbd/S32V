cmake_minimum_required (VERSION 2.6)

set (EMU_LIBS 		"${VISION_SDK}/tools/emu")
set (APU_LIB_DIR    "${EMU_LIBS}/apu")
set (ACF_LIB_DIR   	"${EMU_LIBS}/acf")
set (GENERAL_LIBS 	"${VISION_SDK}/libs")
set (ARM_LIBS 		"${GENERAL_LIBS}/")
set (ARM_COMMON_LIB	"${ARM_LIBS}/utils/common")
set (ARM_Img_Vid_LIB "${ARM_LIBS}/io/image_video_io")
set (EXT_KERNEL_DIR	"${VISION_SDK}/kernels/apu")
#set (ARM_OAL_LIB	"${ARM_LIBS}/utils/oal")
