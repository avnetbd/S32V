#pragma once
#include "seq_graph.h"
#include "seq_graph_meta.h"

/*****************************************************************************
* consts
*****************************************************************************/

enum {
  FDMA_IX_ISP_OUTPUT0,
  FDMA_IX_ISP_OUTPUT1,
  FDMA_IX_END
};

/*****************************************************************************
* exported variables
*****************************************************************************/

extern SEQ_Head_Ptr_t   gpGraph[];
extern GraphMetadata_t  gGraphMetadata;

extern SEQ_Buf_t	OV_QUAD_Buffer_0;
extern SEQ_Buf_t	COMPRESS0_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_0;
extern SEQ_Buf_t	Debayer0_Buffer_0;
extern SEQ_Buf_t	Debayer0_Buffer_2;
extern SEQ_Buf_t	Debayer0_Buffer_1;
extern SEQ_Buf_t	Debayer1_Buffer_0;
extern SEQ_Buf_t	Debayer1_Buffer_2;
extern SEQ_Buf_t	Debayer1_Buffer_1;
extern SEQ_Buf_t	COMPRESS1_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_1;
extern SEQ_Buf_t	RGB2Y0_Buffer_0;
extern SEQ_Buf_t	LUT4K0_Buffer_0;
extern SEQ_Buf_t	RGB2Y1_Buffer_0;
extern SEQ_Buf_t	LUT4K1_Buffer_0;

