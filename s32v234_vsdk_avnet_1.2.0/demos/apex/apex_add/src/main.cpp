/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor;
* Copyright (c) 2016-2018 NXP Semiconductor;
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

// Utility functions for UMat/Mat
#include <umat_helper.h>
// Utility funtions
#include <common_time_measure.h>
#include <common_helpers.h>
#include <string>

// Name definitions used for the ACF graph
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

void compute_ref(vsdk::UMat& lInputA, vsdk::UMat& lInputB, vsdk::UMat& lOutput);

/****************************************************************************
* Constant declarations
****************************************************************************/
const int cIterationCount = 5;
const int cImageWidth     = 2048;
const int cImageHeight    = 1024;

/****************************************************************************
* Main function
* Note: This sample code highlights the steps for using a functionality.
*       To make the code more readable, error management has been simplified.
****************************************************************************/
int main(int argc, char** argv)
{
  std::string helpMsg = std::string("Adds two random matrices on APEX in different ways.\n\tUsage: ") +
                        COMMON_ExtractProgramName(argv[0]);
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  // Return status
  int lRetVal = 0;
  // Time variables
  int64_t lTicks0 = 0;
  int64_t lTicks1 = 0;

  // HW and resources init
  APEX_Init();

  // Image size definition
  int lSrcWidth  = cImageWidth;
  int lSrcHeight = cImageHeight;

  // Allocate the input and output buffers
  vsdk::UMat lInputA(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);
  vsdk::UMat lInputB(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);
  vsdk::UMat lDoSaturate(1, 1, VSDK_CV_8UC1);
  vsdk::UMat lOutput(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);
  vsdk::UMat lOutput_REF(lSrcHeight, lSrcWidth, VSDK_CV_8UC1);

  // Configure single process which shall run on one of the two APEX cores.
  ADD_PI addProcess_single;
  // Configure dual process which shall run in parallel, each on one of the two APEX cores.
  ADD_PI addProcess_dual0;
  ADD_PI addProcess_dual1;

  // ACF Process initialization (single)
  printf("Initialize ACF process\n");
  lRetVal |= addProcess_single.Initialize();

  // ACF Process initialization (dual)
  lRetVal |= addProcess_dual0.Initialize();
  lRetVal |= addProcess_dual1.Initialize();

  // Connect buffers (vsdk::UMat) to all ACF process ports (single)
  printf("Connect buffers to ACF process\n");
  lRetVal |= addProcess_single.ConnectIO(ADD_GRAPH_INA, lInputA);
  lRetVal |= addProcess_single.ConnectIO(ADD_GRAPH_INB, lInputB);
  lRetVal |= addProcess_single.ConnectIO(ADD_GRAPH_INC, lDoSaturate);
  lRetVal |= addProcess_single.ConnectIO(ADD_GRAPH_OUT, lOutput);
  // Connect buffers (vsdk::UMat) to all ACF process ports (dual)
  printf("Connect buffers to ACF process\n");
  lRetVal |= addProcess_dual0.ConnectIO_ROI(ADD_GRAPH_INA, lInputA, 0, 0, lSrcWidth, lSrcHeight / 2);
  lRetVal |= addProcess_dual1.ConnectIO_ROI(ADD_GRAPH_INA, lInputA, 0, lSrcHeight / 2, lSrcWidth, lSrcHeight / 2);
  lRetVal |= addProcess_dual0.ConnectIO_ROI(ADD_GRAPH_INB, lInputB, 0, 0, lSrcWidth, lSrcHeight / 2);
  lRetVal |= addProcess_dual1.ConnectIO_ROI(ADD_GRAPH_INB, lInputB, 0, lSrcHeight / 2, lSrcWidth, lSrcHeight / 2);
  lRetVal |= addProcess_dual0.ConnectIO_ROI(ADD_GRAPH_INC, lDoSaturate, 0, 0, 1, 1);
  lRetVal |= addProcess_dual1.ConnectIO_ROI(ADD_GRAPH_INC, lDoSaturate, 0, 0, 1, 1);
  lRetVal |= addProcess_dual0.ConnectIO_ROI(ADD_GRAPH_OUT, lOutput, 0, 0, lSrcWidth, lSrcHeight / 2);
  lRetVal |= addProcess_dual1.ConnectIO_ROI(ADD_GRAPH_OUT, lOutput, 0, lSrcHeight / 2, lSrcWidth, lSrcHeight / 2);

  // Set random input values
  for(unsigned int i = 0; i < cIterationCount; ++i)
  {
    printf("Iteration %d/%d\n", i + 1, cIterationCount);
    printf("=======================================\n");

    {
      printf("Input random seed.\n");
      lTicks0 = FSL_Ticks();
      // Fill the Mats with the random data
      // note the block definition {} allows to minimize the scope of Mat from UMat :
      // - access virtual address of the UMat data via a Mat object
      // - have the Host processor to change the data of the Mat object
      // - when the block ends, destruction of the Mat instances flushies the cache and updates the UMat content
      {
        vsdk::Mat lInputAMat = lInputA.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
        vsdk::Mat lInputBMat = lInputB.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
        int       size       = lSrcWidth * lSrcHeight;
        uint32_t  aVal[1]    = {1};
        for(int j = 0; j < size; ++j)
        {
          lInputAMat.at< uint8_t >(j) = rand() % 256;
          lInputBMat.at< uint8_t >(j) = rand() % 256;
        }
        vsdk::FillValue(lDoSaturate, aVal);
      }
      lTicks1 = FSL_Ticks();
      printf("%.6f ms and %" PRId64 " ticks Host Random seed, \n",
             (float)FSL_TicksToSeconds(lTicks1 - lTicks0) * 1000.0F, lTicks1 - lTicks0);
    }

    // Compute reference on the Host processor (ARM)
    {
      printf("Compute reference value via SW.\n");
      lTicks0 = FSL_Ticks();
      compute_ref(lInputA, lInputB, lOutput_REF);
      lTicks1 = FSL_Ticks();
      printf("%.6f ms and %" PRId64 " ticks Host computation, \n",
             (float)FSL_TicksToSeconds(lTicks1 - lTicks0) * 1000.0F, lTicks1 - lTicks0);
    }

    // Perform Computation on APEX core (single)
    {
      printf("Start processing (single).\n");
      lTicks0 = FSL_Ticks();
      lRetVal |= addProcess_single.Start();
      lRetVal |= addProcess_single.Wait();
      lTicks1 = FSL_Ticks();
      printf("Processing done.\n");
      printf("%.6f ms and %" PRId64 " ticks APEX computation (single), \n",
             (float)FSL_TicksToSeconds(lTicks1 - lTicks0) * 1000.0F, lTicks1 - lTicks0);
    }

    // Compare the results (single)
    {
      printf("Comparison. \n");
      lTicks0 = FSL_Ticks();
      lRetVal |= vsdk::Compare(lOutput_REF, lOutput);
      lTicks1 = FSL_Ticks();
      printf("%.6f ms and %" PRId64 " ticks Host comparison, \n",
             (float)FSL_TicksToSeconds(lTicks1 - lTicks0) * 1000.0F, lTicks1 - lTicks0);
      printf("Outputs are identical.\n");
    }

    // Perform Computation on APEX core (dual)
    {
      printf("Start processing (dual).\n");
      lTicks0 = FSL_Ticks();
      // Hardware assignment is only available on S32V Chip, not emulator which processes on Start()
#ifndef APEX2_EMULATE
      // assign addProcess_dual0 to APEX core 0
      lRetVal |= addProcess_dual0.SelectApuConfiguration(ACF_APU_CFG__DEFAULT, 0);
      // assign addProcess_dual1 to APEX core 1
      lRetVal |= addProcess_dual1.SelectApuConfiguration(ACF_APU_CFG__DEFAULT, 1);
#endif
      lRetVal |= addProcess_dual0.Start();
      lRetVal |= addProcess_dual1.Start();

      lRetVal |= addProcess_dual0.Wait();
      lRetVal |= addProcess_dual1.Wait();
      lTicks1 = FSL_Ticks();
      printf("Processing done.\n");
      printf("%.6f ms and %" PRId64 " ticks APEX computation (dual), \n",
             (float)FSL_TicksToSeconds(lTicks1 - lTicks0) * 1000.0F, lTicks1 - lTicks0);
    }

    // Compare the results (dual)
    {
      printf("Comparison. \n");
      lTicks0 = FSL_Ticks();
      lRetVal |= vsdk::Compare(lOutput_REF, lOutput);
      lTicks1 = FSL_Ticks();
      printf("%.6f ms and %" PRId64 " ticks Host comparison, \n",
             (float)FSL_TicksToSeconds(lTicks1 - lTicks0) * 1000.0F, lTicks1 - lTicks0);
      printf("Outputs are identical.\n\n");
    }
  }

  if(lRetVal != 0)
  {
    printf("Program Ended Error 0x%X [ERROR]\n", lRetVal);
  }
  else
  {
    printf("Program Ended [SUCCESS]\n");
  }

  return lRetVal;
}

/****************************************************************************
* Compute the same operation on the Host processor as reference
****************************************************************************/
void compute_ref(vsdk::UMat& lInputA, vsdk::UMat& lInputB, vsdk::UMat& lOutput)
{
  // get data in virtual address representation (Mat) so Host can access the data
  vsdk::Mat lInputAMat = lInputA.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);
  vsdk::Mat lInputBMat = lInputB.getMat(vsdk::ACCESS_READ | OAL_USAGE_CACHED);
  vsdk::Mat lOutputMat = lOutput.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);

  const uint8_t* plInputA0 = lInputAMat.data;
  const uint8_t* plInputB0 = lInputBMat.data;
  uint8_t*       plOutput0 = lOutputMat.data;

  int lInputA_str = static_cast< int >(lInputAMat.step[0] / lInputAMat.elemSize());
  int lInputB_str = static_cast< int >(lInputBMat.step[0] / lInputBMat.elemSize());
  int lOutput_str = static_cast< int >(lOutputMat.step[0] / lOutputMat.elemSize());

  for(int32_t i = 0; i < lInputAMat.rows; i++)
  {
    const uint8_t* plInputA = plInputA0;
    const uint8_t* plInputB = plInputB0;
    uint8_t*       plOutput = plOutput0;
    for(int32_t j = 0; j < lInputAMat.cols; j++)
    {
      uint16_t lVal = static_cast< uint16_t >(*plInputA++) + static_cast< uint16_t >(*plInputB++);
      lVal          = lVal < 255 ? lVal : 255;
      *plOutput++   = static_cast< uint8_t >(lVal);
    }
    plInputA0 += lInputA_str;
    plInputB0 += lInputB_str;
    plOutput0 += lOutput_str;
  }
  // release the data virtual addresses (Mat), updates the corresponding UMats.
}
