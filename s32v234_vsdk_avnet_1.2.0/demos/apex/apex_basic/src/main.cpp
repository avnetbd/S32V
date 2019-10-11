/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2018 NXP
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
#include <inttypes.h>
#include <string.h>

// Utility function for UMat/Mat
#include <umat_helper.h>
// Name definitions used for the ACF graph, access to the port names
#include <apu_add_graph_names.h>

// ACF Process class definition
#ifdef APEX2_EMULATE
#include "acf_lib.hpp"
using namespace APEX2;
#include "apu_add_apu_process_desc.hpp"
REGISTER_PROCESS_TYPE(ADD_PI, apu_add_apu_process_desc)
#else
#include "apex.h"
#include str_header(ADD_PI, hpp)
#endif
#include <common_helpers.h>
#include <string>
/****************************************************************************
* Constant declarations
****************************************************************************/
const int cImageWidth  = 2048;
const int cImageHeight = 1024;

/****************************************************************************
* Main function
* Note: This sample code highlights the steps for using a functionality.
*       To make the code more readable, error management has been simplified.
****************************************************************************/
int main(int argc, char** argv)
{
  // 0 success, not 0 error
  int lRetVal = 0;
  printf("Starting APEX BASIC Program.\n");
  std::string helpMsg = std::string("Adds two random matrices on APEX in a very basic way.\n\tUsage: ") +
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

  int lSrcWidth  = cImageWidth;
  int lSrcHeight = cImageHeight;

  // Allocate the input and output buffers
  vsdk::UMat lInput0(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);
  vsdk::UMat lInput1(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);
  vsdk::UMat lDoSaturate(1, 1, VSDK_CV_8UC1);
  vsdk::UMat lOutput0(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);

  // Create an ACF process
  ADD_PI addProcess;

  // ACF Process initialization
  printf("Initialize ACF process\n");
  lRetVal |= addProcess.Initialize();

  // Connect buffers (vsdk::UMat) to all ACF process ports
  printf("Connect buffers to ACF process\n");
  lRetVal |= addProcess.ConnectIO(ADD_GRAPH_INA, lInput0);
  lRetVal |= addProcess.ConnectIO(ADD_GRAPH_INB, lInput1);
  lRetVal |= addProcess.ConnectIO(ADD_GRAPH_INC, lDoSaturate);
  lRetVal |= addProcess.ConnectIO(ADD_GRAPH_OUT, lOutput0);

  // Fill the Images with the random data
  printf("Fill buffers\n");
  {
    uint32_t seed = 0x3d44A792;
    vsdk::FillRandom(lInput0, &seed);
    vsdk::FillRandom(lInput1, &seed);
    vsdk::FillZero(lDoSaturate);
  }
  printf("Initialization Done \n");

  // Perform Computation on APEX core
  printf("Start processing.\n");
  lRetVal |= addProcess.Start();
  lRetVal |= addProcess.Wait();
  printf("Processing done.\n");

  if(lRetVal != 0)
  {
    printf("Program Ended with Error 0x%X [ERROR]\n", lRetVal);
  }
  else
  {
    printf("Program Ended [SUCCESS]\n");
  }

  return lRetVal;
}
