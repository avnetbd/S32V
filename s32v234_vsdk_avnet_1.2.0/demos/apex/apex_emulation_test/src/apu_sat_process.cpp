/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
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
#ifdef APEX2_EMULATE // This is needed only for the Target Compiler
#include "acf_lib.hpp"
#else
#include <apex.h>
#endif

#include <iostream>
#include <ostream>
//#include <memory.h>
using namespace std;

#include "apu_sat_graph_names.h"
#include "apu_sat_process.h"

CSat_Proc::~CSat_Proc()
{
}

int CSat_Proc::InitData(vsdk::UMat& input)
{
  lInput        = input;
  lOutput       = vsdk::UMat(lInput.rows, lInput.cols, VSDK_CV_32SC1);
  lOutputFilter = vsdk::UMat(lInput.rows, lInput.cols, VSDK_CV_8UC1);

  m_errVal |= lOutput.empty();
  m_errVal |= lOutputFilter.empty();
  return m_errVal;
}

int CSat_Proc::Connect()
{
#ifdef APEX2_EMULATE
  cout << "\nInitializing graph:" << SAT_GN << endl;
#endif

  m_errVal |= Initialize();
  m_errVal |= ConnectIO(SAT_GRAPH_IN, lInput);
  m_errVal |= ConnectIO(SAT_GRAPH_OUT, lOutput);
  m_errVal |= ConnectIO(SAT_GRAPH_OUT_FILTER, lOutputFilter);

#ifdef APEX2_EMULATE
  cout << SAT_GN << " graph Plan: " << endl;
  cout << GetExecutionPlanDescription() << endl;
#endif

#ifdef APEX2_EMULATE
  if(m_errVal)
  {
    std::cout << SAT_GN << " plan error!" << endl;
    std::cout << ErrorMessage() << endl;
    char c;
    std::cin >> c;
  }
#endif

  return m_errVal;
}
