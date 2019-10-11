/*
  Copyright (c) 2016-2018 NXP Semiconductors
  All Rights Reserved.
  
  This file contains sample code only.
  
  THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*/

/*==================================================================================================
                                         INCLUDE FILES
==================================================================================================*/
#include <sys/time.h>
#include "video_app.h"
#include "video_app_cfg.h"

/* Related to VSDK */
#include "frame_output_v234fb.h"
#include "sdi.hpp"
#include "vdb_log.h"

#if(DECODER_JPEG == APP_CFG_DECODER_VARIANT)
#if(FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
#include "jpeg_4stream_c.h"
#else
#error Unsupported stream variant (JPEG)
#endif /* APP_CFG_STREAM_VARIANT */
#include "dec_jpeg_hl.h"
#elif(DECODER_H264 == APP_CFG_DECODER_VARIANT)
#if(SINGLE_STREAM == APP_CFG_STREAM_VARIANT)
#include "h264dec_single_stream_c.h"
#elif(FOUR_STREAMS == APP_CFG_STREAM_VARIANT)
#include "h264dec_4stream_c.h"
#else
#error Unsupported stream variant (H264)
#endif /* APP_CFG_STREAM_VARIANT */
#include "dec_h264_hl.h"
#else
#error Unsupported decoder variant
#endif /* APP_CFG_DECODER_VARIANT */

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
/*  Application configuration */
// Possible to set input resolution (must be supported by the DCU)
#if(DECODER_JPEG == APP_CFG_DECODER_VARIANT)
#define WIDTH 1280 ///< width of DDR buffer in pixels
#define HEIGHT 800 ///< height of DDR buffer in pixels
#elif(DECODER_H264 == APP_CFG_DECODER_VARIANT)
#define WIDTH 1280 ///< width of DDR buffer in pixels
#define HEIGHT 960 ///< height of DDR buffer in pixels
#endif             /* APP_CFG_DECODER_VARIANT */

#define FRM_TIME_MSR 300 ///< number of frames to measure the time and fps

#define GETTIME(time)                                                                                                  \
  \
{                                                                                                                   \
    struct timeval lTime;                                                                                              \
    gettimeofday(&lTime, 0);                                                                                           \
    *time = (lTime.tv_sec * 1000000 + lTime.tv_usec);                                                                  \
  \
}

/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/
static sdi_grabber*  lpGrabber;
static bool          bInitialized   = false;
static unsigned int  u32FramesCount = 0U;
static unsigned long lTimeStart = 0, lTimeEnd = 0, lTimeDiff = 0;

/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/
/*
*   Body of the video application. Intended to be periodically called within a thread.
*/
int video_app_body(void)
{
  int RetVal = -1;

  if(false == bInitialized)
  {
    return -1;
  }
  else
  {
#if(DECODER_JPEG == APP_CFG_DECODER_VARIANT)
    RetVal = dec_jpeg_process_frame(lpGrabber);
#elif(DECODER_H264 == APP_CFG_DECODER_VARIANT)
    RetVal = dec_h264_process_frame(lpGrabber);
#endif /* APP_CFG_DECODER_VARIANT */

    if(0 == RetVal)
    {
      if(0U == lTimeDiff)
      {
        GETTIME(&lTimeStart);
        u32FramesCount++;
        lTimeDiff = 1;
      }
      else if((++u32FramesCount % FRM_TIME_MSR) == 0)
      {
        GETTIME(&lTimeEnd);
        lTimeDiff  = lTimeEnd - lTimeStart;
        lTimeStart = lTimeEnd;

        printf("[video_app] %u frames took %lu usec (%5.2ffps)\n", FRM_TIME_MSR, lTimeDiff,
               (FRM_TIME_MSR * 1000000.0) / ((float)lTimeDiff));
      }
      else
      {
        ;
      }
    }
    else if(-2 == RetVal)
    {
      ; /* NOP - frame not decoded yet */
    }
    else
    {
      return RetVal;
    }
  }

  return 0;
}

/*
*   Start of the video application and all underlying Vision components.
*/
int video_app_start(void)
{
  if(false == bInitialized)
  {
    printf("[video_app] Not initialized\n");
    return -1;
  }
  else
  {
  /*
            Start grabbing.
        */
#if(DECODER_JPEG == APP_CFG_DECODER_VARIANT)
    if(LIB_SUCCESS != lpGrabber->Start(0, HEIGHT * 4))
#elif(DECODER_H264 == APP_CFG_DECODER_VARIANT)
    if(LIB_SUCCESS != lpGrabber->Start(0, HEIGHT))
#endif /* APP_CFG_DECODER_VARIANT */
    {
      printf("[video_app] lpGrabber->Start failed\n");
      return -1;
    }
    /* Success */
    u32FramesCount = 0U;
    printf("[video_app] Started\n");
  }

  return 0;
}

/*
*   Stop of the video application and all underlying Vision components. Also destroys internal
*   objects and releases memory.
*/
int video_app_stop(void)
{
  LIB_RESULT lRet = LIB_SUCCESS;

  if(false == bInitialized)
  {
    return -1;
  }
  else
  {
#if(DECODER_H264 == APP_CFG_DECODER_VARIANT)
    if(0 != dec_h264_shutdown())
    {
      printf("[video_app] Can't shut the application down\n");
    }
#endif /* APP_CFG_DECODER_VARIANT */

    /*
            Stop ISP processing.
        */
    lpGrabber->Stop();

    /*
            Close SDI.
        */
    lRet = sdi::Close(0);
    if(lRet != LIB_SUCCESS)
    {
      return -1;
    }

    lpGrabber->Release();
    delete lpGrabber;
  }

  bInitialized = false;
  printf("[video_app] Stopped\n");

  return 0;
}

/*
*   Abort capturing decoder output.
*/
void video_app_abort_capture(bool shall_abort)
{
  dec_abort_reading = shall_abort;
}

/*
*   Read asynchronous error status.
*/
bool video_app_get_async_err(void)
{
  return async_error;
}

/*
*   Clear asynchronous error status.
*/
void video_app_clear_async_err(void)
{
  async_error = false;
}

/*
*   Initialization of the video application.
*/
int video_app_init(void)
{
  /*
        Initialize SDI.
    */
  if(LIB_SUCCESS != sdi::Initialize(0))
  {
    printf("[video_app] sdi::Initialize failed\n");
    return -1;
  }

  /*
        Create the grabber.
    */
  lpGrabber = new(sdi_grabber);
  if(NULL == lpGrabber)
  {
    printf("[video_app] lpGrabber constructor failed\n");
    return -1;
  }
  else
  {
    /* Success */
    ;
  }

  /*
        Set the IPS graph.
    */
  if(LIB_SUCCESS != lpGrabber->ProcessSet(gpGraph, &gGraphMetadata))
  {
    printf("[video_app] sdi::Initialize failed\n");
    return -1;
  }

  /*  
        This is the decoder and application variant -specific part. There shall be distinguished
        which xxx_prepare_ios() will be called.
    */
  printf("[video_app] Preparing stream input variant...\n");
#if(DECODER_JPEG == APP_CFG_DECODER_VARIANT)
  if(0 != dec_jpeg_prepare_ios(lpGrabber, WIDTH, HEIGHT))
  {
    printf("[video_app] dec_jpeg_prepare_ios() failed\n");
    return -1;
  }
#elif(DECODER_H264 == APP_CFG_DECODER_VARIANT)
  if(0 != dec_h264_prepare_ios(lpGrabber, WIDTH, HEIGHT))
  {
    printf("[video_app] dec_h264_prepare_ios() failed\n");
    return -1;
  }
#endif /* APP_CFG_DECODER_VARIANT */

  /* Success */
  bInitialized = true;

  /* Pre-start the grabber */
  if(LIB_SUCCESS != lpGrabber->PreStart())
  {
    printf("[video_app] lpGrabber->PreStart failed\n");
    return -1;
  }

  return 0;
}
