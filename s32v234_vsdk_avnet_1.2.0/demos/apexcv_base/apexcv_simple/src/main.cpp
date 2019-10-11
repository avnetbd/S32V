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
#else
#include "acf_lib.hpp"
#endif

#include <apexcv_base.h>
#include <common_helpers.h>
#include <string>

/****************************************************************************
* Main function
****************************************************************************/
int main(int argc, char** argv)
{
  std::string helpMsg =
      std::string(
          "Simple apexcv demo, demonstrating the histogram computation with the ApexCV-Base library.\n\tUsage: ") +
      COMMON_ExtractProgramName(argv[0]);
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  // This is needed only for the Target Compiler
  // HW and resources init
  APEX_Init();

  const int  lSrcWidth   = 1024;
  const int  lSrcHeight  = 512;
  const int  lHistWidth  = 256;
  const int  lHistHeight = 1;
  const int  cLoopCount  = 10;
  int        lRetVal     = 0;
  vsdk::UMat lInput1;
  vsdk::UMat lOutput1;
  // Allocate the input and output buffers
  vsdk::UMat lInput0(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);
  vsdk::UMat lOutput0(lHistHeight, lHistWidth, VSDK_CV_32SC1);

  // Control output
  printf("Data on: \n");
  printf("   Input             (%dx%d bytes)\n", lSrcWidth, lSrcHeight);
  printf("   Output            (%dx%d words)\n", lSrcWidth, lSrcHeight);

  // Create a new process for the Apex core.
  apexcv::Histogram myHist;

  // Initialize the process, mandatory first call to the object
  lRetVal |= myHist.Initialize(lInput0, lOutput0);
  if(lRetVal)
  {
    printf("Error on Initialize: %d \n", lRetVal);
    goto END;
  }
  printf("Initialize Done \n");

  // Compute the results, call after Initialize for getting result
  lRetVal |= myHist.Process();
  if(lRetVal)
  {
    printf("Error on Process: %d \n", lRetVal);
    goto END;
  }
  printf("Single Process Done \n");

  // For processing in a loop and reduce overhead
  // call reconnectIO and then Process
  lInput1  = vsdk::UMat(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);
  lOutput1 = vsdk::UMat(lHistHeight, lHistWidth, VSDK_CV_32SC1);

  for(int i = 1; i <= cLoopCount; i++)
  {
    printf("Run %d/%d in loop ", i, cLoopCount);

    vsdk::UMat lOutput;
    vsdk::UMat lInput;

    // alternate buffers on each iteration
    if(i & 0x01)
    {
      lOutput = lOutput0;
      lInput  = lInput0;
      printf("using buffers '0' \n");
    }
    else
    {
      lOutput = lOutput1;
      lInput  = lInput1;
      printf("using buffers '1' \n");
    }

    // Change the buffer to use for the processing
    lRetVal |= myHist.ReconnectIO(lInput, lOutput);
    if(lRetVal)
    {
      printf("Error on ReconnectIO: %d \n", lRetVal);
      goto END;
    }
    printf("ReconnectIO Done \n");

    // Compute the results
    lRetVal |= myHist.Process();
    if(lRetVal)
    {
      printf("Error on Process: %d \n", lRetVal);
      goto END;
    }
    printf("Process Done \n");
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
