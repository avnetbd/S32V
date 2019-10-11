#pragma once
#include "seq_graph.h"
#include "seq_graph_meta.h"

/*****************************************************************************
* consts
*****************************************************************************/

enum {
  FDMA_IX_ISP_Output0,
  FDMA_IX_ISP_Output1,
  FDMA_IX_ISP_Output2,
  FDMA_IX_END
};

/*****************************************************************************
* exported variables
*****************************************************************************/

extern SEQ_Head_Ptr_t   gpGraph[];
extern GraphMetadata_t  gGraphMetadata;

extern SEQ_Buf_t	FIX_ToneMapping_Buffer_0;
extern SEQ_Buf_t	RGB2UYVY0_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_0;
extern SEQ_Buf_t	Debayer0_Buffer_0;
extern SEQ_Buf_t	Debayer0_Buffer_1;
extern SEQ_Buf_t	Debayer0_Buffer_2;
extern SEQ_Buf_t	Debayer1_Buffer_0;
extern SEQ_Buf_t	Debayer1_Buffer_1;
extern SEQ_Buf_t	Debayer1_Buffer_2;
extern SEQ_Buf_t	RGB2UYVY1_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_1;
extern SEQ_Buf_t	MIPI0_Buffer_0;
extern SEQ_Buf_t	DEBAYER2_Buffer_0;
extern SEQ_Buf_t	DEBAYER2_Buffer_1;
extern SEQ_Buf_t	DEBAYER2_Buffer_2;
extern SEQ_Buf_t	RGB2UYVY2_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_2;
extern SEQ_Buf_t	FIX_ToneMapping_Buffer_1;
extern SEQ_Buf_t	Histogram_cam0_Buffer_0;
extern SEQ_Buf_t	Histogram_cam1_Buffer_0;

