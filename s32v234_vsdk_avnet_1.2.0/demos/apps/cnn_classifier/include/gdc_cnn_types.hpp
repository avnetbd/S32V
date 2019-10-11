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

#ifndef GDCCNNTYPES_HPP
#define GDCCNNTYPES_HPP


/***************************************************************************/
/* Include files. */
#include "gdc_cnn_configuration.hpp"
#include "GDC_OAL_Semaphore.hpp"
#include "sdi.hpp"
#include "frame_io_types.h"
#ifdef __STANDALONE__
#include "frame_output_dcu.h"
typedef io::FrameOutputDCU io_FrameOutput_t;
#else
#include "frame_output_v234fb.h"
typedef io::FrameOutputV234Fb io_FrameOutput_t;
#endif
#include "icp_data.h"
#include "frame_io_types.h"
#include "oal.h"
#include <global_errors.h>
#include "common_time_measure.h"
#include <opencv2/core/core.hpp>
#include <time.h>
#include <map>
#include <set>
#include <vector>
#include <stdint.h>
#include <stdio.h>


class GdcCnn_ImageReply;
class GdcCnn_ResizeData;
class GdcCnn_ClassifierData;
class GdcCnn_CompositionData;


/*!*********************************************************************************
*  @file
*  @brief Support functions/types for this application.
*
*  This file contains support classes, types and functions related to this application
*
***********************************************************************************/

//**************************************************************************/
// Literals & definitions
#define SHOW_ERROR(expr) if(!(expr)) { printf("%s : %s [%d]\n", __FILE__, __PRETTY_FUNCTION__, __LINE__); }

//#define GDC_TRACE fprintf(stderr, "%s - %u\n", __PRETTY_FUNCTION__, __LINE__);
#define GDC_TRACE



class GdcCnn_MovingAverage
{
public:
   GdcCnn_MovingAverage();
   ~GdcCnn_MovingAverage();

   void Initialize(const uint32_t cCount_Max);
   uint64_t Update(const uint64_t cNewValue); // Returns new average
   uint64_t Get() const;

private:
   uint64_t mSum;
   uint64_t mOldestValue;
   uint32_t mCount_Current;
   uint32_t mCount_Max;
};


/*
 *  \brief Class which hides the complexities of the composition
 *
 */
class GdcCnn_Composition
{
public:
   GdcCnn_Composition();
   ~GdcCnn_Composition();

   LIB_RESULT Initialize(const uint32_t cWidthInPixels,
                         const uint32_t cHeightInPixels,
                         const uint32_t cBitsPerPixel,
                         const uint32_t cScaleNumerator,
                         const uint32_t cScaleDenominator,
                         const bool cWeCompareWithReference);
   LIB_RESULT Deinitialize();

   LIB_RESULT Compose(GdcCnn_ClassifierData& rData,
                      void* const cpCompositionBuffer,
                      uint32_t const cCompositionBufferSizeInBytes,
                      icp::DataDescriptor& rCompositionDd,
                      uint32_t& rRightY); // Need this to know where to draw display performance info...

public:
   LIB_RESULT BufferSizeInBytes_Get(uint32_t& rBufferSizeInBytes);

private:
   uint32_t mClassificationCounter_Top1;
   uint32_t mClassificationCounter_Top5;
   uint32_t mClassificationCounter_Total;

   GdcCnn_MovingAverage mMovingAverage_Scene;
   GdcCnn_MovingAverage mMovingAverage_Resize;
   GdcCnn_MovingAverage mMovingAverage_Padding;
   GdcCnn_MovingAverage mMovingAverage_Classifier;
   GdcCnn_MovingAverage mMovingAverage_SqueezeNet;
   GdcCnn_MovingAverage mMovingAverage_SqueezeNetApex;
   GdcCnn_MovingAverage mMovingAverage_Composition;
   GdcCnn_MovingAverage mMovingAverage_FramePeriod;

   uint64_t mFrameTicks;

private:
   uint32_t mWidthInPixels;
   uint32_t mHeightInPixels;
   uint32_t mBitsPerPixel;

   uint32_t mScaleNumerator;
   uint32_t mScaleDenominator;

   uint32_t mCompositionBufferSizeInBytes;

   bool mWeCompareWithReference;
   bool mIsInitialized;
};



/*
 *  \brief Class which hides the complexities of the io::FrameOutputV234Fb class
 *
 */
class GdcCnn_Display
{
public:
   GdcCnn_Display();
   ~GdcCnn_Display();

   LIB_RESULT Initialize(const uint32_t cWidthInPixels,
                         const uint32_t cHeightInPixels,
                         const uint32_t cBitsPerPixel,
                         const uint32_t cScaleNumerator,
                         const uint32_t cScaleDenominator,
                         const bool cWeRotateDisplay);
   LIB_RESULT Deinitialize();

   LIB_RESULT PutFrame(GdcCnn_CompositionData& rData);

private:
   GdcCnn_MovingAverage mMovingAverage_Display;


private:
   uint32_t mWidthInPixels;
   uint32_t mHeightInPixels;
   uint32_t mBitsPerPixel;

   uint32_t mScaleNumerator;
   uint32_t mScaleDenominator;

   bool     mWeRotateDisplay;

   uint32_t mDisplayBufferSizeInBytes;
   void*    mpDisplayBuffer_Rotated;

   io_FrameOutput_t mFrameOutput; // Used to display image on device

   bool mIsInitialized;
};



typedef void* GdcCnn_TCB; ///< opaque value returned to the task creator to be used when referring to a task
const uint32_t gcGdcCnnTaskNameMaximumSizeInChar = 32;
const char gcGdcCnnTaskSemaphoreThreadSafetyName[] = "TaskTS";


class GdcCnn_Task  // Abstract Task Class
{
/**
*  \addtogroup GdcCnn_Task OAL Task
*  @{
*/
public:
   /***************************************************************************/
   /**
    * \details
    * This function initializes the data members of the object
    *
    **************************************************************************/
   GdcCnn_Task();



   /***************************************************************************/
   /**
    * \details
    * This function frees any outstanding resources in the objects possession.
    *
    **************************************************************************/
   virtual ~GdcCnn_Task();



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
   GdcCnn_TCB           mTaskControlBlock;
   uint32_t             mStackSizeInBytes;
   uint32_t             mPriority;   // Lower the value; higher the priority
   char                 mName[gcGdcCnnTaskNameMaximumSizeInChar];
   GDC_OAL_SEMAPHORE    mSemaphoreThreadSafety;
   bool                 mIsSemaphoreThreadSafetyCreated;
   bool                 mIsTaskSpawned;
   bool                 mIsTaskFinished;

   //TODO: was void instead of void*...
   static void* TaskEntryRoutine(void* pVoid);

private:
   GdcCnn_Task( const GdcCnn_Task& );            // Do not implement
   GdcCnn_Task& operator=( const GdcCnn_Task& ); // Do not implement
}; // End of GdcCnn_Task declaration.






/*!*********************************************************************************
*  @brief Class used to gather utility function needed by the application or its tasks.
 */
namespace GdcCnn_Helper
{


   /*!*********************************************************************************
    *  @brief Function used to ...
    *
    */
   LIB_RESULT ReadDatasetListFromFile(const char* pcDatasetList_Filename,
                                      std::vector<std::string>& rDatasetList);


   /*!*********************************************************************************
    *  @brief Function used to ...
    *
    */
   LIB_RESULT ReadWeightModelFromFile(const char* pcSqueezeNetModel_Filename,
                                      int8_t*& rpSqueezeNet_WeightModel);


   /*!*********************************************************************************
    *  @brief Function used to ...
    *
    */
   LIB_RESULT ReadSynonymSetFromFile(const char* pcSqueezeNetSynonymSet_Filename,
                                     std::vector<std::string>& rSqueezeNet_SynonymSet);


   /*!*********************************************************************************
    *  @brief Function used to ...
    *
    */
   LIB_RESULT ReadReferenceResultsFromFile(const char* pcSqueezeNetReferenceResults_Filename,
                                           std::map<std::string, std::set<int32_t>>& rSqueezeNet_ReferenceResults);


   /*!*********************************************************************************
    *  @brief Function used to ...
    *  
    */
   LIB_RESULT MemCopy2D(void* const cpDestination,
                        void* const cpSource,
                        uint32_t const cDestinationSpanInBytes,
                        uint32_t const cDestinationHorizontalOffsetInPixels,
                        uint32_t const cDestinationVerticalOffsetInPixels,
                        uint32_t const cDestinationBytesPerPixel,
                        uint32_t const cSourceBytesPerPixel,
                        uint32_t const cSourceWidthInPixels,
                        uint32_t const cSourceHeightInPixels);


   /*!*********************************************************************************
    *  @brief Function used to ...
    *
    */
   LIB_RESULT ReverseCopyRgb24(void* const cpDestination,
                               void* const cpSource,
                               uint32_t const cWidthInPixels,
                               uint32_t const cHeightInPixels);


   /*!*********************************************************************************
    *  @brief Function used to crop a RGB24 image
    */
   LIB_RESULT CropRgb24(uint32_t const cSourceWidthInPixels,
                        uint32_t const cSourceHeightInLines,
                        uint32_t const (* const cpcSourceRoi)[4],  // Left, Top, Width, Height
                        const uint8_t* const cpcSourceFrame,
                              uint8_t* const cpcDestinationFrame);


   void SetLogo(int32_t const cWidthInPixels,
                uint32_t const cHorizontalOffsetInPixels,
                uint32_t const cVerticalOffsetInPixels,
                uint8_t* const cpBuffer);


   //**************************************************************************/
   // Message passing
   //**************************************************************************/

   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   LIB_RESULT SendRequest(const int32_t cMessageId,
                          const int32_t cSenderQueueId,
                          const int32_t cReceiverQueueId);

   LIB_RESULT SendRequest(const int32_t cMessageId,
                          const int32_t cSenderQueueId,
                          const int32_t cReceiverQueueId,
                          const int32_t cPriority);




   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   LIB_RESULT SendImageReply(const int32_t      cReceiverQueueId,
                             cv::Mat&           rSceneMat,
                             const uint32_t     cSceneCounter,
                             const std::string& rcSceneName,
                             const uint64_t (&cTicks)[gcApexCvEndTick]);



   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   LIB_RESULT SendResizeData(const int32_t        cReceiverQueueId,
                             const cv::Mat&       rSceneMat,
                             const uint32_t       cSceneCounter,
                             const std::string&   rcSceneName,
                             vsdk::UMat&        rPaddedScene_umat,
                             const void*        pcPaddedScene_umat_data,
                             const uint64_t     (&cResizeTicks)[gcApexCvEndTick]);



   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   LIB_RESULT SendClassifierData(const int32_t  cReceiverQueueId,
                                 const cv::Mat& rcSceneMat,
                                 const uint32_t cSceneCounter,
                                 const uint16_t    (&rcClassificationIndexes_Local)[gcSqueezeNet_NumberOfClassifications],
                                 const std::string (&rcClassificationLabels_Local)[gcSqueezeNet_NumberOfClassifications],
                                 const float       (&rcClassificationScores_Local)[gcSqueezeNet_NumberOfClassifications],
                                 const int32_t      rcClassificationIndex_Reference,
                                 const std::string& rcClassificationLabel_Reference,
                                 const uint64_t (&rcClassificationTicks)[gcApexCvEndTick]);


   /*!*********************************************************************************
    *  @brief Method used to send a message using inter-task communications
    */
   LIB_RESULT SendCompositionData(const int32_t       cReceiverQueueId,
                                  void* const         cpComposition,
                                  /*const*/ icp::DataDescriptor& rcCompositionDd,
                                  const uint32_t      cRightY,
                                  const uint32_t cSceneCounter,
	                              const uint64_t (&rcTicks)[gcApexCvEndTick]);


   LIB_RESULT FreeImageReply(GdcCnn_ImageReply* const cpReply);
   LIB_RESULT FreeResizeData(GdcCnn_ResizeData* const cpData);
   LIB_RESULT FreeClassifierData(GdcCnn_ClassifierData* const cpData);



   timespec timespec_diff(timespec const cMinuend,
                          timespec const cSubtrahend);

   GdcCnn_SceneTask_Enum ConvertToSceneTaskEnum(const uint32_t cValue);

   void trim(std::string& rString);

   void tokenize(const std::string&        rcString,
                 const std::string&        rcDelimiters,
                 std::vector<std::string>& rTokens);

   LIB_RESULT transformUMat(vsdk::UMat& InputImage, apexdnnTensorDescriptor* Tensor);
}


#endif /* GDCCNNTYPES_HPP */
