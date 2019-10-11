#pragma once
#include "seq_graph.h"
#include "seq_graph_meta.h"

/*****************************************************************************
* consts
*****************************************************************************/

enum {
  FDMA_IX_FastDMA_Out_MIPI_SIMPLE,
  FDMA_IX_END_MIPI_SIMPLE
};

/*****************************************************************************
* exported variables
*****************************************************************************/
extern char sequencer_mipi_simple_srec[];
extern char kmem_mipi_simple_srec[];
extern SEQ_Head_Ptr_t   gpGraph_mipi_simple[];
extern GraphMetadata_t  gGraphMetadata_mipi_simple;

extern SEQ_Buf_t	Debayer_RGB_Buffer_0_mipi_simple;
extern SEQ_Buf_t	FastDMA_Buffer_0_mipi_simple;
extern SEQ_Buf_t	MIPI_Buffer_0_mipi_simple;

