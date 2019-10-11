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
#ifndef USERCAM_H
#define USERCAM_H


//
// Includes
//
#include <umat.hpp>
#include <cstdint>

namespace user_utils
{

  /// *********************************************************************************
  ///  @brief          User Camera for getting images.
  ///  @details        Object of this class allows to provide images with changing
  ///                  content frame to frame.
  /// ***********************************************************************************
  class UserCam
  {
  public:
    enum MODE { DEFAULT, TENNIS };///< Provide the type of mode for the source of content.
    UserCam();
    ~UserCam();
    static const int  CAM_WIDTH = 1280; ///< The default width of the images provided by objects of this class.
    static const int  CAM_HEIGHT = 720;///< The default height of the images provided by objects of this class.

    /// *********************************************************************************
    ///  @brief          Initialize objects of this class.
    ///  @details        Initialization is optional. If not called, the default mode will
    ///                  be MODE::DEFAULT.
    /// ***********************************************************************************
    int Initialize(
      const MODE aMode); ///< Selection of the mode to use for generation images.


    /// *********************************************************************************
    ///  @brief          Provides a new image from the source.
    ///  @details        the image provided depends on the mode selected during 
    ///                  initialization, by default it is MODE::DEFAULT.
    /// ***********************************************************************************
    int GetImage(
      vsdk::UMat& aImage); ///< Image that will receive new content

  private:
    class PrivateImplementation;
    PrivateImplementation* mpImpl;

  };

}
#endif
