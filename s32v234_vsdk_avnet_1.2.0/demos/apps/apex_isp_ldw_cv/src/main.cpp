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

#include "webserver.hpp"
#ifndef __STANDALONE__
#include "utils_h264encoder.h"
#include <boost/lexical_cast.hpp>
#include <signal.h>
#endif // #ifdef __STANDALONE__
#include <config_ldw.h>
#include <visualisation.h>

#include "apex.h"

#ifdef __STANDALONE__
#include "common_time_measure.h"
#include "frame_output_dcu.h"
#define CHNL_CNT io::IO_DATA_CH3
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"
#define CHNL_CNT io::IO_DATA_CH3
#endif // else from #ifdef __STANDALONE__

#include "isp_cam_sony.h"
#include "sdi.hpp"
#include "imx224_exp_ctrl_c.h"
#include "iofiles.h"
#include "../include/lut.h" // gamma table
#include <common_helpers.h>
#include <string>

#include "vdb_log.h"
#define STREAM_WIDTH 640
#define STREAM_HEIGHT 384
#define STREAM_LINES_NUMBER 128

//***************************************************************************

#define FRM_TIME_MSR 30 ///< number of frames to measure the time and fps

//***************************************************************************

inline void neon_memcpy_rotate_1280(char* dst, char* src)
{
  char* dst_local         = dst; //+ (180 * 1920 * 3) + 320 * 3;
  char* src_local         = src + 1280 * 720 * 3 - 24;
  long  line_size         = (1280 * 3) / 24;
  long  line_iterations   = line_size;
  long  height_iterations = 720;
  long  stride_dst        = 0; //640 * 3;

  __asm volatile(
      "1: \n\t"
      "PRFM PLDL1KEEP, [%[src_local], #-24] \n\t"
      "LD3 {V0.8B, V1.8B, V2.8B}, [%[src_local]]	\n\t"
      "subs %[src_local],%[src_local],#24				\n\t"
      "ST3 {V0.B, V1.B, V2.B}[7], [%[dst_local]], #3		\n\t"
      "ST3 {V0.B, V1.B, V2.B}[6], [%[dst_local]], #3		\n\t"
      "ST3 {V0.B, V1.B, V2.B}[5], [%[dst_local]], #3		\n\t"
      "ST3 {V0.B, V1.B, V2.B}[4], [%[dst_local]], #3		\n\t"
      "ST3 {V0.B, V1.B, V2.B}[3], [%[dst_local]], #3		\n\t"
      "ST3 {V0.B, V1.B, V2.B}[2], [%[dst_local]], #3		\n\t"
      "ST3 {V0.B, V1.B, V2.B}[1], [%[dst_local]], #3		\n\t"
      "ST3 {V0.B, V1.B, V2.B}[0], [%[dst_local]], #3		\n\t"

      //"DSB SY\n\t"
      "subs %[line_iterations],%[line_iterations],#1 \n\t"
      "bne 1b \n\t"

      "mov %[line_iterations], %[line_size] \n\t"
      "add %[dst_local],%[dst_local],%[stride_dst] \n\t"

      "subs %[height_iterations],%[height_iterations],#1 \n\t"
      "bne 1b \n\t"

      : [src_local] "+r"(src_local), [dst_local] "+r"(dst_local), [line_iterations] "+r"(line_iterations),
        [height_iterations] "+r"(height_iterations), [line_size] "+r"(line_size), [stride_dst] "+r"(stride_dst)
      :);
}

//***************************************************************************

// Possible to set input resolution (must be supported by the DCU)
#define WIDTH 1280       ///< width of DDR buffer in pixels
#define HEIGHT 720       ///< height of DDR buffer in pixels
#define DDR_BUFFER_CNT 3 ///< number of DDR buffers per ISP stream

//***************************************************************************

/************************************************************************/
/** Allocates contiguous DDR buffers for one ISP stream.
  * 
  * \param  appFbsVirtual array of virtual buffer pointers to be filled
  * \param  apFbsPhysical array of buffer physical addresses to be filled
  * 
  * \return 0 if all OK
  *         < 0 otherwise
  ************************************************************************/
int32_t DdrBuffersAlloc(void** appFbsVirtual, uint32_t* apFbsPhysical);

/************************************************************************/
/** Frees DDR buffers for one ISP stream.
  * 
  * \param  appFbsVirtual array of virtual buffer pointers to be filled
  * \param  apFbsPhysical array of buffer physical addresses to be filled
  ************************************************************************/
void DdrBuffersFree(void** appFbsVirtual, uint32_t* apFbsPhysical);

#ifndef __STANDALONE__
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

#endif // #ifndef __STANDALONE__

#define DDR_BUF_SIZE 1920 * 1080 * 3

#define EXPOSURE_ENGINE 1, 1
#define TONEHDR_ENGINE 1, 1
// from 0-256 : 0 = long exposure, 256 short exposure, 128 50/50

#define GAMMA_ENGINE 1, 3

uint32_t gGammaOn = 1;
uint16_t gKneeVal = 256;
//uint32_t gFrameDoneCnt = 0;

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

//***************************************************************************

#ifndef __STANDALONE__
void onInitMethod(std::string* name, std::string* value)
{
  if((*name).compare("tran_up_left_scene_x") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_SCENE_POINT_0_0);
    return;
  }

  if((*name).compare("tran_up_left_scene_y") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_SCENE_POINT_0_1);
    return;
  }

  if((*name).compare("tran_up_right_scene_x") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_SCENE_POINT_1_0);
    return;
  }

  if((*name).compare("tran_up_right_scene_y") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_SCENE_POINT_1_1);
    return;
  }

  if((*name).compare("tran_down_left_scene_x") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_SCENE_POINT_2_0);
    return;
  }

  if((*name).compare("tran_down_left_scene_y") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_SCENE_POINT_2_1);
    return;
  }

  if((*name).compare("tran_down_right_scene_x") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_SCENE_POINT_3_0);
    return;
  }

  if((*name).compare("tran_down_right_scene_y") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_SCENE_POINT_3_1);
    return;
  }

  if((*name).compare("war_right_min") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_RIGHT_MIN);
    return;
  }

  if((*name).compare("war_right_max") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_RIGHT_MAX);
    return;
  }

  if((*name).compare("war_left_min") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_LEFT_MIN);
    return;
  }

  if((*name).compare("war_left_max") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_LEFT_MAX);
    return;
  }

  if((*name).compare("max_of_instability_counter") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_MAX_COUNTER_VAL);
    return;
  }

  if((*name).compare("priority_0_x") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_0_XCOO);
    return;
  }

  if((*name).compare("priority_0_y") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_0_XCOO_PRIORITY);
    return;
  }

  if((*name).compare("priority_1_x") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_1_XCOO);
    return;
  }

  if((*name).compare("priority_1_y") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_1_XCOO_PRIORITY);
    return;
  }

  if((*name).compare("priority_2_x") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_2_XCOO);
    return;
  }

  if((*name).compare("priority_2_y") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_2_XCOO_PRIORITY);
    return;
  }

  if((*name).compare("priority_3_x") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_3_XCOO);
    return;
  }

  if((*name).compare("priority_3_y") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_3_XCOO_PRIORITY);
    return;
  }

  if((*name).compare("priority_4_x") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_4_XCOO);
    return;
  }

  if((*name).compare("priority_4_y") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_4_XCOO_PRIORITY);
    return;
  }

  if((*name).compare("priority_5_x") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_5_XCOO);
    return;
  }

  if((*name).compare("priority_5_y") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_5_XCOO_PRIORITY);
    return;
  }

  if((*name).compare("priority_6_x") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_6_XCOO);
    return;
  }

  if((*name).compare("priority_6_y") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_6_XCOO_PRIORITY);
    return;
  }

  if((*name).compare("priority_7_x") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_7_XCOO);
    return;
  }

  if((*name).compare("priority_7_y") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_CURVE_POINT_7_XCOO_PRIORITY);
    return;
  }

  if((*name).compare("kalman_measurement_noise") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_KALMAN_MEASUREMENT_NOISE);
    return;
  }

  if((*name).compare("kalman_post_error") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_KALMAN_POST_ERROR);
    return;
  }

  if((*name).compare("kalman_noise_var") == 0)
  {
    *value = boost::lexical_cast< std::string >(CONFIG_KALMAN_PROCESS_NOISE_VAR);
    return;
  }
}

void onUpdateMethod(std::string* name, std::string* value)
{
  int         index_of_delimiter = (*name).find('_');
  std::string header             = (*name).substr(0, index_of_delimiter);

  if(header.compare("tran") == 0)
  {
    LaneDetector::update_event = bird_view;
    return;
  }

  if(header.compare("priority") == 0)
  {
    LaneDetector::update_event = priority_curve;
    return;
  }

  if(header.compare("kalman") == 0)
  {
    LaneDetector::update_event = kalman;
    return;
  }
}
#endif

MovingAvg< unsigned long > h264encoder_time(50);

void onH264encodingDone(unsigned char* result, int final_length, long encoding_time_us)
{
  //printf("%lu us\n", encoding_time_us);
  webserver_streamBuffer((char*)result, final_length, "mainCanvas,origin");
  webserver_addOutValue("h264encoder_time", h264encoder_time.GetNext(encoding_time_us));
}

int main(int argc, char* argv[])
{
  std::string helpMsg =
      std::string("Demonstrates the Lane Departure Warning application on APEX with internet interface.\n\t") +
      COMMON_ExtractProgramName(argv[0]) +
      "\n\nUsage: server <address> <port>\n\t"
      "  For IPv4:\n\t"
      "    server 0.0.0.0 7777\n\t"
      "  For IPv6:\n\t"
      "    server 0::0 7777\n\n";
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

#ifndef __STANDALONE__
  try
  {
    // Check command line arguments.
    if(argc != 3)
    {
      std::cerr << helpMsg << endl;
    }
    else
    {
      std::cout << "\nApplication starts with webserver service " << argv[1] << std::endl;
      std::cout << "Server address: " << argv[1] << std::endl;
      std::cout << "Server port: " << argv[2] << std::endl;
      std::cout << "Server is starting..." << std::endl;

      // Initialise the server.
      webserver_setup(argv[1], argv[2]);

      webserver_setOnInitEventListener(&onInitMethod);

      webserver_setOnUpdateEventListener(&onUpdateMethod);
      // Run the server until stopped.

      webserver_run();

      std::cout << "Server has started." << std::endl;
      std::cout << "Press Ctrl+C (Ctrl+Break) to exit.\n" << std::endl << std::endl;
    }
  }
  catch(std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }
#endif

#ifndef __STANDALONE__
  H264Encoder::H264Encoder_setup(STREAM_WIDTH, STREAM_HEIGHT, STREAM_LINES_NUMBER);
  H264Encoder::getInstance()->Initialize();
  H264Encoder::getInstance()->setOnEncoderDoneListener(&onH264encodingDone);
#endif

#ifndef APEX2_EMULATE
  APEX_Init();
  using namespace icp;
#endif

  LaneDetector detector(0);

  LIB_RESULT lRet = LIB_SUCCESS;
  LIB_RESULT lRes = LIB_SUCCESS;

  //*** Init DCU Output ***
#ifdef __STANDALONE__
  io::FrameOutputDCU lDcuOutput(WIDTH, HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT);
#else // #ifdef __STANDALONE__
  // setup Ctrl+C handler
  if(SigintSetup() != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to register Ctrl+C signal handler.");
    return -1;
  }

  printf("Press Ctrl+C to terminate the demo.\n");

  io::FrameOutputV234Fb lDcuOutput(WIDTH, HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT);

#endif // else from #ifdef __STANDALONE__

  // Frame buffer
  vsdk::UMat lp_buffer_umat = vsdk::UMat(1080, 1920, VSDK_CV_8UC3);

  memset(lp_buffer_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, 0, DDR_BUF_SIZE);

  //
  // *** Initialize SDI ***
  //
  lRes = sdi::Initialize(0);

  // *** create grabber ***
  sdi_grabber lGrabber;
  lGrabber.ProcessSet(gpGraph, &gGraphMetadata);

  // *** prepare IOs ***
  sdi_FdmaIO* lpFdma = (sdi_FdmaIO*)lGrabber.IoGet(SEQ_OTHRIX_FDMA);

  // modify DDR frame geometry to fit display output
  SDI_ImageDescriptor lFrmDesc = SDI_ImageDescriptor(WIDTH, HEIGHT, RGB888);
  lpFdma->DdrBufferDescSet(FDMA_IX_RGB888, lFrmDesc);
  SDI_ImageDescriptor lFrmDesc2 = SDI_ImageDescriptor(640, 480, GS8);
  lpFdma->DdrBufferDescSet(FDMA_IX_G_SUB_2, lFrmDesc2);

  //*** allocate DDR buffers ***
  lpFdma->DdrBuffersAlloc(DDR_BUFFER_CNT);

  // *** prestart grabber ***
  lGrabber.PreStart();

  // fetched frame buffer storage
  SDI_Frame lFrame;
  SDI_Frame lFrameYsub;

  // Camera Setup
  CamConfig();

  // *** start grabbing ***
  lGrabber.Start();

  uint32_t lFrmCnt = 0;

  cv::Mat rot_mat = getRotationMatrix2D(cv::Point2f(91, 160), 180, 1.0);

  // variables for measuring times
  unsigned long lTimeStart = 0, lTimeEnd = 0, lTimeDiff = 0;
  unsigned long fpsTimeStart = 0, fpsTimeEnd = 0, fpsTimeDiff = 0;
  unsigned long lStart = 0, lEnd = 0;

  GETTIME(&lTimeStart);
  GETTIME(&fpsTimeStart);

  float fps = 0;

  cv::Mat dest;
  cv::Mat h264frame = cv::Mat(STREAM_HEIGHT, STREAM_WIDTH, CV_8UC1);

  MovingAvg< unsigned long > visualization_time(50);
  MovingAvg< unsigned long > neon_memcpy_time(50);
  MovingAvg< unsigned long > dcu_time(50);
  MovingAvg< unsigned long > opencv_resize_time(50);

  for(;;)
  {
    //grab image for visualization
    lFrame = lGrabber.FramePop(FDMA_IX_RGB888);
    if(lFrame.mUMat.empty())
    {
      break;
    } // if pop failed

    //grab Y sub for analysis
    lFrameYsub = lGrabber.FramePop(FDMA_IX_G_SUB_2);
    if(lFrameYsub.mUMat.empty())
    {
      break;
    } // if pop failed

    GETTIME(&lTimeStart);

    // Analysis of actual frame
    if(!detector.GetNextFrame(lFrameYsub.mUMat))
      break;

    {
      vsdk::Mat frame = lFrame.mUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);

      GETTIME(&lStart);
      // Draw lane healty
      if(detector.m_lane_healthy)
        visualisation::drawLineHealthy(frame, &detector);

      // Draw FPS and analysis time
      visualisation::drawFPSandAnalytime(frame, fps, (float)lTimeDiff);

      // Draw Lane Quadrangle
      visualisation::drawLaneQuadrangle(frame, &detector);

      // Draw left warning
      if(detector.m_left_warning)
        visualisation::drawLeftWarning(frame);

      // Draw Right warning
      if(detector.m_right_warning)
        visualisation::drawRightWarning(frame);

      GETTIME(&lEnd);

      webserver_addOutValue("visualization_time", visualization_time.GetNext(lEnd - lStart));

      GETTIME(&lStart);
      // Rotate image for visualization

      neon_memcpy_rotate_1280((char*)lp_buffer_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data,
                              (char*)frame.data);

      GETTIME(&lEnd);
      webserver_addOutValue("neon_memcpy_time", neon_memcpy_time.GetNext(lEnd - lStart));

      GETTIME(&lStart);
      // Put frame to display
      lDcuOutput.PutFrame(lp_buffer_umat);

      GETTIME(&lEnd);

      webserver_addOutValue("dcu_time", dcu_time.GetNext(lEnd - lStart));

#ifndef __STANDALONE__
      GETTIME(&lStart);
      cv::Size s(STREAM_WIDTH, STREAM_HEIGHT);
      cv::resize((cv::Mat)frame, dest, s);
      GETTIME(&lEnd);
#endif

      webserver_addOutValue("ocv_resize_time", opencv_resize_time.GetNext(lEnd - lStart));

      if(dest.data != NULL && webserver_isRunning())
      {
        if(webserver_getStringValueByName("video_data_sel", "origin").compare("origin") == 0)
        {
        //webserver_streamBuffer((char *)dest.data, final_length, "mainCanvas,origin");
#ifndef __STANDALONE__

          H264Encoder::getInstance()->EncoderFrame((unsigned char*)h264frame.data, (unsigned char*)dest.data, false);
#endif
        }
      }
    }

    // Anylysis time end
    GETTIME(&lTimeEnd);
    lTimeDiff = lTimeEnd - lTimeStart;

    webserver_sendOutSharedValues();

    if(lGrabber.FramePush(lFrame) != LIB_SUCCESS)
    {
      break;
    } // if push failed

    if(lGrabber.FramePush(lFrameYsub) != LIB_SUCCESS)
    {
      break;
    } // if push failed

    // Exposure Control comment/uncomment if its necessary
    ExpControl();

    //Calculate FPS
    if((lFrmCnt % FRM_TIME_MSR) == 0)
      if(lFrmCnt > 0)
      {
        GETTIME(&fpsTimeEnd);
        fpsTimeDiff  = fpsTimeEnd - fpsTimeStart;
        fpsTimeStart = fpsTimeEnd;
        fps          = (FRM_TIME_MSR * 1000000.0) / ((float)fpsTimeDiff);
      } // if time should be measured

    lFrmCnt++;

#ifndef __STANDALONE__
    if(sStop)
    {
      printf("*** STOP ***\n");
      break; // break if Ctrl+C pressed
    }        // if Ctrl+C
#endif       //#ifndef __STANDALONE__
  }

  //*** Stop ISP processing ***
  lGrabber.Stop();

  // clean up grabber resources
  lGrabber.Release();

#ifdef __STANDALONE__
  for(;;)
    ; // *** don't return ***
#else
  H264Encoder::getInstance()->Close();
#endif // #ifdef __STANDALONE__

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

#ifndef __STANDALONE__
void SigintHandler(int aSigNo)
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
  lExp.mConversionGain = 0;
  lExp.mExposureLines  = 250;
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
    SONY_ExposureControlEqual(&lIpuHist, 0.0, &lExp);
    SONY_ExposureSet(CSI_IDX_0, lExp);
  } // if ready for exp update
} // ExpControl

//***************************************************************************
