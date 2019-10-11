#pragma once
#include "seq_graph.h"
#include "seq_graph_meta.h"

/*****************************************************************************
* consts
*****************************************************************************/

enum {
  FDMA_IX_FastDMA_Right_Out,
  FDMA_IX_FastDMA_Left_Out,
  FDMA_IX_FastDMA_RightGs_Out,
  FDMA_IX_FastDMA_LeftGs_Out,
  FDMA_IX_FastDMA_RgbSub_Out,
  FDMA_IX_END
};

/*****************************************************************************
* exported variables
*****************************************************************************/

extern SEQ_Head_Ptr_t   gpGraph[];
extern GraphMetadata_t  gGraphMetadata;

extern SEQ_Buf_t	Debayer_RGB_0_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_1;
extern SEQ_Buf_t	MIPI0_Buffer_0;
extern SEQ_Buf_t	Debayer_G_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_2;
extern SEQ_Buf_t	FastDMA_Buffer_3;
extern SEQ_Buf_t	Debayer_rgb_subsampled_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_4;

