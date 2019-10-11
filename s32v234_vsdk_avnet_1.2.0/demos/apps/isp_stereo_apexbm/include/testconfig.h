/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
/**
* \file 		testconfig.h
* \brief 		test parameters
* \author 		Armin Strobl
* \version 		0.1
* \date 		03-February-2015
****************************************************************************/
#ifndef TESTCONFIG_H
#define TESTCONFIG_H

#include "../../../libs/io/framebuffer/kernel/include/vdma.h"

/*****************************************************************************
* Consts
*****************************************************************************/

#define SCR_WIDTH		(800)
#define SCR_HEIGHT		(480)
#define SCR_BYTES		(SCR_WIDTH*SCR_HEIGHT*3)

#define IMG_CAM_WIDTH		1312
#define IMG_CAM_HEIGHT		991

#define IMG_DST_G_WIDTH		1296
#define IMG_DST_G_STRIDE	1312
#define IMG_DST_G_HEIGHT	991

#define IMG_DST_BR_WIDTH	656
#define IMG_DST_BR_STRIDE	656
#define IMG_DST_BR_HEIGHT	495

#define IMG_SRC_BYTES		(IMG_SRC_WIDTH*IMG_SRC_HEIGHT)		// *** 8bit/pixel (gray) ***
#define IMG_DST_G_BYTES		(IMG_DST_G_WIDTH*IMG_DST_G_HEIGHT)	// *** 8bit/pixel (gray) ***
#define IMG_DST_BR_BYTES	(IMG_DST_BR_WIDTH*IMG_DST_BR_HEIGHT)	// *** 8bit/pixel (gray) ***

#endif /* TESTCONFIG_H */