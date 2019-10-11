/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2015 Freescale Semiconductor;
* Copyright 2017 NXP
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
#include "gdc_ft_app.hpp"

int UserStartupTask()
{
  LIB_RESULT libResult = LIB_FAILURE;

  GdcFt_App app;

  libResult = app.Start();
  SHOW_ERROR(libResult == LIB_SUCCESS);

  return 0;
}

#ifndef APEX2_EMULATE
#include "apex.h"
#include <oal.h>
#include <oal_protected.h>
#endif

#include <common_helpers.h>
#include <string>

int main(int argc, char** argv)
{
  std::string helpMsg =
      std::string("Demonstrates feature tracking task on APEX.\n\tUsage: ") + COMMON_ExtractProgramName(argv[0]);
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

#if !defined(__CYGWIN__) && !defined(_WIN32)
  APEX_Init();
#endif

  UserStartupTask();

#if !defined(__CYGWIN__) && !defined(_WIN32)
  APEX_Deinit();
#endif

  return 0;
}
