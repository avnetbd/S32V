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
#include "user_process.hpp"     // Public header
#include "user_process_fct.hpp" // Private header with the processing classes

#ifndef APEX2_EMULATE
#include "apex.h"
#endif

namespace user_utils
{
// **************************************************************************
// Definition of the PrivateImplementation class
// **************************************************************************
class UserProcess::PrivateImplementation
{
public:
  UserHistEqual pUserHistEqual;
  PrivateImplementation(){};
};

// **************************************************************************
// Create object
// **************************************************************************
UserProcess::UserProcess()
{
// This is needed only for the Target Compiler
// HW and resources init
#ifndef APEX2_EMULATE
  APEX_Init();
  printf("ACF Initialized\n");
#endif
  mpImpl = new UserProcess::PrivateImplementation();
}

// **************************************************************************
// Delete object
// **************************************************************************
UserProcess::~UserProcess()
{
  if(nullptr != mpImpl)
  {
    delete mpImpl;
  }
}

int_fast32_t UserProcess::HistEqual(vsdk::UMat& aSrc, ///< image source, type is VSDK_CV_8UC3
                                    vsdk::UMat& aDst) ///< destination image, type is VSDK_CV_8UC3
{
  int_fast32_t lRetVal = 999;
  if(nullptr != mpImpl)
  {
    lRetVal = mpImpl->pUserHistEqual.Process(aSrc, aDst);
  }
  else
  {
    printf("Memory allocation error !!! in %s line %d\n", __FILE__, __LINE__);
  }
  return lRetVal;
}
}
