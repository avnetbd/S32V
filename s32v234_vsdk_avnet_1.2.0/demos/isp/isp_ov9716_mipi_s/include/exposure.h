/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016-2018 NXP Semiconductors;
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

#ifndef __EXPOSURE_H__
#define __EXPOSURE_H__
#include "typedefs.h"
#include "isp_csi.h"                    // CSI_IDX
#include "isp_seq.h"                    // histogram types
#include "isp_cam_ov9716.h"
#ifndef __STANDALONE__
#include "unistd.h"                     // msleep/usleep
#endif

#ifdef __cplusplus
extern "C" {
#endif

// **************************************************************************
// constants
// **************************************************************************
#define EXPCYCLE    (2)
#define MINCHGAIN   (1.0)
#define CHGAINTAU   (32.0)

// **************************************************************************
// global variables
// **************************************************************************
extern OV9716_Exposure_t gOVExposure[4];

enum gParam     {Time=0, AG=1, DG=2, CG=3};
enum gParamSource {SourceCamera=0, SourceS32v=1};

// **************************************************************************
// macro
// **************************************************************************


#ifdef __STANDALONE__
extern "C" {
  unsigned long get_uptime_microS(void);
}
#define msleep(_val)                                    \
  {                                                     \
    unsigned long lStart = get_uptime_microS();         \
    while((get_uptime_microS() - lStart) < _val*1000);  \
  }

#else

#define msleep(_msec)(usleep(_msec*1000))

#endif

// **************************************************************************
// local functions
// **************************************************************************

void MyOV9716_Exposure_Print(CSI_IDX arCsiIdx, gParamSource arSource);

/************************************************************************/
/** set HDR IPUS engine index.
  *
  * \param  arHDREngine     IPUS Index for HDR engine to saet HDR Shift
  ************************************************************************/
void SetHDREngine(uint8_t arHDREngine);

/************************************************************************/
/** set number of maximum lines for exposure time.
  *
  * \param  arExpRange      range of valid exposure lines for l/S
  ************************************************************************/
void SetExpRange(uint16_t arExpRange);

/************************************************************************/
/** get number of maximum lines for exposure time.
 ************************************************************************/
uint16_t GetExpRange();

/************************************************************************/
/** set conversion gain mode.
  *
  * \param  newMode      0: on, 1: off, 2:auto
  ************************************************************************/
void SetConverGainMode(uint8_t arNewMode);

/************************************************************************/
/** get conversion gain mode.
 ************************************************************************/
uint8_t GetConverGainMode();

/************************************************************************/
/** set period of which AEC is applied
  *
  * \param  arAECPeriod      period in number of frame
  ************************************************************************/
void SetAECPeriod(uint8_t arAECPeriod);

/************************************************************************/
/** get AEC period.
 ************************************************************************/
uint8_t GetAECPeriod();

/************************************************************************/
/** set blue target value in AWB
  *
  * \param  arBlueTarget      blue target value
  ************************************************************************/
void SetBlueTarget(uint16_t arBlueTarget);

/************************************************************************/
/** get blue target value in AWB.
 ************************************************************************/
uint16_t GetBlueTarget();

/************************************************************************/
/** set red target value in AWB
  *
  * \param  arRedTarget      red target value
  ************************************************************************/
void SetRedTarget(uint16_t arRedTarget);

/************************************************************************/
/** get red target value in AWB.
 ************************************************************************/
uint16_t GetRedTarget();
/************************************************************************/
/** set deflicker on or off
  *
  * \param  arOnOff         0:off 1:on
  ************************************************************************/
void EnableDeflicker(uint8_t arOnOff);

/************************************************************************/
/** set de-flciker frequency.
  *
  * \param  arFrequency     in Hz. Line setting is computetd in fucntion
  * \param  arFps           camera FPS setting
  ************************************************************************/
void SetDeflickerHz(float arFrequency,float arFps);

/************************************************************************/
/** ExposureInit initialise exposure parameters of the 4 cameras with global variables
  *
  ************************************************************************/
void ExposureInit(void);

/************************************************************************/
/** Update exposure setup.
  *
  * \param  arCsiIdx       index to CSI port
  * \param  arExpCnt       number of exposure executions
  * \param  arIpuHist      Histogramn pointer for 2x 8x32 bins
  * \param  arLogLevel     print log level
  * \param  arAEC          AEC enabled
  * \param  arTargetBinLS    target bin to measure pixel count for combined exposure
  * \param  arTargetRatioLS  relatice target pixel count 0.0 - 1.0 for combined exposure
  * \param  arTargetBinVS    target bin to measure pixel count for very short exposure
  * \param  arTargetRatioVS  relatice target pixel count 0.0 - 1.0 for very short exposure
  * \param  arAWB          AWB enabled
  ************************************************************************/
void ExposureCtl(CSI_IDX arCsiIdx,
                 uint8_t *arExpCnt,
                 SEQ_IpuHist_t *arIpuHist,
                 uint8_t arLogLevel,
                 uint8_t arAECon,
                 uint8_t arTargetBinLS,
                 double  arTargetRatioLS,
                 uint8_t arTargetBinVS,
                 double  arTargetRatioVS,
                 uint8_t arAWBon
                 );

/************************************************************************/
/** ExposureTotal Conpute total exposure based on exposure time and gains
  *
  * \param  arExp       Exposure parameters structure
  * \param  arE         Exposure number (VS,LCG or HCG)
  ************************************************************************/
void MyOV9716_ExposureTotal(OV9716_Exposure_t &arExp,
                            OV9716_ExposureType_t aExpT);


/************************************************************************/
/** ExposureDCGComputation compute exposure parameter of the exposure based on Update value
 *
 * \param  arExp  Exposures parameters
 * \param  arNewHCGExpTotal  New HCG exposure
 ************************************************************************/

void MyOV9716_ExposureDCGComputation(OV9716_Exposure_t &arExp,
                                     float arNewHCGExpTotal);

/************************************************************************/
/** ExposureDCGRatioChange changes the ratio between HCG and LCG and compute new exposure parameters
 *
 * \param  arNewRatio  New HCG/LCG ratio
 ************************************************************************/
void MyOV9716_ExposureDCGRatioChange(float arNewRatio);

/************************************************************************/
/** MyOV9716_ExposureVSComputation compute exposure parameter of the
 *  exposure based on Update value
 *
 * \param  arExp  Exposures parameters
 * \param  arTotalExposure  New VS total exposure
 ************************************************************************/

void MyOV9716_ExposureVSComputation(OV9716_Exposure_t &arExp,
                                    float arTotalExposure);

void MyOV9716_ExposureValidation(OV9716_Exposure_t &arExp,
                                 uint32_t arSampleNumber,
                                 float arSampleStep,
                                 float arBelowMinMargin,
                                 float arAboveMaxMargin,
                                 uint8_t arExposure);

void MyOV9716_ExposureVSSetNormal(uint8_t arCamIndex,
                                  float aVSUpdate);

/************************************************************************/
/** ExposureSet updates exposure parameters in camera
 *
 * \param  arCsiIdx    index to CSI port
 * \param  arCamNum    number of cmera to set the value for
 * \param  arLSUpdate  relative update for long+short exposure
 * \param  arVSUpdate  reative update for very short exposure
 ************************************************************************/
int MyOV9716_ExposureSet(CSI_IDX aCsiIdx, uint8_t aCamNum,
                         float aLSUpdate, float aVSUpdate);

/************************************************************************/
/** ExposureCamPrint print the exposure value from the camera (I2C read)
 *
 * \param  arCsiIdx    index to CSI port
 * \param  arCamNum    number of the camera
 ************************************************************************/
int MyOV9716_ExposureCamPrint(CSI_IDX arCsiIdx,
                              uint8_t arCamNum);

/************************************************************************/
/** ChGainSet updates channel gain values for white balancing
 *
 * \param  arCsiIdx    index to CSI port
 * \param  arCamNum    number of the camera
 * \param  arUpdateR   relative update for R gain
 * \param  arUpdateB   reative update for B gain
  ************************************************************************/
void ChgainUpdate(CSI_IDX arCsiIdx,
                  uint8_t arCamNum,
                  double  arUpdateR,
                  double  arUpdateB);

/************************************************************************/
/** ChgainManSetR updates Red channel gain values for white balancing
 *
 * \param  arCsiIdx    index to CSI port
 * \param  arCamNum    number of the camera
 * \param  arIncrementR increment for Red
 ************************************************************************/
void MyOV9716_ChgainManSetR(CSI_IDX arCsiIdx,
                            uint8_t arCamNum,
                            int16_t arIncrementR);

/************************************************************************/
/** MyOV9716_ChgainManSetB updates Blue channel gain values for white balancing
 *
 * \param  arCsiIdx    index to CSI port
 * \param  arCamNum    number of the camera
 * \param  arIncrementB increment for Blue
 ************************************************************************/
void MyOV9716_ChgainManSetB(CSI_IDX arCsiIdx,
                            uint8_t arCamNum,
                            int16_t arIncrementB);

/************************************************************************/
/** MyOV9716_ChgainSet updates Rewd, Green Blue channel gain values for white balancing
 *
 * \param  arCsiIdx    index to CSI port
 * \param  arCamNum    number of the camera
 * \param  arRGain     Red   channel gain value in 3.8
 * \param  arRGain     Green channel gain value in 3.8
 * \param  arRGain     Blue  channel gain value in 3.8
 ************************************************************************/
void MyOV9716_ChgainSet(CSI_IDX arCsiIdx,
                        uint8_t arCamNum,
                        double  arRGain,
                        double  arGGain,
                        double  arBGain);

/************************************************************************/
/** MyOV9716_Chgain_Print Print current Channel Gain Parameters
 *
 * \param  arCsiIdx    index to CSI port
 * \param  arSource    Source of parameters: SourceCamera or SourceS32V
 ************************************************************************/
void MyOV9716_Chgain_Print(CSI_IDX arCsiIdx, gParamSource arSource);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __EXPOSUER_H__

/* EOF */
