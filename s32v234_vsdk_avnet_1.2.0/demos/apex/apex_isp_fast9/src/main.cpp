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
#include <APU_FAST9_COLOR.hpp>
#include <umat.hpp>
#include <apex.h>

// If we are in Linux, we need signal for ctrl+c termination
#ifndef __STANDALONE__
#include <signal.h>
#endif // #ifdef __STANDALONE__

#include "isp_cam_sony.h"
#include <common_time_measure.h>
#include <common_helpers.h>
#include <string>

///////////////////////////////
// SDI input
#include "sdi.hpp"
#include "mipi_simple_c.h"

///////////////////////////////
// DCU output
#ifdef __STANDALONE__
#include "frame_output_dcu.h"
#define CHNL_CNT io::IO_DATA_CH3
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"
#define CHNL_CNT io::IO_DATA_CH3
#endif // else from #ifdef __STANDALONE__

// Possible to set input resolution (must be supported by the DCU)
#define WIDTH 1280       ///< width of DDR buffer in pixels
#define HEIGHT 720       ///< height of DDR buffer in pixels
#define DDR_BUFFER_CNT 3 ///< number of DDR buffers per ISP stream
#define DDR_BUF_SIZE WIDTH* HEIGHT* DDR_BUFFER_CNT

///////////////////////////////
// Ctrl+c signal for correct SDI shutdown
#ifndef __STANDALONE__
void        SigintHandler(int aSigNo);
int32_t     SigintSetup(void);
static bool sStop = false; ///< to signal Ctrl+c from command line
#endif

///////////////////////////////
// Main - starts grabbing the frames, executes the FAST9 on APEX for each frame
int main(int argc, char** argv)
{
  std::string helpMsg = std::string("Computes APEX Fast9 corners on the frames grabbed by the camera.\n\tUsage: ") +
                        COMMON_ExtractProgramName(argv[0]);
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  int                 lRetVal = 0;
  sdi_grabber         lGrabber;
  sdi_FdmaIO*         lpFdma;
  SDI_ImageDescriptor lFrmDesc;
  APU_FAST9_COLOR     process;
  // Fetched frame buffer storage
  SDI_Frame       lFrame;
  SONY_Geometry_t lGeo;
  vsdk::UMat      dataThreshold;
  vsdk::UMat      dataMarkColorChannel;
  vsdk::UMat      dataOut;
  // OAL and ACF init
  APEX_Init();

  // DCU Output init
#ifdef __STANDALONE__
  io::FrameOutputDCU lDcuOutput(WIDTH, HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT);
#else
  io::FrameOutputV234Fb lDcuOutput(WIDTH, HEIGHT, io::IO_DATA_DEPTH_08, CHNL_CNT);

  // setup Ctrl+C handler
  if(SigintSetup() != SEQ_LIB_SUCCESS)
  {
    printf("Failed to register Ctrl+C signal handler.");
    lRetVal = -1;
    goto END;
  }

  printf("Press Ctrl+C to terminate the demo.\n");
#endif

  // Initialize SDI input
  //
  sdi::Initialize(0);

  // Create grabber
  lGrabber.ProcessSet(gpGraph_mipi_simple, &gGraphMetadata_mipi_simple,
                      kmem_mipi_simple_srec, sequencer_mipi_simple_srec);

  // Prepare IOs
  lpFdma = (sdi_FdmaIO*)lGrabber.IoGet(SEQ_OTHRIX_FDMA);

  // Modify DDR frame geometry to fit display output
  lFrmDesc = SDI_ImageDescriptor(WIDTH, HEIGHT, RGB888);
  lpFdma->DdrBufferDescSet(0, lFrmDesc);

  // Allocate DDR buffers
  lpFdma->DdrBuffersAlloc(0, DDR_BUFFER_CNT);

  // Prestart grabber
  lGrabber.PreStart();

  // Modify camera geometry
  lGeo.mCsiIdx = CSI_IDX_0;
  SONY_GeometryGet(lGeo);
  lGeo.mVerFlip = 1;
  lGeo.mHorFlip = 1;
  SONY_GeometrySet(lGeo);

  // Start grabbing
  lGrabber.Start();

  // Data alloc + init for FAST 9
  dataThreshold        = vsdk::UMat(1, 1, VSDK_CV_8UC1);          // threshold parameter
  dataMarkColorChannel = vsdk::UMat(1, 1, VSDK_CV_8UC1);          // mark color channel parameter
  dataOut              = vsdk::UMat(HEIGHT, WIDTH, VSDK_CV_8UC3); // data out buffer

  // Set up the parameters
  dataThreshold.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).at< uint8_t >(0)        = 10;
  dataMarkColorChannel.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED).at< uint8_t >(0) = 1;

  // Init APEX process and connect the ports

  lRetVal = 0;
  lRetVal |= process.Initialize();
  lRetVal |= process.ConnectIO("THRESHOLD", dataThreshold);
  lRetVal |= process.ConnectIO("MARK_COLOR_CHANNEL", dataMarkColorChannel);
  lRetVal |= process.ConnectIO("OUTPUT", dataOut);

  if(0 != lRetVal)
  {
    goto END;
  }

  // Grabbing loop (infinite until ctrl+c)
  while(1)
  {
    // get the frame from queue
    lFrame = lGrabber.FramePop();

    // check if frame is ok
    if(lFrame.mUMat.empty())
    {
      break;
    }

    // Need to connect because of buffer change
    lRetVal |= process.ConnectIO("INPUT", lFrame.mUMat);

    int ApuRuntimeStart = FSL_Ticks();

    // execute on APEX
    lRetVal |= process.Start();
    lRetVal |= process.Wait();

    int ApuRuntimeStop = FSL_Ticks();

    printf("Frame %.6f sec.\n", (float)FSL_TicksToSeconds(ApuRuntimeStop - ApuRuntimeStart));

    // output the image
    lDcuOutput.PutFrame(dataOut);

    // Frame push back because of synchronization
    if(lGrabber.FramePush(lFrame) != LIB_SUCCESS)
    {
      break;
    }

#ifndef __STANDALONE__
    if(sStop) // if Ctrl+C
    {
      break;
    }
#endif //#ifndef __STANDALONE__
  }

  // Stop ISP processing
  lGrabber.Stop();

  // clean up grabber resources
  lGrabber.Release();

END:
  if(0 != lRetVal)
  {
    printf("Program Ended Error 0x%X [ERROR]\n", lRetVal);
  }
  else
  {
    printf("Program Ended [SUCCESS]\n");
  }

#ifdef __STANDALONE__
  for(;;)
    ; // Don't return on standalone
#endif

  return lRetVal;
} // main()

#ifndef __STANDALONE__
void SigintHandler(int /*aSigNo*/)
{
  sStop = true;
} // SigintHandler()

int32_t SigintSetup()
{
  int32_t lRet = SEQ_LIB_SUCCESS;

  // prepare internal signal handler
  struct sigaction lSa;
  memset(&lSa, 0, sizeof(lSa));
  lSa.sa_handler = SigintHandler;

  if(sigaction(SIGINT, &lSa, NULL) != 0)
  {
    printf("Failed to register signal handler.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if signal not registered

  return lRet;
} // SigintSetup()

#endif // #ifndef __STANDALONE__
