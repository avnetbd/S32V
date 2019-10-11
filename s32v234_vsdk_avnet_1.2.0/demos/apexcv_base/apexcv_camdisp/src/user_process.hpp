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
    ///  @brief          User Display for see images on screen.
    ///  @details        Object of this class allows to display images on the
    ///                  screen.
    /// *************************************************************************
    int_fast32_t ChannelInvert(
      vsdk::UMat& aSrc, ///< image source, type is VSDK_CV_8UC3
      int_fast8_t aChannelIndex, ///< index of the channel to invert, 1 to 3
      vsdk::UMat& aDst); ///< destination image, type is VSDK_CV_8UC3
    /// *************************************************************************
    ///  @brief          User Display for see images on screen.
    ///  @details        Object of this class allows to display images on the
    ///                  screen.
    /// *************************************************************************
    int_fast32_t ChannelIntensity(
      vsdk::UMat&   aSrc, 
      uint_fast8_t& aIntensity);

  private:
    class PrivateImplementation;
    PrivateImplementation* mpImpl;

  };

}
#endif
