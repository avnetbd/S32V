/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
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
#include <camera_input.h>
#include "isp_cam_sony.h"

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
bool ISPCameraInputStereo::sStop = false;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
ISPCameraInputStereo::ISPCameraInputStereo()
{
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int32_t ISPCameraInputStereo::Init()
{
  // *** Initialize SDI ***
  if(sdi::Initialize(0) != LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to initialized SDI.");
    return (-1);
  } // if sdi init failed

  // *** create grabber ***
  //sdi_grabber *lpGrabber = new(sdi_grabber);  //done statically
  mGrabber.ProcessSet(gpGraph, &gGraphMetadata);

  // *** prepare IOs ***
  sdi_FdmaIO* lpFdma = (sdi_FdmaIO*)mGrabber.IoGet(SEQ_OTHRIX_FDMA);

  // *** setup DDR frames geometry to fit display output ***
  // *** RGB888 images ***
  FDMA_Tc_t           lTc; // SRAM address of the FDMA transfer needs updates
  SDI_ImageDescriptor lFrmDesc = SDI_ImageDescriptor(SCR_WIDTH, SCR_HEIGHT, RGB888);
  // combined subsampled
  lpFdma->DdrBufferDescSet(FDMA_IX_FastDMA_RgbSub_Out, lFrmDesc);
  //right
  lpFdma->DdrBufferDescSet(FDMA_IX_FastDMA_Right_Out, lFrmDesc);
  //left
  lpFdma->DdrBufferDescSet(FDMA_IX_FastDMA_Left_Out, lFrmDesc);

  // *** GS8 images ***
  // right
  lFrmDesc = SDI_ImageDescriptor(SCR_WIDTH / 2, SCR_HEIGHT / 2, GS8);
  lpFdma->DdrBufferDescSet(FDMA_IX_FastDMA_RightGs_Out, lFrmDesc);

  // left
  lpFdma->DdrBufferDescSet(FDMA_IX_FastDMA_LeftGs_Out, lFrmDesc);

  //*** allocate DDR buffers ***
  lpFdma->DdrBuffersAlloc(DDR_BUFFER_CNT);

  // *** update SRAM side of the FDMA TDs

  // RGB888
  // right: update transfer size
  lTc.mTdm.mTdIdx = FDMA_IX_FastDMA_Right_Out;
  lpFdma->TcGet(lTc);
  lTc.mTd.mLineSize = (SCR_WIDTH) * (uint32_t)io::IO_DATA_CH3;
  lpFdma->TcSet(lTc);

  // left: shift SRAM adress of the FDMA transfer and update transfer size
  lTc.mTdm.mTdIdx = FDMA_IX_FastDMA_Left_Out;
  lpFdma->TcGet(lTc);
  lTc.mTd.mSramImgStartAddr += (SRC_WIDTH) * (uint32_t)io::IO_DATA_CH3;
  lTc.mTd.mLineSize = (SCR_WIDTH) * (uint32_t)io::IO_DATA_CH3;
  lpFdma->TcSet(lTc);

  // GS8
  // right: update transfer size
  lTc.mTdm.mTdIdx = FDMA_IX_FastDMA_RightGs_Out;
  lpFdma->TcGet(lTc);
  lTc.mTd.mLineSize = (SCR_WIDTH / 2) * (uint32_t)io::IO_DATA_CH1;
  lpFdma->TcSet(lTc);

  // left: shift SRAM adress of the FDMA transfer and update transfer size
  lTc.mTdm.mTdIdx = FDMA_IX_FastDMA_LeftGs_Out;
  lpFdma->TcGet(lTc);
  lTc.mTd.mSramImgStartAddr += (SRC_WIDTH / 2) * (uint32_t)io::IO_DATA_CH1;
  lTc.mTd.mLineSize = (SCR_WIDTH / 2) * (uint32_t)io::IO_DATA_CH1;
  lpFdma->TcSet(lTc);

  // *** prestart grabber ***
  mGrabber.PreStart();

  // *** configure camera ***
  {
    SONY_Geometry_t lGeo;
    lGeo.mCsiIdx = CSI_IDX_0;
    SONY_GeometryGet(lGeo);
    lGeo.mVerFlip  = 1;
    lGeo.mHorFlip  = 1;
    lGeo.mFps      = ISP_FPS;
    lGeo.mSyncMode = SONY_MASTER;
    SONY_GeometrySet(lGeo);

    lGeo.mCsiIdx = CSI_IDX_1;
    SONY_GeometryGet(lGeo);
    lGeo.mVerFlip = 1;
    lGeo.mHorFlip = 1;
    lGeo.mFps     = ISP_FPS;
    //lGeo.mSyncMode  = SONY_MASTER;
    lGeo.mSyncMode = SONY_SLAVE;
    SONY_GeometrySet(lGeo);

    SONY_Exposure_t lExp;
    lExp.mCsiIdx = CSI_IDX_0;
    SONY_ExposureGet(lExp);
    lExp.mAnalogGain     = 0;
    lExp.mConversionGain = 0;
    lExp.mExposureLines  = 500;
    lExp.mConfigFlags    = SONY_EXP_EL | SONY_EXP_CG | SONY_EXP_AG;
    SONY_ExposureSet(lExp);

    lExp.mCsiIdx = CSI_IDX_1;
    SONY_ExposureGet(lExp);
    lExp.mAnalogGain     = 0;
    lExp.mConversionGain = 0;
    lExp.mExposureLines  = 500;
    lExp.mConfigFlags    = SONY_EXP_EL | SONY_EXP_CG | SONY_EXP_AG;
    SONY_ExposureSet(lExp);
  }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int32_t ISPCameraInputStereo::Start()
{
  //*** Start ISP processing ***
  mGrabber.Start();

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int32_t ISPCameraInputStereo::Stop()
{
  mGrabber.Stop();

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int32_t ISPCameraInputStereo::Deinit()
{
  mGrabber.Release();

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
vsdk::UMat ISPCameraInputStereo::GetFrame(uint32_t aIdx, SDI_Frame& arFrame)
{
  arFrame = mGrabber.FramePop(aIdx);
  return arFrame.mUMat;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void ISPCameraInputStereo::PutFrame(SDI_Frame& arFrame)
{
  mGrabber.FramePush(arFrame);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void ISPCameraInputStereo::SigintHandler(int)
{
#ifndef __STANDALONE__
  sStop = true;
#endif
} // SigintHandler()

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int32_t ISPCameraInputStereo::SigintSetup()
{
#ifndef __STANDALONE__
  int32_t lRet = SEQ_LIB_SUCCESS;

  // prepare internal signal handler
  struct sigaction lSa;
  memset(&lSa, 0, sizeof(lSa));
  lSa.sa_handler = SigintHandler;

  if(sigaction(SIGINT, &lSa, NULL) != 0)
  {
    VDB_LOG_ERROR("Failed to register signal handler.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if signal not registered

  return lRet;
#else
  return 0;
#endif
} // SigintSetup()