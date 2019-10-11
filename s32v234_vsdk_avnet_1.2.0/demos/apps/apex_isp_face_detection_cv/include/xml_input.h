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

#ifndef XMLINPUT_H
#define XMLINPUT_H

#include <stdint.h>
#include <detector_types.h>
#include "tinyxml2.h"
#include <vector>

class XMLReader
{
public:
  XMLReader();
  ~XMLReader();
  void Init();
  
  APEX_lbpFeature* apexLBPFeatures;
  APEX_lbpStage* apexLBPStages; 
  int apexLBPFeaturesSize;
  int apexLBPStagesSize;
  
  int windowWidth;
  int windowHeight;
  
  std::vector<lbpFeature> lbpFeatures;
  std::vector<lbpRectangle> lbpRectangles;
  std::vector<lbpStage> lbpStages;
};


#endif /* XMLINPUT_H */