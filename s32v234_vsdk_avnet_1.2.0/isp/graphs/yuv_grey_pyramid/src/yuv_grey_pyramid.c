/*******************************************************************************
 *
 * NXP Confidential Proprietary
 *
 * Copyright 2017-2018 NXP
 * All Rights Reserved
 *
 *****************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#ifdef  __cplusplus
extern "C" {
#endif

#include "yuv_grey_pyramid_c.h"
#include "iofiles.h"
#include "kmem.c"
#include "kmem_sym.h"

// *** forwards needed for cross-linking ***

static SEQ_FdmaCfg_t FastDMA;
static SEQ_MipiCfg_t CAM0;
static SEQ_IpuCfg_t ConvertYUV;
static SEQ_IpuCfg_t ChGain_Exposure;
static SEQ_IpuCfg_t Debayer_G;
static SEQ_IpuCfg_t Debayer_BR;
static SEQ_IpuCfg_t RGB_2_YUV;
static SEQ_IpuCfg_t Y2_DOWNSCALE;
static SEQ_IpuCfg_t Y4_DOWNSCALE;
static SEQ_IpuCfg_t Y8_DOWNSCALE;
static SEQ_IpuCfg_t Y16_DOWNSCALE;
static SEQ_IpuCfg_t Y32_DOWNSCALE;
static SEQ_Load_t sCodegamma_yuv444touyvy_ConvertYUV;
static SEQ_Load_t sCodeexposure_tonehdr_ChGain_Exposure;
static SEQ_Load_t sCodedebayer_g_Debayer_G;
static SEQ_Load_t sCodedebayer_br_advanced_Debayer_BR;
static SEQ_Load_t sCodergb2yuv_uint_RGB_2_YUV;
static SEQ_Load_t sCodescale2by1_11_Y2_DOWNSCALE;
static SEQ_Load_t sCodescale2by1_11_Y4_DOWNSCALE;
static SEQ_Load_t sCodescale2by1_11_Y8_DOWNSCALE;
static SEQ_Load_t sCodescale2by1_11_Y16_DOWNSCALE;
static SEQ_Load_t sCodescale2by1_11_ipuv_Y32_DOWNSCALE;

/*** Buffers ****************************************************************/

SEQ_Buf_t ConvertYUV_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  4,				// *** number of lines fitting into output buffer (wrap) ***
  2560 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(2560),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 92 /*ConvertYUV*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 3232 /* FastDMA.mHead */,	// *** offset to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      2560 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // ConvertYUV_Buffer_0

SEQ_Buf_t FastDMA_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 1,		// *** pointer to output buffer ***
  720,				// *** number of lines fitting into output buffer (wrap) ***
  2560 * 1,				// *** number of bytes per line ***
  2560,				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 3232 /*FastDMA*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      -3,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      2560 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // FastDMA_Buffer_0

SEQ_Buf_t ChGain_Exposure_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  8,				// *** number of lines fitting into output buffer (wrap) ***
  1288 * 2,				// *** number of bytes per line ***
  BUF_ALIGN(2576),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 400 /*ChGain_Exposure*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 700 /* Debayer_G.mHead */,	// *** offset to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      1288 * 2,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 700 /* Debayer_G.mHead */,	// *** offset to consumers (node) configuration header ***
      1,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      2,				// *** number of lines required for processing ***
      1288 * 2,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 700 /* Debayer_G.mHead */,	// *** offset to consumers (node) configuration header ***
      2,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      3,				// *** number of lines required for processing ***
      1288 * 2,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // ChGain_Exposure_Buffer_0

SEQ_Buf_t Debayer_G_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  8,				// *** number of lines fitting into output buffer (wrap) ***
  1286 * 2,				// *** number of bytes per line ***
  BUF_ALIGN(2576),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 700 /*Debayer_G*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 1016 /* Debayer_BR.mHead */,	// *** offset to consumers (node) configuration header ***
      3,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      1286 * 2,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 1016 /* Debayer_BR.mHead */,	// *** offset to consumers (node) configuration header ***
      4,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      2,				// *** number of lines required for processing ***
      1286 * 2,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 1016 /* Debayer_BR.mHead */,	// *** offset to consumers (node) configuration header ***
      5,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      3,				// *** number of lines required for processing ***
      1286 * 2,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // Debayer_G_Buffer_0

SEQ_Buf_t Debayer_G_Buffer_1 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  8,				// *** number of lines fitting into output buffer (wrap) ***
  643 * 2,				// *** number of bytes per line ***
  BUF_ALIGN(1286),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 700 /*Debayer_G*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 1016 /* Debayer_BR.mHead */,	// *** offset to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      643 * 2,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 1016 /* Debayer_BR.mHead */,	// *** offset to consumers (node) configuration header ***
      1,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      2,				// *** number of lines required for processing ***
      643 * 2,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 1016 /* Debayer_BR.mHead */,	// *** offset to consumers (node) configuration header ***
      2,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      3,				// *** number of lines required for processing ***
      643 * 2,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // Debayer_G_Buffer_1

SEQ_Buf_t Debayer_BR_Buffer_2 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  8,				// *** number of lines fitting into output buffer (wrap) ***
  1288 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(1288),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 1016 /*Debayer_BR*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 1372 /* RGB_2_YUV.mHead */,	// *** offset to consumers (node) configuration header ***
      1,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      1288 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // Debayer_BR_Buffer_2

SEQ_Buf_t Debayer_BR_Buffer_1 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  8,				// *** number of lines fitting into output buffer (wrap) ***
  1288 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(1288),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 1016 /*Debayer_BR*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 1372 /* RGB_2_YUV.mHead */,	// *** offset to consumers (node) configuration header ***
      2,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      1288 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // Debayer_BR_Buffer_1

SEQ_Buf_t Debayer_BR_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  8,				// *** number of lines fitting into output buffer (wrap) ***
  1288 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(1288),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 1016 /*Debayer_BR*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 1372 /* RGB_2_YUV.mHead */,	// *** offset to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      1288 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // Debayer_BR_Buffer_0

SEQ_Buf_t RGB_2_YUV_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  8,				// *** number of lines fitting into output buffer (wrap) ***
  1280 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(1280),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 1372 /*RGB_2_YUV*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 92 /* ConvertYUV.mHead */,	// *** offset to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      1280 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 1732 /* Y2_DOWNSCALE.mHead */,	// *** offset to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      1280 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      2,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 1732 /* Y2_DOWNSCALE.mHead */,	// *** offset to consumers (node) configuration header ***
      1,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      2,				// *** number of lines required for processing ***
      1280 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      2,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // RGB_2_YUV_Buffer_0

SEQ_Buf_t RGB_2_YUV_Buffer_1 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  8,				// *** number of lines fitting into output buffer (wrap) ***
  1280 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(1280),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 1372 /*RGB_2_YUV*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 92 /* ConvertYUV.mHead */,	// *** offset to consumers (node) configuration header ***
      1,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      1280 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // RGB_2_YUV_Buffer_1

SEQ_Buf_t RGB_2_YUV_Buffer_2 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  8,				// *** number of lines fitting into output buffer (wrap) ***
  1280 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(1280),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 1372 /*RGB_2_YUV*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 92 /* ConvertYUV.mHead */,	// *** offset to consumers (node) configuration header ***
      2,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      1280 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // RGB_2_YUV_Buffer_2

SEQ_Buf_t CAM0_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  16,				// *** number of lines fitting into output buffer (wrap) ***
  1288 * 2,				// *** number of bytes per line ***
  BUF_ALIGN(2576),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 1696 /*CAM0*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 400 /* ChGain_Exposure.mHead */,	// *** offset to consumers (node) configuration header ***
      1,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      1288 * 2,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 400 /* ChGain_Exposure.mHead */,	// *** offset to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      1288 * 2,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // CAM0_Buffer_0

SEQ_Buf_t Y2_DOWNSCALE_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  16,				// *** number of lines fitting into output buffer (wrap) ***
  640 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(640),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 1732 /*Y2_DOWNSCALE*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 3232 /* FastDMA.mHead */,	// *** offset to consumers (node) configuration header ***
      1,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      640 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 2032 /* Y4_DOWNSCALE.mHead */,	// *** offset to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      640 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      2,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 2032 /* Y4_DOWNSCALE.mHead */,	// *** offset to consumers (node) configuration header ***
      1,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      2,				// *** number of lines required for processing ***
      640 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      2,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // Y2_DOWNSCALE_Buffer_0

SEQ_Buf_t FastDMA_Buffer_1 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 1,		// *** pointer to output buffer ***
  360,				// *** number of lines fitting into output buffer (wrap) ***
  640 * 1,				// *** number of bytes per line ***
  640,				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 3232 /*FastDMA*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      -3,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      640 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // FastDMA_Buffer_1

SEQ_Buf_t Y4_DOWNSCALE_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  16,				// *** number of lines fitting into output buffer (wrap) ***
  320 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(320),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 2032 /*Y4_DOWNSCALE*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 3232 /* FastDMA.mHead */,	// *** offset to consumers (node) configuration header ***
      2,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      320 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 2332 /* Y8_DOWNSCALE.mHead */,	// *** offset to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      320 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      2,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 2332 /* Y8_DOWNSCALE.mHead */,	// *** offset to consumers (node) configuration header ***
      1,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      2,				// *** number of lines required for processing ***
      320 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      2,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // Y4_DOWNSCALE_Buffer_0

SEQ_Buf_t FastDMA_Buffer_2 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 1,		// *** pointer to output buffer ***
  180,				// *** number of lines fitting into output buffer (wrap) ***
  320 * 1,				// *** number of bytes per line ***
  320,				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 3232 /*FastDMA*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      -3,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      320 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // FastDMA_Buffer_2

SEQ_Buf_t Y8_DOWNSCALE_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  16,				// *** number of lines fitting into output buffer (wrap) ***
  160 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(160),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 2332 /*Y8_DOWNSCALE*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 3232 /* FastDMA.mHead */,	// *** offset to consumers (node) configuration header ***
      3,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      160 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 2632 /* Y16_DOWNSCALE.mHead */,	// *** offset to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      160 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      2,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 2632 /* Y16_DOWNSCALE.mHead */,	// *** offset to consumers (node) configuration header ***
      1,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      2,				// *** number of lines required for processing ***
      160 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      2,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // Y8_DOWNSCALE_Buffer_0

SEQ_Buf_t FastDMA_Buffer_3 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 1,		// *** pointer to output buffer ***
  90,				// *** number of lines fitting into output buffer (wrap) ***
  160 * 1,				// *** number of bytes per line ***
  160,				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 3232 /*FastDMA*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      -3,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      160 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // FastDMA_Buffer_3

SEQ_Buf_t Y16_DOWNSCALE_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  16,				// *** number of lines fitting into output buffer (wrap) ***
  80 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(80),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 2632 /*Y16_DOWNSCALE*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 3232 /* FastDMA.mHead */,	// *** offset to consumers (node) configuration header ***
      4,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      80 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 2932 /* Y32_DOWNSCALE.mHead */,	// *** offset to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      80 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      2,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 2932 /* Y32_DOWNSCALE.mHead */,	// *** offset to consumers (node) configuration header ***
      1,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      2,				// *** number of lines required for processing ***
      80 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      2,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // Y16_DOWNSCALE_Buffer_0

SEQ_Buf_t FastDMA_Buffer_4 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 1,		// *** pointer to output buffer ***
  45,				// *** number of lines fitting into output buffer (wrap) ***
  80 * 1,				// *** number of bytes per line ***
  80,				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 3232 /*FastDMA*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      -3,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      80 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // FastDMA_Buffer_4

SEQ_Buf_t Y32_DOWNSCALE_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  16,				// *** number of lines fitting into output buffer (wrap) ***
  40 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(40),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 2932 /*Y32_DOWNSCALE*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 3232 /* FastDMA.mHead */,	// *** offset to consumers (node) configuration header ***
      5,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      1,				// *** number of lines required for processing ***
      40 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // Y32_DOWNSCALE_Buffer_0

SEQ_Buf_t FastDMA_Buffer_5 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 1,		// *** pointer to output buffer ***
  22,				// *** number of lines fitting into output buffer (wrap) ***
  40 * 1,				// *** number of bytes per line ***
  40,				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 3232 /*FastDMA*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
    // *** consumer/read #0 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      -3,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      40 * 1,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      1,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #1 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #2 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #3 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #4 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #5 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #6 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },
    // *** consumer/read #7 ***
    {
      (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
      0,				// *** the devices channel used ***
      0,				// *** number of lines to skip at frame begin ***
      0,				// *** number of lines required for processing ***
      0,				// *** number of bytes per line to read ***
      0,				// *** current relative index into source buffer (read) ***
      0,				// *** increment of current relative index ***
      0,				// *** total lines read from buffer ***
      0,				// *** buffer is ready to be read by this consumer ***
      0,				// *** maximum difference between read and write ***
      0,				// *** pad aligment ***
    },

  }
}; // FastDMA_Buffer_5


static SEQ_CfgPara_t ConvertYUVPara [] = {
  { D_IPUS_SYN_HOST_XCFG_OFF, (2560 << 16) | 0x1  },
  { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000700  },
  { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100  },
  { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x10000000  },
};

static SEQ_CfgPara_t ChGain_ExposurePara [] = {
  { D_IPUS_SYN_HOST_XCFG_OFF, (1288 << 16) | 0x1  },
  { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000300  },
  { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100  },
  { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00058000  },
  { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x00058000  },
  { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x00000000  },
};

static SEQ_CfgPara_t Debayer_GPara [] = {
  { D_IPUS_SYN_HOST_XCFG_OFF, (1286 << 16) | 0x1 },
  { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000700  },
  { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000300  },
  { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00058000  },
  { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x00058000  },
  { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x00058000  },
  { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x00000000  },
  { D_IPUS_SYN_S_CH1_CFG_OUT_OFF, 0x00010000  },
};

static SEQ_CfgPara_t Debayer_BRPara [] = {
  { D_IPUS_SYN_HOST_XCFG_OFF, (1284 << 16) | 0x1 },
  { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000700  },
  { D_IPUS_SYN_HOST_INBCFG_OFF, 0x00000700  },
  { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000700  },
  { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x00098000  },
  { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x00098000  },
  { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x00098000  },
  { D_IPUS_SYN_S_CH0_CFG_INB_OFF, 0x00058000  },
  { D_IPUS_SYN_S_CH1_CFG_INB_OFF, 0x00058000  },
  { D_IPUS_SYN_S_CH2_CFG_INB_OFF, 0x00058000  },
  { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x10000000  },
  { D_IPUS_SYN_S_CH1_CFG_OUT_OFF, 0x10000000  },
  { D_IPUS_SYN_S_CH2_CFG_OUT_OFF, 0x10000000  },
};

static SEQ_CfgPara_t RGB_2_YUVPara [] = {
  { D_IPUS_SYN_HOST_XCFG_OFF, (1284<< 16) | 0x0  },
  { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000700  },
  { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000700  },
  { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x10058000  },
  { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x10000000  },
  { D_IPUS_SYN_S_CH1_CFG_OUT_OFF, 0x10000000  },
  { D_IPUS_SYN_S_CH2_CFG_OUT_OFF, 0x10000000  },
};

static SEQ_CfgPara_t Y2_DOWNSCALEPara [] = {
  { D_IPUS_SYN_HOST_XCFG_OFF, (1280 << 16) | 0x1  },
  { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000300  },
  { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100  },
  { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x10000000  },
};

static SEQ_CfgPara_t Y4_DOWNSCALEPara [] = {
  { D_IPUS_SYN_HOST_XCFG_OFF, (320 << 16) | 0x1  },
  { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000300  },
  { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100  },
  { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x10000000  },
};

static SEQ_CfgPara_t Y8_DOWNSCALEPara [] = {
  { D_IPUS_SYN_HOST_XCFG_OFF, (160 << 16) | 0x1  },
  { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000300  },
  { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100  },
  { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x10000000  },
};

static SEQ_CfgPara_t Y16_DOWNSCALEPara [] = {
  { D_IPUS_SYN_HOST_XCFG_OFF, (80 << 16) | 0x1  },
  { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000300  },
  { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100  },
  { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x10000000  },
};

static SEQ_CfgPara_t Y32_DOWNSCALEPara [] = {
  { D_IPUS_SYN_HOST_XCFG_OFF, (40 << 16) | 0x1  },
  { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000300  },
  { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000100  },
  { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x10054000  },
  { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x10000000  },
};

static SEQ_MipiCfg_t CAM0 = {
  {
    SEQ_NODE_OTHER,			// *** type of engine ***
    ISP_IO_DEV_BASE+ISP_OFFS_FILEIO,	// *** address of hardware device ***
    SEQ_OTHRIX_MIPICSI0,			// *** index of requested engine ***
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },


  // *** output buffers ***
  {
    (SEQ_Buf_Ptr_t) 5816,			// *** offset to output buffer ***

    (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***

    (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***

    (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***

  },
  729,			// *** number of input image lines ***
  0				// *** pad alignment ***
}; // CAM0

static SEQ_IpuCfg_t ConvertYUV = {
  {
    SEQ_NODE_SCALAR,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 3*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    3,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  0x00000000,			// *** code entry address *** (TODO)

  0,					// *** current processing steps ***
  720,					// *** number of processing steps per frame ***

  {
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 3440 /* ConvertYUV_Buffer_0 */,			// *** pointer to output buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        2560,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
  },	// output buffer structs

  {
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 5168 /* RGB_2_YUV_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1280,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 5384 /* RGB_2_YUV_Buffer_1 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE1_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1280,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 5600 /* RGB_2_YUV_Buffer_2 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE2_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1280,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
  },	// input buffer structs
  (SEQ_CfgPara_CPtr_t) 316 /*ConvertYUVPara */,	// *** pointer to configuration parameters ***
  7,					// *** number of configuration parameters following ***
  0,				// *** pad alignment ***
};

static SEQ_IpuCfg_t ChGain_Exposure = {
  {
    SEQ_NODE_SCALAR,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 1*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    1,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  0x00000000,			// *** code entry address *** (TODO)

  0,					// *** current processing steps ***
  728,					// *** number of processing steps per frame ***

  {
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 3872 /* ChGain_Exposure_Buffer_0 */,			// *** pointer to output buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1288,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
  },	// output buffer structs

  {
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 5816 /* CAM0_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1288,					// *** number of pixels to transfer ***
      },
      1,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 5816 /* CAM0_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE1_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1288,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
  },	// input buffer structs
  (SEQ_CfgPara_CPtr_t) 624 /*ChGain_ExposurePara */,	// *** pointer to configuration parameters ***
  6,					// *** number of configuration parameters following ***
  0,				// *** pad alignment ***
};

static SEQ_IpuCfg_t Debayer_G = {
  {
    SEQ_NODE_SCALAR,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 2*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    2,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  0x00000000,			// *** code entry address *** (TODO)

  0,					// *** current processing steps ***
  724,					// *** number of processing steps per frame ***

  {
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 4088 /* Debayer_G_Buffer_0 */,			// *** pointer to output buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1286,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 4304 /* Debayer_G_Buffer_1 */,			// *** pointer to output buffer ***
      {
        D_IPUS_SYN_S_LINE1_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        643,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
  },	// output buffer structs

  {
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 3872 /* ChGain_Exposure_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1288,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 3872 /* ChGain_Exposure_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE1_LEN_INA_OFF,				// *** offset to SDMA's register ***
        1,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1288,					// *** number of pixels to transfer ***
      },
      1,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 3872 /* ChGain_Exposure_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE2_LEN_INA_OFF,				// *** offset to SDMA's register ***
        2,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1288,					// *** number of pixels to transfer ***
      },
      2,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
  },	// input buffer structs
  (SEQ_CfgPara_CPtr_t) 924 /*Debayer_GPara */,	// *** pointer to configuration parameters ***
  8,					// *** number of configuration parameters following ***
  0,				// *** pad alignment ***
};

static SEQ_IpuCfg_t Debayer_BR = {
  {
    SEQ_NODE_SCALAR,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 5*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    5,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  0x00000000,			// *** code entry address *** (TODO)

  0,					// *** current processing steps ***
  720,					// *** number of processing steps per frame ***

  {
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 4520 /* Debayer_BR_Buffer_2 */,			// *** pointer to output buffer ***
      {
        D_IPUS_SYN_S_LINE2_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1284,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 4736 /* Debayer_BR_Buffer_1 */,			// *** pointer to output buffer ***
      {
        D_IPUS_SYN_S_LINE1_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1284,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 4952 /* Debayer_BR_Buffer_0 */,			// *** pointer to output buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1284,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
  },	// output buffer structs

  {
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 4304 /* Debayer_G_Buffer_1 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        643,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 4304 /* Debayer_G_Buffer_1 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE1_LEN_INA_OFF,				// *** offset to SDMA's register ***
        1,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        643,					// *** number of pixels to transfer ***
      },
      1,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 4304 /* Debayer_G_Buffer_1 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE2_LEN_INA_OFF,				// *** offset to SDMA's register ***
        2,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        643,					// *** number of pixels to transfer ***
      },
      2,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 4088 /* Debayer_G_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_INB_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1286,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 4088 /* Debayer_G_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE1_LEN_INB_OFF,				// *** offset to SDMA's register ***
        1,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1286,					// *** number of pixels to transfer ***
      },
      1,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 4088 /* Debayer_G_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE2_LEN_INB_OFF,				// *** offset to SDMA's register ***
        2,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1286,					// *** number of pixels to transfer ***
      },
      2,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
  },	// input buffer structs
  (SEQ_CfgPara_CPtr_t) 1240 /*Debayer_BRPara */,	// *** pointer to configuration parameters ***
  13,					// *** number of configuration parameters following ***
  0,				// *** pad alignment ***
};

static SEQ_IpuCfg_t RGB_2_YUV = {
  {
    SEQ_NODE_VECTOR,			// *** type of engine ***
    D_IPUV_SYN_BASEADDRESS + 1*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    1,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  0x00000000,			// *** code entry address *** (TODO)

  0,					// *** current processing steps ***
  720,					// *** number of processing steps per frame ***

  {
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 5168 /* RGB_2_YUV_Buffer_0 */,			// *** pointer to output buffer ***
      {
        D_IPUV_SYN_S_LINE0_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1280,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 5384 /* RGB_2_YUV_Buffer_1 */,			// *** pointer to output buffer ***
      {
        D_IPUV_SYN_S_LINE1_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1280,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 5600 /* RGB_2_YUV_Buffer_2 */,			// *** pointer to output buffer ***
      {
        D_IPUV_SYN_S_LINE2_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1280,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
  },	// output buffer structs

  {
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 4952 /* Debayer_BR_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUV_SYN_S_LINE0_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1280,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 4520 /* Debayer_BR_Buffer_2 */,			// *** pointer to input buffer ***
      {
        D_IPUV_SYN_S_LINE1_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1280,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 4736 /* Debayer_BR_Buffer_1 */,			// *** pointer to input buffer ***
      {
        D_IPUV_SYN_S_LINE2_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1280,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
  },	// input buffer structs
  (SEQ_CfgPara_CPtr_t) 1596 /*RGB_2_YUVPara */,	// *** pointer to configuration parameters ***
  9,					// *** number of configuration parameters following ***
  0,				// *** pad alignment ***
};

static SEQ_IpuCfg_t Y2_DOWNSCALE = {
  {
    SEQ_NODE_SCALAR,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 6*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    6,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  0x00000000,			// *** code entry address *** (TODO)

  0,					// *** current processing steps ***
  360,					// *** number of processing steps per frame ***

  {
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 6032 /* Y2_DOWNSCALE_Buffer_0 */,			// *** pointer to output buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        640,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
  },	// output buffer structs

  {
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 5168 /* RGB_2_YUV_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1280,					// *** number of pixels to transfer ***
      },
      1,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 5168 /* RGB_2_YUV_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE1_LEN_INA_OFF,				// *** offset to SDMA's register ***
        1,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        1280,					// *** number of pixels to transfer ***
      },
      2,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
  },	// input buffer structs
  (SEQ_CfgPara_CPtr_t) 1956 /*Y2_DOWNSCALEPara */,	// *** pointer to configuration parameters ***
  6,					// *** number of configuration parameters following ***
  0,				// *** pad alignment ***
};

static SEQ_IpuCfg_t Y4_DOWNSCALE = {
  {
    SEQ_NODE_SCALAR,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS,		// *** address of hardware device ***
    0,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  0x00000000,			// *** code entry address *** (TODO)

  0,					// *** current processing steps ***
  180,					// *** number of processing steps per frame ***

  {
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 6464 /* Y4_DOWNSCALE_Buffer_0 */,			// *** pointer to output buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        320,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
  },	// output buffer structs

  {
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 6032 /* Y2_DOWNSCALE_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        640,					// *** number of pixels to transfer ***
      },
      1,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 6032 /* Y2_DOWNSCALE_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE1_LEN_INA_OFF,				// *** offset to SDMA's register ***
        1,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        640,					// *** number of pixels to transfer ***
      },
      2,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
  },	// input buffer structs
  (SEQ_CfgPara_CPtr_t) 2256 /*Y4_DOWNSCALEPara */,	// *** pointer to configuration parameters ***
  6,					// *** number of configuration parameters following ***
  0,				// *** pad alignment ***
};

static SEQ_IpuCfg_t Y8_DOWNSCALE = {
  {
    SEQ_NODE_SCALAR,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 4*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    4,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  0x00000000,			// *** code entry address *** (TODO)

  0,					// *** current processing steps ***
  90,					// *** number of processing steps per frame ***

  {
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 6896 /* Y8_DOWNSCALE_Buffer_0 */,			// *** pointer to output buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        160,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
  },	// output buffer structs

  {
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 6464 /* Y4_DOWNSCALE_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        320,					// *** number of pixels to transfer ***
      },
      1,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 6464 /* Y4_DOWNSCALE_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE1_LEN_INA_OFF,				// *** offset to SDMA's register ***
        1,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        320,					// *** number of pixels to transfer ***
      },
      2,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
  },	// input buffer structs
  (SEQ_CfgPara_CPtr_t) 2556 /*Y8_DOWNSCALEPara */,	// *** pointer to configuration parameters ***
  6,					// *** number of configuration parameters following ***
  0,				// *** pad alignment ***
};

static SEQ_IpuCfg_t Y16_DOWNSCALE = {
  {
    SEQ_NODE_SCALAR,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 7*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    7,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  0x00000000,			// *** code entry address *** (TODO)

  0,					// *** current processing steps ***
  45,					// *** number of processing steps per frame ***

  {
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 7328 /* Y16_DOWNSCALE_Buffer_0 */,			// *** pointer to output buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        80,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
  },	// output buffer structs

  {
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 6896 /* Y8_DOWNSCALE_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE0_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        160,					// *** number of pixels to transfer ***
      },
      1,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 6896 /* Y8_DOWNSCALE_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUS_SYN_S_LINE1_LEN_INA_OFF,				// *** offset to SDMA's register ***
        1,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        160,					// *** number of pixels to transfer ***
      },
      2,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
  },	// input buffer structs
  (SEQ_CfgPara_CPtr_t) 2856 /*Y16_DOWNSCALEPara */,	// *** pointer to configuration parameters ***
  6,					// *** number of configuration parameters following ***
  0,				// *** pad alignment ***
};

static SEQ_IpuCfg_t Y32_DOWNSCALE = {
  {
    SEQ_NODE_VECTOR,			// *** type of engine ***
    D_IPUV_SYN_BASEADDRESS,		// *** address of hardware device ***
    0,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  0x00000000,			// *** code entry address *** (TODO)

  0,					// *** current processing steps ***
  22,					// *** number of processing steps per frame ***

  {
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 7760 /* Y32_DOWNSCALE_Buffer_0 */,			// *** pointer to output buffer ***
      {
        D_IPUV_SYN_S_LINE0_LEN_OUT_OFF,	// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        40,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
    // *** output buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to output buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
    },
  },	// output buffer structs

  {
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 7328 /* Y16_DOWNSCALE_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUV_SYN_S_LINE0_LEN_INA_OFF,				// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        80,					// *** number of pixels to transfer ***
      },
      1,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 7328 /* Y16_DOWNSCALE_Buffer_0 */,			// *** pointer to input buffer ***
      {
        D_IPUV_SYN_S_LINE1_LEN_INA_OFF,				// *** offset to SDMA's register ***
        1,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        80,					// *** number of pixels to transfer ***
      },
      2,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
    // *** input buffer # ***
    {
      (SEQ_Buf_Ptr_t) 0,				// *** pointer to input buffer ***
      {
        0,					// *** offset to SDMA's register ***
        0,					// *** number of first line to be used ***
        0,					// *** number of bytes to first pixel in line ***
        0,					// *** number of pixels to transfer ***
      },
      0,				// *** our index in buffers consumer array ***
      0,				// *** pad alignment ***
    },
  },	// input buffer structs
  (SEQ_CfgPara_CPtr_t) 3156 /*Y32_DOWNSCALEPara */,	// *** pointer to configuration parameters ***
  6,					// *** number of configuration parameters following ***
  0,				// *** pad alignment ***
};

static SEQ_FdmaCfg_t FastDMA = {
  {
    SEQ_NODE_OTHER,			// *** type of engine ***
    (Addr_t)(uintptr_t)&FDMA,	// *** address of hardware device ***
    SEQ_OTHRIX_FDMA,			// *** index of requested engine ***
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  {
    // *** output buffer #0 ***
    {
      (SEQ_Buf_Ptr_t) 3656 /* FastDMA_Buffer_0 */,	// *** offset to destination buffer ***
    },

    // *** output buffer #1 ***
    {
      (SEQ_Buf_Ptr_t) 6248 /* FastDMA_Buffer_1 */,	// *** offset to destination buffer ***
    },

    // *** output buffer #2 ***
    {
      (SEQ_Buf_Ptr_t) 6680 /* FastDMA_Buffer_2 */,	// *** offset to destination buffer ***
    },

    // *** output buffer #3 ***
    {
      (SEQ_Buf_Ptr_t) 7112 /* FastDMA_Buffer_3 */,	// *** offset to destination buffer ***
    },

    // *** output buffer #4 ***
    {
      (SEQ_Buf_Ptr_t) 7544 /* FastDMA_Buffer_4 */,	// *** offset to destination buffer ***
    },

    // *** output buffer #5 ***
    {
      (SEQ_Buf_Ptr_t) 7976 /* FastDMA_Buffer_5 */,	// *** offset to destination buffer ***
    },

    // *** output buffer #6 ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to destination buffer ***
    },

    // *** output buffer #7 ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to destination buffer ***
    },

    // *** output buffer #8 ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to destination buffer ***
    },

    // *** output buffer #9 ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to destination buffer ***
    },

    // *** output buffer #10 ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to destination buffer ***
    },

    // *** output buffer #11 ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to destination buffer ***
    },

    // *** output buffer #12 ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to destination buffer ***
    },

    // *** output buffer #13 ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to destination buffer ***
    },

    // *** output buffer #14 ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to destination buffer ***
    },

    // *** output buffer #15 ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to destination buffer ***
    },

  },

  // ### inputs ###############################################################

  {
    {
      // *** input buffer #0 ***
      (SEQ_Buf_Ptr_t) 3440 /* ConvertYUV_Buffer_0 */,	// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #1 ***
      (SEQ_Buf_Ptr_t) 6032 /* Y2_DOWNSCALE_Buffer_0 */,	// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #2 ***
      (SEQ_Buf_Ptr_t) 6464 /* Y4_DOWNSCALE_Buffer_0 */,	// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #3 ***
      (SEQ_Buf_Ptr_t) 6896 /* Y8_DOWNSCALE_Buffer_0 */,	// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #4 ***
      (SEQ_Buf_Ptr_t) 7328 /* Y16_DOWNSCALE_Buffer_0 */,	// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #5 ***
      (SEQ_Buf_Ptr_t) 7760 /* Y32_DOWNSCALE_Buffer_0 */,	// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #6 ***
      (SEQ_Buf_Ptr_t) 0,			// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #7 ***
      (SEQ_Buf_Ptr_t) 0,			// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #8 ***
      (SEQ_Buf_Ptr_t) 0,			// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #9 ***
      (SEQ_Buf_Ptr_t) 0,			// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #10 ***
      (SEQ_Buf_Ptr_t) 0,			// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #11 ***
      (SEQ_Buf_Ptr_t) 0,			// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #12 ***
      (SEQ_Buf_Ptr_t) 0,			// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #13 ***
      (SEQ_Buf_Ptr_t) 0,			// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #14 ***
      (SEQ_Buf_Ptr_t) 0,			// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #15 ***
      (SEQ_Buf_Ptr_t) 0,			// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

  },
}; // FastDMA

static SEQ_Load_t sCodegamma_yuv444touyvy_ConvertYUV = {
  {
    SEQ_NODE_FWLOAD,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 3*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    3,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  gamma_yuv444touyvy_start,		// *** Kernel address in KMEM *** 
  gamma_yuv444touyvy_end - gamma_yuv444touyvy_start,	// *** Kernel length in bytes ***
  0,						// *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,				// *** pad alignment ***
  (SEQ_Head_Ptr_t) 92 /* ConvertYUV.mHead */,			// *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodeexposure_tonehdr_ChGain_Exposure = {
  {
    SEQ_NODE_FWLOAD,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 1*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    1,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  exposure_tonehdr_start,		// *** Kernel address in KMEM *** 
  exposure_tonehdr_end - exposure_tonehdr_start,	// *** Kernel length in bytes ***
  0,						// *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,				// *** pad alignment ***
  (SEQ_Head_Ptr_t) 400 /* ChGain_Exposure.mHead */,			// *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodedebayer_g_Debayer_G = {
  {
    SEQ_NODE_FWLOAD,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 2*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    2,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  debayer_g_start,		// *** Kernel address in KMEM *** 
  debayer_g_end - debayer_g_start,	// *** Kernel length in bytes ***
  0,						// *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,				// *** pad alignment ***
  (SEQ_Head_Ptr_t) 700 /* Debayer_G.mHead */,			// *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodedebayer_br_advanced_Debayer_BR = {
  {
    SEQ_NODE_FWLOAD,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 5*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    5,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  debayer_br_advanced_start,		// *** Kernel address in KMEM *** 
  debayer_br_advanced_end - debayer_br_advanced_start,	// *** Kernel length in bytes ***
  0,						// *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,				// *** pad alignment ***
  (SEQ_Head_Ptr_t) 1016 /* Debayer_BR.mHead */,			// *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodergb2yuv_uint_RGB_2_YUV = {
  {
    SEQ_NODE_FWLOAD,			// *** type of engine ***
    D_IPUV_SYN_BASEADDRESS + 1*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    1,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  rgb2yuv_uint_start,		// *** Kernel address in KMEM *** 
  rgb2yuv_uint_end - rgb2yuv_uint_start,	// *** Kernel length in bytes ***
  0,						// *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,				// *** pad alignment ***
  (SEQ_Head_Ptr_t) 1372 /* RGB_2_YUV.mHead */,			// *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodescale2by1_11_Y2_DOWNSCALE = {
  {
    SEQ_NODE_FWLOAD,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 6*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    6,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  scale2by1_11_start,		// *** Kernel address in KMEM *** 
  scale2by1_11_end - scale2by1_11_start,	// *** Kernel length in bytes ***
  0,						// *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,				// *** pad alignment ***
  (SEQ_Head_Ptr_t) 1732 /* Y2_DOWNSCALE.mHead */,			// *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodescale2by1_11_Y4_DOWNSCALE = {
  {
    SEQ_NODE_FWLOAD,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS,		// *** address of hardware device ***
    0,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  scale2by1_11_start,		// *** Kernel address in KMEM *** 
  scale2by1_11_end - scale2by1_11_start,	// *** Kernel length in bytes ***
  0,						// *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,				// *** pad alignment ***
  (SEQ_Head_Ptr_t) 2032 /* Y4_DOWNSCALE.mHead */,			// *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodescale2by1_11_Y8_DOWNSCALE = {
  {
    SEQ_NODE_FWLOAD,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 4*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    4,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  scale2by1_11_start,		// *** Kernel address in KMEM *** 
  scale2by1_11_end - scale2by1_11_start,	// *** Kernel length in bytes ***
  0,						// *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,				// *** pad alignment ***
  (SEQ_Head_Ptr_t) 2332 /* Y8_DOWNSCALE.mHead */,			// *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodescale2by1_11_Y16_DOWNSCALE = {
  {
    SEQ_NODE_FWLOAD,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 7*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    7,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  scale2by1_11_start,		// *** Kernel address in KMEM *** 
  scale2by1_11_end - scale2by1_11_start,	// *** Kernel length in bytes ***
  0,						// *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,				// *** pad alignment ***
  (SEQ_Head_Ptr_t) 2632 /* Y16_DOWNSCALE.mHead */,			// *** pointer to IPU node to start ***
};

static SEQ_Load_t sCodescale2by1_11_ipuv_Y32_DOWNSCALE = {
  {
    SEQ_NODE_FWLOAD,			// *** type of engine ***
    D_IPUV_SYN_BASEADDRESS,		// *** address of hardware device ***
    0,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  scale2by1_11_ipuv_start,		// *** Kernel address in KMEM *** 
  scale2by1_11_ipuv_end - scale2by1_11_ipuv_start,	// *** Kernel length in bytes ***
  0,						// *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,				// *** pad alignment ***
  (SEQ_Head_Ptr_t) 2932 /* Y32_DOWNSCALE.mHead */,			// *** pointer to IPU node to start ***
};

/*****************************************************************************
* exported data
*****************************************************************************/


// *** graph ***

SEQ_Head_Ptr_t gpGraph[] = {
  (SEQ_Head_Ptr_t) 3232, // FastDMA
  (SEQ_Head_Ptr_t) 1696, // CAM0
  (SEQ_Head_Ptr_t) 92, // ConvertYUV
  (SEQ_Head_Ptr_t) 400, // ChGain_Exposure
  (SEQ_Head_Ptr_t) 700, // Debayer_G
  (SEQ_Head_Ptr_t) 1016, // Debayer_BR
  (SEQ_Head_Ptr_t) 1372, // RGB_2_YUV
  (SEQ_Head_Ptr_t) 1732, // Y2_DOWNSCALE
  (SEQ_Head_Ptr_t) 2032, // Y4_DOWNSCALE
  (SEQ_Head_Ptr_t) 2332, // Y8_DOWNSCALE
  (SEQ_Head_Ptr_t) 2632, // Y16_DOWNSCALE
  (SEQ_Head_Ptr_t) 2932, // Y32_DOWNSCALE
  (SEQ_Head_Ptr_t) 372, // sCodegamma_yuv444touyvy_ConvertYUV
  (SEQ_Head_Ptr_t) 672, // sCodeexposure_tonehdr_ChGain_Exposure
  (SEQ_Head_Ptr_t) 988, // sCodedebayer_g_Debayer_G
  (SEQ_Head_Ptr_t) 1344, // sCodedebayer_br_advanced_Debayer_BR
  (SEQ_Head_Ptr_t) 1668, // sCodergb2yuv_uint_RGB_2_YUV
  (SEQ_Head_Ptr_t) 2004, // sCodescale2by1_11_Y2_DOWNSCALE
  (SEQ_Head_Ptr_t) 2304, // sCodescale2by1_11_Y4_DOWNSCALE
  (SEQ_Head_Ptr_t) 2604, // sCodescale2by1_11_Y8_DOWNSCALE
  (SEQ_Head_Ptr_t) 2904, // sCodescale2by1_11_Y16_DOWNSCALE
  (SEQ_Head_Ptr_t) 3204, // sCodescale2by1_11_ipuv_Y32_DOWNSCALE
  (SEQ_Head_Ptr_t) 0		// ** END ***
};

// *** Graph parsing metadata ***

static Object_t sObjectList[55] = {
  { &gpGraph,	92,	0 },
  { &ConvertYUV,	224,	92 },
  { &ConvertYUVPara,	56,	316 },
  { &sCodegamma_yuv444touyvy_ConvertYUV,	28,	372 },
  { &ChGain_Exposure,	224,	400 },
  { &ChGain_ExposurePara,	48,	624 },
  { &sCodeexposure_tonehdr_ChGain_Exposure,	28,	672 },
  { &Debayer_G,	224,	700 },
  { &Debayer_GPara,	64,	924 },
  { &sCodedebayer_g_Debayer_G,	28,	988 },
  { &Debayer_BR,	224,	1016 },
  { &Debayer_BRPara,	104,	1240 },
  { &sCodedebayer_br_advanced_Debayer_BR,	28,	1344 },
  { &RGB_2_YUV,	224,	1372 },
  { &RGB_2_YUVPara,	72,	1596 },
  { &sCodergb2yuv_uint_RGB_2_YUV,	28,	1668 },
  { &CAM0,	36,	1696 },
  { &Y2_DOWNSCALE,	224,	1732 },
  { &Y2_DOWNSCALEPara,	48,	1956 },
  { &sCodescale2by1_11_Y2_DOWNSCALE,	28,	2004 },
  { &Y4_DOWNSCALE,	224,	2032 },
  { &Y4_DOWNSCALEPara,	48,	2256 },
  { &sCodescale2by1_11_Y4_DOWNSCALE,	28,	2304 },
  { &Y8_DOWNSCALE,	224,	2332 },
  { &Y8_DOWNSCALEPara,	48,	2556 },
  { &sCodescale2by1_11_Y8_DOWNSCALE,	28,	2604 },
  { &Y16_DOWNSCALE,	224,	2632 },
  { &Y16_DOWNSCALEPara,	48,	2856 },
  { &sCodescale2by1_11_Y16_DOWNSCALE,	28,	2904 },
  { &Y32_DOWNSCALE,	224,	2932 },
  { &Y32_DOWNSCALEPara,	48,	3156 },
  { &sCodescale2by1_11_ipuv_Y32_DOWNSCALE,	28,	3204 },
  { &FastDMA,	208,	3232 },
  { &ConvertYUV_Buffer_0,	216,	3440 },
  { &FastDMA_Buffer_0,	216,	3656 },
  { &ChGain_Exposure_Buffer_0,	216,	3872 },
  { &Debayer_G_Buffer_0,	216,	4088 },
  { &Debayer_G_Buffer_1,	216,	4304 },
  { &Debayer_BR_Buffer_2,	216,	4520 },
  { &Debayer_BR_Buffer_1,	216,	4736 },
  { &Debayer_BR_Buffer_0,	216,	4952 },
  { &RGB_2_YUV_Buffer_0,	216,	5168 },
  { &RGB_2_YUV_Buffer_1,	216,	5384 },
  { &RGB_2_YUV_Buffer_2,	216,	5600 },
  { &CAM0_Buffer_0,	216,	5816 },
  { &Y2_DOWNSCALE_Buffer_0,	216,	6032 },
  { &FastDMA_Buffer_1,	216,	6248 },
  { &Y4_DOWNSCALE_Buffer_0,	216,	6464 },
  { &FastDMA_Buffer_2,	216,	6680 },
  { &Y8_DOWNSCALE_Buffer_0,	216,	6896 },
  { &FastDMA_Buffer_3,	216,	7112 },
  { &Y16_DOWNSCALE_Buffer_0,	216,	7328 },
  { &FastDMA_Buffer_4,	216,	7544 },
  { &Y32_DOWNSCALE_Buffer_0,	216,	7760 },
  { &FastDMA_Buffer_5,	216,	7976 },
};

static uint32_t sPointerList[] = {
  0, /* gpGraph: pointer to FastDMA */
  4, /* gpGraph: pointer to CAM0 */
  8, /* gpGraph: pointer to ConvertYUV */
  12, /* gpGraph: pointer to ChGain_Exposure */
  16, /* gpGraph: pointer to Debayer_G */
  20, /* gpGraph: pointer to Debayer_BR */
  24, /* gpGraph: pointer to RGB_2_YUV */
  28, /* gpGraph: pointer to Y2_DOWNSCALE */
  32, /* gpGraph: pointer to Y4_DOWNSCALE */
  36, /* gpGraph: pointer to Y8_DOWNSCALE */
  40, /* gpGraph: pointer to Y16_DOWNSCALE */
  44, /* gpGraph: pointer to Y32_DOWNSCALE */
  48, /* gpGraph: pointer to sCodegamma_yuv444touyvy_ConvertYUV */
  52, /* gpGraph: pointer to sCodeexposure_tonehdr_ChGain_Exposure */
  56, /* gpGraph: pointer to sCodedebayer_g_Debayer_G */
  60, /* gpGraph: pointer to sCodedebayer_br_advanced_Debayer_BR */
  64, /* gpGraph: pointer to sCodergb2yuv_uint_RGB_2_YUV */
  68, /* gpGraph: pointer to sCodescale2by1_11_Y2_DOWNSCALE */
  72, /* gpGraph: pointer to sCodescale2by1_11_Y4_DOWNSCALE */
  76, /* gpGraph: pointer to sCodescale2by1_11_Y8_DOWNSCALE */
  80, /* gpGraph: pointer to sCodescale2by1_11_Y16_DOWNSCALE */
  84, /* gpGraph: pointer to sCodescale2by1_11_ipuv_Y32_DOWNSCALE */
  116, /* ConvertYUV: pointer to OUTput buffer */
  164, /* ConvertYUV: pointer to INput buffer */
  180, /* ConvertYUV: pointer to INput buffer */
  196, /* ConvertYUV: pointer to INput buffer */
  308, /* ConvertYUV: pointer to config parameters */
  396, /* sCodegamma_yuv444touyvy_ConvertYUV: pointer to ConvertYUV.mHead */
  424, /* ChGain_Exposure: pointer to OUTput buffer */
  472, /* ChGain_Exposure: pointer to INput buffer */
  488, /* ChGain_Exposure: pointer to INput buffer */
  616, /* ChGain_Exposure: pointer to config parameters */
  696, /* sCodeexposure_tonehdr_ChGain_Exposure: pointer to ChGain_Exposure.mHead */
  724, /* Debayer_G: pointer to OUTput buffer */
  736, /* Debayer_G: pointer to OUTput buffer */
  772, /* Debayer_G: pointer to INput buffer */
  788, /* Debayer_G: pointer to INput buffer */
  804, /* Debayer_G: pointer to INput buffer */
  916, /* Debayer_G: pointer to config parameters */
  1012, /* sCodedebayer_g_Debayer_G: pointer to Debayer_G.mHead */
  1040, /* Debayer_BR: pointer to OUTput buffer */
  1052, /* Debayer_BR: pointer to OUTput buffer */
  1064, /* Debayer_BR: pointer to OUTput buffer */
  1088, /* Debayer_BR: pointer to INput buffer */
  1104, /* Debayer_BR: pointer to INput buffer */
  1120, /* Debayer_BR: pointer to INput buffer */
  1136, /* Debayer_BR: pointer to INput buffer */
  1152, /* Debayer_BR: pointer to INput buffer */
  1168, /* Debayer_BR: pointer to INput buffer */
  1232, /* Debayer_BR: pointer to config parameters */
  1368, /* sCodedebayer_br_advanced_Debayer_BR: pointer to Debayer_BR.mHead */
  1396, /* RGB_2_YUV: pointer to OUTput buffer */
  1408, /* RGB_2_YUV: pointer to OUTput buffer */
  1420, /* RGB_2_YUV: pointer to OUTput buffer */
  1444, /* RGB_2_YUV: pointer to INput buffer */
  1460, /* RGB_2_YUV: pointer to INput buffer */
  1476, /* RGB_2_YUV: pointer to INput buffer */
  1588, /* RGB_2_YUV: pointer to config parameters */
  1692, /* sCodergb2yuv_uint_RGB_2_YUV: pointer to RGB_2_YUV.mHead */
  1712, /* CAM0: pointer to OUTput buffer */
  1756, /* Y2_DOWNSCALE: pointer to OUTput buffer */
  1804, /* Y2_DOWNSCALE: pointer to INput buffer */
  1820, /* Y2_DOWNSCALE: pointer to INput buffer */
  1948, /* Y2_DOWNSCALE: pointer to config parameters */
  2028, /* sCodescale2by1_11_Y2_DOWNSCALE: pointer to Y2_DOWNSCALE.mHead */
  2056, /* Y4_DOWNSCALE: pointer to OUTput buffer */
  2104, /* Y4_DOWNSCALE: pointer to INput buffer */
  2120, /* Y4_DOWNSCALE: pointer to INput buffer */
  2248, /* Y4_DOWNSCALE: pointer to config parameters */
  2328, /* sCodescale2by1_11_Y4_DOWNSCALE: pointer to Y4_DOWNSCALE.mHead */
  2356, /* Y8_DOWNSCALE: pointer to OUTput buffer */
  2404, /* Y8_DOWNSCALE: pointer to INput buffer */
  2420, /* Y8_DOWNSCALE: pointer to INput buffer */
  2548, /* Y8_DOWNSCALE: pointer to config parameters */
  2628, /* sCodescale2by1_11_Y8_DOWNSCALE: pointer to Y8_DOWNSCALE.mHead */
  2656, /* Y16_DOWNSCALE: pointer to OUTput buffer */
  2704, /* Y16_DOWNSCALE: pointer to INput buffer */
  2720, /* Y16_DOWNSCALE: pointer to INput buffer */
  2848, /* Y16_DOWNSCALE: pointer to config parameters */
  2928, /* sCodescale2by1_11_Y16_DOWNSCALE: pointer to Y16_DOWNSCALE.mHead */
  2956, /* Y32_DOWNSCALE: pointer to OUTput buffer */
  3004, /* Y32_DOWNSCALE: pointer to INput buffer */
  3020, /* Y32_DOWNSCALE: pointer to INput buffer */
  3148, /* Y32_DOWNSCALE: pointer to config parameters */
  3228, /* sCodescale2by1_11_ipuv_Y32_DOWNSCALE: pointer to Y32_DOWNSCALE.mHead */
  3452, /* ConvertYUV_Buffer_0: pointer to producer */
  3464, /* ConvertYUV_Buffer_0: pointer to consumer */
  3668, /* FastDMA_Buffer_0: pointer to producer */
  3884, /* ChGain_Exposure_Buffer_0: pointer to producer */
  3896, /* ChGain_Exposure_Buffer_0: pointer to consumer */
  3920, /* ChGain_Exposure_Buffer_0: pointer to consumer */
  3944, /* ChGain_Exposure_Buffer_0: pointer to consumer */
  4100, /* Debayer_G_Buffer_0: pointer to producer */
  4112, /* Debayer_G_Buffer_0: pointer to consumer */
  4136, /* Debayer_G_Buffer_0: pointer to consumer */
  4160, /* Debayer_G_Buffer_0: pointer to consumer */
  4316, /* Debayer_G_Buffer_1: pointer to producer */
  4328, /* Debayer_G_Buffer_1: pointer to consumer */
  4352, /* Debayer_G_Buffer_1: pointer to consumer */
  4376, /* Debayer_G_Buffer_1: pointer to consumer */
  4532, /* Debayer_BR_Buffer_2: pointer to producer */
  4544, /* Debayer_BR_Buffer_2: pointer to consumer */
  4748, /* Debayer_BR_Buffer_1: pointer to producer */
  4760, /* Debayer_BR_Buffer_1: pointer to consumer */
  4964, /* Debayer_BR_Buffer_0: pointer to producer */
  4976, /* Debayer_BR_Buffer_0: pointer to consumer */
  5180, /* RGB_2_YUV_Buffer_0: pointer to producer */
  5192, /* RGB_2_YUV_Buffer_0: pointer to consumer */
  5216, /* RGB_2_YUV_Buffer_0: pointer to consumer */
  5240, /* RGB_2_YUV_Buffer_0: pointer to consumer */
  5396, /* RGB_2_YUV_Buffer_1: pointer to producer */
  5408, /* RGB_2_YUV_Buffer_1: pointer to consumer */
  5612, /* RGB_2_YUV_Buffer_2: pointer to producer */
  5624, /* RGB_2_YUV_Buffer_2: pointer to consumer */
  5828, /* CAM0_Buffer_0: pointer to producer */
  5840, /* CAM0_Buffer_0: pointer to consumer */
  5864, /* CAM0_Buffer_0: pointer to consumer */
  6044, /* Y2_DOWNSCALE_Buffer_0: pointer to producer */
  6056, /* Y2_DOWNSCALE_Buffer_0: pointer to consumer */
  6080, /* Y2_DOWNSCALE_Buffer_0: pointer to consumer */
  6104, /* Y2_DOWNSCALE_Buffer_0: pointer to consumer */
  6260, /* FastDMA_Buffer_1: pointer to producer */
  6476, /* Y4_DOWNSCALE_Buffer_0: pointer to producer */
  6488, /* Y4_DOWNSCALE_Buffer_0: pointer to consumer */
  6512, /* Y4_DOWNSCALE_Buffer_0: pointer to consumer */
  6536, /* Y4_DOWNSCALE_Buffer_0: pointer to consumer */
  6692, /* FastDMA_Buffer_2: pointer to producer */
  6908, /* Y8_DOWNSCALE_Buffer_0: pointer to producer */
  6920, /* Y8_DOWNSCALE_Buffer_0: pointer to consumer */
  6944, /* Y8_DOWNSCALE_Buffer_0: pointer to consumer */
  6968, /* Y8_DOWNSCALE_Buffer_0: pointer to consumer */
  7124, /* FastDMA_Buffer_3: pointer to producer */
  7340, /* Y16_DOWNSCALE_Buffer_0: pointer to producer */
  7352, /* Y16_DOWNSCALE_Buffer_0: pointer to consumer */
  7376, /* Y16_DOWNSCALE_Buffer_0: pointer to consumer */
  7400, /* Y16_DOWNSCALE_Buffer_0: pointer to consumer */
  7556, /* FastDMA_Buffer_4: pointer to producer */
  7772, /* Y32_DOWNSCALE_Buffer_0: pointer to producer */
  7784, /* Y32_DOWNSCALE_Buffer_0: pointer to consumer */
  7988, /* FastDMA_Buffer_5: pointer to producer */
};

static uint32_t sBufferList[] = {
  3440, /* ConvertYUV_Buffer_0 */
  3656, /* FastDMA_Buffer_0 */
  3872, /* ChGain_Exposure_Buffer_0 */
  4088, /* Debayer_G_Buffer_0 */
  4304, /* Debayer_G_Buffer_1 */
  4520, /* Debayer_BR_Buffer_2 */
  4736, /* Debayer_BR_Buffer_1 */
  4952, /* Debayer_BR_Buffer_0 */
  5168, /* RGB_2_YUV_Buffer_0 */
  5384, /* RGB_2_YUV_Buffer_1 */
  5600, /* RGB_2_YUV_Buffer_2 */
  5816, /* CAM0_Buffer_0 */
  6032, /* Y2_DOWNSCALE_Buffer_0 */
  6248, /* FastDMA_Buffer_1 */
  6464, /* Y4_DOWNSCALE_Buffer_0 */
  6680, /* FastDMA_Buffer_2 */
  6896, /* Y8_DOWNSCALE_Buffer_0 */
  7112, /* FastDMA_Buffer_3 */
  7328, /* Y16_DOWNSCALE_Buffer_0 */
  7544, /* FastDMA_Buffer_4 */
  7760, /* Y32_DOWNSCALE_Buffer_0 */
  7976, /* FastDMA_Buffer_5 */
};

static const char* scppFdmaChannelNames[] = {
  "YUV_TO_DDR",
  "Y2_SCALED_TO_DDR",
  "Y4_SCALED_TO_DDR",
  "Y8_SCALED_TO_DDR",
  "Y16_SCALED_TO_DDR",
  "Y32_SCALED_TO_DDR",
};

static const char* scppBufferNames[] = {
  "ConvertYUV_Buffer_0",
  "FastDMA_Buffer_0",
  "ChGain_Exposure_Buffer_0",
  "Debayer_G_Buffer_0",
  "Debayer_G_Buffer_1",
  "Debayer_BR_Buffer_2",
  "Debayer_BR_Buffer_1",
  "Debayer_BR_Buffer_0",
  "RGB_2_YUV_Buffer_0",
  "RGB_2_YUV_Buffer_1",
  "RGB_2_YUV_Buffer_2",
  "CAM0_Buffer_0",
  "Y2_DOWNSCALE_Buffer_0",
  "FastDMA_Buffer_1",
  "Y4_DOWNSCALE_Buffer_0",
  "FastDMA_Buffer_2",
  "Y8_DOWNSCALE_Buffer_0",
  "FastDMA_Buffer_3",
  "Y16_DOWNSCALE_Buffer_0",
  "FastDMA_Buffer_4",
  "Y32_DOWNSCALE_Buffer_0",
  "FastDMA_Buffer_5",
};

GraphMetadata_t gGraphMetadata = {
  8,		// *** max number of buffer consumers
  8192,		// *** graph size [bytes]
  (const Object_t*) &sObjectList,	// *** object list pointer
  55,		// *** object count
  (const uint32_t*) &sPointerList,	// *** pointer list pointer
  140,		// *** pointer count
  (const uint32_t*) &sBufferList,	// *** buffer list pointer
  22,		// *** buffer count
  {
    0
  },
  { // MIPI-CSI metadata
    { // node CAM0
      CSI_SONY224, 1,
      {
        0, 0, 0, 0, 
      },
      {
        1288 * 2, 0 * 1, 0 * 1, 0 * 1, 
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
