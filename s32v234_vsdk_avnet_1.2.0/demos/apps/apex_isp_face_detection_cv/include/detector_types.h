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

#ifndef DETECTORTYPES_H
#define DETECTORTYPES_H
#include <kalman_filter.h>

typedef int32_t STAGE_FIXED_POINT_TYPE;

const int STAGE_FRACTIONAL_BITS = 28;
const int STAGE_FIXED_POINT_MULTIPLIER = (1 << STAGE_FRACTIONAL_BITS);

struct Size
{
  int width, height;
  
  Size(int w, int h) : width(w), height(h){}
  Size() : width(0), height(0){}
};

struct lbpFeature
{
  int rectangleIndex;
  int values[8];

  STAGE_FIXED_POINT_TYPE leafValuesFixed[2];

};

struct lbpRectangle
{
  int x, y, width, height;
};

struct lbpStage
{
  int firstFeatureIndex;
  int featureCount;
  STAGE_FIXED_POINT_TYPE thresholdFixed;
};

struct APEX_lbpFeature
{
  int32_t values[8];
  STAGE_FIXED_POINT_TYPE leafValues[2];
  uint8_t x, y, width, height;
};

struct APEX_lbpStage
{
  uint8_t featureCount;
  STAGE_FIXED_POINT_TYPE threshold;
};

struct FaceRectangle
{
  int x, y;
  int width, height;

  FaceRectangle(int aX, int aY, int aWidth, int aHeight) : x(aX), y(aY), width(aWidth), height(aHeight){}

  bool ContainsCenter(const FaceRectangle& otherRectangle) const
  {
    int cxOffset = otherRectangle.x + (otherRectangle.width >> 1) - x;
    int cyOffset = otherRectangle.y + (otherRectangle.height >> 1) - y;

    return (cxOffset >= 0 && cxOffset <= width && cyOffset >= 0 && cyOffset <= height);
  }
};

struct DetectedFace
{
  FaceRectangle testingFaceRectangle;

  FaceRectangle minFaceRectangle;
  FaceRectangle maxFaceRectangle;

  int faceRectangleCount;

  DetectedFace(const FaceRectangle& faceRectangle):
    testingFaceRectangle(faceRectangle),
    minFaceRectangle(faceRectangle),
    maxFaceRectangle(faceRectangle),
    faceRectangleCount(1)
  {}

  bool TryAddFaceRectangle(const FaceRectangle& faceRectangle)
  {
    if (!faceRectangle.ContainsCenter(testingFaceRectangle))
    {
      return false;
    }

    if (faceRectangle.x < minFaceRectangle.x) minFaceRectangle.x = faceRectangle.x;
    if (faceRectangle.y < minFaceRectangle.y) minFaceRectangle.y = faceRectangle.y;
    if (faceRectangle.width < minFaceRectangle.width) minFaceRectangle.width = faceRectangle.width;
    if (faceRectangle.height < minFaceRectangle.height) minFaceRectangle.height = faceRectangle.height;

    if (faceRectangle.x > maxFaceRectangle.x) maxFaceRectangle.x = faceRectangle.x;
    if (faceRectangle.y > maxFaceRectangle.y) maxFaceRectangle.y = faceRectangle.y;
    if (faceRectangle.width > maxFaceRectangle.width) maxFaceRectangle.width = faceRectangle.width;
    if (faceRectangle.height > maxFaceRectangle.height) maxFaceRectangle.height = faceRectangle.height;

    ++faceRectangleCount;

    return true;
  }

  FaceRectangle GetMidFaceRectangle()
  {
    return FaceRectangle(
      (minFaceRectangle.x + maxFaceRectangle.x) >> 1,
      (minFaceRectangle.y + maxFaceRectangle.y) >> 1,
      (minFaceRectangle.width + maxFaceRectangle.width) >> 1,
      (minFaceRectangle.height + maxFaceRectangle.height) >> 1);
  }
};

struct DetectedFaceFiltered
{
  DetectedFace _detectedFace;

  int framesNotDetected;
  int framesDetected;
  unsigned char color[3];
  
  KalmanFilter kalman_x;
  KalmanFilter kalman_y;
  KalmanFilter kalman_w;
  KalmanFilter kalman_h;
  
  DetectedFaceFiltered(DetectedFace& detectedFace):
    _detectedFace(detectedFace),
    framesNotDetected(0),
        framesDetected(0)
  {
          FaceRectangle fr = detectedFace.GetMidFaceRectangle();
          kalman_x.Initialize(0.01, 0.001, fr.x + (fr.width/2));
          kalman_y.Initialize(0.01, 0.001, fr.y + (fr.height/2));
          kalman_w.Initialize(0.5, 0.05, fr.width);
          kalman_h.Initialize(0.5, 0.05, fr.height);
          
          color[0] = 0;
          color[1] = 0;
          color[2] = 0;
  }

  bool Update(DetectedFace& detectedFace)
  {
          FaceRectangle fr = detectedFace.GetMidFaceRectangle();
        kalman_x.NextMeasurement(fr.x + (fr.width/2));
        kalman_y.NextMeasurement(fr.y + (fr.height/2));
        kalman_w.NextMeasurement(fr.width);
        kalman_h.NextMeasurement(fr.height);
    
        
        _detectedFace.testingFaceRectangle.width = kalman_w.GetValue();
        _detectedFace.testingFaceRectangle.height = kalman_h.GetValue();
        _detectedFace.testingFaceRectangle.x = kalman_x.GetValue() - (kalman_w.GetValue()/2);
        _detectedFace.testingFaceRectangle.y = kalman_y.GetValue() - (kalman_h.GetValue()/2);

        framesNotDetected = 0;
        framesDetected += 1;
    return true;
  }

/*  FaceRectangle GetMidFaceRectangle()
  {
    return FaceRectangle(
      (minFaceRectangle.x + maxFaceRectangle.x) >> 1,
      (minFaceRectangle.y + maxFaceRectangle.y) >> 1,
      (minFaceRectangle.width + maxFaceRectangle.width) >> 1,
      (minFaceRectangle.height + maxFaceRectangle.height) >> 1);
  }*/
};


#endif /* DETECTORTYPES_H */
