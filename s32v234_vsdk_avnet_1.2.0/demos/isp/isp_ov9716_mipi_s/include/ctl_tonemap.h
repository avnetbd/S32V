/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2017-2018 NXP Semiconductors;
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

#ifndef __CTL_TONEMAP_H__
#define __CTL_TONEMAP_H__
#include "typedefs.h"
#include "isp_seq.h"                    // histogram types

#ifdef __cplusplus
extern "C" {
#endif

// **************************************************************************
// constants
// **************************************************************************

// **************************************************************************
// global variables
// **************************************************************************


// **************************************************************************
// local functions
// **************************************************************************

//***********************************************************************
/** call histogram equalization: read histogram and create new OutLut
 *
 * \param  apHist        pointer to IPU histogram
 * \param  aLutOffset    lower table: 0 or upper table: 2048
 * \param  aLogLevel     5: save/print histograms and LUT
 *
 * \return 0 if all ok -1 otherwise
 **********************************************************************/

uint8_t EqualizeHistogram(SEQ_IpuHist_t *apHist,uint16_t aLutOffset/*,
                          uint8_t aLogLevel*/);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __CTL_TONEMAP_H__

//***************************************************************************
/* EOF */
