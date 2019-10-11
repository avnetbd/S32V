/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2016 Freescale Semiconductor
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
#ifndef CAMERAINPUT_H
#define CAMERAINPUT_H

/** File implements the main setup of ISP camera input (stereo) **/

#include <string.h>
#include "vdb_log.h"
#include "frame_io_types.h"
#include "sdi.hpp"
#include "isp_seq.h"
#include "imx224_exp_ctrl_c.h"
#include "isp_cam_sony.h"
#include "sdi.hpp"
#include "umat.hpp"

#include "iofiles.h"



#ifndef __STANDALONE__
  #include <signal.h>
#endif // #ifdef __STANDALONE__

/***************************************************************************
 * Defines - ISP parameters
 ***************************************************************************/

#define EXPOSURE_ENGINE 1,1
#define TONEHDR_ENGINE  1,1
// from 0-256 : 0 = long exposure, 256 short exposure, 128 50/50

#define GAMMA_ENGINE 1,3

#define SCR_WIDTH       1280       ///< width of DDR buffer in pixels
#define SCR_HEIGHT      720        ///< height of DDR buffer in pixels
#define SRC_WIDTH       1292       ///< width of DDR buffer in pixels
#define DDR_BUFFER_CNT  3          ///< number of DDR buffers per ISP stream
#define SUB_NUM_LINE    360
#define ISP_FPS         30
#ifdef ISP_FRAME_RATE_MEASURE_NUM
#define	NUM_FRAMES      ISP_FRAME_RATE_MEASURE_NUM
#else
#define	NUM_FRAMES      0x1000000  ///< number of frames to grab
#endif

  
/***************************************************************************
 * ISP Input class 
 ***************************************************************************/
class ISPCameraInputStereo
{
  public:
    /************************************************************************/
    /**
     * Constructor - basically creates the object (does not initialize!)
     ************************************************************************/
    ISPCameraInputStereo();
    
    /************************************************************************/
    /**
     * Initialization function - initializes the ISP and prepares everything for run
     * 
     * \return 0 if all ok
     ************************************************************************/
    int32_t Init();
    
    /************************************************************************/
    /**
     * De-Initialization function - Frees all allocated resources
     * 
     * \return 0 if all ok
     ************************************************************************/
    int32_t Deinit();
    
    /************************************************************************/
    /**
     * Start camera grabbing - starts ISP + cameras
     * 
     * \return 0 if all ok
     ************************************************************************/
    int32_t Start();
    
    /************************************************************************/
    /**
     * Stop cameras
     * 
     * \return 0 if all ok
     ************************************************************************/
    int32_t Stop();
    
    /************************************************************************/
    /**
     * Wrapper for SDI FramePop - returns current frame specified by the index
     * 
     * \param aIdx    Index of the frame
     * \param arFrame [out] poped SDI frame
     * \return Cached, non buffered pointer to the buffer
     ************************************************************************/
    vsdk::UMat GetFrame(uint32_t aIdx, SDI_Frame &arFrame);
    
    /************************************************************************/
    /**
     * Wrapper for SDI FramePush - used for synchronization of frames
     * 
     * \param arFrame frame to be pushed back to ISP workqueue.
     ************************************************************************/
    void PutFrame( SDI_Frame &arFrame);
    
    /************************************************************************/
    /** 
     * SIGINT handler.
     * 
     * \param  aSigNo 
     ************************************************************************/
    static void SigintHandler(int aSigNo);

    /************************************************************************/
    /**
     * SIGINT handler.
     * 
     * \param  aSigNo 
     * 
     * \return SEQ_LIB_SUCCESS if all ok
     *         SEQ_LIB_FAILURE if failed
     ************************************************************************/
    static int32_t SigintSetup(void);
    void ExpControl(void);
    
    /************************************************************************/
    /**
     * Static flag for loop continuation
     ************************************************************************/
    static bool sStop;
    
  private:
    
  public:    
    sdi_grabber mGrabber;   ///< class resposible for ISP management and config
    sdi_FdmaIO *mpFdma;     ///< class describin ISP HW engine
};

#endif /* CAMERAINPUT_H */
