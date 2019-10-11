/*****************************************************************************
*
* freescale Confidential Proprietary
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

#include <string.h>
#include <math.h>

#include "typedefs.h"               // uint*_t types
#include "sdi.hpp"                  // camera middle ware
#include "Ipu_coherent_reg.h"       // setRegCoherent
#include "global_defines.h"         // graph definitions
#include "ctl_tonemap.h"            // control functions for tonemapping


//***************************************************************************

uint8_t EqualizeHistogram(SEQ_IpuHist_t *apHist, uint16_t aLutOffset)
{
  uint32_t lTotal;
  uint32_t lCams;
  int32_t *lpData;
  uint32_t lNextSum;
  int32_t  lVal;
  float_t  lfValEq;
  float    lfValStretch;
  float_t  lfThisTarget;
  float_t  lfNextTarget;
  float_t  lfSlopeTarget;
  float_t  lfStretchSlope;
  uint32_t lHistLowCnt;
  uint32_t lHistHighCnt;
  uint16_t lHistLow;
  uint16_t lHistHigh;
  uint16_t lHistCnt;
  uint16_t lLutCnt;
  uint8_t  lError = LIB_FAILURE;
  uint8_t  lLutSegSize = OUTLUT_SIZE / 256;
  uint8_t  lSegStep;
  uint32_t lFilteredHist[256];
  uint16_t lpLutData[OUTLUT_SIZE];

  // get Histogram
  SEQ_HistogramGet(apHist);
  lpData = (int32_t*)apHist->mpData;

  lTotal = 0;
  for (lHistCnt = 0 ; lHistCnt < 256; lHistCnt++)
  {
    lTotal += lpData[lHistCnt];
  }

  // check if total is as expected
  if (lTotal !=  RGB2YUV_PIX * (RGB2YUV_LINES2_1)) {
    printf("Error in Compression Histogram total=%d (should be %d)\n",
           lTotal,
           RGB2YUV_PIX * RGB2YUV_LINES2_1);
    return lError;
  }

  // check cameras
  lCams = 4;
  while (lpData[0] >= (RGB2YUV_PIX * RGB2YUV_LINES2_1 >> 2))
  {
    lpData[0] -= (RGB2YUV_PIX * RGB2YUV_LINES2_1 >> 2);
    lTotal -= (RGB2YUV_PIX * RGB2YUV_LINES2_1 >> 2);
    lCams--;
  }
  if (lCams != CAMS)
  {
    return lError;
  }
  lHistLowCnt = lTotal / 100;
  lHistHighCnt = lTotal - lTotal / 100;

  lTotal = 0;
  lHistLow = 0;
  lHistHigh = 0;
  for (lHistCnt = 0 ; lHistCnt < 256; lHistCnt++)
  {
    lTotal += lpData[lHistCnt];
    if (lTotal < lHistLowCnt)
      lHistLow = lHistCnt;
    if (lTotal <= lHistHighCnt)
      lHistHigh = lHistCnt;
  }

  lHistLow *= lLutSegSize;
  lHistHigh *= lLutSegSize;
  lfStretchSlope = 1.0 / ((float)(lHistHigh - lHistLow + 1));

  { // cut peaks from Histogram
    uint32_t lThreshold;
    lTotal = lFilteredHist[0]  = lpData[0];
    lTotal += lFilteredHist[255] = lpData[255];
    for (lHistCnt = 1 ; lHistCnt < 255; lHistCnt++)
    {
      lTotal +=
        lFilteredHist[lHistCnt] =
          (lpData[lHistCnt - 1] +
           2 * lpData[lHistCnt] +
           lpData[lHistCnt + 1] +
           2) / 4;
    }

    lThreshold = lTotal / 50; // 2% max per bin

    for (lHistCnt = 0 ; lHistCnt < 256; lHistCnt++)
    {
      if (lFilteredHist[lHistCnt] > lThreshold)
      {
        lTotal -= lFilteredHist[lHistCnt] - lThreshold;
        lFilteredHist[lHistCnt] = lThreshold;
      }
    }
  } // cut peaks

  // **** Compute LUT from Histogram ****
  lNextSum = 0;
  lfThisTarget = lfNextTarget = 0.0;
  lLutCnt = 0;
  lHistCnt = 0;

  while (lLutCnt < OUTLUT_SIZE)
  {
    lfThisTarget = lfNextTarget;
    lNextSum    += lFilteredHist[lHistCnt++];

    lfNextTarget =
      ((float)lNextSum) / ((float)lTotal);

    lfSlopeTarget = (lfNextTarget - lfThisTarget) / (float)lLutSegSize;

    lSegStep = 0;
    while (lSegStep < lLutSegSize)
    {
      // 256 means factor of 1!
      if (lLutCnt)
      {
        // equalized
        lfValEq =
          powf(lfThisTarget + (float)lSegStep * lfSlopeTarget,
               0.5 /*arContext.mEqualGamma*/);

        // stretched
        if (lLutCnt > lHistLow)
        {
          if (lLutCnt >= lHistHigh)
          {
            lfValStretch = 1.0;
          }
          else
          {
            lfValStretch =
              powf(((float_t)(lLutCnt - lHistLow) * lfStretchSlope),
                   0.5 /* arContext.mStretchGamma*/);
          }
        }
        else
        {
          lfValStretch = 0.0;
        }

        // blend equalized and stretched
        lVal = (int32_t)((
                           lfValEq * (1.0 - 0.5/* arContext.mStretchAlpha*/) +
                           lfValStretch * 0.5/*arContext.mStretchAlpha*/) /
                         ((float)(lLutCnt << (16 - OUTLUT_BITS))) *
                         65535.0 * OUTLUT_SCALE);
      }
      else
      {
        lVal = 0;
      }


      // threshold value
      if (lVal > 0xffff)
      {
        lVal = 0xffff;
      }
      if (lVal < 0)
      {
        printf("Lut[%d]=%d\n", lLutCnt, lVal);
      }
      lpLutData[lLutCnt++] = lVal;
      lSegStep++;
    }
  } // while lutcnt < OUTLUT_SIZE

  // apply LUT updates
  if (ISP_LutSet(OUTLUT_IPUS0, aLutOffset, lpLutData, OUTLUT_SIZE)
      != SEQ_LIB_SUCCESS)
  {
    printf("Failed to update IPUS %d LUT\n", OUTLUT_IPUS0);
    return lError;
  }
  // lutoffset to gpr15
  setRegCoherent(OUTLUT_ENGINE0, 0, 0x7f, aLutOffset);
#if (OUTLUT_IPUS0 != OUTLUT_IPUS1)
  if (ISP_LutSet(OUTLUT_IPUS1, aLutOffset, lpLutData, OUTLUT_SIZE)
      != SEQ_LIB_SUCCESS)
  {
    printf("Failed to update IPUS %d LUT\n", OUTLUT_IPUS1);
    return lError;
  }
  setRegCoherent(OUTLUT_ENGINE1, 0, 0x7f, aLutOffset);
#endif
  lError = LIB_SUCCESS;
  return lError;
} // EqualizeHistogram()

//***************************************************************************
/* EOF */
