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

#ifndef GDCFEATURETYPESVERIFICATION_HPP
#define GDCFEATURETYPESVERIFICATION_HPP


/***************************************************************************/
/* Include files. */
#include "gdc_ft_types.hpp"


/*!*********************************************************************************
*  @file
*  @brief Support functions for this application.
*
*  This file contains support classes and functions related to the application verification
*
***********************************************************************************/

//**************************************************************************/
// Literals & definitions



//**************************************************************************/
// Class declaration.



/*!*********************************************************************************
*  @brief Class used to write or read output sequence to/from files.
 */
class GdcFt_OutputFile
{
public:
   GdcFt_OutputFile();
   ~GdcFt_OutputFile();

   LIB_RESULT Open(const char* const cpcFilenameWrite,
                   const char* const cpcFilenameRead);

   LIB_RESULT Close();

protected:
   FILE* mpFileWrite;
   FILE* mpFileRead;
};




/*!*********************************************************************************
*  @brief structure storing output values generated by the algorithm for one image.
 *  \details The output structure is used for regression testing.
 *  It is used to store the output for a video sequence and/or compare
 *  a reference output to the current output if modification were made to the algorithm.
 */
struct GdcFt_Output_HarrisCorners
{
   uint32_t               mSceneCounter;
   void*                  mhFeatureDescriptorOal;
   icp::FeatureDescriptor mFeatureDescriptor;
   bool                   mWeFreeBuffers;

public:
   GdcFt_Output_HarrisCorners();
   ~GdcFt_Output_HarrisCorners();
};



bool operator==(const icp::Feature& rcLeft, const icp::Feature& rcRight);
bool operator==(const GdcFt_Output_HarrisCorners& rcLeft, const GdcFt_Output_HarrisCorners& rcRight);



/*!*********************************************************************************
*  @brief Class used to write or read output sequence to/from files.
 */
class GdcFt_OutputFile_HarrisCorners : public GdcFt_OutputFile
{
public:
   GdcFt_OutputFile_HarrisCorners();
   ~GdcFt_OutputFile_HarrisCorners();

   LIB_RESULT Write(GdcFt_Output_HarrisCorners& rOutput);

   LIB_RESULT Read(GdcFt_Output_HarrisCorners& rOutput);
};



/*!*********************************************************************************
*  @brief structure storing output values generated by the algorithm for one image.
 *  \details The output structure is used for regression testing.
 *  It is used to store the output for a video sequence and/or compare
 *  a reference output to the current output if modification were made to the algorithm.
 */
struct GdcFt_Output_PyramidMultiCreation
{
   uint32_t            mSceneCounter;
   void*               mhImagePyramidOal[gcDepthOfImagePyramid];
   icp::DataDescriptor mImagePyramidDd[gcDepthOfImagePyramid];
   bool                mWeFreeBuffers;

public:
   GdcFt_Output_PyramidMultiCreation();
   ~GdcFt_Output_PyramidMultiCreation();
};



bool operator==(const GdcFt_Output_PyramidMultiCreation& rcLeft, const GdcFt_Output_PyramidMultiCreation& rcRight);



/*!*********************************************************************************
*  @brief Class used to write or read output sequence to/from files.
 */
class GdcFt_OutputFile_PyramidMultiCreation : public GdcFt_OutputFile
{
public:
   GdcFt_OutputFile_PyramidMultiCreation();
   ~GdcFt_OutputFile_PyramidMultiCreation();

   LIB_RESULT Write(GdcFt_Output_PyramidMultiCreation& rOutput);

   LIB_RESULT Read(GdcFt_Output_PyramidMultiCreation& rOutput);
};


/*!*********************************************************************************
*  @brief structure storing output values generated by the algorithm for one image.
 *  \details The output structure is used for regression testing.
 *  It is used to store the output for a video sequence and/or compare
 *  a reference output to the current output if modification were made to the algorithm.
 */
struct GdcFt_Output_LKTrackerOpticalFlow
{
   uint32_t                  mSceneCounter;
   void*                     mhFeature32SDescriptorOal;
   icp::Feature32SDescriptor mFeature32SDescriptor;
   bool                      mWeFreeBuffers;

public:
   GdcFt_Output_LKTrackerOpticalFlow();
   ~GdcFt_Output_LKTrackerOpticalFlow();
};



bool operator==(const icp::Feature32S& rcLeft, const icp::Feature32S& rcRight);
bool operator==(const GdcFt_Output_LKTrackerOpticalFlow& rcLeft, const GdcFt_Output_LKTrackerOpticalFlow& rcRight);



/*!*********************************************************************************
*  @brief Class used to write or read output sequence to/from files.
 */
class GdcFt_OutputFile_LKTrackerOpticalFlow : public GdcFt_OutputFile
{
public:
   GdcFt_OutputFile_LKTrackerOpticalFlow();
   ~GdcFt_OutputFile_LKTrackerOpticalFlow();

   LIB_RESULT Write(GdcFt_Output_LKTrackerOpticalFlow& rOutput);

   LIB_RESULT Read(GdcFt_Output_LKTrackerOpticalFlow& rOutput);
};



/*!*********************************************************************************
*  @brief structure storing output values generated by the algorithm for one image.
 *  \details The output structure is used for regression testing.
 *  It is used to store the output for a video sequence and/or compare
 *  a reference output to the current output if modification were made to the algorithm.
 */
struct GdcFt_Output_FeatureTracking
{
   uint32_t               mSceneCounter;
   uint32_t               mNumberOfDetectedFeatures;
   GdcFt_TrackedFeature** mppTrackedFeatures;
   bool                   mWeFreeBuffers;

public:
   GdcFt_Output_FeatureTracking();
   ~GdcFt_Output_FeatureTracking();
};



bool operator==(const GdcFt_TrackedFeature& rcLeft, const GdcFt_TrackedFeature& rcRight);
bool operator==(const GdcFt_Output_FeatureTracking& rcLeft, const GdcFt_Output_FeatureTracking& rcRight);



/*!*********************************************************************************
*  @brief Class used to write or read output sequence to/from files.
 */
class GdcFt_OutputFile_FeatureTracking : public GdcFt_OutputFile
{
public:
   GdcFt_OutputFile_FeatureTracking();
   ~GdcFt_OutputFile_FeatureTracking();

   LIB_RESULT Write(GdcFt_Output_FeatureTracking& rOutput);

   LIB_RESULT Read(GdcFt_Output_FeatureTracking& rOutput);
};



/*!*********************************************************************************
*  @brief Class used to gather utility function needed by the verification application or its tasks.
 */
namespace GdcFt_Helper
{
   void PrintMismatch_HarrisCorners(FILE* const cpFile,
                                    const GdcFt_Output_HarrisCorners& rcLocalOutput,
                                    const GdcFt_Output_HarrisCorners& rcReferenceOutput);

   void PrintMismatch_PyramidMultiCreation(FILE* const cpFile,
                                           const GdcFt_Output_PyramidMultiCreation& rcLocalOutput,
                                           const GdcFt_Output_PyramidMultiCreation& rcReferenceOutput);

   void PrintMismatch_LKTrackerOpticalFlow(FILE* const cpFile,
                                           const GdcFt_Output_LKTrackerOpticalFlow& rcLocalOutput,
                                           const GdcFt_Output_LKTrackerOpticalFlow& rcReferenceOutput);

   void PrintMismatch_FeatureTracking(FILE* const cpFile,
                                      const GdcFt_Output_FeatureTracking& rcLocalOutput,
                                      const GdcFt_Output_FeatureTracking& rcReferenceOutput);

   bool LessThan(const icp::Feature& rcLeft,
                 const icp::Feature& rcRight);

   bool LessThan32S(const icp::Feature32S& rcLeft,
                    const icp::Feature32S& rcRight);
}


#endif /* GDCFEATURETYPESVERIFICATION_HPP */