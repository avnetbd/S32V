/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016-2018 NXP Semiconductors;
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

#ifndef __YUV420TORGB_H__
#define __YUV420TORGB_H__

#include "typedefs.h"

extern "C"
{
/************************************************************************/
/** Neon_Yuv420ToRgb  Converts the Y + UV buffer back to RGB
 *
 * \param  arRgb       pointer to interleaved RGB result buffer
 *                     in BGR sequence (little endian)
 * \param  arY         pointer to Y  input buffer 8bpp
 * \param  arUv        pointer to UV input buffer in UV sequence 8bpp
 * \param  arLength    no of Y pixels
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
 * \param  arY         pointer to Y  input buffer 8bpp
 * \param  arUv        pointer to UV input buffer in UV sequence 8bpp
 * \param  arLength    no of Y pixels
 ************************************************************************/
void A53_Yuv420ToRgb(unsigned char *arRgb,
                     unsigned char *arY,
                     unsigned char *arUv,
                     uint32_t arLength);

}
#endif // __YUV420TORGB_H__
