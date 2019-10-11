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

#ifndef GDCPEDESTRIANDETECTIONTYPESVERIFICATION_HPP
#define GDCPEDESTRIANDETECTIONTYPESVERIFICATION_HPP


/***************************************************************************/
/* Include files. */
#include "gdc_pd_types.hpp"



/*!*********************************************************************************
*  @brief Class used to write or read output sequence to/from files.
 */
class GdcPd_OutputFile
{
public:
   GdcPd_OutputFile();
   ~GdcPd_OutputFile();

   LIB_RESULT Open(const char* const cpcFilenameWrite,
                   const char* const cpcFilenameRead);

   LIB_RESULT Close();

protected:
   FILE* mpFileWrite;
   FILE* mpFileRead;
};

#endif /* GDCPEDESTRIANDETECTIONTYPESVERIFICATION_HPP */
