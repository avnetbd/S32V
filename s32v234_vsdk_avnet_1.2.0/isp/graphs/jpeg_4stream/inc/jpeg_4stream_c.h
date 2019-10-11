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

#pragma once
#include "seq_graph.h"
#include "seq_graph_meta.h"

/*****************************************************************************
* consts
*****************************************************************************/

enum {
  FDMA_IX_UYVY_0,
  FDMA_IX_UYVY_1,
  FDMA_IX_UYVY_2,
  FDMA_IX_UYVY_3,
  FDMA_IX_END
};

/*****************************************************************************
* exported variables
*****************************************************************************/

extern SEQ_Head_Ptr_t   gpGraph[];
extern GraphMetadata_t  gGraphMetadata;

extern SEQ_Buf_t	JpegDec_Buffer_1;
extern SEQ_Buf_t	JpegDec_Buffer_2;
extern SEQ_Buf_t	JpegDec_Buffer_0;
extern SEQ_Buf_t	Yuv420ToInterleaved_Buffer_0;
extern SEQ_Buf_t	Yuv420ToInterleaved_Buffer_1;
extern SEQ_Buf_t	FastDMA_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_1;
extern SEQ_Buf_t	Yuv420ToInterleaved_Buffer_2;
extern SEQ_Buf_t	Yuv420ToInterleaved_Buffer_3;
extern SEQ_Buf_t	FastDMA_Buffer_2;
extern SEQ_Buf_t	FastDMA_Buffer_3;
extern SEQ_Buf_t	JpegDec_Buffer_3;
