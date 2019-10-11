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
#ifndef USERPROCESS_H
#define USERPROCESS_H


//
// Includes
//
#include <cstdint>
#include <umat.hpp>

namespace user_utils
{

  /// *************************************************************************
  ///  @brief          User Display for see images on screen.
  ///  @details        Object of this class allows to display images on the
  ///                  screen.
  /// *************************************************************************
  class UserProcess
  {
  public:
    UserProcess();
    ~UserProcess();
    /// *************************************************************************
    ///  @brief          Process the source image to equalize its histogram.
    ///  @details        Process the source image to equalize its histogram,
    ///                  destination image returns the processed results.
    ///                  The 3 channels are processed separately.
    /// *************************************************************************
    int_fast32_t HistEqual(
      vsdk::UMat& aSrc, ///< image source, type is VSDK_CV_8UC3
      vsdk::UMat& aDst); ///< destination image, type is VSDK_CV_8UC3

  private:
    class PrivateImplementation;
    PrivateImplementation* mpImpl;

  };

}
#endif
