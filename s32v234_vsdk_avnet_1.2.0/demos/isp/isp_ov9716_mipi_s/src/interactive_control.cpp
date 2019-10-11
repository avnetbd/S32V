/*
 * Copyright (c) 2016 Freescale Semiconductor
 * Copyright 2017-2018 NXP
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

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

#include "interactive_control.h"
#include "sdi.hpp"          // for getting the types included
#include "isp_cam_maxim.h"  // includes IIC read write functions
#include "isp_cam_ov9716.h" // includes IIC read write functions

#include "global_defines.h"
#include "ov9716_defines.h" // camera register definition

// processing steps
#include "context.h"
#include "app_config.h"
#include "Ipu_coherent_reg.h" // setRegisterCoherent
#include "mymemcpy.h"         // fast Neon memcopy routine
#include "exposure.h"         // functions for exposure control
#include "ctl_tonemap.h"      // tone_mapping with LUT

#include <common_time_measure.h>

// propotypes from main
void    CcmCscMatrix(float_t* apCcmCsCMatrix, float_t* apCcmMatrix, float_t* apCscMatrix, float_t aSaturation);
int32_t IniParse(AppContext_t& arContext);
void    IspConfig(AppContext_t& arContext);

//***************************************************************************
// constants
//***************************************************************************
#define CAM_SAVE_PGM

#ifdef LOGHIST
#define HISTOGRAM_DATA_FILE "hist_pix_cnt.txt"
#endif

// ***** ISP Graph Defines *******

// Possible to set input resolution (must be supported by the DCU)
#define SCR_WIDTH ((DISPLAY_PIXELS + 31) & ~31) ///< width of DDR buffer in pixels
#define SCR_HEIGHT (DISPLAY_LINES)              ///< height of DDR buffer in pixels

// for file saving
#if(ISP_OUTPUT == ISP_OUTPUT_YUV422)
#define EXTRACT_MEMCOPY
#define DCU_OUT_FILE_EXT "uyvy"
#define ISP_CHNL_CNT io::IO_DATA_CH2
#define DCU_TYPE DCU_BPP_YCbCr422
#define DCU_CHNL_CNT io::IO_DATA_CH2
#elif(ISP_OUTPUT == ISP_OUTPUT_YUV420)
#define EXTRACT_YUV2RGB
#define DCU_OUT_FILE_EXT "ppm"
#define ISP_CHNL_CNT io::IO_DATA_CH1
#define DCU_TYPE DCU_BPP_82
#define DCU_CHNL_CNT io::IO_DATA_CH3
//  #define DCU_CHNL_CNT io::IO_DATA_CH1
#endif

// ***** APEX Processing Defines *******
#define EDGETHRDIFF 16

#ifdef ISP_FRAME_RATE_MEASURE_NUM
#define LOOP_NUM ISP_FRAME_RATE_MEASURE_NUM
#else
#define LOOP_NUM 30
#endif

#define IPU_PROF_FRAMES ((LOOP_NUM < 10) ? LOOP_NUM : 10)

//***************************************************************************
// global variables
//***************************************************************************

// ** white balancing
static uint16_t sChGainTab[CHGAIN_SETS][4] = // values in 8.8 bits
    {{400, 256, 256, 400}, {400, 256, 256, 300}, {300, 256, 256, 400},     {256, 000, 000, 000}, {000, 256, 256, 000},
     {000, 000, 000, 256}, {256, 256, 256, 256}, {0x277, 256, 256, 0x1db}, {300, 256, 256, 700}};

static const char*   sExposureName[3] = {"VS", "LCG", "HCG"};
static const char*   sParamName[4]    = {"Time", "AG", "DG", "CG"};
static const uint8_t sExpName[6][9]   = {"DCG_VS", "VS", "LCG", "HCG", "DCG", "HCG_VS"};

//***************************************************************************
// macro
//***************************************************************************

#if defined(__STANDALONE__) && defined(__ARMV8)
extern "C" {
void flush_dcache_range(void*, unsigned int);
}
#endif

//***************************************************************************
// exported functions
//***************************************************************************

//***************************************************************************
// local functions
//***************************************************************************

/************************************************************************/
/** Status logging.
 *
 ************************************************************************/
void LogStatus(AppContext_t& arContext);

/************************************************************************/
/** Save output frame
 * \param  apSaveString   string appended to filename
 *
 * \return 0 if all ok -1 otherwise
 ************************************************************************/
static int32_t FrameOutSave(const uint8_t* apSaveString, AppContext_t& arContext);

/************************************************************************/
/** Save input frame
 * \param  apSaveString   string appended to filename
 *
 * \return 0 if all ok -1 otherwise
 ************************************************************************/
static int32_t FrameInSave(const uint8_t* apSaveString, AppContext_t& arContext);

#ifndef __STANDALONE__
/************************************************************************/
/** Checks if key was pressed or not.
 *
 * \return 0 if no key pressed
 *         >0 if key pressed
 ************************************************************************/
static int KeyDown(void);

//***************************************************************************

static int GetCharNonBlock(void);

//***************************************************************************

//***************************************************************************

#endif // #ifndef __STANDALONE__

//***************************************************************************

static char GetChar(void);

//***************************************************************************

static int32_t FrameOutSave(const uint8_t* apSaveString, AppContext_t& arContext)
{
  int32_t  lRet = 0;
  uint8_t  cam  = 0;
  uint8_t  val, valL, valH, valV;
  uint16_t val16;
  uint32_t val32;
  float    valF;

#ifndef __STANDALONE__
#define EXPPRINT fprintf(lpSavefile,
#else
#define EXPPRINT printf(
#endif
  if(arContext.mKbXoffset == 1280)
  {
    cam = 1;
  }
  else if(arContext.mKbXoffset == 2560)
  {
    cam = 2;
  }
  else if(arContext.mKbXoffset == 3840)
  {
    cam = 3;
  }
  if(arContext.mpFrameFbs != NULL)
  {
#ifndef __STANDALONE__
    FILE* lpSavefile = 0;
    char  lFileName[256];

    if(apSaveString && apSaveString[0])
    {
      snprintf(lFileName, 255, "ov9716_quad_%s.ppm", apSaveString);
    }
    else
    {
      strncpy(lFileName, "ov9716_quad.ppm", 255);
    }
    lpSavefile = fopen(lFileName, "w+");
    if(!lpSavefile)
    {
      printf("Error: could not open save file %s\n", lFileName);
      return -1;
    }

    fprintf(lpSavefile, "P6\n# Maxim Ov9716 quad raw image 16 bit\n");

#endif //#ifndef __STANDALONE__

    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_DCG_H, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_DCG_L, val, (MAXIM_CHNL)(cam + 1));
    val16 += val;
#ifndef __STANDALONE__
    fprintf(lpSavefile, "#cam%d -LS: t=%d", cam, val16);
#else
    printf("#cam%d -LS: t=%d", cam, val16);
#endif
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_VS_H, val, (MAXIM_CHNL)(cam + 1));
    val32 = val << 16;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_VS_M, val, (MAXIM_CHNL)(cam + 1));
    val32 += val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_VS_L, val, (MAXIM_CHNL)(cam + 1));
    valF = (float)(val32 >> 8) + (float)(val) / 32;
    val32 += val;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CG_GAIN, val, (MAXIM_CHNL)(cam + 1));
    valH = (val)&0x3;
    valL = (val >> 2) & 0x3;
    valV = (val >> 4) & 0x3;
#ifndef __STANDALONE__
    fprintf(lpSavefile, " - HCG ag=%dx", 1 << valH);
    fprintf(lpSavefile, " - LCG ag=%dx\n", 1 << valL);
    fprintf(lpSavefile, "#cam%d -VS: t=%f", cam, valF);
    fprintf(lpSavefile, " cg=%d ag=%dx", (val >> 7) & 0x1, valV);
#else
    printf(" - HCG ag=%dx", 1 << valH);
    printf(" - LCG ag=%dx\n", 1 << valL);
    printf("#cam%d -VS: t=%f", cam, valF);
    printf(" cg=%d ag=%dx", (val >> 7) & 0x1, valV);
#endif
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_VS_DELAY_H, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_VS_DELAY_L, val, (MAXIM_CHNL)(cam + 1));
    val16 += val;
#ifndef __STANDALONE__
    fprintf(lpSavefile, " -VS delay: %dlines\n", val16);
#else
    printf(" -VS delay: %dlines\n", val16);
#endif
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_DIG_GAIN_HCG_H, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_DIG_GAIN_HCG_L, val, (MAXIM_CHNL)(cam + 1));
    val16 += val;
#ifndef __STANDALONE__
    fprintf(lpSavefile, "#     -DG: HCG=%f", (float)(val16) / 256);
#else
    printf("#     -DG: HCG=%f", (float)(val16) / 256);
#endif
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_DIG_GAIN_LCG_H, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_DIG_GAIN_LCG_L, val, (MAXIM_CHNL)(cam + 1));
    val16 += val;
#ifndef __STANDALONE__
    fprintf(lpSavefile, " LCG=%f", (float)(val16) / 256);
#else
    printf(" LCG=%f", (float)(val16) / 256);
#endif
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_DIG_GAIN_VS_H, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_DIG_GAIN_VS_L, val, (MAXIM_CHNL)(cam + 1));
    val16 += val;
#ifndef __STANDALONE__
    fprintf(lpSavefile, " VS=%f\n", (float)(val16) / 256);
#else
    printf(" VS=%f\n", (float)(val16) / 256);
#endif
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L + 1, val, (MAXIM_CHNL)(cam + 1));
    val16 |= val;
#ifndef __STANDALONE__
    fprintf(lpSavefile, "#chgain R=%fx", (float)(val16) / 256);
#else
    printf("#chgain R=%fx", (float)(val16) / 256);
#endif
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L + 2, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L + 3, val, (MAXIM_CHNL)(cam + 1));
    val16 |= val;
#ifndef __STANDALONE__
    fprintf(lpSavefile, " G=%fx", (float)(val16) / 256);
#else
    printf(" G=%fx", (float)(val16) / 256);
#endif
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L + 4, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L + 5, val, (MAXIM_CHNL)(cam + 1));
    val16 |= val;
#ifndef __STANDALONE__
    fprintf(lpSavefile, " G=%fx", (float)(val16) / 256);
#else
    printf(" G=%fx", (float)(val16) / 256);
#endif
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L + 6, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L + 7, val, (MAXIM_CHNL)(cam + 1));
    val16 |= val;
#ifndef __STANDALONE__
    fprintf(lpSavefile, " B=%fx\n", (float)(val16) / 256);
#else
    printf(" B=%fx\n", (float)(val16) / 256);
#endif

#ifndef __STANDALONE__
    fprintf(lpSavefile, "%d %d\n255\n", DISPLAY_PIXELS, DISPLAY_LINES);
    fwrite((void*)(uintptr_t)arContext.mpFrameFbs, DISPLAY_PIXELS * DCU_CHNL_CNT, DISPLAY_LINES, lpSavefile);
    printf("saved output images as %s\n", lFileName);
    fclose(lpSavefile);

#else  // ifndef __STANDALONE__

    // copy to debugger readable buffer
    mymemcpy(arContext.mpSaveISP, arContext.mpFrameFbs, DISPLAY_PIXELS * DISPLAY_LINES * DCU_CHNL_CNT);

    printf("data.image eAHB:0x%x %d. %d. /RGB888LE /Stride %d.\n", arContext.mpSaveISP, DISPLAY_PIXELS, DISPLAY_LINES,
           DISPLAY_PIXELS * DCU_CHNL_CNT);
    printf("data.save.binary image.rgb eAHB:0x%x++(%d.)\n", arContext.mpSaveISP,
           DISPLAY_PIXELS * DISPLAY_LINES * DCU_CHNL_CNT);
#endif // __STANDALONE__

  } // if image available
  else
  {
    printf("output image NULL\n");
  } // else from if image available

  return lRet;
} // FrameOutSave()

//***************************************************************************

int32_t FrameInSave(const uint8_t* apSaveString, AppContext_t& arContext)
{
  int32_t  lRet = 0;
  uint8_t  cam  = 0;
  uint8_t  val, valL, valH, valV;
  uint16_t val16;
  uint32_t val32;
  float    valF;

#if(CAM_SAVE == 1)
#ifndef __STANDALONE__
  if(arContext.mpFrameFbs != NULL)
  {
    FILE* lpSavefile = 0;
    char  lFileName[256];

    //printf("O"); fflush(stdout);
    if(apSaveString && apSaveString[0])
    {
      snprintf(lFileName, 255, "ov9716_quad_cam_%s.pgm", apSaveString);
    }
    else
    {
      snprintf(lFileName, 255, "ov9716_quad.pgm");
    }

    lpSavefile = fopen(lFileName, "w+");

    if(!lpSavefile)
    {
      printf("Error: could not open save file for %s\n", lFileName);
      return -1;
    }

    int            j;
    unsigned char* lRawData;

    lRawData = (unsigned char*)arContext.mpFrameCAM;

    OAL_MemoryInvalidate(lRawData, CAM_SAVE_STRIDE16 * CAM_LINES);

    // ** PGM header
    fprintf(lpSavefile, "P5\n# Maxim Ov9716 quad raw image 16 bit\n");

    // print exposure settings as comment
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_DCG_H, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_DCG_L, val, (MAXIM_CHNL)(cam + 1));
    val16 += val;
    fprintf(lpSavefile, "#cam0 - L: t=%d", val16);
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_VS_H, val, (MAXIM_CHNL)(cam + 1));
    val32 = val << 16;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_VS_M, val, (MAXIM_CHNL)(cam + 1));
    val32 += val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CEXP_VS_L, val, (MAXIM_CHNL)(cam + 1));
    valF = (float)(val32 >> 8) + (float)(val) / 32;
    val32 += val;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CG_GAIN, val, (MAXIM_CHNL)(cam + 1));
    valH = (val)&0x3;
    valL = (val >> 2) & 0x3;
    valV = (val >> 4) & 0x3;
    fprintf(lpSavefile, " - HCG ag=%dx", 1 << valH);
    fprintf(lpSavefile, " - LCG ag=%dx", 1 << valL);
    fprintf(lpSavefile, " - VS: t=%f", valF);
    fprintf(lpSavefile, " cg=%d ag=%dx", (val >> 7) & 0x1, valV);
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_VS_DELAY_H, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_VS_DELAY_L, val, (MAXIM_CHNL)(cam + 1));
    val16 += val;
    fprintf(lpSavefile, " delay: %d lines", val);
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_DIG_GAIN_HCG_H, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_DIG_GAIN_HCG_L, val, (MAXIM_CHNL)(cam + 1));
    val16 += val;
    fprintf(lpSavefile, " - DG: HCG=%f", (float)(val16) / 256);
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_DIG_GAIN_LCG_H, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_DIG_GAIN_LCG_L, val, (MAXIM_CHNL)(cam + 1));
    val16 += val;
    fprintf(lpSavefile, " LCG=%f", (float)(val16) / 256);
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_DIG_GAIN_VS_H, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_DIG_GAIN_VS_L, val, (MAXIM_CHNL)(cam + 1));
    val16 += val;
    fprintf(lpSavefile, " VS=%f\n", (float)(val16) / 256);

    // print white balance settings as comment
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L + 1, val, (MAXIM_CHNL)(cam + 1));
    val16 |= val;
    fprintf(lpSavefile, "#chgain R=%fx/256", (float)(val16) / 256);
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L + 2, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L + 3, val, (MAXIM_CHNL)(cam + 1));
    val16 |= val;
    fprintf(lpSavefile, " G=%fx", (float)(val16) / 256);
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L + 4, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L + 5, val, (MAXIM_CHNL)(cam + 1));
    val16 |= val;
    fprintf(lpSavefile, " G=%fx", (float)(val16) / 256);
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L + 6, val, (MAXIM_CHNL)(cam + 1));
    val16 = val << 8;
    CAM_N_RegRead(CSI_IDX_CAM, OV9716_CHGAIN_L + 7, val, (MAXIM_CHNL)(cam + 1));
    val16 |= val;
    fprintf(lpSavefile, " B=%fx\n", (float)(val16) / 256);
    fprintf(lpSavefile, "%d %d\n%d\n", CAM_SAVE_PIXELS, CAM_LINES, (1 << 16) - 1);

    uint8_t* lRawBufferOut = 0;
    lRawBufferOut          = (uint8_t*)malloc(CAM_SAVE_STRIDE16 * CAM_LINES);
    if(lRawBufferOut)
    {
      uint8_t* lRawLineOut = lRawBufferOut;
      uint8_t* lRawLineIn  = lRawData;

      memset(lRawLineOut, 0, CAM_SAVE_STRIDE16);
      for(j = 0; j < CAM_LINES; j++)
      {
        memcpy(lRawLineOut, lRawLineIn, CAM_SAVE_STRIDE16);
        lRawLineOut += CAM_SAVE_STRIDE16;
        lRawLineIn += CAM_SAVE_STRIDE16;
      }
      fwrite(lRawBufferOut, CAM_LINES * CAM_SAVE_STRIDE16, 1, lpSavefile);
      free(lRawBufferOut);
    }
    else
    {
      uint8_t* lRawLineIn = lRawData;

      for(j = 0; j < CAM_LINES; j++)
      {
        fwrite(lRawLineIn, IN_PIXELS * 2, 1, lpSavefile);
        lRawLineIn = lRawLineIn + CAM_SAVE_STRIDE16;
      }
    }
    fclose(lpSavefile);
    printf("saved input image as %s\n", lFileName);
  } // if image available
  else
  {
    printf("input image NULL\n");
  } // else from if image available

#else  // #ifndef __STANDALONE__
  printf("%s(): '%c' Function NOT IMPLEMTED.\n", __func__, arContext.mKbMode);
#endif // #ifndef __STANDALONE__
#endif // if(CAM_SAVE==1)

  return lRet;
} // FrameInSave()

//***************************************************************************

void KeyboardInputProcess(AppContext_t& arContext)
{
  // *** get keyboard input ***
  if(arContext.mKbMode != 'm')
  {
    arContext.mKbKey = GetChar();
    if(arContext.mKbKey)
    {
      KbAction(arContext);
    }
    arContext.mKbKey = 0;

    // *** do logging ***
    LogStatus(arContext);

  } // if mKbMode not 'm'
  else
  {
    arContext.mKbMeasure++;
    switch(arContext.mKbMeasure)
    {
      case 1:
        arContext.mKbEdgeMaxA1 = arContext.mKbEdgeMaxA;
        arContext.mKbEdgeMaxA  = 0; // turn off edge enhancement to reduce noise
        arContext.mKbYuvToRgb  = 0; // A53
        arContext.mKbAEC       = 0;
        arContext.mKbAWB       = 0;
        arContext.mKbMode      = 0;
        arContext.mKbMode      = 'M';
        arContext.mKbKey       = '0';
        KbAction(arContext);
        arContext.mKbMode = 'm';
        return;
      case 5: // dump IIC registers
        arContext.mKbMode = 'D';
        arContext.mKbKey  = 'D';
        KbAction(arContext);
        arContext.mKbMode = 'm';
        return;
      case 10: // DCG + VS
        FrameOutSave(sExpName[arContext.mKbExpSel], arContext);
        FrameInSave(sExpName[arContext.mKbExpSel], arContext);
        arContext.mKbMode = 'm';
        return;
      case 15: // DCG
        arContext.mKbMode = 'M';
        arContext.mKbKey  = '1';
        KbAction(arContext);
        arContext.mKbMode = 'm';
        return;
      case 20:
        FrameOutSave(sExpName[arContext.mKbExpSel], arContext);
        arContext.mKbMode = 'm';
        return;
      case 25: // HGC
        arContext.mKbMode = 'M';
        arContext.mKbKey  = '2';
        KbAction(arContext);
        arContext.mKbMode = 'm';
        return;
      case 30:
        FrameOutSave(sExpName[arContext.mKbExpSel], arContext);
        FrameInSave(sExpName[arContext.mKbExpSel], arContext);
        arContext.mKbMode = 'm';
        return;
      case 35: // HGC + VS
        arContext.mKbMode = 'M';
        arContext.mKbKey  = '3';
        KbAction(arContext);
        arContext.mKbMode = 'm';
        return;
      case 40:
        FrameOutSave(sExpName[arContext.mKbExpSel], arContext);
        arContext.mKbMode = 'm';
        return;
      case 45: // VS only
        arContext.mKbMode = 'K';
        arContext.mKbKey  = '1';
        KbAction(arContext);
        arContext.mOutLut = 1;
        arContext.mKbMode = 'm';
        return;
      case 50:
        FrameOutSave(sExpName[arContext.mKbExpSel], arContext);
        arContext.mKbMode = 'm';
        return;
      case 55: // LCG
        arContext.mKbMode = 'K';
        arContext.mKbKey  = '2';
        KbAction(arContext);
        arContext.mKbMode = 'm';
        return;
      case 60:
        FrameOutSave(sExpName[arContext.mKbExpSel], arContext);
        FrameInSave(sExpName[arContext.mKbExpSel], arContext);
        arContext.mKbMode = 'm';
        return;
      case 65:
        printf("Measurment stopped\n");
        arContext.mKbMode = 'M';
        arContext.mKbKey  = '0';
        KbAction(arContext);
        arContext.mKbYuvToRgb = 1; // Neon
        arContext.mKbMode     = 'm';
        return;
      case 70:
        arContext.mKbAEC      = 1;
        arContext.mKbAWB      = 1;
        arContext.mKbMode     = 0;
        arContext.mKbMeasure  = 0;
        arContext.mKbEdgeMaxA = arContext.mKbEdgeMaxA1; // restore edge enhancement
        break;
    } // switch mKbMeasure
  }   // else from if mKbMode not 'm'
} // KeyboardInputProcess()

//***************************************************************************

void LogStatus(AppContext_t& arContext)
{
  if(arContext.mKbLog == 0)
  {
    return;
  }

  // **** loging per frame

  // AEC results
  if(arContext.mKbLog == 3)
  {
    if(arContext.mKbAEC)
    {
      MyOV9716_Exposure_Print(CSI_IDX_CAM, SourceS32v);
    }
    return;
  }

  // AWB results
  if(arContext.mKbLog == 4)
  {
    if(arContext.mKbAWB == 1)
    {
      MyOV9716_Chgain_Print(CSI_IDX_CAM, SourceS32v);
    }
    return;
  }

  // AEC statistics
  if(arContext.mKbLog == 6)
  {
    return;
  }

  // AWB statistics
  if(arContext.mKbLog == 7)
  {
    return;
  }

  // **** loging per second
  if(arContext.mFrmCnt % LOOP_NUM != (LOOP_NUM - 1))
  {
    return;
  }

  GETTIME(&arContext.mTimee);
  arContext.mTimed           = arContext.mTimee - arContext.mTimes;
  arContext.mTimes           = arContext.mTimee;
  arContext.mFrameIRQCnt     = arContext.mFrmCnt;
  arContext.mFrameIRQDiff    = arContext.mFrameIRQCnt - arContext.mFrameIRQCntLast;
  arContext.mFrameIRQCntLast = arContext.mFrameIRQCnt;

  if(arContext.mKbLog == 1)
  {
    printf("Frame: %7d (Isp.GetFrame:%d)\n", arContext.mFrameIRQCnt, arContext.mFrmCnt);

    printf("%d frames took %d usec (%5.2ffps)\n", arContext.mFrameIRQDiff, arContext.mTimed,
           (arContext.mFrameIRQDiff) * 1000000.0 / ((float_t)arContext.mTimed));
    return;
  }
  if(arContext.mKbLog == 2)
  {
    printf("Frame: %7d (Isp.GetFrame:%d)\n", arContext.mFrameIRQCnt, arContext.mFrmCnt);

    ISP_DumpPerfCounters(IPU_PROF_FRAMES);
    SEQ_IpuPerformanceInfoRequest(IPU_PROF_FRAMES);
    if(arContext.mTimeCSC)
    {
      printf("YUV2RGB   took %9.2f      ms/frame; %6.2ffps\n",
             (float_t)arContext.mTimeCSC / (float_t)arContext.mFrameIRQDiff / 1000.0,
             1000000.0 * (float_t)arContext.mFrameIRQDiff / (float_t)arContext.mTimeCSC);
      arContext.mTimeCSC = 0;
    }
    if(arContext.mTimed && arContext.mTimeA53)
    {
      printf("A53 total took %9.2f      ms/frame; %6.2ffps\n",
             (float_t)(arContext.mTimed - arContext.mTimeA53) / (float_t)arContext.mFrameIRQDiff / 1000.0,
             1000000.0 * (float_t)arContext.mFrameIRQDiff / (float_t)(arContext.mTimed - arContext.mTimeA53));
      arContext.mTimeA53 = 0;
    }
    if(arContext.mTimed)
    {
      printf("A53 + Pop took %9.2f      ms/frame; %6.2ffps\n",
             (float_t)arContext.mTimed / (float_t)arContext.mFrameIRQDiff / 1000.0,
             1000000.0 * (float_t)arContext.mFrameIRQDiff / (float_t)arContext.mTimed);
      arContext.mTimeA53 = 0;
    }
    return;
  } // (if arContext.mKbLog ==2)
  if(arContext.mKbLog == 5)
  {
    SEQ_Head_Ptr_t* lpFetchedGraph = NULL;
    SEQ_Buf_t**     lppFetchedBufferList;

    lppFetchedBufferList = arContext.mpGrabber->GraphFetch(&lpFetchedGraph);
    ISP_PrintGraphStat(lppFetchedBufferList);
    return;
  } // (if arContext.mKbLog ==5)
  if(arContext.mKbLog == 8)
  {
    if(arContext.mTimeApex)
    {
      printf("APEX      took %9.2f      ms/frame; %6.2ffps\n",
             (float_t)arContext.mTimeApex / (float_t)arContext.mFrameIRQDiff / 1000.0,
             1000000.0 * (float_t)arContext.mFrameIRQDiff / (float_t)arContext.mTimeApex);
      arContext.mTimeApex = 0;
    }
    return;
  } // (if arContext.mKbLog == 8)

} // LogStatus()

#ifndef __STANDALONE__

//***************************************************************************

static int KeyDown()
{
  struct timeval lTv;
  fd_set         lFdSet;
  lTv.tv_sec  = 0;
  lTv.tv_usec = 50;
  FD_ZERO(&lFdSet);
  FD_SET(STDIN_FILENO, &lFdSet);
  select(STDIN_FILENO + 1, &lFdSet, NULL, NULL, &lTv);
  return FD_ISSET(STDIN_FILENO, &lFdSet);
} // KeyDown()

//***************************************************************************

static int GetCharNonBlock()
{
  int lChar = EOF;

  usleep(1);
  if(KeyDown())
  {
    lChar = fgetc(stdin);
  } // if Key pressed

  return lChar;
} // KeyDown()

//***************************************************************************

void TermNonBlockSet()
{
  struct termios lTermState;

  // get current state of the terminal
  tcgetattr(STDIN_FILENO, &lTermState);

  // disable canonical mode
  lTermState.c_lflag &= ~ICANON;
  // set minimum number of input characters to read
  lTermState.c_cc[VMIN] = 1;

  // set the terminal attributes
  tcsetattr(STDIN_FILENO, TCSANOW, &lTermState);
} // TermNonBlockSet()

//***************************************************************************

void TermBlockSet()
{
  struct termios lTermState;

  // get current state of the terminal
  tcgetattr(STDIN_FILENO, &lTermState);

  //enable canonical mode
  lTermState.c_lflag |= ICANON;

  // set the terminal attributes
  tcsetattr(STDIN_FILENO, TCSANOW, &lTermState);
} // TermBlockSet()

#endif // #ifndef __STANDALONE__

//***************************************************************************

static char GetChar()
{
#ifdef __STANDALONE__
  static uint8_t lSciDetected = 0;
  static uint8_t lSciSelected = 1;
  char           lChar;
  if(lSciSelected)
  {
    lChar = sci_1_testchar();
  }
  else
  {
    lChar = sci_0_testchar();
  }
  if(lChar)
  {
    lSciDetected = 1;
    return lChar;
  }
  else if(!lSciDetected)
  {
    lSciSelected ^= 1;
  }
  return lChar;
#else  // #ifdef __STANDALONE__
  int lChar = GetCharNonBlock();
  return (char)((lChar < 0) ? 0 : lChar & 0xff);
#endif // else from #ifdef __STANDALONE__
} // GetChar()

//***************************************************************************

int32_t KbAction(AppContext_t& arContext)
{
  static char lPrintMode[MAXLOG + 1][12] = {"off",         "fps",     "ipu prof", "aec",      "awb",
                                            "buffer size", "exphist", "awb stat", "APEX prof"};

  if(arContext.mKbKey == 'h')
  { // help
    printf(
        "\nkeyboard help:\n"
        "\n*** Generic commands\n"
        "h .........help\n"
#ifndef __STANDALONE__
        "X..........reload init file\n"
#endif
        "c .........toggle cursor (w/a/s/[y or z])\n"
        "d .........toggle display on off\n"
        "p .........change logging print mode\n"
        "e .........toggle engine to set parameter (0-11)\n"
        "g .........toggle GPR register to write parameter (0-15/31)\n"
        "v .........parameter value at Engine E Register G (0-f,+,-,w,r)\n"
        "i .........enter IIC command mode\n"
        "  aaaavv ..enter IIC command to 0xaaaa with value 0xvv\n"
        "  w/ret ...write to IIC\n"
        "  r .......read from IIC\n"
        "D .........Camera I2C dump to the console. This kills streaming\n"
        "t .........toggle test bar\n"
        //"o .........camera/offline image input switch\n"
        //"l .........load file from SD card for offline image input\n"
        "m .........measure camera using multiple settings\n"
        "s .........save output image to file\n"
#if(CAM_SAVE == 1)
        "S .........Save input  image to file\n"
#endif //if(CAM_SAVE==1)
        "x .........enter display X-offset mode (+/-)\n"
        "\n*** Input control commands\n"
        "I .........toggle input LUT\n"
        "b .........select black level mode (+/-)\n"
        "\n*** Exposure control commands\n"
        "A .........toggle AEC on off (+/-/2/3/4)\n"
        "  +........increase AEC update factor of 0.05 AEC\n"
        "  -........decrease AEC update factor of 0.05 AEC\n"
        "  2/3/4....Change the AEC period to 2/3/4 frames\n"
        "M .........toggle exposure mode 0:outdoor, 1:indoor, 2:dark,"
        "3: dark+headlamp\n"
        "L .........select long exposure target brightness mode(+/-)\n"
        "V .........select very short exposure target brightness mode\n"
        "  C .......toggle conversion gain enable\n"
        //"view VSCG  var.addwatch *(char*)0x%p\n"
        "r .........changes exposure ratio between HCG and LCG off\n"
        "  +........increase ratio by 5percent\n"
        "  -........decrease ratio by 5percent\n"
        "J .........manual exposure setting\n"
        "  ELSV.....exposure  to change\n"
        "  PTADC....parameter to change\n"
        "  aaaa ....enter the new parameter value\n"
        "  w/ret ...update the parameter in the camera configuration\n"
        "  + .......increase the parameter and update it directly\n"
        "  - .......decrease the parameter and update it directly\n"
        "K .........select exposure mode (0/1/2/3/4/5)\n"
        //"k .........show selected alphas (0/1/2/3) in HDR merge\n"
        "H .........toggle HDR shift for Long+Short\n"
        "f .........toggle deflicker (off/50Hz/60Hz)\n"
        "\n*** White balance control commands\n"
        "W .........toggle AWB on off\n"
        "G .........toggle channel gain\n"
        "T .........select color saturation mode (+/-)\n"
        "R .........select red  target mode (+/-)\n"
        "B .........select blue target mode (+/-)\n"
        "\n*** Tone Mapping\n"
        "O .........toggle output LUT\n"
        "\n*** CCM control\n"
        "C .........toggle CCM table\n"
        "\n*** Denoising control commands\n"
        "Y .........set Y-denoising mode (+/-/*/E)\n"
        "  0 .......off level\n"
        "  * .......on level\n"
        "  E .......toggle edge view in denoising\n"
        //"U .........set UV-denoising mode (+/-)\n"
        "\n*** YUV420 to RGB control commands ***\n"
        "y .........toggle mode between A53 and Neon\n");
    arContext.mKbMode = 'h';
    return 0;
  } // if help requested

  uint8_t lUpperKey = toupper(arContext.mKbKey);
  uint8_t lNumKey   = arContext.mKbKey - '0';
  uint8_t lHexKey;
  lHexKey = lNumKey;
  if(!(lHexKey <= 9))
  {
    lHexKey = lUpperKey - 'A' + 10;
  }

  //check if we are in an active mode
  if(arContext.mKbMode == 'c')
  {
    uint8_t lSet = 0;

    if(arContext.mKbKey == 'c')
    {
      arContext.mKbCursorOn ^= 1;
      printf("CursorMode %s\n", (arContext.mKbCursorOn) ? "on (a,w,s,y/z, r)" : "off");
      printf("Cursor (%d,%d)\n", arContext.mKbCursorX, arContext.mKbCursorY);
      return 0;
    } // if d pressed

    if(lUpperKey == 'W')
    { // up
      if(arContext.mKbCursorY)
        arContext.mKbCursorY--;
      lSet = 1;
    }
    else if(strchr("YZ", lUpperKey))
    { // down
      if(arContext.mKbCursorY < 720)
        arContext.mKbCursorY++;
      lSet = 1;
    }
    else if(lUpperKey == 'A')
    { // right
      if(arContext.mKbCursorX)
        arContext.mKbCursorX--;
      lSet = 1;
    }
    else if(lUpperKey == 'S')
    { // left
      if(arContext.mKbCursorX < 1280)
        arContext.mKbCursorX++;
      lSet = 1;
    }
    else if(lUpperKey == 'R')
    { // left
      lSet = 1;
    }
    if(lSet)
    {
      printf("Cursor (%d,%d)", arContext.mKbCursorX, arContext.mKbCursorY);
      //if(lUpperKey=='R') printf("=%d\n",gKbCursorVal);
      //else
      printf("\n");
      return 0;
    }
  }

  if(arContext.mKbMode == 'p')
  {
    uint8_t lSet = 0;

    if(lHexKey <= MAXLOG)
    {
      arContext.mKbLog = lHexKey;
      lSet             = 1;
    }
    else if(arContext.mKbKey == 'p')
    {
      arContext.mKbLog++;
      if(arContext.mKbLog > MAXLOG)
        arContext.mKbLog = 0;
      lSet = 1;
    }
    if(lSet)
    {
      printf("Statistics Print = %s\n", lPrintMode[arContext.mKbLog]);
      return 0;
    }
  }

  if(arContext.mKbMode == 'e')
  {
    uint8_t lSet = 0;
    if(lUpperKey == 'E')
    {
      arContext.mKbEngine++;
      if(arContext.mKbEngine > 0xb)
        arContext.mKbEngine = 0;
      lSet = 1;
    }
    else if(lHexKey < 12)
    {
      arContext.mKbEngine = lHexKey;
      lSet                = 1;
    }
    if(lSet)
    {
      if(arContext.mKbEngine < 8)
        arContext.mKbGPR &= 0xf;
      else
        arContext.mKbGPR &= 0x1f;
      printf("Parameter Engine = %d\n", arContext.mKbEngine);
      return 0;
    }
  }

  if(arContext.mKbMode == 'g')
  {
    uint8_t lSet = 0;
    if(lUpperKey == 'G')
    {
      arContext.mKbGPR++;
      lSet = 1;
    }
    else if(lHexKey < 32)
    {
      arContext.mKbGPR = lHexKey;
      lSet             = 1;
    }
    if(lSet)
    {
      if(arContext.mKbEngine < 8)
        arContext.mKbGPR &= 0xf;
      else
        arContext.mKbGPR &= 0x1f;
      printf("Parameter Engine = %d GPR%d\n", arContext.mKbEngine, arContext.mKbGPR);
      return 0;
    }
  } // if mode g

  if(arContext.mKbMode == 'v')
  {
    if(lHexKey <= 15)
    { // enter parameter
      arContext.mKbParVal <<= 4;
      arContext.mKbParVal += lHexKey;
      arContext.mKbParVal &= 0xffff;
      printf("Engine %d GPR%d: 0x%x\n", arContext.mKbEngine, arContext.mKbGPR, arContext.mKbParVal);
      return 0;
    }
    if(arContext.mKbKey == '+')
    { // enter parameter
      arContext.mKbParVal += 16;
      arContext.mKbParVal &= 0xffff;
      printf("Engine %d GPR%d: 0x%x\n", arContext.mKbEngine, arContext.mKbGPR, arContext.mKbParVal);
      lUpperKey = 'W';
    }
    if(arContext.mKbKey == '-')
    { // enter parameter
      arContext.mKbParVal -= 16;
      arContext.mKbParVal &= 0xffff;
      printf("Engine %d GPR%d: 0x%x\n", arContext.mKbEngine, arContext.mKbGPR, arContext.mKbParVal);
      lUpperKey = 'W';
    }
    if(strrchr("W\n\r", lUpperKey))
    {
      printf("Writing IPU%c%d GPR%d: 0x%x\n", (arContext.mKbEngine & 0x8) ? 'V' : 'S', arContext.mKbEngine & 0x7,
             arContext.mKbGPR, arContext.mKbParVal);
      setRegCoherent(((arContext.mKbEngine >> 3) & 0x1) ^ 0x1, (arContext.mKbEngine & 0x7), 0,
                     ((arContext.mKbEngine & 0x8) ? 0x50 : 0x70) + arContext.mKbGPR, arContext.mKbParVal);
      return 0;
    }
    if(lUpperKey == 'R')
    {
      printf("Reading IPU%c%d GPR%d: ", (arContext.mKbEngine & 0x8) ? 'V' : 'S', arContext.mKbEngine & 0x7,
             arContext.mKbGPR);
      {
        seq_getReg(((arContext.mKbEngine >> 3) & 0x1) ^ 0x1, (arContext.mKbEngine & 0x7), 0,
                   ((arContext.mKbEngine & 0x8) ? 0x50 : 0x70) + arContext.mKbGPR, &arContext.mKbParVal);
        printf("0x%x / %d\n", arContext.mKbParVal, arContext.mKbParVal);
      }
      return 0;
    }
  } // if mode v

  if(arContext.mKbMode == 'i')
  {
    if(lHexKey <= 15)
    { // enter IIC command
      arContext.mKbI2c <<= 4;
      arContext.mKbI2c += lHexKey;
      arContext.mKbI2c &= 0xffffff;
      printf("IIC command 0x%x - 0x%x\n", (arContext.mKbI2c >> 8), (arContext.mKbI2c & 0xff));
      return 0;
    } // if key <0;9>

    if(strrchr("\n\rwW", arContext.mKbKey))
    {
      printf("Writing to IIC\n");
      CAM_RegWrite(CSI_IDX_CAM, arContext.mKbI2c >> 8, arContext.mKbI2c & 0xff);
      return 0;
    } // if key w
    if(lUpperKey == 'R')
    {
      printf("Read from IIC\n");

      uint8_t val;
      CAM_RegRead(CSI_IDX_CAM, (arContext.mKbI2c >> 8) & 0xffff, val);
      printf("CSI%d %x = 0x%x / %d\n", CSI_IDX_CAM, arContext.mKbI2c >> 8, val, val);

      return 0;
    } // if key r
  }   // if mode i

  if(arContext.mKbMode == 'x')
  {
#define X_PAN_STEPSIZE 80
    uint8_t lSet = 0;
    if(arContext.mKbKey == '+')
    { // show combined
      if(arContext.mKbXoffset <= (OUT_PIXELS - DISPLAY_PIXELS - X_PAN_STEPSIZE))
      {
        arContext.mKbXoffset += X_PAN_STEPSIZE;
      } // if offset <= in range
      lSet = 1;
    } // if key +
    else if(arContext.mKbKey == '-')
    {
      if(arContext.mKbXoffset >= X_PAN_STEPSIZE)
      {
        arContext.mKbXoffset -= X_PAN_STEPSIZE;
      } // if offset >= X_PAN_STEPSIZE
      lSet = 1;
    } // if key -
    else if(lNumKey < 4)
    {
      arContext.mKbXoffset = lNumKey * DISPLAY_PIXELS;
      lSet                 = 1;
    } // if key r

    if(lSet)
    {
      printf("Display X-Offset =%d\n", arContext.mKbXoffset);
      return 0;
    }
  } // if mode x

  if(arContext.mKbMode == 'I')
  {
    uint8_t lSet = 0;
    if(lNumKey < INLUT_SETS)
    {
      arContext.mInLut = lNumKey;
      lSet             = 1;
    }
    else if(arContext.mKbKey == 'I')
    {
      arContext.mInLut++;
      if(arContext.mInLut >= INLUT_SETS)
      {
        arContext.mInLut = 0;
      }
      lSet = 1;
    }
    if(lSet)
    {
      // Shift VS  GPR15
      setRegCoherent(INLUT_ENGINE, 0, 0x7f, arContext.mInLut);
      printf("Input  Lut %d\n", arContext.mInLut);

      return 0;
    }
  } // if mode I

  if(arContext.mKbMode == 'b')
  {
    uint8_t lSet = 0;
    if(arContext.mKbKey == '+')
    {
      if(arContext.mKbBlackLevel < 255)
      {
        arContext.mKbBlackLevel++;
      } // if offset <= in range
      lSet = 1;
    } // if key +
    else if(arContext.mKbKey == '-')
    {
      if(arContext.mKbBlackLevel)
      {
        arContext.mKbBlackLevel--;
      } // if offset >= X_PAN_STEPSIZE
      lSet = 1;
    } // if key -

    if(lSet)
    {
      printf("Black level =%d\n", arContext.mKbBlackLevel);
      CAM_RegWrite(CSI_IDX_CAM, 0x3168, 0x00);
      CAM_RegWrite(CSI_IDX_CAM, 0x3169, arContext.mKbBlackLevel);
      CAM_RegWrite(CSI_IDX_CAM, 0x316a, 0x00);
      CAM_RegWrite(CSI_IDX_CAM, 0x316b, arContext.mKbBlackLevel);
      CAM_RegWrite(CSI_IDX_CAM, 0x316c, 0x00);
      CAM_RegWrite(CSI_IDX_CAM, 0x316d, arContext.mKbBlackLevel);
      return 0;
    }
  } // if mode b

  if(arContext.mKbMode == 'A')
  {
    uint8_t lCam;
    float   aUpdate;
    if(arContext.mKbKey == 'A')
    {
      arContext.mKbAEC ^= 1;
      printf("AEC %s\n", (arContext.mKbAEC) ? "on" : "off");
      return 0;
    }
    if(arContext.mKbKey == '+')
    {
      for(lCam = 0; lCam < 4; lCam++)
      {
        aUpdate = gOVExposure[lCam].mAECFactor + 0.05;
        if(aUpdate <= 0.51)
        {
          gOVExposure[lCam].mAECFactor = aUpdate;
        }
      }
      printf("New Update factor: %f\n", gOVExposure[0].mAECFactor);
      return 0;
    }
    if(arContext.mKbKey == '-')
    {
      for(lCam = 0; lCam < 4; lCam++)
      {
        aUpdate = gOVExposure[lCam].mAECFactor - 0.05;
        if(aUpdate >= 0.05)
        {
          gOVExposure[lCam].mAECFactor = aUpdate;
        }
      }
      printf("New Update factor: %f\n", gOVExposure[0].mAECFactor);
      return 0;
    }
    if((lNumKey >= 2) && (lNumKey <= 4))
    {
      SetAECPeriod(lNumKey);
      printf("AEC period is: %d\n", lNumKey);
      return 0;
    }
  } //'A' mode

  if(arContext.mKbMode == 'M')
  {
    uint8_t lSet = 0;

    // rotate with M key
    if(arContext.mKbKey == 'M')
    {
      if(arContext.mExpMode >= 3)
      {
        arContext.mKbKey = '0';
      }
      else
      {
        arContext.mKbKey = '0' + (arContext.mExpMode++);
      }
      lSet = 1;
    }
    // direct dial mode from 0-3
    else if(lNumKey <= 3)
    {
      arContext.mExpMode = lNumKey;
      lSet               = 1;
    }

    if(lSet)
    {
      arContext.mKbAEC = 0;
      if(arContext.mExpMode == 0)
      {
        printf("HDR ");
        arContext.mKbMode = 'I';
        arContext.mKbKey  = '1';
        KbAction(arContext);
        //arContext.mHDRShift=arContext.mHDRShift1;
        arContext.mKbMode = 'K';
        arContext.mKbKey  = '0';
        KbAction(arContext);
      }
      else if(arContext.mExpMode == 1)
      {
        printf("WDR ");
        arContext.mKbMode = 'I';
        arContext.mKbKey  = '1';
        KbAction(arContext);
        arContext.mKbMode = 'K';
        arContext.mKbKey  = '4';
        KbAction(arContext);
      }
      else if(arContext.mExpMode == 2)
      {
        printf("Single\n");
        arContext.mKbMode = 'I';
        arContext.mKbKey  = '0';
        KbAction(arContext);
        arContext.mKbMode = 'K';
        arContext.mKbKey  = '3';
        KbAction(arContext);
      }
      else if(arContext.mExpMode == 3)
      {
        printf("WDR ");
        printf("Single\n");
        arContext.mKbMode = 'I';
        arContext.mKbKey  = '0';
        //arContext.mHDRShift=arContext.mHDRShift1;
        arContext.mKbMode = 'K';
        arContext.mKbKey  = '5';
        KbAction(arContext);
      }
      arContext.mKbMode = 'M';
      return 0;
    } // if(lSet)
  }   // if M pressed

  if(arContext.mKbMode == 'L')
  {
    uint8_t lSet = 0;

    if(arContext.mKbAEC)
    {
      if(arContext.mKbKey == '+') // enter parameter
      {
        if(arContext.mKbLtrgt < 240)
          arContext.mKbLtrgt += 16;
        lSet = 1;
      }
      if(arContext.mKbKey == '-') // enter parameter
      {
        if(arContext.mKbLtrgt > 31)
          arContext.mKbLtrgt -= 16;
        lSet = 1;
      }
      if(lSet) // make parameter valid
      {
        printf("long target= %d/256\n", arContext.mKbLtrgt);
        return 0;
      }
    }
    else
    {
      float_t lLSUpdate;
      lLSUpdate = 1.0;

      if(arContext.mKbKey == '+') // enter parameter
      {
        lLSUpdate = 1.1;
      }
      if(arContext.mKbKey == '-') // enter parameter
      {
        lLSUpdate = 0.91;
      }
      if(lLSUpdate != 1.0)
      {
        MyOV9716_ExposureSet(CSI_IDX_CAM, MAXIM_CHNL_ORIG_BC, lLSUpdate, 1.0);
        MyOV9716_Exposure_Print(CSI_IDX_CAM, SourceCamera);
        return 0;
      }
    }
  } // if 'L' pressed

  if(arContext.mKbMode == 'V')
  {
    if(arContext.mKbKey == 'C') // toggle conversion gain enable
    {
      char    lOnOff[3][5] = {"on", "off", "auto"};
      uint8_t lNextMode    = 0;
      uint8_t lCurrentMode = GetConverGainMode();
      lNextMode            = lCurrentMode + 1;
      if(lNextMode > 2)
      {
        lNextMode = 0;
      }
      SetConverGainMode(lNextMode);
      printf("VS conversion gain control %s\n", lOnOff[lNextMode]);
      return 0;
    }

    uint8_t lSet = 0;

    if(arContext.mKbAEC)
    {
      if(arContext.mKbKey == '+') // enter parameter
      {
        if(arContext.mKbVStrgt < 248)
          arContext.mKbVStrgt += 4;
        lSet = 1;
      }
      if(arContext.mKbKey == '-') // enter parameter
      {
        if(arContext.mKbVStrgt > 4)
          arContext.mKbVStrgt -= 4;
        lSet = 1;
      }
      if(lSet)
      {
        printf("very short target= %d/256\n", arContext.mKbVStrgt);
        return 0;
      }
    }
    else
    {
      float_t lVSUpdate;
      lVSUpdate = 1.0;

      if(arContext.mKbKey == '+') // enter parameter
      {
        lVSUpdate = 1.1;
        lSet      = 1;
      }
      if(arContext.mKbKey == '-') // enter parameter
      {
        lVSUpdate = 0.91;
        lSet      = 1;
      }
      if(lSet)
      {
        MyOV9716_ExposureSet(CSI_IDX_CAM, MAXIM_CHNL_ORIG_BC, 1.0, lVSUpdate);
        MyOV9716_Exposure_Print(CSI_IDX_CAM, SourceCamera);
        return 0;
      }
      if(arContext.mKbKey == 'i') // enter parameter
      {
        uint8_t val;
        CAM_RegRead(CSI_IDX_CAM, 0x30cf, val);
        printf("VS delay %d (%s)\n", val, (val == 3) ? "OK" : "should be 3");
        return 0;
      }
    }
  } // if 'V' pressed

  if(arContext.mKbMode == 'r')
  {
    if(arContext.mKbKey == '+')
    {
      arContext.mRatio = gOVExposure[0].mRatioHLExp * 1.05;
      MyOV9716_ExposureDCGRatioChange(arContext.mRatio);
      printf("New Ratio is %f\n", arContext.mRatio);
      return 0;
    }
    if(arContext.mKbKey == '-')
    {
      arContext.mRatio = gOVExposure[0].mRatioHLExp * 0.95;
      MyOV9716_ExposureDCGRatioChange(arContext.mRatio);
      printf("New Ratio is %f\n", arContext.mRatio);
      return 0;
    }
  } // if 'r' pressed

  if(arContext.mKbMode == 'J')
  {
    uint8_t lModeChanged = 0;

    if(lUpperKey == 'E')
    {
      arContext.mKbExpToChange = (OV9716_ExposureType)(((uint8_t)arContext.mKbExpToChange - 1) & 3);
      lModeChanged             = 1;
    }
    if(lUpperKey == 'L')
    {
      arContext.mKbExpToChange = OV9716_ExpType_HCG;
      lModeChanged             = 1;
    }
    if(lUpperKey == 'S')
    {
      arContext.mKbExpToChange = OV9716_ExpType_LCG;
      lModeChanged             = 1;
    }
    if(lUpperKey == 'V')
    {
      arContext.mKbExpToChange = OV9716_ExpType_VS;
      lModeChanged             = 1;
    }
    if(lUpperKey == 'P')
    {
      arContext.mKbExpParamChange = (gParam)(((uint8_t)arContext.mKbExpParamChange + 1) % 4);
      lModeChanged                = 1;
    }
    if(lUpperKey == 'T')
    {
      arContext.mKbExpParamChange = Time;
      lModeChanged                = 1;
    }
    if(lUpperKey == 'A')
    {
      arContext.mKbExpParamChange = AG;
      lModeChanged                = 1;
    }
    if(lUpperKey == 'D')
    {
      arContext.mKbExpParamChange = DG;
      lModeChanged                = 1;
    }
    if(lUpperKey == 'C')
    {
      arContext.mKbExpParamChange = CG;
      lModeChanged                = 1;
    }
    if(lUpperKey == 'R')
    {
      lModeChanged = 1;
    }
    if(lModeChanged)
    {
      printf(" %s\t- %s\n", sExposureName[arContext.mKbExpToChange], sParamName[arContext.mKbExpParamChange]);
      MyOV9716_Exposure_Print(CSI_IDX_CAM, SourceCamera);
      arContext.mKbExpVal  = 0;
      arContext.mKbExpTime = 0;
      return 0;
    }
    if(strchr("W\n\r", lUpperKey))
    {
      gOVExposure[0].mCsiIdx = CSI_IDX_CAM;
      printf("Update %s - %s\n", sExposureName[arContext.mKbExpToChange], sParamName[arContext.mKbExpParamChange]);
      if(arContext.mKbExpParamChange == Time)
      {
        if(arContext.mKbExpToChange == OV9716_ExpType_VS)
        {
          gOVExposure[0].mVSExposureTime = (gOVExposure[0].mVSExposureTime & 0xffe0) + arContext.mKbExpVal;
          OV9716_ExposureTimeSet(gOVExposure[0], arContext.mKbExpToChange);
        }
        else
        {
          gOVExposure[0].mDCGExposureTime = arContext.mKbExpTime;
          OV9716_ExposureTimeSet(gOVExposure[0], arContext.mKbExpToChange);
        }
      }
      else if(arContext.mKbExpParamChange == AG)
      {
        gOVExposure[0].mAnalogGain[arContext.mKbExpToChange] = arContext.mKbExpVal;
        OV9716_ExposureAnalogConversionGainSet(gOVExposure[0]);
      }
      else if(arContext.mKbExpParamChange == DG)
      {
        gOVExposure[0].mDigitalGain[arContext.mKbExpToChange] = arContext.mKbExpVal;
        OV9716_ExposureDigitalGainSet(gOVExposure[0], arContext.mKbExpToChange);
      }
      else if(arContext.mKbExpParamChange == CG)
      {
        gOVExposure[0].mVSConversionGainOn = arContext.mKbExpVal;
        OV9716_ExposureAnalogConversionGainSet(gOVExposure[0]);
      }
      MyOV9716_Exposure_Print(CSI_IDX_CAM, SourceCamera);
      arContext.mKbExpVal  = 0;
      arContext.mKbExpTime = 0;
      return 0;
    }

    if(arContext.mKbKey == '-')
    {
      gOVExposure[0].mCsiIdx = CSI_IDX_CAM;
      printf("Update %s - %s\n", sExposureName[arContext.mKbExpToChange], sParamName[arContext.mKbExpParamChange]);

      if(arContext.mKbExpParamChange == Time)
      {
        if(arContext.mKbExpToChange == OV9716_ExpType_VS)
        {
          if(gOVExposure[0].mVSExposureTime > 0)
          {
            gOVExposure[0].mVSExposureTime--;
          }
        }
        else
        {
          if(gOVExposure[0].mDCGExposureTime > (arContext.mExpIncrement + 16))
          {
            gOVExposure[0].mDCGExposureTime -= arContext.mExpIncrement;
          }
          else
          {
            gOVExposure[0].mDCGExposureTime = 16;
          }
        }
        OV9716_ExposureTimeSet(gOVExposure[0], arContext.mKbExpToChange);
      }

      else if(arContext.mKbExpParamChange == AG)
      {
        if(gOVExposure[0].mAnalogGain[arContext.mKbExpToChange] > 0)
        {
          gOVExposure[0].mAnalogGain[arContext.mKbExpToChange]--;
          OV9716_ExposureAnalogConversionGainSet(gOVExposure[0]);
        }
      }

      else if(arContext.mKbExpParamChange == DG)
      {
        if(gOVExposure[0].mDigitalGain[arContext.mKbExpToChange] > (arContext.mDgIncrement + 0x100))
        {
          gOVExposure[0].mDigitalGain[arContext.mKbExpToChange] -= arContext.mDgIncrement;
        }
        else
        {
          gOVExposure[0].mDigitalGain[arContext.mKbExpToChange] = 0x100;
        }
        OV9716_ExposureDigitalGainSet(gOVExposure[0], arContext.mKbExpToChange);
      }

      else if(arContext.mKbExpParamChange == CG)
      {
        gOVExposure[0].mVSConversionGainOn = 0;
        OV9716_ExposureAnalogConversionGainSet(gOVExposure[0]);
      }

      MyOV9716_Exposure_Print(CSI_IDX_CAM, SourceCamera);
      return 0;
    } //  '-' key

    if(arContext.mKbKey == '+')
    {
      gOVExposure[0].mCsiIdx = CSI_IDX_CAM;
      printf("Update %s - %s\n", sExposureName[arContext.mKbExpToChange], sParamName[arContext.mKbExpParamChange]);

      if(arContext.mKbExpParamChange == Time)
      {
        if(arContext.mKbExpToChange == OV9716_ExpType_VS)
        {
          if(gOVExposure[0].mVSExposureTime < 0x1f)
          {
            gOVExposure[0].mVSExposureTime++;
          }
        }
        else
        {
          uint16_t lExpRange = GetExpRange();
          if(gOVExposure[0].mDCGExposureTime < (lExpRange - arContext.mExpIncrement))
          {
            gOVExposure[0].mDCGExposureTime += arContext.mExpIncrement;
          }
          else
          {
            gOVExposure[0].mDCGExposureTime = lExpRange;
          }
          OV9716_ExposureTimeSet(gOVExposure[0], arContext.mKbExpToChange);
        }
      }

      else if(arContext.mKbExpParamChange == AG)
      {
        if(gOVExposure[0].mAnalogGain[arContext.mKbExpToChange] < 3)
        {
          gOVExposure[0].mAnalogGain[arContext.mKbExpToChange]++;
          OV9716_ExposureAnalogConversionGainSet(gOVExposure[0]);
        }
      }

      else if(arContext.mKbExpParamChange == DG)
      {
        if(gOVExposure[0].mDigitalGain[arContext.mKbExpToChange] < (0x800 - arContext.mDgIncrement))
        {
          gOVExposure[0].mDigitalGain[arContext.mKbExpToChange] += arContext.mDgIncrement;
        }
        else
        {
          gOVExposure[0].mDigitalGain[arContext.mKbExpToChange] = 0x800;
        }
        OV9716_ExposureDigitalGainSet(gOVExposure[0], arContext.mKbExpToChange);
      }

      else if(arContext.mKbExpParamChange == CG)
      {
        gOVExposure[0].mVSConversionGainOn = 1;
        OV9716_ExposureAnalogConversionGainSet(gOVExposure[0]);
      }

      MyOV9716_Exposure_Print(CSI_IDX_CAM, SourceCamera);
      return 0;

    } //  '+' key

    if(lNumKey <= 9)
    { // enter IIC command
      arContext.mKbExpVal *= 10;
      arContext.mKbExpVal += lNumKey;
      if((arContext.mKbExpToChange != OV9716_ExpType_VS) & (arContext.mKbExpParamChange == Time)) //DCG time
      {
        if(arContext.mKbExpTime < 1000)
        {
          arContext.mKbExpTime = arContext.mKbExpTime * 10 + lNumKey;
        }
        else
        {
          arContext.mKbExpTime = lNumKey;
        }
        printf("Entered Exp time: %d\n", arContext.mKbExpTime);
        return 0;
      }
      if(arContext.mKbExpParamChange == AG)
      {
        arContext.mKbExpVal &= 0x3;
        printf("Entered AG: %d (x%d)\n", arContext.mKbExpVal, 1 << arContext.mKbExpVal);
        return 0;
      }
      if(arContext.mKbExpParamChange == DG)
      {
        arContext.mKbExpVal &= 0x3fff; //6.8b value
        printf("Entered DG: 0x%04x\n", arContext.mKbExpVal);
        return 0;
      }
      if(arContext.mKbExpParamChange == CG)
      {
        arContext.mKbExpVal &= 0x1;
        printf("Entered CG: %d\n", arContext.mKbExpVal);
        return 0;
      }
      else //Exposure time VS
      {
        arContext.mKbExpVal &= 0x1f;
        printf("Entered fractionnal part of VS Exp time [0-0x1f]: 0x%02x\n", arContext.mKbExpVal);
        return 0;
      }
    }
  }

  if(arContext.mKbMode == 'K')
  {
    if(lNumKey == 0)
    { // show combined
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_HOLD, 0);
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_SEL, 1 << 2);
#if(OV9716_QUAD_GRAPH == 2)
      CAM_RegWrite(CSI_IDX_CAM, 0x3190 | OV9716_GRP, 0x02);
#else
      CAM_RegWrite(CSI_IDX_CAM, 0x3190 | OV9716_GRP, 0x06);
#endif // if(OV9716_QUAD_GRAPH==2)
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_SEL, 0x20 | 1 << 2);
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_HOLD, 1);
      setRegCoherent(INLUT_ENGINE, 0, 0x7f, arContext.mInLut); // GPR15 (IPUS)
      // GPR14 = VS Enable
      setRegCoherent(HDR_ENGINE, 0, HDR_GPR0 + 14, 0x0);
      // GPR15 = L/S Shift
      setRegCoherent(HDR_ENGINE, 0, HDR_GPR0 + 15, (arContext.mExpMode < 4) ? gHDRShift[arContext.mExpMode] : 4);
      arContext.mKbExpSel = 0;
      arContext.mOutLut   = arContext.mpOutLut1[0];
      printf("%d %s HDRShift %d ILUT %d OLUT %d\n", arContext.mKbExpSel, sExpName[arContext.mKbExpSel],
             (arContext.mExpMode < 4) ? gHDRShift[arContext.mExpMode] : 4, arContext.mInLut, arContext.mOutLut);
      return 0;
    } // if key 0
    if(lNumKey == 1)
    { // VS only
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_HOLD, 0);
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_SEL, 1 << 2);
#if(OV9716_QUAD_GRAPH == 2)
      CAM_RegWrite(CSI_IDX_CAM, 0x3190 | OV9716_GRP, 0x02);
#else
      CAM_RegWrite(CSI_IDX_CAM, 0x3190 | OV9716_GRP, 0x06);
#endif // if(OV9716_QUAD_GRAPH==2)
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_SEL, 0x20 | 1 << 2);
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_HOLD, 1);
      setRegCoherent(INLUT_ENGINE, 0, 0x7f, 0); // GPR15 (IPUS)
      // GPR14 = VS Enable
      setRegCoherent(HDR_ENGINE, 0, HDR_GPR0 + 14, 0x0);
      // GPR15 = L/S Shift
      setRegCoherent(HDR_ENGINE, 0, HDR_GPR0 + 15, 0xf);
      arContext.mKbExpSel = 1;
      arContext.mOutLut   = 1;
      printf("%d %s HDRShift %d ILUT %d OLUT %d\n", arContext.mKbExpSel, sExpName[arContext.mKbExpSel], 0xf, 0,
             arContext.mOutLut);
      return 0;
    } // if key 1
    if(lNumKey == 2)
    { // S only
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_HOLD, 0);
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_SEL, 1 << 2);
#if(OV9716_QUAD_GRAPH == 2)
      CAM_RegWrite(CSI_IDX_CAM, 0x3190 | OV9716_GRP, 0x82);
#else
      CAM_RegWrite(CSI_IDX_CAM, 0x3190 | OV9716_GRP, 0x85);
#endif // if(OV9716_QUAD_GRAPH==2)
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_SEL, 0x20 | 1 << 2);
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_HOLD, 1);
      setRegCoherent(INLUT_ENGINE, 0, 0x7f, 0); // GPR15 (IPUS)
      // GPR14 = VS Enable
      setRegCoherent(HDR_ENGINE, 0, HDR_GPR0 + 14, 0xf);
      // GPR15 = L/S Shift
      setRegCoherent(HDR_ENGINE, 0, HDR_GPR0 + 15, 0);
      arContext.mKbExpSel = 2;
      arContext.mOutLut   = 1;
      printf("%d %s HDRShift %d ILUT %d OLUT %d\n", arContext.mKbExpSel, sExpName[arContext.mKbExpSel], 0, 0,
             arContext.mOutLut);
      return 0;
    } // if key 2
    if(lNumKey == 3)
    { // L only
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_HOLD, 0);
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_SEL, 1 << 2);
#if(OV9716_QUAD_GRAPH == 2)
      CAM_RegWrite(CSI_IDX_CAM, 0x3190 | OV9716_GRP, 0xc2);
#else
      CAM_RegWrite(CSI_IDX_CAM, 0x3190 | OV9716_GRP, 0xc5);
#endif // if(OV9716_QUAD_GRAPH==2)
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_SEL, 0x20 | 1 << 2);
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_HOLD, 1);
      setRegCoherent(INLUT_ENGINE, 0, 0x7f, 0); // GPR15 (IPUS)
      // GPR14 = VS Enable
      setRegCoherent(HDR_ENGINE, 0, HDR_GPR0 + 14, 0xf);
      // GPR15 = L/S Shift
      setRegCoherent(HDR_ENGINE, 0, HDR_GPR0 + 15, 0);
      arContext.mKbExpSel = 3;
      arContext.mOutLut   = 1;
      printf("%d %s HDRShift %d ILUT %d OLUT %d\n", arContext.mKbExpSel, sExpName[arContext.mKbExpSel], 0, 0,
             arContext.mOutLut);
      return 0;
    } // if key 3
    if(lNumKey == 4)
    { // L/S only
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_HOLD, 0);
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_SEL, 1 << 2);
#if(OV9716_QUAD_GRAPH == 2)
      CAM_RegWrite(CSI_IDX_CAM, 0x3190 | OV9716_GRP, 0x02);
#else
      CAM_RegWrite(CSI_IDX_CAM, 0x3190 | OV9716_GRP, 0x06);
#endif // if(OV9716_QUAD_GRAPH==2)
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_SEL, 0x20 | 1 << 2);
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_HOLD, 1);
      setRegCoherent(INLUT_ENGINE, 0, 0x7f, arContext.mInLut); // GPR15 (IPUS)
      // GPR14 = VS Enable
      setRegCoherent(HDR_ENGINE, 0, HDR_GPR0 + 14, 0xf);
      // GPR15 = L/S Shift
      setRegCoherent(HDR_ENGINE, 0, HDR_GPR0 + 15, 0x0);
      arContext.mKbExpSel = 4;
      arContext.mOutLut   = arContext.mpOutLut1[1];
      printf("%d %s HDRShift %d ILUT %d OLUT %d\n", arContext.mKbExpSel, sExpName[arContext.mKbExpSel], 0,
             arContext.mInLut, arContext.mOutLut);
      return 0;
    } // if key 4
    if(lNumKey == 5)
    { // L+VS only
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_HOLD, 0);
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_SEL, 1 << 2);
#if(OV9716_QUAD_GRAPH == 2)
      CAM_RegWrite(CSI_IDX_CAM, 0x3190 | OV9716_GRP, 0xc2);
#else
      CAM_RegWrite(CSI_IDX_CAM, 0x3190 | OV9716_GRP, 0xc5);
#endif // if(OV9716_QUAD_GRAPH==2)
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_SEL, 0x20 | 1 << 2);
      CAM_RegWrite(CSI_IDX_CAM, OV9716_GRP_HOLD, 1);
      setRegCoherent(INLUT_ENGINE, 0, 0x7f, 0); // GPR15 (IPUS)
      // GPR14 = VS Enable
      setRegCoherent(HDR_ENGINE, 0, HDR_GPR0 + 14, 0x0);
      // GPR15 = L/S Shift
      setRegCoherent(HDR_ENGINE, 0, HDR_GPR0 + 15, (arContext.mExpMode < 4) ? gHDRShift[arContext.mExpMode] : 4);
      arContext.mKbExpSel = 5;
      arContext.mOutLut   = arContext.mpOutLut1[3];
      printf("%d %s HDRShift %d ILUT %d OLUT %d\n", arContext.mKbExpSel, sExpName[arContext.mKbExpSel],
             (arContext.mExpMode < 4) ? gHDRShift[arContext.mExpMode] : 4, 0, arContext.mOutLut);
      return 0;
    } // if key 5
  }   // if mode k

  if(arContext.mKbMode == 'H')
  {
    if(lNumKey < 8)
    {
      gHDRShift[arContext.mExpMode] = lNumKey;
      printf("HDR Long/Short Shift Right = %d\n", gHDRShift[arContext.mExpMode]);
      setRegCoherent(HDR_ENGINE, 0, HDR_GPR0 + 15, gHDRShift[arContext.mExpMode]);
      return 0;
    }
  }

  if(arContext.mKbMode == 'G')
  {
    uint8_t lSet = 0;

    if(arContext.mKbKey == 'G')
    {
      if(arContext.mKbChgain < (CHGAIN_SETS - 1))
      {
        arContext.mKbChgain++;
      }
      else
      {
        arContext.mKbChgain = 0;
      } // else from if chgain < 6
      lSet = 1;
    }

    else if(lHexKey < CHGAIN_SETS)
    {
      arContext.mKbChgain = lHexKey;
      lSet                = 1;
    }

    if(lSet)
    {
      MyOV9716_ChgainSet(CSI_IDX_CAM, MAXIM_CHNL_ORIG_BC, (double)sChGainTab[arContext.mKbChgain][0],
                         (double)((sChGainTab[arContext.mKbChgain][1] + sChGainTab[arContext.mKbChgain][2]) / 2),
                         (double)sChGainTab[arContext.mKbChgain][3]);
      MyOV9716_Chgain_Print(CSI_IDX_CAM, SourceCamera);
      arContext.mKbAWB = 0;
      return 0;
    }
  }

  if(arContext.mKbMode == 'T')
  {
    uint8_t lSet = 0;
    if(arContext.mKbKey == '+')
    {
      arContext.mKbSaturation += 0.05;
      if(arContext.mKbSaturation > 2.0)
      {
        arContext.mKbSaturation = 2.0;
      }
      lSet = 1;
    } // if key +
    else if(arContext.mKbKey == '-')
    {
      arContext.mKbSaturation -= 0.05;
      if(arContext.mKbSaturation < 0.0)
      {
        arContext.mKbSaturation = 0.0;
      }
      lSet = 1;
    } // if key -
    else if(lNumKey == 0)
    {
      arContext.mKbSaturation = 0.0;
      lSet                    = 1;
    } // if key 0
    else if(lNumKey == 1)
    {
      arContext.mKbSaturation = 1.0;
      lSet                    = 1;
    } // if key 1

    if(lSet)
    {
      printf("Color saturation = %f\n", arContext.mKbSaturation);
      CcmCscMatrix(gCcmCscMatrix, gKbCcmMatrix[arContext.mKbCCM], gRgb2YuvMatrix, arContext.mKbSaturation);

      // *** TONEMAP Engine
      // gpr0: R->U, gpr1: G->U, gpr2: B->U
      // gpr3: R->V, gpr4: G->V, gpr5: B->V
      seq_setReg(TONEMAP_ENGINE0, 0, 0x50, // GPR0 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[3]));
      seq_setReg(TONEMAP_ENGINE0, 0, 0x51, // GPR1 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[4]));
      seq_setReg(TONEMAP_ENGINE0, 0, 0x52, // GPR2 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[5]));
      seq_setReg(TONEMAP_ENGINE0, 0, 0x53, // GPR3 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[6]));
      seq_setReg(TONEMAP_ENGINE0, 0, 0x54, // GPR4 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[7]));
      seq_setReg(TONEMAP_ENGINE0, 0, 0x55, // GPR5 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[8]));

      seq_setReg(TONEMAP_ENGINE1, 0, 0x50, // GPR0 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[3]));
      seq_setReg(TONEMAP_ENGINE1, 0, 0x51, // GPR1 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[4]));
      seq_setReg(TONEMAP_ENGINE1, 0, 0x52, // GPR2 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[5]));
      seq_setReg(TONEMAP_ENGINE1, 0, 0x53, // GPR3 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[6]));
      seq_setReg(TONEMAP_ENGINE1, 0, 0x54, // GPR4 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[7]));
      seq_setReg(TONEMAP_ENGINE1, 0, 0x55, // GPR5 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[8]));
      return 0;
    }
  } // if mode T

  if(arContext.mKbMode == 'R')
  {
    uint16_t lRedTarget = GetRedTarget();
    uint8_t  lSet       = 0;
    if(arContext.mKbKey == '+')
    {                      // show combined
      if(arContext.mKbAWB) // if AWB is on
      {
        if(lRedTarget < 0xFFF0)
        {
          lRedTarget += arContext.mRBIncrement;
          SetRedTarget(lRedTarget);
        } // if offset <= in range
      }   //if(arContext.mKbAWB)
      else
      {
        MyOV9716_ChgainManSetR(CSI_IDX_CAM, MAXIM_CHNL_ORIG_BC, arContext.mRBIncrement);
        MyOV9716_Chgain_Print(CSI_IDX_CAM, SourceCamera);
      }
      lSet = 1;
    } // if key +
    else if(arContext.mKbKey == '-')
    {
      if(arContext.mKbAWB) // if AWB is on
      {
        if(lRedTarget > 15)
        {
          lRedTarget -= arContext.mRBIncrement;
          SetRedTarget(lRedTarget);
        } // if offset >= X_PAN_STEPSIZE
      }   // if(arContext.mKbAWB)
      else
      {
        MyOV9716_ChgainManSetR(CSI_IDX_CAM, MAXIM_CHNL_ORIG_BC, -arContext.mRBIncrement);
        MyOV9716_Chgain_Print(CSI_IDX_CAM, SourceCamera);
      }
      lSet = 1;
    } // if key -

    if(lSet)
    {
      if(arContext.mKbAWB)
        printf("Red target value = %f\n", (float_t)lRedTarget / 256.0);
      return 0;
    }
  } // if mode R

  if(arContext.mKbMode == 'B')
  {
    uint8_t  lSet        = 0;
    uint16_t lBlueTarget = GetBlueTarget();
    if(arContext.mKbKey == '+')
    {                      // show combined
      if(arContext.mKbAWB) // if AWB is on
      {
        if(lBlueTarget < 0xFFF0)
        {
          lBlueTarget += arContext.mRBIncrement;
          SetBlueTarget(lBlueTarget);
        } // if offset <= in range
      }   //if(arContext.mKbAWB)
      else
      {
        MyOV9716_ChgainManSetB(CSI_IDX_CAM, MAXIM_CHNL_ORIG_BC, arContext.mRBIncrement);
        MyOV9716_Chgain_Print(CSI_IDX_CAM, SourceCamera);
      }
      lSet = 1;
    } // if key +
    else if(arContext.mKbKey == '-')
    {
      if(arContext.mKbAWB) // if AWB is on
      {
        if(lBlueTarget > 15)
        {
          lBlueTarget -= arContext.mRBIncrement;
          SetBlueTarget(lBlueTarget);
        } // if offset >= X_PAN_STEPSIZE
      }
      else
      {
        MyOV9716_ChgainManSetB(CSI_IDX_CAM, MAXIM_CHNL_ORIG_BC, -arContext.mRBIncrement);
        MyOV9716_Chgain_Print(CSI_IDX_CAM, SourceCamera);
      }
      lSet = 1;
    } // if key -

    if(lSet)
    {
      if(arContext.mKbAWB)
        printf("Blue target value = %f\n", (float_t)lBlueTarget / 256.0);
      return 0;
    }
  } // if mode B

  if(arContext.mKbMode == 'O')
  {
    if(lNumKey <= OUTLUT_SETS)
    {
      arContext.mOutLut    = lNumKey;
      arContext.mOldOutLut = -1; // force OutLut rewrite
      if(arContext.mExpMode < 4)
        arContext.mpOutLut1[arContext.mExpMode] = arContext.mOutLut;
      return 0;
    }
    else if(arContext.mKbKey == 'O')
    {
      arContext.mOutLut++;
      if(arContext.mOutLut > OUTLUT_SETS)
      {
        arContext.mOutLut = 0;
      }
      arContext.mOldOutLut = -1; // force OutLut rewrite
      if(arContext.mExpMode < 4)
      {
        arContext.mpOutLut1[arContext.mExpMode] = arContext.mOutLut;
      }
      return 0;
    }
  }

  if(arContext.mKbMode == 'C')
  {
    uint8_t lSet = 0;

    if(arContext.mKbKey == 'C')
    {
      if(arContext.mKbCCM < (CCM_SETS - 1))
      {
        arContext.mKbCCM++;
      } // if table < CCM_SET
      else
      {
        arContext.mKbCCM = 0;
      }
      lSet = 1;
    }
    else if(lHexKey < CCM_SETS)
    {
      arContext.mKbCCM = lHexKey;
      lSet             = 1;
    }

    if(lSet)
    {
      printf(
          "CCM table %d\n"
          "%6.3f %6.3f %6.3f\n"
          "%6.3f %6.3f %6.3f\n"
          "%6.3f %6.3f %6.3f\n",
          arContext.mKbCCM, gKbCcmMatrix[arContext.mKbCCM][0], gKbCcmMatrix[arContext.mKbCCM][1],
          gKbCcmMatrix[arContext.mKbCCM][2], gKbCcmMatrix[arContext.mKbCCM][3], gKbCcmMatrix[arContext.mKbCCM][4],
          gKbCcmMatrix[arContext.mKbCCM][5], gKbCcmMatrix[arContext.mKbCCM][6], gKbCcmMatrix[arContext.mKbCCM][7],
          gKbCcmMatrix[arContext.mKbCCM][8]);

      CcmCscMatrix(gCcmCscMatrix, gKbCcmMatrix[arContext.mKbCCM], gRgb2YuvMatrix, arContext.mKbSaturation);
      // color space conversion factors
      // gpr0: R->Y, gpr1: G->Y, gpr2: B->Y
      seq_setReg(RGB2Y_ENGINE0, 0, 0x54, // GPR0 (IPUV)
                 (int16_t)(32768.0 * gCcmCscMatrix[0]));
      seq_setReg(RGB2Y_ENGINE0, 0, 0x55, // GPR1 (IPUV)
                 (int16_t)(32768.0 * gCcmCscMatrix[1]));
      seq_setReg(RGB2Y_ENGINE0, 0, 0x56, // GPR2 (IPUV)
                 (int16_t)(32768.0 * gCcmCscMatrix[2]));

      seq_setReg(RGB2Y_ENGINE1, 0, 0x54, // GPR0 (IPUV)
                 (int16_t)(32768.0 * gCcmCscMatrix[0]));
      seq_setReg(RGB2Y_ENGINE1, 0, 0x55, // GPR1 (IPUV)
                 (int16_t)(32768.0 * gCcmCscMatrix[1]));
      seq_setReg(RGB2Y_ENGINE1, 0, 0x56, // GPR2 (IPUV)
                 (int16_t)(32768.0 * gCcmCscMatrix[2]));

      // *** TONEMAP Engine
      // gpr0: R->U, gpr1: G->U, gpr2: B->U
      // gpr3: R->V, gpr4: G->V, gpr5: B->V
      seq_setReg(TONEMAP_ENGINE0, 0, 0x50, // GPR0 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[3]));
      seq_setReg(TONEMAP_ENGINE0, 0, 0x51, // GPR1 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[4]));
      seq_setReg(TONEMAP_ENGINE0, 0, 0x52, // GPR2 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[5]));
      seq_setReg(TONEMAP_ENGINE0, 0, 0x53, // GPR3 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[6]));
      seq_setReg(TONEMAP_ENGINE0, 0, 0x54, // GPR4 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[7]));
      seq_setReg(TONEMAP_ENGINE0, 0, 0x55, // GPR5 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[8]));

      seq_setReg(TONEMAP_ENGINE1, 0, 0x50, // GPR0 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[3]));
      seq_setReg(TONEMAP_ENGINE1, 0, 0x51, // GPR1 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[4]));
      seq_setReg(TONEMAP_ENGINE1, 0, 0x52, // GPR2 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[5]));
      seq_setReg(TONEMAP_ENGINE1, 0, 0x53, // GPR3 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[6]));
      seq_setReg(TONEMAP_ENGINE1, 0, 0x54, // GPR4 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[7]));
      seq_setReg(TONEMAP_ENGINE1, 0, 0x55, // GPR5 (IPUV)
                 (int16_t)(4096.0 * gCcmCscMatrix[8]));

      return 0;
    }
  } // Mode C

  if(arContext.mKbMode == 'Y')
  {
    if(lUpperKey == 'E')
    {
      if(arContext.mKbViewEdge)
      {
        arContext.mKbViewEdge = 0;
      }
      else
      {
        arContext.mKbViewEdge = 0xffff;
      }
      // selector for edge view
      seq_setReg(DENOISEY_ENGINE0, 0, DENOISEY_GPR + 12, arContext.mKbViewEdge);
#if(DENOISEY_IPU0 != DENOISEY_IPU1)
      seq_setReg(DENOISEY_ENGINE1, 0, DENOISEY_GPR + 12, arContext.mKbViewEdge);
#endif

      printf("Denoise view: %s\n", (arContext.mKbViewEdge) ? "normal" : "edge");
      return 0;
    } // if E pressed

    uint8_t lSet = 0;
    if(arContext.mKbKey == '+')
    { // show combined
      arContext.mKbYDenoise += 64;
      lSet = 1;
    } // if key +
    else if(arContext.mKbKey == '-')
    {
      arContext.mKbYDenoise -= 64;
      lSet = 1;
    } // if key -
    else if(lNumKey == 0)
    {
      arContext.mKbYDenoise = 0;
      lSet                  = 1;
    } // if key 0
    else if(arContext.mKbKey == '*')
    {
      arContext.mKbYDenoise = -64;
      lSet                  = 1;
    } // if key *

    if(lSet)
    {
      printf("Y de-noise level = %d\n", arContext.mKbYDenoise);
      // set threshold
      seq_setReg(DENOISEY_ENGINE0, 0, DENOISEY_GPR + 7, arContext.mKbYDenoise);
#if(DENOISEY_IPU0 != DENOISEY_IPU1)
      seq_setReg(DENOISEY_ENGINE1, 0, DENOISEY_GPR + 7, arContext.mKbYDenoise);
#endif
      return 0;
    }
  } // if mode B

    // *************************************
    // *****check if we need to enter a mode
#ifndef __STANDALONE__
  if(arContext.mKbKey == 'X')
  {
    printf("Reloading init file...\n");
    IniParse(arContext);
    IspConfig(arContext);
    SetDeflickerHz(arContext.mFlickerFrequency[arContext.mKbDeFlicker],
                   arContext.mCamFps); //needs to be executed after ExposureInit
    return 0;
  }
#endif

  if(arContext.mKbKey == 'c')
  {
    arContext.mKbMode = 'c';
    arContext.mKbCursorOn ^= 1;
    printf("CursorMode %s\n", (arContext.mKbCursorOn) ? "on (a,w,s,y/z)" : "off");
    printf("Cursor (%d,%d)\n", arContext.mKbCursorX, arContext.mKbCursorY);
    return 0;
  } // if c pressed

  if(arContext.mKbKey == 'd')
  {
    arContext.mKbMode = 'd';
    arContext.mKbDisplay ^= 1;
    printf("Display update %s\n", (arContext.mKbDisplay) ? "on" : "off");
    if(!arContext.mKbDisplay)
      printf("Frame Buffer %d\n", arContext.mApexActive ? arContext.mApexBufferNr : arContext.mGpuBufferNr);
    return 0;
  } // if d pressed

  if(arContext.mKbKey == 'p')
  {
    printf("Statistics Print = %s\n", lPrintMode[arContext.mKbLog]);
    arContext.mKbMode = 'p';
    return 0;
  } // if p pressed

  if(arContext.mKbKey == 'e')
  {
    arContext.mKbMode = 'e';
    printf("Parameter Engine = %d\n", arContext.mKbEngine);
    return 0;
  }

  if(arContext.mKbKey == 'g')
  {
    arContext.mKbMode = 'g';
    printf("Parameter Engine = %d GPR%d\n", arContext.mKbEngine, arContext.mKbGPR);
    return 0;
  }

  if(arContext.mKbKey == 'v')
  {
    printf("Mode = parameter value selection (0-f,+,-,w,r)\n");
    arContext.mKbMode = 'v';
    return 0;
  }

  if(arContext.mKbKey == 'i')
  {
    printf("IIC command mode\n");
    printf("IIC command 0x%x - 0x%x\n", (arContext.mKbI2c >> 8), (arContext.mKbI2c & 0xff));
    arContext.mKbMode = 'i';
    return 0;
  } // if i pressed

  if(arContext.mKbKey == 'D')
  {
    uint8_t lVal8;

    if(arContext.mKbMode != 'D')
    {
      printf(
          "Do you really want to dump all IIC registers?"
          " yes: press \'D\' again\n");
      arContext.mKbMode = 'D';
      return 0;
    }

#ifndef __STANDALONE__
    FILE* lIicOutfile = fopen("ov9716_quad_IIC.txt", "w+");
    if(!lIicOutfile)
    {
      printf("could not open ov9716_quad_IIC.txt\n");
      return 1;
    }
#endif

    printf("Doing IIC register dump:\n");
    for(uint16_t i = 0x3000; i < 0x3500; i++)
    {
      CAM_RegRead(CSI_IDX_CAM, i, lVal8);
      //msleep(5);
#ifndef __STANDALONE__
      if(lIicOutfile)
      {
        fprintf(lIicOutfile, "0x%x = 0x%x\n", i, lVal8);
      }
#endif
      printf("0x%x = 0x%x\n", i, lVal8);
    }

    for(uint16_t i = 0x7000; i < 0x7800; i++)
    {
      CAM_RegRead(CSI_IDX_CAM, i, lVal8);
      //msleep(5);
#ifndef __STANDALONE__
      if(lIicOutfile)
      {
        fprintf(lIicOutfile, "0x%x = 0x%x\n", i, lVal8);
      }
#endif
      printf("0x%x = 0x%x\n", i, lVal8);
    }

#ifndef __STANDALONE__
    if(lIicOutfile)
    {
      fclose(lIicOutfile);
    }
#endif

    arContext.mKbMode = 0;
    return 0;
  }

  if(arContext.mKbKey == 't')
  {
    if(arContext.mKbTestbar)
    {
      arContext.mKbTestbar = 0;
      printf("Testbar off\n");
      CAM_RegWrite(CSI_IDX_CAM, 0x3253, 0x00);
    }
    else
    {
      arContext.mKbTestbar = 1;
      CAM_RegWrite(CSI_IDX_CAM, 0x3253, 0x83);
      printf("Testbar on\n");
    }
    return 0;
  }

  // ***** saving
  // ***** saving
  // ***** saving

  if(arContext.mKbKey == 'm')
  {
    arContext.mKbMode = 'm';
    printf("Measurment start\n");
    arContext.mKbMeasure = 0;
    return 0;
  }

  if(arContext.mKbKey == 's')
  {
    arContext.mKbMode = 's';
    FrameOutSave(sExpName[arContext.mKbExpSel], arContext);
    return 0;
  }
#if(CAM_SAVE == 1)
  if(arContext.mKbKey == 'S')
  {
    arContext.mKbMode = 'S';
    FrameInSave(sExpName[arContext.mKbExpSel], arContext);
    return 0;
  }
#endif //if(CAM_SAVE==1)

  if(arContext.mKbKey == 'x')
  {
    printf("Set X-Offset for Display (+/-)\n");
    arContext.mKbMode = 'x';
    return 0;
  } // if x pressed

  if(arContext.mKbKey == 'I')
  {
    printf("Input  Lut %d\n", arContext.mInLut);
    arContext.mKbMode = 'I';
    return 0;
  } // if I pressed

  // **** black level
  // **** black level
  // **** black level
  if(arContext.mKbKey == 'b')
  {
    printf("Black Level = %d  (+/-)\n", arContext.mKbBlackLevel);
    arContext.mKbMode = 'b';
    return 0;
  } // if b pressed

  if(arContext.mKbKey == 'A')
  {
    printf(
        "Control AEC (+/-/2/3/4)\n"
        "  +........increase AEC update factor of 0.05 AEC\n"
        "  -........decrease AEC update factor of 0.05 AEC\n"
        "  2/3/4....Change the AEC period to 2/3/4 frames\n");
    printf("AEC %s\n", (arContext.mKbAEC) ? "on" : "off");
    arContext.mKbMode = 'A';
    return 0;
  } // if A pressed

  if(arContext.mKbKey == 'M')
  {
    printf(
        "M .........toggle exposure mode\n"
        "  0........outdoor\n"
        "  1........indoor\n"
        "  2........dark\n"
        "  3........dark+headlamp\n");
    printf("Exposure Mode = %d\n", arContext.mExpMode);
    arContext.mKbMode = 'M';
    return 0;
  } // if M pressed

  if(arContext.mKbKey == 'L')
  {
    printf("Select long/short exposure mode\n");
    arContext.mKbMode = 'L';
    printf("long target= %d\n", arContext.mKbLtrgt);
    return 0;
  } // if 'L' pressed

  if(arContext.mKbKey == 'V')
  {
    printf("Select very short exposure mode\n");
    arContext.mKbMode = 'V';
    printf("vs target= %d\n", arContext.mKbVStrgt);
    return 0;
  } // if 'V' pressed

  if(arContext.mKbKey == 'r')
  {
    printf(
        "Changes exposure ratio between HCG and LCG off (+/-)\n"
        "  +........increase ratio by 5%%\n"
        "  -........decrease ratio by 5%%\n");
    printf("Set HCG/LCG ratio %f\n", arContext.mRatio);
    arContext.mKbMode = 'r';
    return 0;
  } // if r pressed

  if(arContext.mKbKey == 'J')
  {
    arContext.mKbMode = 'J';
    arContext.mKbAEC  = 0;
    arContext.mKbAWB  = 0;
    printf(
        "AEC off\n"
        "AWB off\n"
        " E ....... select Exposure\n"
        " P ....... select Parameter (time, ag, dg, cg)\n"
        " w/ret ... write parameter\n"
        "  + .......increase the parameter and update it directly \n"
        "  - .......decrease the parameter and update it directly \n"
        "Manual change of camera parameter: %s - %s\n",
        sExposureName[arContext.mKbExpToChange], sParamName[arContext.mKbExpParamChange]);
    MyOV9716_Exposure_Print(CSI_IDX_CAM, SourceCamera);
    return 0;
    //     printf(
  } // if J pressed

  if(arContext.mKbKey == 'K')
  {
    printf(
        "Mode = exposure mode selection %d"
        "(0[hdr]/1[vs]/2[s]/3[l]/4[l/s]/5(l+vs))\n",
        arContext.mKbExpSel);
    arContext.mKbMode = 'K';
    arContext.mKbAEC  = 0;
    printf("AEC off\n");
    return 0;
  } // if k pressed

  if(arContext.mKbKey == 'H')
  {
    if(arContext.mKbMode == 'H')
    {
      gHDRShift[arContext.mExpMode]++;
      if(gHDRShift[arContext.mExpMode] > 8)
        gHDRShift[arContext.mExpMode] = 0;
    }

    printf("HDR Long+Short Shift Right = %d\n", gHDRShift[arContext.mExpMode]);
    // set the IPU register in a frame/synced way
    setRegCoherent(HDR_ENGINE, 0, HDR_GPR0 + 15, gHDRShift[arContext.mExpMode]);
    //setRegCoherentClose();
    arContext.mKbMode = 'H';
    return 0;
  } // if H pressed

  if(arContext.mKbKey == 'f')
  {
    printf(
        "toggle deflicker (off/50Hz/60Hz)\n"
        "  f........switch next\n");
    float_t lFrequency[4] = {100.0, 100.0, 120.0, 90.0};
    uint8_t lEnable[4]    = {0, 1, 1, 1};

    if(arContext.mKbMode == 'f')
    {
      if(arContext.mKbDeFlicker < 3)
      {
        arContext.mKbDeFlicker++;
      }
      else
      {
        arContext.mKbDeFlicker = 0;
      }
    }
    SetDeflickerHz(lFrequency[arContext.mKbDeFlicker], arContext.mCamFps);
    EnableDeflicker(lEnable[arContext.mKbDeFlicker]);

    arContext.mKbMode = 'f';
    return 0;
  } // if f pressed

  if(arContext.mKbKey == 'W')
  {
    arContext.mKbAWB ^= 1;
    printf("AWB %s\n", (arContext.mKbAWB) ? "on" : "off");
    arContext.mKbMode = 'W';
    return 0;
  } // if W pressed

  if(arContext.mKbKey == 'G')
  {
    printf("chgain %d (%d,%d,%d,%d)\n", arContext.mKbChgain, sChGainTab[arContext.mKbChgain][0],
           sChGainTab[arContext.mKbChgain][1], sChGainTab[arContext.mKbChgain][2], sChGainTab[arContext.mKbChgain][3]);

    arContext.mKbMode = 'G';
    return 0;
  } // if 'G' pressed

  if(arContext.mKbKey == 'T')
  {
    printf("Color Saturation = %f  (+/-)\n", arContext.mKbSaturation);
    arContext.mKbMode = 'T';
    return 0;
  } // if T pressed

  if(arContext.mKbKey == 'R')
  {
    printf(
        "Red Control ((+/-)\n"
        "AEC ON: Red target control; AEC OFF Red Increase decrease (%d):\n",
        arContext.mRBIncrement);
    printf("Red Target = %f\n", (float_t)GetRedTarget() / 256.0);
    arContext.mKbMode = 'R';
    return 0;
  } // if R pressed

  if(arContext.mKbKey == 'B')
  {
    printf(
        "Blue target control (+/-):\n"
        "  +.......AEC ON: increase blue target  "
        "AEC off: increse blue gain\n\n"
        "  -.......decrease blue target\n");
    printf("Blue Target = %f (+/-)\n", (float_t)GetBlueTarget() / 256.0);
    arContext.mKbMode = 'B';
    return 0;
  } // if B pressed

  if(arContext.mKbKey == 'O')
  {
    printf("Output Lut %d\n", arContext.mOutLut);
    arContext.mKbMode = 'O';
    return 0;
  } // if < pressed

  if(arContext.mKbKey == 'C')
  {
    arContext.mKbAWB = 0;
    printf("AWB is off\n");
    printf("CCM Table = %d\n", arContext.mKbCCM);

    printf(
        "CCM table %d\n"
        "%6.3f %6.3f %6.3f\n"
        "%6.3f %6.3f %6.3f\n"
        "%6.3f %6.3f %6.3f\n",
        arContext.mKbCCM, gKbCcmMatrix[arContext.mKbCCM][0], gKbCcmMatrix[arContext.mKbCCM][1],
        gKbCcmMatrix[arContext.mKbCCM][2], gKbCcmMatrix[arContext.mKbCCM][3], gKbCcmMatrix[arContext.mKbCCM][4],
        gKbCcmMatrix[arContext.mKbCCM][5], gKbCcmMatrix[arContext.mKbCCM][6], gKbCcmMatrix[arContext.mKbCCM][7],
        gKbCcmMatrix[arContext.mKbCCM][8]);
    arContext.mKbMode = 'C';
    return 0;
  } // if C pressed

  if(arContext.mKbKey == 'Y')
  {
    printf(
        "Y-denoising control (+/-/E):\n"
        "  +.......increase Y-denoising level\n"
        "  -.......decrease Y-denoising level\n"
        "  0.......Y-denoising off\n"
        "  *.......Y-denoising all on\n"
        "  E.......toggle edge view in denoising\n");
    printf("Set Y de-noise level %d\n", arContext.mKbYDenoise);
    arContext.mKbMode = 'Y';
    return 0;
  } // if Y pressed

  if(arContext.mKbKey == 'y')
  {
    if(arContext.mKbMode == 'y')
    {
      arContext.mKbYuvToRgb ^= 0x1;
    }
    printf("Toggle YUV420 to RGB conversion to %s\n", (arContext.mKbYuvToRgb) ? "Neon" : "A53");
    arContext.mKbMode = 'y';
    return 0;
  } // if y pressed

  if(strchr(" \n\r", arContext.mKbKey))
  {
    printf("Clear key mode\n");
    arContext.mKbMode = 0;
    return 0;
  } // if space pressed

  printf("unknown key %d %c\n", arContext.mKbKey, arContext.mKbKey);
  return 1;

} // KbAction()

/* EOF */
