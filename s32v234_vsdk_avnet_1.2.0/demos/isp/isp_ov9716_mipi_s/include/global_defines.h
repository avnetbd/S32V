/*****************************************************************************
*
* freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
* Copyright 2017-2018 NXP
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
#ifndef GLOBAL_DEFINES_H
#define GLOBAL_DEFINES_H

#include "ov9716_graph_geometries.h"

// **************************************************************************
// constants
// **************************************************************************

#define IPUVTYPE              0
#define IPUSTYPE              1

#define IPUS_GPR0             0x70
#define IPUV_GPR0             0x50

#define EXPHIST_ENGINE 0
#if (EXPHIST_ENGINE==0)
#define SEQ_EXP_HIST          SEQ_IPUS0
#elif (EXPHIST_ENGINE==1)
#define SEQ_EXP_HIST          SEQ_IPUS1
#endif
#define EXPOSURE_ENGINE       IPUSTYPE,EXPHIST_ENGINE

#define HDR_BLEND
#if defined(HDR_BLEND)
#define HDR_IPUSIDX           3
#define HDR_ENGINE            IPUVTYPE,HDR_IPUSIDX
#define HDR_GPR0              IPUV_GPR0
#else
#define HDR_IPUSIDX           7
#define HDR_ENGINE            IPUSTYPE,HDR_IPUSIDX
#define HDR_GPR0              IPUS_GPR0
#endif

#define DEBAYER_ENGINE0       IPUSTYPE,1
#define DEBAYER_ENGINE1       IPUSTYPE,4

#define RGB2Y_ENGINE0         IPUVTYPE,0
#define RGB2Y_ENGINE1         IPUVTYPE,3

#define INLUT_IPUS            2
#define INLUT_ENGINE          IPUSTYPE,INLUT_IPUS
#define IPUS_INLUT            IPUS_2

#define OUTLUT_IPUS0          3
#define OUTLUT_IPUS1          3
#define OUTLUT_ENGINE0        IPUSTYPE,OUTLUT_IPUS0
#define OUTLUT_ENGINE1        IPUSTYPE,OUTLUT_IPUS1
#if (OUTLUT_IPUS0==2)
#define IPUS_OUTLUT0          IPUS_2
#else
#define IPUS_OUTLUT0          IPUS_3
#endif
#if (OUTLUT_IPUS1==2)
#define IPUS_OUTLUT1          IPUS_2
#define SEQ_LUT_HIST          SEQ_IPUS2
#else
#define IPUS_OUTLUT1          IPUS_3
#define SEQ_LUT_HIST          SEQ_IPUS3
#endif
#define OUTLUT_BITS           11 // bits for 2048 lutsize
#define OUTLUT_SIZE           (1<<OUTLUT_BITS) // 2048
#define OUTLUT_SCALE          256.0

#define TONEMAP_ENGINE0       IPUVTYPE,1
#define TONEMAP_ENGINE1       IPUVTYPE,2

#define DENOISEY_TYPE         IPUSTYPE
#if (DENOISEY_TYPE==IPUVTYPE)
#define DENOISEY_IPU0         0
#define DENOISEY_IPU1         3
#define DENOISEY_ENGINE0      IPUVTYPE,DENOISEY_IPU0
#define DENOISEY_ENGINE1      IPUVTYPE,DENOISEY_IPU1
#define DENOISEY_GPR          IPUV_GPR0
#else
#define DENOISEY_IPU0         5
#define DENOISEY_IPU1         6
#define DENOISEY_ENGINE0      IPUSTYPE,DENOISEY_IPU0
#define DENOISEY_ENGINE1      IPUSTYPE,DENOISEY_IPU1
#define DENOISEY_GPR          IPUS_GPR0
#endif

#define DENOISEUV_ENGINE      IPUVTYPE,0

#define DDR_OUT_BUFFER_CNT    3   ///< number of DDR buffers per ISP stream
#define STREAM_LINES_NUMBER   16

// **************************************************************************
// macro
// **************************************************************************

#if(MIPI_PORT == 1)  // for IIC communication
#define CSI_IDX_CAM     CSI_IDX_1
#else
#define CSI_IDX_CAM     CSI_IDX_0
#endif

#if ((OV9716_QUAD_GRAPH==MIPI) || (OV9716_QUAD_GRAPH==0))
#define CAM_RegRead(a,b,c)       OV9716_RegRead(a,b,c)
#define CAM_RegWrite(a,b,c)      OV9716_RegWrite(a,b,c)
#define CAM_N_RegRead(a,b,c,d)   ((d<2)?OV9716_RegRead(a,b,c):0)
#define CAM_N_RegWrite(a,b,c,d)  ((d<2)?OV9716_RegWrite(a,b,c):0)
#define CAM_MEDIA                "MIPI"
#else
#define CAM_RegRead(a,b,c)       MAXIM_CAM_RegRead(a,b,c)
#define CAM_RegWrite(a,b,c)      MAXIM_CAM_RegWrite(a,b,c)
#define CAM_N_RegRead(a,b,c,d)   MAXIM_CAM_RegRead(a,b,c,d)
#define CAM_N_RegWrite(a,b,c,d)  MAXIM_CAM_RegWrite(a,b,c,d)
#define CAM_MEDIA                "LVDS"
#endif


#endif
