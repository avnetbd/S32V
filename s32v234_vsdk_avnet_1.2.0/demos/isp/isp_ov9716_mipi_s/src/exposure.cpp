/*****************************************************************************
*
* freescale Confidential Proprietary
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

// for image geometries
#if((OV9716_QUAD_GRAPH == MIPI) || (OV9716_QUAD_GRAPH == 0))
#include "ov9716_mipi_s_c.h"
#include <ov9716_graph_geometries.h>
#elif(OV9716_QUAD_GRAPH == 1)
#include "ov9716_quad1_c.h"
#include <ov9716_quad1_graph_geometries.h>
#else
#include "ov9716_quad_c.h"
#include <ov9716_quad_graph_geometries.h>
#endif

#include "ov9716_defines.h" // OV9716  definitions
#include "global_defines.h" // global definitions
#include "exposure.h"       // exposure control functions

#include "isp_cam_maxim.h"  // includes IIC read write functions
#include "isp_cam_ov9716.h" // includes IIC read write functions
#include "../../../../libs/isp/cam_generic/kernel/include/ov9716_types.h"

// **************************************************************************
// constants
// **************************************************************************
#define EXPOSURE_FLICKERING_PERIOD 234
#define AEC_PERIOD 3
#define UPDATEFACTOR (0.3)

// **************************************************************************
// global variables
// **************************************************************************

// de flicker settings
static uint16_t sExpRange = 750; // default: read from VTSs are controlled
static uint8_t  sParamLEn =      // define which parameters are controlled
    OV9716_PARAM_EXPOSURE_TIME_EN | OV9716_PARAM_ANA_GAIN_EN |
    /*OV9716_PARAM_CONVERSION_GAIN_EN | */
    OV9716_PARAM_DIG_GAIN_EN | 0
    /*OV9716_PARAM_ALL_EN |*/;
static uint8_t sParamVSEn = // define which parameters are controlled
    OV9716_PARAM_EXPOSURE_TIME_EN | OV9716_PARAM_ANA_GAIN_EN |
    /*OV9716_PARAM_CONVERSION_GAIN_EN | */
    OV9716_PARAM_DIG_GAIN_EN | 0
    /*OV9716_PARAM_ALL_EN |*/;
static float sFlickerPeriod   = 234.8; // in lines
uint8_t      sDeflickerEnable = 0;     // on / off

static uint8_t sHDREngine = 7; //IPUS7 unless set differently

// DCG Exposure time
static uint16_t sDCGExp    = 235;
static uint16_t sDCGMinExp = OV9716_EXPOSURE_TIME_LS_MIN;
static uint16_t sDCGMaxExp = OV9716_EXPOSURE_TIME_LS_MAX;

// long/short maximum multiplier of Flickering period
static uint8_t sDCGExpCoefMax = 1;

// VS Exposure time
static uint16_t sVSExp    = OV9716_EXPOSURE_TIME_VS_MAX; //8.5b
static uint16_t sVSMinExp = OV9716_EXPOSURE_TIME_VS_MIN;
static uint16_t sVSMaxExp = OV9716_EXPOSURE_TIME_VS_MAX;

// HCG analog gain
static uint8_t sHCGAG    = 1;
static uint8_t sHCGMinAG = OV9716_ANA_GAIN_LS_MIN;
static uint8_t sHCGMaxAG = OV9716_ANA_GAIN_LS_MAX;
// LCG analog gain
static uint8_t sLCGAG    = 1;
static uint8_t sLCGMinAG = OV9716_ANA_GAIN_LS_MIN;
static uint8_t sLCGMaxAG = OV9716_ANA_GAIN_LS_MAX;
// VS analog gain
static uint8_t sVSAG    = 1;
static uint8_t sVSMinAG = OV9716_ANA_GAIN_VS_MIN;
static uint8_t sVSMaxAG = OV9716_ANA_GAIN_VS_MAX;

// HCG digital gain
static uint16_t sHCGDG    = OV9716_DIGITAL_GAIN_L_MIN;
static uint16_t sHCGMinDG = OV9716_DIGITAL_GAIN_L_MIN;
static uint16_t sHCGMaxDG = OV9716_DIGITAL_GAIN_L_MAX;
// LCG digital gain
static uint16_t sLCGDG    = OV9716_DIGITAL_GAIN_S_MIN;
static uint16_t sLCGMinDG = OV9716_DIGITAL_GAIN_S_MIN;
static uint16_t sLCGMaxDG = OV9716_DIGITAL_GAIN_S_MAX;
// VS digital gain
static uint16_t sVSDG    = OV9716_DIGITAL_GAIN_VS_MIN;
static uint16_t sVSMinDG = OV9716_DIGITAL_GAIN_VS_MIN;
static uint16_t sVSMaxDG = OV9716_DIGITAL_GAIN_VS_MAX;
// VS conversion gain
static uint8_t sVSCGOn                 = 1;
static uint8_t sVSConversionGainEnable = 1;

static float   sRatioMargin = 0.95;
static float   sRatio       = 1.075; //Computed based on intilialisation values
static uint8_t sAECPer      = AEC_PERIOD;

//Indicate which histogram structure to use to save the latest histogram
static uint8_t sCamHistIndex = 0;

// cam0-3
static double sChR[4] = {400.0, 400.0, 400.0, 400.0};
static double sChG[4] = {256.0, 256.0, 256.0, 256.0};
static double sChB[4] = {400.0, 400.0, 400.0, 400.0};

static uint16_t sRedTarget  = 256; // red  target value in AWB
static uint16_t sBlueTarget = 256; // blue target value in AWB

OV9716_Exposure_t        gOVExposure[4];
static OV9716_Exposure_t sOVExpPrint;

// **************************************************************************
// local functions
// **************************************************************************

// **************************************************************************
// exported functions
// **************************************************************************

// **************************************************************************
void SetHDREngine(uint8_t arHDREngine)
{
  sHDREngine = arHDREngine;
}

// **************************************************************************
void SetConverGainMode(uint8_t arNewMode)
{
  switch(arNewMode)
  {
    case 0: // on
    {
      gOVExposure[0].mVSConversionGainOn = 1;
      gOVExposure[1].mVSConversionGainOn = 1;
      gOVExposure[2].mVSConversionGainOn = 1;
      gOVExposure[3].mVSConversionGainOn = 1;
      gOVExposure[0].mVSConversionGain   = OV9716_CONVERSION_GAIN_FACTOR;
      gOVExposure[1].mVSConversionGain   = OV9716_CONVERSION_GAIN_FACTOR;
      gOVExposure[2].mVSConversionGain   = OV9716_CONVERSION_GAIN_FACTOR;
      gOVExposure[3].mVSConversionGain   = OV9716_CONVERSION_GAIN_FACTOR;
      sParamVSEn &= ~OV9716_PARAM_CONVERSION_GAIN_EN;
      sVSCGOn = 1;
    }
    break;
    case 1: // off
    {
      gOVExposure[0].mVSConversionGainOn = 0;
      gOVExposure[1].mVSConversionGainOn = 0;
      gOVExposure[2].mVSConversionGainOn = 0;
      gOVExposure[3].mVSConversionGainOn = 0;
      gOVExposure[0].mVSConversionGain   = 1;
      gOVExposure[1].mVSConversionGain   = 1;
      gOVExposure[2].mVSConversionGain   = 1;
      gOVExposure[3].mVSConversionGain   = 1;
      sVSCGOn                            = 0;
    }
    break;
    case 2: // auto
    {
      sParamVSEn |= OV9716_PARAM_CONVERSION_GAIN_EN;
    }
    break;
    default:
      break;
  }
} // SetConverGainMode

uint8_t GetConverGainMode()
{
  uint8_t mode = 0;
  if(sParamVSEn & OV9716_PARAM_CONVERSION_GAIN_EN)
  {
    mode = 2;
  }
  else if(sVSCGOn)
  {
    mode = 0;
  }
  else
  {
    mode = 1;
  }
  return mode;
} // GetConverGainMode

/************************************************************************/
void SetExpRange(uint16_t arExpRange)
{
  sExpRange = arExpRange;
  printf("Maximum exposure time is %d lines\n", sExpRange);
} //SetExpRange

/************************************************************************/
uint16_t GetExpRange()
{
  return sExpRange;
} // GetExpRange

/************************************************************************/
void SetAECPeriod(uint8_t arAECPeriod)
{
  sAECPer = arAECPeriod;
} // SetAECPeriod

/************************************************************************/
uint8_t GetAECPeriod()
{
  return sAECPer;
} // GetAECPeriod

/************************************************************************/
void SetBlueTarget(uint16_t arBlueTarget)
{
  sBlueTarget = arBlueTarget;
} // SetBlueTarget

/************************************************************************/
uint16_t GetBlueTarget()
{
  return sBlueTarget;
} // GetBlueTarget

/************************************************************************/
void SetRedTarget(uint16_t arRedTarget)
{
  sRedTarget = arRedTarget;
} // SetRedTarget

/************************************************************************/
uint16_t GetRedTarget()
{
  return sRedTarget;
} // GetRedTarget

/************************************************************************/
void EnableDeflicker(uint8_t arOnOff)

{
  uint16_t lMinExp, lMaxExp;
  if(arOnOff)
  {
    lMinExp = (uint16_t)(sFlickerPeriod + 0.5);
    lMaxExp = (uint16_t)(sDCGExpCoefMax * sFlickerPeriod + 0.5);
    for(uint32_t lCam = 0; lCam < 4; lCam++)
    {
      gOVExposure[lCam].mDCGDeflickeringEnable = 1;
      gOVExposure[lCam].mDCGFlickerPeriod      = sFlickerPeriod;
      gOVExposure[lCam].mMinDCGExposureTime    = lMinExp;
      gOVExposure[lCam].mMaxDCGExposureTime    = lMaxExp;
    } // for all cameras
  }
  else
  { // Deflickering off
    for(uint32_t lCam = 0; lCam < 4; lCam++)
    {
      gOVExposure[lCam].mDCGDeflickeringEnable = 0;
      gOVExposure[lCam].mMinDCGExposureTime    = sDCGMinExp;
      gOVExposure[lCam].mMaxDCGExposureTime    = sDCGMaxExp;
    } // for all cameras
  }

  // Recompute Min/Max exposure
  for(uint32_t lCam = 0; lCam < 4; lCam++)
  {
    gOVExposure[lCam].mMinExposure[OV9716_ExpType_HCG] =
        (float)(gOVExposure[lCam].mMinDCGExposureTime) *
        (float)(1 << gOVExposure[lCam].mMinAnalogGain[OV9716_ExpType_HCG]) *
        ((float)(gOVExposure[lCam].mMinDigitalGain[OV9716_ExpType_HCG]) / 256); // 6.8b value

    gOVExposure[lCam].mMinExposure[OV9716_ExpType_LCG] =
        (float)(gOVExposure[lCam].mMinDCGExposureTime) *
        (float)(1 << gOVExposure[lCam].mMinAnalogGain[OV9716_ExpType_LCG]) *
        ((float)(gOVExposure[lCam].mMinDigitalGain[OV9716_ExpType_LCG]) / 256); // 6.8b val

    // Compute total Maximum of the exposures
    gOVExposure[lCam].mMaxExposure[OV9716_ExpType_HCG] =
        (float)(gOVExposure[lCam].mMaxDCGExposureTime) *
        (float)(1 << gOVExposure[lCam].mMaxAnalogGain[OV9716_ExpType_HCG]) *
        ((float)(gOVExposure[lCam].mMaxDigitalGain[OV9716_ExpType_HCG]) / 256); // 6.8b value

    gOVExposure[lCam].mMaxExposure[OV9716_ExpType_LCG] =
        (float)(gOVExposure[lCam].mMaxDCGExposureTime) *
        (float)(1 << gOVExposure[lCam].mMaxAnalogGain[OV9716_ExpType_LCG]) *
        ((float)(gOVExposure[lCam].mMaxDigitalGain[OV9716_ExpType_LCG]) / 256); // 6.8b value
  }                                                                             // for all cameras

  printf("Setting de-flicker %s\n", (arOnOff) ? "on" : "off");
} // EnableDeflicker()

/************************************************************************/
void SetDeflickerHz(float arFrequency, float arFps)
{
  uint8_t i = 0;

  sFlickerPeriod = (float)(sExpRange) / arFrequency * (float)(arFps);

  printf("Setting Flicker frequency to %fHz %flines\n", arFrequency, sFlickerPeriod);

  while(((uint32_t)(i * sFlickerPeriod + 0.5)) <= (uint32_t)(sExpRange))
  {
    i++;
  }
  sDCGExpCoefMax = i - 1;
  printf("Maximum deflicker exposure time is %d lines\n", (uint16_t)(sDCGExpCoefMax * sFlickerPeriod + 0.5));
} // SetDeflickerHz()

// **************************************************************************
void ExposureInit()
{
  float lExpHCGSat;
  for(int i = 0; i < 4; i++)
  {
    gOVExposure[i].mDCGExposureTime = sDCGExp;
    // Disable by default -
    // Can only be enabled by calling EnableDeflicker
    gOVExposure[i].mDCGDeflickeringEnable = 0;

    gOVExposure[i].mAECFactor = UPDATEFACTOR;

    gOVExposure[i].mVSExposureTime                  = sVSExp; //8.5b
    gOVExposure[i].mVSConversionGainOn              = sVSCGOn;
    gOVExposure[i].mVSConversionGainEnable          = sVSConversionGainEnable;
    gOVExposure[i].mAnalogGain[OV9716_ExpType_HCG]  = sHCGAG;
    gOVExposure[i].mAnalogGain[OV9716_ExpType_LCG]  = sLCGAG;
    gOVExposure[i].mAnalogGain[OV9716_ExpType_VS]   = sVSAG;
    gOVExposure[i].mDigitalGain[OV9716_ExpType_HCG] = sHCGDG;
    gOVExposure[i].mDigitalGain[OV9716_ExpType_LCG] = sLCGDG;
    gOVExposure[i].mDigitalGain[OV9716_ExpType_VS]  = sVSDG;

    // Min/Max Exposure parameters
    gOVExposure[i].mMinDCGExposureTime                 = sDCGMinExp;
    gOVExposure[i].mMinVSExposureTime                  = sVSMinExp;
    gOVExposure[i].mMinAnalogGain[OV9716_ExpType_HCG]  = sHCGMinAG;
    gOVExposure[i].mMinAnalogGain[OV9716_ExpType_LCG]  = sLCGMinAG;
    gOVExposure[i].mMinAnalogGain[OV9716_ExpType_VS]   = sVSMinAG;
    gOVExposure[i].mMinDigitalGain[OV9716_ExpType_HCG] = sHCGMinDG;
    gOVExposure[i].mMinDigitalGain[OV9716_ExpType_LCG] = sLCGMinDG;
    gOVExposure[i].mMinDigitalGain[OV9716_ExpType_VS]  = sVSMinDG;

    gOVExposure[i].mMaxDCGExposureTime                 = sDCGMaxExp;
    gOVExposure[i].mMaxVSExposureTime                  = sVSMaxExp;
    gOVExposure[i].mMaxAnalogGain[OV9716_ExpType_HCG]  = sHCGMaxAG;
    gOVExposure[i].mMaxAnalogGain[OV9716_ExpType_LCG]  = sLCGMaxAG;
    gOVExposure[i].mMaxAnalogGain[OV9716_ExpType_VS]   = sVSMaxAG;
    gOVExposure[i].mMaxDigitalGain[OV9716_ExpType_HCG] = sHCGMaxDG;
    gOVExposure[i].mMaxDigitalGain[OV9716_ExpType_LCG] = sLCGMaxDG;
    gOVExposure[i].mMaxDigitalGain[OV9716_ExpType_VS]  = sVSMaxDG;

    // Compute total Minimums of the exposures
    gOVExposure[i].mMinExposure[OV9716_ExpType_HCG] =
        (float)(gOVExposure[i].mMinDCGExposureTime) * (float)(1 << gOVExposure[i].mMinAnalogGain[OV9716_ExpType_HCG]) *
        ((float)(gOVExposure[i].mMinDigitalGain[OV9716_ExpType_HCG]) / 256); // 6.8b value

    gOVExposure[i].mMinExposure[OV9716_ExpType_LCG] =
        (float)(gOVExposure[i].mMinDCGExposureTime) * (float)(1 << gOVExposure[i].mMinAnalogGain[OV9716_ExpType_LCG]) *
        ((float)(gOVExposure[i].mMinDigitalGain[OV9716_ExpType_LCG]) / 256); // 6.8b val

    if(sVSCGOn)
    {
      gOVExposure[i].mMinExposure[OV9716_ExpType_VS] =
          ((float)(gOVExposure[i].mMinVSExposureTime) / 32) * //8.5b
          (float)(1 << gOVExposure[i].mMinAnalogGain[OV9716_ExpType_VS]) * OV9716_CONVERSION_GAIN_FACTOR *
          ((float)(gOVExposure[i].mMinDigitalGain[OV9716_ExpType_VS]) / 256); // 6.8b value
    }
    else
    {
      gOVExposure[i].mMinExposure[OV9716_ExpType_VS] =
          ((float)(gOVExposure[i].mMinVSExposureTime) / 32) * //8.5b
          (float)(1 << gOVExposure[i].mMinAnalogGain[OV9716_ExpType_VS]) *
          ((float)(gOVExposure[i].mMinDigitalGain[OV9716_ExpType_VS]) / 256); // 6.8b value
    }
    // Compute total Maximum of the exposures
    gOVExposure[i].mMaxExposure[OV9716_ExpType_HCG] =
        (float)(gOVExposure[i].mMaxDCGExposureTime) * (float)(1 << gOVExposure[i].mMaxAnalogGain[OV9716_ExpType_HCG]) *
        ((float)(gOVExposure[i].mMaxDigitalGain[OV9716_ExpType_HCG]) / 256); // 6.8b value

    gOVExposure[i].mMaxExposure[OV9716_ExpType_LCG] =
        (float)(gOVExposure[i].mMaxDCGExposureTime) * (float)(1 << gOVExposure[i].mMaxAnalogGain[OV9716_ExpType_LCG]) *
        ((float)(gOVExposure[i].mMaxDigitalGain[OV9716_ExpType_LCG]) / 256); // 6.8b value
    if(sVSCGOn)
    {
      gOVExposure[i].mMaxExposure[OV9716_ExpType_VS] =
          ((float)(gOVExposure[i].mMaxVSExposureTime) / 32) * //8.5b
          (float)(1 << gOVExposure[i].mMaxAnalogGain[OV9716_ExpType_VS]) * OV9716_CONVERSION_GAIN_FACTOR *
          ((float)(gOVExposure[i].mMaxDigitalGain[OV9716_ExpType_VS]) / 256); // 6.8b value
    }
    else
    {
      gOVExposure[i].mMaxExposure[OV9716_ExpType_VS] =
          ((float)(gOVExposure[i].mMaxVSExposureTime) / 32) * //8.5b
          (float)(1 << gOVExposure[i].mMaxAnalogGain[OV9716_ExpType_VS]) *
          ((float)(gOVExposure[i].mMaxDigitalGain[OV9716_ExpType_VS]) / 256); // 6.8b value
    }

      // Compute total configured exposures
#if 1
    gOVExposure[i].mExposure[OV9716_ExpType_VS] =
        ((float)(gOVExposure[i].mVSExposureTime) / 32) * //8.5b
        (1 << gOVExposure[i].mAnalogGain[OV9716_ExpType_VS]) * gOVExposure[i].mVSConversionGain *
        ((float)(gOVExposure[i].mDigitalGain[OV9716_ExpType_VS]) / 256); // 6.8b value
    gOVExposure[i].mExposure[OV9716_ExpType_LCG] =
        (float)(gOVExposure[i].mDCGExposureTime) * (1 << gOVExposure[i].mAnalogGain[OV9716_ExpType_LCG]) *
        ((float)(gOVExposure[i].mDigitalGain[OV9716_ExpType_LCG]) / 256); // 6.8b value
    gOVExposure[i].mExposure[OV9716_ExpType_HCG] =
        (float)(gOVExposure[i].mDCGExposureTime) * (1 << gOVExposure[i].mAnalogGain[OV9716_ExpType_HCG]) *
        ((float)(gOVExposure[i].mDigitalGain[OV9716_ExpType_HCG]) / 256); // 6.8b value
#else
    MyOV9716_ExposureTotal(gOVExposure[i], OV9716_ExpType_VS);
    MyOV9716_ExposureTotal(gOVExposure[i], OV9716_ExpType_LCG);
    MyOV9716_ExposureTotal(gOVExposure[i], OV9716_ExpType_HCG);
#endif
    gOVExposure[i].mRatioHLExp =
        (float)(gOVExposure[i].mExposure[OV9716_ExpType_HCG]) / (float)(gOVExposure[i].mExposure[OV9716_ExpType_LCG]);
    gOVExposure[i].mRatioHLExpCG =
        (OV9716_CONVERSION_GAIN_FACTOR * (float)(gOVExposure[i].mExposure[OV9716_ExpType_HCG])) /
        (float)(gOVExposure[i].mExposure[OV9716_ExpType_LCG]);
    gOVExposure[i].mRatioMargin = sRatioMargin;

    // Computation of the priority between HCG and LCG for AEC computation
    lExpHCGSat = gOVExposure[i].mRatioHLExp *
                 gOVExposure[i].mMinExposure[OV9716_ExpType_LCG]; // Compute HCG exposure when LCG is at its minimum
    if(lExpHCGSat > gOVExposure[i].mMinExposure[OV9716_ExpType_HCG])
    { // HCG exposure will reach its minimum before LCG

      // It needs the priority to get more flexibility with exposure time
      gOVExposure[i].mExposurePriority = OV9716_ExpType_LCG;
    }
    else
    {
      gOVExposure[i].mExposurePriority = OV9716_ExpType_HCG;
    }
  }
  sRatio = gOVExposure[0].mRatioHLExp;
} // ExposureInit()

// **************************************************************************
void ExposureCtl(CSI_IDX        arCsiIdx,
                 uint8_t*       arExpCnt,
                 SEQ_IpuHist_t* arIpuHist,
                 uint8_t        arLogLevel,
                 uint8_t        arAECon,
                 uint8_t        arTargetBinLS,
                 double         arTargetRatioLS,
                 uint8_t        arTargetBinVS,
                 double         arTargetRatioVS,
                 uint8_t        arAWBon)
{
#define PIXCOUNT (57600)
  uint32_t lTargetCnt;
  uint32_t lTargetCntVS;
  uint8_t  lCam;
  uint8_t  lCamStart;
  uint8_t  lCamEnd;
  //char lChanStr[4][3]={"R ","GR","GV","B "};
  //return;

// #define AEC_UNIQUECTL
#ifdef AEC_UNIQUECTL
#define AEC_SET_BASE MAXIM_CHNL_ORIG_BC
  // use always camera 0 to measure
  SEQ_HistogramGet(&arIpuHist[sCamHistIndex]);
  lCamStart = 0;
  lCamEnd   = 0;
#else
#define AEC_SET_BASE MAXIM_CHNL_0
  // *** get histogram ***
  SEQ_HistogramGet(&arIpuHist[sCamHistIndex]);

#if(CAMS == 1)
  lCamStart = 0;
  lCamEnd = 0;
  if((arIpuHist[sCamHistIndex].mFrmSeqNum % sAECPer) != 0)
  {
    return;
  }
    //   printf("f:%d\n", arIpuHist[sCamHistIndex].mFrmSeqNum);

#else
  if((arIpuHist[sCamHistIndex].mFrmSeqNum % 2) == 0)
  // if(sCamHistIndex == 0)
  {
    //SEQ_HistogramGet(&arIpuHist[0]);
    lCamStart = 0;
    lCamEnd   = 1;
  }
  else
  {
    //SEQ_HistogramGet(&arIpuHist[1]);
    lCamStart = 2;
    lCamEnd   = 3;
  }
#endif //CAMS 1
#endif //AEC_UNIQUECTL

  lCam = lCamStart;
  while(lCam <= lCamEnd)
  {
    double   lLevel;
    double   lUpdateExpLS;
    double   lUpdateExpVS;
    uint32_t lSumL, lSumH;
    uint32_t lSumR, lSumB;
    uint32_t lSumVsL, lSumVsH;
    uint32_t lWeightR, lWeightG, lWeightB;
    double   lUpdateR, lUpdateB;
    uint16_t lHistIndex;
    uint16_t lHistBin;
    uint8_t  lOffset;

    if((lCam == 1) || (lCam == 3))
    {
      lOffset = 1;
    }
    else
    {
      lOffset = 0;
    }

    if(arTargetRatioLS < 0)
    {
      arTargetRatioLS = 0.0;
    }
    lUpdateExpLS = 1.0;
    lTargetCnt   = arTargetRatioLS * PIXCOUNT;
    if(lTargetCnt < 10)
    {
      lTargetCnt = 10;
    }
    if(lTargetCnt > (PIXCOUNT - 10))
    {
      lTargetCnt = PIXCOUNT - 10;
    }

    if(arTargetRatioVS < 0)
    {
      arTargetRatioVS = 0.0;
    }
    lUpdateExpVS = 1.0;
    lTargetCntVS = arTargetRatioVS * PIXCOUNT;
    if(lTargetCntVS < 10)
    {
      lTargetCntVS = 10;
    }
    if(lTargetCntVS > (PIXCOUNT - 10))
    {
      lTargetCntVS = (PIXCOUNT - 10);
    }

    lUpdateR = lUpdateB = 1.0;
    //camera 0
    lHistIndex = 0;
    lLevel     = 0;
    lHistBin   = 128 * lOffset;

    lSumL = lSumH = lSumR = lSumB = 0;
    lWeightR = lWeightG = lWeightB = 0;

    while(lHistIndex < 32)
    { // GR/GB long+shorthistogram
      lSumL = lSumH;
      lSumH += arIpuHist[sCamHistIndex].mpData[lHistBin + 32];
      lSumR += arIpuHist[sCamHistIndex].mpData[lHistBin + 96];
      lSumB += arIpuHist[sCamHistIndex].mpData[lHistBin];
      lWeightR += arIpuHist[sCamHistIndex].mpData[lHistBin + 96] * (2 * lHistIndex + 1);
      lWeightG += arIpuHist[sCamHistIndex].mpData[lHistBin + 32] * (2 * lHistIndex + 1);
      lWeightB += arIpuHist[sCamHistIndex].mpData[lHistBin] * (2 * lHistIndex + 1);
      lHistIndex++;
      lHistBin++;
      // found limit now get the level
      if(lSumH > lTargetCnt)
      {
        uint32_t diffhist   = lSumH - lSumL;
        uint32_t difftarget = lSumH - lTargetCnt;
        lLevel              = lHistIndex;
        if(diffhist)
        {
          lLevel = lLevel - (double)difftarget / (double)diffhist;
        }
        lUpdateExpLS = (((double)arTargetBinLS) / lLevel - 1.0) * gOVExposure[lCam].mAECFactor + 1.0;
        break;
      }
    }

    lSumL = lSumH;
    while(lHistIndex < 32)
    { // GR/GB long+shorthistogram
      lSumH += arIpuHist[sCamHistIndex].mpData[lHistBin + 32];
      lSumR += arIpuHist[sCamHistIndex].mpData[lHistBin + 96];
      lSumB += arIpuHist[sCamHistIndex].mpData[lHistBin];
      // OVT swapped R and B
      lWeightR += arIpuHist[sCamHistIndex].mpData[lHistBin + 96] * (2 * lHistIndex + 1);
      lWeightG += arIpuHist[sCamHistIndex].mpData[lHistBin + 32] * (2 * lHistIndex + 1);
      lWeightB += arIpuHist[sCamHistIndex].mpData[lHistBin] * (2 * lHistIndex + 1);
      lHistIndex++;
      lHistBin++;
    }

    //lLevel = 0;
    lHistIndex = 0;
    lSumVsL = lSumVsH = 0;
    lHistBin          = 128 * lOffset;
    while(lHistIndex < 32)
    { // VS
      lSumVsL = lSumVsH;
      lSumVsH += arIpuHist[sCamHistIndex].mpData[lHistBin + 64];

      lHistIndex++;
      lHistBin++;
      if(lSumVsH > lTargetCntVS)
      {
        uint32_t diffhist   = lSumVsH - lSumVsL;
        uint32_t difftarget = lSumVsH - lTargetCntVS;

        lLevel = lHistIndex;
        if(diffhist)
        {
          lLevel = lLevel - (double)difftarget / (double)diffhist;
        }
        lUpdateExpVS = (((double)arTargetBinVS) / lLevel - 1.0) * gOVExposure[lCam].mAECFactor + 1.0;
        break;
      }
    }

    if((lSumH != PIXCOUNT) || (lSumH != lSumR) || (lSumH != lSumB))
    {
      printf("Histogram sum does not match G/R/B  (%d/%d/%d)\n", lSumH, lSumR, lSumB);
    }

    // grouped update
    CAM_N_RegWrite(arCsiIdx, OV9716_GRP_HOLD, 0, (MAXIM_CHNL)(AEC_SET_BASE + lCam));
    CAM_N_RegWrite(arCsiIdx, OV9716_GRP_SEL, 0, (MAXIM_CHNL)(AEC_SET_BASE + lCam));

    if(arAECon)
    {
      MyOV9716_ExposureSet(arCsiIdx, AEC_SET_BASE + lCam, lUpdateExpLS, lUpdateExpVS);
    }

    if(arAWBon)
    {
      lUpdateR = (double)lWeightG / (double)lWeightR * (double)sRedTarget / 256.0;
      if(lUpdateR > 2.0)
      {
        lUpdateR = 2.0;
      }
      if(lUpdateR < 0.5)
      {
        lUpdateR = 0.5;
      }
      lUpdateB = (double)lWeightG / (double)lWeightB * (double)sBlueTarget / 256.0;
      if(lUpdateB > 2.0)
      {
        lUpdateB = 2.0;
      }
      if(lUpdateB < 0.5)
      {
        lUpdateB = 0.5;
      }

      ChgainUpdate(arCsiIdx, AEC_SET_BASE + lCam, lUpdateR, lUpdateB);
    }

    // update group
    CAM_N_RegWrite(arCsiIdx, OV9716_GRP_SEL, 0x20, (MAXIM_CHNL)(AEC_SET_BASE + lCam));
    CAM_N_RegWrite(arCsiIdx, OV9716_GRP_HOLD, 1, (MAXIM_CHNL)(AEC_SET_BASE + lCam));
    //gGroupRegisters=0x0;

    if((arLogLevel == 6) && (lCam == 0))
    {
      printf(" Cam%d - DCG:%f    - VS:%f \n", lCam, lUpdateExpLS, lUpdateExpVS);
    }
    if((arLogLevel == 7) && (lCam == 0))
    {
      printf(" Cam%d WeightR %d, WeightG %d, WeightB %d\n", lCam, lWeightR, lWeightG, lWeightB);
    }

    lCam++;
  }

  if(*arExpCnt == (EXPCYCLE - 1))
  {
    *arExpCnt = 0;
  }
  else
  {
    // increment exposure cycle counter
    (*arExpCnt)++;
  }
} // ExpControl()

// **************************************************************************

void MyOV9716_ExposureTotal(OV9716_Exposure_t& arExp, OV9716_ExposureType_t arE)
{
  if(arE == OV9716_ExpType_HCG)
  {
    arExp.mExposure[OV9716_ExpType_HCG] = (float)(arExp.mDCGExposureTime) *
                                          (1 << arExp.mAnalogGain[OV9716_ExpType_HCG]) *
                                          ((float)(arExp.mDigitalGain[OV9716_ExpType_HCG]) / 256); // 6.8b value
  }
  else if(arE == OV9716_ExpType_LCG)
  {
    arExp.mExposure[OV9716_ExpType_LCG] = (float)(arExp.mDCGExposureTime) *
                                          (1 << arExp.mAnalogGain[OV9716_ExpType_LCG]) *
                                          ((float)(arExp.mDigitalGain[OV9716_ExpType_LCG]) / 256); // 6.8b value
  }
  else
  {
    arExp.mExposure[OV9716_ExpType_VS] = ((float)(arExp.mVSExposureTime) / 32) * //8.5b
                                         (1 << arExp.mAnalogGain[OV9716_ExpType_VS]) * arExp.mVSConversionGain *
                                         ((float)(arExp.mDigitalGain[OV9716_ExpType_VS]) / 256); // 6.8b value
  }
} // ExposureTotal()

// **************************************************************************

int32_t MyOV9716_ExposureSet(CSI_IDX aCsiIdx, uint8_t aCamNum, float aLSUpdate, float aVSUpdate)
{
  uint8_t lCamIndex;
  int32_t lRet = 0;

  if(aCamNum == MAXIM_CHNL_ORIG_BC)
  {
    lCamIndex = 0; // use index for cam0 and broadcast
  }
  else
  {
    lCamIndex = aCamNum - 1; // use index for cam0 and broadcast
  }

#if 0
  if(MAXIM_OV9716_ExposureSet(aCsiIdx,
                              aCamNum,
                              gOVExposure[lCamIndex],
                              aLSUpdate,
                              aVSUpdate) != CAM_LIB_SUCCESS)
  {
    printf("Failed to set exposure updates \n");
    lRet = -1;
  }
#else
  // check if increase is out of bounds
  if(aLSUpdate >= 2.0)
    aLSUpdate = 2.0;
  MyOV9716_ExposureDCGComputation(gOVExposure[lCamIndex],
                                  aLSUpdate * gOVExposure[lCamIndex].mExposure[OV9716_ExpType_HCG]);

  // Update new values for L/S exposures
  CAM_N_RegWrite(aCsiIdx, OV9716_GRP_CEXP_DCG_H, gOVExposure[lCamIndex].mDCGExposureTime >> 8, (MAXIM_CHNL)aCamNum);
  CAM_N_RegWrite(aCsiIdx, OV9716_GRP_CEXP_DCG_L, gOVExposure[lCamIndex].mDCGExposureTime & 0xff, (MAXIM_CHNL)aCamNum);
  CAM_N_RegWrite(aCsiIdx, OV9716_GRP_CG_GAIN,
                 ((gOVExposure[lCamIndex].mVSConversionGainOn & 1) << 7) |
                     ((gOVExposure[lCamIndex].mAnalogGain[OV9716_ExpType_VS] & 3) << 4) |
                     ((gOVExposure[lCamIndex].mAnalogGain[OV9716_ExpType_LCG] & 3) << 2) |
                     ((gOVExposure[lCamIndex].mAnalogGain[OV9716_ExpType_HCG] & 3) << 0),
                 (MAXIM_CHNL)aCamNum);

  CAM_N_RegWrite(aCsiIdx, OV9716_GRP_DIG_GAIN_HCG_H, gOVExposure[lCamIndex].mDigitalGain[OV9716_ExpType_HCG] >> 8,
                 (MAXIM_CHNL)aCamNum);
  CAM_N_RegWrite(aCsiIdx, OV9716_GRP_DIG_GAIN_HCG_L, gOVExposure[lCamIndex].mDigitalGain[OV9716_ExpType_HCG] & 0xff,
                 (MAXIM_CHNL)aCamNum);
  CAM_N_RegWrite(aCsiIdx, OV9716_GRP_DIG_GAIN_LCG_H,
                 gOVExposure[lCamIndex].mDigitalGain[OV9716_ExpType_LCG] >> 8, // sth why not gSDG?
                 (MAXIM_CHNL)aCamNum);
  CAM_N_RegWrite(aCsiIdx, OV9716_GRP_DIG_GAIN_LCG_L,
                 gOVExposure[lCamIndex].mDigitalGain[OV9716_ExpType_LCG] & 0xff, // sth why not gSDG?
                 (MAXIM_CHNL)aCamNum);

  if(aVSUpdate != 1.0)
  {
    if(aVSUpdate < 0.5)
      aVSUpdate = 0.5;
    if(aVSUpdate > 2.0)
      aVSUpdate = 2.0;

    CAM_N_RegWrite(aCsiIdx, OV9716_GRP_CG_GAIN,
                   ((gOVExposure[lCamIndex].mVSConversionGainOn & 1) << 7) |
                       ((gOVExposure[lCamIndex].mAnalogGain[OV9716_ExpType_VS] & 3) << 4) |
                       ((gOVExposure[lCamIndex].mAnalogGain[OV9716_ExpType_LCG] & 3) << 2) |
                       ((gOVExposure[lCamIndex].mAnalogGain[OV9716_ExpType_HCG] & 3) << 0),
                   (MAXIM_CHNL)aCamNum);

    CAM_N_RegWrite(aCsiIdx, OV9716_GRP_DIG_GAIN_VS_H, gOVExposure[lCamIndex].mDigitalGain[OV9716_ExpType_VS] >> 8,
                   (MAXIM_CHNL)aCamNum);
    CAM_N_RegWrite(aCsiIdx, OV9716_GRP_DIG_GAIN_VS_L, gOVExposure[lCamIndex].mDigitalGain[OV9716_ExpType_VS] & 0xff,
                   (MAXIM_CHNL)aCamNum);
    CAM_N_RegWrite(aCsiIdx, OV9716_GRP_CEXP_VS_L, gOVExposure[lCamIndex].mVSExposureTime & 0x1f,
                   (MAXIM_CHNL)aCamNum); // update fractional part of exposure time
  }
#endif

  return lRet;

} //ExposureSet()

// **************************************************************************

void MyOV9716_ExposureDCGComputation(OV9716_Exposure_t& arExp, float arNewHCGExpTotal)
{
  float    lNewHCGExposure, lNewLCGExposure, lNewExposure, lTempExp, lRatio;
  float    lFloatDigGain, lGain, lExpTime;
  uint8_t  lCurrCG, lAnGain;
  uint16_t lDigGain, lExpCoef = 0;

  lNewHCGExposure = arNewHCGExpTotal;
  /* Maintain the ratio between LCG and HCG, saturation of the new exposure based on min/max */
  if(arNewHCGExpTotal > arExp.mMaxExposure[OV9716_ExpType_HCG])
  {
    // Saturate HCG exposure to maximum
    lNewHCGExposure = arExp.mMaxExposure[OV9716_ExpType_HCG];
  }
  else if(arNewHCGExpTotal < arExp.mMinExposure[OV9716_ExpType_HCG])
  {
    // Saturate HCG exposure to minimum
    lNewHCGExposure = arExp.mMinExposure[OV9716_ExpType_HCG];
  }
  lNewLCGExposure = lNewHCGExposure / arExp.mRatioHLExp; //Calculate LCG exposure based on HCG/LCG ratio
  if(lNewLCGExposure > arExp.mMaxExposure[OV9716_ExpType_LCG])
  {
    // Saturate LCG exposure to maximum and recompute HCG exposure to keep ratio
    lNewLCGExposure = arExp.mMaxExposure[OV9716_ExpType_LCG];
    lNewHCGExposure = lNewLCGExposure * arExp.mRatioHLExp;
  }
  else if(lNewLCGExposure < arExp.mMinExposure[OV9716_ExpType_LCG])
  {
    // Saturate LCG exposure to minimum and recompute HCG exposure
    lNewLCGExposure = arExp.mMinExposure[OV9716_ExpType_LCG];
    lNewHCGExposure = lNewLCGExposure * arExp.mRatioHLExp;
  }

  // Compute one of the exposure (HCG or LCG) based of the priority parameter)
  lCurrCG = arExp.mExposurePriority;
  if(lCurrCG == OV9716_ExpType_HCG)
  {
    lNewExposure = lNewHCGExposure;
  }
  else
  {
    lNewExposure = lNewLCGExposure;
  }
  lAnGain  = arExp.mMinAnalogGain[lCurrCG];
  lDigGain = arExp.mMinDigitalGain[lCurrCG];
  lGain    = (1 << lAnGain) * (float)(lDigGain) / 256;
  while((lAnGain < arExp.mMaxAnalogGain[lCurrCG]) && (lNewExposure / lGain) >= arExp.mMaxDCGExposureTime)
  {
    // Maximising exposure time is not enough to reach new exposure and analog gain can be increased
    lAnGain++;
    lGain = lGain * 2;
  }
  lExpTime = lNewExposure / lGain;

  if(lExpTime > arExp.mMaxDCGExposureTime)
  {
    // Maximise exposure time but also need to increase digital gain
    lExpTime      = arExp.mMaxDCGExposureTime;
    lFloatDigGain = lNewExposure / (arExp.mMaxDCGExposureTime * (1 << lAnGain));
    lDigGain      = (uint16_t)(lFloatDigGain * 256 + 0.5);
  }
  else if(arExp.mDCGDeflickeringEnable) //Deflickering
  {
    lExpCoef = (uint16_t)(lExpTime / arExp.mDCGFlickerPeriod);
    if(lExpCoef < 1)
    {
      lExpCoef = 1;
    }
    lExpTime          = lExpCoef * arExp.mDCGFlickerPeriod;
    lFloatDigGain     = lNewExposure / (lExpTime * (1 << lAnGain)); //Compensate with digital gain
    lDigGain          = (uint16_t)(lFloatDigGain * 256 + 0.5);
    arExp.mDCGExpCoef = lExpCoef;
  }

  arExp.mAnalogGain[lCurrCG]  = lAnGain;
  arExp.mDigitalGain[lCurrCG] = lDigGain;
  arExp.mDCGExposureTime      = (uint16_t)(lExpTime + 0.5);
  MyOV9716_ExposureTotal(arExp, (OV9716_ExposureType_t)lCurrCG);

  // Compute second exposure parameters (exposure time is now fixed)
  if(lCurrCG == OV9716_ExpType_HCG)
  {
    lNewExposure = lNewLCGExposure;
    lCurrCG      = OV9716_ExpType_LCG;
  }
  else
  {
    lNewExposure = lNewHCGExposure;
    ;
    lCurrCG = OV9716_ExpType_HCG;
  }

  lAnGain  = arExp.mMinAnalogGain[lCurrCG];
  lDigGain = arExp.mMinDigitalGain[lCurrCG];
  lGain    = (1 << lAnGain) * (float)(lDigGain) / 256;
  lTempExp = lExpTime * lGain;
  while((lAnGain < arExp.mMaxAnalogGain[lCurrCG]) && (lTempExp * 2 < lNewExposure))
  {
    lAnGain++;
    lTempExp = lTempExp * 2;
  }
  //Adjust Digital gain
  lFloatDigGain = lNewExposure / (lExpTime * (1 << lAnGain));
  lDigGain      = (uint16_t)(lFloatDigGain * 256 + 0.5);

  arExp.mAnalogGain[lCurrCG]  = lAnGain;
  arExp.mDigitalGain[lCurrCG] = lDigGain;
  MyOV9716_ExposureTotal(arExp, (OV9716_ExposureType_t)lCurrCG);

  /* Verify that parameters are whithin limits */
  if((arExp.mDigitalGain[OV9716_ExpType_LCG] < arExp.mMinDigitalGain[OV9716_ExpType_LCG]) ||
     (arExp.mDigitalGain[OV9716_ExpType_LCG] > arExp.mMaxDigitalGain[OV9716_ExpType_LCG]))
  {
    printf("Digital Gain of LCG out of limit [%d ; %d]: %d\n", arExp.mMinDigitalGain[OV9716_ExpType_LCG],
           arExp.mMaxDigitalGain[OV9716_ExpType_LCG], arExp.mDigitalGain[OV9716_ExpType_LCG]);
  }
  if((arExp.mAnalogGain[OV9716_ExpType_LCG] < arExp.mMinAnalogGain[OV9716_ExpType_LCG]) ||
     (arExp.mAnalogGain[OV9716_ExpType_LCG] > arExp.mMaxAnalogGain[OV9716_ExpType_LCG]))
  {
    printf("Analog Gain of LCG out of limit [%d ; %d]: %d\n", 1 << arExp.mMinAnalogGain[OV9716_ExpType_LCG],
           1 << arExp.mMaxAnalogGain[OV9716_ExpType_LCG], 1 << arExp.mAnalogGain[OV9716_ExpType_LCG]);
  }
  if((arExp.mDigitalGain[OV9716_ExpType_HCG] < arExp.mMinDigitalGain[OV9716_ExpType_HCG]) ||
     (arExp.mDigitalGain[OV9716_ExpType_HCG] > arExp.mMaxDigitalGain[OV9716_ExpType_HCG]))
  {
    printf("Digital Gain of HCG out of limit [%d ; %d]: %d\n", arExp.mMinDigitalGain[OV9716_ExpType_HCG],
           arExp.mMaxDigitalGain[OV9716_ExpType_HCG], arExp.mDigitalGain[OV9716_ExpType_HCG]);
  }
  if((arExp.mAnalogGain[OV9716_ExpType_HCG] < arExp.mMinAnalogGain[OV9716_ExpType_HCG]) ||
     (arExp.mAnalogGain[OV9716_ExpType_HCG] > arExp.mMaxAnalogGain[OV9716_ExpType_HCG]))
  {
    printf("Analog Gain of HCG out of limit [%d ; %d]: %d\n", 1 << arExp.mMinAnalogGain[OV9716_ExpType_HCG],
           1 << arExp.mMaxAnalogGain[OV9716_ExpType_HCG], 1 << arExp.mAnalogGain[OV9716_ExpType_HCG]);
  }
  if((arExp.mDCGExposureTime < arExp.mMinDCGExposureTime) || (arExp.mDCGExposureTime > arExp.mMaxDCGExposureTime))
  {
    printf("Exposure time of DCG is out of limit [%d ; %d]: %d\n", 1 << arExp.mMinDCGExposureTime,
           1 << arExp.mMaxDCGExposureTime, 1 << arExp.mDCGExposureTime);
  }

  /* Verify that the ratio between the exposure is ok */
  lRatio = arExp.mExposure[OV9716_ExpType_HCG] / arExp.mExposure[OV9716_ExpType_LCG];
  if((lRatio <= arExp.mRatioMargin * arExp.mRatioHLExp) || (lRatio >= (1 / arExp.mRatioMargin) * arExp.mRatioHLExp))
  {
    printf(
        "Ratio between HCG and LCG is not maintained\n"
        "HCG exp:%f\n    LCG exp:%f\n"
        "Target ratio:%f\n"
        "Current ratio:%f\n",
        arExp.mExposure[OV9716_ExpType_HCG], arExp.mExposure[OV9716_ExpType_LCG], arExp.mRatioHLExp, lRatio);
  }
} // ExposureDCGComputation()

// **************************************************************************

void MyOV9716_ExposureDCGRatioChange(float arNewRatio)
{
  float lExpHCGSat;

  lExpHCGSat =
      arNewRatio * gOVExposure[0].mMinExposure[OV9716_ExpType_LCG]; // Compute HCG exposure when LCG is at its minimum
  if(lExpHCGSat > gOVExposure[0].mMinExposure[OV9716_ExpType_HCG])
  {
    // HCG exposure will reach its minimum before LCG
    // It needs the priority to get more flexibility with exposure time
    gOVExposure[0].mExposurePriority = OV9716_ExpType_LCG;
  }
  else
  {
    gOVExposure[0].mExposurePriority = OV9716_ExpType_HCG;
  }

  for(uint8_t cam = 0; cam < 4; cam++)
  {
    gOVExposure[cam].mRatioHLExp       = arNewRatio;
    gOVExposure[cam].mExposurePriority = gOVExposure[0].mExposurePriority;
    MyOV9716_ExposureDCGComputation(gOVExposure[cam], gOVExposure[cam].mExposure[OV9716_ExpType_HCG]);
  }
}

// **************************************************************************

void MyOV9716_ExposureVSIndependant(OV9716_Exposure_t& arExp, float arVSUpdate)
{
  float lExposureTarget;
  lExposureTarget = arVSUpdate * arExp.mExposure[OV9716_ExpType_VS];
  MyOV9716_ExposureVSComputation(arExp, lExposureTarget);
}

// **************************************************************************

void MyOV9716_ExposureVSComputation(OV9716_Exposure_t& arExp, float arTotalExposure)
{
  float                     lNewExposure, lTempExp /*, lRatio*/;
  float /*lFloatDigGain, */ lGain, /*lExpTime,*/ lDigGain;
  uint8_t /*lCurrCG, */     lAnGain;

  /* Maintain VS whithin min/max */
  if(arTotalExposure > arExp.mMaxExposure[OV9716_ExpType_VS])
  {
    // Saturate VS exposure to minimum
    lNewExposure = arExp.mMaxExposure[OV9716_ExpType_VS];
    if(arExp.mExposure[OV9716_ExpType_VS] == arExp.mMaxExposure[OV9716_ExpType_VS])
    {
      return; // VS alreay at it maximum no need to compute new parameters
    }
  }
  else if(arTotalExposure < arExp.mMinExposure[OV9716_ExpType_VS])
  {
    // Saturate VS exposure to its minimum
    lNewExposure = arExp.mMinExposure[OV9716_ExpType_VS];
    if(arExp.mExposure[OV9716_ExpType_VS] == arExp.mMinExposure[OV9716_ExpType_VS])
    {
      return; // VS alreay at it minimum
    }
  }
  else
  {
    lNewExposure = arTotalExposure;
  }

  lAnGain  = arExp.mMinAnalogGain[OV9716_ExpType_VS];
  lDigGain = (float)(arExp.mMinDigitalGain[OV9716_ExpType_VS]) / 256;
  lGain    = (1 << lAnGain) * lDigGain * arExp.mVSConversionGain;
  if(lGain * ((float)(arExp.mMaxVSExposureTime) / 32) > lNewExposure)
  {
    lTempExp                              = lNewExposure / lGain;
    arExp.mVSExposureTime                 = (uint16_t)(lTempExp * 32 + 0.5);
    arExp.mAnalogGain[OV9716_ExpType_VS]  = lAnGain;
    arExp.mDigitalGain[OV9716_ExpType_VS] = (uint16_t)(lDigGain * 256 + 0.5);
  }
  else
  {
    arExp.mVSExposureTime = arExp.mMaxVSExposureTime;
    lTempExp              = ((float)(arExp.mMaxVSExposureTime) / 32) * lGain; //(1<<lAnGain) * lDigGain;
    while((lAnGain < arExp.mMaxAnalogGain[OV9716_ExpType_VS]) && (lTempExp * 2 <= lNewExposure))
    {
      // Maximising exposure time is not enough to reach new exposure and analog gain can be increased
      // As long as it is not exceding the target exposure
      lAnGain++;
      lTempExp = lTempExp * 2;
    }
    lDigGain = lNewExposure / (((float)(arExp.mMaxVSExposureTime) / 32) * (1 << lAnGain) * arExp.mVSConversionGain);
    arExp.mDigitalGain[OV9716_ExpType_VS] = (uint16_t)(lDigGain * 256 + 0.5);
    arExp.mAnalogGain[OV9716_ExpType_VS]  = lAnGain;
  }
  MyOV9716_ExposureTotal(arExp, OV9716_ExpType_VS); // Compute the total of the new exposure of VS

  // Verify that parameters are whithin limits
  if((arExp.mDigitalGain[OV9716_ExpType_VS] < arExp.mMinDigitalGain[OV9716_ExpType_VS]) ||
     (arExp.mDigitalGain[OV9716_ExpType_VS] > arExp.mMaxDigitalGain[OV9716_ExpType_VS]))
  {
    printf("Digital Gain of VS out of limit [%f ; %f]: %f\n", (float)(arExp.mMinDigitalGain[OV9716_ExpType_VS]) / 256,
           (float)(arExp.mMaxDigitalGain[OV9716_ExpType_VS]) / 256,
           (float)(arExp.mDigitalGain[OV9716_ExpType_VS]) / 256);
  }
  if((arExp.mAnalogGain[OV9716_ExpType_VS] < arExp.mMinAnalogGain[OV9716_ExpType_VS]) ||
     (arExp.mAnalogGain[OV9716_ExpType_VS] > arExp.mMaxAnalogGain[OV9716_ExpType_VS]))
  {
    printf("Analog Gain of VS out of limit [%d ; %d]: %d\n", 1 << arExp.mMinAnalogGain[OV9716_ExpType_VS],
           1 << arExp.mMaxAnalogGain[OV9716_ExpType_VS], 1 << arExp.mAnalogGain[OV9716_ExpType_VS]);
  }
  if((arExp.mVSExposureTime < arExp.mMinVSExposureTime) || (arExp.mVSExposureTime > arExp.mMaxVSExposureTime))
  {
    printf("Exposure time of VS is out of limit [%f ; %f]: %f\n", (float)(arExp.mMinVSExposureTime) / 32,
           (float)(arExp.mMaxVSExposureTime) / 32, (float)(arExp.mVSExposureTime) / 32);
  }

  // Verify that the New exposure match the target
  if(arExp.mExposure[OV9716_ExpType_VS] < 0.97 * lNewExposure)
  {
    printf(
        "VS exposure below target - %f instead of %f -"
        "Exposure time:%f  CG:%d  AG:%d  DG:%f\n",
        arExp.mExposure[OV9716_ExpType_VS], lNewExposure, ((float)arExp.mVSExposureTime) / 32.0,
        arExp.mVSConversionGainOn, ((uint8_t)1) << arExp.mAnalogGain[OV9716_ExpType_VS],
        ((float)arExp.mDigitalGain[OV9716_ExpType_VS]) / 256.0);
  }
  if(arExp.mExposure[OV9716_ExpType_VS] > 1.03 * lNewExposure)
  {
    printf("VS exposure above target - %f instead of %f - Exposure time:%f  CG:%d  AG:%d  DG:%f\n",
           arExp.mExposure[OV9716_ExpType_VS], lNewExposure, ((float)arExp.mVSExposureTime) / 32.0,
           arExp.mVSConversionGainOn, ((uint8_t)1) << arExp.mAnalogGain[OV9716_ExpType_VS],
           ((float)arExp.mDigitalGain[OV9716_ExpType_VS]) / 256.0);
  }
}

// **************************************************************************

void MyOV9716_ExposureValidation(OV9716_Exposure_t& arExp,
                                 uint32_t           arSampleNumber,
                                 float              arSampleStep,
                                 float              arBelowMinMargin,
                                 float              arAboveMaxMargin,
                                 uint8_t            arExposure)
{
  float lFirstExp, lLastExp, lCurrExp;
  ;
  float    lSampleStep;
  uint32_t lSampleNumber;
  lFirstExp = arBelowMinMargin * arExp.mMinExposure[arExposure];
  lLastExp  = arAboveMaxMargin * arExp.mMaxExposure[arExposure];
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

  if((arExposure == OV9716_ExpType_LCG) || (arExposure == OV9716_ExpType_HCG))
  {
    printf("Exposure parameters Validation of DCG (HCG/LCG = %f)\n", arExp.mRatioHLExp);
    printf("LCG  -  Min Exp:%f  -  Max Exp:%f \n", arExp.mMinExposure[OV9716_ExpType_LCG],
           arExp.mMaxExposure[OV9716_ExpType_LCG]);
    printf(
        "HCG  -  Min Exp:%f  -  Max Exp:%f \n  Start Exp:%f  -  Stop Exp:%f  -  Sample step:%f  -  Sample Numbers:%d\n",
        arExp.mMinExposure[OV9716_ExpType_HCG], arExp.mMaxExposure[OV9716_ExpType_HCG], lFirstExp, lLastExp,
        lSampleStep, lSampleNumber);

    lCurrExp = lFirstExp;
    printf(
        "HCG Exposure Target,Exposure Time,HCG Analog Gain,HCG Digital Gain,Real HCG Exposure,LCG Analog Gain,LCG "
        "Digital Gain,Real DCG Exposure,Reat HCG/LCG Ratio\n");
    while(lCurrExp <= lLastExp)
    {
      MyOV9716_ExposureDCGComputation(arExp, lCurrExp);
      printf("%f,%d,%d,%f,%f,%d,%f,%f,%f\n", lCurrExp, arExp.mDCGExposureTime,
             (1 << arExp.mAnalogGain[OV9716_ExpType_HCG]), (float)(arExp.mDigitalGain[OV9716_ExpType_HCG]) / 256,
             arExp.mExposure[OV9716_ExpType_HCG], (1 << arExp.mAnalogGain[OV9716_ExpType_LCG]),
             (float)(arExp.mDigitalGain[OV9716_ExpType_LCG]) / 256, arExp.mExposure[OV9716_ExpType_LCG],
             arExp.mExposure[OV9716_ExpType_HCG] / arExp.mExposure[OV9716_ExpType_LCG]);
      lCurrExp += lSampleStep;
    }
  }
  else // VS
  {
    printf("Exposure parameters Validation of VS\n");
    printf("Min Exp:%f  -  Max Exp:%f \n  Start Exp:%f  -  Stop Exp:%f  -  Sample step:%f  -  Sample Numbers:%d\n",
           arExp.mMinExposure[OV9716_ExpType_VS], arExp.mMaxExposure[OV9716_ExpType_VS], lFirstExp, lLastExp,
           lSampleStep, lSampleNumber);
    lCurrExp = lFirstExp;
    printf(
        "VS Exposure Target,VS Exposure Time, VS Conversion Gain, VS Analog Gain,VS Digital Gain,Real VS Exposure\n");
    while(lCurrExp <= lLastExp)
    {
      MyOV9716_ExposureVSComputation(arExp, lCurrExp);
      printf("%f,%f,%f,%d,%f,%f\n", lCurrExp, (float)(arExp.mVSExposureTime) / 32, arExp.mVSConversionGain,
             (1 << arExp.mAnalogGain[OV9716_ExpType_VS]), (float)(arExp.mDigitalGain[OV9716_ExpType_VS]) / 256,
             arExp.mExposure[OV9716_ExpType_VS]);
      lCurrExp += lSampleStep;
    }
  }
}

//***************************************************************************

void MyOV9716_Exposure_Print(CSI_IDX arCsiIdx, gParamSource arSource)
{
  if(arSource == SourceCamera)
  {
    //OV9716_Exposure_t lExposure;
    sOVExpPrint.mCsiIdx = arCsiIdx;
    OV9716_ExposureTimeGet(sOVExpPrint, OV9716_ExpType_HCG);
    OV9716_ExposureTimeGet(sOVExpPrint, OV9716_ExpType_VS);
    OV9716_ExposureAnalogConversionGainGet(sOVExpPrint);
    OV9716_ExposureDigitalGainGet(sOVExpPrint, OV9716_ExpType_HCG);
    OV9716_ExposureDigitalGainGet(sOVExpPrint, OV9716_ExpType_LCG);
    OV9716_ExposureDigitalGainGet(sOVExpPrint, OV9716_ExpType_VS);

    printf(
        "DCG: t=%d  [HCG (L): AG=%d (x%d) DG=0x%04x]"
        " [LCG (S): AG=%d (x%d)  DG=0x%04x] -"
        " VS t_frac=0x%04x  CG=%d AG=%d (x%d) DG=0x%04x\n",
        sOVExpPrint.mDCGExposureTime, sOVExpPrint.mAnalogGain[OV9716_ExpType_HCG],
        (1 << sOVExpPrint.mAnalogGain[OV9716_ExpType_HCG]), sOVExpPrint.mDigitalGain[OV9716_ExpType_HCG],
        sOVExpPrint.mAnalogGain[OV9716_ExpType_LCG], (1 << sOVExpPrint.mAnalogGain[OV9716_ExpType_LCG]),
        sOVExpPrint.mDigitalGain[OV9716_ExpType_LCG], (sOVExpPrint.mVSExposureTime & 0x1f),
        sOVExpPrint.mVSConversionGainOn, sOVExpPrint.mAnalogGain[OV9716_ExpType_VS],
        (1 << sOVExpPrint.mAnalogGain[OV9716_ExpType_VS]), sOVExpPrint.mDigitalGain[OV9716_ExpType_VS]);
  }
  else //PARAM_SOURCE_S32V
  {
    printf(
        "DCG: t=%d  [HCG (L): AG=%d (x%d) DG=0x%04x] [LCG (S): AG=%d (x%d)  DG=0x%04x] - VS t_frac=0x%04x  CG=%d AG=%d "
        "(x%d) DG=0x%04x\n",
        gOVExposure[0].mDCGExposureTime, gOVExposure[0].mAnalogGain[OV9716_ExpType_HCG],
        (1 << gOVExposure[0].mAnalogGain[OV9716_ExpType_HCG]), gOVExposure[0].mDigitalGain[OV9716_ExpType_HCG],
        gOVExposure[0].mAnalogGain[OV9716_ExpType_LCG], (1 << gOVExposure[0].mAnalogGain[OV9716_ExpType_LCG]),
        gOVExposure[0].mDigitalGain[OV9716_ExpType_LCG], (gOVExposure[0].mVSExposureTime & 0x1f),
        gOVExposure[0].mVSConversionGainOn, gOVExposure[0].mAnalogGain[OV9716_ExpType_VS],
        (1 << gOVExposure[0].mAnalogGain[OV9716_ExpType_VS]), gOVExposure[0].mDigitalGain[OV9716_ExpType_VS]);
  }

} // MyOV9716_Exposure_Print()

// **************************************************************************
void ChgainUpdate(CSI_IDX arCsiIdx, uint8_t arCamNum, double arUpdateR, double arUpdateB)

{
  double  lMin;
  uint8_t lCamIndex;
  uint8_t i;

  if(arCamNum == MAXIM_CHNL_ORIG_BC)
  {
    lCamIndex = 0; // use index for cam0 and broadcast
    for(i = 0; i < 4; i++)
    {
    }
  }
  else
  {
    lCamIndex = arCamNum - 1;
  }

  // IIR filter
  sChR[lCamIndex] += (sChR[lCamIndex] * arUpdateR - sChR[lCamIndex]) / CHGAINTAU;
  sChB[lCamIndex] += (sChB[lCamIndex] * arUpdateB - sChB[lCamIndex]) / CHGAINTAU;
  lMin = sChG[lCamIndex];
  if(lMin > sChR[lCamIndex])
  {
    lMin = sChR[lCamIndex];
  }
  if(lMin > sChB[lCamIndex])
  {
    lMin = sChB[lCamIndex];
  }
  if(lMin < 64.0)
  {
    lMin = 64.0;
  }
  sChR[lCamIndex] = 256.0 / lMin * sChR[lCamIndex];
  sChG[lCamIndex] = 256.0 / lMin * sChG[lCamIndex];
  sChB[lCamIndex] = 256.0 / lMin * sChB[lCamIndex];

#define CHGAINMAX (800.0)
  if(sChR[lCamIndex] > CHGAINMAX)
  {
    sChR[lCamIndex] = CHGAINMAX;
  }
  if(sChR[lCamIndex] < 256.0)
  {
    sChR[lCamIndex] = 256.0;
  }
  if(sChG[lCamIndex] > CHGAINMAX)
  {
    sChG[lCamIndex] = CHGAINMAX;
  }
  if(sChG[lCamIndex] < 256.0)
  {
    sChG[lCamIndex] = 256.0;
  }
  if(sChB[lCamIndex] > CHGAINMAX)
  {
    sChB[lCamIndex] = CHGAINMAX;
  }
  if(sChB[lCamIndex] < 256.0)
  {
    sChB[lCamIndex] = 256.0;
  }

  MyOV9716_ChgainSet(arCsiIdx, arCamNum, sChR[lCamIndex], sChG[lCamIndex], sChB[lCamIndex]);
} // ChgainUpdate()

// **************************************************************************
void MyOV9716_ChgainManSetR(CSI_IDX arCsiIdx, uint8_t arCamNum, int16_t arIncrementR)
{
  uint8_t lCamIndex;
  uint8_t i;

  if(arCamNum == MAXIM_CHNL_ORIG_BC)
  {
    lCamIndex = 0; // use index for cam0 and broadcast
    for(i = 0; i < 4; i++)
    {
      sChR[i] += (double)arIncrementR;
      if(sChR[i] > 768.0)
      {
        sChR[i] = 768.0;
        printf("Red Channel gain reached max\n");
      }
      else if(sChR[i] < 256.0)
      {
        sChR[i] = 256.0;
        printf("Red Channel gain reached min\n");
      }
    }
  }
  else
  {
    lCamIndex = arCamNum - 1;
    sChR[lCamIndex] += (double)arIncrementR;
    if(sChR[lCamIndex] > 768.0)
    {
      sChR[lCamIndex] = 768.0;
      printf("Red Channel gain reached max\n");
    }
    if(sChR[lCamIndex] < 256.0)
    {
      sChR[lCamIndex] = 256.0;
      printf("Red Channel gain reached min\n");
    }
  }

  MyOV9716_ChgainSet(arCsiIdx, arCamNum, sChR[lCamIndex], sChG[lCamIndex], sChB[lCamIndex]);
} //MyOV9716_ChgainManSetR()

// **************************************************************************
void MyOV9716_ChgainManSetB(CSI_IDX arCsiIdx, uint8_t arCamNum, int16_t arIncrementB)
{
  uint8_t lCamIndex;
  uint8_t i;

  if(arCamNum == MAXIM_CHNL_ORIG_BC)
  {
    lCamIndex = 0; // use index for cam0 and broadcast
    for(i = 0; i < 4; i++)
    {
      sChB[i] += (double)arIncrementB;
      if(sChB[i] > 768.0)
      {
        sChB[i] = 768.0;
        printf("Blue Channel gain reached max\n");
      }
      else if(sChB[i] < 256.0)
      {
        sChB[i] = 256.0;
        printf("Blue Channel gain reached min\n");
      }
    }
  }
  else
  {
    lCamIndex = arCamNum - 1;
    sChB[lCamIndex] += (double)arIncrementB;
    if(sChB[lCamIndex] > 768.0)
    {
      sChB[lCamIndex] = 768.0;
      printf("Blue Channel gain reached max\n");
    }
    if(sChB[lCamIndex] < 256.0)
    {
      sChB[lCamIndex] = 256.0;
      printf("Blue Channel gain reached min\n");
    }
  }

  MyOV9716_ChgainSet(arCsiIdx, arCamNum, sChR[lCamIndex], sChG[lCamIndex], sChB[lCamIndex]);
} // MyOV9716_ChgainManSetB()

// **************************************************************************
void MyOV9716_ChgainSet(CSI_IDX arCsiIdx, uint8_t arCamNum, double arRGain, double arGGain, double arBGain)

{
  uint8_t lCamIndex;
  uint8_t i;

  if(arCamNum == MAXIM_CHNL_ORIG_BC)
  {
    lCamIndex = 0; // use index for cam0 and broadcast
    for(i = 0; i < 4; i++)
    {
      sChR[i] = arRGain;
      sChG[i] = arGGain;
      sChB[i] = arBGain;
    }
  }
  else
  {
    lCamIndex       = arCamNum - 1;
    sChR[lCamIndex] = arRGain;
    sChG[lCamIndex] = arGGain;
    sChB[lCamIndex] = arBGain;
  }

  for(uint8_t i = 0; i < 3; i++)
  {
    CAM_N_RegWrite(arCsiIdx, OV9716_GRP_CHGAIN_L + i * 8, ((uint32_t)(MINCHGAIN * arRGain)) >> 8, (MAXIM_CHNL)arCamNum);
    CAM_N_RegWrite(arCsiIdx, OV9716_GRP_CHGAIN_L + 1 + i * 8, ((uint32_t)(MINCHGAIN * arRGain)) & 0xff,
                   (MAXIM_CHNL)arCamNum);
    CAM_N_RegWrite(arCsiIdx, OV9716_GRP_CHGAIN_L + 2 + i * 8, ((uint32_t)(MINCHGAIN * arGGain)) >> 8,
                   (MAXIM_CHNL)arCamNum);
    CAM_N_RegWrite(arCsiIdx, OV9716_GRP_CHGAIN_L + 3 + i * 8, ((uint32_t)(MINCHGAIN * arGGain)) & 0xff,
                   (MAXIM_CHNL)arCamNum);
    CAM_N_RegWrite(arCsiIdx, OV9716_GRP_CHGAIN_L + 4 + i * 8, ((uint32_t)(MINCHGAIN * arGGain)) >> 8,
                   (MAXIM_CHNL)arCamNum);
    CAM_N_RegWrite(arCsiIdx, OV9716_GRP_CHGAIN_L + 5 + i * 8, ((uint32_t)(MINCHGAIN * arGGain)) & 0xff,
                   (MAXIM_CHNL)arCamNum);
    CAM_N_RegWrite(arCsiIdx, OV9716_GRP_CHGAIN_L + 6 + i * 8, ((uint32_t)(MINCHGAIN * arBGain)) >> 8,
                   (MAXIM_CHNL)arCamNum);
    CAM_N_RegWrite(arCsiIdx, OV9716_GRP_CHGAIN_L + 7 + i * 8, ((uint32_t)(MINCHGAIN * arBGain)) & 0xff,
                   (MAXIM_CHNL)arCamNum);
  } // for all color compononents
}

//***************************************************************************

void MyOV9716_Chgain_Print(CSI_IDX arCsiIdx, gParamSource arSource)
{
  if(arSource == SourceCamera)
  {
    uint8_t  val;
    uint16_t val16;
    // print log for camera 0

    CAM_N_RegRead(arCsiIdx, OV9716_CHGAIN_L + 0, val, MAXIM_CHNL_0);
    val16 = val << 8;
    CAM_N_RegRead(arCsiIdx, OV9716_CHGAIN_L + 1, val, MAXIM_CHNL_0);
    printf("cam%d chgain R:%d", MAXIM_CHNL_0 - 1, val16 | val);
    CAM_N_RegRead(arCsiIdx, OV9716_CHGAIN_L + 2, val, MAXIM_CHNL_0);
    val16 = val << 8;
    CAM_N_RegRead(arCsiIdx, OV9716_CHGAIN_L + 3, val, MAXIM_CHNL_0);
    printf(" Gr:%d", val16 | val);
    CAM_N_RegRead(arCsiIdx, OV9716_CHGAIN_L + 4, val, MAXIM_CHNL_0);
    val16 = val << 8;
    CAM_N_RegRead(arCsiIdx, OV9716_CHGAIN_L + 5, val, MAXIM_CHNL_0);
    printf(" Gb:%d", val16 | val);
    CAM_N_RegRead(arCsiIdx, OV9716_CHGAIN_L + 6, val, MAXIM_CHNL_0);
    val16 = val << 8;
    CAM_N_RegRead(arCsiIdx, OV9716_CHGAIN_L + 7, val, MAXIM_CHNL_0);
    printf(" B:%d", val16 | val);
    printf("\n");
  }
  else //SourceS32v
  {
    printf("cam%d chgain R:%d G:%d B:%d\n", MAXIM_CHNL_0 - 1, (int32_t)sChR[0], (int32_t)sChG[0], (int32_t)sChB[0]);
  }
}

/* EOF */
