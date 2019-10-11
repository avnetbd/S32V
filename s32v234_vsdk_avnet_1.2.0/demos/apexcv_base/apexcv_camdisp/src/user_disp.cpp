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

#include "user_disp.hpp"

#ifdef __STANDALONE__
#include "frame_output_dcu.h"
#define CHNL_CNT io::IO_DATA_CH3
#else // #ifdef __STANDALONE__
#ifdef WIN32
#include <opencv2/opencv.hpp>
#else
#include "frame_output_v234fb.h"
#define CHNL_CNT io::IO_DATA_CH3
#endif
#endif // else from #ifdef __STANDALONE__

#include "vdb_log.h"

#ifndef __STANDALONE__
#include <signal.h>
#include <cstring>
#endif

extern user_utils::UserEvent g_myEvent;

namespace user_utils
{
// System Display Section
// Handle to realize display function
// All platforms have different way
#ifdef __STANDALONE__
io::FrameOutputDCU* gFrameOut = nullptr;
#else
#ifdef WIN32
// Create a class for Win32 execusion using openCV
class FrameOutputOCV
{
public:
  void PutFrame(const vsdk::UMat& aImage)
  {
    // display image in a window
    cv::imshow("UserDisp", (cv::UMat)aImage);
    // waitkey is required to display image, the function also allows to capture key press
    int key = cv::waitKey(1);
    // if a key is pressed, send signal to event, will end program
    if(key != -1)
    {
      g_myEvent.Signal();
    }
  }

private:
};
FrameOutputOCV* gFrameOut = nullptr;

#else
io::FrameOutputV234Fb* gFrameOut = nullptr;
#endif
#endif

// Event section
///< to signal Ctrl+c from command line
bool UserEvent::sStop = false;

void UserEvent::Signal(int signo)
{
#ifndef __STANDALONE__
  UserEvent::sStop = (SIGINT == signo);
#endif
}

void UserEvent::Signal()
{
  sStop = true;
}

bool UserEvent::IsSignaled()
{
  return sStop;
}

UserEvent::~UserEvent()
{
#ifndef __STANDALONE__
#ifndef WIN32
  //    sigaction(SIGINT, SIG_DFL, nullptr);
  signal(SIGINT, SIG_DFL);
#endif
#endif
}

UserEvent::UserEvent()
{
#ifndef __STANDALONE__
#ifndef WIN32
  // prepare internal signal handler
  struct sigaction lSa;
  memset(&lSa, 0, sizeof(lSa));
  lSa.sa_handler = UserEvent::Signal;

  if(sigaction(SIGINT, &lSa, nullptr) != 0)
  {
    VDB_LOG_ERROR("Failed to register signal handler.\n");
  } // if signal not registered
  else
  {
    // setup Ctrl+C handler
    printf("Press Ctrl+C to terminate the demo.\n");
  }
#endif
#endif
} // SigintSetup()

class UserDisp::PrivateImplementation
{
public:
  vsdk::Rect mDimensions;
  bool       mInitialized;
  vsdk::UMat mColorImage;
#ifndef __STANDALONE__
  static const int DISP_INT_BUF_COUNT = 1; // no flicker, in linux
#else
  static const int DISP_INT_BUF_COUNT = 3; // needed in sa, as no frame buffer on display
#endif
  int_fast8_t mIntBufIndex;
  vsdk::UMat  mPreImage[DISP_INT_BUF_COUNT];
  bool        mbPreShow;

  PrivateImplementation() : mInitialized(false), mbPreShow(false), mIntBufIndex(0){};
};

// User Display Section
UserDisp::UserDisp() : mBackgnd(cv::Scalar(0, 0, 0))
{
  mpImpl = new UserDisp::PrivateImplementation();
}

UserDisp::~UserDisp()
{
  delete gFrameOut;
  if(nullptr != mpImpl)
  {
    delete mpImpl;
  }
}

int UserDisp::Initialize()
{
  vsdk::Rect lDimensions;
  return Initialize(lDimensions);
}

void UserDisp::GetDimensions(vsdk::Rect& aDimensions)
{
  if(nullptr != mpImpl)
  {
    aDimensions = mpImpl->mDimensions;
  }
}

int UserDisp::Initialize(const vsdk::Rect& aDimensions)
{
  int lRetVal = 1;
  if(nullptr != mpImpl)
  {
    // verify dimensions
    mpImpl->mDimensions        = aDimensions;
    mpImpl->mDimensions.width  = mpImpl->mDimensions.width + mpImpl->mDimensions.x;
    mpImpl->mDimensions.height = mpImpl->mDimensions.height + mpImpl->mDimensions.y;

    if(aDimensions.height <= 0)
    {
      mpImpl->mDimensions.height = MAX_DISP_HEIGHT;
    }
    if(aDimensions.width <= 0)
    {
      mpImpl->mDimensions.width = MAX_DISP_WIDTH;
    }

    if(MAX_DISP_WIDTH < mpImpl->mDimensions.width)
      mpImpl->mDimensions.width = MAX_DISP_WIDTH;
    if(MAX_DISP_HEIGHT < mpImpl->mDimensions.height)
      mpImpl->mDimensions.height = MAX_DISP_HEIGHT;

      //*** Init DCU Output / Display HW ***
#ifdef __STANDALONE__
    gFrameOut =
        new io::FrameOutputDCU(mpImpl->mDimensions.width, mpImpl->mDimensions.height, io::IO_DATA_DEPTH_08, CHNL_CNT);
#else
#ifdef WIN32
    gFrameOut = new FrameOutputOCV();
#else
    gFrameOut = new io::FrameOutputV234Fb(mpImpl->mDimensions.width, mpImpl->mDimensions.height, io::IO_DATA_DEPTH_08,
                                          CHNL_CNT);
#endif
#endif

    // Initialize image buffers
    mpImpl->mColorImage = vsdk::UMat(mpImpl->mDimensions.height, mpImpl->mDimensions.width, VSDK_CV_8UC3);
    for(int i = 0; i < mpImpl->DISP_INT_BUF_COUNT; i++)
    {
      mpImpl->mPreImage[i] = vsdk::UMat(mpImpl->mDimensions.height, mpImpl->mDimensions.width, VSDK_CV_8UC3);
    }
    ((cv::UMat)mpImpl->mPreImage[mpImpl->mIntBufIndex]) = mBackgnd;
    mpImpl->mInitialized                                = true;
    printf("UserDisp Initialized (%d x %d)\n", mpImpl->mDimensions.width, mpImpl->mDimensions.height);
    lRetVal = 0;
  }
  return lRetVal;
}

int UserDisp::Show(const vsdk::UMat& aImage)
{
  return FinalShow(0, 0, aImage);
}

int UserDisp::PreShow(const int_fast16_t aX, const int_fast16_t aY, const vsdk::UMat& aImage)
{
  int lRetVal = 1;
  if(nullptr != mpImpl)
  {
    mpImpl->mbPreShow = true;
    lRetVal           = FinalShow(aX, aY, aImage);
    mpImpl->mbPreShow = false;
  }
  return lRetVal;
}

int UserDisp::FinalShow(const int_fast16_t aX, const int_fast16_t aY, const vsdk::UMat& aImage)
{
  int lRetVal = -1;
  // Make sure initialization was performed
  if(nullptr != mpImpl)
  {
    if(!mpImpl->mInitialized)
    {
      vsdk::Rect lDimensions(aX, aY, aImage.cols, aImage.rows);
      Initialize(lDimensions);
    }

    // If image to display is same dimension as the screen
    if((aImage.cols == mpImpl->mDimensions.width) && (aImage.rows == mpImpl->mDimensions.height))
    {
      switch(aImage.type())
      {
        case VSDK_CV_8UC1:
        {
          if(mpImpl->mbPreShow)
          {
            cv::cvtColor((cv::UMat)aImage, (cv::UMat)mpImpl->mPreImage[mpImpl->mIntBufIndex], cv::COLOR_GRAY2BGR);
          }
          else
          {
            // convert to RGB
            cv::cvtColor((cv::UMat)aImage, (cv::UMat)mpImpl->mColorImage, cv::COLOR_GRAY2BGR);
            gFrameOut->PutFrame(mpImpl->mColorImage);
          }
          lRetVal = 0;
        }
        break;
        case VSDK_CV_8UC3:
        {
          // direct display
          if(mpImpl->mbPreShow)
          {
            cv::Mat srcMat = (cv::Mat)aImage.getMat(OAL_USAGE_CACHED | vsdk::ACCESS_RW);
            cv::Mat dstMat =
                (cv::Mat)mpImpl->mPreImage[mpImpl->mIntBufIndex].getMat(OAL_USAGE_CACHED | vsdk::ACCESS_RW);
            srcMat.copyTo(dstMat);
          }
          else
          {
            gFrameOut->PutFrame(aImage);
          }
          lRetVal = 0;
        }
        break;
        default:
        {
          // not supported
          printf("image format not supported, %s (%d)\n", __FILE__, __LINE__);
          lRetVal = 1;
        }
        break;
      }
    }
    else
    { // Image to display has different size, update a portion of the screen
      switch(aImage.type())
      {
        case VSDK_CV_8UC1:
        {
          // prepare a sub-image to avoid creation of a new image
          vsdk::UMat lColorImage = vsdk::UMat(mpImpl->mColorImage, vsdk::Rect(0, 0, aImage.cols, aImage.rows));
          // convert to RGB
          cv::cvtColor((cv::UMat)aImage, (cv::UMat)lColorImage, cv::COLOR_GRAY2BGR);
          {
            // copy content to display buffer
            cv::Mat    srcMat = (cv::Mat)lColorImage.getMat(OAL_USAGE_CACHED | vsdk::ACCESS_RW);
            vsdk::UMat tmpUMat =
                vsdk::UMat(mpImpl->mPreImage[mpImpl->mIntBufIndex], vsdk::Rect(aX, aY, aImage.cols, aImage.rows));
            cv::Mat dstMat = (cv::Mat)tmpUMat.getMat(OAL_USAGE_CACHED | vsdk::ACCESS_RW);
            srcMat.copyTo(dstMat);
          }
          if(!mpImpl->mbPreShow)
          {
            // put on the screen
            gFrameOut->PutFrame(mpImpl->mPreImage[mpImpl->mIntBufIndex]);
          }
          lRetVal = 0;
        }
        break;
        case VSDK_CV_8UC3:
        {
          // direct display
          {
            cv::Mat srcMat = (cv::Mat)aImage.getMat(OAL_USAGE_CACHED | vsdk::ACCESS_RW);
            // get a sub-image to update
            vsdk::UMat tmpUMat =
                vsdk::UMat(mpImpl->mPreImage[mpImpl->mIntBufIndex], vsdk::Rect(aX, aY, aImage.cols, aImage.rows));
            cv::Mat dstMat = (cv::Mat)tmpUMat.getMat(OAL_USAGE_CACHED | vsdk::ACCESS_RW);
            // copy content to display buffer
            srcMat.copyTo(dstMat);
          }
          if(!mpImpl->mbPreShow)
          {
            // put on the screen
            gFrameOut->PutFrame(mpImpl->mPreImage[mpImpl->mIntBufIndex]);
          }
          lRetVal = 0;
        }
        break;
        default:
        {
          // not supported
          printf("image format not supported, %s (%d)\n", __FILE__, __LINE__);
          lRetVal = 1;
        }
        break;
      }
    }

    if(!mpImpl->mbPreShow)
    {
      if(++mpImpl->mIntBufIndex >= mpImpl->DISP_INT_BUF_COUNT)
      {
        mpImpl->mIntBufIndex = 0;
      }
      // empty buffer with background color
      ((cv::UMat)mpImpl->mPreImage[mpImpl->mIntBufIndex]) = mBackgnd;
    }
  }
  return lRetVal;
}
}
