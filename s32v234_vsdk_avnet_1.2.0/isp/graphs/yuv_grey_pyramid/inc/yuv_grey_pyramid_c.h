#pragma once
#include "seq_graph.h"
#include "seq_graph_meta.h"

/*****************************************************************************
* consts
*****************************************************************************/

enum {
  FDMA_IX_YUV_TO_DDR,
  FDMA_IX_Y2_SCALED_TO_DDR,
  FDMA_IX_Y4_SCALED_TO_DDR,
  FDMA_IX_Y8_SCALED_TO_DDR,
  FDMA_IX_Y16_SCALED_TO_DDR,
  FDMA_IX_Y32_SCALED_TO_DDR,
  FDMA_IX_END
};

/*****************************************************************************
* exported variables
*****************************************************************************/

extern SEQ_Head_Ptr_t   gpGraph[];
extern GraphMetadata_t  gGraphMetadata;

extern SEQ_Buf_t	ConvertYUV_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_0;
extern SEQ_Buf_t	ChGain_Exposure_Buffer_0;
extern SEQ_Buf_t	Debayer_G_Buffer_0;
extern SEQ_Buf_t	Debayer_G_Buffer_1;
extern SEQ_Buf_t	Debayer_BR_Buffer_2;
extern SEQ_Buf_t	Debayer_BR_Buffer_1;
extern SEQ_Buf_t	Debayer_BR_Buffer_0;
extern SEQ_Buf_t	RGB_2_YUV_Buffer_0;
extern SEQ_Buf_t	RGB_2_YUV_Buffer_1;
extern SEQ_Buf_t	RGB_2_YUV_Buffer_2;
extern SEQ_Buf_t	CAM0_Buffer_0;
extern SEQ_Buf_t	Y2_DOWNSCALE_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_1;
extern SEQ_Buf_t	Y4_DOWNSCALE_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_2;
extern SEQ_Buf_t	Y8_DOWNSCALE_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_3;
extern SEQ_Buf_t	Y16_DOWNSCALE_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_4;
extern SEQ_Buf_t	Y32_DOWNSCALE_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_5;

