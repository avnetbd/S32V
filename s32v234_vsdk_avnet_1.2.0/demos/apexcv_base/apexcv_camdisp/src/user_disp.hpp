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
#ifndef USERDISP_H
#define USERDISP_H


//
// Includes
//
#include <umat.hpp>

namespace user_utils
{

  /// *************************************************************************
  ///  @brief          User Event to inform about ctrl-C is pressed.
  /// *************************************************************************
  class UserEvent
  {
  public:
    UserEvent();
    ~UserEvent();
    /// *************************************************************************
    ///  @brief          Informed is ctrl-C was pressed.
    /// *************************************************************************
    bool IsSignaled();
    /// *************************************************************************
    ///  @brief          Force a signal as if ctrl-C was pressed.
    /// *************************************************************************
    void Signal();
  private:
    static void Signal(int signo);
    static bool sStop;
  };



  /// *************************************************************************
  ///  @brief          User Display for see images on screen.
  ///  @details        Object of this class allows to display images on the
  ///                  screen.
  /// *************************************************************************
  class UserDisp
  {
  public:
    cv::Scalar mBackgnd;///< This variable allows to change the background color of the display.
    static const int MAX_DISP_WIDTH = 1920;
    static const int MAX_DISP_HEIGHT = 1080;

  public:
    UserDisp();
    ~UserDisp();
    /// ***********************************************************************
    ///  @brief          Default initialization of the object.
    ///  @details        Object will be initialized with default values.
    /// ***********************************************************************
    int Initialize();
    /// ***********************************************************************
    ///  @brief          Initialization of the object with specific dimensions.
    ///  @details        Object will be initialized with the provided
    ///                  dimensions, note that dimensions will be cropped
    ///                  to the maximum possible dimensions.
    /// ***********************************************************************
    int Initialize(
      const vsdk::Rect& aDimensions); ///< sets the dimension of the screen used for display
    /// ***********************************************************************
    ///  @brief          Simple display function.
    ///  @details        Display an image to the screen.
    /// ***********************************************************************
    int Show(
      const vsdk::UMat& aImage); ///< image to put on the screen
    /// ***********************************************************************
    ///  @brief          Pre compose display with an image content.
    ///  @details        This function places the provided image content to 
    ///                  the display buffer at the provided location.  The
    ///                  content will go on display when FinalShow is called.
    /// ***********************************************************************
    int PreShow(
      const int_fast16_t aX, ///< Horizontal position of where to start placing the image
      const int_fast16_t aY, ///< Vertical position of where to start placing the image
      const vsdk::UMat& aImage); ///< image to pu on the screen
    /// ***********************************************************************
    ///  @brief          Compose display and put content on screen.
    ///  @details        this function places the provided image content to 
    ///                  the display buffer at the provided location, and send
    ///                  the display content on screen.
    /// ***********************************************************************
    int FinalShow(
    const int_fast16_t aX, ///< Horizontal position of where to start placing the image
      const int_fast16_t aY, ///< Vertical position of where to start placing the image
      const vsdk::UMat& aImage); ///< image to pu on the screen
      /// ***********************************************************************
    ///  @brief          Returns the dimensions used for display.
    ///  @details        This function provides the dimension of the display 
    ///                  buffer.
    /// ***********************************************************************
    void GetDimensions(
      vsdk::Rect& aDimensions);///< returns the dimension of the screen used for display

  private:
    class PrivateImplementation;
    PrivateImplementation* mpImpl;
  };

}
#endif
