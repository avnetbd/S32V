/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
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
#include "lane_detector.h"

#include "least_squares_line.h"
#include <config_ldw.h>
#include "apex.h"

////////////////////////////////////////////////////////////////////////////////
// Aux structure to be used for sorting two vectors of vectors along Z axis
////////////////////////////////////////////////////////////////////////////////
struct vectorSorter {
  bool operator() (cv::Vec3d pt1, cv::Vec3d pt2) 
  { 
    return (pt1.val[2] < pt2.val[2]); 
  }
} vectorSort;

////////////////////////////////////////////////////////////////////////////////
// Constructor
// Empty only for alloc
////////////////////////////////////////////////////////////////////////////////
Lane::Lane()
{
  
}

////////////////////////////////////////////////////////////////////////////////
// Constructor
// Copy constructor
////////////////////////////////////////////////////////////////////////////////
Lane::Lane(const Lane &lane)
{
  left[0] = lane.left[0];
  left[1] = lane.left[1];
  
  right[0] = lane.right[0];
  right[1] = lane.right[1];
  
  left_bird[0] = lane.left_bird[0];
  left_bird[1] = lane.left_bird[1];
  
  right_bird[0] = lane.right_bird[0];
  right_bird[1] = lane.right_bird[1];
  
  left_eq = lane.left_eq;
  right_eq = lane.right_eq;
  
  left_peak_power = lane.left_peak_power;
  right_peak_power = lane.right_peak_power;
}

Lane::~Lane()
{
  
}


////////////////////////////////////////////////////////////////////////////////
// Constructor
// Empty only for alloc
////////////////////////////////////////////////////////////////////////////////
LaneDetector::LaneDetector():
analysis_times_avg(50),
history_times_avg(50),
findlane_times_avg(50),
birdview_times_avg(50)//: m_maximum_process(APEX_APEX1)
{
  
}

Update_events LaneDetector::update_event = none;

////////////////////////////////////////////////////////////////////////////////
// Constructor
// Initializes parameters and opens the input file
// Pre-computes camera parameters.
// @param filename Input file
// @param params Detection parameters
////////////////////////////////////////////////////////////////////////////////
LaneDetector::LaneDetector(int apuldx):
analysis_times_avg(50),
history_times_avg(50),
findlane_times_avg(50),
birdview_times_avg(50)//: m_maximum_process(APEX_APEX1)
{ 
  m_apuldx = apuldx;
  
  new (&m_maximum_process) HISTORY_MAXIMUM(apuldx);
  
  new (&m_analysis_process) ANALYSIS(apuldx);
  
  new (&warp) Warp(apuldx);
  
  //Set curve width
  priority_curve = (char*)malloc(CONFIG_BIRD_WIDTH * sizeof(char));
  
  m_analysis_stream = (char*)malloc((CONFIG_BIRD_WIDTH * 8 + CONFIG_BIRD_WIDTH * CONFIG_BIRD_HEIGHT * 2)* sizeof(char));
  
  FillPriorityCurve();
  
  //Init kalman filter position
  LeftPosition_down.Initialize( webserver_getDoubleValueByName("kalman_measurement_noise",CONFIG_KALMAN_MEASUREMENT_NOISE), 
                        webserver_getDoubleValueByName("kalman_post_error",CONFIG_KALMAN_POST_ERROR), 20, webserver_getDoubleValueByName("kalman_noise_var",CONFIG_KALMAN_PROCESS_NOISE_VAR));
  
  RightPosition_down.Initialize( webserver_getDoubleValueByName("kalman_measurement_noise",CONFIG_KALMAN_MEASUREMENT_NOISE),
                        webserver_getDoubleValueByName("kalman_post_error",CONFIG_KALMAN_POST_ERROR), 140, webserver_getDoubleValueByName("kalman_noise_var",CONFIG_KALMAN_PROCESS_NOISE_VAR));
  
  LeftPosition_up.Initialize( webserver_getDoubleValueByName("kalman_measurement_noise",CONFIG_KALMAN_MEASUREMENT_NOISE), 
                        webserver_getDoubleValueByName("kalman_post_error",CONFIG_KALMAN_POST_ERROR), 20, webserver_getDoubleValueByName("kalman_noise_var",CONFIG_KALMAN_PROCESS_NOISE_VAR));
  
  RightPosition_up.Initialize( webserver_getDoubleValueByName("kalman_measurement_noise",CONFIG_KALMAN_MEASUREMENT_NOISE), 
                        webserver_getDoubleValueByName("kalman_post_error",CONFIG_KALMAN_POST_ERROR), 140, webserver_getDoubleValueByName("kalman_noise_var",CONFIG_KALMAN_PROCESS_NOISE_VAR));
  
  // History maximum image initi
  m_bird_max = cv::UMat(CONFIG_BIRD_HEIGHT, CONFIG_BIRD_WIDTH,VSDK_CV_8UC1);
  memset(m_bird_max.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, 0, CONFIG_BIRD_HEIGHT * CONFIG_BIRD_WIDTH);
  
  // History offset init
  m_bird_history_offset = 0;
  
  // Init history edge images
  m_bird_history_icp = new vsdk::UMat[CONFIG_HISTORY];
  for (int i = 0; i < CONFIG_HISTORY; ++i)
    m_bird_history_icp[i] = vsdk::UMat(CONFIG_BIRD_HEIGHT, CONFIG_BIRD_WIDTH, VSDK_CV_8UC1);
    
  m_bird_history_icp_last = m_bird_history_icp;
  
  unsigned int help = 1;
  m_priority_down = vsdk::UMat(1, 1, VSDK_CV_32SC1); 
  memcpy(m_priority_down.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, &help, 4);
  
  help = (CONFIG_BIRD_HEIGHT / 3 * 2) / 10;
  m_priority_up = vsdk::UMat(1, 1, VSDK_CV_32SC1); 
  memcpy(m_priority_up.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, &help, 4);
  
  help = (CONFIG_BIRD_HEIGHT / 10) /3 * 2;
  m_start_tile = vsdk::UMat(1, 1, VSDK_CV_32SC1); 
  memcpy(m_start_tile.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, &help, 4);
  
  m_signal_down = vsdk::UMat(1, CONFIG_BIRD_WIDTH, VSDK_CV_32SC1); 
  m_signal_up   = vsdk::UMat(1, CONFIG_BIRD_WIDTH, VSDK_CV_32SC1);
  
  m_bird_image = vsdk::UMat(CONFIG_BIRD_HEIGHT, CONFIG_BIRD_WIDTH, VSDK_CV_8UC1);
  
  // Initialize history APEX process
  m_maximum_process.Initialize();
  m_maximum_process.ConnectIO("SRC_0",  m_bird_history_icp[0]);
  m_maximum_process.ConnectIO("SRC_1",  m_bird_history_icp[1]);
  m_maximum_process.ConnectIO("SRC_2",  m_bird_history_icp[2]);
  m_maximum_process.ConnectIO("SRC_3",  m_bird_history_icp[3]);
  m_maximum_process.ConnectIO("SRC_4",  m_bird_history_icp[4]);
  m_maximum_process.ConnectIO("SRC_5",  m_bird_history_icp[5]);
  m_maximum_process.ConnectIO("SRC_6",  m_bird_history_icp[6]);
  m_maximum_process.ConnectIO("SRC_7",  m_bird_history_icp[7]);
  m_maximum_process.ConnectIO("SRC_8",  m_bird_history_icp[8]);
  m_maximum_process.ConnectIO("SRC_9",  m_bird_history_icp[9]);
  /*m_maximum_process.ConnectIO("SRC_10", m_bird_history_icp[10]);
  m_maximum_process.ConnectIO("SRC_11", m_bird_history_icp[11]);
  m_maximum_process.ConnectIO("SRC_12", m_bird_history_icp[12]);
  m_maximum_process.ConnectIO("SRC_13", m_bird_history_icp[13]);
  m_maximum_process.ConnectIO("SRC_14", m_bird_history_icp[14]);*/
  m_maximum_process.ConnectIO("DST_IMG", m_bird_max);
  
  m_analysis_process.Initialize();
  m_analysis_process.ConnectIO("InputBirdView", m_bird_max);
  m_analysis_process.ConnectIO("Prior_down", m_priority_down);
  m_analysis_process.ConnectIO("Prior_up", m_priority_up);
  m_analysis_process.ConnectIO("Start_tile", m_start_tile);
  m_analysis_process.ConnectIO("Columns_sum_down", m_signal_down);
  m_analysis_process.ConnectIO("Columns_sum_up", m_signal_up);
  
}

////////////////////////////////////////////////////////////////////////////////
// Destructor - frees resources
////////////////////////////////////////////////////////////////////////////////
LaneDetector::~LaneDetector()
{
  free(priority_curve);
  free(m_analysis_stream);
  delete[] m_bird_history_icp;
  delete[] m_lookup_line_ends; 
}

////////////////////////////////////////////////////////////////////////////////
// Auxiliary function used in GetNextFrame
// Saves current frame, warps bird eye image and computes maximum image
// from n last bird eye frames
////////////////////////////////////////////////////////////////////////////////

void LaneDetector::SaveHistory()
{ 
  unsigned long ltimeStart = 0; 
  unsigned long ltimeEnd = 0;
  
  GETTIME(&ltimeStart);
  // Warp it into bird eye view
  warp.Warp_BirdEye(m_image, m_bird_image, m_transform);
  GETTIME(&ltimeEnd);

  webserver_addOutValue("birdview_time",birdview_times_avg.GetNext(ltimeEnd - ltimeStart));
  
  // Copy result in bird eye history vector and incremnt offset
  memcpy(m_bird_history_icp_last->getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).data, 
         m_bird_image.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, 
         m_bird_image.rows * m_bird_image.cols);
  
  
  
  GETTIME(&ltimeStart);
  m_maximum_process.Start();
  //m_maximum_process.Start_Run();
  
  m_bird_history_offset = (m_bird_history_offset + 1) % CONFIG_HISTORY;
  m_bird_history_icp_last = m_bird_history_icp + m_bird_history_offset;
  
  //m_maximum_process.Wait_NoReset();
  m_maximum_process.Wait();  
  GETTIME(&ltimeEnd);
  
  webserver_addOutValue("history_time", history_times_avg.GetNext(ltimeEnd - ltimeStart)); 
  
  GETTIME(&ltimeStart);
  m_analysis_process.Start();
  m_analysis_process.Wait();
  GETTIME(&ltimeEnd);
  
  webserver_addOutValue("to_signals_time", analysis_times_avg.GetNext(ltimeEnd - ltimeStart)); 
  
  // Copy result into OpenCV structure
  
  GETTIME(&ltimeStart);
  FindLane();
  GETTIME(&ltimeEnd);
  
  webserver_addOutValue("findlane_time", findlane_times_avg.GetNext(ltimeEnd - ltimeStart)); 
}

void LaneDetector::FindLane()
{          
      vsdk::Mat m_signal_down_mat = m_signal_down.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
      signal_down = (int *)m_signal_down_mat.data;
      
      vsdk::Mat m_signal_up_mat = m_signal_up.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
      signal_up = (int *)m_signal_up_mat.data;
      
      // Aplication of priority curve on both signals
      for(int x = 0; x < CONFIG_BIRD_WIDTH; x++)
      {
        signal_down[x] *= (short)priority_curve[x];
        signal_up[x] *= (short)priority_curve[x];
      }
      
      if(webserver_getStringValueByName("video_data_sel","origin").compare("analysis") == 0)
      {
        memcpy(m_analysis_stream, m_bird_image.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data, CONFIG_BIRD_HEIGHT * CONFIG_BIRD_WIDTH);
        memcpy(m_analysis_stream + CONFIG_BIRD_HEIGHT * CONFIG_BIRD_WIDTH, m_bird_max.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED).data, CONFIG_BIRD_HEIGHT * CONFIG_BIRD_WIDTH);
        memcpy(m_analysis_stream + CONFIG_BIRD_HEIGHT * CONFIG_BIRD_WIDTH * 2, signal_down, CONFIG_BIRD_WIDTH * 4);
        memcpy(m_analysis_stream + CONFIG_BIRD_HEIGHT * CONFIG_BIRD_WIDTH * 2 + CONFIG_BIRD_WIDTH * 4, signal_up, CONFIG_BIRD_WIDTH * 4);
        webserver_streamBuffer(m_analysis_stream, CONFIG_BIRD_HEIGHT * CONFIG_BIRD_WIDTH * 2 + CONFIG_BIRD_WIDTH * 8, "mainCanvas,analysis");
      }
      
      // Simple average 
      for(int j = 0; j < 2;j++)
      {
        int last_down = 0;
        int last_up = 0;
        
        for(int i = 1; i < CONFIG_BIRD_WIDTH - 1; i++)
        {
          int temp_down = (signal_down[i] + signal_down[i + 1] + last_down) / 3;
          int temp_up = (signal_up[i] + signal_up[i + 1] + last_up) / 3;
          last_down = signal_down[i];
          last_up = signal_up[i];
          signal_down[i] = temp_down;
          signal_up[i] = temp_up;
        }
      } 
      
      int first_max_down = 0;
      int first_index_down = -1;
      
      int first_max_up = 0;
      int first_index_up = -1;
      
      // Find maximal peak of left line
      for(int i = 0; i < CONFIG_BIRD_WIDTH / 2; i++)
      {
        
        if(signal_down[i] > first_max_down)
        {
          first_index_down = i;
          first_max_down = signal_down[i];
        }     
      
        if(signal_up[i] > first_max_up)
        {
          first_index_up = i;
          first_max_up = signal_up[i];
        }
      }
      if(abs(LeftPosition_down.GetValue() - (double)first_index_down)<CONFIG_MAX_DIFF)
      {
        LeftPosition_down.NextMeasurement((double)first_index_down);
        if(m_down_left_ctr > 0)
            m_down_left_ctr--;
      }
      
      else
      {
           m_down_left_ctr++;
      }
      
      if(abs(LeftPosition_up.GetValue() - (double)first_index_up)<CONFIG_MAX_DIFF)
      {
        LeftPosition_up.NextMeasurement((double)first_index_up);
        if(m_up_left_ctr > 0)
            m_up_left_ctr--;
      }
      
      else
      {
          m_up_left_ctr++;
      }
      
      if((m_down_left_ctr > CONFIG_MAX_COUNTER_VAL && m_up_left_ctr > CONFIG_MAX_COUNTER_VAL) || m_down_left_ctr >= CONFIG_MAX_COUNTER_VAL * 2 || m_up_left_ctr >= CONFIG_MAX_COUNTER_VAL * 2)
      {
        LeftPosition_up.NextMeasurement((double)first_index_up);
        LeftPosition_down.NextMeasurement((double)first_index_down);
      }
                
      
      ActualLeftline.x = first_index_up;
      ActualLeftline.y = first_index_down;
      
      first_index_down = -1;
      first_max_down = 0;
      first_index_up = -1;
      first_max_up = 0;
      
      // Find maximal peak of right line
      for(int i = CONFIG_BIRD_WIDTH / 2; i < CONFIG_BIRD_WIDTH; i++)
      {       
        if(signal_down[i] > first_max_down)
        {
          first_index_down = i;
          first_max_down = signal_down[i];
        }     
      
        if(signal_up[i] > first_max_up)
        {
          first_index_up = i;
          first_max_up = signal_up[i];
        }
      }
      
      if(abs(RightPosition_down.GetValue() - (double)first_index_down)<CONFIG_MAX_DIFF)
      {
        RightPosition_down.NextMeasurement((double)first_index_down);
        if(m_down_right_ctr > 0)
            m_down_right_ctr--;
        
      }
      
      else
      {
          m_down_right_ctr++;        
      }
        
      if(abs(RightPosition_up.GetValue() - (double)first_index_up)<CONFIG_MAX_DIFF)
      {
        RightPosition_up.NextMeasurement((double)first_index_up); 
        if(m_up_right_ctr > 0)
            m_up_right_ctr--;
      }
      else
      {
          m_up_right_ctr++;        
      }
      
      if((m_down_right_ctr > CONFIG_MAX_COUNTER_VAL && m_up_right_ctr > CONFIG_MAX_COUNTER_VAL) || m_down_right_ctr >= CONFIG_MAX_COUNTER_VAL * 2 || m_up_right_ctr >= CONFIG_MAX_COUNTER_VAL * 2)
      {
        RightPosition_down.NextMeasurement((double)first_index_down);
        RightPosition_up.NextMeasurement((double)first_index_up);
      }
           
      
      ActualRightline.x = first_index_up;
      ActualRightline.y = first_index_down;
   
      // Get lines approximation for visualisation and check lines possitions
      GetApproxLine();
      //GetApproxActualLine();     
}

void LaneDetector::GetApproxActualLine()
{
  
  cv::Point Lone(ActualLeftline.x, m_bird_image.rows / 2);
  cv::Point Ltwo(ActualLeftline.y, m_bird_image.rows - 1);
  
  cv::Point Rone(ActualRightline.x,  m_bird_image.rows / 2);
  cv::Point Rtwo(ActualRightline.y, m_bird_image.rows - 1);
  
  cv::Mat pt_bird(cv::Size(1, 3), CV_64FC1);
  cv::Mat pt_scene(cv::Size(1, 3), CV_64FC1);

  pt_bird.at<double>(0, 0) = Lone.x;
  pt_bird.at<double>(1, 0) = Lone.y;
  pt_bird.at<double>(2, 0) = 1;

  gemm(m_transform.GetHMat(),pt_bird,1,NULL,0,pt_scene);

  cv::Point2i scene;
  if (pt_scene.at<double>(2, 0) != 0)
  {
          Lone.x = (int)(pt_scene.at<double>(0, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
          Lone.y = (int)(pt_scene.at<double>(1, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
  }

  pt_bird.at<double>(0, 0) = Ltwo.x;
  pt_bird.at<double>(1, 0) = Ltwo.y;
  pt_bird.at<double>(2, 0) = 1;

  gemm(m_transform.GetHMat(),pt_bird,1,NULL,0,pt_scene);

  if (pt_scene.at<double>(2, 0) != 0)
  {
          Ltwo.x = (int)(pt_scene.at<double>(0, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
          Ltwo.y = (int)(pt_scene.at<double>(1, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
  }

  
  m_actual_detected_lane.left[0] = Lone;
  m_actual_detected_lane.left[1] = Ltwo;
  
  pt_bird.at<double>(0, 0) = Rone.x;
  pt_bird.at<double>(1, 0) = Rone.y;
  pt_bird.at<double>(2, 0) = 1;

  gemm(m_transform.GetHMat(),pt_bird,1,NULL,0,pt_scene);

  if (pt_scene.at<double>(2, 0) != 0)
  {
          Rone.x = (int)(pt_scene.at<double>(0, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
          Rone.y = (int)(pt_scene.at<double>(1, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
  }

  pt_bird.at<double>(0, 0) = Rtwo.x;
  pt_bird.at<double>(1, 0) = Rtwo.y;
  pt_bird.at<double>(2, 0) = 1;

  gemm(m_transform.GetHMat(),pt_bird,1,NULL,0,pt_scene);

  if (pt_scene.at<double>(2, 0) != 0)
  {
          Rtwo.x = (int)(pt_scene.at<double>(0, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
          Rtwo.y = (int)(pt_scene.at<double>(1, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
  }
  
  m_actual_detected_lane.right[0] = Rone;
  m_actual_detected_lane.right[1] = Rtwo;
}

////////////////////////////////////////////////////////////////////////////////
// Auxiliary function used in GetNextFrame
// Recount finded points from bird view to scene view
////////////////////////////////////////////////////////////////////////////////
void LaneDetector::GetApproxLine()
{
  // Compite points on line
  
  cv::Point Lone((int)LeftPosition_up.GetValue(), m_bird_image.rows / 2);
  cv::Point Ltwo((int)LeftPosition_down.GetValue(), m_bird_image.rows - 1);
  
  cv::Point Rone((int)RightPosition_up.GetValue(),  m_bird_image.rows / 2);
  cv::Point Rtwo((int)RightPosition_down.GetValue(), m_bird_image.rows - 1);
  
  // Logic of warnings
  int Rupdiff = (Rone.x - Lone.x);
  int Rdowndiff = (Rtwo.x - Ltwo.x);
  
  if(abs(Rupdiff - Rdowndiff) < CONFIG_MAX_DIFF && Rupdiff > (int)(CONFIG_BIRD_WIDTH * 0.5) && Rdowndiff > (int)(CONFIG_BIRD_WIDTH * 0.5)) m_lane_healthy = true;
    else m_lane_healthy = false;
  
  if(m_down_right_ctr > webserver_getIntValueByName("max_of_instability_counter",CONFIG_MAX_COUNTER_VAL)/4 && m_up_right_ctr > webserver_getIntValueByName("max_of_instability_counter",CONFIG_MAX_COUNTER_VAL)/4) m_left_peak_stable = false;
    else m_left_peak_stable = true;
    
  if(m_down_left_ctr > webserver_getIntValueByName("max_of_instability_counter",CONFIG_MAX_COUNTER_VAL)/4 && m_up_left_ctr > webserver_getIntValueByName("max_of_instability_counter",CONFIG_MAX_COUNTER_VAL)/4) m_right_peak_stable = false;
    else m_right_peak_stable = true;
    
  if(!m_right_peak_stable || !m_left_peak_stable) m_lane_healthy = false;
  
  int leftMax = webserver_getIntValueByName("war_left_max",CONFIG_LEFT_MAX) * CONFIG_BIRD_WIDTH / 100;
  int rightMax = webserver_getIntValueByName("war_right_max",CONFIG_RIGHT_MAX) * CONFIG_BIRD_WIDTH / 100;
  int rightMin = webserver_getIntValueByName("war_right_min",CONFIG_RIGHT_MIN) * CONFIG_BIRD_WIDTH / 100;
  int leftMin = webserver_getIntValueByName("war_left_min",CONFIG_LEFT_MIN) * CONFIG_BIRD_WIDTH / 100;
  
  if((Rone.x + Rtwo.x)/2  > rightMax && (Lone.x + Ltwo.x)/2 > leftMax && !m_left_warning) m_left_warning = true;
  else if(m_left_warning && (Lone.x + Ltwo.x)/2 > leftMax) m_left_warning = true;
  else if (m_left_warning) m_left_warning_drop_ctr ++;
  
  if(m_left_warning_drop_ctr > 5)
  {
    m_left_warning_drop_ctr = 0;
    m_left_warning = false;
  }
  
  if((Rone.x + Rtwo.x)/2  < rightMin && (Lone.x + Ltwo.x)/2 < leftMin && !m_right_warning) m_right_warning = true;
  else if(m_right_warning && (Rone.x + Rtwo.x)/2  < rightMin) m_right_warning = true;
  else if(m_right_warning) m_right_warning_drop_ctr++;
  
  if(m_right_warning_drop_ctr > 5)
  {
    m_right_warning_drop_ctr = 0;
    m_right_warning = false;
  }
 
  
  m_lane_middle_diff = (Rone.x + Rtwo.x)/2 - (Lone.x + Ltwo.x)/2;

  m_detected_lane.left_bird[0] = Lone;
  m_detected_lane.left_bird[1] = Ltwo;
  
  m_detected_lane.right_bird[0] = Rone;
  m_detected_lane.right_bird[1] = Rtwo;

  cv::Mat pt_bird(cv::Size(1, 3), CV_64FC1);
  cv::Mat pt_scene(cv::Size(1, 3), CV_64FC1);

  pt_bird.at<double>(0, 0) = Lone.x;
  pt_bird.at<double>(1, 0) = Lone.y;
  pt_bird.at<double>(2, 0) = 1;

  gemm(m_transform.GetHMat(),pt_bird,1,NULL,0,pt_scene);

  cv::Point2i scene;
  if (pt_scene.at<double>(2, 0) != 0)
  {
          Lone.x = (int)(pt_scene.at<double>(0, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
          Lone.y = (int)(pt_scene.at<double>(1, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
  }

  pt_bird.at<double>(0, 0) = Ltwo.x;
  pt_bird.at<double>(1, 0) = Ltwo.y;
  pt_bird.at<double>(2, 0) = 1;

  gemm(m_transform.GetHMat(),pt_bird,1,NULL,0,pt_scene);

  if (pt_scene.at<double>(2, 0) != 0)
  {
          Ltwo.x = (int)(pt_scene.at<double>(0, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
          Ltwo.y = (int)(pt_scene.at<double>(1, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
  }

  
  m_detected_lane.left[0] = Lone;
  m_detected_lane.left[1] = Ltwo;
  
  pt_bird.at<double>(0, 0) = Rone.x;
  pt_bird.at<double>(1, 0) = Rone.y;
  pt_bird.at<double>(2, 0) = 1;

  gemm(m_transform.GetHMat(),pt_bird,1,NULL,0,pt_scene);

  if (pt_scene.at<double>(2, 0) != 0)
  {
          Rone.x = (int)(pt_scene.at<double>(0, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
          Rone.y = (int)(pt_scene.at<double>(1, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
  }

  pt_bird.at<double>(0, 0) = Rtwo.x;
  pt_bird.at<double>(1, 0) = Rtwo.y;
  pt_bird.at<double>(2, 0) = 1;

  gemm(m_transform.GetHMat(),pt_bird,1,NULL,0,pt_scene);

  if (pt_scene.at<double>(2, 0) != 0)
  {
          Rtwo.x = (int)(pt_scene.at<double>(0, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
          Rtwo.y = (int)(pt_scene.at<double>(1, 0) / 
                        pt_scene.at<double>(2, 0) + 0.5);
  }

  m_detected_lane.right[0] = Rone;
  m_detected_lane.right[1] = Rtwo;
}

////////////////////////////////////////////////////////////////////////////////
// Fills priority curve higher priority is close to start and end of bird view
////////////////////////////////////////////////////////////////////////////////
void LaneDetector::FillPriorityCurve()
{
  
  
  int point_0 = CONFIG_BIRD_WIDTH * webserver_getIntValueByName("priority_0_x",CONFIG_CURVE_POINT_0_XCOO) / 100;
  int point_1 = CONFIG_BIRD_WIDTH * webserver_getIntValueByName("priority_1_x",CONFIG_CURVE_POINT_1_XCOO) / 100;
  int point_2 = CONFIG_BIRD_WIDTH * webserver_getIntValueByName("priority_2_x",CONFIG_CURVE_POINT_2_XCOO) / 100;
  int point_3 = CONFIG_BIRD_WIDTH * webserver_getIntValueByName("priority_3_x",CONFIG_CURVE_POINT_3_XCOO) / 100;
  int point_4 = CONFIG_BIRD_WIDTH * webserver_getIntValueByName("priority_4_x",CONFIG_CURVE_POINT_4_XCOO) / 100;
  int point_5 = CONFIG_BIRD_WIDTH * webserver_getIntValueByName("priority_5_x",CONFIG_CURVE_POINT_5_XCOO) / 100;
  int point_6 = CONFIG_BIRD_WIDTH * webserver_getIntValueByName("priority_6_x",CONFIG_CURVE_POINT_6_XCOO) / 100;
  int point_7 = CONFIG_BIRD_WIDTH * webserver_getIntValueByName("priority_7_x",CONFIG_CURVE_POINT_7_XCOO) / 100;
  
  float step = (webserver_getIntValueByName("priority_1_y",CONFIG_CURVE_POINT_1_XCOO_PRIORITY) - webserver_getIntValueByName("priority_0_y",CONFIG_CURVE_POINT_0_XCOO_PRIORITY))/(float)(point_1 - point_0);
  float value = webserver_getIntValueByName("priority_0_y",CONFIG_CURVE_POINT_0_XCOO_PRIORITY);
  
  for(int i = point_0; i < point_1; i++)
  {
    priority_curve[i] = (char)value;
    value += step;
  }
  
  step = (webserver_getIntValueByName("priority_2_y",CONFIG_CURVE_POINT_2_XCOO_PRIORITY) - webserver_getIntValueByName("priority_1_y",CONFIG_CURVE_POINT_1_XCOO_PRIORITY))/(float)(point_2 - point_1);
  value = webserver_getIntValueByName("priority_1_y",CONFIG_CURVE_POINT_1_XCOO_PRIORITY);
  
  for(int i = point_1; i < point_2; i++)
  {
    priority_curve[i] = (char)value;
    value += step;
  }
  
  step = (webserver_getIntValueByName("priority_3_y",CONFIG_CURVE_POINT_3_XCOO_PRIORITY) - webserver_getIntValueByName("priority_2_y",CONFIG_CURVE_POINT_2_XCOO_PRIORITY))/(float)(point_3 - point_2);
  value = webserver_getIntValueByName("priority_2_y",CONFIG_CURVE_POINT_2_XCOO_PRIORITY);
  
  for(int i = point_2; i < point_3; i++)
  {
    priority_curve[i] = (char)value;
    value += step;
  }
  
  step = (webserver_getIntValueByName("priority_4_y",CONFIG_CURVE_POINT_4_XCOO_PRIORITY) - webserver_getIntValueByName("priority_3_y",CONFIG_CURVE_POINT_3_XCOO_PRIORITY))/(float)(point_4 - point_3);
  value = webserver_getIntValueByName("priority_3_y",CONFIG_CURVE_POINT_3_XCOO_PRIORITY);
  
  for(int i = point_3; i < point_4; i++)
  {
    priority_curve[i] = (char)value;
    value += step;
  }
  
  step = (webserver_getIntValueByName("priority_5_y",CONFIG_CURVE_POINT_5_XCOO_PRIORITY) - webserver_getIntValueByName("priority_4_y",CONFIG_CURVE_POINT_4_XCOO_PRIORITY))/(float)(point_5 - point_4);
  value = webserver_getIntValueByName("priority_4_y",CONFIG_CURVE_POINT_4_XCOO_PRIORITY);
  
  for(int i = point_4; i < point_5; i++)
  {
    priority_curve[i] = (char)value;
    value += step;
  }
  
  step = (webserver_getIntValueByName("priority_6_y",CONFIG_CURVE_POINT_6_XCOO_PRIORITY) - webserver_getIntValueByName("priority_5_y",CONFIG_CURVE_POINT_5_XCOO_PRIORITY))/(float)(point_6 - point_5);
  value = webserver_getIntValueByName("priority_5_y",CONFIG_CURVE_POINT_5_XCOO_PRIORITY);
  
  for(int i = point_5; i < point_6; i++)
  {
    priority_curve[i] = (char)value;
    value += step;
  }
  
  step = (webserver_getIntValueByName("priority_7_y",CONFIG_CURVE_POINT_7_XCOO_PRIORITY) - webserver_getIntValueByName("priority_6_y",CONFIG_CURVE_POINT_6_XCOO_PRIORITY))/(float)(point_7 - point_6);
  value = webserver_getIntValueByName("priority_6_y",CONFIG_CURVE_POINT_6_XCOO_PRIORITY);
  
  for(int i = point_6; i < point_7; i++)
  {
    priority_curve[i] = (char)value;
    value += step;
  }
}


////////////////////////////////////////////////////////////////////////////////
// Returns lane information for current frame filtered by kalman
// @return Detected lane
////////////////////////////////////////////////////////////////////////////////
Lane LaneDetector::GetCurrentLane()
{
	return m_detected_lane;
}

///////////////////////////////////////////////////////////////////////////////
// Returns lane information for current frame
// @return Detected lane
////////////////////////////////////////////////////////////////////////////////
Lane LaneDetector::GetCurrentOfFrameLane()
{
        return m_actual_detected_lane;
}


////////////////////////////////////////////////////////////////////////////////
// Function loads next frame from the input file and finds lane position
// using implemented algorithm.
// @return True if there is frame read and detected
////////////////////////////////////////////////////////////////////////////////
bool LaneDetector::GetNextFrame(vsdk::UMat buffer)
{
  // Get next frame
  
  switch (update_event)
  {
    case Update_events::bird_view:
        new (&m_transform) Transform_LUT();
        warp.set_warp_init(false);
      break;
    case Update_events::priority_curve:
        FillPriorityCurve();
      break;
    case Update_events::kalman:
      //Init kalman filter position
      LeftPosition_down.Initialize( webserver_getDoubleValueByName("kalman_measurement_noise",CONFIG_KALMAN_MEASUREMENT_NOISE), 
                            webserver_getDoubleValueByName("kalman_post_error",CONFIG_KALMAN_POST_ERROR), 20, webserver_getDoubleValueByName("kalman_noise_var",CONFIG_KALMAN_PROCESS_NOISE_VAR));
      
      RightPosition_down.Initialize( webserver_getDoubleValueByName("kalman_measurement_noise",CONFIG_KALMAN_MEASUREMENT_NOISE),
                            webserver_getDoubleValueByName("kalman_post_error",CONFIG_KALMAN_POST_ERROR), 140, webserver_getDoubleValueByName("kalman_noise_var",CONFIG_KALMAN_PROCESS_NOISE_VAR));
      
      LeftPosition_up.Initialize( webserver_getDoubleValueByName("kalman_measurement_noise",CONFIG_KALMAN_MEASUREMENT_NOISE), 
                            webserver_getDoubleValueByName("kalman_post_error",CONFIG_KALMAN_POST_ERROR), 20, webserver_getDoubleValueByName("kalman_noise_var",CONFIG_KALMAN_PROCESS_NOISE_VAR));
      
      RightPosition_up.Initialize( webserver_getDoubleValueByName("kalman_measurement_noise",CONFIG_KALMAN_MEASUREMENT_NOISE), 
                            webserver_getDoubleValueByName("kalman_post_error",CONFIG_KALMAN_POST_ERROR), 140, webserver_getDoubleValueByName("kalman_noise_var",CONFIG_KALMAN_PROCESS_NOISE_VAR));
      break;
  }
  
  update_event = none;
  
  m_image = buffer;
  //m_image = vsdk::UMat(CONFIG_SCENE_SIZE_H, CONFIG_SCENE_SIZE_W, CV_8UC1);
  //memcpy(m_image.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data, buffer.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data, CONFIG_SCENE_SIZE_W * CONFIG_SCENE_SIZE_H);
  
  if (m_image.empty()) return false;

  SaveHistory();
  return true;
}
