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

#ifndef YUV420TORGB_H
#define YUV420TORGB_H

#include "typedefs.h"                   // where do we find uintXX_t ??

extern "C"
{

/************************************************************************/
/** Neon_Yuv420ToRgb  Converts the Y + UV buffer back to RGB
 *
 * \param  arRgb       pointer to interleaved RGB result buffer
 *                     in BGR sequence (little endian)
 * \param  arY         pointer to Y  input buffer
 * \param  arUv        pointer to UV input buffer in UV sequence
 * \param  arLength    no of Y pixels 
 * \param  arLogLEvel  print log level
 ************************************************************************/
void Neon_Yuv420ToRgb(unsigned char* arRgb,
		      unsigned char* arY,
		      unsigned char* arUv,
		      uint16_t arLength);

/************************************************************************/
/** A53_Yuv420ToRgb  Converts the Y + UV buffer back to RGB
 *
 * \param  arRgb       pointer to interleaved RGB result buffer
 *                     in BGR sequence (little endian)
 * \param  arY         pointer to Y  input buffer
 * \param  arUv        pointer to UV input buffer in UV sequence
 * \param  arLength    no of Y pixels 
 * \param  arLogLEvel  print log level
 ************************************************************************/
void A53_Yuv420ToRgb(unsigned char *arRgb,
		     unsigned char *arY,
		     unsigned char *arUv,
		     uint32_t arLength);

}
#endif // YUV420TORGB_H
