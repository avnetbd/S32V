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

/*****************************************************************************
*
* Picture segmantation demo implements modified superpixel SLIC algorithm 
* http://ivrl.epfl.ch/research/superpixels
* Modified algorithm use graysecale picture as an input instead of
* CIELAB color space used in original algorithm.
* Also k-means algorithm use just one iteration to met real time requirements.
* It produce 64 * 36 superpixels for 720p resolution.
*
****************************************************************************/

#ifndef __STANDALONE__
#include <signal.h>
#include <sys/time.h>
#endif // #ifdef __STANDALONE__
#include <string.h>

#include "config.h"

#ifdef __STANDALONE__
#include "frame_output_dcu.h"
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"
#endif // else from #ifdef __STANDALONE__

#include "isp_cam_sony.h"
#include "sdi.hpp"
#include "rgb_yuv_gs8_c.h"
#include "iofiles.h"        // *** from graph ***
#include "../include/lut.h" // gamma table

#include "vdb_log.h"
#include "iofiles.h"       // *** from graph ***
#include "rgb_yuv_gs8_c.h" // *** from graph ***

#include <common_time_measure.h>
#include <common_helpers.h>
#include <string>

#include "apex.h"
#include <umat.hpp>
#include <oal.h>
#include <APU_K_MEANS.hpp>
//#include <APU_DRAW_SEGMENTS.hpp>  // Can be used for grayscale output
#include <APU_DRAW_SEGMENTS_RGB.hpp>
#include <APU_GAUSS_5X5.hpp>

//#include <time_measure.h>

//***************************************************************************

#define EXPOSURE_ENGINE 1, 1
#define TONEHDR_ENGINE 1, 1
// from 0-256 : 0 = long exposure, 256 short exposure, 128 50/50

#define GAMMA_ENGINE 1, 3

#define DO_IPU_PROF
//#define DO_EXP_LOG

#define DDR_OUT_BUFFER_CNT 3 ///< number of DDR buffers per ISP stream

// Possible to set input resolution (must be supported by the DCU)
#define IMG_WIDTH 1280 ///< width of DDR buffer in pixels
#define IMG_HEIGHT 720 ///< height of DDR buffer in pixels
#define LUMA_WIDTH (IMG_WIDTH)
#define LUMA_HEIGHT (IMG_HEIGHT)

#define CHNL_CNT_Y io::IO_DATA_CH1
#define CHNL_CNT_YUV io::IO_DATA_CH2
#define CHNL_CNT_RGB io::IO_DATA_CH3

#define LOOP_NUM 100

uint32_t gGammaOn      = 1;
uint16_t gKneeVal      = 256;
uint32_t gFrameDoneCnt = 0;

/************************************************************************/
/** Prepare luminance seads for k-means.
  * \param m_Yvec   grayscale picture
  * \param kseedsY  luminance seeds for k-means algorithm
  * \param imWidth  image width
  * \param imHeight image height
  ************************************************************************/
void GetYSeeds(uint8_t* m_Yvec, uint16_t* kseedsY, uint16_t imWidth, uint16_t imHeight);
/************************************************************************/
/** Prepare index for indexing Yseeds using APEX.
  * \param index    index
  * \param width    kseedsY array width = no. of superpixels * 3
  * \param height   kseedsY array height = no. of superpixels * 3
  ************************************************************************/
void MakeIndex(uint16_t* index, int width, int height);
/************************************************************************/
/** Mapping seeds to array for APEX.
  * \param index    index
  * \param kseedsY  luminance seeds for k-means algorithm
  * \param AkseedsY luminance seeds mapped to array for algorithm using APEX
  * \param width    kseedsY array width = no. of superpixels * 3
  * \param height   kseedsY array height = no. of superpixels * 3
  ************************************************************************/
void PrepareSeedsForApex(uint16_t* index, uint16_t* kseedsY, uint16_t* AkseedsY, int width, int height);
/************************************************************************/
/** Configure camera.
  * 
  ************************************************************************/
void CamConfig(void);

/************************************************************************/
/** Update exposure setup.
  * 
  ************************************************************************/
void ExpControl(void);

/************************************************************************/
/** Log performance and status.
  * 
  * \param arTimeStart last interval start time
  * \param aFrmCnt     current number of frames captured
  ************************************************************************/
void LogPerformance(unsigned long& arTimeStart, uint32_t aFrmCnt);

//extern SEQ_Buf_t      producerless_buffer_1;
#ifdef __STANDALONE__
/************************************************************************/
/** Handler for ISP events.
  * 
  * \param  aEventType type of the event received from ISP
  ************************************************************************/
void SeqEventHandler(uint32_t aEventType);

/************************************************************************/
/** Init DCU mode.
  * 
  * \param  aMode       mode of DCU operation
  * 
  * \return             n/a  
  ************************************************************************/
static void DcuInit(io::FrameOutputDCU& arDCU, int aChan);

#else // #ifdef __STANDALONE__

/************************************************************************/
/** Init DCU mode.
  * 
  * \param  arDCU reference to DCU handling object
  * \param  aChan number of channels  
  ************************************************************************/
static void   DcuInit(io::FrameOutputV234Fb& arDCU, int aChan);

#endif // else from #ifdef __STANDALONE__

//***************************************************************************

#ifdef __STANDALONE__
extern "C" {
void          flush_dcache_range(void*, unsigned int);
unsigned long get_uptime_microS(void);
}
#else // #ifdef __STANDALONE__
unsigned long get_uptime_microS(void) // TODO: define
{
  struct timeval tv;

  gettimeofday(&tv, NULL);

  return 1000000 * tv.tv_sec + tv.tv_usec;
}

// *** Linux signal handling ***

/************************************************************************/
/** SIGINT handler.
    * 
    * \param  aSigNo 
    ************************************************************************/
void SigintHandler(int aSigNo);

/************************************************************************/
/** SIGINT handler.
    * 
    * \param  aSigNo 
    * 
    * \return SEQ_LIB_SUCCESS if all ok
    *         SEQ_LIB_FAILURE if failed
    ************************************************************************/
int32_t SigintSetup(void);

//***************************************************************************

static bool sStop = false; ///< to signal Ctrl+c from command line

#endif // else from #ifdef __STANDALONE__

//***************************************************************************

int main(int argc, char** argv)
{
  std::string helpMsg =
      std::string("Segments the input image grabbed by the camera.\n\tUsage: ") + COMMON_ExtractProgramName(argv[0]);
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  LIB_RESULT lRet = LIB_SUCCESS;
  LIB_RESULT lRes = LIB_SUCCESS;
  APEX_Init();

//*** Init DCU Output ***
#ifdef __STANDALONE__
  io::FrameOutputDCU lDcuOutput;
#else // #ifdef __STANDALONE__
  // setup Ctrl+C handler
  if(SigintSetup() != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to register Ctrl+C signal handler.");
    return -1;
  }

  printf("Press Ctrl+C to terminate the demo.\n");

  io::FrameOutputV234Fb lDcuOutput;
#endif

  //
  // *** Initialize SDI ***
  //
  lRes = sdi::Initialize(0);

#ifdef __STANDALONE__
  SEQ_EventHandlerSet(SeqEventHandler); // set the event handler
#endif

  // *** create grabber ***
  sdi_grabber* lpGrabber = new(sdi_grabber);
  lpGrabber->ProcessSet(gpGraph, &gGraphMetadata);

  // *** prepare IOs ***
  sdi_FdmaIO* lpFdma = (sdi_FdmaIO*)lpGrabber->IoGet(SEQ_OTHRIX_FDMA);

  // modify DDR frame geometry to fit display output
  SDI_ImageDescriptor lFrmDesc = SDI_ImageDescriptor(IMG_WIDTH, NUM_OUT_LINES, YUV422Stream_YUYV);
  lpFdma->DdrBufferDescSet(FDMA_IX_YUV_TO_DDR, lFrmDesc);
  lFrmDesc = SDI_ImageDescriptor(LUMA_WIDTH, NUM_OUT_LINES, GS8);
  lpFdma->DdrBufferDescSet(FDMA_IX_Y_TO_DDR, lFrmDesc);
  lFrmDesc = SDI_ImageDescriptor(IMG_WIDTH, NUM_OUT_LINES, RGB888);
  lpFdma->DdrBufferDescSet(FDMA_IX_RGB_TO_DDR, lFrmDesc);
  lFrmDesc = SDI_ImageDescriptor(NUM_OUT_Y_SCALED_BYTES, NUM_OUT_Y_SCALED_LINES, GS8);
  lpFdma->DdrBufferDescSet(FDMA_IX_Y_SCALED_TO_DDR, lFrmDesc);

  //*** allocate DDR buffers ***
  lpFdma->DdrBuffersAlloc(DDR_OUT_BUFFER_CNT);

  // *** prestart grabber ***
  lpGrabber->PreStart();

  // *** configure camera ***
  CamConfig();

  // fetched frame buffer storage
  SDI_Frame lFrame[2];

  // *** start grabbing ***
  lpGrabber->Start();

  using namespace icp;

  int lSrcWidth     = 1280;
  int lSrcHeight    = 720;
  int lKernelWidth  = 192; // no of superpixels * 3
  int lKernelHeight = 108; // no of superpixels * 3

  //int sz = lSrcWidth * lSrcHeight;

  int x_Superpixels = 64; // no of superpixels x axes
  int y_Superpixels = 36; // no of superpixels y axes

  int lRetVal = 0;

  // creates seeds and labels
  vsdk::UMat kseedsY(y_Superpixels, x_Superpixels, VSDK_CV_16UC1);
  vsdk::UMat AkseedsY(lKernelHeight, lKernelWidth, VSDK_CV_16UC1);
  vsdk::UMat Aindex(lKernelHeight, lKernelWidth, VSDK_CV_16UC1);
  vsdk::UMat klabels(lSrcHeight, lSrcWidth, VSDK_CV_16UC1);
  // creates pictures
  //icp::DataDescriptor outpicture(lSrcWidth, lSrcHeight, icp::DATATYPE_08U);
  vsdk::UMat filteredpicture(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);
  vsdk::UMat outpictureRGB(lSrcHeight, lSrcWidth, VSDK_CV_8UC3);

  MakeIndex((uint16_t*)Aindex.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, lKernelWidth, lKernelHeight);

  uint32_t lFrmCnt = 0;
  uint32_t lLoop   = 0;

  unsigned long lTimeStart;
  int           ApuRuntimeStart;
  int           ApuRuntimeStop;

  // *** init DCU ***
  DcuInit(lDcuOutput, FDMA_IX_RGB_TO_DDR);

  lTimeStart = get_uptime_microS();

  while(1)
  {
#ifndef __STANDALONE__
    if(sStop)
    {
      break; // break if Ctrl+C pressed
    }        // if Ctrl+C
#endif       //#ifndef __STANDALONE__

    lFrame[0] = lpGrabber->FramePop(FDMA_IX_Y_TO_DDR);
    lFrame[1] = lpGrabber->FramePop(FDMA_IX_RGB_TO_DDR);

    if(lFrame[0].mUMat.empty() || lFrame[1].mUMat.empty())
      break;

    GetYSeeds(lFrame[0].mUMat.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data,
              (uint16_t*)kseedsY.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, lSrcWidth, lSrcHeight);
    PrepareSeedsForApex((uint16_t*)Aindex.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data,
                        (uint16_t*)kseedsY.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data,
                        (uint16_t*)AkseedsY.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, lKernelWidth,
                        lKernelHeight);

    ApuRuntimeStart = FSL_Ticks();

    // gauss filtering using 5 * 5 kernel
    APU_GAUSS_5X5 gauss;
    // Init APEX
    lRetVal |= gauss.Initialize();
    lRetVal |= gauss.ConnectIO("INPUT_0", lFrame[0].mUMat);
    lRetVal |= gauss.ConnectIO("OUTPUT_0", filteredpicture);
    // execute
    lRetVal |= gauss.Start();
    // wait
    lRetVal |= gauss.Wait();
    // local k-means algorithm
    APU_K_MEANS k_means;
    // Init APEX
    lRetVal |= k_means.Initialize();
    lRetVal |= k_means.ConnectIO("INPUT_0", filteredpicture);
    lRetVal |= k_means.ConnectIO("INPUT_1", AkseedsY);
    lRetVal |= k_means.ConnectIO("INPUT_2", Aindex);
    lRetVal |= k_means.ConnectIO("OUTPUT_0", klabels);
    // execute
    lRetVal |= k_means.Start();
    // wait
    lRetVal |= k_means.Wait();

    // draw segments
    APU_DRAW_SEGMENTS_RGB drawRGB;

    lRetVal |= drawRGB.Initialize();
    lRetVal |= drawRGB.ConnectIO("INPUT_0", klabels);
    lRetVal |= drawRGB.ConnectIO("INPUT_1", lFrame[1].mUMat);
    lRetVal |= drawRGB.ConnectIO("OUTPUT_0", outpictureRGB);
    // execute
    lRetVal |= drawRGB.Start();
    // wait
    lRetVal |= drawRGB.Wait();

    ApuRuntimeStop = FSL_Ticks();

    // *** do exposure control ***
    ExpControl();

    lFrmCnt++;
    // display the frame
    lDcuOutput.PutFrame(outpictureRGB);

    if(lpGrabber->FramePush(lFrame[0]) != LIB_SUCCESS)
    {
      break;
    } // if push failed
    if(lpGrabber->FramePush(lFrame[1]) != LIB_SUCCESS)
    {
      break;
    } // if push failed

    LogPerformance(lTimeStart, lFrmCnt); // camera performance
    printf("APEX functions took %.6f sec and %d ticks.\n", (float)FSL_TicksToSeconds(ApuRuntimeStop - ApuRuntimeStart),
           ApuRuntimeStop - ApuRuntimeStart); // APEX performance

#ifndef __STANDALONE__
    if(sStop)
    {
      break; // break if Ctrl+C pressed
    }        // if Ctrl+C
#endif       // #ifndef __STANDALONE__

  } // while(1)

  if(lpGrabber)
  {
    //*** Stop ISP processing ***
    lpGrabber->Stop();

    // clean up grabber resources
    lpGrabber->Release();

    delete(lpGrabber);
  } // if grabber exists

  //
  // Close SDI
  //

  lRes = sdi::Close(0);
  if(lRes != LIB_SUCCESS)
  {
    lRet = LIB_FAILURE;
  } // if lRes not LIB_SUCCESS
  return lRet;

} // main()

//***************************************************************************

void CamConfig()
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
  seq_setReg(EXPOSURE_ENGINE, 0, 0x77, 400); // GPR7 (IPUS): B

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
  //lGeo.mWinMode = SONY_720P;
  lGeo.mWinMode = SONY_CROP;
  //lGeo.mHeight = 729;
  //lGeo.mVmax = (lGeo.mHeight+15) | 1;
  lGeo.mWidth = 1288;
  lGeo.mHmax  = 0;
  lGeo.mOb    = 2;
  lGeo.mXoff  = 0;
  lGeo.mYoff  = 0;
  //flip picture
  lGeo.mVerFlip = 1;
  lGeo.mHorFlip = 1;
  lGeo.mFps     = 20;
  //lGeo.mExpMode =SONY_SINGLE_EXP;
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
} // CamConfig();

//***************************************************************************

void ExpControl()
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
    SONY_ExposureControlEqual(&lIpuHist, -0.1, &lExp);
    SONY_ExposureSet(CSI_IDX_0, lExp);
  } // if ready for exp update
} // ExpControl

//***************************************************************************

void LogPerformance(unsigned long& arTimeStart, uint32_t aFrmCnt)
{
  if((aFrmCnt % LOOP_NUM) != 0)
  {
    return;
  }

  unsigned long lTimeEnd, lTimeDelta;

  printf("Frame: %7d     Gamma: %d\n", aFrmCnt, gGammaOn);

  lTimeEnd    = get_uptime_microS();
  lTimeDelta  = lTimeEnd - arTimeStart;
  arTimeStart = lTimeEnd;

  printf("%d frames took %lu usec (%5.2ffps)\n", LOOP_NUM, lTimeDelta, LOOP_NUM * 1000000.0 / ((float)lTimeDelta));

#ifdef DO_EXP_LOG
  {
    SONY_Exposure_t lExp;
    SONY_ExposureGet(CSI_IDX_0, &lExp);

    printf("Exposure Lines = %3d CG= %d AG=%2ddB\n", lGeo.mVmax - lExp.mExposureLines - 1, lExp.mConversionGain,
           lExp.mAnalogGain / 10 /*sGain * 6*/
    );
  }
#endif
} // LogPerformace()

//***************************************************************************

void SeqEventHandler(uint32_t aEventType)
{
  switch(aEventType)
  {
    case SEQ_MSG_TYPE_FRAMEDONE:;
      //gFrameDoneCnt++;
      break;
    case SEQ_MSG_TYPE_OTHERERROR:
      char lString[SEQ_PRAM_AUX_DATA_SIZE] = {0};

      printf("*** Sequencer-Error: ");

      if(SEQ_MsgAuxDataGet(lString, SEQ_PRAM_AUX_DATA_SIZE) != SEQ_LIB_SUCCESS)
      {
        VDB_LOG_NOTE("Failed to get the auxiliary data from PRAM.\n");
      }
      printf("%s\n", lString);
      break;
  }

} // SeqEventHandler(uint32_t aEventType)

  //***************************************************************************

#ifdef __STANDALONE__
static void DcuInit(io::FrameOutputDCU& arDCU, int aChan)
{
  switch(aChan)
  {
    case FDMA_IX_Y_TO_DDR:
      printf("*** Mode: Y ***\n");
      arDCU.Init(LUMA_WIDTH, LUMA_HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT_Y, DCU_BPP_8);
      break;

    case FDMA_IX_YUV_TO_DDR:
      printf("*** Mode: YUV ***\n");
      arDCU.Init(IMG_WIDTH, IMG_HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT_YUV, DCU_BPP_YCbCr422);
      break;

    case FDMA_IX_RGB_TO_DDR:
      printf("*** Mode: RGB ***\n");
      arDCU.Init(IMG_WIDTH, IMG_HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT_RGB);
      break;

    case FDMA_IX_Y_SCALED_TO_DDR:
      printf("*** Mode: Y Scaled ***\n");
      arDCU.Init(NUM_OUT_Y_SCALED_BYTES, NUM_OUT_Y_SCALED_LINES, io::IO_DATA_DEPTH_08, CHNL_CNT_Y, DCU_BPP_8);
      break;

    default:
    {
      printf("Output format not supported!\n");
    } // default

  } // switch()
} // DcuInit()

#else  //#ifdef __STANDALONE__

//****************************************************************************

static void DcuInit(io::FrameOutputV234Fb& arDCU, int32_t aChan)
{
  switch(aChan)
  {
    case FDMA_IX_Y_TO_DDR:
      printf("*** Mode: Y ***\n");
      /*arDCU.Init(
        LUMA_WIDTH,
        LUMA_HEIGHT,
        io::IO_DATA_DEPTH_08, 
        CHNL_CNT_Y,
        DCU_BPP_8);
      break;*/

    case FDMA_IX_YUV_TO_DDR:
      printf("*** Mode: YUV ***\n");
      arDCU.Init(IMG_WIDTH, IMG_HEIGHT, io::IO_DATA_DEPTH_08, io::IO_DATA_CH3, DCU_BPP_YCbCr422);
      break;

    case FDMA_IX_RGB_TO_DDR:
      printf("*** Mode: RGB ***\n");
      arDCU.Init(IMG_WIDTH, IMG_HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT_RGB);
      break;

    case FDMA_IX_Y_SCALED_TO_DDR:
      printf("*** Mode: Y Scaled ***\n");
      /*arDCU.Init(
        NUM_OUT_Y_SCALED_BYTES,
        NUM_OUT_Y_SCALED_LINES,
        io::IO_DATA_DEPTH_08, 
        CHNL_CNT_Y,
        DCU_BPP_8);
      break;*/

    default:
    {
      printf("Output format not supported!\n");
    } // default

  } // switch()
} // DcuInit()

//***************************************************************************

void SigintHandler(int)
{
  sStop = true;
} // SigintHandler()

//***************************************************************************

int32_t SigintSetup()
{
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
} // SigintSetup()

//***************************************************************************
#endif // #ifndef __STANDALONE__

void GetYSeeds(uint8_t* m_Yvec, uint16_t* kseedsY, uint16_t imWidth, uint16_t /*imHeight*/)
{
  int xoff = 10;
  int yoff = 10;
  int step = 20;

  for(int y = 0; y < 36; y++)
  {
    int Y = y * step + yoff;

    for(int x = 0; x < 64; x++)
    {
      //using square grid
      int X = x * step + xoff;

      int i = Y * imWidth + X;
      int I = y * 64 + x;
      //central value of the superpixel
      kseedsY[I] = m_Yvec[i];
    }
  }
} // GetYSeeds()

void MakeIndex(uint16_t* index, int width, int height)
{
  /* index required for APEX implementation
   * 
   * x = 65535
   * 
   * 1 2 3
   * 4 5 6
   * 7 8 9
   * 
   * x x x x x x x x x ...
   * x 1 2 1 2 3 2 3 x ...
   * x 4 5 4 5 6 4 6 x ...
   * x 1 2 1 2 3 2 3 x ...
   * x 4 5 4 5 6 5 6 x ...
   * x 8 9 7 8 9 8 9 x ...
   * 
   * .
   * .
   * .
   */

  int w = width;
  int h = height;

  int x = w / 3;
  int y = h / 3;

  const int dx8[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
  const int dy8[8] = {0, -1, -1, -1, 0, 1, 1, 1};

  int* data = new int[x * y];
  for(int i = 0; i < x * y; ++i)
    data[i] = i;

  for(int i = 0; i < h; ++i)
    for(int j = 0; j < w; ++j)
    {
      if(i == 0 || j == 0 || i == h - 1 || j == w - 1)
        index[i * w + j] = 65535;
      else if((i - 1) % 3 == 0 && (j - 1) % 3 == 0)
      {
        index[i * w + j] = data[i / 3 * x + j / 3];
        for(int k = 0; k < 8; ++k)
        {
          int nx = j / 3 + dx8[k]; //new x
          int ny = i / 3 + dy8[k]; //new y

          if(nx >= 0 && nx < x && ny >= 0 && ny < y)
          {
            int nind        = ny * x + nx;
            int indexind    = (i + dy8[k]) * w + j + dx8[k];
            index[indexind] = data[nind];
          }
        }
      }
    }
  delete[] data;
} // MakeIndex()

void PrepareSeedsForApex(uint16_t* index, uint16_t* kseedsY, uint16_t* AkseedsY, int width, int height)
{
  for(int i = 0; i < width * height; ++i)
  {
    // seeds outside of picture border
    if(index[i] == 65535)
    {
      AkseedsY[i] = 65535;
    }
    // seeds inside of picture border
    else
    {
      AkseedsY[i] = kseedsY[index[i]];
    }
  }
} // PrepareSeedsForApex()

/* EOF */
