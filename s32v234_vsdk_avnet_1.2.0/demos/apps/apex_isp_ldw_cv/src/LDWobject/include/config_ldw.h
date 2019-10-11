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
#ifndef CONFIGLDW_H
#define CONFIGLDW_H

#include <opencv2/opencv.hpp>

#ifdef __INTEGRITY__
  #define INPUT_ROOT "/apex/"
#else
  #define INPUT_ROOT ""
#endif

//namespace config

	////////////////////////////////////////////////////////////////////////
	// Input file from front camera (BGR video)
	const std::string 	input_file = 			INPUT_ROOT"mvi_0049.avi";
	
	// Output file (BGR video)
	const std::string 	output_file = 			INPUT_ROOT"output.avi";
	
	// Input file size
	const cv::Size 		scene_size(640, 480);
	#define CONFIG_SCENE_SIZE_W 640
	#define CONFIG_SCENE_SIZE_H 360
        
        #define CONFIG_SCENE_POINT_0_0 176
        #define CONFIG_SCENE_POINT_0_1 167
        #define CONFIG_SCENE_POINT_1_0 396
        #define CONFIG_SCENE_POINT_1_1 167
        #define CONFIG_SCENE_POINT_2_0 10
        #define CONFIG_SCENE_POINT_2_1 218
        #define CONFIG_SCENE_POINT_3_0 502
        #define CONFIG_SCENE_POINT_3_1 228

	////////////////////////////////////////////////////////////////////////
	// Bird eye view size
	#define CONFIG_BIRD_SIZE_W 192
	#define CONFIG_BIRD_SIZE_H 320
	
	// Bird eye view chunk size
	#define CONFIG_BIRD_CHUNK_SIZE_W 6
	#define CONFIG_BIRD_CHUNK_SIZE_H 8
	
	
	// Bird eye view coordinates corresponding to the searched region
	// points (scene_points)
	#define CONFIG_BIRD_POINT_0_0 0
	#define CONFIG_BIRD_POINT_0_1 0
	#define CONFIG_BIRD_POINT_1_0 192
	#define CONFIG_BIRD_POINT_1_1 0
	#define CONFIG_BIRD_POINT_2_0 0
	#define CONFIG_BIRD_POINT_2_1 320
	#define CONFIG_BIRD_POINT_3_0 192
	#define CONFIG_BIRD_POINT_3_1 320
				
	// Bird eye view width
	#define CONFIG_BIRD_WIDTH 192
	
	// Bird eye view height
	#define CONFIG_BIRD_HEIGHT 320
	
	////////////////////////////////////////////////////////////////////////
	// Amount of frames from which maximum frame is computed
	// (dashed lines and noise filtering)
	#define CONFIG_HISTORY 10
	
	// Maximum Y coordinate to be considered on bird eye view
	#define CONFIG_Y_MAX_POS 140
	//#define CONFIG_Y_MAX_POS 200

	// Kalman filter measurement noise
	#define CONFIG_KALMAN_MEASUREMENT_NOISE   0.05
	
	// Kalman filter post error
	#define CONFIG_KALMAN_POST_ERROR    0.005
	
	// Kalman filter processNoiseVar (for this demo it can be considered like speed of reaction)
        #define CONFIG_KALMAN_PROCESS_NOISE_VAR    0.0007	
        
        // Priority Curve definition to xCOO use values between 0 - 100 %
        #define CONFIG_CURVE_POINT_0_XCOO               0
        #define CONFIG_CURVE_POINT_0_XCOO_PRIORITY      8
        
        #define CONFIG_CURVE_POINT_1_XCOO               15
        #define CONFIG_CURVE_POINT_1_XCOO_PRIORITY      12
        
        #define CONFIG_CURVE_POINT_2_XCOO               30
        #define CONFIG_CURVE_POINT_2_XCOO_PRIORITY      12
        
        #define CONFIG_CURVE_POINT_3_XCOO               40
        #define CONFIG_CURVE_POINT_3_XCOO_PRIORITY      8
        
        #define CONFIG_CURVE_POINT_4_XCOO               70
        #define CONFIG_CURVE_POINT_4_XCOO_PRIORITY      8
        
        #define CONFIG_CURVE_POINT_5_XCOO               72
        #define CONFIG_CURVE_POINT_5_XCOO_PRIORITY      12
        
        #define CONFIG_CURVE_POINT_6_XCOO               90
        #define CONFIG_CURVE_POINT_6_XCOO_PRIORITY      12
        
        #define CONFIG_CURVE_POINT_7_XCOO               100
        #define CONFIG_CURVE_POINT_7_XCOO_PRIORITY      8
        
        // Warning triggers
        #define CONFIG_RIGHT_MIN                        85
        #define CONFIG_RIGHT_MAX                        89
        
        #define CONFIG_LEFT_MIN                         20
        #define CONFIG_LEFT_MAX                         27
        
        #define CONFIG_MAX_DIFF                         18
        #define CONFIG_MAX_COUNTER_VAL                  30

#endif /* CONFIGLDW_H */
    