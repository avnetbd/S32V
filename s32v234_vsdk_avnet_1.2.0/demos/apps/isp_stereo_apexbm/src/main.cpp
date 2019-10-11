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

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <icp_data.h>
#include <apex.h>
#include <common_helpers.h>
#include <string>

#include <APU_REMAP_BM.hpp>
#include <APU_DISPARITY.hpp>

using namespace cv;
using namespace icp;

/*unsigned char pallette[64][3] = {
				{0, 0, 12},
				{0, 0, 24},
				{0, 0, 36},
				{0, 0, 48},
				{0, 0, 60},
				{0, 0, 72},
				{0, 0, 84},
				{0, 0, 96},
				{0, 0, 108},
				{0, 0, 120},
				{0, 0, 132},
				{0, 0, 144},
				{0, 0, 156},
				{0, 0, 168},
				{0, 0, 180},
				{0, 0, 192},
				{0, 0, 204},
				{0, 0, 216},
				{0, 0, 228},
				{0, 0, 240},
				{0, 0, 252},
				{0, 12, 0},
				{0, 24, 0},
				{0, 36, 0},
				{0, 48, 0},
				{0, 60, 0},
				{0, 72, 0},
				{0, 84, 0},
				{0, 96, 0},
				{0, 108, 0},
				{0, 120, 0},
				{0, 132, 0},
				{0, 144, 0},
				{0, 156, 0},
				{0, 168, 0},
				{0, 180, 0},
				{0, 192, 0},
				{0, 204, 0},
				{0, 216, 0},
				{0, 228, 0},
				{0, 240, 0},
				{0, 252, 0},
				{12, 0, 0},
				{24, 0, 0},
				{36, 0, 0},
				{48, 0, 0},
				{60, 0, 0},
				{72, 0, 0},
				{84, 0, 0},
				{96, 0, 0},
				{108, 0, 0},
				{120, 0, 0},
				{132, 0, 0},
				{144, 0, 0},
				{156, 0, 0},
				{168, 0, 0},
				{180, 0, 0},
				{192, 0, 0},
				{204, 0, 0},
				{216, 0, 0},
				{228, 0, 0},
				{240, 0, 0},
				{252, 0, 0},
				{0, 0, 0}
			      };*/

unsigned char pallette[64][3] = {{21, 0, 0},
                                 {42, 0, 0},
                                 {63, 0, 0},
                                 {84, 0, 0},
                                 {105, 0, 0},
                                 {126, 0, 0},
                                 {147, 0, 0},
                                 {168, 0, 0},
                                 {189, 0, 0},
                                 {210, 0, 0},
                                 /* 11 */ {231, 21, 0},
                                 {255, 42, 0},
                                 {255, 63, 0},
                                 {255, 84, 0},
                                 {255, 105, 0},
                                 {255, 126, 0},
                                 {255, 147, 0},
                                 {255, 168, 0},
                                 {255, 189, 0},
                                 {255, 210, 0},
                                 {255, 231, 0},
                                 /* 22 */ {255, 255, 0},
                                 {231, 255, 0},
                                 {210, 255, 0},
                                 {189, 255, 0},
                                 {168, 255, 0},
                                 {147, 255, 0},
                                 {126, 255, 0},
                                 {105, 255, 0},
                                 {84, 255, 0},
                                 {63, 255, 0},
                                 {42, 255, 0},
                                 /* 33 */ {21, 255, 21},
                                 {0, 255, 42},
                                 {0, 255, 63},
                                 {0, 255, 84},
                                 {0, 255, 105},
                                 {0, 255, 126},
                                 {0, 255, 147},
                                 {0, 255, 168},
                                 {0, 255, 189},
                                 {0, 255, 210},
                                 {0, 255, 231},
                                 /* 44 */ {0, 255, 255},
                                 {0, 231, 255},
                                 {0, 210, 255},
                                 {0, 189, 255},
                                 {0, 168, 255},
                                 {0, 147, 255},
                                 {0, 126, 255},
                                 {0, 105, 255},
                                 {0, 84, 255},
                                 {0, 63, 255},
                                 {0, 42, 255},
                                 /* 55 */ {0, 21, 255},
                                 {0, 0, 231},
                                 {0, 0, 210},
                                 {0, 0, 189},
                                 {0, 0, 168},
                                 {0, 0, 147},
                                 {0, 0, 126},
                                 {0, 0, 105},
                                 {0, 0, 84},
                                 {0, 0, 63}};

#include <common_time_measure.h>

#ifdef __STANDALONE__
#include "frame_output_dcu.h"
#define CHNL_CNT io::IO_DATA_CH3
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"
#define CHNL_CNT io::IO_DATA_CH3
#endif // else from #ifdef __STANDALONE__

#include "camera_input.h"

const int TRANSFER_TILE_SIZE_X = 20;
const int TRANSFER_TILE_SIZE_Y = 4;

#define IMG_W 640
#define IMG_H 360
int main(int argc, char** argv)
{
  std::string helpMsg =
      std::string("Demonstrates the disparity computation on APEX with camera input and ISP .\n\tUsage: ") +
      COMMON_ExtractProgramName(argv[0]);
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  APEX_Init();
  // Frame buffer pointers
  vsdk::UMat lpFrameRight;
  vsdk::UMat lpFrameLeft;
  vsdk::UMat lpFrameRightGs8;
  vsdk::UMat lpFrameLeftGs8;
  vsdk::UMat lpFrameRgbSub;
  uint32_t   loop;

  //unsigned long start, end;
  ISPCameraInputStereo stereo_camera_handle;

// Init DCU Output
#ifdef __STANDALONE__
  io::FrameOutputDCU lDcuOutput(SCR_WIDTH, SCR_HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT
#ifdef DISP_GS8
                                ,
                                DCU_BPP_8
#endif
  );
#else
  // setup Ctrl+C handler
  if(stereo_camera_handle.SigintSetup() != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to register Ctrl+C signal handler.");
    return -1;
  }

  printf("Press Ctrl+C to terminate the demo.\n");

  io::FrameOutputV234Fb lDcuOutput(SCR_WIDTH, SCR_HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT);
#endif // else from #ifdef __STANDALONE__

  // reading intrinsic parameters
  FileStorage fs("data/apps/isp_stereo_apexbm/stereo_calibration.yml", CV_STORAGE_READ);
  if(!fs.isOpened())
  {
    printf("Failed to open file stereo_calibration.yml\n");
    return -1;
  }

  Mat CM1, CM2, D1, D2, R, T, E, F, R1, R2, P1, P2, Q;
  fs["CM1"] >> CM1;
  fs["CM2"] >> CM2;
  fs["D1"] >> D1;
  fs["D2"] >> D2;
  fs["R"] >> R;
  fs["T"] >> T;
  fs["E"] >> E;
  fs["F"] >> F;
  fs["R1"] >> R1;
  fs["R2"] >> R2;
  fs["P1"] >> P1;
  fs["P2"] >> P2;
  fs["Q"] >> Q;

  Mat map11, map12, map21, map22;
  initUndistortRectifyMap(CM1, D1, R1, P1, Size(IMG_W, IMG_H), CV_16SC2, map11, map12);
  initUndistortRectifyMap(CM2, D2, R2, P2, Size(IMG_W, IMG_H), CV_16SC2, map21, map22);

  vsdk::UMat outputbuffer_umat(720, 1280, VSDK_CV_8UC3);

  //  DataDescriptor block_offsets(64, 720/8, DATATYPE_32U);
  vsdk::UMat block_offsets1(IMG_H / TRANSFER_TILE_SIZE_Y, IMG_W / TRANSFER_TILE_SIZE_X, VSDK_CV_32SC1);
  vsdk::UMat local_offsets1(IMG_H, IMG_W, VSDK_CV_16UC1);
  vsdk::UMat block_offsets2(IMG_H / TRANSFER_TILE_SIZE_Y, IMG_W / TRANSFER_TILE_SIZE_X, VSDK_CV_32SC1);
  vsdk::UMat local_offsets2(IMG_H, IMG_W, VSDK_CV_16UC1);
  //DataDescriptor local_offsets(in.cols, in.rows, DATATYPE_16U);

#define CONFIG_CHUNK_SIZE_NEIGB_W 5

#define CONFIG_DST_SIZE_W IMG_W
#define CONFIG_DST_SIZE_H IMG_H

#define WPAD 10
#define HPAD 10

  {
    vsdk::Mat offsetData1Mat = block_offsets1.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
    vsdk::Mat offsetData2Mat = block_offsets2.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
    uint32_t* offsetData1    = (uint32_t*)offsetData1Mat.data;
    uint32_t* offsetData2    = (uint32_t*)offsetData2Mat.data;
    for(int y = 0; y < block_offsets1.rows; ++y)
    {
      for(int x = 0; x < block_offsets1.cols; ++x)
      {
        unsigned int index     = y * block_offsets1.cols + x;
        int          xShuffled = map11.at< Vec2s >(y * TRANSFER_TILE_SIZE_Y, x * TRANSFER_TILE_SIZE_X)[0]; //x;
        int          yShuffled = map11.at< Vec2s >(y * TRANSFER_TILE_SIZE_Y, x * TRANSFER_TILE_SIZE_X)[1]; //x;

        int offset         = yShuffled * IMG_W + xShuffled;
        offsetData1[index] = offset;

        index     = y * block_offsets2.cols + x;
        xShuffled = map21.at< Vec2s >(y * TRANSFER_TILE_SIZE_Y, x * TRANSFER_TILE_SIZE_X)[0]; //x;
        yShuffled = map21.at< Vec2s >(y * TRANSFER_TILE_SIZE_Y, x * TRANSFER_TILE_SIZE_X)[1]; //x;

        offset             = yShuffled * IMG_W + xShuffled;
        offsetData2[index] = offset;
      }
    }

    vsdk::Mat local_offsets1Mat = local_offsets1.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
    vsdk::Mat local_offsets2Mat = local_offsets2.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
    uint16_t* local1            = (uint16_t*)local_offsets1Mat.data;
    uint16_t* local2            = (uint16_t*)local_offsets2Mat.data;

    unsigned int index = 0;

    for(int y = 0; y < local_offsets1.rows; ++y)
    {
      for(int x = 0; x < local_offsets1.cols; ++x)
      {
        int xShuffled = map11.at< Vec2s >(y, x)[0]; //x;
        int yShuffled = map11.at< Vec2s >(y, x)[1]; //x;
        int xbase     = map11.at< Vec2s >((y / TRANSFER_TILE_SIZE_Y) * TRANSFER_TILE_SIZE_Y,
                                      (x / TRANSFER_TILE_SIZE_X) * TRANSFER_TILE_SIZE_X)[0];
        int ybase     = map11.at< Vec2s >((y / TRANSFER_TILE_SIZE_Y) * TRANSFER_TILE_SIZE_Y,
                                      (x / TRANSFER_TILE_SIZE_X) * TRANSFER_TILE_SIZE_X)[1];

        int out;
        if(yShuffled - ybase >= TRANSFER_TILE_SIZE_Y + HPAD || yShuffled - ybase <= -HPAD ||
           xShuffled - xbase >= TRANSFER_TILE_SIZE_X + WPAD || xShuffled - xbase <= -WPAD || xShuffled < 0 ||
           xShuffled >= IMG_W || yShuffled < 0 || yShuffled >= IMG_H)
          out = 0x8000;

        else
          out = (yShuffled - ybase) * (TRANSFER_TILE_SIZE_X + WPAD + WPAD) + (xShuffled - xbase);
        local1[index] = out;

        xShuffled = map21.at< Vec2s >(y, x)[0]; //x;
        yShuffled = map21.at< Vec2s >(y, x)[1]; //x;
        xbase     = map21.at< Vec2s >((y / TRANSFER_TILE_SIZE_Y) * TRANSFER_TILE_SIZE_Y,
                                  (x / TRANSFER_TILE_SIZE_X) * TRANSFER_TILE_SIZE_X)[0];
        ybase     = map21.at< Vec2s >((y / TRANSFER_TILE_SIZE_Y) * TRANSFER_TILE_SIZE_Y,
                                  (x / TRANSFER_TILE_SIZE_X) * TRANSFER_TILE_SIZE_X)[1];

        if(yShuffled - ybase >= TRANSFER_TILE_SIZE_Y + HPAD || yShuffled - ybase <= -HPAD ||
           xShuffled - xbase >= TRANSFER_TILE_SIZE_X + WPAD || xShuffled - xbase <= -WPAD || xShuffled < 0 ||
           xShuffled >= IMG_W || yShuffled < 0 || yShuffled >= IMG_H)
          out = 0x8000;
        else
          out = (yShuffled - ybase) * (TRANSFER_TILE_SIZE_X + WPAD + WPAD) + (xShuffled - xbase);

        local2[index] = out;
        ++index;
      }
    }
  }

  /*for (int y = 0; y < 720; ++y)
  {
    for (int x = 0; x < 1280; ++x)
    {
      printf("%d ", m_local[y * 1280 + x]);
    }
    printf("\n");
  }*/

  vsdk::UMat dst_datadescriptor1(IMG_H, IMG_W, VSDK_CV_8UC1);
  vsdk::UMat dst_datadescriptor2(IMG_H, IMG_W, VSDK_CV_8UC1);
  vsdk::UMat disparity_out[2];
  disparity_out[0] = vsdk::UMat(IMG_H, IMG_W, DATATYPE_08U);
  disparity_out[1] = vsdk::UMat(IMG_H, IMG_W, DATATYPE_08U);

  memset(disparity_out[0].getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, 0, IMG_H * IMG_W);
  memset(disparity_out[1].getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, 0, IMG_H * IMG_W);

  vsdk::CustomPointerUMat dst_datadescriptor1_lo(IMG_H / 2 / 3, IMG_W * 3, VSDK_CV_8UC1, dst_datadescriptor1.u->handle,
                                                 NULL);
  vsdk::CustomPointerUMat dst_datadescriptor1_hi(IMG_H / 2 / 3, IMG_W * 3, VSDK_CV_8UC1,
                                                 (uint8_t*)dst_datadescriptor1.u->handle + IMG_W * IMG_H / 2, NULL);
  vsdk::CustomPointerUMat dst_datadescriptor2_lo(IMG_H / 2 / 3, IMG_W * 3, VSDK_CV_8UC1, dst_datadescriptor2.u->handle,
                                                 NULL);
  vsdk::CustomPointerUMat dst_datadescriptor2_hi(IMG_H / 2 / 3, IMG_W * 3, VSDK_CV_8UC1,
                                                 (uint8_t*)dst_datadescriptor2.u->handle + IMG_W * IMG_H / 2, NULL);
  vsdk::CustomPointerUMat disparity_out_lo0(IMG_H / 2 / 3, IMG_W * 3, VSDK_CV_8UC1, disparity_out[0].u->handle, NULL);
  vsdk::CustomPointerUMat disparity_out_lo1(IMG_H / 2 / 3, IMG_W * 3, VSDK_CV_8UC1, disparity_out[1].u->handle, NULL);
  vsdk::CustomPointerUMat disparity_out_hi0(IMG_H / 2 / 3, IMG_W * 3, VSDK_CV_8UC1,
                                            (uint8_t*)disparity_out[0].u->handle + IMG_W * IMG_H / 2, NULL);
  vsdk::CustomPointerUMat disparity_out_hi1(IMG_H / 2 / 3, IMG_W * 3, VSDK_CV_8UC1,
                                            (uint8_t*)disparity_out[1].u->handle + IMG_W * IMG_H / 2, NULL);

  // Start the ISP
  stereo_camera_handle.Init();
  stereo_camera_handle.Start();

  APU_REMAP_BM process_remap_left(APEX_APEX0);
  APU_REMAP_BM process_remap_right(APEX_APEX1);

  int lRetVal = 0;
  lRetVal |= process_remap_left.Initialize();
  lRetVal |= process_remap_left.ConnectIO("OFFSET", local_offsets1);
  lRetVal |= process_remap_left.ConnectIO("OUTPUT", dst_datadescriptor1);

  lRetVal |= process_remap_right.Initialize();
  lRetVal |= process_remap_right.ConnectIO("OFFSET", local_offsets2);
  lRetVal |= process_remap_right.ConnectIO("OUTPUT", dst_datadescriptor2);
  int framecnt = 0;
  // Main loop
  printf("Grabbing/measuring...\n");

  APU_DISPARITY process_bm_lo(APEX_APEX0);
  lRetVal |= process_bm_lo.Initialize();
  lRetVal |= process_bm_lo.ConnectIO("IN0", dst_datadescriptor2_lo);
  lRetVal |= process_bm_lo.ConnectIO("IN1", dst_datadescriptor1_lo);

  APU_DISPARITY process_bm_hi(APEX_APEX1);
  lRetVal |= process_bm_hi.Initialize();
  lRetVal |= process_bm_hi.ConnectIO("IN0", dst_datadescriptor2_hi);
  lRetVal |= process_bm_hi.ConnectIO("IN1", dst_datadescriptor1_hi);

  int current = 0;
  int next    = 1;

  // fetched frame buffer storages
  SDI_Frame lFrame[FDMA_IX_END];

  for(loop = 0; loop < NUM_FRAMES; loop++)
  {
    framecnt++;
    // Grab the images
    lpFrameRight = stereo_camera_handle.GetFrame(FDMA_IX_FastDMA_Right_Out, lFrame[FDMA_IX_FastDMA_Right_Out]);
    if(lpFrameRight.empty())
    {
      break;
    }

    lpFrameLeft = stereo_camera_handle.GetFrame(FDMA_IX_FastDMA_Left_Out, lFrame[FDMA_IX_FastDMA_Left_Out]);
    if(lpFrameLeft.empty())
    {
      break;
    }

    lpFrameRightGs8 = stereo_camera_handle.GetFrame(FDMA_IX_FastDMA_RightGs_Out, lFrame[FDMA_IX_FastDMA_RightGs_Out]);
    if(lpFrameRightGs8.empty())
    {
      break;
    }

    lpFrameLeftGs8 = stereo_camera_handle.GetFrame(FDMA_IX_FastDMA_LeftGs_Out, lFrame[FDMA_IX_FastDMA_LeftGs_Out]);
    if(lpFrameLeftGs8.empty())
    {
      break;
    }

    lpFrameRgbSub = stereo_camera_handle.GetFrame(FDMA_IX_FastDMA_RgbSub_Out, lFrame[FDMA_IX_FastDMA_RgbSub_Out]);
    if(lpFrameRgbSub.empty())
    {
      break;
    }

    lRetVal |= process_remap_left.ConnectIndirectInput("INPUT", lpFrameLeftGs8, block_offsets1);
    lRetVal |= process_remap_right.ConnectIndirectInput("INPUT", lpFrameRightGs8, block_offsets2);

    // execute remap
    //start = FSL_Ticks();
    lRetVal |= process_remap_left.Start();
    lRetVal |= process_remap_right.Start();
    lRetVal |= process_remap_left.Wait();
    lRetVal |= process_remap_right.Wait();

    //blur(out1, out1, Size(3, 1));
    //blur(out2, out2, Size(3, 1));
    if(current == 0)
    {
      lRetVal |= process_bm_lo.ConnectIO("OUT", disparity_out_lo0);
      lRetVal |= process_bm_hi.ConnectIO("OUT", disparity_out_hi0);
    }
    else
    {
      lRetVal |= process_bm_lo.ConnectIO("OUT", disparity_out_lo1);
      lRetVal |= process_bm_hi.ConnectIO("OUT", disparity_out_hi1);
    }

    lRetVal |= process_bm_lo.Start();
    lRetVal |= process_bm_hi.Start();

    {
      vsdk::Mat outputbuffer_mat   = outputbuffer_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
      vsdk::Mat disparity_out0_mat = disparity_out[0].getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
      vsdk::Mat disparity_out1_mat = disparity_out[1].getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);

      unsigned char* rgboutput = (unsigned char*)outputbuffer_mat.data;

      unsigned char* disp_out_ptr[2];
      disp_out_ptr[0] = (unsigned char*)disparity_out0_mat.data;
      disp_out_ptr[1] = (unsigned char*)disparity_out1_mat.data;

      int y_local  = 0;
      int y_local1 = 0;
      int x_local  = 0;
      int val;
      //      memset(rgboutput, 0, 1280*760*3);
      for(int y = 0; y < 360; ++y)
        for(int x = 0; x < 640; ++x)
        {
          val = (int)(disp_out_ptr[next][y * 640 + x]);
          if(val != 0)
          {
            y_local  = y * 7680;
            y_local1 = y_local + 3840;
            x_local  = x * 6;

            rgboutput[y_local + x_local]     = pallette[val][0];
            rgboutput[y_local + x_local + 1] = pallette[val][1];
            rgboutput[y_local + x_local + 2] = pallette[val][2];

            rgboutput[y_local + x_local + 3] = pallette[val][0];
            rgboutput[y_local + x_local + 4] = pallette[val][1];
            rgboutput[y_local + x_local + 5] = pallette[val][2];

            rgboutput[y_local1 + x_local]     = pallette[val][0];
            rgboutput[y_local1 + x_local + 1] = pallette[val][1];
            rgboutput[y_local1 + x_local + 2] = pallette[val][2];

            rgboutput[y_local1 + x_local + 3] = pallette[val][0];
            rgboutput[y_local1 + x_local + 4] = pallette[val][1];
            rgboutput[y_local1 + x_local + 5] = pallette[val][2];
          }
          else
          {
            y_local  = y * 7680;
            y_local1 = y_local + 3840;
            x_local  = x * 6;

            rgboutput[y_local + x_local]     = 0;
            rgboutput[y_local + x_local + 1] = 0;
            rgboutput[y_local + x_local + 2] = 0;

            rgboutput[y_local + x_local + 3] = 0;
            rgboutput[y_local + x_local + 4] = 0;
            rgboutput[y_local + x_local + 5] = 0;

            rgboutput[y_local1 + x_local]     = 0;
            rgboutput[y_local1 + x_local + 1] = 0;
            rgboutput[y_local1 + x_local + 2] = 0;

            rgboutput[y_local1 + x_local + 3] = 0;
            rgboutput[y_local1 + x_local + 4] = 0;
            rgboutput[y_local1 + x_local + 5] = 0;
          }
        }
    }

    //Mat out(720, 1280, CV_8UC3, outputbuffer);
    //imwrite("pic.png", out);
    lDcuOutput.PutFrame(outputbuffer_umat);

    lRetVal |= process_bm_lo.Wait();
    lRetVal |= process_bm_hi.Wait();
    //end = FSL_Ticks();
    //printf("BM:      %fs.\n", FSL_TicksToSeconds(end-start));

    // Output, TODO faster
    /*Mat disp8;
    disp_out.convertTo(disp8, CV_8U, 255/64);
    cvtColor(disp8, disp8, CV_GRAY2RGB);
    resize(disp8, disp8, Size(1280, 720));
    memcpy(outputbuffer, disp8.data, 1280*720*3);
      
    // Show the images
    lDcuOutput.PutFrame(outputbuffer, false);*/

    // Output, TODO faster

    // Sync the input
    stereo_camera_handle.PutFrame(lFrame[FDMA_IX_FastDMA_Right_Out]);
    stereo_camera_handle.PutFrame(lFrame[FDMA_IX_FastDMA_Left_Out]);
    stereo_camera_handle.PutFrame(lFrame[FDMA_IX_FastDMA_RightGs_Out]);
    stereo_camera_handle.PutFrame(lFrame[FDMA_IX_FastDMA_LeftGs_Out]);
    stereo_camera_handle.PutFrame(lFrame[FDMA_IX_FastDMA_RgbSub_Out]);

    current = (current + 1) % 2;
    next    = (next + 1) % 2;
    // If Ctrl+c pressed, break
    if(stereo_camera_handle.sStop)
    {
      break;
    }
  }

  printf("***** Loop exit *****\n");

  // Stop ISP processing
  stereo_camera_handle.Stop();
  stereo_camera_handle.Deinit();
}
