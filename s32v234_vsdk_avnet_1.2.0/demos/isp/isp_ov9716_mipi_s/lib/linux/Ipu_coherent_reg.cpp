/*****************************************************************************
*
* freescale Confidential Proprietary
*
* Copyright (c) 2017 NXP Semiconductors;
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
#include "typedefs.h"                   // uint*_t types
#include "Ipu_coherent_reg.h"


//***************************************************************************
/* IPU registers with coherent update */
static SEQM_ipu_reg_t  gRegVal[MAX_COHERENT_REGS];
static uint16_t        gCoherentRegIx = 0;


//***************************************************************************

SEQ_LIB_RESULT setRegCoherent(uint16_t aIsIPUS,
                              uint16_t aEngNr,
                              uint16_t aDirect,
                              uint16_t aIndex,
                              uint32_t aData)
{
  SEQ_LIB_RESULT        err = SEQ_LIB_SUCCESS;
  uint32_t              addr= (uint32_t)(uintptr_t)&IPUS_0;

  if(gCoherentRegIx >= MAX_COHERENT_REGS)
  {
    return(SEQ_LIB_FAILURE);
  }

  if(aIsIPUS)
  {
    switch(aEngNr)
    {
    case 0:
      addr = (uint32_t)(uintptr_t)&IPUS_0;
      break;

    case 1:
      addr = (uint32_t)(uintptr_t)&IPUS_1;
      break;

    case 2:
      addr = (uint32_t)(uintptr_t)&IPUS_2;
      break;

    case 3:
      addr = (uint32_t)(uintptr_t)&IPUS_3;
      break;

    case 4:
      addr = (uint32_t)(uintptr_t)&IPUS_4;
      break;

    case 5:
      addr = (uint32_t)(uintptr_t)&IPUS_5;
      break;

    case 6:
      addr = (uint32_t)(uintptr_t)&IPUS_6;
      break;

    case 7:
      addr = (uint32_t)(uintptr_t)&IPUS_7;
      break;

    default:
      err = SEQ_LIB_FAILURE;
    }
  }
  else
  {
    switch(aEngNr)
    {
    case 0:
      addr = (uint32_t)(uintptr_t)&IPUV_0;
      break;

    case 1:
      addr = (uint32_t)(uintptr_t)&IPUV_1;
      break;

    case 2:
      addr = (uint32_t)(uintptr_t)&IPUV_2;
      break;

    case 3:
      addr = (uint32_t)(uintptr_t)&IPUV_3;
      break;

    default:
      err = SEQ_LIB_FAILURE;
    }
  }
  if (err == SEQ_LIB_FAILURE)
  {
    printf("Selected wrong IPU for resister updaten\n");
    return err;
  }
  else {
    addr -= SEQ_ADDR_OFF;

    gRegVal[gCoherentRegIx].mEngBase = addr;
    gRegVal[gCoherentRegIx].mIndex   = aIndex;
    gRegVal[gCoherentRegIx].mData    = aData;
    gRegVal[gCoherentRegIx].mDirect  = aDirect;

    gCoherentRegIx++;
  }

  return(err);

} // setRegCoherent()

//***************************************************************************

SEQ_LIB_RESULT setRegCoherentFlush(void)
{
  SEQ_LIB_RESULT        err = SEQ_LIB_SUCCESS;
  SEQ_RegList_t         lRegList;

  lRegList.mCnt       = gCoherentRegIx;
  lRegList.mDirection = IPU_REGLIST_WRITE;
  lRegList.mpData     = gRegVal;

  err = SEQ_RegListSet(&lRegList);

  if((err == SEQ_LIB_SUCCESS) /*&& (lRegList.mCnt != gCoherentRegIx)*/)
  {
    gCoherentRegIx = 0;
  } // is reg list set successfully
  if (gCoherentRegIx) {
    printf("*");
  }

  return(err);
} // setRegCoherentFlush()

//***************************************************************************

SEQ_LIB_RESULT setRegCoherentClear(void)
{
  gCoherentRegIx=0;
  printf("Buffer size for coherent registers: %d values\n",MAX_COHERENT_REGS);
  return SEQ_LIB_SUCCESS;
}
