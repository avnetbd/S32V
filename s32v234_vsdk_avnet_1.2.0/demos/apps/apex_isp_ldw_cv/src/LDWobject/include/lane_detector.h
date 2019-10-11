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
#ifndef LANEDETECTOR_H
#define LANEDETECTOR_H

#include <opencv2/opencv.hpp>
#include "webserver.hpp"
#include "common_time_measure.h"
#include "transform_lut.h"
#include "kalman_filter.h"
#include "HISTORY_MAXIMUM.hpp"
#include "ANALYSIS.hpp"
#include <icp_data.h>
#include "warp.h"
#include "umat.hpp"

#include <frame_input_file.h>
#include <list>

enum Update_events {none, bird_view, priority_curve, kalman};

////////////////////////////////////////////////////////////////////////////////
/**
 * Class encapsulates detected lane data
 */
class Lane
{
  public:
    /**
     * Left lane points in camera coordinates
     */
    cv::Point2i left[2];
    
    /**
     * Right lane points in camera coordinates
     */
    cv::Point2i right[2];
    
    /**
     * Left lane points in bird eye view coordinates
     */
    cv::Point2i left_bird[2];
    
    /**
     * Right lane points in bird eye view coordinates
     */
    cv::Point2i right_bird[2];

    /**
     * Left lane line equation in bird eye view coordinates
     */
    cv::Vec3d left_eq;
    
    /**
     * Right lane line equation in bird eye view coordinates
     */
    cv::Vec3d right_eq;
    
     /**
     * power of left peak
     */
    int left_peak_power;
    
    /**
     * power of right peak
     */
    int right_peak_power;
    
    /**
     * Constructor
     **/
    Lane();
    
    /**
     * Constructor
     * @param Lane
     **/
    Lane(const Lane &lane);
    
    /**
     * Destructor - frees resources
     */
    ~Lane();
};

////////////////////////////////////////////////////////////////////////////////
/**
 * Class encapsulating main lane detector functions 
 */
class LaneDetector
{
  public:
    /**
     * Constructor
     * Initializes parameters and opens the input file
     * Pre-computes camera parameters.
     * @param filename Input file
     * @param params Detection parameters
     */
    LaneDetector();
    
    /**
     * Constructor
     * Initializes parameters and opens the input file
     * Pre-computes camera parameters.
     * @param filename Input file
     * @param params Detection parameters
     */
    LaneDetector(int apuldx);
    
    /**
     * Destructor - frees resources
     */
    ~LaneDetector();

    /**
     * Function loads next frame from the input file and finds lane position
     * using implemented algorithm.
     * @return True if there is frame read and detected
     */
    bool GetNextFrame(vsdk::UMat buffer);
    
    /**
     * Returns lane information filtered by kalman for current frame
     * @return Detected lane
     */
    Lane GetCurrentLane();

    /**
     * Returns lane information for current frame
     * @return Detected lane
     */
    Lane GetCurrentOfFrameLane(); 

    /**
     * Health of lane 
     */
    bool m_lane_healthy;
    
    /**
     * Difference between midle of bird view and lane middle 
     */
    int m_lane_middle_diff;
    
    bool m_left_warning;
    
    bool m_right_warning;
    
    static Update_events update_event;
    /**
     * Input image Greyscale
     */
    vsdk::UMat m_image;
    
    /**
     * Input bird eye image (edge intensity)
     */
    vsdk::UMat m_bird_image;
    
    /**
     * History maximum from bitd eye images
     */
    vsdk::UMat m_bird_max;
    
//     /**Frame
//      * History maximum from bitd eye images
//      */
//     vsdk::UMat m_bird_max_icp;
    
  private:
    
    int m_up_left_ctr;
    int m_down_left_ctr;
    int m_up_right_ctr;
    int m_down_right_ctr;
    int m_left_warning_drop_ctr;
    int m_right_warning_drop_ctr;
    
    bool m_left_peak_stable;
    bool m_right_peak_stable;
    
    MovingAvg<unsigned long> analysis_times_avg;
    MovingAvg<unsigned long> history_times_avg;
    MovingAvg<unsigned long> findlane_times_avg;
    MovingAvg<unsigned long> birdview_times_avg;
    
    
    char *m_analysis_stream;
    /**
     * APU index
     */
    int m_apuldx;   
    
    /**
     * Auxiliary function used in GetNextFrame
     * Saves current frame, warps bird eye image and computes maximum image
     * from n last bird eye frames
     */
    void SaveHistory();
    
    /**
     * Auxiliary function used in GetNextFrame
     * Calculate position of lanes
     */
    void FindLane();
    
    /**
     * Function to generate priority curve
     */
    void FillPriorityCurve();
    
     /**
     * Auxiliary function used in GetNextFrame
     * Approximates lane 
     */
    void GetApproxLine();
    
    /**
     * Auxiliary function used in GetNextFrame
     * Approximates lane 
     */
    void GetApproxActualLine();

    /**
     * LeftPosition kalman filter
     */
    KalmanFilter LeftPosition_down;
       
    /**
     * RightPosition kalman filter
     */
    KalmanFilter RightPosition_down;
  
    /**
     * LeftPosition kalman filter
     */
    KalmanFilter LeftPosition_up;
       
    /**
     * RightPosition kalman filter
     */
    KalmanFilter RightPosition_up;
    
    /**
     * Curve of priority 
     */
    char *priority_curve;
    
    /**
     * Analysis buffer lower part of birdview
     */
    int *signal_down;
    
    /**
     * Analysis buffer upper part of birdview
     */
    int *signal_up;
    
    /**
     * Transformation bird eye view -> scene 
     */
    Transform_LUT m_transform;
	
    /**
     * Pre-computed lookup lanes for ray casting into bird-eye view - origin
     */
    cv::Point2i m_lookup_line_origin;
    
    /**
     * Pre-computed lookup lanes for ray casting into bird-eye view - ends
     */
    cv::Point2i *m_lookup_line_ends;
    
    /**
     * Actual Left line point x = up y = down 
     */
    cv::Point2i ActualLeftline;
    
    /**
     * Actual Right line point x = up y = down 
     */
    cv::Point2i ActualRightline;

    /**
     * Found contour points (inner road lane contour)
     */
    std::vector<cv::Point2i> m_contour_pts;
    
    /**
     * Filtered right lane points candidates
     */
    std::vector<cv::Point2i> m_line_right_pts;
    
    /**
     * Filtered left lane points candidates
     */
    std::vector<cv::Point2i> m_line_left_pts;
    
    /**
     * Detected and filtered lane for current frame
     */
    Lane m_detected_lane;
    
    /**
     * Detected actual lane for current frame
     */
    Lane m_actual_detected_lane;
	
    /**
     * APEX computed history process
     */
    HISTORY_MAXIMUM m_maximum_process;
    
    /**
     * APEX ldw analysis
     */
    ANALYSIS m_analysis_process;
    
    /**
     * Last n bird eye views in history
     */
    vsdk::UMat *m_bird_history_icp;
    
    /**
     * Pointer to the last value in circular history buffer
     */
    vsdk::UMat *m_bird_history_icp_last;
    
     /**
     * DataDescriptor for init priority down
     */
    vsdk::UMat m_priority_down;
    
    /**
     * DataDescriptor for init priority up
     */
    vsdk::UMat m_priority_up;
    
    /**
     * DataDescriptor for init startTile
     */
    vsdk::UMat m_start_tile;
    
    /**
     * DataDescriptor for signal_down
     */
    vsdk::UMat m_signal_down;
    
     /**
     * DataDescriptor for signal_up
     */
    vsdk::UMat m_signal_up;
    
    /**
     * Offset from array of history images start to the last
     */
    int m_bird_history_offset;
    
    /**
     * Warp bird-eye
     */
    Warp warp;
};


#endif /* LANEDETECTOR_H */
