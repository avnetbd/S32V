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
// Z:\newmount\vsdk_repo\s32v234_sdk\isp\graphs\ov9716_mipi_s\model\ov9716_mipi_s.isp 

#pragma once
#include "seq_graph.h"
#include "seq_graph_meta.h"

/*****************************************************************************
* consts
*****************************************************************************/
#define   MIPI_PORT  0

enum {
  FDMA_IX_CAM_OUTPUT,
  FDMA_IX_ISP_OUTPUT_UV0,
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
extern SEQ_Buf_t	HDR_Buffer_0;
extern SEQ_Buf_t	Debayer0_Buffer_0;
extern SEQ_Buf_t	Debayer0_Buffer_1;
extern SEQ_Buf_t	Debayer0_Buffer_2;
extern SEQ_Buf_t	Debayer1_Buffer_0;
extern SEQ_Buf_t	Debayer1_Buffer_1;
extern SEQ_Buf_t	Debayer1_Buffer_2;
extern SEQ_Buf_t	RGB2Y0_Buffer_0;
extern SEQ_Buf_t	RGB2Y1_Buffer_0;
extern SEQ_Buf_t	LUT4K0_Buffer_0;
extern SEQ_Buf_t	LUT4K1_Buffer_0;
extern SEQ_Buf_t	COMPRESS0_Buffer_0;
extern SEQ_Buf_t	COMPRESS0_Buffer_1;
extern SEQ_Buf_t	COMPRESS1_Buffer_0;
extern SEQ_Buf_t	COMPRESS1_Buffer_1;
extern SEQ_Buf_t	DENOISE_UV_Buffer_0;
extern SEQ_Buf_t	DENOISEY2_0_Buffer_0;
extern SEQ_Buf_t	DENOISEY2_1_Buffer_0;
extern SEQ_Buf_t	INLUT_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_0;
extern SEQ_Buf_t	FastDMA_Buffer_1;
extern SEQ_Buf_t	FastDMA_Buffer_2;
extern SEQ_Buf_t	FastDMA_Buffer_3;
