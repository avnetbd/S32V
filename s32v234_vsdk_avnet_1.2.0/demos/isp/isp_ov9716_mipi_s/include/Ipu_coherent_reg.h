/*****************************************************************************
*
* freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
* Copyright 2017-2018 NXP
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

#ifndef __MY_MIDDLEWARE_H__
#define __MY_MIDDLEWARE_H__

#include "typedefs.h"
#include "isp_seq.h"                    // set ISP registers

#ifdef __cplusplus
extern "C" {
#endif

// **************************************************************************
// constants
// **************************************************************************
#define MAX_COHERENT_REGS        SEQ_REG_WRITE_NUM

// **************************************************************************
// macro
// **************************************************************************

// **************************************************************************
// local functions
// **************************************************************************

//***************************************************************************
/** setRegCoherent
 *
 * Add a register to coherent register update
 *
 * \param  aIsIPUS  0: IPUV; 1: IPUS
 * \param  aEngNr   for IPUSXX or IPUVXX
 * \param  aDirect  0: engine peripheral register; 1: engine core register
 * \param  aIndex   offset of register
 * \param  aData    data to be written
 *
 ************************************************************************/
SEQ_LIB_RESULT setRegCoherent(uint16_t aIsIPUS,
                              uint16_t aEngNr,
                              uint16_t aDirect,
                              uint16_t aIndex,
                              uint32_t aData);



//***************************************************************************
/** setRegCoherentFlush
 *
 * Flush prepared register list. Will be updated in the IPUS at next
 * frame start
 *
 *
 ************************************************************************/
SEQ_LIB_RESULT setRegCoherentFlush(void);

//***************************************************************************
/** setRegCoherentClear
 *
 * sets mCoherentRegIx to 0;
 *
 *
 ************************************************************************/
SEQ_LIB_RESULT setRegCoherentClear(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif
