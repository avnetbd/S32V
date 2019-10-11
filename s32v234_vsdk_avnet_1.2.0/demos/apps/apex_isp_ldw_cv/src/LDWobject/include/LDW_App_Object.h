/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
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

/**
* \file LDW_App_Object.h
* \brief LDW object 
* \author Daniel Lazar
* \version
* \date
****************************************************************************/

#ifndef LDWAPPOBJECT_H
#define LDWAPPOBJECT_H

#include "multithread_app_base_app_object.h"
#include "lane_detector.h"
#include "app_defines.h"
#include "umat.hpp"

/*
 * Result structure
 */
typedef struct LDWdataStructures
{
  bool leftWarning;
  bool rightWarning;
  bool laneHealthy;
  Lane CurrentLane;
}LDWdataStructure;


class LDWobject: public BaseAppObject {
  
private:
  
  /*
   *  Lane detector
   */
  LaneDetector m_detector;
  
  /*
   *  Input data for analysis
   */
  vsdk::UMat m_input_data;
  
public:
  
  /**
  *  Constructor 
  *  \param apuldx - index of apuldx
  *  \param oddResults - pointer to odd result structure
  *  \param evenResults - pointer to odd result structure
  *  \param even_or_odd - init value => true = first is even structure used
  **/
  LDWobject(int apuldx, void* oddResults, void* evenResults, _cycleType even_or_odd);
  
  /*
   *  Destructor
   */
  ~LDWobject();
  
  /**
   *  Set pointer to input data
   *  \param data - pointer to data
   **/
  int setInputData(vsdk::UMat data);
  
  /**
   *  init function
   **/
  int init();
  
  /**
   *  Start analysis of input data
   **/
  int computeOnCrtFrame();
  
};

#endif /* LDWAPPOBJECT_H */

