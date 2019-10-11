/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
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

/**
* \file LDW_App_Object.cpp
* \brief LDW object
* \author Daniel Lazar
* \version
* \date
****************************************************************************/

#include "LDW_App_Object.h"

/**
  *  Constructor Calls BaseAppObject constructor
  *  \param apuldx - index of apuldx
  *  \param oddResults - pointer to odd result structure
  *  \param evenResults - pointer to odd result structure
  *  \param even_or_odd - init value => true = first is even structure used
  **/
LDWobject::LDWobject(int apuldx, void* oddResults, void* evenResults, _cycleType even_or_odd)
:BaseAppObject(apuldx, oddResults, evenResults, even_or_odd)
{
  
}

/*
 *  Destructor
 */
LDWobject::~LDWobject()
{
  
}

/**
 *  Set pointer to input data
 *  \param data - pointer to data
 **/
int LDWobject::setInputData(vsdk::UMat data)
{
  m_input_data = data;
  return 0;
}

/**
 *  init LaneDetector
 **/
int LDWobject::init()
{
  new (&m_detector) LaneDetector(m_apuldx);
  return 0;
}

/**
 *  Start analysis of input data
 **/
int LDWobject::computeOnCrtFrame()
{
  //printf("LDW computeOnFrame %d\n", m_nrFrames);
  bool result = m_detector.GetNextFrame(m_input_data); // returns true if ok
  
  if(m_even_or_odd == even)
  {
    ((LDWdataStructure*)m_evenResults)->leftWarning = m_detector.m_left_warning;
    ((LDWdataStructure*)m_evenResults)->rightWarning = m_detector.m_right_warning;
    ((LDWdataStructure*)m_evenResults)->laneHealthy = m_detector.m_lane_healthy;
    ((LDWdataStructure*)m_evenResults)->CurrentLane = Lane(m_detector.GetCurrentLane());
  }
  else
  {
    ((LDWdataStructure*)m_oddResults)->leftWarning = m_detector.m_left_warning;
    ((LDWdataStructure*)m_oddResults)->rightWarning = m_detector.m_right_warning;
    ((LDWdataStructure*)m_oddResults)->laneHealthy = m_detector.m_lane_healthy;
    ((LDWdataStructure*)m_oddResults)->CurrentLane = Lane(m_detector.GetCurrentLane());
  }
  
  m_even_or_odd = m_even_or_odd == even ? odd : even;
 // printf("End  LDW ComputeOnFrame with result %d\n", (int) result);
  return (int) !result; // outside expects false/0 if ok
}
  


