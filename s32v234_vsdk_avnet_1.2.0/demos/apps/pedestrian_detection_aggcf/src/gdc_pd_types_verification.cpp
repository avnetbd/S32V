/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2015 Freescale Semiconductor;
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

/***************************************************************************/
/* Include files. */
#include "gdc_pd_types_verification.hpp"
#include <limits>
#include <math.h>

/***************************************************************************/
/* Local Literals & definitions */

/**********************************************************************************************************/
// GdcPd_OutputFile function definitions
/**********************************************************************************************************/

GdcPd_OutputFile::GdcPd_OutputFile() : mpFileWrite(0), mpFileRead(0)
{
}

GdcPd_OutputFile::~GdcPd_OutputFile()
{
}

LIB_RESULT GdcPd_OutputFile::Open(const char* const cpcFilenameWrite, const char* const cpcFilenameRead)
{
  // Write file
  //--------------------------------------
  LIB_RESULT libResultWrite = LIB_FAILURE;

  if(cpcFilenameWrite != 0 && mpFileWrite == 0)
  {
    mpFileWrite = fopen(cpcFilenameWrite,
                        "wb"); // Hmm should I use binary in this case?

    if(mpFileWrite != 0)
    {
      libResultWrite = LIB_SUCCESS;
    }
  }
  else if(cpcFilenameWrite == 0)
  {
    libResultWrite = LIB_SUCCESS;
  }

  // Read file
  //--------------------------------------
  LIB_RESULT libResultRead = LIB_FAILURE;

  if(cpcFilenameRead != 0 && mpFileRead == 0)
  {
    mpFileRead = fopen(cpcFilenameRead,
                       "rb"); // Hmm should I use binary in this case?

    if(mpFileRead != 0)
    {
      libResultRead = LIB_SUCCESS;
    }
  }
  else if(cpcFilenameRead == 0)
  {
    libResultRead = LIB_SUCCESS;
  }

  return ((libResultWrite == LIB_SUCCESS) && (libResultRead == LIB_SUCCESS)) ? LIB_SUCCESS : LIB_FAILURE;
}

LIB_RESULT GdcPd_OutputFile::Close()
{
  // Write file
  //--------------------------------------
  int intResultWrite = 0;

  if(mpFileWrite != 0)
  {
    intResultWrite = fclose(mpFileWrite);
    mpFileWrite    = 0;
  }

  // Read file
  //--------------------------------------
  int intResultRead = 0;

  if(mpFileRead != 0)
  {
    intResultRead = fclose(mpFileRead);
    mpFileRead    = 0;
  }

  return ((intResultWrite == 0) && (intResultRead == 0)) ? LIB_SUCCESS : LIB_FAILURE;
}
