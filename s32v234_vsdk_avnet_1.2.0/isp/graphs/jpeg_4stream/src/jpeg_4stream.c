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
// U:\s32v234_sdk\isp\graphs\jpeg_4stream\ispgraph\model\jpeg_4stream.isp 

#ifdef  __cplusplus
extern "C" {
#endif

#include "jpeg_4stream_c.h"
#include "iofiles.h"
#ifndef S32DS_BUILD
#include "kmem.c"
#endif
#include "kmem_sym.h"
// *** forwards needed for cross-linking ***

static SEQ_FdmaCfg_t FastDMA;
static SEQ_IpuCfg_t Yuv420ToInterleaved;
static SEQ_Load_t sCodeyuv420planar2yuv422interleaved0_quad_Yuv420ToInterleaved;
static SEQ_JpegDecCfg_t JpegDec;

/*** Buffers ****************************************************************/

SEQ_Buf_t JpegDec_Buffer_1 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  64,				// *** number of lines fitting into output buffer (wrap) ***
  640 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(640),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 352 /*JpegDec*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  8,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
  // *** consumer/read #0 ***
  {
    (SEQ_Head_Ptr_t) 20 /* Yuv420ToInterleaved.mHead */,	// *** offset to consumers (node) configuration header ***
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
}; // JpegDec_Buffer_1

SEQ_Buf_t JpegDec_Buffer_2 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  64,				// *** number of lines fitting into output buffer (wrap) ***
  640 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(640),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 352 /*JpegDec*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  8,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
  // *** consumer/read #0 ***
  {
    (SEQ_Head_Ptr_t) 20 /* Yuv420ToInterleaved.mHead */,	// *** offset to consumers (node) configuration header ***
    2,				// *** the devices channel used ***
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
}; // JpegDec_Buffer_2

SEQ_Buf_t JpegDec_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  128,				// *** number of lines fitting into output buffer (wrap) ***
  1280 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(1280),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 352 /*JpegDec*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  16,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
  // *** consumer/read #0 ***
  {
    (SEQ_Head_Ptr_t) 20 /* Yuv420ToInterleaved.mHead */,	// *** offset to consumers (node) configuration header ***
    0,				// *** the devices channel used ***
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
}; // JpegDec_Buffer_0

SEQ_Buf_t Yuv420ToInterleaved_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  32,				// *** number of lines fitting into output buffer (wrap) ***
  1280 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(1280),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 20 /*Yuv420ToInterleaved*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
  // *** consumer/read #0 ***
  {
    (SEQ_Head_Ptr_t) 388 /* FastDMA.mHead */,	// *** offset to consumers (node) configuration header ***
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
}; // Yuv420ToInterleaved_Buffer_0

SEQ_Buf_t Yuv420ToInterleaved_Buffer_1 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  32,				// *** number of lines fitting into output buffer (wrap) ***
  1280 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(1280),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 20 /*Yuv420ToInterleaved*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
  // *** consumer/read #0 ***
  {
    (SEQ_Head_Ptr_t) 388 /* FastDMA.mHead */,	// *** offset to consumers (node) configuration header ***
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
}; // Yuv420ToInterleaved_Buffer_1

SEQ_Buf_t Yuv420ToInterleaved_Buffer_2 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  32,				// *** number of lines fitting into output buffer (wrap) ***
  1280 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(1280),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 20 /*Yuv420ToInterleaved*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
  // *** consumer/read #0 ***
  {
    (SEQ_Head_Ptr_t) 388 /* FastDMA.mHead */,	// *** offset to consumers (node) configuration header ***
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
}; // Yuv420ToInterleaved_Buffer_2

SEQ_Buf_t Yuv420ToInterleaved_Buffer_3 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  32,				// *** number of lines fitting into output buffer (wrap) ***
  1280 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(1280),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 20 /*Yuv420ToInterleaved*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
  // *** consumer/read #0 ***
  {
    (SEQ_Head_Ptr_t) 388 /* FastDMA.mHead */,	// *** offset to consumers (node) configuration header ***
    3,				// *** the devices channel used ***
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
}; // Yuv420ToInterleaved_Buffer_3

SEQ_Buf_t JpegDec_Buffer_3 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 0,		// *** pointer to output buffer ***
  2,				// *** number of lines fitting into output buffer (wrap) ***
  1280 * 1,				// *** number of bytes per line ***
  BUF_ALIGN(1280),				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 352 /*JpegDec*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
  // *** consumer/read #0 ***
  {
    (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
    0,				// *** the devices channel used ***
    0,				// *** number of lines to skip at frame begin ***
    0,				// *** number of lines required for processing ***
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
}; // JpegDec_Buffer_3

SEQ_Buf_t FastDMA_Buffer_0 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 1,		// *** pointer to output buffer ***
  400,				// *** number of lines fitting into output buffer (wrap) ***
  1280 * 1,				// *** number of bytes per line ***
  1280,				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 388 /*FastDMA*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
  // *** consumer/read #0 ***
  {
    (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
    0,				// *** the devices channel used ***
    0,				// *** number of lines to skip at frame begin ***
    0,				// *** number of lines required for processing ***
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
}; // FastDMA_Buffer_0

SEQ_Buf_t FastDMA_Buffer_1 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 1,		// *** pointer to output buffer ***
  400,				// *** number of lines fitting into output buffer (wrap) ***
  1280 * 1,				// *** number of bytes per line ***
  1280,				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 388 /*FastDMA*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
  // *** consumer/read #0 ***
  {
    (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
    0,				// *** the devices channel used ***
    0,				// *** number of lines to skip at frame begin ***
    0,				// *** number of lines required for processing ***
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
}; // FastDMA_Buffer_1

SEQ_Buf_t FastDMA_Buffer_2 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 1,		// *** pointer to output buffer ***
  400,				// *** number of lines fitting into output buffer (wrap) ***
  1280 * 1,				// *** number of bytes per line ***
  1280,				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 388 /*FastDMA*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
  // *** consumer/read #0 ***
  {
    (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
    0,				// *** the devices channel used ***
    0,				// *** number of lines to skip at frame begin ***
    0,				// *** number of lines required for processing ***
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
}; // FastDMA_Buffer_2

SEQ_Buf_t FastDMA_Buffer_3 = {

  // *** buffer ***
  (SEQ_Mem_Ptr_t) 1,		// *** pointer to output buffer ***
  400,				// *** number of lines fitting into output buffer (wrap) ***
  1280 * 1,				// *** number of bytes per line ***
  1280,				// *** number of bytes to next line begin ***

  // *** producer/write ***
  0,				// *** buffers status ***
  (SEQ_Head_Ptr_t) 388 /*FastDMA*/,		// *** offset to producers (node) configuration header ***
  0,				// *** current relative index into output buffer (write) ***
  1,				// *** increment of current relative index ***
  0,				// *** total lines written to buffer ***
  0,				// *** pad alignment ***

  {
  // *** consumer/read #0 ***
  {
    (SEQ_Head_Ptr_t) 0,		// *** pointer to consumers (node) configuration header ***
    0,				// *** the devices channel used ***
    0,				// *** number of lines to skip at frame begin ***
    0,				// *** number of lines required for processing ***
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
}; // FastDMA_Buffer_3


static SEQ_CfgPara_t Yuv420ToInterleavedPara [] = {
    { D_IPUS_SYN_HOST_INACFG_OFF, 0x00000700 },
    { D_IPUS_SYN_HOST_XCFG_OFF, (320 << 16) | 0x1 },
    { D_IPUS_SYN_HOST_OUTCFG_OFF, 0x00000f00 },
    { D_IPUS_SYN_S_CH0_CFG_INA_OFF, 0x10000000 },
    { D_IPUS_SYN_S_CH1_CFG_INA_OFF, 0x10020000 },
    { D_IPUS_SYN_S_CH2_CFG_INA_OFF, 0x10020000 },
    { D_IPUS_SYN_S_CH0_CFG_OUT_OFF, 0x10000000 },
    { D_IPUS_SYN_S_CH1_CFG_OUT_OFF, 0x10000000 },
    { D_IPUS_SYN_S_CH2_CFG_OUT_OFF, 0x10000000 },
    { D_IPUS_SYN_S_CH3_CFG_OUT_OFF, 0x10000000 },
};

static SEQ_JpegDecCfg_t JpegDec = {
      {
        SEQ_NODE_OTHER,			// *** type of engine ***
        (Addr_t)(uintptr_t)&jpeg,	// *** address of hardware device ***
        SEQ_OTHRIX_JPEGDEC,			// *** index of requested engine ***
        0,					// *** engine is running ***
        0,					// *** engine is done for this frame ***
        0,					// *** engine got some update ***
      },
    
      // *** output buffer ***
        (SEQ_Buf_Ptr_t) 1028 /*JpegDec_Buffer_0*/,	// *** pointer to output buffer ***
    
        (SEQ_Buf_Ptr_t) 596 /*JpegDec_Buffer_1*/,	// *** pointer to output buffer ***
    
        (SEQ_Buf_Ptr_t) 812 /*JpegDec_Buffer_2*/,	// *** pointer to output buffer ***
    
        (SEQ_Buf_Ptr_t) 2108 /*JpegDec_Buffer_3*/,	// *** pointer to output buffer ***
    
      800,			// *** number of input image lines ***
      0				// *** pad alignment ***
    }; // JpegDec

static SEQ_IpuCfg_t Yuv420ToInterleaved = {
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
      1600,					// *** number of processing steps per frame ***
    
      {
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 1244 /* Yuv420ToInterleaved_Buffer_0 */,			// *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE0_LEN_OUT_OFF,	// *** offset to SDMA's register ***
            0,					// *** number of first line to be used ***
            0,					// *** number of bytes to first pixel in line ***
            1280,					// *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 1460 /* Yuv420ToInterleaved_Buffer_1 */,			// *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE1_LEN_OUT_OFF,	// *** offset to SDMA's register ***
            0,					// *** number of first line to be used ***
            0,					// *** number of bytes to first pixel in line ***
            1280,					// *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 1676 /* Yuv420ToInterleaved_Buffer_2 */,			// *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE2_LEN_OUT_OFF,	// *** offset to SDMA's register ***
            0,					// *** number of first line to be used ***
            0,					// *** number of bytes to first pixel in line ***
            1280,					// *** number of pixels to transfer ***
          },
        },
        // *** output buffer # ***
        {
          (SEQ_Buf_Ptr_t) 1892 /* Yuv420ToInterleaved_Buffer_3 */,			// *** pointer to output buffer ***
          {
            D_IPUS_SYN_S_LINE3_LEN_OUT_OFF,	// *** offset to SDMA's register ***
            0,					// *** number of first line to be used ***
            0,					// *** number of bytes to first pixel in line ***
            1280,					// *** number of pixels to transfer ***
          },
        },
      },	// output buffer structs
    
      {
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 1028 /* JpegDec_Buffer_0 */,			// *** pointer to input buffer ***
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
          (SEQ_Buf_Ptr_t) 596 /* JpegDec_Buffer_1 */,			// *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE1_LEN_INA_OFF,				// *** offset to SDMA's register ***
            0,					// *** number of first line to be used ***
            0,					// *** number of bytes to first pixel in line ***
            640,					// *** number of pixels to transfer ***
          },
          0,				// *** our index in buffers consumer array ***
          0,				// *** pad alignment ***
        },
        // *** input buffer # ***
        {
          (SEQ_Buf_Ptr_t) 812 /* JpegDec_Buffer_2 */,			// *** pointer to input buffer ***
          {
            D_IPUS_SYN_S_LINE2_LEN_INA_OFF,				// *** offset to SDMA's register ***
            0,					// *** number of first line to be used ***
            0,					// *** number of bytes to first pixel in line ***
            640,					// *** number of pixels to transfer ***
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
      (SEQ_CfgPara_CPtr_t) 244 /*Yuv420ToInterleavedPara */,	// *** pointer to configuration parameters ***
      10,					// *** number of configuration parameters following ***
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
      (SEQ_Buf_Ptr_t) 2324 /* FastDMA_Buffer_0 */,	// *** offset to destination buffer ***
    },

    // *** output buffer #1 ***
    {
      (SEQ_Buf_Ptr_t) 2540 /* FastDMA_Buffer_1 */,	// *** offset to destination buffer ***
    },

    // *** output buffer #2 ***
    {
      (SEQ_Buf_Ptr_t) 2756 /* FastDMA_Buffer_2 */,	// *** offset to destination buffer ***
    },

    // *** output buffer #3 ***
    {
      (SEQ_Buf_Ptr_t) 2972 /* FastDMA_Buffer_3 */,	// *** offset to destination buffer ***
    },

    // *** output buffer #4 ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to destination buffer ***
    },

    // *** output buffer #5 ***
    {
      (SEQ_Buf_Ptr_t) 0,			// *** pointer to destination buffer ***
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
      (SEQ_Buf_Ptr_t) 1244 /* Yuv420ToInterleaved_Buffer_0 */,	// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #1 ***
      (SEQ_Buf_Ptr_t) 1460 /* Yuv420ToInterleaved_Buffer_1 */,	// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #2 ***
      (SEQ_Buf_Ptr_t) 1676 /* Yuv420ToInterleaved_Buffer_2 */,	// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #3 ***
      (SEQ_Buf_Ptr_t) 1892 /* Yuv420ToInterleaved_Buffer_3 */,	// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #4 ***
      (SEQ_Buf_Ptr_t) 0,			// *** input buffer ***
      0, 					// *** index into buffers consumer array ***
      0, 				// *** line offset ***
    },

    {
      // *** input buffer #5 ***
      (SEQ_Buf_Ptr_t) 0,			// *** input buffer ***
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

static SEQ_Load_t sCodeyuv420planar2yuv422interleaved0_quad_Yuv420ToInterleaved = {
  {
    SEQ_NODE_FWLOAD,			// *** type of engine ***
    D_IPUS_SYN_BASEADDRESS + 2*ISP_IO_DEV_SPACE,		// *** address of hardware device ***
    2,			// *** index of requested engine *** (TODO resolve for -1)
    0,					// *** engine is running ***
    0,					// *** engine is done for this frame ***
    0,					// *** engine got some update ***
  },

  yuv420planar2yuv422interleaved0_quad_start,		// *** Kernel address in KMEM *** 
  yuv420planar2yuv422interleaved0_quad_end - yuv420planar2yuv422interleaved0_quad_start,	// *** Kernel length in bytes ***
  0,						// *** Kernel address in IPU instruction mem *** (ASSUMING 1 KERNEL = 1 IPU - FOR NOW!)
  0,				// *** pad alignment ***
  (SEQ_Head_Ptr_t) 20 /* Yuv420ToInterleaved.mHead */,			// *** pointer to IPU node to start ***
};


/*****************************************************************************
* exported data
*****************************************************************************/

// *** graph ***

SEQ_Head_Ptr_t gpGraph[] = {
  (SEQ_Head_Ptr_t) 388, // FastDMA
  (SEQ_Head_Ptr_t) 20, // Yuv420ToInterleaved
  (SEQ_Head_Ptr_t) 324, // sCodeyuv420planar2yuv422interleaved0_quad_Yuv420ToInterleaved
  (SEQ_Head_Ptr_t) 352, // JpegDec
  (SEQ_Head_Ptr_t) 0		// ** END ***
};

// *** Graph parsing metadata ***

static Object_t sObjectList[18] = {
  { &gpGraph,	20,	0 },
  { &Yuv420ToInterleaved,	224,	20 },
  { &Yuv420ToInterleavedPara,	80,	244 },
  { &sCodeyuv420planar2yuv422interleaved0_quad_Yuv420ToInterleaved,	28,	324 },
  { &JpegDec,	36,	352 },
  { &FastDMA,	208,	388 },
  { &JpegDec_Buffer_1,	216,	596 },
  { &JpegDec_Buffer_2,	216,	812 },
  { &JpegDec_Buffer_0,	216,	1028 },
  { &Yuv420ToInterleaved_Buffer_0,	216,	1244 },
  { &Yuv420ToInterleaved_Buffer_1,	216,	1460 },
  { &Yuv420ToInterleaved_Buffer_2,	216,	1676 },
  { &Yuv420ToInterleaved_Buffer_3,	216,	1892 },
  { &JpegDec_Buffer_3,	216,	2108 },
  { &FastDMA_Buffer_0,	216,	2324 },
  { &FastDMA_Buffer_1,	216,	2540 },
  { &FastDMA_Buffer_2,	216,	2756 },
  { &FastDMA_Buffer_3,	216,	2972 },
};

static uint32_t sPointerList[] = {
  0, /* gpGraph: pointer to FastDMA */
  4, /* gpGraph: pointer to Yuv420ToInterleaved */
  8, /* gpGraph: pointer to sCodeyuv420planar2yuv422interleaved0_quad_Yuv420ToInterleaved */
  12, /* gpGraph: pointer to JpegDec */
  44, /* Yuv420ToInterleaved: pointer to OUTput buffer */
  56, /* Yuv420ToInterleaved: pointer to OUTput buffer */
  68, /* Yuv420ToInterleaved: pointer to OUTput buffer */
  80, /* Yuv420ToInterleaved: pointer to OUTput buffer */
  92, /* Yuv420ToInterleaved: pointer to INput buffer */
  108, /* Yuv420ToInterleaved: pointer to INput buffer */
  124, /* Yuv420ToInterleaved: pointer to INput buffer */
  236, /* Yuv420ToInterleaved: pointer to config parameters */
  348, /* sCodeyuv420planar2yuv422interleaved0_quad_Yuv420ToInterleaved: pointer to Yuv420ToInterleaved.mHead */
  368, /* JpegDec: pointer to JPEG OUTput buffer */
  372, /* JpegDec: pointer to JPEG OUTput buffer */
  376, /* JpegDec: pointer to JPEG OUTput buffer */
  380, /* JpegDec: pointer to JPEG OUTput buffer */
  608, /* JpegDec_Buffer_1: pointer to producer */
  620, /* JpegDec_Buffer_1: pointer to consumer */
  824, /* JpegDec_Buffer_2: pointer to producer */
  836, /* JpegDec_Buffer_2: pointer to consumer */
  1040, /* JpegDec_Buffer_0: pointer to producer */
  1052, /* JpegDec_Buffer_0: pointer to consumer */
  1256, /* Yuv420ToInterleaved_Buffer_0: pointer to producer */
  1268, /* Yuv420ToInterleaved_Buffer_0: pointer to consumer */
  1472, /* Yuv420ToInterleaved_Buffer_1: pointer to producer */
  1484, /* Yuv420ToInterleaved_Buffer_1: pointer to consumer */
  1688, /* Yuv420ToInterleaved_Buffer_2: pointer to producer */
  1700, /* Yuv420ToInterleaved_Buffer_2: pointer to consumer */
  1904, /* Yuv420ToInterleaved_Buffer_3: pointer to producer */
  1916, /* Yuv420ToInterleaved_Buffer_3: pointer to consumer */
  2120, /* JpegDec_Buffer_3: pointer to producer */
  2336, /* FastDMA_Buffer_0: pointer to producer */
  2552, /* FastDMA_Buffer_1: pointer to producer */
  2768, /* FastDMA_Buffer_2: pointer to producer */
  2984, /* FastDMA_Buffer_3: pointer to producer */
};

static uint32_t sBufferList[] = {
  596, /* JpegDec_Buffer_1 */
  812, /* JpegDec_Buffer_2 */
  1028, /* JpegDec_Buffer_0 */
  1244, /* Yuv420ToInterleaved_Buffer_0 */
  1460, /* Yuv420ToInterleaved_Buffer_1 */
  1676, /* Yuv420ToInterleaved_Buffer_2 */
  1892, /* Yuv420ToInterleaved_Buffer_3 */
  2108, /* JpegDec_Buffer_3 */
  2324, /* FastDMA_Buffer_0 */
  2540, /* FastDMA_Buffer_1 */
  2756, /* FastDMA_Buffer_2 */
  2972, /* FastDMA_Buffer_3 */
};

static const char* scppFdmaChannelNames[] = {
  "UYVY_0",
  "UYVY_1",
  "UYVY_2",
  "UYVY_3",
};

static const char* scppBufferNames[] = {
  "JpegDec_Buffer_1",
  "JpegDec_Buffer_2",
  "JpegDec_Buffer_0",
  "Yuv420ToInterleaved_Buffer_0",
  "Yuv420ToInterleaved_Buffer_1",
  "Yuv420ToInterleaved_Buffer_2",
  "Yuv420ToInterleaved_Buffer_3",
  "JpegDec_Buffer_3",
  "FastDMA_Buffer_0",
  "FastDMA_Buffer_1",
  "FastDMA_Buffer_2",
  "FastDMA_Buffer_3",
};

GraphMetadata_t gGraphMetadata = {
  8,		// *** max number of buffer consumers
  3188,		// *** graph size [bytes]
  (const Object_t*) &sObjectList,	// *** object list pointer
  18,		// *** object count
  (const uint32_t*) &sPointerList,	// *** pointer list pointer
  36,		// *** pointer count
  (const uint32_t*) &sBufferList,	// *** buffer list pointer
  12,		// *** buffer count
  {
    0
  },
  { // MIPI-CSI metadata
    { // node not used
      CSI_NONE, 0, {0}, {0}
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
