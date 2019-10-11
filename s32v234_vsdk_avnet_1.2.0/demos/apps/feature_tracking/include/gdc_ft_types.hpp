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
*
***********************************************************************************/

#ifndef GDCFEATURETRACKINGTYPES_HPP
#define GDCFEATURETRACKINGTYPES_HPP


/***************************************************************************/
/* Include files. */
#include "gdc_ft_configuration.hpp"
#include "GDC_OAL_Semaphore.hpp"
#include "icp_feature.h"
#include "icp_feature_32s.h"
#include "sdi.hpp"
#include "frame_io_types.h"
#ifdef __STANDALONE__
#include "frame_output_dcu.h"
typedef io::FrameOutputDCU io_FrameOutput_t;
#else
#include "frame_output_v234fb.h"
typedef io::FrameOutputV234Fb io_FrameOutput_t;
#endif
#include "umat.hpp"
#include "icp_data.h"
#include "frame_io_types.h"
#include "oal.h"
#include <global_errors.h>
#include "common_time_measure.h"
#include <opencv2/core/core.hpp>
#include <time.h>
#include <vector>
#include <stdint.h>
#include <stdio.h>


class GdcFt_ImageReply;
class GdcFt_DetectorData;
class GdcFt_PyramidData;
class GdcFt_TrackerData;
class GdcFt_CompositionData;


/*!*********************************************************************************
*  @file
*  @brief Support functions for this application.
*
*  This file contains support classes and functions related to this application
*
***********************************************************************************/

//**************************************************************************/
// Literals & definitions
#define SHOW_ERROR(expr) if(!(expr)) { printf("%s : %s [%d]\n", __FILE__, __PRETTY_FUNCTION__, __LINE__); }

//#define GDC_TRACE fprintf(stderr, "%s - %u\n", __PRETTY_FUNCTION__, __LINE__);
#define GDC_TRACE


//**************************************************************************/
// Class declaration.
class GdcFt_MovingAverage
{
public:
   GdcFt_MovingAverage();
   ~GdcFt_MovingAverage();

   void Initialize(const uint32_t cCount_Max);
   uint64_t Update(const uint64_t cNewValue); // Returns new average
   uint64_t Get() const;

private:
   uint64_t mSum;
   uint64_t mOldestValue;
   uint32_t mCount_Current;
   uint32_t mCount_Max;
};


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



namespace GdcFt {
    class Size
    {
    public:
        int width;
        int height;

        Size() {};

        Size(int _w, int _h)
        {
            width = _w; height = _h;
        };

        ~Size() {};

        int area()
        {
            return width * height;
        };
    };
}



/**
 * \brief Struct that stores min and max values.
 *
 */
struct GdcFt_MinMax
{
   GdcFt_MinMax(const int32_t cMin,
                const int32_t cMax);

   int32_t mMin;
   int32_t mMax;
};



/**
 * \brief This class is an iterator which facilitates traversal
 *
 */
class GdcFt_TrackedFeatureIterator
{
   friend class GdcFt_TrackedFeature;

public:
   GdcFt_TrackedFeatureIterator();
   virtual ~GdcFt_TrackedFeatureIterator();

   bool HasNext();

   const ICP_Point* Next();

private:
   const ICP_Point* mpPositions;                // Pointer to the mPositions array of the TrackedFeature
   uint32_t         mPositionsIndex_Current;    // Start at the back and updated till hits front
   uint32_t         mPositionsCount;            // Counting up till reaches size
   uint32_t         mPositionsSize;             // Number of valid positions in mpPositions

private:
   GdcFt_TrackedFeatureIterator(const GdcFt_TrackedFeatureIterator&);            // Do not implement
   GdcFt_TrackedFeatureIterator& operator=(const GdcFt_TrackedFeatureIterator&); // Do not implement
};



/**
 * \brief Class that stores a detected feature tracked across multiple frames.
 *
 */
class GdcFt_TrackedFeature
{
public:
   GdcFt_TrackedFeature();
   ~GdcFt_TrackedFeature();

   LIB_RESULT Update(/*const*/ icp::Feature32S& rcFeature);

   LIB_RESULT GetFront(ICP_Point& rPositionFront) const;
   LIB_RESULT GetBack(ICP_Point& rPositionBack) const;

   int32_t DisplacementAbs() const;              // Does NOT preserve direction
   int32_t DisplacementHistoryAbs();             // Does NOT preserve direction

   LIB_RESULT Iterator(GdcFt_TrackedFeatureIterator& rReturnedIterator) const;

public:
   icp::Feature32S mCurrent;
   uint32_t        mAgeInFramesSinceDetection;

   ICP_Point mPositions[gcTrackedFeatureHistoryLength];  // Circular array
   uint32_t  mPositionsIndex_Current;                    // Index of front element
   uint32_t  mPositionsSize;                             // Size should not exceed array size
};



/**
 * \brief Class that manages an allocation pool of tracked features.
 *
 */
class GdcFt_TrackedFeatureAllocator
{
public:
   GdcFt_TrackedFeatureAllocator();
   ~GdcFt_TrackedFeatureAllocator();

   LIB_RESULT Initialize(void* const cpBuffer,
                         uint32_t const cBufferSizeInBytes,
                         uint32_t const cMaxNumberOfFeatures,
                         uint32_t& rReturnedUsedSizeInBytes);

   LIB_RESULT Allocate(GdcFt_TrackedFeature*& rpReturnedObject);
   LIB_RESULT Deallocate(GdcFt_TrackedFeature*& rpObjectToBeDeallocated);

   uint32_t Capacity() const;
   uint32_t Size() const;

private:
   GdcFt_TrackedFeature* mpObjectArray;
   uint32_t*             mpFreeObjects;
   uint32_t              mNumberOfFreeObjects;
   uint32_t              mMaxNumberOfObjects;
   bool                  mIsInitialized;
};



/*
 *  \brief Class which hides the complexities of the io::FrameOutputV234Fb class
 *
 */
class GdcFt_Composition
{
public:
   GdcFt_Composition();
   ~GdcFt_Composition();

   LIB_RESULT Initialize(const bool cWeDrawFullPaths);
   LIB_RESULT Deinitialize();

   LIB_RESULT PutFrame(GdcFt_TrackerData& rData);

public:
   bool    mWeDrawFullPaths;

   GdcFt_MovingAverage mMovingAverage_FramePeriod;
   uint64_t mFrameTicks;
   uint64_t mDisplayTicks;

private:
   bool     mIsInitialized;
   uint64_t mAvgPerformance[3][32];
   uint32_t mAvgIndex;
};



/*
 *  \brief Class which hides the complexities of the io::FrameOutputV234Fb class
 *
 */
class GdcFt_Display
{
public:
   GdcFt_Display();
   ~GdcFt_Display();

   LIB_RESULT Initialize(const uint32_t cWidthInPixels,
                         const uint32_t cHeightInPixels,
                         const uint32_t cBitsPerPixel,
                         const bool cWeRotateDisplay);

   LIB_RESULT Deinitialize();

   LIB_RESULT PutFrame(GdcFt_CompositionData& rData);


private:
   uint32_t mDisplayWidthInPixels;
   uint32_t mDisplayHeightInPixels;
   uint32_t mDisplayBitsPerPixel;

   bool     mWeRotateDisplay;

   void*    mpSceneRgb24_Rotated;
   uint32_t mSceneRgb24_Rotated_SizeInBytes;

   void*    mpSceneRgb24_Resized;
   uint32_t mSceneRgb24_Resized_SizeInBytes;

   io_FrameOutput_t mFrameOutput; // Used to display image on device

   bool mIsInitialized;
};



typedef void* GdcFt_TCB; ///< opaque value returned to the task creator to be used when referring to a task
const uint32_t gcGdcFtTaskNameMaximumSizeInChar = 32;
const char gcGdcFtTaskSemaphoreThreadSafetyName[] = "TaskTS";


class GdcFt_Task  // Abstract Task Class
{
/**
*  \addtogroup GdcFt_Task OAL Task
*  @{
*/
public:
   /***************************************************************************/
   /**
    * \details
    * This function initializes the data members of the object
    *
    **************************************************************************/
   GdcFt_Task();



   /***************************************************************************/
   /**
    * \details
    * This function frees any outstanding resources in the objects possession.
    *
    **************************************************************************/
   virtual ~GdcFt_Task();



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
   GdcFt_TCB            mTaskControlBlock;
   uint32_t             mStackSizeInBytes;
   uint32_t             mPriority;   // Lower the value; higher the priority
   char                 mName[gcGdcFtTaskNameMaximumSizeInChar];
   GDC_OAL_SEMAPHORE    mSemaphoreThreadSafety;
   bool                 mIsSemaphoreThreadSafetyCreated;
   bool                 mIsTaskSpawned;
   bool                 mIsTaskFinished;

   //TODO: was void instead of void*...
   static void* TaskEntryRoutine(void* pVoid);

private:
   GdcFt_Task( const GdcFt_Task& );            // Do not implement
   GdcFt_Task& operator=( const GdcFt_Task& ); // Do not implement
}; // End of GdcFt_Task declaration.






/*!*********************************************************************************
*  @brief Class used to gather utility function needed by the application or its tasks.
 */
namespace GdcFt_Helper
{
   /*!*********************************************************************************
    *  @brief Function used to set a logo of RGB24 formast buffer
    *  
    */
   void SetLogo( int32_t const cmWidthInPixels,
                 int32_t const cmHeightInPixels,
                 uint32_t const cHorizontalOffsetInPixels,
                 uint32_t const cVerticalOffsetInPixels,
                 uint8_t* const cpBuffer);

   /*!*********************************************************************************
    *  @brief Function used to render a string of RGB24 formast buffer
    *  
    */
   void RenderTextToRgb24Buffer(int32_t const cFrameWidthInPixels,
                                int32_t const cFrameHeightInPixels,
                                uint32_t const cHorizontalOffsetInPixels,
                                uint32_t const cVerticalOffsetInPixels,
                                const uint8_t* const cpText,
                                uint32_t const cTextColor,
                                uint8_t* const cpBuffer,
                                uint32_t const cScale = 1);


   /*!*********************************************************************************
    *  @brief Function used to convert a set of RGB24 pixels to the Y pixel format
    *  It is required since apexcv::ColorConverter does not support RGB24 to Y conversion
    */
   LIB_RESULT CropAndConvertRgb24ToY(vsdk::UMat& rSensorRgb24_umat,
                                     vsdk::UMat& rSceneRgb24_umat,
                                     vsdk::UMat& rSceneY_umat,
                                     uint32_t const cR2Y,
                                     uint32_t const cG2Y,
                                     uint32_t const cB2Y);


   /*!*********************************************************************************
    *  @brief Function used to convert a set of Y pixels to the RGB24 pixel format
    */
   LIB_RESULT ConvertYToRgb24(vsdk::UMat& rSource_umat,
                              vsdk::UMat& rDestination_umat);


   /*!*********************************************************************************
    *  @brief Function used to crop a RGB24 image
    */
   LIB_RESULT CropRgb24(uint32_t const cSourceWidthInPixels,
                        uint32_t const cSourceHeightInLines,
                        uint32_t const (* const cpcSourceRoi)[4],  // Left, Top, Width, Height
                        const uint8_t* const cpcSourceFrame,
                              uint8_t* const cpcDestinationFrame);



   /*!*********************************************************************************
    *  @brief Function used to generate different colors based on the slope of a lin
    */
   LIB_RESULT GetLineColorFromSlopeRgb24(const int32_t cLineX1,
                                         const int32_t cLineY1,
                                         const int32_t cLineX2,
                                         const int32_t cLineY2,
                                         uint32_t& rReturnedColorValueRgb24);


   /*!*********************************************************************************
    *  @brief Method used to draw a line into an image buffer
    */
   LIB_RESULT DrawLineRgb24(const int32_t cImageWidthInPixels,
                            const int32_t cImageHeightInPixels,
                            const int32_t cLineX1,
                            const int32_t cLineY1,
                            const int32_t cLineX2,
                            const int32_t cLineY2,
                            uint32_t const cColorValue,
                            void* const cpImageBuffer);


   /*!*********************************************************************************
    *  @brief Method used to draw a single pixel in an image buffer
    */
   void PutPixel(const uint32_t cX,
                 const uint32_t cY,
                 const uint32_t cImageWidthInPixels,
                 const uint32_t cImageHeightInPixels,
                 const uint32_t cSpanInBytes,
                 const uint32_t cBitsPerPixel,
                 uint8_t* const   cpImageInUint8,
                 const uint32_t cColorValue);

   /*!*********************************************************************************
    *  @brief Function used to draw a rectangle in an RGB24 image
    */
   LIB_RESULT DrawFeatureRgb24(uint8_t* const cpFrame,
                               int32_t const cFrameWidthInPixels,
                               int32_t const cFrameHeightInPixels,
                               icp::Feature32S& rFeature,
                               const uint32_t cSizeInPixels,
                               const uint32_t cRgb24Color);



   /*!*********************************************************************************
    *  @brief Function used to ...
    *
    */
   LIB_RESULT ReverseCopyRgb24(void* const cpDestination,
                               void* const cpSource,
                               uint32_t const cWidthInPixels,
                               uint32_t const cHeightInPixels);



   /*!*********************************************************************************
    *  @brief Calculates the optimal sensor region of interest(ROI), based on the scene dimensions
    */
   LIB_RESULT CalculateSensorRoi(const uint32_t  cSensorWidthInPixels,
                                 const uint32_t  cSensorHeightInPixels,
                                 const uint32_t  cSceneWidthInPixels,
                                 const uint32_t  cSceneHeightInPixels,
                        /*out*/  vsdk::Rect& rRect);


   /*!*********************************************************************************
    *  @brief Method used to calculate the absolute displacement between two points
    */
   int32_t DisplacementAbs(ICP_Point const cPoint1,
                           ICP_Point const cPoint2);

   /*!*********************************************************************************
    *  @brief Method used to filter out features which should no longer be tracked
    */
   LIB_RESULT FilterFeatures(GdcFt_TrackedFeatureAllocator& rTrackedFeatureAllocator,
                             const uint32_t cTrackedFeatureFilterGridWidth,
                             const uint32_t cTrackedFeatureFilterGridHeight,
                             GdcFt_TrackedFeature** const cppTrackedFeatureFilter,
                             GdcFt_TrackedFeature** const cppTrackedFeatures,
                             int32_t& rNumberOfTrackedFeatures,
                             /*const*/ icp::FeatureDescriptor& rcFeatureDescriptor,
                             const int16_t cTrackedFeatureStrength_Threshold,
                             const int16_t cDetectedFeatureStrength_Threshold);


   //**************************************************************************/
   // Message passing
   //**************************************************************************/

   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   LIB_RESULT SendRequest(const int32_t cMessageId,
                          const int32_t cSenderQueueId,
                          const int32_t cReceiverQueueId);




   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   LIB_RESULT SendImageReply(const int32_t cReceiverQueueId,
                             vsdk::UMat&   rSceneRgb24_umat,
                             const void*   pcSceneRgb24_umat_data,
                             vsdk::UMat&   rScene_umat,
                             const void*   pcScene_umat_data,
                             const uint32_t cSceneCounter,
                             const uint64_t (&cTicks)[2]);



   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   LIB_RESULT SendDetectorData(const int32_t cReceiverQueueId,
                               vsdk::UMat&   rSceneRgb24_umat,
                               const void*   pcSceneRgb24_umat_data,
                               vsdk::UMat&   rScene_umat,
                               const void*   pcScene_umat_data,
                               const uint32_t cSceneCounter,
                               void* const chFeatureDescriptorOal,
                               icp::FeatureDescriptor& rFeatureDescriptor,
                               const bool cWeDetectedFeatures,
                               const uint64_t (&cDetectorTicks)[gcApexCvEndTick]);



   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   LIB_RESULT SendPyramidData(const int32_t cReceiverQueueId,
                              vsdk::UMat&   rSceneRgb24_umat,
                              const void*   pcSceneRgb24_umat_data,
                              vsdk::UMat&   rScene_umat,
                              const void*   pcScene_umat_data,
                              const uint32_t cSceneCounter,
                              void* const chFeatureDescriptorOal,
                              icp::FeatureDescriptor& rFeatureDescriptor,
                              const bool cWeDetectedFeatures,
                              vsdk::UMat (&rImagePyramid_umat)[gcDepthOfImagePyramid],
                              const uint64_t (&cPyramidTicks)[gcApexCvEndTick]);



   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   LIB_RESULT SendTrackerData(const int32_t cReceiverQueueId,
                              vsdk::UMat&   rSceneRgb24_umat,
                              const void*   pcSceneRgb24_umat_data,
                              vsdk::UMat&   rScene_umat,
                              const void*   pcScene_umat_data,
                              const uint32_t cSceneCounter,
                              /*const*/ GdcFt_TrackedFeature** const ppcTrackedFeatures,
                              const uint32_t cNumberOfTrackedFeatures,
                              const uint32_t cNumberOfDetectedFeatures,
                              const uint64_t (&cTrackerTicks)[gcApexCvEndTick]);



   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   LIB_RESULT SendCompositionData(const int32_t cReceiverQueueId,
                                  vsdk::UMat&   rSceneRgb24_umat,
                                  const void*   pcSceneRgb24_umat_data,
	                              const uint32_t cSceneCounter,
                                  const uint64_t (&cTrackerTicks)[gcApexCvEndTick]);



   LIB_RESULT FreeImageReply(GdcFt_ImageReply* const cpReply);
   LIB_RESULT FreeDetectorData(GdcFt_DetectorData* const cpData);
   LIB_RESULT FreePyramidData(GdcFt_PyramidData* const cpData);
   LIB_RESULT FreeTrackerData(GdcFt_TrackerData* const cpData);
   LIB_RESULT FreeCompositionData(GdcFt_CompositionData* const cpData);
}


#endif /* GDCFEATURETRACKINGTYPES_HPP */
