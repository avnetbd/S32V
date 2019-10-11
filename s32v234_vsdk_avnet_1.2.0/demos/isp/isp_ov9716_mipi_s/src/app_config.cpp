/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "app_config.h"
#include "udp264.hpp"
extern "C" {
#include "iniparser.h"
}

#ifdef LOGHIST
#define HISTOGRAM_DATA_FILE "hist_pix_cnt.txt"
#endif

#define INI_FILE_NAME "ov9716_quad.ini"

float_t gKbCcmMatrix[CCM_SETS][9] = // signed values in s1.6 bits
    {{                              // default for config file 0
      1.5f, -0.5f, 0.0f, -0.15f, 1.4f, -0.25f, -0.0f, -0.5f, 1.5f},
     {// default for config file 1
      1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
     {// default for config file 2
      1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
     {// default for config file 3
      1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
     {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
     {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
     {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
     {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
     {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
     {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f}}; // do nothing matrix
float_t gRgb2YuvMatrix[9] = {0.299, 0.587, 0.144, -0.14741, -0.28939, 0.422008, 0.614777, -0.5148, -0.12629};

float_t gCcmCscMatrix[9] = {0.299, 0.587, 0.144, -0.14741, -0.28939, 0.422008, 0.614777, -0.5148, -0.12629};

uint8_t gHDRShift[4] = {4, 0, 0, 4}; // 0 for dual, 4 for friple
//***************************************************************************

#ifndef __STANDALONE__
int32_t IniParse(AppContext_t& arContext)
{
  dictionary* lpIniFile;

  // open the ini file
  lpIniFile = iniparser_load(INI_FILE_NAME);
  if(lpIniFile == NULL)
  {
    printf("Cannot parse ini file: %s\n", INI_FILE_NAME);
    printf("  Application default values will be used.\n");
    return 0;
  } // if ini file not parsed

  printf("Reading default values from ini file %s\n", INI_FILE_NAME);

#if(UDP264 == 1)
  {
    char* lString;
    lString = iniparser_getstring(lpIniFile, "general:UDPTRGTIP", gUdpTargetIpAddress);

    strncpy(gUdpTargetIpAddress, lString, 31);
    gUdpTargetIpAddress[31] = 0;
    lString                 = iniparser_getstring(lpIniFile, "general:UDPTRGTPORT", gUdpTargetPort);

    strncpy(gUdpTargetPort, lString, 31);
    gUdpTargetPort[31] = 0;
    printf("UDP264 target %s:%s\n", gUdpTargetIpAddress, gUdpTargetPort);
  }
#endif // if (UDP264 == 1)

  arContext.mKbAEC        = iniparser_getboolean(lpIniFile, "aec:AECON", arContext.mKbAEC);
  arContext.mExpMode      = iniparser_getint(lpIniFile, "aec:AECMODE", arContext.mExpMode);
  arContext.mKbBlackLevel = iniparser_getint(lpIniFile, "aec:BLACKLEVEL", arContext.mKbBlackLevel);
  arContext.mKbDeFlicker  = iniparser_getint(lpIniFile, "aec:FLICKERMODE", arContext.mKbDeFlicker);

  arContext.mKbLtrgt  = iniparser_getint(lpIniFile, "aec:LTRGT", arContext.mKbLtrgt);
  arContext.mKbVStrgt = iniparser_getint(lpIniFile, "aec:VSTRGT", arContext.mKbVStrgt);
  gHDRShift[0]        = iniparser_getint(lpIniFile, "aec:HDRSHIFT0", gHDRShift[0]);

  gHDRShift[1] = iniparser_getint(lpIniFile, "aec:HDRSHIFT1", gHDRShift[1]);

  gHDRShift[2] = iniparser_getint(lpIniFile, "aec:HDRSHIFT2", gHDRShift[2]);

  gHDRShift[3] = iniparser_getint(lpIniFile, "aec:HDRSHIFT3", gHDRShift[3]);

  arContext.mpOutLut1[0] = iniparser_getint(lpIniFile, "aec:OUTLUT0", arContext.mpOutLut1[0]);

  arContext.mpOutLut1[1] = iniparser_getint(lpIniFile, "aec:OUTLUT1", arContext.mpOutLut1[1]);

  arContext.mpOutLut1[2] = iniparser_getint(lpIniFile, "aec:OUTLUT2", arContext.mpOutLut1[2]);

  arContext.mpOutLut1[3] = iniparser_getint(lpIniFile, "aec:OUTLUT3", arContext.mpOutLut1[3]);

  arContext.mExpIncrement = iniparser_getint(lpIniFile, "aec:INCREMENTEXP", arContext.mExpIncrement);

  arContext.mDgIncrement = iniparser_getint(lpIniFile, "aec:INCREMENTDG", arContext.mDgIncrement);

  // AWB
  arContext.mKbAWB = iniparser_getboolean(lpIniFile, "awb:AWBON", arContext.mKbAWB);
  SetRedTarget((uint16_t)(256.0 * iniparser_getdouble(lpIniFile, "awb:RBALANCE", ((double)GetRedTarget()) / 256.0)));

  SetBlueTarget((uint16_t)(256.0 * iniparser_getdouble(lpIniFile, "awb:BBALANCE", ((double)GetBlueTarget()) / 256.0)));

  arContext.mKbSaturation = iniparser_getdouble(lpIniFile, "awb:SATURATION", arContext.mKbSaturation);

  arContext.mRBIncrement = iniparser_getint(lpIniFile, "awb:INCREMENTRB", arContext.mRBIncrement);

  // CcmMatrix[0]
  gKbCcmMatrix[0][0] = iniparser_getdouble(lpIniFile, "ccm:CCM00", gKbCcmMatrix[0][0]);
  gKbCcmMatrix[0][1] = iniparser_getdouble(lpIniFile, "ccm:CCM01", gKbCcmMatrix[0][1]);
  gKbCcmMatrix[0][2] = iniparser_getdouble(lpIniFile, "ccm:CCM02", gKbCcmMatrix[0][2]);
  gKbCcmMatrix[0][3] = iniparser_getdouble(lpIniFile, "ccm:CCM10", gKbCcmMatrix[0][3]);
  gKbCcmMatrix[0][4] = iniparser_getdouble(lpIniFile, "ccm:CCM11", gKbCcmMatrix[0][4]);
  gKbCcmMatrix[0][5] = iniparser_getdouble(lpIniFile, "ccm:CCM12", gKbCcmMatrix[0][5]);
  gKbCcmMatrix[0][6] = iniparser_getdouble(lpIniFile, "ccm:CCM20", gKbCcmMatrix[0][6]);
  gKbCcmMatrix[0][7] = iniparser_getdouble(lpIniFile, "ccm:CCM21", gKbCcmMatrix[0][7]);
  gKbCcmMatrix[0][8] = iniparser_getdouble(lpIniFile, "ccm:CCM22", gKbCcmMatrix[0][8]);

  // CcmMatrix[1]
  gKbCcmMatrix[1][0] = iniparser_getdouble(lpIniFile, "ccm:CCM1_00", gKbCcmMatrix[1][0]);
  gKbCcmMatrix[1][1] = iniparser_getdouble(lpIniFile, "ccm:CCM1_01", gKbCcmMatrix[1][1]);
  gKbCcmMatrix[1][2] = iniparser_getdouble(lpIniFile, "ccm:CCM1_02", gKbCcmMatrix[1][2]);
  gKbCcmMatrix[1][3] = iniparser_getdouble(lpIniFile, "ccm:CCM1_10", gKbCcmMatrix[1][3]);
  gKbCcmMatrix[1][4] = iniparser_getdouble(lpIniFile, "ccm:CCM1_11", gKbCcmMatrix[1][4]);
  gKbCcmMatrix[1][5] = iniparser_getdouble(lpIniFile, "ccm:CCM1_12", gKbCcmMatrix[1][5]);
  gKbCcmMatrix[1][6] = iniparser_getdouble(lpIniFile, "ccm:CCM1_20", gKbCcmMatrix[1][6]);
  gKbCcmMatrix[1][7] = iniparser_getdouble(lpIniFile, "ccm:CCM1_21", gKbCcmMatrix[1][7]);
  gKbCcmMatrix[1][8] = iniparser_getdouble(lpIniFile, "ccm:CCM1_22", gKbCcmMatrix[1][8]);

  // CcmMatrix[2]
  gKbCcmMatrix[2][0] = iniparser_getdouble(lpIniFile, "ccm:CCM2_00", gKbCcmMatrix[2][0]);
  gKbCcmMatrix[2][1] = iniparser_getdouble(lpIniFile, "ccm:CCM2_01", gKbCcmMatrix[2][1]);
  gKbCcmMatrix[2][2] = iniparser_getdouble(lpIniFile, "ccm:CCM2_02", gKbCcmMatrix[2][2]);
  gKbCcmMatrix[2][3] = iniparser_getdouble(lpIniFile, "ccm:CCM2_10", gKbCcmMatrix[2][3]);
  gKbCcmMatrix[2][4] = iniparser_getdouble(lpIniFile, "ccm:CCM2_11", gKbCcmMatrix[2][4]);
  gKbCcmMatrix[2][5] = iniparser_getdouble(lpIniFile, "ccm:CCM2_12", gKbCcmMatrix[2][5]);
  gKbCcmMatrix[2][6] = iniparser_getdouble(lpIniFile, "ccm:CCM2_20", gKbCcmMatrix[2][6]);
  gKbCcmMatrix[2][7] = iniparser_getdouble(lpIniFile, "ccm:CCM2_21", gKbCcmMatrix[2][7]);
  gKbCcmMatrix[2][8] = iniparser_getdouble(lpIniFile, "ccm:CCM2_22", gKbCcmMatrix[2][8]);
  // CcmMatrix[3]
  gKbCcmMatrix[3][0] = iniparser_getdouble(lpIniFile, "ccm:CCM3_00", gKbCcmMatrix[3][0]);
  gKbCcmMatrix[3][1] = iniparser_getdouble(lpIniFile, "ccm:CCM3_01", gKbCcmMatrix[3][1]);
  gKbCcmMatrix[3][2] = iniparser_getdouble(lpIniFile, "ccm:CCM3_02", gKbCcmMatrix[3][2]);
  gKbCcmMatrix[3][3] = iniparser_getdouble(lpIniFile, "ccm:CCM3_10", gKbCcmMatrix[3][3]);
  gKbCcmMatrix[3][4] = iniparser_getdouble(lpIniFile, "ccm:CCM3_11", gKbCcmMatrix[3][4]);
  gKbCcmMatrix[3][5] = iniparser_getdouble(lpIniFile, "ccm:CCM3_12", gKbCcmMatrix[3][5]);
  gKbCcmMatrix[3][6] = iniparser_getdouble(lpIniFile, "ccm:CCM3_20", gKbCcmMatrix[3][6]);
  gKbCcmMatrix[3][7] = iniparser_getdouble(lpIniFile, "ccm:CCM3_21", gKbCcmMatrix[3][7]);
  gKbCcmMatrix[3][8] = iniparser_getdouble(lpIniFile, "ccm:CCM3_22", gKbCcmMatrix[3][8]);

  // deboise
  arContext.mKbYDenoise = iniparser_getint(lpIniFile, "ynr:THRESHOLD", arContext.mKbYDenoise);

  arContext.mKbEdgeThrLow = iniparser_getint(lpIniFile, "edge:THRLOW", arContext.mKbEdgeThrLow);
  arContext.mKbEdgeMaxA =
      (uint8_t)(64.0 * iniparser_getdouble(lpIniFile, "edge:MAXA", ((double)arContext.mKbEdgeMaxA) / 64.0));

  // ***********************
  // update camera registers
  arContext.mCamFps = iniparser_getdouble(lpIniFile, "cam:FSP", (30.0));

  {
    int  i = 0;
    int  lIic;
    char parname[256];

    while(1)
    {
      snprintf(parname, 255, "cam:IIC%03d", i);
      lIic = iniparser_getint(lpIniFile, parname, 0);
      if(!lIic)
        break;

      CAM_RegWrite(CSI_IDX_CAM, (lIic >> 8) & 0xffff, lIic & 0xff);
      printf("%s : 0x%06x\n", parname, lIic);
      i++;
    }
  } // end read camera parameters
  if(lpIniFile)
  {
    iniparser_freedict(lpIniFile);
  } // if ini exists
  return 0;
} // IniParse()
#endif // ifndef __STANDALONE__
