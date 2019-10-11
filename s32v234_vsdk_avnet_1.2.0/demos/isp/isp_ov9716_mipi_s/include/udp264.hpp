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

#ifndef __UDP264_H__
#define __UDP264_H__

#include <sys/types.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <errno.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

extern "C"
{
  #include "iniparser.h"
}

#include "utils_h264encoder.h"
#include "sdi.hpp"                      // camera middle ware

// **************************************************************************
// constants
// **************************************************************************

// **************************************************************************
// global variables
// **************************************************************************

extern cv::Mat gH264frame;        // input frame buffer for H.264 encode
extern std::thread sending_thread;

// parameters from parameter file
extern char gUdpTargetIpAddress[32];
extern char gUdpTargetPort[32];

// Flag indicates that data is ready to send.
extern bool gDataReady;
extern std::mutex gMtxDataReady;

// Flag indicates that is required termination of thread.
extern bool gTerminate;
extern std::mutex gMtxTerminate;

extern std::condition_variable gCvWaiting;

// **************************************************************************
// macro
// **************************************************************************


// **************************************************************************
// local functions
// **************************************************************************

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/
/** set data_sending: send H.264 input buffer to other thread
  *
  * \param  none
  ************************************************************************/
void data_sending();

void onH264encodingDone(unsigned char *result, int final_length, long encoding_time_us);
int resolvehelper(const char* hostname, int family, const char* service, sockaddr_storage* pAddr);
int UdpTransmitInit();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __UDP264_H__

/* EOF */
