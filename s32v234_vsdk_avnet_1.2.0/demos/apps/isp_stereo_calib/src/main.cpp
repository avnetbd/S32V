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
using namespace cv;

#include <common_time_measure.h>
#include <common_helpers.h>
#include <string>

#ifdef __STANDALONE__
#include "frame_output_dcu.h"
#define CHNL_CNT io::IO_DATA_CH3
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"
#define CHNL_CNT io::IO_DATA_CH3
#endif // else from #ifdef __STANDALONE__

#include "camera_input.h"

// Calibration specifics
int  numBoards = 30; // number of different poses
int  board_w   = 9;  // number of horizontal corners
int  board_h   = 6;  // number of vertical corners
Size board_sz  = Size(board_w, board_h);
int  board_n   = board_w * board_h;

// Stereo calibration lists
vector< vector< Point3f > > objectPoints;      // points on chessboard for the current frame
vector< vector< Point2f > > imagePoints_left;  // points on left image for the current frame
vector< vector< Point2f > > imagePoints_right; // points on right image for the current frame
vector< Point2f >           corners_left;      // detected corners on left
vector< Point2f >           corners_right;     // detected corners on right
vector< Point3f >           obj;               // precomputed corners on chessboard

static volatile bool thread_running = false;
static char          image_static_left[360 * 640];
static char          image_static_right[360 * 640];

Mat left_img(360, 640, CV_8UC1, image_static_left);
Mat right_img(360, 640, CV_8UC1, image_static_right);

// Thread is started when no other detection thread is running (thread running flag)
void* detectcornersthread(void*)
{
  printf("Detection thread started.\n");

  // find corners
  unsigned long start = FSL_Ticks();
  bool          found_left =
      findChessboardCorners(left_img, board_sz, corners_left, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
  bool found_right =
      findChessboardCorners(right_img, board_sz, corners_right, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
  unsigned long end = FSL_Ticks();
  printf("Search for corners:        %fs.\n", FSL_TicksToSeconds(end - start));

  // If found, refine the search and save the points
  if(found_left && found_right)
  {
    printf("Found, going sub-pixel...\n");
    start = FSL_Ticks();
    cornerSubPix(left_img, corners_left, Size(11, 11), Size(-1, -1),
                 TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
    cornerSubPix(right_img, corners_right, Size(11, 11), Size(-1, -1),
                 TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
    end = FSL_Ticks();
    printf("Search for corners subpix: %fs.\n", FSL_TicksToSeconds(end - start));

    imagePoints_left.push_back(corners_left);
    imagePoints_right.push_back(corners_right);
    objectPoints.push_back(obj);
    numBoards--;
    printf("Corners stored, %d to go...\n", numBoards);
    sleep(1);
  }

  thread_running = false;
  return 0;
}

int main(int argc, char** argv)
{
  std::string helpMsg =
      std::string("Demonstrates stereo camera calibration with ISP.\n\tUsage: ") + COMMON_ExtractProgramName(argv[0]);
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  // Frame buffer pointers
  vsdk::UMat lpFrameRight;
  vsdk::UMat lpFrameLeft;
  vsdk::UMat lpFrameRightGs8;
  vsdk::UMat lpFrameLeftGs8;
  vsdk::UMat lpFrameRgbSub;
  uint32_t   loop;

  unsigned long        start, end;
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

  // Save chessboard corners
  for(int j = 0; j < board_n; j++)
  {
    obj.push_back(Point3f(j / board_w, j % board_w, 0.0f));
  }

  // Start the ISP
  stereo_camera_handle.Init();
  stereo_camera_handle.Start();

  // fetched frame buffer storages
  SDI_Frame lFrame[FDMA_IX_END];

  // Main loop
  printf("Grabbing/measuring...\n");
  for(loop = 0; loop < NUM_FRAMES; loop++)
  {
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

    // when thread is not running, make a copy of the buffers and start the detection thread. Continue displaying the input during computation.
    if(!thread_running && numBoards > 0)
    {
      thread_running = true;

      memcpy(image_static_left, lpFrameLeftGs8.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, 640 * 360);
      memcpy(image_static_right, lpFrameRightGs8.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, 640 * 360);
      OAL_TCB task;
      OAL_TaskInit(&detectcornersthread, NULL, OAL_TASK_DEFAULT_STACK, OAL_TASK_DEFAULT_PRIORITY, (char*)"mytask",
                   &task);
    }

    // Show the images
    lDcuOutput.PutFrame(lpFrameRgbSub);

    // Sync the input
    stereo_camera_handle.PutFrame(lFrame[FDMA_IX_FastDMA_Right_Out]);
    stereo_camera_handle.PutFrame(lFrame[FDMA_IX_FastDMA_Left_Out]);
    stereo_camera_handle.PutFrame(lFrame[FDMA_IX_FastDMA_RightGs_Out]);
    stereo_camera_handle.PutFrame(lFrame[FDMA_IX_FastDMA_LeftGs_Out]);
    stereo_camera_handle.PutFrame(lFrame[FDMA_IX_FastDMA_RgbSub_Out]);

    // If Ctrl+c pressed, break
    if(stereo_camera_handle.sStop || numBoards == 0)
    {
      break;
    }
  }

  printf("***** Loop exit *****\n");

  if(numBoards == 0)
  {
    printf("All boards available, starting calibration...\n");
    start   = FSL_Ticks();
    Mat CM1 = Mat(3, 3, CV_64FC1);
    Mat CM2 = Mat(3, 3, CV_64FC1);
    Mat D1, D2;
    Mat R, T, E, F;

    stereoCalibrate(objectPoints, imagePoints_left, imagePoints_right, CM1, D1, CM2, D2, Size(640, 360), R, T, E, F,
                    CV_CALIB_SAME_FOCAL_LENGTH | CV_CALIB_ZERO_TANGENT_DIST,
                    cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 100, 1e-5));

    Mat R1, R2, P1, P2, Q;

    stereoRectify(CM1, D1, CM2, D2, Size(640, 360), R, T, R1, R2, P1, P2, Q);

    end = FSL_Ticks();
    printf("Calibration done in:       %fs.\n", FSL_TicksToSeconds(end - start));

    FileStorage fs1("data/apps/isp_stereo_apexbm/stereo_calibration.yml", FileStorage::WRITE);
    fs1 << "CM1" << CM1;
    fs1 << "CM2" << CM2;
    fs1 << "D1" << D1;
    fs1 << "D2" << D2;
    fs1 << "R" << R;
    fs1 << "T" << T;
    fs1 << "E" << E;
    fs1 << "F" << F;
    fs1 << "R1" << R1;
    fs1 << "R2" << R2;
    fs1 << "P1" << P1;
    fs1 << "P2" << P2;
    fs1 << "Q" << Q;

    fs1.release();
  }

  // Stop ISP processing
  stereo_camera_handle.Stop();
  stereo_camera_handle.Deinit();
}
