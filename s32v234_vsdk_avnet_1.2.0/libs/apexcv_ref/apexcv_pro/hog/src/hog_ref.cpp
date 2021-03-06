/**************************************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright 2013-2017 NXP
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
**************************************************************************************************/

#include <oal.h>
#include <stdio.h>
#include <string.h>
#include "hog_ref.h"
#include "hog_ref_exec.h"
#include "hog_types_ref.h"
#include "hog_config.h"
#include <unistd.h>
#include "string.h"



/******** Fixed Point Macros ***********************************/

// 16-bit fixed point arithmetic
const uint TWO_HIST_Q = HIST_Q << 1;
const uint HIST_TO_DESC_Q = TWO_HIST_Q - DESC_Q;
const uint SVM_Q = SCOR_Q - FUNC_Q;
const fix32u EPSILON = fxcast32u(0.2, TWO_HIST_Q);

void HOG_setDefaultPeopleDetectorParams(HOG_Params& params)
{
  params.cellSize.width = 8;
  params.cellSize.height = 8;
  params.nCells.width = 1;
  params.nCells.height = 1;
  params.nBlocks.width = 8;
  params.nBlocks.height = 16;
  params.blockStride.width = 8;
  params.blockStride.height = 8;
}

#ifdef __cplusplus

/*********************** Cache Allocation/Deletion **************************/
void HOG_allocateImageCache(HOG_ImageCache& imageCache,
                            const HOG_Params& params,
                            const Rect& imageROI)
{
  HOG_allocateGradientCache(imageCache.grad, imageROI);

  const int blockDataBufferSize = AREA(params.nBlocks);
  VECTOR_RESERVE(imageCache.blockData, blockDataBufferSize, BlockData);

  const int blockArea = AREA(params.cellSize) * AREA(params.nCells);
  VECTOR_RESERVE(imageCache.imgOffsets, blockArea, int16_t);
}

void HOG_deleteImageCache(HOG_ImageCache& imageCache)
{
  HOG_deleteGradientCache(imageCache.grad);
  VECTOR_DELETE(imageCache.blockData);
  VECTOR_DELETE(imageCache.imgOffsets);
}

void HOG_allocateScanCache(HOG_ScanCache& scanCache,
                           const HOG_Params& params,
                           const Rect& imgROI,
                           const Size& cacheStride)
{
  int blockHistogramSize = AREA(params.nCells) * params.nBins;
  int blockWidth = params.cellSize.width * params.nCells.width;
  int blockHeight = params.cellSize.height * params.nCells.height;
  int windowHeight = blockHeight * params.nBlocks.height;

  Size cacheSize;
  cacheSize.width = (imgROI.width - blockWidth) / cacheStride.width + 1;
  cacheSize.height = (windowHeight / cacheStride.height) + 1;

  const int cacheArea = AREA(cacheSize);

  const int blockCacheBufferSize = cacheArea * blockHistogramSize;

  MATRIX_RESERVE(scanCache.histograms, blockCacheBufferSize, fix8u);
  MATRIX_RESERVE(scanCache.histogramComputed, cacheArea, uchar);
  VECTOR_RESERVE(scanCache.ymaxCached, cacheSize.height, int32_t);
}

void HOG_deleteScanCache(HOG_ScanCache& scanCache)
{
  VECTOR_DELETE(scanCache.ymaxCached);
  MATRIX_DELETE(scanCache.histogramComputed);
  MATRIX_DELETE(scanCache.histograms);
}

void HOG_allocateCache(HOG_Detector& detector,
                       const HOG_Params& params,
                       const Rect& imageROI,
                       const Size& winStride)
{
  Size adjustedWindowStride = (winStride.width <= 0 || winStride.height <= 0) ? params.cellSize : winStride;
  HOG_allocateImageCache(detector.imageCache, params, imageROI);
  Size cacheStride = HOG_getCacheStride(adjustedWindowStride, params.blockStride);
  HOG_allocateScanCache(detector.scanCache, params, imageROI, cacheStride);
}

void HOG_deleteCache(HOG_Detector& detector)
{
  HOG_deleteImageCache(detector.imageCache);
  HOG_deleteScanCache(detector.scanCache);
}

#endif   // __cplusplus

/************************ Cache Initialization **************************/

void HOG_getDetectionWindowSize(Size& detectionWindowSize, const HOG_Params& params)
{
  detectionWindowSize.width = (params.nBlocks.width - 1) * params.blockStride.width + (params.cellSize.width * params.nCells.width);
  detectionWindowSize.height = (params.nBlocks.height - 1) * params.blockStride.height + (params.cellSize.height * params.nCells.height);
}

void HOG_configureImageCache(HOG_ImageCache& cache,
                             const HOG_Params& params,
                             const Mat8u& image,
                             const Rect& imageROI)
{
  bool earlyExit = (!memcmp(&params, &cache.params, sizeof(HOG_Params)) &&    // HOG parameters are the same
                    imageROI.width == cache.grad.magnitude.cols &&            // gradient image width is the same
                    imageROI.height == cache.grad.magnitude.rows);            // gradient image height is the same

  HOG_computeGradient(cache.grad, image, imageROI);             // Gradient image is computed here

  if(earlyExit)
  {
    return;
  }

  cache.params = params;

  /******** Derived Constants ***********************************/

  cache.blockSize.width = params.cellSize.width * params.nCells.width;
  cache.blockSize.height = params.cellSize.height * params.nCells.height;
  cache.windowSize.width = (params.nBlocks.width - 1) * params.blockStride.width + cache.blockSize.width;
  cache.windowSize.height = (params.nBlocks.height - 1) * params.blockStride.height + cache.blockSize.height;
  cache.blockHistogramSize = params.nBins * params.nCells.width * params.nCells.height;
  cache.descriptorSize = cache.blockHistogramSize * params.nBlocks.width * params.nBlocks.height;

  // Assert the image size is at least as large as the detection window size
  HOG_ASSERT(imageROI.width >= cache.windowSize.width && imageROI.height >= cache.windowSize.height);

  // Compute the gaussian weights
  int blockArea = AREA(cache.blockSize);

  // 2. Compute the gradient image offsets
  VECTOR_RESIZE(cache.imgOffsets, blockArea);
  int16_t* iPtr = cache.imgOffsets.data;

  for(int i = 0; i < cache.blockSize.height; ++i)
  {
    int gradRowOffset = cache.grad.magnitude.cols * i;      // depends on the gradient image width

    for(int j = 0; j < cache.blockSize.width; ++j, ++iPtr)
    {
      *iPtr = (int16_t)(gradRowOffset + j);
    }
  }

  VECTOR_RESIZE(cache.blockData, AREA(cache.params.nBlocks));
  BlockData* blockDataPtr = cache.blockData.data;
  int histOffset = 0;

  // Order the block histograms and SVM decision function by columns
  int xOffset = 0;

  for(int j = 0; j < cache.params.nBlocks.width; ++j, xOffset += cache.params.blockStride.width)
  {
    int yOffset = 0;

    for(int i = 0; i < cache.params.nBlocks.height; ++i, yOffset += cache.params.blockStride.height, ++blockDataPtr, histOffset += cache.blockHistogramSize)
    {
      BlockData& blockData = *blockDataPtr;
      // offset in the HOG Descriptor for block (i,j)
      blockData.histOfs = histOffset;
      // top left pixel location corresponding to block (i,j)
      blockData.imgOffset.x = xOffset;
      blockData.imgOffset.y = yOffset;
    }
  }
}

void HOG_configureCache(HOG_ImageCache& imageCache,
                        HOG_ScanCache& scanCache,
                        const HOG_Params& params,
                        const Mat8u& image,
                        const Rect& imageROI,
                        const Size& winStride)
{
  HOG_configureImageCache(imageCache,
                          params,
                          image,
                          imageROI);

  HOG_ASSERT(winStride.width > 0 && winStride.height > 0);

  scanCache.windowStride = winStride;
  scanCache.cacheStride = HOG_getCacheStride(scanCache.windowStride, params.blockStride);

  Size cacheSize;
  cacheSize.width = (imageCache.grad.magnitude.cols - imageCache.blockSize.width) / scanCache.cacheStride.width + 1;
  cacheSize.height = (imageCache.windowSize.height / scanCache.cacheStride.height) + 1;

  scanCache.nWindows.width = (imageCache.grad.magnitude.cols - imageCache.windowSize.width) / scanCache.windowStride.width + 1;
  scanCache.nWindows.height = (imageCache.grad.magnitude.rows - imageCache.windowSize.height) / scanCache.windowStride.height + 1;

  MATRIX_RESIZE(scanCache.histograms, cacheSize, imageCache.blockHistogramSize);
  MATRIX_RESIZE(scanCache.histogramComputed, cacheSize, 1);
  VECTOR_RESIZE(scanCache.ymaxCached, scanCache.histograms.rows);

  for(int i = 0; i < scanCache.ymaxCached.size; ++i)
  {
    scanCache.ymaxCached.data[i] = -1;
  }
}

/************************ HOG Descriptor Computation **************************/

void normalizeL2(fix8u* normalizedHist, const fix16u* hist, int histSize)
{
  // L2-Normalization of the block histogram using a quasi-floating point method

  // 1.  Get the sum of the squared histogram elements
  fix32u sum = EPSILON;

  for(int i = 0; i < histSize; i++)
  {
    fix32u h = hist[i];
    sum = fxadd(sum, fxmul(h, h));
  }

  fix16u scale;
  uint R;
  fxinvsqrt(scale, R, sum, TWO_HIST_Q);
  scale = fxcvt(scale, R, HIST_Q);

  for(int i = 0; i < histSize; i++)
  {
    normalizedHist[i] = fxcast8(fxsr((fix32u)hist[i] * (fix32u)scale, HIST_TO_DESC_Q));
  }
}

void HOG_getBlockHistogram(fix8u* blockHistogram, const HOG_ImageCache& imageCache, const Point& point)
{
  // Compute the block histogram

  //Assert the location is within range of the image
  HOG_ASSERT((unsigned)point.x <= (unsigned)(imageCache.grad.magnitude.cols - imageCache.blockSize.width) &&
             (unsigned)point.y <= (unsigned)(imageCache.grad.magnitude.rows - imageCache.blockSize.height));

  HOG_ASSERT(blockHistogram != 0);

  const int blockHistogramSize = imageCache.params.nBins * imageCache.params.nCells.width * imageCache.params.nCells.height;
  HOG_ASSERT(imageCache.blockHistogramSize <= blockHistogramSize);
  fix16u* blockHist = new fix16u[blockHistogramSize];
  // Set each bin of the histogram to zero
  memset(blockHist, 0, imageCache.blockHistogramSize * sizeof(fix16u));

  const uchar* gradPtr;
  MAT_PTR(gradPtr, imageCache.grad.magnitude, point);    // gradient image pointer, given the absolute pixel offset point

  const uchar* qanglePtr;
  MAT_PTR(qanglePtr, imageCache.grad.qangle, point);     // quantized angle pointer, given the absolute pixel offset

  const int blockArea = AREA(imageCache.blockSize);
  const int N_CELLS = 1;

  // iterate over all blocks in which all four cells count towards the block histogram
  for(int k = 0; k < blockArea; k++)
  {
    const int16_t imgOffset = imageCache.imgOffsets.data[k];
    const fix16u m = gradPtr[imgOffset];                     // gradient for the k'th pixel
    fix16u* hist = blockHist + qanglePtr[imgOffset];       // pointer the orientation been of the k'th pixel

    for(int c = 0; c < N_CELLS; ++c, hist += imageCache.params.nBins)
    {
      *hist = fxadd(*hist, m);
    }
  }

  normalizeL2(blockHistogram, blockHist, imageCache.blockHistogramSize);
  delete[] blockHist;
}

fix8u* HOG_getBlockHistogram(HOG_ImageCache& imageCache, HOG_ScanCache& scanCache, const Point& location)
{
  // Assert the specified point corresponds to one of the cached blocks
  HOG_ASSERT(location.x % scanCache.cacheStride.width == 0 &&
             location.y % scanCache.cacheStride.height == 0);

  Point cacheIdx;
  cacheIdx.x = location.x / scanCache.cacheStride.width;      // horizontal and vertical cache indeces, given the pixel offset
  cacheIdx.y = (location.y / scanCache.cacheStride.height) % scanCache.histograms.rows;

  if(location.y != scanCache.ymaxCached.data[cacheIdx.y])
  {
    memset(scanCache.histogramComputed.data + cacheIdx.y * scanCache.histogramComputed.step, 0, scanCache.histogramComputed.step);
    scanCache.ymaxCached.data[cacheIdx.y] = location.y;
  }

  fix8u* blockHist;
  MAT_PTR(blockHist, scanCache.histograms, cacheIdx);       // pointer to the histogram for this block
  uchar* computedFlagPtr;
  MAT_PTR(computedFlagPtr, scanCache.histogramComputed, cacheIdx);
  uchar& computedFlag = *computedFlagPtr;

  if(!computedFlag)
  {
    // The histogram for this block has not yet been computed, so computed it and set the flag to 1
    HOG_getBlockHistogram(blockHist, imageCache, location);
    computedFlag = (uchar)1;
  }

  return blockHist;
}

void HOG_compute(VecFix8u& descriptors,
                 HOG_ImageCache& imageCache,
                 const VecPoint& locations)
{
  // This function computes the HOG params of an image at specified locations OR using a default window stride

  int nwindows = locations.size;
  int nblocks = AREA(imageCache.params.nBlocks);

  VECTOR_RESIZE(descriptors, imageCache.descriptorSize * nwindows);

  Point maxPt;
  maxPt.x = imageCache.grad.magnitude.cols - imageCache.windowSize.width;
  maxPt.y = imageCache.grad.magnitude.rows - imageCache.windowSize.height;

  for(int i = 0; i < nwindows; i++)
  {
    fix8u* descriptor = descriptors.data + (i * imageCache.descriptorSize);
    const Point& pt0 = locations.data[i];

    if(pt0.x < 0 || pt0.x > maxPt.x || pt0.y < 0 || pt0.y > maxPt.y)
    {
      continue;
    }

    for(int j = 0; j < nblocks; j++)
    {
      const BlockData& bj = imageCache.blockData.data[j];

      Point pt = pt0;
      pt.x += bj.imgOffset.x;
      pt.y += bj.imgOffset.y;

      fix8u* dst = descriptor + bj.histOfs;

      HOG_getBlockHistogram(dst, imageCache, pt);
    }
  }
}

void detectionWindowLocation(Point& loc, int idx, const HOG_ImageCache& imageCache, const HOG_ScanCache& scanCache)
{
  // Returns the top-left corner of the idx^th detection window, given the image size and window stride

  int y = idx / scanCache.nWindows.width;      // vertical   offset for this window (in units of number of window strides)
  int x = idx - scanCache.nWindows.width * y;  // horizontal offset for this window (in units of number of window strides)

  loc.x = x * scanCache.windowStride.width;
  loc.y = y * scanCache.windowStride.height;

  HOG_ASSERT(loc.x % scanCache.cacheStride.width == 0 && loc.y % scanCache.cacheStride.height == 0);
}

void HOG_compute(VecFix8u& descriptors,
                 HOG_ImageCache& imageCache,
                 HOG_ScanCache& scanCache)
{
  // This function computes the HOG params of an image at specified locations OR using a default window stride

  int nwindows = AREA(scanCache.nWindows);
  int nblocks = AREA(imageCache.params.nBlocks);
  int blockHistogramSize = imageCache.blockHistogramSize;

  VECTOR_RESIZE(descriptors, imageCache.descriptorSize * nwindows);

  for(int i = 0; i < nwindows; i++)
  {
    fix8u* descriptor = descriptors.data + (i * imageCache.descriptorSize);

    Point loc;
    detectionWindowLocation(loc, i, imageCache, scanCache);

    for(int j = 0; j < nblocks; j++)
    {
      const BlockData& bj = imageCache.blockData.data[j];

      Point pt = loc;
      pt.x += bj.imgOffset.x;
      pt.y += bj.imgOffset.y;

      fix8u* dst = descriptor + bj.histOfs;
      fix8u* blockHistogram = HOG_getBlockHistogram(imageCache, scanCache, pt);
      memcpy(dst, blockHistogram, blockHistogramSize * sizeof(fix8u));
    }
  }
}

void HOG_compute2(VecFix8u& blockHistograms,
                  HOG_ImageCache& imageCache,
                  HOG_ScanCache& scanCache)
{
  // This function computes the HOG params of an image using a default window stride
  const int imageRows = imageCache.grad.magnitude.rows;
  const int imageCols = imageCache.grad.magnitude.cols;
  const int yStep = scanCache.windowStride.height;
  const int xStep = scanCache.windowStride.width;
  const int histogramCols = (imageCols - (imageCache.blockSize.width - scanCache.windowStride.width)) / scanCache.windowStride.width;
  const int histogramRows = (imageRows - (imageCache.blockSize.height - scanCache.windowStride.height)) / scanCache.windowStride.height;
  const int blockHistogramSize = imageCache.blockHistogramSize;

  VECTOR_RESIZE(blockHistograms, histogramCols * histogramRows * blockHistogramSize);

  fix8u* hist = blockHistograms.data;
  const int yMax = imageRows - imageCache.blockSize.height;
  const int xMax = imageCols - imageCache.blockSize.width;

  Point p;

  for(p.y = 0; p.y <= yMax; p.y += yStep)
  {
    for(p.x = 0; p.x <= xMax; p.x += xStep, hist += blockHistogramSize)
    {
      fix8u* tempHist = HOG_getBlockHistogram(imageCache, scanCache, p);
      memcpy(hist, tempHist, imageCache.blockHistogramSize);
    }
  }
}


/************************ HOG Detector **************************/

void HOG_detect(VecPoint& foundLocations,
                VecFix16& scores,
                HOG_ImageCache& imageCache,
                const VecPoint& searchLocations,
                VecFix8u& descriptorBuffer,
                const HOG_DecisionFunction& decisionFunction,
                fix16s threshold)
{
  // This function computes the HOG params of an image at specified locations OR using a default window stride

  int nblocks = AREA(imageCache.params.nBlocks);

  HOG_ASSERT(decisionFunction.normal.data && decisionFunction.normal.size >= imageCache.descriptorSize);
  HOG_ASSERT(descriptorBuffer.data && descriptorBuffer.capacity >= imageCache.descriptorSize);
  HOG_ASSERT(foundLocations.data && foundLocations.capacity >= searchLocations.size);
  HOG_ASSERT(scores.data && scores.capacity >= searchLocations.size);

  foundLocations.size = 0;
  scores.size = 0;

  Point maxPt;
  maxPt.x = imageCache.grad.magnitude.cols - imageCache.windowSize.width;
  maxPt.y = imageCache.grad.magnitude.rows - imageCache.windowSize.height;

  for(int i = 0; i < searchLocations.size; i++)
  {
    const Point& loc = searchLocations.data[i];

    if(loc.x < 0 || loc.x > maxPt.x || loc.y < 0 || loc.y > maxPt.y)
    {
      continue;
    }

    fix16s score = decisionFunction.offset;
    const fix8s* func = decisionFunction.normal.data;

    for(int j = 0; j < nblocks; j++, func += imageCache.blockHistogramSize)
    {
      const BlockData& bj = imageCache.blockData.data[j];

      Point pt = loc;
      pt.x += bj.imgOffset.x;
      pt.y += bj.imgOffset.y;

      fix8u* hist = descriptorBuffer.data + bj.histOfs;

      HOG_getBlockHistogram(hist, imageCache, pt);

      fix32s t_score = 0;

      for(int k = 0; k < imageCache.blockHistogramSize; ++k)
      {
        t_score = fxadd(t_score, (fix32s)fxmul((fix16s)hist[k], (fix16s)func[k]));
      }

      score = fxadd(score, (fix16s)fxsr(t_score, SVM_Q));
    }

    if(score >= threshold)
    {
      VECTOR_PUSH_BACK(foundLocations, loc);
      VECTOR_PUSH_BACK(scores, score);
    }
  }
}

void HOG_detect(VecPoint& foundLocations,
                VecFix16& scores,
                HOG_ImageCache& imageCache,
                HOG_ScanCache& scanCache,
                const HOG_DecisionFunction& decisionFunction,
                fix16s threshold)
{
  // This function computes the HOG params of an image at specified locations OR using a default window stride

  int nwindows = AREA(scanCache.nWindows);
  int nblocks = AREA(imageCache.params.nBlocks);

  HOG_ASSERT(decisionFunction.normal.data && decisionFunction.normal.size == imageCache.descriptorSize);
  HOG_ASSERT(foundLocations.data && foundLocations.capacity > 0);
  HOG_ASSERT(scores.data);

  VECTOR_RESIZE(foundLocations, 0);
  VECTOR_RESIZE(scores, 0);

  for(int i = 0; i < nwindows; i++)
  {
    Point loc;
    detectionWindowLocation(loc, i, imageCache, scanCache);
    HOG_ASSERT(loc.x >= 0 && loc.y >= 0 &&
               loc.x <= imageCache.grad.magnitude.cols - imageCache.windowSize.width &&
               loc.y <= imageCache.grad.magnitude.rows - imageCache.windowSize.height);

    fix16s score = decisionFunction.offset;
    const fix8s* func = decisionFunction.normal.data;

    for(int j = 0; j < nblocks; j++, func += imageCache.blockHistogramSize)
    {
      const BlockData& bj = imageCache.blockData.data[j];

      Point pt = loc;
      pt.x += bj.imgOffset.x;
      pt.y += bj.imgOffset.y;

      fix8u* hist = HOG_getBlockHistogram(imageCache, scanCache, pt);

      fix32s t_score = 0;

      for(int k = 0; k < imageCache.blockHistogramSize; ++k)
      {
        t_score = fxadd(t_score, (fix32s)fxmul((fix16s)hist[k], (fix16s)func[k]));
      }

      score = fxadd(score, (fix16s)fxsr(t_score, SVM_Q));
    }

    if(score >= threshold)
    {
      VECTOR_PUSH_BACK(foundLocations, loc);
      VECTOR_PUSH_BACK(scores, score);
    }
  }
}

void HOG_configureDetector(HOG_Detector& detector,
                           const HOG_Params& params,
                           const Mat8u& image,
                           const Rect& imageROI,
                           const Size& winStride)
{
  Size adjustedWindowStride = (winStride.width <= 0 || winStride.height <= 0) ? params.cellSize : winStride;
  HOG_configureCache(detector.imageCache, detector.scanCache, params, image, imageROI, adjustedWindowStride);
}

void HOG_compute(VecFix8u& descriptors,
                 HOG_Detector& detector,
                 const Mat8u& image,
                 const Rect& imageROI,
                 const Size& winStride)
{
  HOG_configureDetector(detector, detector.imageCache.params, image, imageROI, winStride);
  HOG_compute(descriptors, detector.imageCache, detector.scanCache);
}

void HOG_compute2(VecFix8u& blockHistograms,
                  HOG_Detector& detector,
                  const Mat8u& image,
                  const Rect& imageROI,
                  const Size& winStride)
{
  HOG_configureDetector(detector, detector.imageCache.params, image, imageROI, winStride);
  HOG_compute2(blockHistograms, detector.imageCache, detector.scanCache);
}

void HOG_detect(VecPoint& foundLocations,
                VecFix16& scores,
                HOG_Detector& detector,
                const Mat8u& image,
                const Rect& imageROI,
                const Size& winStride,
                fix16s threshold)
{
  HOG_configureDetector(detector, detector.imageCache.params, image, imageROI, winStride);
  HOG_detect(foundLocations, scores, detector.imageCache, detector.scanCache, detector.decisionFunction, threshold);
}


namespace ref
{
/***********************
 * Default constructor
 **********************/
Hog::Hog()
{
}

/**************
 * Destructor
 *************/
Hog::~Hog()
{
}

/*************************
 * Block Histogram Process
 *************************/
APEXCV_LIB_RESULT Hog::GetBlockHistograms(const vsdk::UMat& aInput, vsdk::UMat& aOutput)
{

  aOutput = vsdk::UMat((aInput.rows - (DefaultParams.block.height - DefaultParams.winStride.height)) / DefaultParams.winStride.height,
                       ((aInput.cols - (DefaultParams.block.width - DefaultParams.winStride.width)) / DefaultParams.winStride.width) * N_BINS,
                       VSDK_CV_8UC1);

  if(nullptr == aOutput.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data)
  {
    return APEXCV_ERROR_MEMORY_ALLOCATION_FAILED;
  }

  if(!aInput.empty())
  {
    HOG_Params lHogParams;

    lHogParams.cellSize.width = 8;
    lHogParams.cellSize.height = 8;
    lHogParams.nCells.width = 1;
    lHogParams.nCells.height = 1;
    lHogParams.nBlocks.width = 8;
    lHogParams.nBlocks.height = 16;
    lHogParams.blockStride.width = 8;
    lHogParams.blockStride.height = 8;

    hog_descriptor_ref_exec((uint8_t*)aOutput.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data,
                            (const uint8_t*)aInput.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data,
                            aInput.step[0],
                            aInput.rows,
                            aInput.cols,
                            lHogParams);
  }
  return APEXCV_SUCCESS;
}

/**********************************
 * APU HOG Descriptors Computation
 *********************************/
APEXCV_LIB_RESULT Hog::GetDescriptors(const vsdk::UMat &aInput, vsdk::UMat &aOutput)
{

  vsdk::UMat blockHist;
  int outRows = (aInput.rows - (DefaultParams.winPixels.height - DefaultParams.winStride.height)) / DefaultParams.winStride.height;
  int outCols = (aInput.cols - (DefaultParams.winPixels.width - DefaultParams.winStride.width)) / DefaultParams.winStride.width;
  int descriptorSize = GetDescriptorSize();

  aOutput = vsdk::UMat(outRows, outCols * descriptorSize, VSDK_CV_8UC1);

  if(nullptr == aOutput.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data)
  {
    return APEXCV_ERROR_MEMORY_ALLOCATION_FAILED;
  }

  APEXCV_LIB_RESULT retVal = GetBlockHistograms(aInput, blockHist);
  if(APEXCV_SUCCESS != retVal)
  {
    return retVal;
  }

  // Build descriptor by concatenating block histograms column-wise
  // Assume block stride == block size
  vsdk::Mat outMat = aOutput.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
  vsdk::Mat blockMat = blockHist.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
  for(int row = 0; row < outRows; ++row)
  {
    for(int col = 0; col < outCols; ++col)
    {
      // Pointer to output descriptor
      uint8_t* pOut = outMat.data + row * outMat.step[0] + col * descriptorSize;

      // Pointer to block histogram in upper left corner of detection window
      uint8_t* pBlock = blockMat.data + row * blockMat.step[0] + col * N_BINS;

      // For each histogram in the window
      for(int x = 0; x < DefaultParams.winBlocks.width; ++x)
      {
        int colOffset = x * DefaultParams.winStridePerBlock.width * N_BINS;

        for(int y = 0; y < DefaultParams.winBlocks.height; ++y)
        {
          int rowOffset = y * DefaultParams.winStridePerBlock.height * blockMat.step[0];

          memcpy(pOut, pBlock + colOffset + rowOffset, N_BINS);
          pOut += N_BINS;
        }
      }
    }
  }

  return APEXCV_SUCCESS;
}

/*******************
 * Detection Process
 *******************/
APEXCV_LIB_RESULT Hog::Detect(const vsdk::UMat &aInput, vsdk::UMat &aOutput)
{

  aOutput = vsdk::UMat(
        (aInput.rows - (DefaultParams.winPixels.height - DefaultParams.winStride.height)) / DefaultParams.winStride.height,
        (aInput.cols - (DefaultParams.winPixels.width - DefaultParams.winStride.width)) / DefaultParams.winStride.width,
        VSDK_CV_16SC1);

  if(nullptr == aOutput.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data)
  {
    return APEXCV_ERROR_MEMORY_ALLOCATION_FAILED;
  }

  if(!aInput.empty() &&
     !mSVM.empty())
  {
    HOG_Params lHogParams;

    lHogParams.cellSize.width = 8;
    lHogParams.cellSize.height = 8;
    lHogParams.nCells.width = 1;
    lHogParams.nCells.height = 1;
    lHogParams.nBlocks.width = 8;
    lHogParams.nBlocks.height = 16;
    lHogParams.blockStride.width = 8;
    lHogParams.blockStride.height = 8;

    hog_ref_exec((int16_t*)aOutput.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data,
                 (uint8_t*)aInput.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data,
                 aInput.step[0],
                 aInput.rows,
                 aInput.cols,
                 lHogParams,
                 (const int8_t*)mSVM.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data);

  }

  return APEXCV_SUCCESS;
}



/**************************************************
 * Set the linear SVM classifier decision function
 *************************************************/
APEXCV_LIB_RESULT Hog::SetSVM(const vsdk::UMat& aSVM)
{
  // aSVM is an array of 8200 bytes = 1025*sizeof(double)
  //                        1024               1
  const int cSvmSize = DefaultParams.svm.width * DefaultParams.svm.height;

  if((size_t(cSvmSize + 1) != aSVM.total()) || (VSDK_CV_64FC1 != aSVM.type()))
  {
    return APEXCV_ERROR_INVALID_ARGUMENT;
  }

  // Allocate for SVM function, +2 for 16-bit offset
  mSVM = vsdk::UMat(DefaultParams.svm.height, DefaultParams.svm.width + 2, VSDK_CV_8SC1);

  if(nullptr == mSVM.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data)
  {
    return APEXCV_ERROR_MEMORY_ALLOCATION_FAILED;
  }

  vsdk::Mat mSVM_mat = mSVM.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
  int8_t *pSVM = (int8_t*)mSVM_mat.data;

  vsdk::Mat doubleSVM_mat = aSVM.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
  double *pDoubleSVM = (double*) doubleSVM_mat.data;

  for(int i = 0; i < cSvmSize; ++i)
  {
    // Convert weights from double to Q0.7
    pSVM[i] = (int8_t)(pDoubleSVM[i] * 128.0 - 0.5);
  }

  // Convert offset from double to Q4.11
  const int16_t cOffset = (int16_t)(pDoubleSVM[cSvmSize] * 2048.0 - 0.5);

  memcpy(&pSVM[cSvmSize], &cOffset, 2);

  return APEXCV_SUCCESS;
}

/***************************************
 * Get fixed-point SVM decision function
 ***************************************/
APEXCV_LIB_RESULT Hog::GetSVM(vsdk::UMat& aSVM, int16_t& offset)
{
  if(mSVM.empty())
  {
    return APEXCV_ERROR_OBJECT_ISNOT_INITIALIZED;
  }

  size_t svmSize = GetDescriptorSize();

  aSVM = vsdk::UMat(mSVM,
      vsdk::Rect(0, 0, DefaultParams.svm.height, DefaultParams.svm.width));

  if(nullptr == aSVM.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED).data)
  {
    return APEXCV_ERROR_MEMORY_ALLOCATION_FAILED;
  }

  // Copy 16-bit offset
  vsdk::Mat SVM_Mat = mSVM.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
  int8_t* pSVM = (int8_t*)SVM_Mat.data;
  memcpy(&offset, &pSVM[svmSize], 2);

  return APEXCV_SUCCESS;
}

/**************************************************
 * Get the number of elements in the HOG descriptor
 **************************************************/
size_t Hog::GetDescriptorSize()
{
  return DefaultParams.svm.width * DefaultParams.svm.height;
}


}  /* namespace apex */

