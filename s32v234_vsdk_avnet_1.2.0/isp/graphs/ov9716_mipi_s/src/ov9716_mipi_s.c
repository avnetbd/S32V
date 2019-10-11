/*****************************************************************************
 *
 * Freescale Confidential Proprietary
 *
 * Copyright (c) 2017 Freescale Semiconductor;
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

// Generated from:
// Z:\newmount\vsdk_repo\s32v234_sdk\isp\graphs\ov9716_mipi_s\model\ov9716_mipi_s.isp

#ifdef  __cplusplus
extern "C" {
#endif

#include "ov9716_mipi_s_c.h"
#include "ov9716_graph_geometries.h"
#ifndef S32DS_BUILD
#include "kmem.c"
#endif
#include "kmem_sym.h"
// *** forwards needed for cross-linking ***

static SEQ_FdmaCfg_t FastDMA;
static SEQ_MipiCfg_t OV_QUAD;
static SEQ_IpuCfg_t Histogram;
static SEQ_IpuCfg_t Debayer0;
static SEQ_IpuCfg_t Debayer1;
static SEQ_IpuCfg_t RGB2Y0;
static SEQ_IpuCfg_t RGB2Y1;
static SEQ_IpuCfg_t LUT4K0;
static SEQ_IpuCfg_t LUT4K1;
static SEQ_IpuCfg_t COMPRESS0;
static SEQ_IpuCfg_t COMPRESS1;
static SEQ_IpuCfg_t DENOISEY2_0;
static SEQ_IpuCfg_t DENOISEY2_1;
static SEQ_IpuCfg_t DENOISE_UV;
static SEQ_IpuCfg_t HDR;
static SEQ_IpuCfg_t INLUT;
static SEQ_Load_t sCodehistogram_Histogram;
static SEQ_Load_t sCodedebayer_even_Debayer0;
static SEQ_Load_t sCodedebayer_odd_Debayer1;
static SEQ_Load_t sCodergb2y_RGB2Y0;
static SEQ_Load_t sCodergb2y_RGB2Y1;
static SEQ_Load_t sCodelut4k_LUT4K0;
static SEQ_Load_t sCodelut4k_hist_LUT4K1;
static SEQ_Load_t sCodergb_compress_y_uv_ccm_COMPRESS0;
static SEQ_Load_t sCodergb_compress_y_uv_ccm_COMPRESS1;
static SEQ_Load_t sCodedenoise_median_DENOISEY2_0;
static SEQ_Load_t sCodedenoise_median_DENOISEY2_1;
static SEQ_Load_t sCodedenoise4x4_uv_DENOISE_UV;
static SEQ_Load_t sCodehdr_blend_HDR;
static SEQ_Load_t sCodelut4k_tee_INLUT;

/*** Buffers ****************************************************************/

SEQ_Buf_t OV_QUAD_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  9,        // *** number of lines fitting into output buffer (wrap) ***
  IN_PIXELS * 2,        // *** number of bytes per line ***
  BUF_ALIGN(IN_STRIDE16),       // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 4468 /*OV_QUAD*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 4504 /* FastDMA.mHead */,  // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      IN_PIXELS * 2,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 4176 /* INLUT.mHead */,  // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      2,        // *** number of lines required for processing ***
      IN_PIXELS * 2,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      2,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 3876 /* HDR.mHead */,  // *** offset to consumers (node) configuration header ***
      1,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      8,        // *** number of lines required for processing ***
      IN_PIXELS * 2,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      2,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 124 /* Histogram.mHead */, // *** offset to consumers (node) configuration header ***
      2,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      8,        // *** number of lines required for processing ***
      IN_PIXELS * 2,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      8,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // OV_QUAD_Buffer_0

SEQ_Buf_t HDR_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  5,        // *** number of lines fitting into output buffer (wrap) ***
  HDR_OUTPIX * 2,       // *** number of bytes per line ***
  BUF_ALIGN(HDR_OUTSTRIDE16),       // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 3876 /*HDR*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 432 /* Debayer0.mHead */,  // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      HDR_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      2,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 432 /* Debayer0.mHead */,  // *** offset to consumers (node) configuration header ***
      1,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      2,        // *** number of lines required for processing ***
      HDR_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      2,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 432 /* Debayer0.mHead */,  // *** offset to consumers (node) configuration header ***
      2,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      3,        // *** number of lines required for processing ***
      HDR_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      2,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 756 /* Debayer1.mHead */,  // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      2,        // *** number of lines required for processing ***
      HDR_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      2,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 756 /* Debayer1.mHead */,  // *** offset to consumers (node) configuration header ***
      1,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      3,        // *** number of lines required for processing ***
      HDR_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      2,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 756 /* Debayer1.mHead */,  // *** offset to consumers (node) configuration header ***
      2,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      4,        // *** number of lines required for processing ***
      HDR_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      2,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // HDR_Buffer_0

SEQ_Buf_t Debayer0_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  6,        // *** number of lines fitting into output buffer (wrap) ***
  DEBAYER_OUTPIX * 2,       // *** number of bytes per line ***
  BUF_ALIGN(DEBAYER_STRIDE16),        // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 432 /*Debayer0*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 1080 /* RGB2Y0.mHead */, // *** offset to consumers (node) configuration header ***
      1,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      DEBAYER_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 2280 /* COMPRESS0.mHead */,  // *** offset to consumers (node) configuration header ***
      1,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      DEBAYER_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // Debayer0_Buffer_0

SEQ_Buf_t Debayer0_Buffer_1 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  6,        // *** number of lines fitting into output buffer (wrap) ***
  DEBAYER_OUTPIX * 2,       // *** number of bytes per line ***
  BUF_ALIGN(DEBAYER_STRIDE16),        // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 432 /*Debayer0*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 1080 /* RGB2Y0.mHead */, // *** offset to consumers (node) configuration header ***
      2,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      DEBAYER_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 2280 /* COMPRESS0.mHead */,  // *** offset to consumers (node) configuration header ***
      2,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      DEBAYER_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // Debayer0_Buffer_1

SEQ_Buf_t Debayer0_Buffer_2 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  6,        // *** number of lines fitting into output buffer (wrap) ***
  DEBAYER_OUTPIX * 2,       // *** number of bytes per line ***
  BUF_ALIGN(DEBAYER_STRIDE16),        // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 432 /*Debayer0*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 1080 /* RGB2Y0.mHead */, // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      DEBAYER_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 2280 /* COMPRESS0.mHead */,  // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      DEBAYER_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // Debayer0_Buffer_2

SEQ_Buf_t Debayer1_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  6,        // *** number of lines fitting into output buffer (wrap) ***
  DEBAYER_OUTPIX * 2,       // *** number of bytes per line ***
  BUF_ALIGN(DEBAYER_STRIDE16),        // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 756 /*Debayer1*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 1388 /* RGB2Y1.mHead */, // *** offset to consumers (node) configuration header ***
      1,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      DEBAYER_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 2612 /* COMPRESS1.mHead */,  // *** offset to consumers (node) configuration header ***
      1,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      DEBAYER_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // Debayer1_Buffer_0

SEQ_Buf_t Debayer1_Buffer_1 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  6,        // *** number of lines fitting into output buffer (wrap) ***
  DEBAYER_OUTPIX * 2,       // *** number of bytes per line ***
  BUF_ALIGN(DEBAYER_STRIDE16),        // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 756 /*Debayer1*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 1388 /* RGB2Y1.mHead */, // *** offset to consumers (node) configuration header ***
      2,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      DEBAYER_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 2612 /* COMPRESS1.mHead */,  // *** offset to consumers (node) configuration header ***
      2,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      DEBAYER_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // Debayer1_Buffer_1

SEQ_Buf_t Debayer1_Buffer_2 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  6,        // *** number of lines fitting into output buffer (wrap) ***
  DEBAYER_OUTPIX * 2,       // *** number of bytes per line ***
  BUF_ALIGN(DEBAYER_STRIDE16),        // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 756 /*Debayer1*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 1388 /* RGB2Y1.mHead */, // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      DEBAYER_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 2612 /* COMPRESS1.mHead */,  // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      DEBAYER_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // Debayer1_Buffer_2

SEQ_Buf_t RGB2Y0_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  5,        // *** number of lines fitting into output buffer (wrap) ***
  RGB2YUV_PIX * 2,        // *** number of bytes per line ***
  BUF_ALIGN(OUT_STRIDE8_YUV),       // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 1080 /*RGB2Y0*/,   // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 1696 /* LUT4K0.mHead */, // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      RGB2YUV_PIX * 2,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 2280 /* COMPRESS0.mHead */,  // *** offset to consumers (node) configuration header ***
      1,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      RGB2YUV_PIX * 2,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // RGB2Y0_Buffer_0

SEQ_Buf_t RGB2Y1_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  5,        // *** number of lines fitting into output buffer (wrap) ***
  RGB2YUV_PIX * 2,        // *** number of bytes per line ***
  BUF_ALIGN(RGB2YUV_STRIDE16),        // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 1388 /*RGB2Y1*/,   // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 1988 /* LUT4K1.mHead */, // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      RGB2YUV_PIX * 2,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 2612 /* COMPRESS1.mHead */,  // *** offset to consumers (node) configuration header ***
      1,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      RGB2YUV_PIX * 2,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // RGB2Y1_Buffer_0

SEQ_Buf_t LUT4K0_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  3,        // *** number of lines fitting into output buffer (wrap) ***
  RGB2YUV_PIX * 2,        // *** number of bytes per line ***
  BUF_ALIGN(RGB2YUV_STRIDE16),        // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 1696 /*LUT4K0*/,   // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 2280 /* COMPRESS0.mHead */,  // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      RGB2YUV_PIX * 2,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // LUT4K0_Buffer_0

SEQ_Buf_t LUT4K1_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  3,        // *** number of lines fitting into output buffer (wrap) ***
  RGB2YUV_PIX * 2,        // *** number of bytes per line ***
  BUF_ALIGN(RGB2YUV_STRIDE16),        // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 1988 /*LUT4K1*/,   // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 2612 /* COMPRESS1.mHead */,  // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      RGB2YUV_PIX * 2,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // LUT4K1_Buffer_0

SEQ_Buf_t COMPRESS0_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  4,        // *** number of lines fitting into output buffer (wrap) ***
  OUT_PIXELS_UV * 2,        // *** number of bytes per line ***
  BUF_ALIGN(OUT_STRIDE16_UV),       // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 2280 /*COMPRESS0*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 3560 /* DENOISE_UV.mHead */, // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      OUT_PIXELS_UV * 2,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 3560 /* DENOISE_UV.mHead */, // *** offset to consumers (node) configuration header ***
      2,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      2,        // *** number of lines required for processing ***
      OUT_PIXELS_UV * 2,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // COMPRESS0_Buffer_0

SEQ_Buf_t COMPRESS0_Buffer_1 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  3,        // *** number of lines fitting into output buffer (wrap) ***
  COMPRESS_PIX * 2,       // *** number of bytes per line ***
  BUF_ALIGN(COMPRESS_STRIDE16),       // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 2280 /*COMPRESS0*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 2944 /* DENOISEY2_0.mHead */,  // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      COMPRESS_PIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 2944 /* DENOISEY2_0.mHead */,  // *** offset to consumers (node) configuration header ***
      2,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      2,        // *** number of lines required for processing ***
      COMPRESS_PIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 3252 /* DENOISEY2_1.mHead */,  // *** offset to consumers (node) configuration header ***
      1,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      2,        // *** number of lines required for processing ***
      COMPRESS_PIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // COMPRESS0_Buffer_1

SEQ_Buf_t COMPRESS1_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  4,        // *** number of lines fitting into output buffer (wrap) ***
  OUT_PIXELS_UV * 2,        // *** number of bytes per line ***
  BUF_ALIGN(OUT_STRIDE16_UV),       // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 2612 /*COMPRESS1*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 3560 /* DENOISE_UV.mHead */, // *** offset to consumers (node) configuration header ***
      1,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      OUT_PIXELS_UV * 2,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 3560 /* DENOISE_UV.mHead */, // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      2,        // *** number of lines required for processing ***
      OUT_PIXELS_UV * 2,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // COMPRESS1_Buffer_0

SEQ_Buf_t COMPRESS1_Buffer_1 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  3,        // *** number of lines fitting into output buffer (wrap) ***
  COMPRESS_PIX * 2,       // *** number of bytes per line ***
  BUF_ALIGN(COMPRESS_STRIDE16),       // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 2612 /*COMPRESS1*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 2944 /* DENOISEY2_0.mHead */,  // *** offset to consumers (node) configuration header ***
      1,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      COMPRESS_PIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 3252 /* DENOISEY2_1.mHead */,  // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      COMPRESS_PIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 3252 /* DENOISEY2_1.mHead */,  // *** offset to consumers (node) configuration header ***
      2,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      2,        // *** number of lines required for processing ***
      COMPRESS_PIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // COMPRESS1_Buffer_1

SEQ_Buf_t DENOISE_UV_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  3,        // *** number of lines fitting into output buffer (wrap) ***
  OUT_PIXELS_UV * 1,        // *** number of bytes per line ***
  BUF_ALIGN(OUT_STRIDE8_UV),        // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 3560 /*DENOISE_UV*/,   // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 4504 /* FastDMA.mHead */,  // *** offset to consumers (node) configuration header ***
      1,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      OUT_PIXELS_UV * 1,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // DENOISE_UV_Buffer_0

SEQ_Buf_t DENOISEY2_0_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  3,        // *** number of lines fitting into output buffer (wrap) ***
  OUT_PIXELS * 1,       // *** number of bytes per line ***
  BUF_ALIGN(OUT_STRIDE8_Y),       // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 2944 /*DENOISEY2_0*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 4504 /* FastDMA.mHead */,  // *** offset to consumers (node) configuration header ***
      2,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      OUT_PIXELS * 1,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // DENOISEY2_0_Buffer_0

SEQ_Buf_t DENOISEY2_1_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  6,        // *** number of lines fitting into output buffer (wrap) ***
  OUT_PIXELS * 1,       // *** number of bytes per line ***
  BUF_ALIGN(OUT_STRIDE8_Y),       // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 3252 /*DENOISEY2_1*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 4504 /* FastDMA.mHead */,  // *** offset to consumers (node) configuration header ***
      3,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      1,        // *** number of lines required for processing ***
      OUT_PIXELS * 1,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // DENOISEY2_1_Buffer_0

SEQ_Buf_t INLUT_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,    // *** pointer to output buffer ***
  5,        // *** number of lines fitting into output buffer (wrap) ***
  INLUT_OUTPIX * 2,       // *** number of bytes per line ***
  BUF_ALIGN(INLUT_STRIDE16),        // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 4176 /*INLUT*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 3876 /* HDR.mHead */,  // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      8,        // *** number of lines required for processing ***
      INLUT_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 124 /* Histogram.mHead */, // *** offset to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      8,        // *** number of lines required for processing ***
      INLUT_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      4,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 124 /* Histogram.mHead */, // *** offset to consumers (node) configuration header ***
      1,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      8,        // *** number of lines required for processing ***
      INLUT_OUTPIX * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      4,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // INLUT_Buffer_0

SEQ_Buf_t FastDMA_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 1,    // *** pointer to output buffer ***
  CAM_LINES,        // *** number of lines fitting into output buffer (wrap) ***
  CAM_PIXELS * 2,       // *** number of bytes per line ***
  CAM_SAVE_STRIDE16,        // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 4504 /*FastDMA*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      CAM_PIXELS * 2,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // FastDMA_Buffer_0

SEQ_Buf_t FastDMA_Buffer_1 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 1,    // *** pointer to output buffer ***
  OUT_LINESUV2_0,       // *** number of lines fitting into output buffer (wrap) ***
  OUT_PIXELS_UV * 1,        // *** number of bytes per line ***
  OUT_STRIDE8_UV,       // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 4504 /*FastDMA*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      OUT_PIXELS_UV * 1,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // FastDMA_Buffer_1

SEQ_Buf_t FastDMA_Buffer_2 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 1,    // *** pointer to output buffer ***
  OUT_LINES2_0,       // *** number of lines fitting into output buffer (wrap) ***
  OUT_PIXELS * 1,       // *** number of bytes per line ***
  OUT_STRIDE8_Y,        // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 4504 /*FastDMA*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      OUT_PIXELS * 1,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // FastDMA_Buffer_2

SEQ_Buf_t FastDMA_Buffer_3 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 1,    // *** pointer to output buffer ***
  OUT_LINES2_1,       // *** number of lines fitting into output buffer (wrap) ***
  OUT_PIXELS * 1,       // *** number of bytes per line ***
  OUT_STRIDE8_Y,        // *** number of bytes to next line begin ***

  // *** producer/write ***
  0,        // *** buffers status ***
  (SEQ_Head_Ptr_t) 4504 /*FastDMA*/,    // *** offset to producers (node) configuration header ***
  0,        // *** current relative index into output buffer (write) ***
  1,        // *** increment of current relative index ***
  0,        // *** total lines written to buffer ***
  0,        // *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      OUT_PIXELS * 1,       // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      1,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,   // *** pointer to consumers (node) configuration header ***
      0,        // *** the devices channel used ***
      0,        // *** number of lines to skip at frame begin ***
      0,        // *** number of lines required for processing ***
      0,        // *** number of bytes per line to read ***
      0,        // *** current relative index into source buffer (read) ***
      0,        // *** increment of current relative index ***
      0,        // *** total lines read from buffer ***
      0,        // *** buffer is ready to be read by this consumer ***
      0,        // *** maximum difference between read and write ***
      0,        // *** pad aligment ***
    },

  }
}; // FastDMA_Buffer_3


static SEQ_CfgPara_t HistogramPara [] = {
    { D_IPUS_SYN_HOST_XCFG_OFF, (HIST_PROC << 16) | 0x1 },
    { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000700 },
    { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000000 },
    { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00050000 },
    { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x00050000 },
    { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x00050000 },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x00000000 },
};

static SEQ_CfgPara_t Debayer0Para [] = {
    { D_IPUS_SYN_HOST_XCFG_OFF, (DEBAYER_OUTPIX << 16) | 0x1 },
    { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000700 },
    { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000700 },
    { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00150000 },
    { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x00150000 },
    { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x00150000 },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x00000000 },
    { D_IPUS_SYN_S_CH1_CFG_OUT_OFF, 0x00000000 },
    { D_IPUS_SYN_S_CH2_CFG_OUT_OFF, 0x00000000 },
};

static SEQ_CfgPara_t Debayer1Para [] = {
    { D_IPUS_SYN_HOST_XCFG_OFF, (DEBAYER_OUTPIX << 16) | 0x1 },
    { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000700 },
    { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000700 },
    { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00150000 },
    { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x00150000 },
    { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x00150000 },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x00000000 },
    { D_IPUS_SYN_S_CH1_CFG_OUT_OFF, 0x00000000 },
    { D_IPUS_SYN_S_CH2_CFG_OUT_OFF, 0x00000000 },
};

static SEQ_CfgPara_t RGB2Y0Para [] = {
    { D_IPUS_SYN_HOST_XCFG_OFF, (RGB2YUV_PIX << 16) | 0x0 },
    { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000700 },
    { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100 },
    { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00158000 },
    { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x00158000 },
    { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x00158000 },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x00000000 },
};

static SEQ_CfgPara_t RGB2Y1Para [] = {
    { D_IPUS_SYN_HOST_XCFG_OFF, (RGB2YUV_PIX << 16) | 0x0 },
    { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000700 },
    { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100 },
    { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00158000 },
    { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x00158000 },
    { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x00158000 },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x00000000 },
};

static SEQ_CfgPara_t LUT4K0Para [] = {
    { D_IPUS_SYN_HOST_XCFG_OFF, (RGB2YUV_PIX << 16) | 0x1 },
    { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000100 },
    { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100 },
    { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00058000 },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x00000000 },
};

static SEQ_CfgPara_t LUT4K1Para [] = {
    { D_IPUS_SYN_HOST_XCFG_OFF, (RGB2YUV_PIX << 16) | 0x1 },
    { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000100 },
    { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100 },
    { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00058000 },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x00000000 },
};

static SEQ_CfgPara_t COMPRESS0Para [] = {
    { D_IPUS_SYN_HOST_XCFG_OFF, (COMPRESS_PIX << 16) | 0x0 },
    { D_IPUS_SYN_HOST_INACFG_OFF, 0x00001f00 },
    { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000300 | OUTPUT_SHIFT },
    { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00051000 },
    { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x00058000 },
    { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x0005e000 },
    { D_IPUS_SYN_S_CH0_CFG_INB_OFF, 0x00050100 },
    { D_IPUS_SYN_S_CH1_CFG_INB_OFF, 0x0005ffff },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x00000000 },
    { D_IPUS_SYN_S_CH1_CFG_OUT_OFF, 0x00000000 },
};

static SEQ_CfgPara_t COMPRESS1Para [] = {
    { D_IPUS_SYN_HOST_XCFG_OFF, (COMPRESS_PIX << 16) | 0x0 },
    { D_IPUS_SYN_HOST_INACFG_OFF, 0x00001f00 },
    { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000300 | OUTPUT_SHIFT },
    { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00051000 },
    { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x00058000 },
    { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x0005e000 },
    { D_IPUS_SYN_S_CH0_CFG_INB_OFF, 0x00050100 },
    { D_IPUS_SYN_S_CH1_CFG_INB_OFF, 0x0005ffff },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x00000000 },
    { D_IPUS_SYN_S_CH1_CFG_OUT_OFF, 0x00000000 },
};

static SEQ_CfgPara_t DENOISEY2_0Para [] = {
    { D_IPUS_SYN_HOST_XCFG_OFF, (DENOISE_OUTPIX << 16) | 0x1 },
    { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000700 },
    { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100 },
    { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00158000 },
    { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x00158000 },
    { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x00158000 },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x10000000 },
};

static SEQ_CfgPara_t DENOISEY2_1Para [] = {
    { D_IPUS_SYN_HOST_XCFG_OFF, (DENOISE_OUTPIX << 16) | 0x1 },
    { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000700 },
    { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100 },
    { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00158000 },
    { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x00158000 },
    { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x00158000 },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x10000000 },
};

static SEQ_CfgPara_t DENOISE_UVPara [] = {
    { D_IPUV_SYN_HOST_XCFG_OFF, (COMPRESS_PIX << 16) | 0x0 },
    { D_IPUV_SYN_HOST_INACFG_OFF, 0x00000f00 },
    { D_IPUV_SYN_HOST_OUTCFG_OFF, 0x00000100 },
    { D_IPUV_SYN_S_CH0_CFG_INA_OFF, 0x00050000 },
    { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x00050000 },
    { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x00050000 },
    { D_IPUS_SYN_S_CH0_CFG_INB_OFF, 0x00050000 },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x10000000 },
};

static SEQ_CfgPara_t HDRPara [] = {
    { D_IPUS_SYN_HOST_XCFG_OFF, (HDR_PROC << 16) | 0x0 },
    { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000300 },
    { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100 },
    { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00050000 },
    { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x00050000 },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x00000000 },
};

static SEQ_CfgPara_t INLUTPara [] = {
    { D_IPUS_SYN_HOST_XCFG_OFF, (INLUT_OUTPIX << 16) | 0x1 },
    { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000100 },
    { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100 },
    { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00050000 },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x00000000 },
};

static SEQ_MipiCfg_t OV_QUAD = {
      {
        SEQ_NODE_OTHER,     // *** type of engine ***
        ISP_IO_DEV_BASE+ISP_OFFS_FILEIO,  // *** address of hardware device ***
        SEQ_OTHRIX_MIPICSI0,      // *** index of requested engine ***
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },


      // *** output buffers ***
      {
        (SEQ_Buf_Ptr_t) 4712 /*OV_QUAD_Buffer_0*/,      // *** pointer to output buffer ***

        (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***

        (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***

        (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***

      },
      CAM_LINES,      // *** number of input image lines ***
      0       // *** pad alignment ***
    }; // OV_QUAD

static SEQ_IpuCfg_t Histogram = {
      {
        SEQ_NODE_SCALAR,      // *** type of engine ***
        D_IPUS_SYN_BASEADDRESS,   // *** address of hardware device ***
        0,      // *** index of requested engine *** (TODO resolve for -1)
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },

      0x00000000,     // *** code entry address *** (TODO)

      0,          // *** current processing steps ***
      HIST_LINES,         // *** number of processing steps per frame ***

      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
      },  // output buffer structs

      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 8816 /* INLUT_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            HIST_OFFSET0 * 2,         // *** number of bytes to first pixel in line ***
            IN_PIXELS,          // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 8816 /* INLUT_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE1_LEN_INA_OFF,       // *** offset to SDMA's register ***
            1,          // *** number of first line to be used ***
            HIST_OFFSET0 * 2,         // *** number of bytes to first pixel in line ***
            IN_PIXELS,          // *** number of pixels to transfer ***
          },
          2,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 4712 /* OV_QUAD_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE2_LEN_INA_OFF,       // *** offset to SDMA's register ***
            VS_LINE_DELAY,          // *** number of first line to be used ***
            HIST_OFFSETVS0 * 2,         // *** number of bytes to first pixel in line ***
            IN_PIXELS,          // *** number of pixels to transfer ***
          },
          3,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
      },  // input buffer structs
      (SEQ_CfgPara_CPtr_t) 348 /*HistogramPara */,  // *** pointer to configuration parameters ***
      7,          // *** number of configuration parameters following ***
      0,        // *** pad alignment ***
    };

static SEQ_IpuCfg_t Debayer0 = {
      {
        SEQ_NODE_SCALAR,      // *** type of engine ***
        D_IPUS_SYN_BASEADDRESS + 1*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
        1,      // *** index of requested engine *** (TODO resolve for -1)
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },

      0x00000000,     // *** code entry address *** (TODO)

      0,          // *** current processing steps ***
      DEBAYER_LINES2_0,         // *** number of processing steps per frame ***

      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 5144 /* Debayer0_Buffer_0 */,     // *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 5360 /* Debayer0_Buffer_1 */,     // *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE1_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 5576 /* Debayer0_Buffer_2 */,     // *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE2_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
      },  // output buffer structs

      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 4928 /* HDR_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_INPIX,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 4928 /* HDR_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE1_LEN_INA_OFF,       // *** offset to SDMA's register ***
            1,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_INPIX,          // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 4928 /* HDR_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE2_LEN_INA_OFF,       // *** offset to SDMA's register ***
            2,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_INPIX,          // *** number of pixels to transfer ***
          },
          2,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
      },  // input buffer structs
      (SEQ_CfgPara_CPtr_t) 656 /*Debayer0Para */, // *** pointer to configuration parameters ***
      9,          // *** number of configuration parameters following ***
      0,        // *** pad alignment ***
    };

static SEQ_IpuCfg_t Debayer1 = {
      {
        SEQ_NODE_SCALAR,      // *** type of engine ***
        D_IPUS_SYN_BASEADDRESS + 4*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
        4,      // *** index of requested engine *** (TODO resolve for -1)
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },

      0x00000000,     // *** code entry address *** (TODO)

      0,          // *** current processing steps ***
      DEBAYER_LINES2_1,         // *** number of processing steps per frame ***

      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 5792 /* Debayer1_Buffer_0 */,     // *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 6008 /* Debayer1_Buffer_1 */,     // *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE1_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 6224 /* Debayer1_Buffer_2 */,     // *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE2_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
      },  // output buffer structs

      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 4928 /* HDR_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_INA_OFF,       // *** offset to SDMA's register ***
            1,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_INPIX,          // *** number of pixels to transfer ***
          },
          3,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 4928 /* HDR_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE1_LEN_INA_OFF,       // *** offset to SDMA's register ***
            2,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_INPIX,          // *** number of pixels to transfer ***
          },
          4,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 4928 /* HDR_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE2_LEN_INA_OFF,       // *** offset to SDMA's register ***
            3,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_INPIX,          // *** number of pixels to transfer ***
          },
          5,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
      },  // input buffer structs
      (SEQ_CfgPara_CPtr_t) 980 /*Debayer1Para */, // *** pointer to configuration parameters ***
      9,          // *** number of configuration parameters following ***
      0,        // *** pad alignment ***
    };

static SEQ_IpuCfg_t RGB2Y0 = {
      {
        SEQ_NODE_VECTOR,      // *** type of engine ***
        D_IPUV_SYN_BASEADDRESS,   // *** address of hardware device ***
        0,      // *** index of requested engine *** (TODO resolve for -1)
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },

      0x00000000,     // *** code entry address *** (TODO)

      0,          // *** current processing steps ***
      RGB2YUV_LINES2_0,         // *** number of processing steps per frame ***

      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 6440 /* RGB2Y0_Buffer_0 */,     // *** pointer to output buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            RGB2YUV_PIX,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
      },  // output buffer structs

      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 5576 /* Debayer0_Buffer_2 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 5144 /* Debayer0_Buffer_0 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE1_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 5360 /* Debayer0_Buffer_1 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE2_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
      },  // input buffer structs
      (SEQ_CfgPara_CPtr_t) 1304 /*RGB2Y0Para */,  // *** pointer to configuration parameters ***
      7,          // *** number of configuration parameters following ***
      0,        // *** pad alignment ***
    };

static SEQ_IpuCfg_t RGB2Y1 = {
      {
        SEQ_NODE_VECTOR,      // *** type of engine ***
        D_IPUV_SYN_BASEADDRESS + 3*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
        3,      // *** index of requested engine *** (TODO resolve for -1)
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },

      0x00000000,     // *** code entry address *** (TODO)

      0,          // *** current processing steps ***
      RGB2YUV_LINES2_1,         // *** number of processing steps per frame ***

      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 6656 /* RGB2Y1_Buffer_0 */,     // *** pointer to output buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            RGB2YUV_PIX,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
      },  // output buffer structs

      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 6224 /* Debayer1_Buffer_2 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 5792 /* Debayer1_Buffer_0 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE1_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 6008 /* Debayer1_Buffer_1 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE2_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
      },  // input buffer structs
      (SEQ_CfgPara_CPtr_t) 1612 /*RGB2Y1Para */,  // *** pointer to configuration parameters ***
      7,          // *** number of configuration parameters following ***
      0,        // *** pad alignment ***
    };

static SEQ_IpuCfg_t LUT4K0 = {
      {
        SEQ_NODE_SCALAR,      // *** type of engine ***
        D_IPUS_SYN_BASEADDRESS + 3*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
        3,      // *** index of requested engine *** (TODO resolve for -1)
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },

      0x00000000,     // *** code entry address *** (TODO)

      0,          // *** current processing steps ***
      RGB2YUV_LINES2_0,         // *** number of processing steps per frame ***

      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 6872 /* LUT4K0_Buffer_0 */,     // *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            RGB2YUV_PIX,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
      },  // output buffer structs

      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 6440 /* RGB2Y0_Buffer_0 */,     // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            RGB2YUV_PIX,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
      },  // input buffer structs
      (SEQ_CfgPara_CPtr_t) 1920 /*LUT4K0Para */,  // *** pointer to configuration parameters ***
      5,          // *** number of configuration parameters following ***
      0,        // *** pad alignment ***
    };

static SEQ_IpuCfg_t LUT4K1 = {
      {
        SEQ_NODE_SCALAR,      // *** type of engine ***
        D_IPUS_SYN_BASEADDRESS + 3*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
        3,      // *** index of requested engine *** (TODO resolve for -1)
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },

      0x00000040,     // *** code entry address *** (TODO)

      0,          // *** current processing steps ***
      RGB2YUV_LINES2_1,         // *** number of processing steps per frame ***

      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7088 /* LUT4K1_Buffer_0 */,     // *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            RGB2YUV_PIX,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
      },  // output buffer structs

      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 6656 /* RGB2Y1_Buffer_0 */,     // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            RGB2YUV_PIX,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
      },  // input buffer structs
      (SEQ_CfgPara_CPtr_t) 2212 /*LUT4K1Para */,  // *** pointer to configuration parameters ***
      5,          // *** number of configuration parameters following ***
      0,        // *** pad alignment ***
    };

static SEQ_IpuCfg_t COMPRESS0 = {
      {
        SEQ_NODE_VECTOR,      // *** type of engine ***
        D_IPUV_SYN_BASEADDRESS + 1*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
        1,      // *** index of requested engine *** (TODO resolve for -1)
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },

      0x00000000,     // *** code entry address *** (TODO)

      0,          // *** current processing steps ***
      COMPRESS_LINES2_0,          // *** number of processing steps per frame ***

      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7304 /* COMPRESS0_Buffer_0 */,      // *** pointer to output buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            COMPRESS_PIX,         // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7520 /* COMPRESS0_Buffer_1 */,      // *** pointer to output buffer ***
          {
            D_IPUV_SYN_S_LINE1_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            COMPRESS_PIX,         // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
      },  // output buffer structs

      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 5576 /* Debayer0_Buffer_2 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 5144 /* Debayer0_Buffer_0 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE1_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 5360 /* Debayer0_Buffer_1 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE2_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 6872 /* LUT4K0_Buffer_0 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_INB_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            RGB2YUV_PIX,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 6440 /* RGB2Y0_Buffer_0 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE1_LEN_INB_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            RGB2YUV_PIX,          // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
      },  // input buffer structs
      (SEQ_CfgPara_CPtr_t) 2504 /*COMPRESS0Para */, // *** pointer to configuration parameters ***
      10,         // *** number of configuration parameters following ***
      0,        // *** pad alignment ***
    };

static SEQ_IpuCfg_t COMPRESS1 = {
      {
        SEQ_NODE_VECTOR,      // *** type of engine ***
        D_IPUV_SYN_BASEADDRESS + 2*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
        2,      // *** index of requested engine *** (TODO resolve for -1)
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },

      0x00000000,     // *** code entry address *** (TODO)

      0,          // *** current processing steps ***
      COMPRESS_LINES2_1,          // *** number of processing steps per frame ***

      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7736 /* COMPRESS1_Buffer_0 */,      // *** pointer to output buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            COMPRESS_PIX,         // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7952 /* COMPRESS1_Buffer_1 */,      // *** pointer to output buffer ***
          {
            D_IPUV_SYN_S_LINE1_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            COMPRESS_PIX,         // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
      },  // output buffer structs

      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 6224 /* Debayer1_Buffer_2 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 5792 /* Debayer1_Buffer_0 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE1_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 6008 /* Debayer1_Buffer_1 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE2_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DEBAYER_OUTPIX,         // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7088 /* LUT4K1_Buffer_0 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_INB_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            RGB2YUV_PIX,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 6656 /* RGB2Y1_Buffer_0 */,     // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE1_LEN_INB_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            RGB2YUV_PIX,          // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
      },  // input buffer structs
      (SEQ_CfgPara_CPtr_t) 2836 /*COMPRESS1Para */, // *** pointer to configuration parameters ***
      10,         // *** number of configuration parameters following ***
      0,        // *** pad alignment ***
    };

static SEQ_IpuCfg_t DENOISEY2_0 = {
      {
        SEQ_NODE_SCALAR,      // *** type of engine ***
        D_IPUS_SYN_BASEADDRESS + 5*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
        5,      // *** index of requested engine *** (TODO resolve for -1)
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },

      0x00000000,     // *** code entry address *** (TODO)

      0,          // *** current processing steps ***
      DENOISE_LINES2_0,         // *** number of processing steps per frame ***

      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 8384 /* DENOISEY2_0_Buffer_0 */,      // *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DENOISE_OUTPIX,         // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
      },  // output buffer structs

      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7520 /* COMPRESS0_Buffer_1 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DENOISE_INPIX,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7952 /* COMPRESS1_Buffer_1 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE1_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DENOISE_INPIX,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7520 /* COMPRESS0_Buffer_1 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE2_LEN_INA_OFF,       // *** offset to SDMA's register ***
            1,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DENOISE_INPIX,          // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
      },  // input buffer structs
      (SEQ_CfgPara_CPtr_t) 3168 /*DENOISEY2_0Para */, // *** pointer to configuration parameters ***
      7,          // *** number of configuration parameters following ***
      0,        // *** pad alignment ***
    };

static SEQ_IpuCfg_t DENOISEY2_1 = {
      {
        SEQ_NODE_SCALAR,      // *** type of engine ***
        D_IPUS_SYN_BASEADDRESS + 6*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
        6,      // *** index of requested engine *** (TODO resolve for -1)
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },

      0x00000000,     // *** code entry address *** (TODO)

      0,          // *** current processing steps ***
      DENOISE_LINES2_1,         // *** number of processing steps per frame ***

      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 8600 /* DENOISEY2_1_Buffer_0 */,      // *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DENOISE_OUTPIX,         // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
      },  // output buffer structs

      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7952 /* COMPRESS1_Buffer_1 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DENOISE_INPIX,          // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7520 /* COMPRESS0_Buffer_1 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE1_LEN_INA_OFF,       // *** offset to SDMA's register ***
            1,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DENOISE_INPIX,          // *** number of pixels to transfer ***
          },
          2,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7952 /* COMPRESS1_Buffer_1 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE2_LEN_INA_OFF,       // *** offset to SDMA's register ***
            1,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            DENOISE_INPIX,          // *** number of pixels to transfer ***
          },
          2,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
      },  // input buffer structs
      (SEQ_CfgPara_CPtr_t) 3476 /*DENOISEY2_1Para */, // *** pointer to configuration parameters ***
      7,          // *** number of configuration parameters following ***
      0,        // *** pad alignment ***
    };

static SEQ_IpuCfg_t DENOISE_UV = {
      {
        SEQ_NODE_VECTOR,      // *** type of engine ***
        D_IPUV_SYN_BASEADDRESS,   // *** address of hardware device ***
        0,      // *** index of requested engine *** (TODO resolve for -1)
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },

      0x00000080,     // *** code entry address *** (TODO)

      0,          // *** current processing steps ***
      OUT_LINESUV2_0,         // *** number of processing steps per frame ***

      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 8168 /* DENOISE_UV_Buffer_0 */,     // *** pointer to output buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            OUT_PIXELS_UV,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
      },  // output buffer structs

      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7304 /* COMPRESS0_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            OUT_PIXELS_UV,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7736 /* COMPRESS1_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE1_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            OUT_PIXELS_UV,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7304 /* COMPRESS0_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE2_LEN_INA_OFF,       // *** offset to SDMA's register ***
            1,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            OUT_PIXELS_UV,          // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 7736 /* COMPRESS1_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_INB_OFF,       // *** offset to SDMA's register ***
            1,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            OUT_PIXELS_UV,          // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
      },  // input buffer structs
      (SEQ_CfgPara_CPtr_t) 3784 /*DENOISE_UVPara */,  // *** pointer to configuration parameters ***
      8,          // *** number of configuration parameters following ***
      0,        // *** pad alignment ***
    };

static SEQ_IpuCfg_t HDR = {
      {
        SEQ_NODE_VECTOR,      // *** type of engine ***
        D_IPUV_SYN_BASEADDRESS + 3*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
        3,      // *** index of requested engine *** (TODO resolve for -1)
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },

      0x00000040,     // *** code entry address *** (TODO)

      0,          // *** current processing steps ***
      HDR_LINES,          // *** number of processing steps per frame ***

      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 4928 /* HDR_Buffer_0 */,      // *** pointer to output buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            HDR_OUTPIX,         // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
      },  // output buffer structs

      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 8816 /* INLUT_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE0_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            RED_LSSTARTPIX * 2,         // *** number of bytes to first pixel in line ***
            IN_PIXELS,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 4712 /* OV_QUAD_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUV_SYN_S_LINE1_LEN_INA_OFF,       // *** offset to SDMA's register ***
            VS_LINE_DELAY,          // *** number of first line to be used ***
            RED_VSSTARTPIX * 2,         // *** number of bytes to first pixel in line ***
            IN_PIXELS,          // *** number of pixels to transfer ***
          },
          2,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
      },  // input buffer structs
      (SEQ_CfgPara_CPtr_t) 4100 /*HDRPara */, // *** pointer to configuration parameters ***
      6,          // *** number of configuration parameters following ***
      0,        // *** pad alignment ***
    };

static SEQ_IpuCfg_t INLUT = {
      {
        SEQ_NODE_SCALAR,      // *** type of engine ***
        D_IPUS_SYN_BASEADDRESS + 2*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
        2,      // *** index of requested engine *** (TODO resolve for -1)
        0,          // *** engine is running ***
        0,          // *** engine is done for this frame ***
        0,          // *** engine got some update ***
      },

      0x00000000,     // *** code entry address *** (TODO)

      0,          // *** current processing steps ***
      INLUT_LINES,          // *** number of processing steps per frame ***

      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 8816 /* INLUT_Buffer_0 */,      // *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_OUT_OFF, // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            INLUT_OUTPIX,         // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,      // *** pointer to output buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
        },
      },  // output buffer structs

      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 4712 /* OV_QUAD_Buffer_0 */,      // *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_INA_OFF,       // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            RED_LSSTARTPIX * 2,         // *** number of bytes to first pixel in line ***
            IN_PIXELS,          // *** number of pixels to transfer ***
          },
          1,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 0,        // *** pointer to input buffer ***
          {
            0,          // *** offset to SDMA's register ***
            0,          // *** number of first line to be used ***
            0,          // *** number of bytes to first pixel in line ***
            0,          // *** number of pixels to transfer ***
          },
          0,        // *** our index in buffers consumer array ***
          0,        // *** pad alignment ***
        },
      },  // input buffer structs
      (SEQ_CfgPara_CPtr_t) 4400 /*INLUTPara */, // *** pointer to configuration parameters ***
      5,          // *** number of configuration parameters following ***
      0,        // *** pad alignment ***
    };

static SEQ_FdmaCfg_t FastDMA = {
  {
    SEQ_NODE_OTHER,     // *** type of engine ***
    (Addr_t)(uintptr_t)&FDMA, // *** address of hardware device ***
    SEQ_OTHRIX_FDMA,      // *** index of requested engine ***
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  {
    // *** output buffer #0 ***
    {
      (SEQ_Buf_Ptr_t) 9032 /* FastDMA_Buffer_0 */,  // *** offset to destination buffer ***
    },

    // *** output buffer #1 ***
    {
      (SEQ_Buf_Ptr_t) 9248 /* FastDMA_Buffer_1 */,  // *** offset to destination buffer ***
    },

    // *** output buffer #2 ***
    {
      (SEQ_Buf_Ptr_t) 9464 /* FastDMA_Buffer_2 */,  // *** offset to destination buffer ***
    },

    // *** output buffer #3 ***
    {
      (SEQ_Buf_Ptr_t) 9680 /* FastDMA_Buffer_3 */,  // *** offset to destination buffer ***
    },

    // *** output buffer #4 ***
    {
      (SEQ_Buf_Ptr_t) 0,      // *** pointer to destination buffer ***
    },

    // *** output buffer #5 ***
    {
      (SEQ_Buf_Ptr_t) 0,      // *** pointer to destination buffer ***
    },

    // *** output buffer #6 ***
    {
      (SEQ_Buf_Ptr_t) 0,      // *** pointer to destination buffer ***
    },

    // *** output buffer #7 ***
    {
      (SEQ_Buf_Ptr_t) 0,      // *** pointer to destination buffer ***
    },

    // *** output buffer #8 ***
    {
      (SEQ_Buf_Ptr_t) 0,      // *** pointer to destination buffer ***
    },

    // *** output buffer #9 ***
    {
      (SEQ_Buf_Ptr_t) 0,      // *** pointer to destination buffer ***
    },

    // *** output buffer #10 ***
    {
      (SEQ_Buf_Ptr_t) 0,      // *** pointer to destination buffer ***
    },

    // *** output buffer #11 ***
    {
      (SEQ_Buf_Ptr_t) 0,      // *** pointer to destination buffer ***
    },

    // *** output buffer #12 ***
    {
      (SEQ_Buf_Ptr_t) 0,      // *** pointer to destination buffer ***
    },

    // *** output buffer #13 ***
    {
      (SEQ_Buf_Ptr_t) 0,      // *** pointer to destination buffer ***
    },

    // *** output buffer #14 ***
    {
      (SEQ_Buf_Ptr_t) 0,      // *** pointer to destination buffer ***
    },

    // *** output buffer #15 ***
    {
      (SEQ_Buf_Ptr_t) 0,      // *** pointer to destination buffer ***
    },

  },

  // ### inputs ###############################################################

  {
    {
      // *** input buffer #0 ***
      (SEQ_Buf_Ptr_t) 4712 /* OV_QUAD_Buffer_0 */,  // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #1 ***
      (SEQ_Buf_Ptr_t) 8168 /* DENOISE_UV_Buffer_0 */, // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #2 ***
      (SEQ_Buf_Ptr_t) 8384 /* DENOISEY2_0_Buffer_0 */,  // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #3 ***
      (SEQ_Buf_Ptr_t) 8600 /* DENOISEY2_1_Buffer_0 */,  // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #4 ***
      (SEQ_Buf_Ptr_t) 0,      // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #5 ***
      (SEQ_Buf_Ptr_t) 0,      // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #6 ***
      (SEQ_Buf_Ptr_t) 0,      // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #7 ***
      (SEQ_Buf_Ptr_t) 0,      // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #8 ***
      (SEQ_Buf_Ptr_t) 0,      // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #9 ***
      (SEQ_Buf_Ptr_t) 0,      // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #10 ***
      (SEQ_Buf_Ptr_t) 0,      // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #11 ***
      (SEQ_Buf_Ptr_t) 0,      // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #12 ***
      (SEQ_Buf_Ptr_t) 0,      // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #13 ***
      (SEQ_Buf_Ptr_t) 0,      // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #14 ***
      (SEQ_Buf_Ptr_t) 0,      // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

    {
      // *** input buffer #15 ***
      (SEQ_Buf_Ptr_t) 0,      // *** input buffer ***
      0,          // *** index into buffers consumer array ***
      0,        // *** line offset ***
    },

  },
}; // FastDMA

static SEQ_Load_t sCodehistogram_Histogram = {
  {
    SEQ_NODE_FWLOAD,      // *** type of engine ***
    D_IPUS_SYN_BASEADDRESS,   // *** address of hardware device ***
    0,      // *** index of requested engine *** (TODO resolve for -1)
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  histogram_start,    // *** Kernel address in KMEM ***
  histogram_end - histogram_start,  // *** Kernel length in bytes ***
  0,            // *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,        // *** pad alignment ***
  (SEQ_Head_Ptr_t) 124 /* Histogram.mHead */,     // *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodedebayer_even_Debayer0 = {
  {
    SEQ_NODE_FWLOAD,      // *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 1*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
    1,      // *** index of requested engine *** (TODO resolve for -1)
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  debayer_even_start,   // *** Kernel address in KMEM ***
  debayer_even_end - debayer_even_start,  // *** Kernel length in bytes ***
  0,            // *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,        // *** pad alignment ***
  (SEQ_Head_Ptr_t) 432 /* Debayer0.mHead */,      // *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodedebayer_odd_Debayer1 = {
  {
    SEQ_NODE_FWLOAD,      // *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 4*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
    4,      // *** index of requested engine *** (TODO resolve for -1)
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  debayer_odd_start,    // *** Kernel address in KMEM ***
  debayer_odd_end - debayer_odd_start,  // *** Kernel length in bytes ***
  0,            // *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,        // *** pad alignment ***
  (SEQ_Head_Ptr_t) 756 /* Debayer1.mHead */,      // *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodergb2y_RGB2Y0 = {
  {
    SEQ_NODE_FWLOAD,      // *** type of engine ***
    D_IPUV_SYN_BASEADDRESS,   // *** address of hardware device ***
    0,      // *** index of requested engine *** (TODO resolve for -1)
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  rgb2y_start,    // *** Kernel address in KMEM ***
  rgb2y_end - rgb2y_start,  // *** Kernel length in bytes ***
  0,            // *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,        // *** pad alignment ***
  (SEQ_Head_Ptr_t) 1080 /* RGB2Y0.mHead */,     // *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodergb2y_RGB2Y1 = {
  {
    SEQ_NODE_FWLOAD,      // *** type of engine ***
    D_IPUV_SYN_BASEADDRESS + 3*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
    3,      // *** index of requested engine *** (TODO resolve for -1)
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  rgb2y_start,    // *** Kernel address in KMEM ***
  rgb2y_end - rgb2y_start,  // *** Kernel length in bytes ***
  0,            // *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,        // *** pad alignment ***
  (SEQ_Head_Ptr_t) 1388 /* RGB2Y1.mHead */,     // *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodelut4k_LUT4K0 = {
  {
    SEQ_NODE_FWLOAD,      // *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 3*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
    3,      // *** index of requested engine *** (TODO resolve for -1)
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  lut4k_start,    // *** Kernel address in KMEM ***
  lut4k_end - lut4k_start,  // *** Kernel length in bytes ***
  0,            // *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,        // *** pad alignment ***
  (SEQ_Head_Ptr_t) 1696 /* LUT4K0.mHead */,     // *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodelut4k_hist_LUT4K1 = {
  {
    SEQ_NODE_FWLOAD,      // *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 3*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
    3,      // *** index of requested engine *** (TODO resolve for -1)
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  lut4k_hist_start,   // *** Kernel address in KMEM ***
  lut4k_hist_end - lut4k_hist_start,  // *** Kernel length in bytes ***
  0x10,           // *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,        // *** pad alignment ***
  (SEQ_Head_Ptr_t) 1988 /* LUT4K1.mHead */,     // *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodergb_compress_y_uv_ccm_COMPRESS0 = {
  {
    SEQ_NODE_FWLOAD,      // *** type of engine ***
    D_IPUV_SYN_BASEADDRESS + 1*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
    1,      // *** index of requested engine *** (TODO resolve for -1)
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  rgb_compress_y_uv_ccm_start,    // *** Kernel address in KMEM ***
  rgb_compress_y_uv_ccm_end - rgb_compress_y_uv_ccm_start,  // *** Kernel length in bytes ***
  0,            // *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,        // *** pad alignment ***
  (SEQ_Head_Ptr_t) 2280 /* COMPRESS0.mHead */,      // *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodergb_compress_y_uv_ccm_COMPRESS1 = {
  {
    SEQ_NODE_FWLOAD,      // *** type of engine ***
    D_IPUV_SYN_BASEADDRESS + 2*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
    2,      // *** index of requested engine *** (TODO resolve for -1)
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  rgb_compress_y_uv_ccm_start,    // *** Kernel address in KMEM ***
  rgb_compress_y_uv_ccm_end - rgb_compress_y_uv_ccm_start,  // *** Kernel length in bytes ***
  0,            // *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,        // *** pad alignment ***
  (SEQ_Head_Ptr_t) 2612 /* COMPRESS1.mHead */,      // *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodedenoise_median_DENOISEY2_0 = {
  {
    SEQ_NODE_FWLOAD,      // *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 5*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
    5,      // *** index of requested engine *** (TODO resolve for -1)
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  denoise_median_start,   // *** Kernel address in KMEM ***
  denoise_median_end - denoise_median_start,  // *** Kernel length in bytes ***
  0,            // *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,        // *** pad alignment ***
  (SEQ_Head_Ptr_t) 2944 /* DENOISEY2_0.mHead */,      // *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodedenoise_median_DENOISEY2_1 = {
  {
    SEQ_NODE_FWLOAD,      // *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 6*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
    6,      // *** index of requested engine *** (TODO resolve for -1)
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  denoise_median_start,   // *** Kernel address in KMEM ***
  denoise_median_end - denoise_median_start,  // *** Kernel length in bytes ***
  0,            // *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,        // *** pad alignment ***
  (SEQ_Head_Ptr_t) 3252 /* DENOISEY2_1.mHead */,      // *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodedenoise4x4_uv_DENOISE_UV = {
  {
    SEQ_NODE_FWLOAD,      // *** type of engine ***
    D_IPUV_SYN_BASEADDRESS,   // *** address of hardware device ***
    0,      // *** index of requested engine *** (TODO resolve for -1)
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  denoise4x4_uv_start,    // *** Kernel address in KMEM ***
  denoise4x4_uv_end - denoise4x4_uv_start,  // *** Kernel length in bytes ***
  0x20,           // *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,        // *** pad alignment ***
  (SEQ_Head_Ptr_t) 3560 /* DENOISE_UV.mHead */,     // *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodehdr_blend_HDR = {
  {
    SEQ_NODE_FWLOAD,      // *** type of engine ***
    D_IPUV_SYN_BASEADDRESS + 3*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
    3,      // *** index of requested engine *** (TODO resolve for -1)
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  hdr_blend_start,    // *** Kernel address in KMEM ***
  hdr_blend_end - hdr_blend_start,  // *** Kernel length in bytes ***
  0x10,           // *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,        // *** pad alignment ***
  (SEQ_Head_Ptr_t) 3876 /* HDR.mHead */,      // *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodelut4k_tee_INLUT = {
  {
    SEQ_NODE_FWLOAD,      // *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 2*ISP_IO_DEV_SPACE,    // *** address of hardware device ***
    2,      // *** index of requested engine *** (TODO resolve for -1)
    0,          // *** engine is running ***
    0,          // *** engine is done for this frame ***
    0,          // *** engine got some update ***
  },

  lut4k_tee_start,    // *** Kernel address in KMEM ***
  lut4k_tee_end - lut4k_tee_start,  // *** Kernel length in bytes ***
  0,            // *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,        // *** pad alignment ***
  (SEQ_Head_Ptr_t) 4176 /* INLUT.mHead */,      // *** pointer to IPU node to start ***
};


/*****************************************************************************
* exported data
*****************************************************************************/

// *** graph ***

SEQ_Head_Ptr_t gpGraph[] = {
  (SEQ_Head_Ptr_t) 4504, // FastDMA     // #0
  (SEQ_Head_Ptr_t) 4468, // OV_QUAD     // #1
  (SEQ_Head_Ptr_t) 124, // Histogram    // #2
  (SEQ_Head_Ptr_t) 432, // Debayer0     // #3
  (SEQ_Head_Ptr_t) 756, // Debayer1     // #4
  (SEQ_Head_Ptr_t) 1080, // RGB2Y0      // #5
  (SEQ_Head_Ptr_t) 1388, // RGB2Y1      // #6
  (SEQ_Head_Ptr_t) 1696, // LUT4K0      // #7
  (SEQ_Head_Ptr_t) 1988, // LUT4K1      // #8
  (SEQ_Head_Ptr_t) 2280, // COMPRESS0   // #9
  (SEQ_Head_Ptr_t) 2612, // COMPRESS1   // #10
  (SEQ_Head_Ptr_t) 2944, // DENOISEY2_0 // #11
  (SEQ_Head_Ptr_t) 3252, // DENOISEY2_1 // #12
  (SEQ_Head_Ptr_t) 3560, // DENOISE_UV  // #13
  (SEQ_Head_Ptr_t) 3876, // HDR         // #14
  (SEQ_Head_Ptr_t) 4176, // INLUT       // #15
  (SEQ_Head_Ptr_t) 404, // sCodehistogram_Histogram
  (SEQ_Head_Ptr_t) 728, // sCodedebayer_even_Debayer0
  (SEQ_Head_Ptr_t) 1052, // sCodedebayer_odd_Debayer1
  (SEQ_Head_Ptr_t) 1360, // sCodergb2y_RGB2Y0
  (SEQ_Head_Ptr_t) 1668, // sCodergb2y_RGB2Y1
  (SEQ_Head_Ptr_t) 1960, // sCodelut4k_LUT4K0
  (SEQ_Head_Ptr_t) 2252, // sCodelut4k_hist_LUT4K1
  (SEQ_Head_Ptr_t) 2584, // sCodergb_compress_y_uv_ccm_COMPRESS0
  (SEQ_Head_Ptr_t) 2916, // sCodergb_compress_y_uv_ccm_COMPRESS1
  (SEQ_Head_Ptr_t) 3224, // sCodedenoise_median_DENOISEY2_0
  (SEQ_Head_Ptr_t) 3532, // sCodedenoise_median_DENOISEY2_1
  (SEQ_Head_Ptr_t) 3848, // sCodedenoise4x4_uv_DENOISE_UV
  (SEQ_Head_Ptr_t) 4148, // sCodehdr_blend_HDR
  (SEQ_Head_Ptr_t) 4440, // sCodelut4k_tee_INLUT
  (SEQ_Head_Ptr_t) 0    // ** END ***
};

// *** Graph parsing metadata ***

static Object_t sObjectList[69] = {
  { &gpGraph, 124,  0 },
  { &Histogram, 224,  124 },
  { &HistogramPara, 56, 348 },
  { &sCodehistogram_Histogram,  28, 404 },
  { &Debayer0,  224,  432 },
  { &Debayer0Para,  72, 656 },
  { &sCodedebayer_even_Debayer0,  28, 728 },
  { &Debayer1,  224,  756 },
  { &Debayer1Para,  72, 980 },
  { &sCodedebayer_odd_Debayer1, 28, 1052 },
  { &RGB2Y0,  224,  1080 },
  { &RGB2Y0Para,  56, 1304 },
  { &sCodergb2y_RGB2Y0, 28, 1360 },
  { &RGB2Y1,  224,  1388 },
  { &RGB2Y1Para,  56, 1612 },
  { &sCodergb2y_RGB2Y1, 28, 1668 },
  { &LUT4K0,  224,  1696 },
  { &LUT4K0Para,  40, 1920 },
  { &sCodelut4k_LUT4K0, 28, 1960 },
  { &LUT4K1,  224,  1988 },
  { &LUT4K1Para,  40, 2212 },
  { &sCodelut4k_hist_LUT4K1,  28, 2252 },
  { &COMPRESS0, 224,  2280 },
  { &COMPRESS0Para, 80, 2504 },
  { &sCodergb_compress_y_uv_ccm_COMPRESS0,  28, 2584 },
  { &COMPRESS1, 224,  2612 },
  { &COMPRESS1Para, 80, 2836 },
  { &sCodergb_compress_y_uv_ccm_COMPRESS1,  28, 2916 },
  { &DENOISEY2_0, 224,  2944 },
  { &DENOISEY2_0Para, 56, 3168 },
  { &sCodedenoise_median_DENOISEY2_0, 28, 3224 },
  { &DENOISEY2_1, 224,  3252 },
  { &DENOISEY2_1Para, 56, 3476 },
  { &sCodedenoise_median_DENOISEY2_1, 28, 3532 },
  { &DENOISE_UV,  224,  3560 },
  { &DENOISE_UVPara,  64, 3784 },
  { &sCodedenoise4x4_uv_DENOISE_UV, 28, 3848 },
  { &HDR, 224,  3876 },
  { &HDRPara, 48, 4100 },
  { &sCodehdr_blend_HDR,  28, 4148 },
  { &INLUT, 224,  4176 },
  { &INLUTPara, 40, 4400 },
  { &sCodelut4k_tee_INLUT,  28, 4440 },
  { &OV_QUAD, 36, 4468 },
  { &FastDMA, 208,  4504 },
  { &OV_QUAD_Buffer_0,  216,  4712 },
  { &HDR_Buffer_0,  216,  4928 },
  { &Debayer0_Buffer_0, 216,  5144 },
  { &Debayer0_Buffer_1, 216,  5360 },
  { &Debayer0_Buffer_2, 216,  5576 },
  { &Debayer1_Buffer_0, 216,  5792 },
  { &Debayer1_Buffer_1, 216,  6008 },
  { &Debayer1_Buffer_2, 216,  6224 },
  { &RGB2Y0_Buffer_0, 216,  6440 },
  { &RGB2Y1_Buffer_0, 216,  6656 },
  { &LUT4K0_Buffer_0, 216,  6872 },
  { &LUT4K1_Buffer_0, 216,  7088 },
  { &COMPRESS0_Buffer_0,  216,  7304 },
  { &COMPRESS0_Buffer_1,  216,  7520 },
  { &COMPRESS1_Buffer_0,  216,  7736 },
  { &COMPRESS1_Buffer_1,  216,  7952 },
  { &DENOISE_UV_Buffer_0, 216,  8168 },
  { &DENOISEY2_0_Buffer_0,  216,  8384 },
  { &DENOISEY2_1_Buffer_0,  216,  8600 },
  { &INLUT_Buffer_0,  216,  8816 },
  { &FastDMA_Buffer_0,  216,  9032 },
  { &FastDMA_Buffer_1,  216,  9248 },
  { &FastDMA_Buffer_2,  216,  9464 },
  { &FastDMA_Buffer_3,  216,  9680 },
};

static uint32_t sPointerList[] = {
  0, /* gpGraph: pointer to FastDMA */
  4, /* gpGraph: pointer to OV_QUAD */
  8, /* gpGraph: pointer to Histogram */
  12, /* gpGraph: pointer to Debayer0 */
  16, /* gpGraph: pointer to Debayer1 */
  20, /* gpGraph: pointer to RGB2Y0 */
  24, /* gpGraph: pointer to RGB2Y1 */
  28, /* gpGraph: pointer to LUT4K0 */
  32, /* gpGraph: pointer to LUT4K1 */
  36, /* gpGraph: pointer to COMPRESS0 */
  40, /* gpGraph: pointer to COMPRESS1 */
  44, /* gpGraph: pointer to DENOISEY2_0 */
  48, /* gpGraph: pointer to DENOISEY2_1 */
  52, /* gpGraph: pointer to DENOISE_UV */
  56, /* gpGraph: pointer to HDR */
  60, /* gpGraph: pointer to INLUT */
  64, /* gpGraph: pointer to sCodehistogram_Histogram */
  68, /* gpGraph: pointer to sCodedebayer_even_Debayer0 */
  72, /* gpGraph: pointer to sCodedebayer_odd_Debayer1 */
  76, /* gpGraph: pointer to sCodergb2y_RGB2Y0 */
  80, /* gpGraph: pointer to sCodergb2y_RGB2Y1 */
  84, /* gpGraph: pointer to sCodelut4k_LUT4K0 */
  88, /* gpGraph: pointer to sCodelut4k_hist_LUT4K1 */
  92, /* gpGraph: pointer to sCodergb_compress_y_uv_ccm_COMPRESS0 */
  96, /* gpGraph: pointer to sCodergb_compress_y_uv_ccm_COMPRESS1 */
  100, /* gpGraph: pointer to sCodedenoise_median_DENOISEY2_0 */
  104, /* gpGraph: pointer to sCodedenoise_median_DENOISEY2_1 */
  108, /* gpGraph: pointer to sCodedenoise4x4_uv_DENOISE_UV */
  112, /* gpGraph: pointer to sCodehdr_blend_HDR */
  116, /* gpGraph: pointer to sCodelut4k_tee_INLUT */
  196, /* Histogram: pointer to INput buffer */
  212, /* Histogram: pointer to INput buffer */
  228, /* Histogram: pointer to INput buffer */
  340, /* Histogram: pointer to config parameters */
  428, /* sCodehistogram_Histogram: pointer to Histogram.mHead */
  456, /* Debayer0: pointer to OUTput buffer */
  468, /* Debayer0: pointer to OUTput buffer */
  480, /* Debayer0: pointer to OUTput buffer */
  504, /* Debayer0: pointer to INput buffer */
  520, /* Debayer0: pointer to INput buffer */
  536, /* Debayer0: pointer to INput buffer */
  648, /* Debayer0: pointer to config parameters */
  752, /* sCodedebayer_even_Debayer0: pointer to Debayer0.mHead */
  780, /* Debayer1: pointer to OUTput buffer */
  792, /* Debayer1: pointer to OUTput buffer */
  804, /* Debayer1: pointer to OUTput buffer */
  828, /* Debayer1: pointer to INput buffer */
  844, /* Debayer1: pointer to INput buffer */
  860, /* Debayer1: pointer to INput buffer */
  972, /* Debayer1: pointer to config parameters */
  1076, /* sCodedebayer_odd_Debayer1: pointer to Debayer1.mHead */
  1104, /* RGB2Y0: pointer to OUTput buffer */
  1152, /* RGB2Y0: pointer to INput buffer */
  1168, /* RGB2Y0: pointer to INput buffer */
  1184, /* RGB2Y0: pointer to INput buffer */
  1296, /* RGB2Y0: pointer to config parameters */
  1384, /* sCodergb2y_RGB2Y0: pointer to RGB2Y0.mHead */
  1412, /* RGB2Y1: pointer to OUTput buffer */
  1460, /* RGB2Y1: pointer to INput buffer */
  1476, /* RGB2Y1: pointer to INput buffer */
  1492, /* RGB2Y1: pointer to INput buffer */
  1604, /* RGB2Y1: pointer to config parameters */
  1692, /* sCodergb2y_RGB2Y1: pointer to RGB2Y1.mHead */
  1720, /* LUT4K0: pointer to OUTput buffer */
  1768, /* LUT4K0: pointer to INput buffer */
  1912, /* LUT4K0: pointer to config parameters */
  1984, /* sCodelut4k_LUT4K0: pointer to LUT4K0.mHead */
  2012, /* LUT4K1: pointer to OUTput buffer */
  2060, /* LUT4K1: pointer to INput buffer */
  2204, /* LUT4K1: pointer to config parameters */
  2276, /* sCodelut4k_hist_LUT4K1: pointer to LUT4K1.mHead */
  2304, /* COMPRESS0: pointer to OUTput buffer */
  2316, /* COMPRESS0: pointer to OUTput buffer */
  2352, /* COMPRESS0: pointer to INput buffer */
  2368, /* COMPRESS0: pointer to INput buffer */
  2384, /* COMPRESS0: pointer to INput buffer */
  2400, /* COMPRESS0: pointer to INput buffer */
  2416, /* COMPRESS0: pointer to INput buffer */
  2496, /* COMPRESS0: pointer to config parameters */
  2608, /* sCodergb_compress_y_uv_ccm_COMPRESS0: pointer to COMPRESS0.mHead */
  2636, /* COMPRESS1: pointer to OUTput buffer */
  2648, /* COMPRESS1: pointer to OUTput buffer */
  2684, /* COMPRESS1: pointer to INput buffer */
  2700, /* COMPRESS1: pointer to INput buffer */
  2716, /* COMPRESS1: pointer to INput buffer */
  2732, /* COMPRESS1: pointer to INput buffer */
  2748, /* COMPRESS1: pointer to INput buffer */
  2828, /* COMPRESS1: pointer to config parameters */
  2940, /* sCodergb_compress_y_uv_ccm_COMPRESS1: pointer to COMPRESS1.mHead */
  2968, /* DENOISEY2_0: pointer to OUTput buffer */
  3016, /* DENOISEY2_0: pointer to INput buffer */
  3032, /* DENOISEY2_0: pointer to INput buffer */
  3048, /* DENOISEY2_0: pointer to INput buffer */
  3160, /* DENOISEY2_0: pointer to config parameters */
  3248, /* sCodedenoise_median_DENOISEY2_0: pointer to DENOISEY2_0.mHead */
  3276, /* DENOISEY2_1: pointer to OUTput buffer */
  3324, /* DENOISEY2_1: pointer to INput buffer */
  3340, /* DENOISEY2_1: pointer to INput buffer */
  3356, /* DENOISEY2_1: pointer to INput buffer */
  3468, /* DENOISEY2_1: pointer to config parameters */
  3556, /* sCodedenoise_median_DENOISEY2_1: pointer to DENOISEY2_1.mHead */
  3584, /* DENOISE_UV: pointer to OUTput buffer */
  3632, /* DENOISE_UV: pointer to INput buffer */
  3648, /* DENOISE_UV: pointer to INput buffer */
  3664, /* DENOISE_UV: pointer to INput buffer */
  3680, /* DENOISE_UV: pointer to INput buffer */
  3776, /* DENOISE_UV: pointer to config parameters */
  3872, /* sCodedenoise4x4_uv_DENOISE_UV: pointer to DENOISE_UV.mHead */
  3900, /* HDR: pointer to OUTput buffer */
  3948, /* HDR: pointer to INput buffer */
  3964, /* HDR: pointer to INput buffer */
  4092, /* HDR: pointer to config parameters */
  4172, /* sCodehdr_blend_HDR: pointer to HDR.mHead */
  4200, /* INLUT: pointer to OUTput buffer */
  4248, /* INLUT: pointer to INput buffer */
  4392, /* INLUT: pointer to config parameters */
  4464, /* sCodelut4k_tee_INLUT: pointer to INLUT.mHead */
  4484, /* OV_QUAD: pointer to OUTput buffer */
  4724, /* OV_QUAD_Buffer_0: pointer to producer */
  4736, /* OV_QUAD_Buffer_0: pointer to consumer */
  4760, /* OV_QUAD_Buffer_0: pointer to consumer */
  4784, /* OV_QUAD_Buffer_0: pointer to consumer */
  4808, /* OV_QUAD_Buffer_0: pointer to consumer */
  4940, /* HDR_Buffer_0: pointer to producer */
  4952, /* HDR_Buffer_0: pointer to consumer */
  4976, /* HDR_Buffer_0: pointer to consumer */
  5000, /* HDR_Buffer_0: pointer to consumer */
  5024, /* HDR_Buffer_0: pointer to consumer */
  5048, /* HDR_Buffer_0: pointer to consumer */
  5072, /* HDR_Buffer_0: pointer to consumer */
  5156, /* Debayer0_Buffer_0: pointer to producer */
  5168, /* Debayer0_Buffer_0: pointer to consumer */
  5192, /* Debayer0_Buffer_0: pointer to consumer */
  5372, /* Debayer0_Buffer_1: pointer to producer */
  5384, /* Debayer0_Buffer_1: pointer to consumer */
  5408, /* Debayer0_Buffer_1: pointer to consumer */
  5588, /* Debayer0_Buffer_2: pointer to producer */
  5600, /* Debayer0_Buffer_2: pointer to consumer */
  5624, /* Debayer0_Buffer_2: pointer to consumer */
  5804, /* Debayer1_Buffer_0: pointer to producer */
  5816, /* Debayer1_Buffer_0: pointer to consumer */
  5840, /* Debayer1_Buffer_0: pointer to consumer */
  6020, /* Debayer1_Buffer_1: pointer to producer */
  6032, /* Debayer1_Buffer_1: pointer to consumer */
  6056, /* Debayer1_Buffer_1: pointer to consumer */
  6236, /* Debayer1_Buffer_2: pointer to producer */
  6248, /* Debayer1_Buffer_2: pointer to consumer */
  6272, /* Debayer1_Buffer_2: pointer to consumer */
  6452, /* RGB2Y0_Buffer_0: pointer to producer */
  6464, /* RGB2Y0_Buffer_0: pointer to consumer */
  6488, /* RGB2Y0_Buffer_0: pointer to consumer */
  6668, /* RGB2Y1_Buffer_0: pointer to producer */
  6680, /* RGB2Y1_Buffer_0: pointer to consumer */
  6704, /* RGB2Y1_Buffer_0: pointer to consumer */
  6884, /* LUT4K0_Buffer_0: pointer to producer */
  6896, /* LUT4K0_Buffer_0: pointer to consumer */
  7100, /* LUT4K1_Buffer_0: pointer to producer */
  7112, /* LUT4K1_Buffer_0: pointer to consumer */
  7316, /* COMPRESS0_Buffer_0: pointer to producer */
  7328, /* COMPRESS0_Buffer_0: pointer to consumer */
  7352, /* COMPRESS0_Buffer_0: pointer to consumer */
  7532, /* COMPRESS0_Buffer_1: pointer to producer */
  7544, /* COMPRESS0_Buffer_1: pointer to consumer */
  7568, /* COMPRESS0_Buffer_1: pointer to consumer */
  7592, /* COMPRESS0_Buffer_1: pointer to consumer */
  7748, /* COMPRESS1_Buffer_0: pointer to producer */
  7760, /* COMPRESS1_Buffer_0: pointer to consumer */
  7784, /* COMPRESS1_Buffer_0: pointer to consumer */
  7964, /* COMPRESS1_Buffer_1: pointer to producer */
  7976, /* COMPRESS1_Buffer_1: pointer to consumer */
  8000, /* COMPRESS1_Buffer_1: pointer to consumer */
  8024, /* COMPRESS1_Buffer_1: pointer to consumer */
  8180, /* DENOISE_UV_Buffer_0: pointer to producer */
  8192, /* DENOISE_UV_Buffer_0: pointer to consumer */
  8396, /* DENOISEY2_0_Buffer_0: pointer to producer */
  8408, /* DENOISEY2_0_Buffer_0: pointer to consumer */
  8612, /* DENOISEY2_1_Buffer_0: pointer to producer */
  8624, /* DENOISEY2_1_Buffer_0: pointer to consumer */
  8828, /* INLUT_Buffer_0: pointer to producer */
  8840, /* INLUT_Buffer_0: pointer to consumer */
  8864, /* INLUT_Buffer_0: pointer to consumer */
  8888, /* INLUT_Buffer_0: pointer to consumer */
  9044, /* FastDMA_Buffer_0: pointer to producer */
  9260, /* FastDMA_Buffer_1: pointer to producer */
  9476, /* FastDMA_Buffer_2: pointer to producer */
  9692, /* FastDMA_Buffer_3: pointer to producer */
};

static uint32_t sBufferList[] = {
  4712, /* OV_QUAD_Buffer_0 */
  4928, /* HDR_Buffer_0 */
  5144, /* Debayer0_Buffer_0 */
  5360, /* Debayer0_Buffer_1 */
  5576, /* Debayer0_Buffer_2 */
  5792, /* Debayer1_Buffer_0 */
  6008, /* Debayer1_Buffer_1 */
  6224, /* Debayer1_Buffer_2 */
  6440, /* RGB2Y0_Buffer_0 */
  6656, /* RGB2Y1_Buffer_0 */
  6872, /* LUT4K0_Buffer_0 */
  7088, /* LUT4K1_Buffer_0 */
  7304, /* COMPRESS0_Buffer_0 */
  7520, /* COMPRESS0_Buffer_1 */
  7736, /* COMPRESS1_Buffer_0 */
  7952, /* COMPRESS1_Buffer_1 */
  8168, /* DENOISE_UV_Buffer_0 */
  8384, /* DENOISEY2_0_Buffer_0 */
  8600, /* DENOISEY2_1_Buffer_0 */
  8816, /* INLUT_Buffer_0 */
  9032, /* FastDMA_Buffer_0 */
  9248, /* FastDMA_Buffer_1 */
  9464, /* FastDMA_Buffer_2 */
  9680, /* FastDMA_Buffer_3 */
};

static const char* scppFdmaChannelNames[] = {
  "CAM_OUTPUT",       // #0
  "ISP_OUTPUT_UV0",     // #1
  "ISP_OUTPUT0",      // #2
  "ISP_OUTPUT1",      // #3
};

static const char* scppBufferNames[] = {
  "OV_QUAD_Buffer_0",
  "HDR_Buffer_0",
  "Debayer0_Buffer_0",
  "Debayer0_Buffer_1",
  "Debayer0_Buffer_2",
  "Debayer1_Buffer_0",
  "Debayer1_Buffer_1",
  "Debayer1_Buffer_2",
  "RGB2Y0_Buffer_0",
  "RGB2Y1_Buffer_0",
  "LUT4K0_Buffer_0",
  "LUT4K1_Buffer_0",
  "COMPRESS0_Buffer_0",
  "COMPRESS0_Buffer_1",
  "COMPRESS1_Buffer_0",
  "COMPRESS1_Buffer_1",
  "DENOISE_UV_Buffer_0",
  "DENOISEY2_0_Buffer_0",
  "DENOISEY2_1_Buffer_0",
  "INLUT_Buffer_0",
  "FastDMA_Buffer_0",
  "FastDMA_Buffer_1",
  "FastDMA_Buffer_2",
  "FastDMA_Buffer_3",
};

GraphMetadata_t gGraphMetadata = {
  8,    // *** max number of buffer consumers
  9896,   // *** graph size [bytes]
  (const Object_t*) &sObjectList, // *** object list pointer
  69,   // *** object count
  (const uint32_t*) &sPointerList,  // *** pointer list pointer
  186,    // *** pointer count
  (const uint32_t*) &sBufferList, // *** buffer list pointer
  24,   // *** buffer count
  {
    0
  },
  { // MIPI-CSI metadata
    { // node OV_QUAD
      CSI_OV9716, CAMS,
      {
        0, 0, 0, 0,
      },
      {
        CAM_PIXELS1 * 2, 0 * 0, 0 * 0, 0 * 0,
      }
    },
    { // node not used
      CSI_NONE, 0, {0}, {0}
    },
  },
  { // VIU Lite metadata
    { // node not used
      VIU_NONE, 0, 0
    },
    { // node not used
      VIU_NONE, 0, 0
    },
  },
  scppFdmaChannelNames,
  scppBufferNames
};

#ifdef  __cplusplus
}
#endif

/*EOF*/
