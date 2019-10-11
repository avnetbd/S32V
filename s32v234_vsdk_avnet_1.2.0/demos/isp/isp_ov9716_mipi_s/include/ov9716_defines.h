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

#ifndef __OV9716_DEFINES_H__
#define __OV9716_DEFINES_H__

// **************************************************************************
// constants
// **************************************************************************

// **** Exposure control algorithm constants
// Exposure times
#define OV9716_EXPOSURE_TIME_LS_MAX             700
#define OV9716_EXPOSURE_TIME_LS_MIN             5
#define OV9716_ANA_GAIN_LS_MAX                  3
#define OV9716_ANA_GAIN_LS_MIN                  0
// Exposure time is 8.5b and the minimum is 2 lines
#define OV9716_EXPOSURE_TIME_VS_MIN             0x40
// Exposure time is 8.5b and the maximum is 2 + 31/32 lines!
#define OV9716_EXPOSURE_TIME_VS_MAX             0x5F
#define OV9716_EXPOSURE_TIME_FRAC_VS_MAX        0x1F
#define OV9716_EXPOSURE_TIME_FRAC_VS_MIN        0

// **** Digital Gain
//0x3FFF is the absolut maximum of sensor (6.8)
#define OV9716_DIGITAL_GAIN_L_MAX               0x800
#define OV9716_DIGITAL_GAIN_S_MAX               0x800
#define OV9716_DIGITAL_GAIN_VS_MAX              0x200
#define OV9716_DIGITAL_GAIN_LS_MAX              0x800
#define OV9716_DIGITAL_GAIN_L_MIN               0x200
#define OV9716_DIGITAL_GAIN_S_MIN               0x200
#define OV9716_DIGITAL_GAIN_VS_MIN              0x100

// **** Analog Gain
#define OV9716_ANA_GAIN_L_MAX                   3
#define OV9716_ANA_GAIN_L_MIN                   0
#define OV9716_ANA_GAIN_S_MAX                   3
#define OV9716_ANA_GAIN_S_MIN                   0
#define OV9716_ANA_GAIN_VS_MAX                  1
#define OV9716_ANA_GAIN_VS_MIN                  0

// **** Conversion Gain (for VS only)
#define OV9716_CONVERSION_GAIN_FACTOR           10.75

// **** AEC algorithm selection
#define OV9716_PARAM_EXPOSURE_TIME_EN           0x1
#define OV9716_PARAM_CONVERSION_GAIN_EN         0x2
#define OV9716_PARAM_ANA_GAIN_EN                0x4
#define OV9716_PARAM_DIG_GAIN_EN                0x8
#define OV9716_PARAM_ALL_EN                     0xF

// **************************************************
// **** OV9716 registers address

#define OV9716_STREAM_ONOFF                     0x3012
#define OV9716_GRP_SEL                          0x3464
#define OV9716_GRP_HOLD                         0x3467
#define OV9716_GRP                              0x8000
#define OV9716_BLACK_LEVEL                      0x3168

// Frame DCG (HCG/LCG) Exposure (in Rows)
#define OV9716_CEXP_DCG_H                       0x30B6
#define OV9716_CEXP_DCG_L                       0x30B7
// Frame VS Exposure
#define OV9716_CEXP_VS_H                        0x30B8
#define OV9716_CEXP_VS_M                        0x30B9
// Fractional part 5bit (0 to 0x1F)
#define OV9716_CEXP_VS_L                        0x30Ba

#define OV9716_VTS                              0x30b2
#define OV9716_VS_DELAY_H                       0x30CE
#define OV9716_VS_DELAY_L                       0x30CF
#define OV9716_DIG_GAIN_HCG_H                   0x3162
#define OV9716_DIG_GAIN_HCG_L                   0x3163
#define OV9716_DIG_GAIN_LCG_H                   0x3164
#define OV9716_DIG_GAIN_LCG_L                   0x3165
#define OV9716_DIG_GAIN_VS_H                    0x3166
#define OV9716_DIG_GAIN_VS_L                    0x3167
#define OV9716_CG_GAIN                          0x30BB
#define OV9716_CHGAIN_L                         0x3360
#define OV9716_CHGAIN_S                         0x3368
#define OV9716_CHGAIN_VS                        0x3370

#define OV9716_GRP_CEXP_DCG_H      (OV9716_CEXP_DCG_H     | OV9716_GRP)
#define OV9716_GRP_CEXP_DCG_L      (OV9716_CEXP_DCG_L     | OV9716_GRP)
#define OV9716_GRP_CEXP_VS_H       (OV9716_CEXP_VS_H      | OV9716_GRP)
#define OV9716_GRP_CEXP_VS_L       (OV9716_CEXP_VS_L      | OV9716_GRP)
#define OV9716_GRP_DIG_GAIN_HCG_H  (OV9716_DIG_GAIN_HCG_H | OV9716_GRP)
#define OV9716_GRP_DIG_GAIN_HCG_L  (OV9716_DIG_GAIN_HCG_L | OV9716_GRP)
#define OV9716_GRP_DIG_GAIN_LCG_H  (OV9716_DIG_GAIN_LCG_H | OV9716_GRP)
#define OV9716_GRP_DIG_GAIN_LCG_L  (OV9716_DIG_GAIN_LCG_L | OV9716_GRP)
#define OV9716_GRP_DIG_GAIN_VS_H   (OV9716_DIG_GAIN_VS_H  | OV9716_GRP)
#define OV9716_GRP_DIG_GAIN_VS_L   (OV9716_DIG_GAIN_VS_L  | OV9716_GRP)
#define OV9716_GRP_CG_GAIN         (OV9716_CG_GAIN        | OV9716_GRP)
#define OV9716_GRP_CHGAIN_L        (OV9716_CHGAIN_L)
#define OV9716_GRP_CHGAIN_S        (OV9716_CHGAIN_S       | OV9716_GRP)
#define OV9716_GRP_CHGAIN_VS       (OV9716_CHGAIN_VS      | OV9716_GRP)

#endif // __OV9716_DEFINES_H__

/* EOF */
 
