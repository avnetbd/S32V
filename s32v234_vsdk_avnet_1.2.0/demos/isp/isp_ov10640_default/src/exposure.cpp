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

/**
 * \file    
 * \brief   Omnivision 10640 MIPI/CSI camera exposure control functions
 * \author  Marie-Anne Le Menn
 * \version 0.1
 * \date    28-July-2016
 ****************************************************************************/

#include "s32vs234.h"
#include "vdb_log.h"

#include "isp_cam_ov10640.h"
#include "../include/exposure.h"

uint16_t gGroupRegisters = 0; // 0= off; 0x8000 = on; all others invalid!!!

uint8_t gExpMode     = 1;            // select exposure mode 0:dual, 1:triple
uint8_t gHDRShift[4] = {4, 0, 0, 4}; // 0 for dual, 4 for friple
uint8_t gHDRShift1   = 4;            // shadow for gHDRShift
// uint8_t  gOutLut   = 2;    // 0/1 for dual, 2 for triple
float    gFlickerPeriod   = 234.8; // in lines
uint8_t  gDeflickerEnable = 0;     // on / off
uint16_t gExpRange        = 750;   // default: read from VTSs are controlled

uint32_t hist_VS[4][16]; // only bin 0 is 24b all the others are 16bits
uint32_t hist_S[4][16];
uint32_t hist_L[4][16];      // only bin 0 is 24b all the others are 16bits
uint32_t hist_pix_cnt[3][4]; // 3 exposure and 4 channels
uint32_t sum_hist[3][4];     // Sum of the histograms for each exposure/channel

// VS exposure initial parameters
uint16_t gVSExpTime    = OV10640_EXPOSURE_TIME_VS_MIN;
uint16_t gVSMinExpTime = OV10640_EXPOSURE_TIME_VS_MIN;
uint16_t gVSMaxExpTime = OV10640_EXPOSURE_TIME_VS_MAX;
uint8_t  gVSCG         = 0;
uint8_t  gVSCGEn       = 1;
uint8_t  gVSAG         = OV10640_ANA_GAIN_VS_MIN;
uint8_t  gVSMinAG      = OV10640_ANA_GAIN_VS_MIN;
uint8_t  gVSMaxAG      = OV10640_ANA_GAIN_VS_MAX;
uint16_t gVSDG         = OV10640_DIGITAL_GAIN_VS_MIN;
uint16_t gVSMinDG      = OV10640_DIGITAL_GAIN_VS_MIN;
uint16_t gVSMaxDG      = OV10640_DIGITAL_GAIN_VS_MAX;
// S exposure initial parameters
uint16_t gSExpTime    = OV10640_EXPOSURE_TIME_S_MIN;
uint16_t gSMinExpTime = OV10640_EXPOSURE_TIME_S_MIN;
uint16_t gSMaxExpTime = OV10640_EXPOSURE_TIME_L_MAX;
uint8_t  gSCG         = 0;
uint8_t  gSCGEn       = 0; //!!!!!!!!!! There is no conversion gain for VS in OV10640
uint8_t  gSAG         = OV10640_ANA_GAIN_S_MIN;
uint8_t  gSMinAG      = OV10640_ANA_GAIN_S_MIN;
uint8_t  gSMaxAG      = OV10640_ANA_GAIN_S_MAX;
uint16_t gSDG         = OV10640_DIGITAL_GAIN_S_MIN;
uint16_t gSMinDG      = OV10640_DIGITAL_GAIN_S_MIN;
uint16_t gSMaxDG      = OV10640_DIGITAL_GAIN_S_MAX;
// L exposure initial parameters
uint16_t gLExpTime    = OV10640_EXPOSURE_TIME_L_MIN;
uint16_t gLMinExpTime = OV10640_EXPOSURE_TIME_L_MIN;
uint16_t gLMaxExpTime = OV10640_EXPOSURE_TIME_L_MAX;
uint8_t  gLCG         = 0;
uint8_t  gLCGEn       = 1;
uint8_t  gLAG         = OV10640_ANA_GAIN_L_MIN;
uint8_t  gLMinAG      = OV10640_ANA_GAIN_L_MIN;
uint8_t  gLMaxAG      = OV10640_ANA_GAIN_L_MAX;
uint16_t gLDG         = OV10640_DIGITAL_GAIN_L_MIN;
uint16_t gLMinDG      = OV10640_DIGITAL_GAIN_L_MIN;
uint16_t gLMaxDG      = OV10640_DIGITAL_GAIN_L_MAX;

float gRatioLSExp = 0.3; // If 0 then the ratio is computed based on initial parameters of L and S
                         // If different than 0 then initial exposure parameters of S are overwrtitten
                         // based on L exposure and this ratio
float gRatioMargin = 0.95;
float gCGThreshold = 1.5; // Has to be >1.0 to work
float gAGThreshold = 1.3; // Has to be >1.0 to work

static uint8_t gHDREngine = 7; //IPUS7 unless set differently

// cam0-3
static double gChR[4] = {400.0, 400.0, 400.0, 400.0};
static double gChG[4] = {256.0, 256.0, 256.0, 256.0};
static double gChB[4] = {400.0, 400.0, 400.0, 400.0};

uint16_t gRedTarget  = 256; // red  target value in AWB
uint16_t gBlueTarget = 256; // blue target value in AWB

uint32_t Cadc[4] = {126, 312, 560, 156}; // See ADC range chapter

int32_t analogMinGain(uint32_t blackLevel, uint32_t cBLC, uint8_t analog_gain)
{
}

OV10640_Exposure_t gOVExposure[4];
OV10640_Exposure_t gOVExpPrint;

// **************************************************************************
// macro
// **************************************************************************
#define UPDATEFACTOR (0.5)

// **************************************************************************
// local functions
// **************************************************************************

// **************************************************************************
extern "C" void SetHDREngine(uint8_t arHDREngine)
{
  gHDREngine = arHDREngine;
}

// **************************************************************************
void OV10640_ExposureInit(float arRatioLS)
{
  float lCG = 1;
  //   float lExpHCGSat;
  for(int i = 0; i < 4; i++)
  {
    // VS exposure
    gOVExposure[i].mExposureTime[VS]         = gVSExpTime;
    gOVExposure[i].mMinExposureTime[VS]      = gVSMinExpTime;
    gOVExposure[i].mMaxExposureTime[VS]      = gVSMaxExpTime;
    gOVExposure[i].mConversionGain[VS]       = gVSCG;
    gOVExposure[i].mConversionGainEnable[VS] = gVSCGEn;
    gOVExposure[i].mAnalogGain[VS]           = gVSAG;
    gOVExposure[i].mMinAnalogGain[VS]        = gVSMinAG;
    gOVExposure[i].mMaxAnalogGain[VS]        = gVSMaxAG;
    gOVExposure[i].mDigitalGain[VS]          = gVSDG;
    gOVExposure[i].mMinDigitalGain[VS]       = gVSMinDG;
    gOVExposure[i].mMaxDigitalGain[VS]       = gVSMaxDG;

    if(gVSCGEn)
    {
      lCG = OV10640_CONVERSION_GAIN_FACTOR;
    }
    else
    {
      lCG = 1;
    }
    gOVExposure[i].mMinExposure[VS] = ((float)(gOVExposure[i].mMinExposureTime[VS]) / 32) * //8.5b
                                      (float)(1 << gOVExposure[i].mMinAnalogGain[VS]) *
                                      ((float)(gOVExposure[i].mMinDigitalGain[VS]) / 256); // 6.8b value

    gOVExposure[i].mMaxExposure[VS] = ((float)(gOVExposure[i].mMaxExposureTime[VS]) / 32) * //8.5b
                                      (float)(1 << gOVExposure[i].mMaxAnalogGain[VS]) * lCG *
                                      ((float)(gOVExposure[i].mMaxDigitalGain[VS]) / 256); // 6.8b value

    // S exposure
    gOVExposure[i].mExposureTime[S]         = gSExpTime;
    gOVExposure[i].mMinExposureTime[S]      = gSMinExpTime;
    gOVExposure[i].mMaxExposureTime[S]      = gSMaxExpTime;
    gOVExposure[i].mConversionGain[S]       = gSCG;
    gOVExposure[i].mConversionGainEnable[S] = gSCGEn;
    gOVExposure[i].mAnalogGain[S]           = gSAG;
    gOVExposure[i].mMinAnalogGain[S]        = gSMinAG;
    gOVExposure[i].mMaxAnalogGain[S]        = gSMaxAG;
    gOVExposure[i].mDigitalGain[S]          = gSDG;
    gOVExposure[i].mMinDigitalGain[S]       = gSMinDG;
    gOVExposure[i].mMaxDigitalGain[S]       = gSMaxDG;

    if(gSCGEn)
    {
      lCG = OV10640_CONVERSION_GAIN_FACTOR;
    }
    else
    {
      lCG = 1;
    }
    gOVExposure[i].mMinExposure[S] = (float)(gOVExposure[i].mMinExposureTime[S]) *
                                     (float)(1 << gOVExposure[i].mMinAnalogGain[S]) *
                                     ((float)(gOVExposure[i].mMinDigitalGain[S]) / 256); // 6.8b value

    gOVExposure[i].mMaxExposure[S] = (float)(gOVExposure[i].mMaxExposureTime[S]) *
                                     (float)(1 << gOVExposure[i].mMaxAnalogGain[S]) * lCG *
                                     ((float)(gOVExposure[i].mMaxDigitalGain[S]) / 256); // 6.8b value

    // L exposure
    gOVExposure[i].mExposureTime[L]         = gLExpTime;
    gOVExposure[i].mMinExposureTime[L]      = gLMinExpTime;
    gOVExposure[i].mMaxExposureTime[L]      = gLMaxExpTime;
    gOVExposure[i].mConversionGain[L]       = gLCG;
    gOVExposure[i].mConversionGainEnable[L] = gLCGEn;
    gOVExposure[i].mAnalogGain[L]           = gLAG;
    gOVExposure[i].mMinAnalogGain[L]        = gLMinAG;
    gOVExposure[i].mMaxAnalogGain[L]        = gLMaxAG;
    gOVExposure[i].mDigitalGain[L]          = gLDG;
    gOVExposure[i].mMinDigitalGain[L]       = gLMinDG;
    gOVExposure[i].mMaxDigitalGain[L]       = gLMaxDG;

    if(gLCGEn)
    {
      lCG = OV10640_CONVERSION_GAIN_FACTOR;
    }
    else
    {
      lCG = 1;
    }
    gOVExposure[i].mMinExposure[L] = (float)(gOVExposure[i].mMinExposureTime[L]) *
                                     (float)(1 << gOVExposure[i].mMinAnalogGain[L]) *
                                     ((float)(gOVExposure[i].mMinDigitalGain[L]) / 256); // 6.8b value

    gOVExposure[i].mMaxExposure[L] = (float)(gOVExposure[i].mMaxExposureTime[L]) *
                                     (float)(1 << gOVExposure[i].mMaxAnalogGain[L]) * lCG *
                                     ((float)(gOVExposure[i].mMaxDigitalGain[L]) / 256); // 6.8b value

    gOVExposure[i].mDeflickeringEnable = 0; // Disable by default -
        // Can only be enabled by calling EnableDeflicker
    gOVExposure[i].mAECFactor = UPDATEFACTOR;

    // Compute total configured exposures
    OV10640_ExposureTotal(gOVExposure[i], ExpTime_VShort);
    OV10640_ExposureTotal(gOVExposure[i], ExpTime_Short);
    OV10640_ExposureTotal(gOVExposure[i], ExpTime_Long);

    gOVExposure[i].mRatioLSExp  = (float)(gOVExposure[i].mExposure[L]) / (float)(gOVExposure[i].mExposure[S]);
    gOVExposure[i].mRatioLVSExp = (float)(gOVExposure[i].mExposure[L]) / (float)(gOVExposure[i].mExposure[VS]);
    //     gOVExposure[i].mRatioPositionS = (float)(gOVExposure[i].mExposure[L])/(float)(gOVExposure[i].mExposure[S]);
    gOVExposure[i].mRatioMargin = gRatioMargin;

    if(arRatioLS != 0) // Ignore init parameters of S and compute them based on ratio
                       // Might also change L exposure parameters in case of saturation
    {
      ExposureLSRatioChange(arRatioLS, 6);
    }

    OV10640_ExposureAllExpSet(gOVExposure[i], ExpTime_VShort);
    OV10640_ExposureAllExpSet(gOVExposure[i], ExpTime_Short);
    OV10640_ExposureAllExpSet(gOVExposure[i], ExpTime_Long);
  }
  gRatioLSExp = gOVExposure[0].mRatioLSExp;
}

// **************************************************************************

int ExposureLSRatioChange(float arNewRatio, uint8_t arLogLevel)
{
  gOVExposure[0].mRatioLSExp = arNewRatio;
  OV10640_ExposureComputationInd(gOVExposure[0], gOVExposure[0].mExposure[L], gOVExposure[0].mExposure[VS]);
}

//***************************************************************************
void OV10640_Exposure_Print(CSI_IDX arCsiIdx, gParamSource arSource)
{
  if(arSource == SourceCamera)
  {
    //OV10640_Exposure_t lExposure;
    OV10640_ExposureAllExpGet(gOVExpPrint, (OV10640_ExpTime_t)2);
    OV10640_ExposureAllExpGet(gOVExpPrint, (OV10640_ExpTime_t)1);
    OV10640_ExposureAllExpGet(gOVExpPrint, (OV10640_ExpTime_t)0);

    printf("L: t=%d CG=%d AG=%d DG=%d - S t=%d CG=%d AG=%d DG=%d - VS t=%d CG=%d AG=%d DG=%d\n",
           gOVExpPrint.mExposureTime[L], gOVExpPrint.mConversionGain[L], gOVExpPrint.mAnalogGain[L],
           gOVExpPrint.mDigitalGain[L], gOVExpPrint.mExposureTime[S], gOVExpPrint.mConversionGain[S],
           gOVExpPrint.mAnalogGain[S], gOVExpPrint.mDigitalGain[S], gOVExpPrint.mExposureTime[VS],
           gOVExpPrint.mConversionGain[VS], gOVExpPrint.mAnalogGain[VS], gOVExpPrint.mDigitalGain[VS]);
  }
  else //PARAM_SOURCE_S32V
  {
    printf("L: t=%d CG=%d AG=%d DG=%d - S t=%d CG=%d AG=%d DG=%d - VS t=%d CG=%d AG=%d DG=%d\n",
           gOVExposure[0].mExposureTime[L], gOVExposure[0].mConversionGain[L], gOVExposure[0].mAnalogGain[L],
           gOVExposure[0].mDigitalGain[L], gOVExposure[0].mExposureTime[S], gOVExposure[0].mConversionGain[S],
           gOVExposure[0].mAnalogGain[S], gOVExposure[0].mDigitalGain[S], gOVExposure[0].mExposureTime[VS],
           gOVExposure[0].mConversionGain[VS], gOVExposure[0].mAnalogGain[VS], gOVExposure[0].mDigitalGain[VS]);
  }
}

//***************************************************************************
void CamHistGet(color_channel ch, uint8_t* stats)
{
  uint16_t i;
  uint8_t  addr;
  uint32_t sum;
  // Very Short exposure
  addr            = 0x14; // First bin at the address 0x4014, embedded row configured to start at 0x4000
  hist_VS[ch][15] = stats[(addr++) * 4 + 3] << 16; // Bin 0 on 24bits
  hist_VS[ch][15] += stats[(addr++) * 4 + 3] << 8;
  hist_VS[ch][15] += stats[(addr++) * 4 + 3]; // Bin 0 on 24bits
  sum = hist_VS[ch][15];
  for(i = 1; i < 15; i++)
  {
    hist_VS[ch][15 - i] = stats[(addr++) * 4 + 3] << 8;
    hist_VS[ch][15 - i] += stats[(addr++) * 4 + 3];
    sum += hist_VS[ch][15 - i];
  }
  sum_hist[VS][ch] = sum;
  hist_VS[ch][0]   = 55825 - sum; // Computing missing bin: 55285 = (1276*700)/16

  // Short (Medium) exposure
  addr = 0x33;
  sum  = 0;
  for(i = 0; i < 16; i++)
  {
    hist_S[ch][i] = stats[(addr++) * 4 + 3] << 8;
    hist_S[ch][i] += stats[(addr++) * 4 + 3];
    sum += hist_S[ch][i];
  }
  sum_hist[S][ch] = sum;

  // Long exposure
  addr          = 0x53;
  hist_L[ch][0] = stats[(addr++) * 4 + 3] << 16;
  hist_L[ch][0] += stats[(addr++) * 4 + 3] << 8;
  hist_L[ch][0] += stats[(addr++) * 4 + 3];
  sum = hist_L[ch][0];
  for(i = 1; i <= 15; i++)
  {
    hist_L[ch][i] = stats[(addr++) * 4 + 3] << 8;
    hist_L[ch][i] += stats[(addr++) * 4 + 3];
    sum += hist_L[ch][i];
  }
  sum_hist[L][ch] = sum;
  hist_L[ch][15]  = 55825 - sum; // Computing missing bin: 55285 = (1276*700)/16
}

//***************************************************************************
void CamHistPrint()
{
  uint16_t i;

  printf("****************** Histograms ****************\n");
  printf("VERY SHORT exposure\n");
  printf("Red    : ");
  for(i = 0; i < 16; i++)
  {
    printf("%6u, ", hist_VS[RED][i]);
  }
  printf("The sum is: %u\n", sum_hist[VS][RED]);

  printf("Green_r: ");
  for(i = 0; i < 16; i++)
  {
    printf("%6u, ", hist_VS[GREEN_R][i]);
  }
  printf("The sum is: %u\n", sum_hist[VS][GREEN_R]);

  printf("Blue   : ");
  for(i = 0; i < 16; i++)
  {
    printf("%6u, ", hist_VS[BLUE][i]);
  }
  printf("The sum is: %u\n", sum_hist[VS][BLUE]);

  printf("Green_b: ");
  for(i = 0; i < 16; i++)
  {
    printf("%6u, ", hist_VS[GREEN_B][i]);
  }
  printf("The sum is: %u\n\n", sum_hist[VS][GREEN_B]);

  //Short exposure
  printf("SHORT exposure\n");

  printf("Red    : ");
  for(i = 0; i < 16; i++)
  {
    printf("%6u, ", hist_S[RED][i]);
  }
  printf("The sum is: %u\n", sum_hist[S][RED]);

  printf("Green_r: ");
  for(i = 0; i < 16; i++)
  {
    printf("%6u, ", hist_S[GREEN_R][i]);
  }
  printf("The sum is: %u\n", sum_hist[S][GREEN_R]);

  printf("Blue   : ");
  for(i = 0; i < 16; i++)
  {
    printf("%6u, ", hist_S[BLUE][i]);
  }
  printf("The sum is: %u\n", sum_hist[S][BLUE]);

  printf("Green_b: ");
  for(i = 0; i < 16; i++)
  {
    printf("%6u, ", hist_S[GREEN_B][i]);
  }
  printf("The sum is: %u\n\n", sum_hist[S][GREEN_B]);

  //Long exposure
  printf("Long exposure\n");

  printf("Red    : ");
  for(i = 0; i < 16; i++)
  {
    printf("%6u, ", hist_L[RED][i]);
  }
  printf("The sum is: %u\n", sum_hist[L][RED]);

  printf("Green_r: ");
  for(i = 0; i < 16; i++)
  {
    printf("%6u, ", hist_L[GREEN_R][i]);
  }
  printf("The sum is: %u\n", sum_hist[L][GREEN_R]);

  printf("Blue   : ");
  for(i = 0; i < 16; i++)
  {
    printf("%6u, ", hist_L[BLUE][i]);
  }
  printf("The sum is: %u\n", sum_hist[L][BLUE]);

  printf("Green_b: ");
  for(i = 0; i < 16; i++)
  {
    printf("%6u, ", hist_L[GREEN_B][i]);
  }
  printf("The sum is: %u\n\n", sum_hist[L][GREEN_B]);
}
/*****************************************************************************/

int OV10640_ExposureValidation(OV10640_Exposure_t& arExp,
                               uint32_t            arSampleNumber,
                               float               arSampleStep,
                               float               arBelowMinMargin,
                               float               arAboveMaxMargin,
                               uint8_t             arUpDown,
                               gExposure           exp)
{
  float    lFirstExp, lLastExp, lCurrExp;
  float    lSampleStep;
  uint32_t lSampleNumber;
  lFirstExp = arBelowMinMargin * arExp.mMinExposure[exp];
  lLastExp  = arAboveMaxMargin * arExp.mMaxExposure[exp];
  if(arSampleStep == 0)
  {
    lSampleStep   = (lLastExp - lFirstExp) / arSampleNumber;
    lSampleNumber = arSampleNumber;
  }

  else
  {
    lSampleStep   = arSampleStep;
    lSampleNumber = (uint32_t)((lLastExp - lFirstExp) / arSampleStep + 1);
  }

  if((exp == L) || (exp == S))
  {
    printf("Exposure parameters Validation of L (L/S = %f)\n", arExp.mRatioLSExp);

    printf(
        "L  -  Min Exp:%f  -  Max Exp:%f \n        Start Exp:%f  -  Stop Exp:%f  -  Sample step:%f  -  Sample "
        "Numbers:%d\n",
        arExp.mMinExposure[L], arExp.mMaxExposure[L], lFirstExp, lLastExp, lSampleStep, lSampleNumber);
    //     }

    if(arUpDown)
    { //going down
      lCurrExp    = lLastExp;
      lSampleStep = -lSampleStep;
    }
    else
    { //going up
      lCurrExp = lFirstExp;
    }

    printf(
        "L Exposure Target,Exposure Time,L Conversion gain, L Analog Gain,L Digital Gain,Real L Exposure,S Analog "
        "Gain,S Digital Gain,Real DCG Exposure,Reat L/S Ratio\n");
    while((lCurrExp <= lLastExp) && (lCurrExp >= lFirstExp))
    {
      OV10640_ExposureComputationInd(arExp, lCurrExp, 0);
      printf("%f,%d,%d,%d,%f,%f\n", lCurrExp, arExp.mExposureTime[L], arExp.mConversionGain[L],
             (1 << arExp.mAnalogGain[L]), (float)(arExp.mDigitalGain[L]) / 256, arExp.mExposure[L]);
      lCurrExp += lSampleStep;
    }
  }
}
