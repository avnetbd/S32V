/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2017 NXP
* All Rights Reserved
*
*****************************************************************************
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

/****************************************************************************
* Includes
****************************************************************************/
#include "user_cam.hpp"
#include <cstdint>

#ifndef WIN32
#include "sdi.hpp"
#else
#include <opencv2/opencv.hpp>

class sdi_grabber
{
public:
  sdi_grabber();
  ~sdi_grabber();

private:
};
#endif

#ifndef __STANDALONE__
#include <signal.h>
#endif // #ifdef __STANDALONE__

#ifndef WIN32
#include "mipi_simple_c.h"
#endif

// Constant definition
const int cL                = 100; // size of the square in TENNIS mode
const int NB_APP_BUFFER_ISP = 3;
// Possible to set input resolution (must be supported by the DCU)
const int DDR_BUFFER_CNT = 2 + NB_APP_BUFFER_ISP; ///< number of DDR buffers per ISP stream
// Local Global variable
volatile int_fast32_t g_sUserCamCallbackData;

namespace user_utils
{
#ifndef WIN32
/************************************************************************/
/** User defined call-back function for Sequencer event handling.
  *
  * \param  aEventType defines Sequencer event type
  * \param  apUserVal  pointer to any user defined object
  ************************************************************************/
void SeqEventCallBack(uint32_t aEventType, void* apUserVal);

// There could be only one grabber
static sdi_grabber* lpGrabber = (sdi_grabber*)nullptr;
#endif

class UserCam::PrivateImplementation
{
public:
  UserCam::MODE mMode;
  bool          mbInitialized;
  bool          mbStarted;
  vsdk::Rect    mDimensions;

  PrivateImplementation() : mbInitialized(false), mbStarted(false)
  {
    mDimensions.width  = UserCam::CAM_WIDTH;
    mDimensions.height = UserCam::CAM_HEIGHT;
  };

  // initialization of the hardware
  int_fast32_t sdi_Initialize(const vsdk::Rect& aDimension)
  {
#ifndef WIN32
      LIB_RESULT lRes = LIB_SUCCESS;
      if (MODE::DEFAULT == mMode)
      {

        // *** Initialize SDI ***
        lRes = sdi::Initialize(0);
        // *** create grabber ***
        lpGrabber = new(sdi_grabber);
        lpGrabber->ProcessSet(gpGraph_mipi_simple, &gGraphMetadata_mipi_simple,
                              kmem_mipi_simple_srec, sequencer_mipi_simple_srec);

      // *** set user defined Sequencer event handler ***
      g_sUserCamCallbackData = -1;

      lpGrabber->SeqEventCallBackInstall(&SeqEventCallBack, (void*)&g_sUserCamCallbackData);

      // *** prepare IOs ***
      sdi_FdmaIO* lpFdma = (sdi_FdmaIO*)lpGrabber->IoGet(SEQ_OTHRIX_FDMA);

      // modify DDR frame geometry to fit display output
      SDI_ImageDescriptor lFrmDesc = SDI_ImageDescriptor(aDimension.width, aDimension.height, RGB888);
      lpFdma->DdrBufferDescSet(0, lFrmDesc);

      //*** allocate DDR buffers ***
      lpFdma->DdrBuffersAlloc(0, DDR_BUFFER_CNT);

      // *** prestart grabber ***
      lpGrabber->PreStart();
      // wait a bit before continuing
      volatile int temp = 0;
      for(int i = 1; i < 100000000; i++)
      {
        if((i % 10000000) == 0)
          printf(".");
      }
      printf("\n");
    }
    return lRes;
#else
    return 0;
#endif
  };
};

// Initialize object at creation
UserCam::UserCam() : mpImpl(nullptr)
{
  mpImpl = new PrivateImplementation();
}

// Properly cleanup object on destroy
UserCam::~UserCam()
{
#ifndef WIN32
  if(nullptr != mpImpl)
  {
    if(MODE::DEFAULT == mpImpl->mMode)
    {
      if(lpGrabber)
      {
        //*** Stop ISP processing ***
        lpGrabber->Stop();

        // clean up grabber resources
        lpGrabber->Release();

        delete(lpGrabber);
      } // if grabber exists

#ifdef __STANDALONE__
      for(;;)
        ; // *** don't return ***
#endif    // #ifdef __STANDALONE__

      sdi::Close(0);
    }
  }
#endif
  if(nullptr != mpImpl)
  {
    delete mpImpl;
  }
}

// initialization handling of the hardware
// User may call this function to force initialization to happen
// before calling GetImage()
int UserCam::Initialize(const MODE aMode)
{
  int lRetVal = 1;
  if(nullptr == mpImpl)
  {
    lRetVal = 3;
    goto EXIT;
  }
  if(mpImpl->mbInitialized)
  {
    lRetVal = 2;
    goto EXIT;
  }
  mpImpl->mMode = aMode;
#ifdef WIN32
  if(MODE::DEFAULT == mpImpl->mMode)
  {
    mpImpl->mMode = MODE::TENNIS;
  }
#endif
  mpImpl->sdi_Initialize(mpImpl->mDimensions);
  mpImpl->mbInitialized = true;
  lRetVal               = 0;

EXIT:
  return lRetVal;
}

// Function to access image acquired by sensor
int UserCam::GetImage(vsdk::UMat& aImage)
{
  int lRetVal = 1;

  // Verify that object is valid
  if(nullptr == mpImpl)
  {
    lRetVal = 3;
    goto EXIT;
  }
  if(!mpImpl->mbInitialized)
  {
    lRetVal = Initialize(MODE::DEFAULT);
    if(0 != lRetVal)
    {
      goto EXIT;
    }
  }

#ifndef WIN32
  if(MODE::DEFAULT == mpImpl->mMode)
  {
    // Start acquisition if not done so yet
    if(!mpImpl->mbStarted)
    {
      // *** start grabbing ***
      lpGrabber->Start();
      mpImpl->mbStarted = true;
    }

    // Get an image from grabber
    {
      SDI_Frame SDIFrame = lpGrabber->FramePop();
      aImage             = vsdk::UMat(SDIFrame.mUMat, mpImpl->mDimensions);

      // Pre-release buffer, will do when UMat will deallocate
      if(lpGrabber->FramePush(SDIFrame) != LIB_SUCCESS)
      {
        printf("FramePush Failed (%s : %d)\n", __FILE__, __LINE__);
      } // if push failed
    }
  }
#endif

  // Code for handling the tennis mode, square moving across the display area
  if(MODE::TENNIS == mpImpl->mMode)
  {
    // build image
    // create image
    vsdk::UMat lNewImage(mpImpl->mDimensions.height, mpImpl->mDimensions.width, VSDK_CV_8UC3);
    // fill the image with a color
    (cv::UMat) lNewImage = cv::Scalar(230, 230, 230);
    // defines the original square position and size
    static vsdk::Rect lr(0, 0, cL, cL);
    // defines a movement, using just x and y
    static vsdk::Rect offset(10, 5, 10, 5);
    {
      // create sub-region for the square in image
      vsdk::UMat lTemp = vsdk::UMat(lNewImage, lr);
      // fill the sub-region with color
      (cv::UMat) lTemp = cv::Scalar(0, 0, 200);
    }
    {
      // prepare position for next image
      lr.x += offset.x;
      if((lr.x < 0) || (lr.x > mpImpl->mDimensions.width - lr.width))
      {
        offset.x = -offset.x;
        lr.x += offset.x;
      }
      lr.y += offset.y;
      if((lr.y < 0) || (lr.y > mpImpl->mDimensions.height - lr.height))
      {
        offset.y = -offset.y;
        lr.y += offset.y;
      }
    }
    aImage = lNewImage;
  }
  lRetVal = 0;
EXIT:
  return lRetVal;
}

#ifndef WIN32
// Callback counting the images received
void SeqEventCallBack(uint32_t aEventType, void* apUserVal)
{
  static int_fast32_t slCounter          = 0;
  int_fast32_t*       psCallbackUserData = (int_fast32_t*)apUserVal;

  if(aEventType == SEQ_MSG_TYPE_FRAMEDONE)
  {
    *psCallbackUserData = ++slCounter;
  } // if frame done arrived
} // SeqEventCallBack()
#endif
}
