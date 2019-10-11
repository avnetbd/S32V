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

// ****************************************************************************
// Includes
// ****************************************************************************
#include "user_process.hpp"
#include "user_cam.hpp"
#include "user_disp.hpp"
#include <string>

// Structure with the application settings
class AppConfig
{
public:
  bool                      bCount; // Flag indicating that application will process a number of images before exiting
  bool                      bExit;  // Flag to exit program after command line option analysis
  user_utils::UserCam::MODE lStartMode;        // Start mode for the camera
  uint_fast8_t              lCount;            // Image count before quitting the program, needs to be positive
  vsdk::Rect                lScreenDimensions; // Dimensions used for display
  // Dimension of source image after cropping
  int lCWidth;  // Reduced from 1280
  int lCHeight; // Reduced from 720
  // Initialization of the object
  AppConfig()
      : bCount(false), bExit(false), lStartMode(user_utils::UserCam::DEFAULT), lCount(1), lCWidth(960), lCHeight(640)
  {
    lScreenDimensions = vsdk::Rect(0, 0, user_utils::UserDisp::MAX_DISP_WIDTH, user_utils::UserDisp::MAX_DISP_HEIGHT);
  }
  // Handles the image count, if required
  uint_fast32_t GetCount()
  {
    if(bCount)
    {
      lCount--;
    }
    return lCount;
  }
} g_myAppConfig;

// global event, so termination can happen from HW or from any other place in code
user_utils::UserEvent g_myEvent;

// function to change the application setting from parameters passed on the command line
void ProcessArguments(int argc, char** argv);

#define RUN(a)                                                                                                         \
  if(0 != (lRetVal = a))                                                                                               \
  {                                                                                                                    \
    printf("ERROR %d returned at %d in %s\n", lRetVal, __LINE__, __FILE__);                                            \
    goto END;                                                                                                          \
  }

// ****************************************************************************
// Main function
// ****************************************************************************
#ifndef __STANDALONE__
int main(int argc, char** argv)
{
  // process the options passed at the command line
  ProcessArguments(argc, argv);
#else
int main(int, char**)
{
#endif
  int lRetVal;

  if(!g_myAppConfig.bExit)
  {
    // Create the main objects
    user_utils::UserCam     myCam;
    user_utils::UserProcess myProcess;

    // Prepare the variable to crop sensor image to desired dimensions
    // Offset in the source image for cropping (sensor image size 1280x720)
    int        lCX = (user_utils::UserCam::CAM_WIDTH - g_myAppConfig.lCWidth) / 2;
    int        lCY = (user_utils::UserCam::CAM_HEIGHT - g_myAppConfig.lCHeight) / 2;
    vsdk::Rect lCrop(lCX, lCY, g_myAppConfig.lCWidth, g_myAppConfig.lCHeight);

    // Prepare the Display
    user_utils::UserDisp myDisp;
    RUN(myDisp.Initialize(g_myAppConfig.lScreenDimensions))
    // Retreive the Screen dimensions
    myDisp.GetDimensions(g_myAppConfig.lScreenDimensions);

    // Prepare the variables
    vsdk::UMat lImage1, lImage2, lImage3;
    // start Cam
    RUN(myCam.Initialize(g_myAppConfig.lStartMode))
    // Get a buffer from sensor to know the image type
    RUN(myCam.GetImage(lImage1))
    // Create (pre-allocate) the buffer receiving the processed image
    lImage3 = vsdk::UMat(lCrop.height, lCrop.width, lImage1.type());

    // loop until ctrl-c is pressed (target linux)
    do
    {
      // Get a new image buffer from sensor
      RUN(myCam.GetImage(lImage1))
      // Crop the image buffer
      lImage2 = vsdk::UMat(lImage1, lCrop);
      // Pre-Display the image at  0,0
      RUN(myDisp.PreShow(0, 0, lImage2))
      // Compute the average intensity of the image
      RUN(myProcess.HistEqual(lImage2, lImage3))
      // Display the processed image (on the Pre-Display) at shifted location (opposite corner)
      RUN(myDisp.FinalShow(g_myAppConfig.lScreenDimensions.width - g_myAppConfig.lCWidth,
                           g_myAppConfig.lScreenDimensions.height - g_myAppConfig.lCHeight, lImage3))
      // count if in test
    } while(!g_myEvent.IsSignaled() && (g_myAppConfig.GetCount() > 0));
  }

END:

  if(0 != lRetVal)
  {
    printf("Program Ended Error 0x%X [ERROR]\n", lRetVal);
  }
  else
  {
    printf("Program Ended [SUCCESS]\n");
  }

  return lRetVal;
}

// function to change the application setting from parameters passed on the command line
#ifndef __STANDALONE__
void ProcessArguments(int argc, char** argv)
{
  // scan the arguments
  for(int i = 0; i < argc; i++)
  {
    std::string arg = argv[i];
    int         pos = arg.find("-tennis");
    if(0 == pos)
    { // requesting TENNIS mode
      g_myAppConfig.lStartMode = user_utils::UserCam::TENNIS;
    }
    pos = arg.find("-count=");
    if(0 == pos)
    { // requesting count mode
      std::string cCount   = arg.substr(7, 5);
      g_myAppConfig.lCount = std::stoi(cCount);
      g_myAppConfig.bCount = true;
    }
    pos = arg.find("-disp=");
    if(0 == pos)
    { // requesting test mode
      std::string cSub = arg.substr(6, arg.length() - 6);
      int         pos2 = cSub.find("x");
      if(pos2 > 0)
      {
        g_myAppConfig.lScreenDimensions = vsdk::Rect(0, 0, std::stoi(cSub.substr(0, pos2)),
                                                     std::stoi(cSub.substr(pos2 + 1, cSub.length() - pos2 - 1)));
        printf("screen dimensions to %dx%d\n", g_myAppConfig.lScreenDimensions.width,
               g_myAppConfig.lScreenDimensions.height);
      }
      else
      {
        //error
        g_myAppConfig.bExit = true;
      }
    }
    pos = arg.find("-cam=");
    if(0 == pos)
    { // requesting test mode
      std::string cSub = arg.substr(5, arg.length() - 5);
      int         pos2 = cSub.find("x");
      if(pos2 > 0)
      {
        g_myAppConfig.lCWidth  = std::stoi(cSub.substr(0, pos2));
        g_myAppConfig.lCHeight = std::stoi(cSub.substr(pos2 + 1, cSub.length() - pos2 - 1));
        printf("cam dimensions to %dx%d\n", g_myAppConfig.lCWidth, g_myAppConfig.lCHeight);
      }
      else
      {
        //error
        g_myAppConfig.bExit = true;
      }
    }
    pos = arg.find("-h");
    if(0 == pos || 0 == arg.find("--help") || 0 == arg.find("/help") || 0 == arg.find("/h"))
    { // requesting help
      printf("Usage, crtl-c to stop the application. Options:\n -h (-help): show usage\n");
      printf(" -tennis : use auto generated image, square moving on screen\n");
      printf(" -count=%d : run only for the number of images indicated and then exit the program\n");
      printf(" -disp=%%dx%%d : setup the screen to the dimensions provided\n");
      printf(" -cam=%%dx%%d : setup the input image to the dimensions provided\n");
      g_myAppConfig.bExit = true;
    }
  }
}
#endif
