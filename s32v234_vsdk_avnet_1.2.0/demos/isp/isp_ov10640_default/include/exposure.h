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

#ifndef EXPOSUER_H
#define EXPOSUER_H
#include "typedefs.h"                   // where do we find uintXX_t ??
#include "isp_csi.h"                    // CSI_IDX
#include "isp_seq.h"                    // histogram types
#ifndef __STANDALONE__
#include "unistd.h"                     // msleep/usleep
#endif

#ifdef __cplusplus
extern "C" {
#endif



#define OV10640_REAL_GAIN_L_H_ADDR  0x3105  ///< Long real gain
#define OV10640_REAL_GAIN_L_L_ADDR  0x3106  ///< Long real gain
#define OV10640_REAL_GAIN_S_H_ADDR  0x3107  ///< short real gain
#define OV10640_REAL_GAIN_S_L_ADDR  0x3108  ///< short real gain
#define OV10640_REAL_GAIN_VS_H_ADDR 0x3109  ///< veryshort real gain
#define OV10640_REAL_GAIN_VS_L_ADDR 0x310a  ///< veryshort real gain

#define OV10640_RATIO_LS_H_ADDR  0x310b  ///< long/short ratio
#define OV10640_RATIO_LS_L_ADDR  0x310c  ///< long/short ratio
#define OV10640_RATIO_LVS_H_ADDR 0x310d  ///< long/veryshort ratio
#define OV10640_RATIO_LVS_L_ADDR 0x310e  ///< long/veryshort ratio
#define OV10640_RATIO_SVS_H_ADDR 0x310f  ///< short/veryshort ratio
#define OV10640_RATIO_SVS_L_ADDR 0x3110  ///< short/veryshort ratio

#define OV10640_D_EV_L_3_ADDR 0x3111  ///< Longest EV byte 3
#define OV10640_D_EV_L_2_ADDR 0x3112  ///< Longest EV byte 2
#define OV10640_D_EV_L_1_ADDR 0x3113  ///< Longest EV byte 1
#define OV10640_D_EV_L_0_ADDR 0x3114  ///< Longest EV byte 0


#define OV10640_D_EV_S_3_ADDR 0x3115  ///< Shortest EV byte 3
#define OV10640_D_EV_S_2_ADDR 0x3116  ///< Shortest EV byte 2
#define OV10640_D_EV_S_1_ADDR 0x3117  ///< Shortest EV byte 1
#define OV10640_D_EV_S_0_ADDR 0x3118  ///< Shortest EV byte 0

// long channel gain
#define OV10640_R_R_GAIN_L_I_H_ADDR  0x31c3
#define OV10640_R_R_GAIN_L_I_L_ADDR  0x31c4
#define OV10640_R_GR_GAIN_L_I_H_ADDR 0x31c5
#define OV10640_R_GR_GAIN_L_I_L_ADDR 0x31c6
#define OV10640_R_GB_GAIN_L_I_H_ADDR 0x31c7
#define OV10640_R_GB_GAIN_L_I_L_ADDR 0x31c8
#define OV10640_R_B_GAIN_L_I_H_ADDR  0x31c9
#define OV10640_R_B_GAIN_L_I_L_ADDR  0x31ca
// short channel gain
#define OV10640_R_R_GAIN_S_I_H_ADDR  0x31cb
#define OV10640_R_R_GAIN_S_I_L_ADDR  0x31cc
#define OV10640_R_GR_GAIN_S_I_H_ADDR 0x31cd
#define OV10640_R_GR_GAIN_S_I_L_ADDR 0x31ce
#define OV10640_R_GB_GAIN_S_I_H_ADDR 0x31cf
#define OV10640_R_GB_GAIN_S_I_L_ADDR 0x31d0
#define OV10640_R_B_GAIN_S_I_H_ADDR  0x31d1
#define OV10640_R_B_GAIN_S_I_L_ADDR  0x31d2
// very short channel gain
#define OV10640_R_R_GAIN_VS_I_H_ADDR  0x31d3
#define OV10640_R_R_GAIN_VS_I_L_ADDR  0x31d4
#define OV10640_R_GR_GAIN_VS_I_H_ADDR 0x31d5
#define OV10640_R_GR_GAIN_VS_I_L_ADDR 0x31d6
#define OV10640_R_GB_GAIN_VS_I_H_ADDR 0x31d7
#define OV10640_R_GB_GAIN_VS_I_L_ADDR 0x31d8
#define OV10640_R_B_GAIN_VS_I_H_ADDR  0x31d9
#define OV10640_R_B_GAIN_VS_I_L_ADDR  0x31da
  
// black level pedestal
#define OV10640_BLC_TARGET_L_H_ADDR  0x30C3
#define OV10640_BLC_TARGET_L_L_ADDR  0x30C4
#define OV10640_BLC_TARGET_S_H_ADDR  0x30C5
#define OV10640_BLC_TARGET_S_L_ADDR  0x30C6
#define OV10640_BLC_TARGET_VS_H_ADDR  0x30C7
#define OV10640_BLC_TARGET_VS_L_ADDR  0x30C8

// Exposure Time
#define OV10640_EXPO_L_H_ADDR  	0x30E6
#define OV10640_EXPO_L_L_ADDR  	0x30E7
#define OV10640_EXPO_S_H_ADDR  	0x30E8
#define OV10640_EXPO_S_L_ADDR  	0x30E9
#define OV10640_EXPO_VS_ADDR  	0x30EA	// last 5bits means fraction

#define OV10640_CG_AGAIN_ADDR	0x30EB
#define OV10640_DGAIN_L_H_ADDR	0x30EC
#define OV10640_DGAIN_L_L_ADDR	0x30ED
#define OV10640_DGAIN_S_H_ADDR	0x30EE
#define OV10640_DGAIN_S_L_ADDR	0x30EF
#define OV10640_DGAIN_VS_H_ADDR	0x30F0
#define OV10640_DGAIN_VS_L_ADDR	0x30F1

  
/******************* Group Hold registers **********************/
#define OV10640_GPR_HOLD_ADDR	0x302F
#define OV10640_GPR_SEL_ADDR	0x302C


#define OV10640_GRP_REAL_GAIN_L_H_ADDR  0xB105  ///< Long real gain
#define OV10640_GRP_REAL_GAIN_L_L_ADDR  0xB106  ///< Long real gain
#define OV10640_GRP_REAL_GAIN_S_H_ADDR  0xB107  ///< short real gain
#define OV10640_GRP_REAL_GAIN_S_L_ADDR  0xB108  ///< short real gain
#define OV10640_GRP_REAL_GAIN_VS_H_ADDR 0xB109  ///< veryshort real gain
#define OV10640_GRP_REAL_GAIN_VS_L_ADDR 0xB10a  ///< veryshort real gain

#define OV10640_GRP_RATIO_LS_H_ADDR  0xB10b  ///< long/short ratio
#define OV10640_GRP_RATIO_LS_L_ADDR  0xB10c  ///< long/short ratio
#define OV10640_GRP_RATIO_LVS_H_ADDR 0xB10d  ///< long/veryshort ratio
#define OV10640_GRP_RATIO_LVS_L_ADDR 0xB10e  ///< long/veryshort ratio
#define OV10640_GRP_RATIO_SVS_H_ADDR 0xB10f  ///< short/veryshort ratio
#define OV10640_GRP_RATIO_SVS_L_ADDR 0xB110  ///< short/veryshort ratio

#define OV10640_GRP_D_EV_L_3_ADDR 0xB111  ///< Longest EV byte 3
#define OV10640_GRP_D_EV_L_2_ADDR 0xB112  ///< Longest EV byte 2
#define OV10640_GRP_D_EV_L_1_ADDR 0xB113  ///< Longest EV byte 1
#define OV10640_GRP_D_EV_L_0_ADDR 0xB114  ///< Longest EV byte 0


#define OV10640_GRP_D_EV_S_3_ADDR 0xB115  ///< Shortest EV byte 3
#define OV10640_GRP_D_EV_S_2_ADDR 0xB116  ///< Shortest EV byte 2
#define OV10640_GRP_D_EV_S_1_ADDR 0xB117  ///< Shortest EV byte 1
#define OV10640_GRP_D_EV_S_0_ADDR 0xB118  ///< Shortest EV byte 0

// long channel gain
#define OV10640_GRP_R_R_GAIN_L_I_H_ADDR  0xB1c3
#define OV10640_GRP_R_R_GAIN_L_I_L_ADDR  0xB1c4
#define OV10640_GRP_R_GR_GAIN_L_I_H_ADDR 0xB1c5
#define OV10640_GRP_R_GR_GAIN_L_I_L_ADDR 0xB1c6
#define OV10640_GRP_R_GB_GAIN_L_I_H_ADDR 0xB1c7
#define OV10640_GRP_R_GB_GAIN_L_I_L_ADDR 0xB1c8
#define OV10640_GRP_R_B_GAIN_L_I_H_ADDR  0xB1c9
#define OV10640_GRP_R_B_GAIN_L_I_L_ADDR  0xB1ca
// short channel gain
#define OV10640_GRP_R_R_GAIN_S_I_H_ADDR  0xB1cb
#define OV10640_GRP_R_R_GAIN_S_I_L_ADDR  0xB1cc
#define OV10640_GRP_R_GR_GAIN_S_I_H_ADDR 0xB1cd
#define OV10640_GRP_R_GR_GAIN_S_I_L_ADDR 0xB1ce
#define OV10640_GRP_R_GB_GAIN_S_I_H_ADDR 0xB1cf
#define OV10640_GRP_R_GB_GAIN_S_I_L_ADDR 0xB1d0
#define OV10640_GRP_R_B_GAIN_S_I_H_ADDR  0xB1d1
#define OV10640_GRP_R_B_GAIN_S_I_L_ADDR  0xB1d2
// very short channel gain
#define OV10640_GRP_R_R_GAIN_VS_I_H_ADDR  0xB1d3
#define OV10640_GRP_R_R_GAIN_VS_I_L_ADDR  0xB1d4
#define OV10640_GRP_R_GR_GAIN_VS_I_H_ADDR 0xB1d5
#define OV10640_GRP_R_GR_GAIN_VS_I_L_ADDR 0xB1d6
#define OV10640_GRP_R_GB_GAIN_VS_I_H_ADDR 0xB1d7
#define OV10640_GRP_R_GB_GAIN_VS_I_L_ADDR 0xB1d8
#define OV10640_GRP_R_B_GAIN_VS_I_H_ADDR  0xB1d9
#define OV10640_GRP_R_B_GAIN_VS_I_L_ADDR  0xB1da

// black level pedestal
#define OV10640_GRP_BLC_TARGET_L_H_ADDR  0xB0C3
#define OV10640_GRP_BLC_TARGET_L_L_ADDR  0xB0C4
#define OV10640_GRP_BLC_TARGET_S_H_ADDR  0xB0C5
#define OV10640_GRP_BLC_TARGET_S_L_ADDR  0xB0C6
#define OV10640_GRP_BLC_TARGET_VS_H_ADDR  0xB0C7
#define OV10640_GRP_BLC_TARGET_VS_L_ADDR  0xB0C8

// Exposure Time
#define OV10640_GRP_EXPO_L_H_ADDR  	0xB0E6
#define OV10640_GRP_EXPO_L_L_ADDR  	0xB0E7
#define OV10640_GRP_EXPO_S_H_ADDR  	0xB0E8
#define OV10640_GRP_EXPO_S_L_ADDR  	0xB0E9
#define OV10640_GRP_EXPO_VS_ADDR  	0xB0EA	// last 5bits means fraction

#define OV10640_GRP_CG_AGAIN_ADDR	0xB0EB
#define OV10640_GRP_DGAIN_L_H_ADDR	0xB0EC
#define OV10640_GRP_DGAIN_L_L_ADDR	0xB0ED
#define OV10640_GRP_DGAIN_S_H_ADDR	0xB0EE
#define OV10640_GRP_DGAIN_S_L_ADDR	0xB0EF
#define OV10640_GRP_DGAIN_VS_H_ADDR	0xB0F0
#define OV10640_GRP_DGAIN_VS_L_ADDR	0xB0F1


/*********** Constant *********************/

#define OV10640_ANA_GAIN_1	0x0
#define OV10640_ANA_GAIN_2	0x1
#define OV10640_ANA_GAIN_4	0x2
#define OV10640_ANA_GAIN_8	0x3

#define OV10640_PARAM_ALL_EN				0xF
#define OV10640_PARAM_EXPOSURE_TIME_EN		0x1
#define OV10640_PARAM_CONVERSION_GAIN_EN	0x2
#define OV10640_PARAM_ANA_GAIN_EN			0x4
#define OV10640_PARAM_DIG_GAIN_EN			0x8

#define OV10640_CONVERSION_GAIN_FACTOR	2.57
	
#define OV10640_DIGITAL_GAIN_S_MIN		576//0x300	// ??
#define OV10640_DIGITAL_GAIN_S_MAX		0x800	// ??
#define OV10640_EXPOSURE_TIME_S_MIN		4	//!! Has to be equal or below L Minimum!!! 
// #define OV10640_EXPOSURE_TIME_S_MAX 	771	// S exposure time is limited by the configured exposure time of L
#define OV10640_ANA_GAIN_S_MIN			0
#define OV10640_ANA_GAIN_S_MAX			0x3	


// Long exposure parameters limits and threshold
#define OV10640_DIGITAL_GAIN_L_MIN		0x150//minimum from AEC of the camera 0x15A	// 0x15A = b1.01011010 = 1.35	Absolute minimum from the camera
#define OV10640_DIGITAL_GAIN_L_MAX		0x800	// (6.16b) Absolute maximum from the camera is 63.99 (0x2FFF) (Probably too high =>satyuration artifact)
#define OV10640_EXPOSURE_TIME_L_MIN		4
#define OV10640_EXPOSURE_TIME_L_MAX 	727	// VTS = Number of frame lines + blank lines
#define OV10640_ANA_GAIN_L_MIN			0
#define OV10640_ANA_GAIN_L_MAX			0x3			

// Very Short exposure parameters limits and threshold
#define OV10640_DIGITAL_GAIN_VS_MIN		0x18F// form the AEc of the camera 0x15A	// 0x15A = b1.01011010 = 1.35	Absolute minimum from the camera
#define OV10640_DIGITAL_GAIN_VS_MAX		0x800	// (6.16b) Absolute maximum from the camera is 63.99 (0x2FFF)(Probably too high =>satyuration artifact)
#define OV10640_EXPOSURE_TIME_VS_MIN	0x10	// Absolute minimum from the camera is 0x10 (0.5 lines)	- 16
#define OV10640_EXPOSURE_TIME_VS_MAX	0x7F	// Absolute maximum from the camera is 0x7f (3.969 lines) - 127
#define OV10640_ANA_GAIN_VS_MIN		0x1		// Asolute maximum is 0x3 (*4)	
#define OV10640_ANA_GAIN_VS_MAX		0x1		// Asolute maximum is 0x3 (*4)	


#define AEC_CAMERA_ON	0
#define AEC_S32V_ON		1
#define AEC_OFF			2


// **************************************************************************
// global variables
// **************************************************************************

  extern OV10640_Exposure_t gOVExposure[4];
  
  extern uint16_t gExpRange;        // read from camera total lines
  extern uint8_t  gParamLEn;        // define which parameters are controlled
  extern uint8_t  gParamVSEn;       // define which parameters are controlled

  extern uint8_t  gExpMode;         // select exposure mode 0:dual, 1:triple
  extern uint8_t  gHDRShift[4];     // 0 for dual, 4 for triple
  //extern uint8_t  gHDRShift1;       // shadow register for HDRShift when mode switch Mode
//   extern uint8_t  gOutLut;          // 0/1 for dual, 2 for triple
  extern float    gFlickerPeriod;   // in lines
  extern uint8_t  gDeflickerEnable; // on / off


  // VS exposure initial parameters
  extern uint16_t gVSExpTime;
  extern uint16_t gVSMinExpTime;
  extern uint16_t gVSMaxExpTime;
  extern uint8_t  gVSCG;
  extern uint8_t  gVSCGEn;
  extern uint8_t  gVSAG;
  extern uint8_t  gVSMinAG;
  extern uint8_t  gVSMaxAG;
  extern uint16_t gVSDG;
  extern uint16_t gVSMinDG;
  extern uint16_t gVSMaxDG;
  // S exposure initial parameters
  extern uint16_t gSExpTime;
  extern uint16_t gSMinExpTime;
  extern uint16_t gSMaxExpTime;
  extern uint8_t  gSCG;
  extern uint8_t  gSCGEn;
  extern uint8_t  gSAG;
  extern uint8_t  gSMinAG;
  extern uint8_t  gSMaxAG;
  extern uint16_t gSDG;
  extern uint16_t gSMinDG;
  extern uint16_t gSMaxDG;
  // L exposure initial parameters
  extern uint16_t gLExpTime;
  extern uint16_t gLMinExpTime;
  extern uint16_t gLMaxExpTime;
  extern uint8_t  gLCG;
  extern uint8_t  gLCGEn;
  extern uint8_t  gLAG;
  extern uint8_t  gLMinAG;
  extern uint8_t  gLMaxAG;
  extern uint16_t gLDG;
  extern uint16_t gLMinDG;
  extern uint16_t gLMaxDG;
  
  extern float gRatioLSExp;
  extern float gRatioMargin;
  extern float gCGThreshold;
  extern float gAGThreshold;
  
  extern uint16_t  gGroupRegisters;

  extern uint16_t gRedTarget;       // red  target value in AWB
  extern uint16_t gBlueTarget;      // blue target value in AWB
  
  extern uint32_t hist_VS[4][16];    // only bin 0 is 24b all the others are 16bits
  extern uint32_t hist_S[4][16];
  extern uint32_t hist_L[4][16];         // only bin 0 is 24b all the others are 16bits
  extern uint32_t hist_pix_cnt[3][4];    // 3 exposure and 4 channels
  extern uint32_t sum_hist[3][4]; 	// Sum of the histograms for each exposure/channel




// **************************************************************************
// macro
// **************************************************************************
#define CAM_RegRead(a,b,c)       OV10640_RegRead(a,b,c)
#define CAM_RegWrite(a,b,c)      OV10640_RegWrite(a,b,c)
#define CAM_N_RegRead(a,b,c,d)   ((d<2)?OV10640_RegRead(a,b,c):0)
#define CAM_N_RegWrite(a,b,c,d)  ((d<2)?OV10640_RegWrite(a,b,c):0)

#ifdef __STANDALONE__

extern "C" {
  unsigned long get_uptime_microS(void);
}
#define msleep(_val)                                    \
  {                                                     \
    unsigned long lStart = get_uptime_microS();         \
    while((get_uptime_microS() - lStart) < _val*1000);  \
  }

#else

#define msleep(_msec)(usleep(_msec*1000))

#endif

// **************************************************************************
// types
// **************************************************************************
enum color_channel 
{
  BLUE=0, 
  GREEN_B=1, 
  GREEN_R=2, 
  RED=3
};
// **************************************************************************
enum gExposure 
{
  VS=0, 	// Very short
  S=1, 		// Short
  L=2		// Longue
};
// **************************************************************************
enum gParam 
{
  Time=0, 
  AG=1, 	//Analogue gain
  DG=2, 	//Digital gain
  CG=3		//Conversion gain
};
// **************************************************************************
// To select to use the exposures paramerters from global structure or to read 
// them from the camera
enum gParamSource 
{
  SourceCamera=0, 
  SourceS32v=1
};


// **************************************************************************
// Functions
// **************************************************************************

/************************************************************************/
/** set HDR IPUS engine index.
  *
  * \param  arHDREngine     IPUS Index for HDR engine to saet HDR Shift
  ************************************************************************/
void SetHDREngine(uint8_t arHDREngine);

/************************************************************************/
/** Saves histogram data for the 3 exposures from the embedded data into 
  * the global histogram table for the selected color channel
  * 
  * \param  ch       	color channel used for which the histogram has been computed
  * \param  stats      pointer to embedded data containing the histogram statistics
  ************************************************************************/
void CamHistGet(color_channel ch, 
		uint8_t* stats);      //  Change the parameters names

/************************************************************************/
/** Print the histogram as well as the total pixel sum for each exposure
  ************************************************************************/
void CamHistPrint();

/************************************************************************/
/** Initialise the exposure parameters structures with the global variables
  *
  * \param  arRatioLS	ratio L/S used to compute S exposure parameters based
  * 			on L configuration.If NULL then the global exposure 
  * 			parameters are used to initialise S.  
  ************************************************************************/
void OV10640_ExposureInit(float arRatioLS);

/************************************************************************/
/** Change L/S ratio. Recompute the total exposure of S and its exposures 
  * parameters
  *
  * \param  arNewRatio		New L/S ratio
  * \param  arLogLevel		print log level
  ************************************************************************/
int ExposureLSRatioChange(float arNewRatio,
			  uint8_t arLogLevel);
//  To add the exposure structure to the parameters and remove unused return!!!!!!!!

/************************************************************************/
/** Print the exposures parameters of the 3 exposures
  *
  * \param  arCsiIdx		MIPI-CSI port to which the camera is connected
  * \param  arSource		source of the printed parameters (global 
  * 				structure or reade the camera configuration)
  ************************************************************************/
void OV10640_Exposure_Print(CSI_IDX arCsiIdx, 
			    gParamSource arSource);

// **************************************************************************
// Validation functions
//
int OV10640_ExposureValidation(OV10640_Exposure_t &arExp,
			       uint32_t arSampleNumber,
			       float arSampleStep,
			       float arBelowMinMargin,
			       float arAboveMaxMargin,
			       uint8_t arUpDown,
			       gExposure exp);

#ifdef __cplusplus 
} //extern "C" { 
#endif

#endif // EXPOSUER_H


