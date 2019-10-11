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

#ifndef APEX2_EMULATE
#include "apex.h"
#endif

#include <apexcv_pro_orb.h>
#include <apexcv_base.h>
#include <opencv2/opencv.hpp>

#include <in_grey_256x256.h>
#include <common_helpers.h>
#include <string>

/****************************************************************************
* Main function
****************************************************************************/
int main(int argc, char** argv)
{
  std::string helpMsg = std::string("Demonstrates the ORB algorithm computed with the ApexCV-Pro library.\n\tUsage: ") +
                        COMMON_ExtractProgramName(argv[0]);
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  int                    lRetVal = 0;
  unsigned int           lNbKeyPointsFound;
  vsdk::UMat             lInput0Smooth;
  vsdk::UMat             lDescriptorsApex;
  apexcv::GaussianFilter myGaussianFilter;
  int                    lDescrSizeBits;
  int                    lTotalPointsInThePattern;
  const int              clPairXY               = 2;
  const int              nrOfPairsForComparison = 2;
  // This is needed only for the Target Compiler
  // HW and resources init
  APEX_Init();

  const int lSrcWidth  = 256;
  const int lSrcHeight = 256;

  // Allocate the input buffers
  vsdk::UMat lInput0(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);

  printf("Data on: \n");
  printf("   Input             (%dx%d bytes) at %p\n", lSrcWidth, lSrcHeight, lInput0.u->handle);

  { // isolate the virtual address mapping of the UMat object
    // fillup the input buffer
    vsdk::Mat mat = lInput0.getMat(OAL_USAGE_CACHED | vsdk::ACCESS_RW);
    if(!mat.empty())
    {
      memcpy(mat.data, in_grey_256x256, 256 * 256);
    }
  } // release the virtual address mapping of the UMat object

  const int clDescrSizeInBytes = 32;
  const int clMaxNbKeyPoints   = 512;

  // Create a new process for the Apex core.
  apexcv::Orb myOrb;

  //*********************
  // Initialize the process, mandatory first call to the object
  //*********************
  lRetVal |= myOrb.Create();
  if(lRetVal)
  {
    printf("Error on Initialize: %d \n", lRetVal);
    goto END;
  }
  printf("Create Done \n");

  //*********************
  // Find keypoints, call after Initialize for getting result
  //*********************

  lRetVal |= myOrb.Detect(lInput0);
  if(lRetVal)
  {
    printf("Error on Process: %d \n", lRetVal);
    goto END;
  }

  lNbKeyPointsFound = myOrb.GetNrOfDetectedKeypoints();
  printf("Detection Done, found %d/%d keypoints \n", lNbKeyPointsFound, clMaxNbKeyPoints);

  if(0 == lNbKeyPointsFound)
  {
    printf("Exiting, no keypoint found\n");
    lRetVal = EXIT_FAILURE;
    goto END;
  }

  //*********************
  // smooth input data
  //*********************
  lInput0Smooth = vsdk::UMat(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);

  // Initialize Gaussian filter
  lRetVal |= myGaussianFilter.Initialize(lInput0, 5, lInput0Smooth);
  if(lRetVal)
  {
    printf("Error on Initialize: %d \n", lRetVal);
    goto END;
  }
  // Smooth image
  lRetVal |= myGaussianFilter.Process();
  if(lRetVal)
  {
    printf("Error on Process: %d \n", lRetVal);
    goto END;
  }
  printf("Gaussian smoothing Done \n");

  lDescrSizeBits           = clDescrSizeInBytes * 8;
  lTotalPointsInThePattern = lDescrSizeBits * clPairXY * nrOfPairsForComparison;
  if(lTotalPointsInThePattern > 2048)
  {
    printf("Descriptor size is too big !\n");
    lRetVal = EXIT_FAILURE;
    goto END;
  }
  lDescriptorsApex = vsdk::UMat(1, lNbKeyPointsFound * clDescrSizeInBytes, VSDK_CV_8UC1);

  //*********************
  // Compute the results, call after Initialize for getting result
  //*********************
  lRetVal |= myOrb.Compute(lInput0Smooth, lDescriptorsApex);
  if(lRetVal)
  {
    printf("Error on Compute: %d \n", lRetVal);
    goto END;
  }
  printf("Compute Done \n");

  //*********************
  // Compute the results, call after Initialize for getting result
  //*********************
  {
    std::vector< apexcv::Orb::Corner >& keypoints = myOrb.GetKeypoints();
    printf("GetKeypoints Done \n");

    for(int idx = 0; idx < (int)keypoints.size(); idx++)
    {
      printf("Key point %d (x:%d, y:%d, strength:%.3f)\n", idx + 1, keypoints[idx].x, keypoints[idx].y,
             keypoints[idx].strength);
    }
  }
  printf("Display some Keypoints Done \n");

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
