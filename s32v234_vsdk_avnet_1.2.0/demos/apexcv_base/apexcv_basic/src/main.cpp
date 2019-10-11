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
      std::string("Basic apexcv demo, demonstrating add functionality with the ApexCV-Base library.\n\tUsage: ") +
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

  int lSrcWidth  = 1024;
  int lSrcHeight = 512;

  // Allocate the input and output buffers
  vsdk::UMat lInput0(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);
  vsdk::UMat lInput1(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);
  vsdk::UMat lOutput0(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);

  // Control output
  printf("Data on: \n");
  printf("   Input 1           (%dx%d bytes)\n", lSrcWidth, lSrcHeight);
  printf("   Input 2           (%dx%d bytes)\n", lSrcWidth, lSrcHeight);
  printf("   Output            (%dx%d words)\n", lSrcWidth, lSrcHeight);

  int lRetVal = 0;

  // Create a new add process for the Apex core.
  apexcv::Add myAdd;

  // Initialize the process, this is the mandatory first call to apexcv object
  lRetVal |= myAdd.Initialize(lInput0, lInput1, lOutput0);
  if(lRetVal)
  {
    printf("Error on Initialize: %d \n", lRetVal);
    goto END;
  }
  printf("Initialize Done \n");

  // Compute the results, the output buffer is updated after Process executes
  lRetVal |= myAdd.Process();
  if(lRetVal)
  {
    printf("Error on Process: %d \n", lRetVal);
    goto END;
  }

  printf("Process Done \n");

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
