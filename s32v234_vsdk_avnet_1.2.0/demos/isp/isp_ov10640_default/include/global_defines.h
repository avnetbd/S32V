/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright (c) 2016-2017  NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GLOBALDEFINES_H
#define GLOBALDEFINES_H

#ifdef __STANDALONE__
#include "frame_output_dcu.h"         // display controller in SA
#include "uartlinflex_driver.h"       // for keyboard input
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"      // display controller for Linux
#endif // else from #ifdef __STANDALONE__

#include "iofiles.h"


// Possible to set input resolution (must be supported by the DCU)
#define SCR_WIDTH  ((DISPLAY_PIXELS+31) & ~31)  ///< width of DDR buffer in pixels
#define SCR_HEIGHT (DISPLAY_LINES)     ///< height of DDR buffer in pixels

#if (ISP_OUTPUT == ISP_OUTPUT_RGBY)
#define EXTRACT_MEMCOPY
#define DCU_OUT_FILE_EXT "rgby"
#define ISP_CHNL_CNT io::IO_DATA_CH4
#define DCU_TYPE DCU_BPP_32
#define DCU_CHNL_CNT io::IO_DATA_CH4
#elif (ISP_OUTPUT == ISP_OUTPUT_RGB)
#define EXTRACT_MEMCOPY
#define DCU_OUT_FILE_EXT "ppm"
#define ISP_CHNL_CNT io::IO_DATA_CH3
#define DCU_TYPE DCU_BPP_24
#define DCU_CHNL_CNT io::IO_DATA_CH3
#elif (ISP_OUTPUT == ISP_OUTPUT_YUV422)
#define EXTRACT_MEMCOPY
#define DCU_OUT_FILE_EXT "uyvy"
#define ISP_CHNL_CNT io::IO_DATA_CH2
#define DCU_TYPE DCU_BPP_YCbCr422
#define DCU_CHNL_CNT io::IO_DATA_CH2
#elif (ISP_OUTPUT == ISP_OUTPUT_YUV420)
#define EXTRACT_YUV2RGB
#define DCU_OUT_FILE_EXT "ppm"
#define ISP_CHNL_CNT io::IO_DATA_CH1
#define DCU_TYPE DCU_BPP_82
#define DCU_CHNL_CNT io::IO_DATA_CH3
#endif

#define DDR_OUT_BUFFER_CNT 3   ///< number of DDR buffers per ISP stream

#endif
