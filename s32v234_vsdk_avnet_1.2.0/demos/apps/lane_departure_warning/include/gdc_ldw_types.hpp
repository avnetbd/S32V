/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
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

/*!*********************************************************************************
*  @file
*  @brief Private API of Support functions for Lane Departure Warning.
*
*  \section ldw_desc Description
*
*  This file contains support functions related to the Lane Departure Warning
*  System, version 2.
*
***********************************************************************************/

#ifndef GDCLDWTYPES_HPP
#define GDCLDWTYPES_HPP

/***************************************************************************/
/* Include files. */
#include <gdc_ldw_configuration.hpp>
#include "gdc_ldw_messages.hpp"
#include "GDC_OAL_Semaphore.hpp"
#include "icp_feature.h"
#include <oal.h>
#if !defined(__CYGWIN__) && !defined(_WIN32)
#include "sdi.hpp"
#include "frame_io_types.h"
#ifdef __STANDALONE__
#include "frame_output_dcu.h"
typedef io::FrameOutputDCU io_FrameOutput_t;
#else
#include "frame_output_v234fb.h"
typedef io::FrameOutputV234Fb io_FrameOutput_t;
#endif
#endif // #if defined(__CYGWIN__) || defined(_WIN32)
#include <global_errors.h>
#include <stdint.h>
#include "icp_data.h"
#include "icp_fixed.hpp"
#include "icp_remap_protected.hpp"
#include "mapper_type_defines.h"
#include "param_loader.h"
#include <mcdma_hal.h>
#include <acf_process.h>
#include <ICL_Kalman.hpp>
#include <umat.hpp>
#include <icp_types.h>
#include <dictionary.h>
#include <apexcv_pro_remap.h>
#include "toggle_regression.h"

/* 
 * Place these lines after #include "toggle_regression.h"
 * Used to toggle between two separate graphs
 * See toggle_regression.h for details
 */
#ifdef _TOGGLE_REG_GRAPH_
  #include "ACF_FILTERS_REG.hpp"
#endif // end: #ifdef _TOGGLE_REG_GRAPH_
#ifndef _TOGGLE_REG_GRAPH_
  #include "ACF_FILTERS_NO_REG.hpp"
#endif // end: #ifndef _TOGGLE_REG_GRAPH_

//**************************************************************************/
// Literals & definitions
//#define SHOW_ERROR(expr) if(!(expr)) { printf("%s : %s [%d]\n", __FILE__, __PRETTY_FUNCTION__, __LINE__); }

//#define GDC_TRACE fprintf(stderr, "%s - %u\n", __PRETTY_FUNCTION__, __LINE__);
#define GDC_TRACE


//**************************************************************************/
// Class declaration.
class GdcLdw_MovingAverage
{
public:
   GdcLdw_MovingAverage();
   ~GdcLdw_MovingAverage();

   void Initialize(const uint32_t cCount_Max);
   uint64_t Update(const uint64_t cNewValue); // Returns new average
   uint64_t Get() const;

private:
   uint64_t mSum;
   uint64_t mOldestValue;
   uint32_t mCount_Current;
   uint32_t mCount_Max;
};

//**************************************************************************/
// Class declaration.

struct ROI
{
    int x;
    int y;
    int width;
    int height;
    int score;
    double h_scale;
    double v_scale;
    int s_width;
    int s_height;
};


/*!*********************************************************************************
*  @brief Structure storing configuration values read in from ini file.
*
*  @details The LDW application is configured during runtime through the use of an ini file.
*  During boot up various values will be read in from the ini file and
*  used to configure the application.
*
*/
struct GdcLdw_LdwConfig
{
   char    mBirdeyeLutFilename[gcStringMaxSize];
   int16_t BirdeyeLutOffsetBpp;        // Bytes Per Pixels
   int16_t BirdeyeLutDeltaBpp;         // Bytes Per Pixels
   int16_t mBirdEyeHeight;             // Pixels
   int16_t mBirdEyeWidth;              // Pixels
   int16_t mMaxLaneWidth;              // Pixels
   int16_t mMinLaneWidth;              // Pixels
   int16_t mInitialKLGap;              // Pixels
   int16_t mInitialKCentre;            // Pixels
   uint8_t mLaneVoteThreshold;
   int16_t mMaxLRAngleDiffInDegrees;
   int16_t mMaxAngleDeltaInDegrees;
   uint8_t mMaxLaneCenterDelta;        // Pixels
   int16_t mWarningWidthFromCenter;    // Pixels
   int16_t mWarningHysteresisPct;
   int16_t mMaxWarningFrameCount;
   char    mOutputReadFilename[gcStringMaxSize];
   char    mOutputWriteFilename[gcStringMaxSize];
   bool    mWeReadOutputFile;
   bool    mWeWriteOutputFile;
};



/**
 *  \brief Type representing a line using polar coordinates.
 *  \details The GdcLdw_Helper::FindLanes function outputs the detected lanes using this
 *  type.
 *
 */
struct GdcLdw_LinePolar
{
   GdcLdw_LinePolar();
   GdcLdw_LinePolar(int32_t rho, int32_t angle, int32_t votes);

   int32_t mRho;
   int32_t mAngleInDegrees;
   float   mAngleInRadians;
   int32_t mVotes;
};



/**
 *  \brief Type used to eliminate noise in values generated by the LDW algorithm.
 *  \details Kalman filters are used to track them in order to manage various unwanted
 *  noise such as cracks in the road, shadows from other vehicles and bridges, and also
 *  in situations when the road lines are not very visible.
 *
 */
class GdcLdw_ConstantValueFilter
{
public:
   GdcLdw_ConstantValueFilter();

   LIB_RESULT Initialize(const float cMeasurementNoiseVar,
                         const float cPostErrorVar = 0.1f,
                         const float cInitialValue = 0.0f,
                         const float cProcessNoiseVar = 0.001f);

   LIB_RESULT Reset(const float cMeasurementNoiseVar,
                    const float cPostErrorVar = 0.1f,
                    const float cInitialValue = 0.0f,
                    const float cProcessNoiseVar = 0.001f);

   LIB_RESULT NextMeasurement(const float cMeasurement);

   float GetValue();
   ICP_KalmanFilter        mKalman;
private:

   ICP_Mat<float>          mMeasurementMat;
   bool                    mIsInitialized;
};



/*!*********************************************************************************
 *  @brief Additional remap functionality required for the CgcLdw2 demo.
 */

namespace apexcv
{
   class GdcLdw_Remap : public Remap
   {
   public:
       /*!*********************************************************************************
       *  \brief converts lines in the birdseye view to the original image
       *
       *  This method returns two scene lines as computed by the transform from the
       *  original initialization
       *
       *  \return Error code for the execution (zero on success).
       ***********************************************************************************/
          int convertBirdEyeLinesToScene(const uint16_t cSrcWidth,                     /*!< Input. remaped image width */
                                         const ICP_Line cSrcLeftLane,                  /*!< Input. ICP_Line representing the left lane from the remaped image*/
                                         const ICP_Line cSrcRightLane,                 /*!< Input. ICP_Line representing the right lane from the remaped image*/
                                         ICP_Line &sceneLeftLane,                      /*!< Output. ICP_Line representing the left lane on the original scene */
                                         ICP_Line &sceneRightLane);                    /*!< Output. ICP_Line representing the right lane on the original scene*/



   }; /* end class */
} /* end namespace apexcv */


typedef void* GdcLdw_TCB; ///< opaque value returned to the task creator to be used when referring to a task
const uint32_t gcGdcLdTaskNameMaximumSizeInChar = 32;
const char gcGdcLdTaskSemaphoreThreadSafetyName[] = "TaskTS";


class GdcLdw_Task  // Abstract Task Class
{
/**
*  \addtogroup GdcLdw_Task OAL Task
*  @{
*/
public:
   /***************************************************************************/
   /**
    * \details
    * This function initializes the data members of the object
    *
    **************************************************************************/
   GdcLdw_Task();



   /***************************************************************************/
   /**
    * \details
    * This function frees any outstanding resources in the objects possession.
    *
    **************************************************************************/
   virtual ~GdcLdw_Task();



   /***************************************************************************/
   /**
    * \details
    * This function creates and activates a new task.
    * It is equivalent to calling "Init" and "Activate".
    *
    *
    * \param  cStackSizeInBytes - [in] Size in bytes of the tasks stack
    * \param  cTaskPriority     - [in] Specifies a priority value between 0 and 255.
    *                             The lower the numeric value, the higher the tasks priority.
    * \param  cTaskName         - [in] Pointer to a 31 character name for the task.
    *                             This name is shared by the task and its default message queue.
    *                             For name registration to succeed this name must be unique.
    *
    *
    * \return
    *    - #LIB_SUCCESS
    *    - #LIB_FAILURE
    *    - #OAL_ERR_LOG_EINVALIDPARAM
    **************************************************************************/
   LIB_RESULT Spawn(const uint32_t cStackSizeInBytes,
                    const uint32_t cTaskPriority,
                    const char     cTaskName[]);


   /***************************************************************************/
   /**
   * \details
   * This function waits for the thread to finish.
   *
   * \return
   *    - #LIB_SUCCESS
   *    - #LIB_FAILURE
   **************************************************************************/
   LIB_RESULT Join();


   /***************************************************************************/
   /**
    * \details
    * This function causes the task to cease to exist; the stack and control block
    * resources are freed. If the task is safe from deletion, This function will block
    * until the task is made unsafe.
    *
    *
    * \return
    *    - #LIB_SUCCESS
    *    - #LIB_FAILURE
    **************************************************************************/
   LIB_RESULT Delete();


private:
   virtual LIB_RESULT TaskOpen(){ return LIB_SUCCESS; };
   virtual LIB_RESULT TaskService() = 0; // Pure Virtual hence you CANNOT instantiate this class
   virtual LIB_RESULT TaskClose(){ return LIB_SUCCESS; };


private:
   GdcLdw_TCB          mTaskControlBlock;
   uint32_t             mStackSizeInBytes;
   uint32_t             mPriority;   // Lower the value; higher the priority
   char                 mName[gcGdcLdTaskNameMaximumSizeInChar];
   GDC_OAL_SEMAPHORE    mSemaphoreThreadSafety;
   bool                 mIsSemaphoreThreadSafetyCreated;
   bool                 mIsTaskSpawned;
   bool                 mIsTaskFinished;

   //TODO: was void instead of void*...
   static void* TaskEntryRoutine(void* pVoid);

private:
   GdcLdw_Task( const GdcLdw_Task& );            // Do not implement
   GdcLdw_Task& operator=( const GdcLdw_Task& ); // Do not implement
}; // End of GdcLdw_Task declaration.



/*!*********************************************************************************
*  @brief Class used to gather utility function needed by the application or its tasks.
 */
class GdcLdw_Helper
{
public:
   /*!*********************************************************************************
    *  @brief Method used to read the ldw configs from the ini fil
    */
   static LIB_RESULT ReadLdwConfigFromIniFile(const char cConfigFilename[],
                                              GdcLdw_LdwConfig& rReturnedConfig);


   /*!*********************************************************************************
    *  @brief Method used to call the acf graph for the image processing
    */

    #ifdef _TOGGLE_REG_GRAPH_
      static LIB_RESULT AcfFilters_reg(
        ACF_FILTERS_REG& rAcfRegProcess,
        icp::DataDescriptor& rcImageSrcCdd,
        icp::DataDescriptor& rcLutDeltaCdd,
        icp::DataDescriptor& rcLutOffsetCdd,
        icp::DataDescriptor& rcLutBlockOffsetCdd,
        icp::DataDescriptor& rcVoteThresholdCdd,
        icp::DataDescriptor& rcLaneDataCdd,
        icp::DataDescriptor& rcPreviousThresholdCdd,
        icp::DataDescriptor& rcBirdEyeSobelDstCdd);

    #endif // #ifdef _TOGGLE_REG_GRAPH_


    #ifndef _TOGGLE_REG_GRAPH_    
      static LIB_RESULT AcfFilters_no_reg(ACF_FILTERS_NO_REG& rAcfNoRegProcess,
                                          vsdk::UMat& rcImageSrc_umat,
                                          vsdk::UMat& rcLutDelta_umat,
                                          vsdk::UMat& rcLutOffset_umat,
                                          vsdk::UMat& rcLutBlockOffset_umat,
                                          vsdk::UMat& rcVoteThreshold_umat,
                                          vsdk::UMat& rcLaneData_umat,
                                          vsdk::UMat& rcPreviousThreshold_umat,
                                          vsdk::UMat& rcBirdEyeSobelDst_umat,
                                          vsdk::UMat& rcLeftMax_umat,    // used for regression
                                          vsdk::UMat& rcRightMax_umat,   // used for regerssion
                                          vsdk::UMat& rcPoints_umat);      // used for regression




      static int16_t FastArcTan(float x);

      static void RegressionLine(const uint8_t    cMaxVotes,
                                 const uint32_t*   cPointsData,
                                 const uint8_t     cDataStride,
                                       uint32_t*   rLane);

    #endif // #ifndef _TOGGLE_REG_GRAPH_

   /*!*********************************************************************************
    *  @brief Updates the structures using the raw data generated by the ACF kernels
    *
    *   expects data to be organized as uint32_t in this order
    *   0. Votes           
    *   1. AngleInDegrees  
    *   2. point1.y        
    *   3. point2.y        
    *   4. point1.x        
    *   5. point2.x        
    */
   static LIB_RESULT UpdateLaneRawData(const vsdk::UMat& rcLaneData_umat,
                                       GdcLdw_LinePolar& rLeftLanePolar,
                                       GdcLdw_LinePolar& rRightLanePolar,
                                       ICP_Line & rLeftLaneCartesian,
                                       ICP_Line & rRightLaneCartesian);


   /*!*********************************************************************************
    *  @brief This method implements a Hough transform using an APU kernel. 
    *
    *  This method is currently not used
    */
  static LIB_RESULT FindLanesHough(vsdk::UMat& rcBirdEyeSobelMedian_umat,
                                   const uint8_t cMinGrayscaleValue,
                                   const int32_t cVoteThreshold,
                                   GdcLdw_LinePolar& rLeftLanePolar,      // OUT
                                   GdcLdw_LinePolar& rRightLanePolar,     // OUT
                                   ICP_Line & rLeftLaneCartesian,         // OUT
                                   ICP_Line & rRightLaneCartesian);       // OUT


   /*!*********************************************************************************
    *  @brief Method used to convert lane coordinates from birdseye to scene perspective
    */
   static LIB_RESULT ConvertBirdEyeToScene(const ICP_Mat<float>& cBirdEyeToSceneMat,
                                                 ICP_Mat<float>& rTempPointMat1,
                                                 ICP_Mat<float>& rTempPointMat2,
                                           const ICP_Line& rcBirdEyeLeftLane,
                                           const ICP_Line& rcBirdEyeRightLane,
                                                 ICP_Line& rSceneLeftLane,
                                                 ICP_Line& rSceneRightLane);

   /*!*********************************************************************************
    *  @brief Method used to convert lane coordinates from scene to a distance in meters on the birdseye image.
    */
   static LIB_RESULT ConvertSceneToBirdEyeMeters(const ICP_Mat<float>& cSceneToBirdEyeMat,
                                                 const ICP_Mat<float>& cBirdEyePixelsToMetersMat,
                                                       ICP_Mat<float>& rTempPointMat1,
                                                       ICP_Mat<float>& rTempPointMat2,
                                                 const float  cVehiclePositionY,
                                                       float& rVehiclePositionMeters);

   /*!*********************************************************************************
    *  @brief Method used to decide if we should keep tracking current vehicle.
    */
   static LIB_RESULT ShouldWeKeepTracking(const ICP_Rectangle& rTrackingRectangle,
                                          const ICP_Line& rSceneLeftLane,
                                          const ICP_Line& rSceneRightLane,
                                          const float cVehicleToLaneWidthRatio,
                                          const int32_t cMinRectangleHeight,
                                          bool& rWeKeepTracking);


   /*!*********************************************************************************
    *  @brief Method used by the vehicle tracker
    */
   static LIB_RESULT Sobel3x3_RowSum(void* const cpSceneRoiCached,
                                     const icp::DataDescriptor& rcSceneRoiCdd,
                                     void* const cpGradientsCached,
                                     icp::DataDescriptor& rGrandientsCdd,
                                     const int16_t cDelta);


   /*!*********************************************************************************
    *  @brief Method used by the vehicle tracker
    */
   static LIB_RESULT Median5x1_HRep(void* const cpGradientCached,
                                    const icp::DataDescriptor& rcCcdOfGradient,
                                    void* const cpMedianCached,
                                          icp::DataDescriptor& rcCcdOfMedian);


   /*!*********************************************************************************
    *  @brief Method used by the vehicle tracker
    */
   static LIB_RESULT Median5(uint16_t cpArray[5]);


   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   static LIB_RESULT SendRequest(const int32_t cMessageId,
                                 const int32_t cSenderQueueId,
                                 const int32_t cReceiverQueueId);




   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   static LIB_RESULT SendImageReply(const int32_t cReceiverQueueId,
                                    vsdk::UMat&   rSceneRgb24_umat,
                                    const void*   pcSceneRgb24_umat_data,
                                    vsdk::UMat&   rScene_umat,
                                    const void*   pcScene_umat_data,
                                    const uint32_t cSceneCounter,
                                    const uint64_t (&cTicks)[2]);


   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   static LIB_RESULT SendDetectionData(const int32_t cReceiverQueueId,
                                       vsdk::UMat&   rSceneRgb24_umat,
                                       const void*   pcSceneRgb24_umat_data,
                                       vsdk::UMat&   rScene_umat,
                                       const void*   pcScene_umat_data,
                                       const uint32_t cSceneCounter,
                                       void*    cpTopViewImage,
                                       const ICP_Line& rcLeftLane,
                                       const ICP_Line& rcRightLane,
                                       const bool cNoLanesFound,
                                       const bool cWeDepartedLeft,
                                       const bool cWeDepartedRight,
                                       const uint64_t (&cTicks)[gcApexCvEndTick]);


   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   static LIB_RESULT SendCompositionData(const int32_t cReceiverQueueId,
                                         vsdk::UMat&   rSceneRgb24_umat,
                                         const void*   pcSceneRgb24_umat_data);


   static LIB_RESULT FreeImageReply(GdcLdw_ImageReply* const cpReply);
   static LIB_RESULT FreeDetectionData(GdcLdw_DetectionData* const cpData);
   static LIB_RESULT FreeCompositionData(GdcLdw_CompositionData* const cpData);

   /*!*********************************************************************************
    *  @brief Function used to render a string of RGB24 formast buffer
    *
    */
   static void RenderTextToRgb24Buffer(int32_t const cFrameWidthInPixels,
                                       int32_t const cFrameHeightInPixels,
                                       uint32_t const cHorizontalOffsetInPixels,
                                       uint32_t const cVerticalOffsetInPixels,
                                       const uint8_t* const cpText,
                                       uint32_t const cTextColor,
                                       uint8_t* const cpBuffer,
                                       uint32_t const cScale = 1);


   /*!*********************************************************************************
    *  @brief Method used to get the size in bits of a pixel
    */
   //static int32_t ConvertToBitsPerPixel(const icp::DataDescriptor cColorModel);
   static int32_t ConvertToBitsPerPixel(const icp::DATATYPE cColorModel);

   static LIB_RESULT CropAndConvertRgb24ToY(vsdk::UMat& rSensorRgb24_umat,
                                            vsdk::UMat& rSceneRgb24_umat,
                                            vsdk::UMat& rSceneY_umat,
                                            uint32_t const cR2Y,
                                            uint32_t const cG2Y,
                                            uint32_t const cB2Y);

  /*!*********************************************************************************
  *  @brief Function used to convert a set of Y pixels to the RGB24 pixel format
  */
   static LIB_RESULT ConvertYToRgb24(vsdk::UMat& rSource_umat,
                                     vsdk::UMat& rDestination_umat);

   /*!*********************************************************************************
    *  @brief Function used to convert a set of RGB24 pixels to the Y pixel format
    *  It is required since apexcv::ColorConverter does not support RGB24 to Y conversion
    */
   static LIB_RESULT ConvertRgb24ToY(      uint32_t const cSourceWidthInPixels,
                                    uint32_t const cSourceHeightInLines,
                                    uint32_t const (* const cpcSourceRoi)[4],  // Left, Top, Width, Height
                              const uint8_t* const cpcSourceFrame,
                              icp::DataDescriptor& rDestinationDd,
                                    uint32_t const cR2Y,
                                    uint32_t const cG2Y,
                                    uint32_t const cB2Y);

   /*!*********************************************************************************
    *  @brief Function used to crop a RGB24 image
    */
   static LIB_RESULT CropRgb24(uint32_t const cSourceWidthInPixels,
                        uint32_t const cSourceHeightInLines,
                        uint32_t const (* const cpcSourceRoi)[4],  // Left, Top, Width, Height
                        const uint8_t* const cpcSourceFrame,
                              uint8_t* const cpcDestinationFrame);

   /*!*********************************************************************************
    *  @brief Calculates the optimal sensor region of interest(ROI), based on the scene dimensions
    */
   static LIB_RESULT CalculateSensorRoi(const uint32_t  cSensorWidthInPixels,
                                        const uint32_t  cSensorHeightInPixels,
                                        const uint32_t  cSceneWidthInPixels,
                                        const uint32_t  cSceneHeightInPixels,
                               /*out*/  vsdk::Rect& rRect);


   /*!*********************************************************************************
    *  @brief Function used to ...
    *
    */
   static LIB_RESULT ReverseCopyRgb24(void* const cpDestination,
                                      void* const cpSource,
                                      uint32_t const cWidthInPixels,
                                      uint32_t const cHeightInPixels);


    /*!*********************************************************************************
  *  @brief Replaces the pixels within a line from ImageBuffer between the two points specified 
  */
   static LIB_RESULT DrawLineToRgb24Buffer(const int32_t cImageWidthInPixels,
                                           const int32_t cImageHeightInPixels,
                                           const int32_t cLineX1,
                                           const int32_t cLineY1,
                                           const int32_t cLineX2,
                                           const int32_t cLineY2,
                                           uint32_t const cColorValue,
                                           void* const cpImageBuffer);

 /*!*********************************************************************************
  *  @brief Replaces the pixels within DestinationFrame if there is a pixel on the SourceFrame that is not transparent
  */
   static LIB_RESULT DrawIcon(const icp::DATATYPE cColorModel,
                              int32_t const cDotX,
                              int32_t const cDotY,
                              int32_t const cDotWidth,
                              int32_t const cDotHeight,
                              int32_t const cFrameWidth,
                              int32_t const cFrameHeight,
                              char const cpSourceIcon[],
                              void* const cpDestinationFrame);

/*!*********************************************************************************
*  @brief Replaces the pixels within DestinationFrame with the pixels in the SourceFrame with in the dimensions specified
*/
   static LIB_RESULT DrawTopView(const icp::DATATYPE cColorModel,
                                 int32_t const cDotX,
                                 int32_t const cDotY,
                                 int32_t const cDotWidth,
                                 int32_t const cDotHeight,
                                 int32_t const cFrameWidth,
                                 int32_t const cFrameHeight,
                                 void*  const cpSourceFrame,
                                 void*  const cpDestinationFrame);

/*!*********************************************************************************
*  @brief Replaces the pixel within Image pased at the cordinates indecated
*/
   static void PutPixel(const uint32_t cX,
                        const uint32_t cY,
                        const uint32_t cSpanInBytes,
                        const uint32_t cBitsPerPixel,
                              uint8_t* cpImageInUint8,
                        const uint32_t cColorValue, 
                        const uint32_t cMaxHeight);

/*!*********************************************************************************
*  @brief read raw image data from file
*/
   static LIB_RESULT readRawImage(const char *pString,
                                  char *pImageBuffer,
                                  const uint32_t cSceneBufferSizeInBytes);

   /*!*********************************************************************************
   *  @brief draw Logo on outputscreen
   */

   static void SetLogo(int32_t const cWidthInPixels,
                       int32_t const /*cHeightInPixels*/,
                       uint32_t const cHorizontalOffsetInPixels,
                       uint32_t const cVerticalOffsetInPixels,
                       uint8_t* const cpBuffer);

};

#endif /* GDCLDWTYPES_HPP */
