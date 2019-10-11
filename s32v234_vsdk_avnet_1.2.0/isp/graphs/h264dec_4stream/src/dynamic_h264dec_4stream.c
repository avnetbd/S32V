/*****************************************************************************
 *
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2014-2016 Freescale Semiconductor
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
 * \file    dynamic_h264dec_4stream.c
 * \brief   ISP (sequencer) dynamic configuration
 * \author  Armin Strobl
 * \author  Nguyen Tri Hai
 * \version 0.1
 * \date    11-01-2017
 * \note    initial version
 ****************************************************************************/

#include "seq_static.h"

/****************************************************************************/

// *** this is a dummy StaticSequencing() for linking with dynamic sequencing ***
void StaticSequencing(void)
{
  sStaticSeq = STAT_SEQ_NONE;
  // ...
} // StaticSequencing()
