/*****************************************************************************
*
* freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
* Copyright 2017-2018 NXP
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

#ifndef __STANDALONE__
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include "udp264.hpp"

//#include "typedefs.h"                   // where do we find uintXX_t ??


//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>
//#include <memory.h>
//#include <ifaddrs.h>
//#include <net/if.h>
//#include <errno.h>
//#include <iostream>
//#include <thread>
//#include <mutex>

//#include "utils_h264encoder.h"
//#include <boost/lexical_cast.hpp>
//#include "boost/filesystem/operations.hpp"
//#include "boost/filesystem/path.hpp"
//#include "boost/progress.hpp"


//***************************************************************************
// constants
//***************************************************************************

// Possible to set input resolution (must be supported by the DCU)
#define SCR_WIDTH  ((DISPLAY_PIXELS+31) & ~31)  ///< width of DDR buffer in pixels
#define SCR_HEIGHT (DISPLAY_LINES)    ///< height of DDR buffer in pixels


//***************************************************************************
// global variables
//***************************************************************************

cv::Mat gH264frame;
std::thread sending_thread(data_sending);

// Multihreading global variables and mutex
char gUdpTargetIpAddress[32] = {"10.0.0.2"};
char gUdpTargetPort[32] = {"9000"};


// Flag indicates that data is ready to send.
bool gDataReady = false;
std::mutex gMtxDataReady;

// Flag indicates that is required termination of thread.
bool gTerminate = false;
std::mutex gMtxTerminate;

std::condition_variable gCvWaiting;

static sockaddr_storage addrDest = {};
static int sock;

static bool gUdpTransmitReady = false;

// Flag indicates that the transmitter is ready to send.
static std::mutex gMtxUdpTransmitReady;

// image data exchange between threads
static const int sendingBufferSize = 300000;
static int encodedImageSize = 0;
static char * sendingBuffer;

//***************************************************************************
// macro
//***************************************************************************

//***************************************************************************
// functions
//***************************************************************************
//***************************************************************************
extern "C"  void data_sending()
{
  gMtxTerminate.lock();
  while(!gTerminate)
  {
    gMtxTerminate.unlock();
    std::unique_lock<std::mutex> mLock(gMtxDataReady);
    gCvWaiting.wait(mLock, []{return gDataReady;});
    gDataReady = false;
    if (gTerminate)
    {
        //std::cout << "Thread #2 Terminating...\n";
        break;
    }
    // after the wait, we own the lock.
    if (encodedImageSize > 10000)
    {
      int udp_result=0;
      int need_send = encodedImageSize;
      int i;
      int packet_size = 50000;
      for(i =0; i< encodedImageSize / packet_size;i++)
      {
        udp_result = sendto(sock, &sendingBuffer[i*packet_size], packet_size, 0, (sockaddr*)&addrDest, sizeof(addrDest));
        if (udp_result == -1)
        {
          printf("error\n");
        }
        need_send -= packet_size;
      }
      udp_result = sendto(sock, &sendingBuffer[i* packet_size], need_send, 0, (sockaddr*)&addrDest, sizeof(addrDest));
      if (udp_result == -1)
      {
        printf("error\n");
      }
    }
    else
    {
      int udp_result=0;
      int need_send = encodedImageSize;
      
      udp_result = sendto(sock, &sendingBuffer[0], need_send, 0, (sockaddr*)&addrDest, sizeof(addrDest));
      if (udp_result == -1)
      {
        printf("error\n");
      }
      // printf("H264Encoder output errot - Frame drop\n");
    }
    gMtxTerminate.lock();
  }
  gMtxTerminate.unlock();
  std::cout << "Thread #2 Thread end\n";
}

//***************************************************************************
MovingAvg<unsigned long> h264encoder_time(50);
FILE * pEncFile;

//***************************************************************************
extern "C" void onH264encodingDone(unsigned char *result, int final_length, long encoding_time_us)
{
  // printf("onH264encodingDone\n");
  gMtxUdpTransmitReady.lock();
  if (gUdpTransmitReady)
  {
    gMtxUdpTransmitReady.unlock();
    // printf("Encoded frame length: %d", final_length);
    // Transfer data from webserver thread into sending thread
    encodedImageSize = final_length;
    memcpy(sendingBuffer, result, final_length);
    // Start UDP sending from other thread
    {
      std::lock_guard<std::mutex> mLock(gMtxDataReady);
      gDataReady = true;
      // std::cout << "Thread #1 Require UDP send\n";
    }
    gCvWaiting.notify_one();
  }

#ifdef ENC_SAVE
   fwrite (result , sizeof(uint8_t), final_length/sizeof(uint8_t), pEncFile);
#endif
}

//***************************************************************************
int resolvehelper(const char* hostname, int family, const char* service, sockaddr_storage* pAddr)
{
    int result;
    addrinfo* result_list = NULL;
    addrinfo hints = {};
    hints.ai_family = family;
    hints.ai_socktype = SOCK_DGRAM; // without this flag, getaddrinfo will return 3x the number of addresses (one for each socket type).
    result = getaddrinfo(hostname, service, &hints, &result_list);
    if (result == 0)
    {
        //ASSERT(result_list->ai_addrlen <= sizeof(sockaddr_in));
        memcpy(pAddr, result_list->ai_addr, result_list->ai_addrlen);
        freeaddrinfo(result_list);
    }

    return result;
}

int UdpTransmitInit();

//***************************************************************************
int UdpTransmitInit()
{
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  sockaddr_in addrListen = {}; // zero-int, sin_port is 0, which picks a random port for bind.
  addrListen.sin_family = AF_INET;

  int result = bind(sock, (sockaddr*)&addrListen, sizeof(addrListen));
  if (result == -1)
  {
     int lasterror = errno;
     std::cout << "error: " << lasterror;
     return -1;
  }

  printf("UDP: streaming to %s:%s\n",gUdpTargetIpAddress,gUdpTargetPort);
  result = resolvehelper(gUdpTargetIpAddress, AF_INET, gUdpTargetPort, &addrDest);
  if (result != 0)
  {
     int lasterror = errno;
     std::cout << "error: " << lasterror;
     return -1;
  }

  // Thread for the UDP sending
  std::cout << "Alocating memmory space for sending thead...\n";
  encodedImageSize = 0;
  sendingBuffer = new char[sendingBufferSize];

  gMtxUdpTransmitReady.lock();
  gUdpTransmitReady = true;
  gMtxUdpTransmitReady.unlock();

  return 0;
}

#endif  // ifndef __STANDALONE__


/* EOF */
