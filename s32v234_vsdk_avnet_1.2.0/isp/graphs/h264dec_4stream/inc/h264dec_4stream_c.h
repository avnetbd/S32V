#pragma once
#include "seq_graph.h"
#include "seq_graph_meta.h"

/*****************************************************************************
* consts
*****************************************************************************/

enum {
  FDMA_IX_UYVY_0,
  FDMA_IX_UYVY_1,
  FDMA_IX_UYVY_3,
  FDMA_IX_UYVY_2,
  FDMA_IX_END
};

/*****************************************************************************
* exported variables
*****************************************************************************/

extern SEQ_Head_Ptr_t   gpGraph[];
extern GraphMetadata_t  gGraphMetadata;

extern SEQ_Buf_t	Yuv420planarToYuv422_q0_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_0;
extern SEQ_Buf_t	H264_Decoder_Buffer_0;
extern SEQ_Buf_t	H264_Decoder_Buffer_1;
extern SEQ_Buf_t	H264_Decoder_Buffer_2;
extern SEQ_Buf_t	H264_Decoder_Buffer_3;
extern SEQ_Buf_t	H264_Decoder_Buffer_4;
extern SEQ_Buf_t	H264_Decoder_Buffer_5;
extern SEQ_Buf_t	Yuv420planarToYuv422_q1_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_1;
extern SEQ_Buf_t	H264_Decoder_Buffer_6;
extern SEQ_Buf_t	H264_Decoder_Buffer_7;
extern SEQ_Buf_t	H264_Decoder_Buffer_8;
extern SEQ_Buf_t	H264_Decoder_Buffer_9;
extern SEQ_Buf_t	H264_Decoder_Buffer_10;
extern SEQ_Buf_t	H264_Decoder_Buffer_11;
extern SEQ_Buf_t	Yuv420planarToYuv422_q3_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_2;
extern SEQ_Buf_t	Yuv420planarToYuv422_q2_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_3;

