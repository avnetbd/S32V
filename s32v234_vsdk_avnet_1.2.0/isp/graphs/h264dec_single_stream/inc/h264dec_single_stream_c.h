#pragma once
#include "seq_graph.h"
#include "seq_graph_meta.h"

/*****************************************************************************
* consts
*****************************************************************************/

enum {
  FDMA_IX_GRAY_AS_RGB,
  FDMA_IX_UYVY_0,
  FDMA_IX_UYVY_1,
  FDMA_IX_END
};

/*****************************************************************************
* exported variables
*****************************************************************************/

extern SEQ_Head_Ptr_t   gpGraph[];
extern GraphMetadata_t  gGraphMetadata;

extern SEQ_Buf_t	Grayscale2Rgb_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_0;
extern SEQ_Buf_t	Yuv420planareToYuv422_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_1;
extern SEQ_Buf_t	Yuv420planareToYuv422_Buffer_1;
extern SEQ_Buf_t	FastDMA_Buffer_2;
extern SEQ_Buf_t	H264_Decoder_Buffer_0;
extern SEQ_Buf_t	H264_Decoder_Buffer_1;
extern SEQ_Buf_t	H264_Decoder_Buffer_2;

