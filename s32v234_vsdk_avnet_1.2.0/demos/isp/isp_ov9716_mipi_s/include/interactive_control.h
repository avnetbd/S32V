/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016 NXP Semiconductors;
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

#ifndef __INTERACTIVE_CONTROL_H__
#define __INTERACTIVE_CONTROL_H__

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

// **************************************************************************
// constants
// **************************************************************************
#define CHGAIN_SETS 9
#define INLUT_SETS 2
#define OUTLUT_SETS 5
#define MAXLOG 8

// **************************************************************************
// global variables
// **************************************************************************

// **************************************************************************
// macro
// **************************************************************************

// **************************************************************************
// exported functions
// **************************************************************************
void KeyboardInputProcess(AppContext_t &arContext);
int32_t KbAction(AppContext_t &arContext);
void LogStatus(AppContext_t &arContext);

#ifndef __STANDALONE__
void TermBlockSet(void);
void TermNonBlockSet(void);
#endif // #ifndef __STANDALONE__


#ifdef __cplusplus
} // extern "C"
#endif

#endif // __INTERACTIVE_CONTROL_H__

/* EOF */
