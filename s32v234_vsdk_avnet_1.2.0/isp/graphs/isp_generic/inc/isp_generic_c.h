#pragma once
#include "seq_graph.h"
#include "seq_graph_meta.h"

/*****************************************************************************
* consts
*****************************************************************************/

enum {
  FDMA_IX_ISP_OUTPUT,
  FDMA_IX_END
};

/*****************************************************************************
* exported variables
*****************************************************************************/

extern SEQ_Head_Ptr_t   gpGraph[];
extern GraphMetadata_t  gGraphMetadata;

extern SEQ_Buf_t	OutputConversion_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_0;
extern SEQ_Buf_t	MIPI0_Buffer_0;
extern SEQ_Buf_t	RGB2YUV_Buffer_0;
extern SEQ_Buf_t	RGB2YUV_Buffer_1;
extern SEQ_Buf_t	RGB2YUV_Buffer_2;
extern SEQ_Buf_t	ISP_Buffer_0;
extern SEQ_Buf_t	ISP_Buffer_1;
extern SEQ_Buf_t	ISP_Buffer_2;
extern SEQ_Buf_t	Generic3x3_Buffer_0;

