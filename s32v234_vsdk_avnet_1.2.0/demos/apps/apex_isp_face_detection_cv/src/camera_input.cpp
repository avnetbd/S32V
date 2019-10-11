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

#include "../include/lut.h" // gamma table

uint32_t gGammaOn = 1;
uint16_t gKneeVal = 256;

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
  //FDMA_Tc_t lTc; // SRAM address of the FDMA transfer needs updates
  SDI_ImageDescriptor lFrmDesc = SDI_ImageDescriptor(SCR_WIDTH, SCR_HEIGHT, RGB888);
  // combined subsampled
  lpFdma->DdrBufferDescSet(FDMA_IX_RGB888, lFrmDesc);
  SDI_ImageDescriptor lFrmDesc2 = SDI_ImageDescriptor(640, 480, GS8);
  lpFdma->DdrBufferDescSet(FDMA_IX_G_SUB_2, lFrmDesc2);

  //*** allocate DDR buffers ***
  lpFdma->DdrBuffersAlloc(DDR_BUFFER_CNT);

  // *** prestart grabber ***
  mGrabber.PreStart();

  // *** configure camera ***
  {
    // *** exposure kernel ***
    // black level correction
    seq_setReg(EXPOSURE_ENGINE, 0, 0x70, 0xf08); // GPR0 (IPUS)
    seq_setReg(EXPOSURE_ENGINE, 0, 0x71, 0xf08); // GPR1 (IPUS)
    seq_setReg(EXPOSURE_ENGINE, 0, 0x72, 0xf08); // GPR2 (IPUS)
    seq_setReg(EXPOSURE_ENGINE, 0, 0x73, 0xf08); // GPR3 (IPUS)
    // channel gain
    seq_setReg(EXPOSURE_ENGINE, 0, 0x74, 450); // GPR4 (IPUS): R
    seq_setReg(EXPOSURE_ENGINE, 0, 0x75, 256); // GPR5 (IPUS): GR
    seq_setReg(EXPOSURE_ENGINE, 0, 0x76, 256); // GPR6 (IPUS): GB
    seq_setReg(EXPOSURE_ENGINE, 0, 0x77, 450); // GPR7 (IPUS): B

    // *** tonemap kernel ***
    seq_setReg(TONEHDR_ENGINE, 0, 0x78, gKneeVal); // GPR8 (IPUV)

    // *** gamma kernel ***
#define GAMMA_LUT_OFFSET 0
    seq_setReg(GAMMA_ENGINE, 0, 0x71, GAMMA_LUT_OFFSET);          // GPR1 (IPUS)
    seq_setReg(GAMMA_ENGINE, 1, 0x78, ((GAMMA_LUT_OFFSET) >> 1)); // LUTA (IPUS)
    {
      uint16_t lutcnt = 0;
      while(lutcnt < 2048)
      {
        uint32_t val;
        val = lut[lutcnt] | ((long)lut[lutcnt] << 16);
        lutcnt++;
        seq_setReg(GAMMA_ENGINE, 1, 0x7C, val); // LUTD (IPUS)
      }
    }
    seq_setReg(GAMMA_ENGINE, 0, 0x70, gGammaOn); // GPR0 (IPUS)

    // Camera Configuration
    // modify calmera geometry !!! setup before setting up expsoure control
    SONY_Geometry_t lGeo;
    lGeo.mCsiIdx = CSI_IDX_0;
    SONY_GeometryGet(lGeo);
    lGeo.mWinMode = SONY_CROP;
    //lGeo.mHeight = 729;
    lGeo.mWidth   = 1288;
    lGeo.mHmax    = 0;
    lGeo.mOb      = 2;
    lGeo.mXoff    = 0;
    lGeo.mYoff    = 0;
    lGeo.mVerFlip = 1;
    lGeo.mHorFlip = 1;
    lGeo.mFps     = 30;

    SONY_GeometrySet(lGeo);

    // modify camera exposure
    SONY_Exposure_t lExp;
    lExp.mCsiIdx = CSI_IDX_0;
    SONY_ExposureGet(lExp);
    lExp.mConversionGain = 1;
    lExp.mExposureLines  = 500;
    lExp.mAnalogGain     = 0;
    lExp.mConfigFlags    = SONY_EXP_EL | SONY_EXP_CG | SONY_EXP_AG;
    SONY_ExposureSet(lExp);

    printf("VMAX = %4d, Lines = %3d, OB = %2d\n", lGeo.mVmax, lGeo.mHeight, lGeo.mOb);

    // *** enable IPU histogram readout ***
    SEQ_HistogramEnable(SEQ_IPUS1);

    // modify camera geometry
    lGeo.mCsiIdx = CSI_IDX_0;
    SONY_GeometryGet(lGeo);
    lGeo.mVerFlip = 1;
    lGeo.mHorFlip = 0;
    SONY_GeometrySet(lGeo);
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

void ISPCameraInputStereo::ExpControl()
{
  SONY_ExpState_t lExpState = SONY_EXP_SENT;
  SONY_ExposureStatusGet(CSI_IDX_0, lExpState);
  if(lExpState == SONY_EXP_READY)
  {
    static SEQ_IpuHist_t lIpuHist;
    uint32_t             lExp = 0;
    memset(&lIpuHist, 0, sizeof(SEQ_IpuHist_t));
    lIpuHist.mIpuIdx = SEQ_IPUS1;

    // do expsoure control
    SEQ_HistogramGet(&lIpuHist);
    SONY_ExposureControlEqual(&lIpuHist, 0.0, &lExp);
    SONY_ExposureSet(CSI_IDX_0, lExp);
  } // if ready for exp update
} // ExpControl

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