/**
  @defgroup         dxgrVLUSERAPP The VideoListener Application Example
  @details          This is the demonstrator of the complete VideoListener application providing
                    video play back on the HW screen from one or more camera sensors connected
                    with the target via Ethernet link. Application depends on the @ref dxgrVLFIRMWARE
                    and the @ref dxgrSMKERNELMOD which also needs to be present within the system.
                    Additionally, there is a list of Vision components needed to be supplied too:
                    - sram.ko
                    - seq.ko
                    - oal_cma.ko
                    - csi.ko
                    - fdma.ko
                    - fsl_jpegdcd.ko (only if application is built with APP_CFG_DECODER_VARIANT == DECODER_JPEG)
                    - h264dcd.ko (only if the application is built with APP_CFG_DECODER_VARIANT == DECODER_H264)

                    When all required components are loaded, application can be executed. Note that
                    user shall ensure that Ethernet AVB traffic from the cameras is visible by the host
                    and is properly VLAN tagged (VLAN priority must be equal to priorities configured through
                    $ref APP_KEY_STRM_VLAN_PRIO_ADD). It shall also be ensured that all VideoListener
                    components are built with the same configuration of the decoder variant, either
                    as JPEG or as H264.

                    Once started the application can be terminated by the Ctrl+c command.
                    
                    This user-space application does not communicate with @ref dxgrSCORE directly. It uses
                    following API of sm_drv, namely write() function:
                    - To execute @ref SCORE_Command(), call write(fd_sm, command, command_length). stream_core commands
                      begin with "score ". Then append stream_core command as it is defined in Streaming Core reference.
                      There are also application commands defined, starting with "app ". These are handled by firmware
                      application, not by SCORE_Command().
                    - To execute both @ref SCORE_Init() and @ref SCORE_PreStart(), and start execution of application
                      core, use command "app start".
                    - To execute @ref SCORE_Start() (to start feeding decoder), use command "score feed".
                    - To execute @ref SCORE_Stop() and stop execution of application core, use command "app stop".
                    - Whenever @ref SCORE_GetErrorMask() returns non-zero mask, the mask is sent to this application
                      in form of SIGPOLL signal with siginfo->si_code equal to POLL_ERR, the error mask is stored in
                      siginfo->si_int. Note that this application shall register the signal in order to receive it.
                    - Whenever @ref SCORE_GetPendingMsgs() returns non-zero mask, the mask is sent to this application
                      in form of SIGPOLL signal with siginfo->si_code equal to POLL_MSG, the message mask is stored in
                      siginfo->si_int.
                    - To register reception of the SIGPOLL interrupt, use fcntl function this way:
                      fcntl(fd_sm, F_SETOWN, getpid()); and then this way
                      fcntl(fd_sm, F_SETFL, fcntl(fd_sm, F_GETFL) | O_ASYNC); Only one process can register that.

                      pointer to process ID (PID). Use SM_DRV_IOCTL_UNREG_SIG and the PID to unregister. sm_drv will
                      automatically generate sm calls to enable/disable generation of events within sm_app.
                    
                    If this application needs to restart decoder, it shall order the @ref dxgrSCORE to initiate the
                    restart. Use command "score reset" to archieve that.
                    Then wait untill @ref SCORE_MSG_RESET_DECODER is received as POLL_MSG. Only then this application
                    shall restart the decoder. Note that after the "score reset" is executed, error
                    recovery procedure shall be followed, as it is described in @ref dxgrSCORE.

  @addtogroup       dxgrVLUSERAPP
  @{

  @file             main.cpp
  @brief            This is the VideoListener video play back demonstration application
  @details

  Copyright (c) 2016-2018 NXP Semiconductors
  All Rights Reserved.

  This file contains sample code only.

  THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include "oal.h"

/* Included from SM application */
#include "application_cfg.h"      /* To access zero buffer configuration */
#include "stream_core_bitmasks.h" /* To access definitions of bitmasks received through SIGPOLL */
#include "cmdb_input_cfg.h"       /* To access command buffer length */
#include "eth_queue_cfg.h"        /* To access Ethernet Rx queue length */

/* Video application */
#include "video_app.h"
#include <common_helpers.h>
#include <string>

//***************************************************************************
//                            PRECOMPILE OPTIONS
//***************************************************************************
/* Include SM debug reader, 0 means Disable, 1 Enable */
#define APP_CFG_ENABLE_DEBUG_OUTPUT_READER 0

/* Used Linux drivers */
#define SRAM_DRV "/dev/sram"
#define SM_DRV "/dev/sm_drv"

/* Used configuration file */
#define CFG_FILE "/etc/vid_list.cfg"
#define CFG_FILE_LOC2 "./vid_list.cfg" /* Used if CFG_FILE not found */
#define CFGFILE_CMD_NUMBER 30          /* Number of commands in config file + some extra */
#define CFGFILE_CMD_LEN 50             /* Maximal length of commands in config file + some bytes extra */

/* Application pre-compile configuration */
#define CFG_ETH_BUFF_LENGTH 1536U /* This value must be evenly divisible by CFG_ETH_RX_BUFF_ALIGN */
/* Hardware specification */
#define CFG_ETH_RX_BUFF_ALIGN 64U /* Requirement for Rx buffers memory */

/* POLL_ERR mask values derived from run-time error macros in stream_core.c */
#define POLL_ERR_FATAL_MASK ((uint32_t)(SCORE_ERR_FATAL_ERR | SCORE_ERR_SM_APP_FAILED))
#define POLL_ERR_STREAM_MASK                                                                                           \
  ((uint32_t)(SCORE_ERR_STRM_NUM_0 | SCORE_ERR_STRM_NUM_1 | SCORE_ERR_STRM_NUM_2 | SCORE_ERR_STRM_NUM_3))
#define POLL_ERR_STREAM_DROP_OUT SCORE_ERR_STREAM_DROP_OUT
#define POLL_ERR_OUT_OF_ORDER_FRAME SCORE_ERR_OUT_OF_ORDER_FRAME
#define POLL_ERR_ETH_BUFFER_OVERWRITE SCORE_ERR_ETH_BUFFER_OVERWRITE
#define POLL_ERR_STREAM_Q_FULL SCORE_ERR_STREAM_Q_FULL
#define POLL_ERR_STREAM_Q_BROKEN SCORE_ERR_STREAM_Q_BROKEN
#define POLL_ERR_DECODER_ERR SCORE_ERR_DECODER_ERR
#define POLL_ERR_DEC_Q_BROKEN SCORE_ERR_DEC_Q_BROKEN
#define POLL_ERR_IMG_LOST_MASK                                                                                         \
  ((uint32_t)(SCORE_ERR_STREAM_DROP_OUT | SCORE_ERR_OUT_OF_ORDER_FRAME | SCORE_ERR_ETH_BUFFER_OVERWRITE |              \
              SCORE_ERR_STREAM_Q_FULL | SCORE_ERR_STREAM_Q_BROKEN | SCORE_ERR_DECODER_ERR | SCORE_ERR_DEC_Q_BROKEN))
#define POLL_ERR_NO_TRAFFIC SCORE_ERR_NO_TRAFFIC

//***************************************************************************
//                             LOCAL TYPEDEFS
//***************************************************************************
typedef struct
{
  /* List of commands from configuration file that shall be run in SM application (firmware): */
  char     aszCmd[CFGFILE_CMD_NUMBER][CFGFILE_CMD_LEN]; /* One string for each command loaded from file */
  uint16_t u16LoadedNum;
} trConfFromFile;

typedef struct
{
  /* Streaming related */
  int StrmFD;
} trExitInfo;

//***************************************************************************
//                        LOCAL FUNCTION PROTOTYPES
//***************************************************************************
/** SIGINT handler.
* \param  aSigNo
*/
static void SigintHandler(int aSigNo);
/** SIGINT handler.
* \param  aSigNo
* \return SEQ_LIB_SUCCESS if all ok
*         SEQ_LIB_FAILURE if failed
*/
static int32_t SigintSetup(void);
/** Handler of asynchronous events generated in Secure Monitor Application
* \param  sig
* \param  siginfo
* \param  context
*/
static void SigpollHandler(int sig, siginfo_t* siginfo, void* context);
/** Function that configures Streaming SM Application
* \param  fd_sm
* \param  EthBufPtr
* \param  EthBufLen
* \retval EXIT_FAILURE Failed to configure
* \retval 0 Successfully configured
*/
static int32_t configure_sm_app(int fd_sm, unsigned long EthBufPtr, unsigned long EthBufLen);
/** Function that opens configuration file, parses options and stores them in an array.
* \param  prOut Pointer to structure where loaded values shall be written
* \retval true Options successfully loaded
* \retval false Failed to load the options
*/
static bool LoadConfFile(trConfFromFile* prOut);
/** This function handles call of a command with numeric argument in running SM application
* \param  fd Opened file descriptor of sm_drv
* \param  u32CmdValue A number to be appended to the text of the command. It will be in hexadecimal form.
* \return Negative number indicates error. Zero or positive number indicates success.
*/
static ssize_t SmCommandUint64(int fd, const char* szCmdText, uint64_t u64CmdValue);
/** This function handles call of a command in running SM application
* \param  fd Opened file descriptor of sm_drv
* \param  szCmdText Text of the command
* \return Negative number indicates error. Zero or positive number indicates success.
*/
static ssize_t SmCommand(int fd, const char* szCmdText);

static void ExitClearly(void);

#if(0 != APP_CFG_ENABLE_DEBUG_OUTPUT_READER)
static void* sm_text_reader_body(void* pvArg);
static bool  sm_text_reader_shall_stop(pthread_mutex_t* pMutex);
#endif /* APP_CFG_ENABLE_DEBUG_OUTPUT_READER */

//***************************************************************************
//                             LOCAL VARIABLES
//***************************************************************************
// For automatic error recovery
static bool sDoGrabberRestart = false;
static bool sCapturingStarted = false;

static bool           sStop         = false; ///< to signal Ctrl+c from command line
static trConfFromFile rConfFromFile = {{{0U}}, 0U};
static trExitInfo     rExitInfo     = {-1};

//***************************************************************************
//                             GLOBAL FUNCTIONS
//***************************************************************************
/**
* @implements VLREQ003
*/
int main(int argc, char** argv)
{
  std::string helpMsg =
      std::string("Demonstrates video listener application.\n\tUsage: ") + COMMON_ExtractProgramName(argv[0]);
  int idxHelp = COMMON_HelpMessage(argc, argv, helpMsg.c_str());
  if(idxHelp > 0)
  {
    //found help in application arguments thus exiting application
    return -1;
  }

  int              fd_sm               = -1;
  void*            sralloc_prm_handler = NULL;
  unsigned long    EthBufPtr = 0, EthBufLen = 0;
  struct sigaction sig_act;
  bool             bVideoAppReady = false;

#if(0 != APP_CFG_ENABLE_DEBUG_OUTPUT_READER)
  bool            bSmTextReaderMutexInitialized  = false;
  bool            bSmTextReaderThreadInitialized = false;
  pthread_t       SmTextReader;
  pthread_mutex_t SmTextReaderMutex;
#endif /* APP_CFG_ENABLE_DEBUG_OUTPUT_READER */
#if TRUE == APP_CFG_ZERO_BUF_ENABLED
  unsigned long zeroBufferAddress;
  void*         sralloc_prm_handler_zero_buf = NULL;
#endif /* APP_CFG_ZERO_BUF_ENABLED */

  /*  Register function that automatically releases resources in case of application termination.
        NOTE: Does not apply for termination from signals */
  if(0 != atexit(&ExitClearly))
  {
    printf("[app] Unable to register the at-exit callback, error code: %s\n", strerror(errno));
    goto finalize;
  }
  /*  Load options from configuration file */
  if(false == LoadConfFile(&rConfFromFile))
  {
    printf("[app] Failed to load configuration file\n");
    goto finalize;
  }
  /*  Open the Secure Monitor Driver */
  rExitInfo.StrmFD = open(SM_DRV, O_WRONLY);
  fd_sm            = rExitInfo.StrmFD;
  if(fd_sm < 0)
  {
    printf("[app] Unable to open device %s: %s\n", SM_DRV, strerror(errno));
    goto finalize;
  }

#if(0 != APP_CFG_ENABLE_DEBUG_OUTPUT_READER)
  int ret;
  /*  Run the string reader thread */
  pthread_mutex_init(&SmTextReaderMutex, NULL);
  bSmTextReaderMutexInitialized = true;
  pthread_mutex_lock(&SmTextReaderMutex);
  ret = pthread_create(&SmTextReader, NULL, &sm_text_reader_body, (void*)(&SmTextReaderMutex));
  if(ret != 0)
  {
    printf("[app] Can't create thread: [%s]", strerror(ret));
    goto finalize;
  }
  else
  {
    /*  Let the thread read all the data */
    sleep(1);
    bSmTextReaderThreadInitialized = true;
  }
#endif /* APP_CFG_ENABLE_DEBUG_OUTPUT_READER */

  /*  Allocate aligned SRAM memory for our ENET Rx queue buffers */
  EthBufLen = (CFG_ETH_BUFF_LENGTH * ETHQ_CFG_RX_BD_RING_LEN);
  sralloc_prm_handler =
      OAL_MemoryAllocFlag(EthBufLen, OAL_MEMORY_FLAG_ALIGN(CFG_ETH_RX_BUFF_ALIGN) | OAL_ALLOC_SRAM_SINGLE);
  if(sralloc_prm_handler == NULL)
  {
    printf("[app] SRAM for ENET buffers allocation failed\n");
    EthBufLen = 0UL;
    goto finalize;
  }
  EthBufPtr = (unsigned long)(uintptr_t)OAL_MemoryReturnAddress(sralloc_prm_handler, ACCESS_PHY);
  if(EthBufPtr == 0)
  {
    printf("[app] Failed to get physical address for ENET buffers.\n");
    goto finalize;
  } // if failed to get physical address

#if TRUE == APP_CFG_ZERO_BUF_ENABLED
  /* Allocate aligned SRAM memory for zero buffer */
  zeroBufferAddress = APP_CFG_ZERO_BUF_HEAD_LENGTH + APP_CFG_ZERO_BUF_DATA_LENGTH;
  sralloc_prm_handler_zero_buf =
      OAL_MemoryAllocFlag(zeroBufferAddress, OAL_MEMORY_FLAG_ALIGN(CFG_ETH_RX_BUFF_ALIGN) | OAL_ALLOC_SRAM_SINGLE);
  if(NULL == sralloc_prm_handler_zero_buf)
  {
    printf("[app] SRAM for zeroBufferAddress buffers allocation failed\n");
    goto finalize;
  }
  zeroBufferAddress = (unsigned int)(uintptr_t)OAL_MemoryReturnAddress(sralloc_prm_handler_zero_buf, ACCESS_PHY);
  if(zeroBufferAddress == 0)
  {
    printf("[app] Failed to get physical address for zero buffer.\n");
    goto finalize;
  }    // if failed to get physical address
#endif /* APP_CFG_ZERO_BUF_ENABLED */

  /*  Configure the SM application */
  if(0 != configure_sm_app(fd_sm, EthBufPtr, EthBufLen))
  {
    printf("[app] Failed to configure SM application\n");
    goto finalize;
  }
#if TRUE == APP_CFG_ZERO_BUF_ENABLED
  if(0 > SmCommandUint64(fd_sm, "score set sram_buffer ", zeroBufferAddress))
  {
    printf("[app] Failed to give the zero buffer to the SM application\n");
    goto finalize;
  }
#endif /* APP_CFG_ZERO_BUF_ENABLED */

  /*  Install SM application signal handler */
  sig_act.sa_sigaction = &SigpollHandler;
  sig_act.sa_flags     = SA_SIGINFO;
  if(sigaction(SIGPOLL, &sig_act, NULL))
  {
    printf("[app] Can't install sm_app signal handler\n");
    goto finalize;
  }
  /*  Register THIS process as an asynchronous events listener */
  if(fcntl(fd_sm, F_SETOWN, getpid()))
  {
    printf("[app] fcntl(F_SETOWN) failed: %s\n", strerror(errno));
    goto finalize;
  }
  if(fcntl(fd_sm, F_SETFL, fcntl(fd_sm, F_GETFL) | O_ASYNC))
  {
    printf("[app] fcntl(F_SETFL) failed: %s\n", strerror(errno));
    goto finalize;
  }
  /*  PreStart the streaming (SM app) */
  if(0 > SmCommand(fd_sm, "app start"))
  {
    printf("[app] \"app start\" failed\n");
    goto finalize;
  }

  /*  Setup Ctrl+C handler */
  if(SigintSetup() != 0)
  {
    printf("[app] Failed to register Ctrl+C signal handler.\n");
    goto finalize;
  }

  printf("[app] Press Ctrl+C to terminate the demo.\n");

  if(0 != video_app_init())
  {
    printf("[app] Video App initialization failed\n");
    goto finalize;
  }
  else
  {
    bVideoAppReady = true;
  }

  if(-1 == video_app_start())
  {
    printf("[app] Can't start the video application\n");
    goto finalize;
  }

  if(0 > SmCommand(fd_sm, "score feed"))
    goto finalize;

  for(;;)
  {
    if(sDoGrabberRestart)
    {
      // Restart graph (so also decoder)
      printf("[app] Stopping Grabber\n");
      if(0 != video_app_stop())
      {
        printf("[app] Failed to stop video application while restarting playback\n");
        goto finalize;
      }
      else
      {
        printf("[app] Initializing Grabber\n");
        if(0 != video_app_init())
        {
          printf("[app] Failed to reinitialize video application while restarting playback\n");
          goto finalize;
        }
        else
        {
          printf("[app] Starting Grabber\n");
          if(0 != video_app_start())
          {
            printf("[app] Failed to start video application while restarting playback\n");
            goto finalize;
          }
          // Firmware: start feeding again
          else
          {
            if(0 > SmCommand(fd_sm, "score feed"))
            {
              goto finalize;
            }
            else
            {
              // Restart was successfull
              printf("[app] Grabber restart was completed\n");
              sDoGrabberRestart = false;
              video_app_abort_capture(false);
            }
          }
        }
      }
    }
    if(TRUE == sCapturingStarted)
    {
      if((0 != video_app_body())                // Run and check synchronous error
         || (true == video_app_get_async_err()) // Check asynchronous error
      )
      {
        // Error was encountered
        if(TRUE == sDoGrabberRestart)
        {
          // Already being handled
          printf("[app] video_app_body() ended with pending decoder reset request.\n");
        }
        else if(false == sStop)
        {
          // Handle the error, only if not stopping
          sCapturingStarted = false;
          printf("[app] Sending INIT_DEC_RESET signal to firmware\n");
          /* Ask firmware to initiate decoder restart */
          if(0 > SmCommand(fd_sm, "score reset"))
            goto finalize;
        }
        video_app_clear_async_err();
      }
    }

    if(sStop)
      break; // break if Ctrl+C pressed
  }          // for ever

finalize:
  printf("[app] Finalizing the application\n");
  /*  Stop the streaming (SM app) */
  (void)SmCommand(fd_sm, "app stop");

  /*  Stop the video application */
  if(true == bVideoAppReady)
    if(0 != video_app_stop())
      printf("[app] Can't stop the video application\n");

  /*  Dispose Eth buffer memory */
  if(0UL != EthBufLen)
    if(OAL_MemoryFree(sralloc_prm_handler))
      printf("[app] Unable to release ENET buff memory\n");

#if(0 != APP_CFG_ENABLE_DEBUG_OUTPUT_READER)
  /*  Stop the debug output reader */
  if(true == bSmTextReaderMutexInitialized)
  {
    int ret = pthread_mutex_unlock(&SmTextReaderMutex);
    if(0 != ret)
    {
      printf("[app] pthread_mutex_unlock() has failed: %d\n", ret);
      if(true == bSmTextReaderThreadInitialized)
      {
        ret = pthread_cancel(SmTextReader);
        if(0 != ret)
          printf("[app] pthread_cancel() has failed: %d\n", ret);
      }
    }
    else
    {
      if(true == bSmTextReaderThreadInitialized)
      {
        pthread_join(SmTextReader, NULL);
      }
    }
  }
#endif /* APP_CFG_ENABLE_DEBUG_OUTPUT_READER */

  /*  Close all opened drivers */
  if(fd_sm >= 0)
    close(fd_sm);
  rExitInfo.StrmFD = -1;

  return 0;
} // main()

//***************************************************************************
//                             LOCAL FUNCTIONS
//***************************************************************************
static void SigintHandler(int aSigNo)
{
  (void)aSigNo; /* Not needed in this case */
  printf("[app] Interrupted from keyboard\n");
  video_app_abort_capture(true);
  sStop = true;
} // SigintHandler()

//***************************************************************************
static int32_t SigintSetup()
{
  // prepare internal signal handler
  struct sigaction lSa;
  memset(&lSa, 0, sizeof(lSa));
  lSa.sa_handler = SigintHandler;

  if(sigaction(SIGINT, &lSa, NULL) != 0)
    return -1;

  return 0;
} // SigintSetup()

//***************************************************************************
static void SigpollHandler(int sig, siginfo_t* siginfo, void* context)
{
  bool     bToStop  = true; /* Will stop if the signal is not correctly handled */
  uint32_t u32SiInt = (uint32_t)(siginfo->si_int);

  (void)context; /* Not needed in this case */
  if(SIGPOLL == sig)
  {
    printf("[app] SIGPOLL: ");
    switch(siginfo->si_code)
    {
      case POLL_IN:
        printf("POLL_IN: unexpected code\n");
        break;
      case POLL_OUT:
        printf("POLL_OUT: unexpected code\n");
        break;
      case POLL_MSG:
        printf("POLL_MSG received: ");
        if(0U == siginfo->si_int)
        {
          printf("No message\n");
        }
        else if(0U != (siginfo->si_int & 1U))
        {
          printf("Decoder restart requested\n");
          sDoGrabberRestart = true;
          sCapturingStarted = false;
          video_app_abort_capture(true);
          bToStop = false;
        }
        else if(0U != (siginfo->si_int & 2U))
        {
          printf("Start capturing decoded data\n");
          sCapturingStarted = true;
          bToStop           = false;
        }
        break;
      case POLL_PRI:
        printf("POLL_PRI: unexpected code\n");
        break;
      case POLL_HUP:
        printf("POLL_HUP: unexpected code\n");
        break;
      case POLL_ERR:
        printf("POLL_ERR: Received error mask: 0x%x\n", u32SiInt);
        /* Check fatal errors */
        if(0U == (uint32_t)(u32SiInt & POLL_ERR_FATAL_MASK))
        {
          int i;
          bToStop = false; /* Not fatal */
          /* Further processing of error mask (siginfo->si_int) is possible here */
          if(0UL != (u32SiInt & 0xfUL))
          { /* Streams mask */
            printf("[app] Streams involved:");
            for(i = 0; i < 4; i++)
              if(1UL == ((u32SiInt >> i) & 1UL))
                printf(" %d", i);
            printf("\n");
          }
          if(0UL != (u32SiInt & POLL_ERR_NO_TRAFFIC))
            printf("[app] No packets received, start delayed\n");
          if(0UL != (u32SiInt & POLL_ERR_IMG_LOST_MASK))
          {
            printf("[app] One or more sets of images lost due to ");
            if(0UL != (u32SiInt & POLL_ERR_STREAM_DROP_OUT))
              printf("no video data being received\n");
            if(0UL != (u32SiInt & POLL_ERR_OUT_OF_ORDER_FRAME))
              printf("one or more lost packets\n");
            if(0UL != (u32SiInt & POLL_ERR_ETH_BUFFER_OVERWRITE))
              printf("Ethernet queue overflow\n");
            if(0UL != (u32SiInt & POLL_ERR_STREAM_Q_FULL))
              printf("stream queue overflow\n");
            if(0UL != (u32SiInt & POLL_ERR_STREAM_Q_BROKEN))
              printf("broken stream queue\n");
            if(0UL != (u32SiInt & POLL_ERR_DECODER_ERR))
              printf("decoder error\n");
            if(0UL != (u32SiInt & POLL_ERR_DEC_Q_BROKEN))
              printf("broken decoder queue\n");
          }
        }
        else
        {
          printf(
              "[app] Streaming stopped due to fatal error. Read from /dev/sm_drv to access last debugging messages.\n");
          video_app_abort_capture(true);
          bToStop = true; /* Currently the action is to stop and exit the application */
        }
        break;
      default:
        printf("UNKNOWN CODE\n");
        break;
    }
  }
  else
    printf("[app] Unexpected signal received: %d\n", sig);
  sStop |= bToStop;
}

//***************************************************************************
static int32_t configure_sm_app(int fd_sm, unsigned long EthBufPtr, unsigned long EthBufLen)
{
  uint32_t u32Ctr;

  /*  Pass options from rConfFromFile to the sm_app */
  for(u32Ctr = 0U; u32Ctr < rConfFromFile.u16LoadedNum; u32Ctr++)
    if(0 > SmCommand(fd_sm, rConfFromFile.aszCmd[u32Ctr]))
    {
      printf("[app] ...the command was from configuration file\n");
      return EXIT_FAILURE;
    }

  /*  Pass options determined in run-time */
  if(0 > SmCommandUint64(fd_sm, "score set ethq_size_of_buff ", EthBufLen))
    return EXIT_FAILURE;
  if(0 > SmCommandUint64(fd_sm, "score set ethq_buff_ring_ptr ", EthBufPtr))
    return EXIT_FAILURE;

  return 0;
}

//***************************************************************************
/* Copies szCmdText to internal buffer, appends u64CmdValue in hexadecimal form and writes to fd */
static ssize_t SmCommandUint64(int fd, const char* szCmdText, uint64_t u64CmdValue)
{
  static const uint8_t au8ToHex[16U] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  uint32_t             u32TextLen    = strlen(szCmdText);
  ssize_t              RetVal;
  char                 szCommand[CMDB_CFG_BUF_SIZE];
  uint32_t             u32Shift;

  if((u32TextLen + 18U) > CMDB_CFG_BUF_SIZE)
  { // +18 for 64bit hexadecimal number
    printf("[app] Firmware cmd too long (max=%d): \"%s0x%016lx\"\n", (int)CMDB_CFG_BUF_SIZE, szCmdText, u64CmdValue);
    return -1;
  }
  memcpy(szCommand, szCmdText, u32TextLen);
  /* Convert number to hexadecimal representation */
  szCommand[u32TextLen++] = '0';
  szCommand[u32TextLen++] = 'x';
  for(u32Shift = 60U; u32Shift < 61U; u32Shift -= 4U)
    szCommand[u32TextLen++] = au8ToHex[(u64CmdValue >> u32Shift) & 0xFULL];
  /* Write result */
  RetVal = write(fd, szCommand, u32TextLen);
  if(0 > RetVal)
    printf("[app] Firmware cmd FAIL (%ld): ", RetVal);
  else
    printf("[app] Firmware cmd OK: ");
  fwrite(szCommand, 1, u32TextLen, stdout);
  fputc('\n', stdout);
  return RetVal;
}
//***************************************************************************
static ssize_t SmCommand(int fd, const char* szCmdText)
{
  uint32_t u32TextLen = strlen(szCmdText);
  ssize_t  RetVal;

  if(u32TextLen > CMDB_CFG_BUF_SIZE)
  {
    printf("[app] Firmware cmd too long (max=%d): %s\n", (int)CMDB_CFG_BUF_SIZE, szCmdText);
    return -1;
  }
  RetVal = write(fd, szCmdText, u32TextLen);
  if(0 > RetVal)
    printf("[app] Firmware cmd FAIL (%ld): %s\n", RetVal, szCmdText);
  else
    printf("[app] Firmware cmd OK: %s\n", szCmdText);
  return RetVal;
}

//***************************************************************************
static bool LoadConfFile(trConfFromFile* prOut)
{
  int32_t  s32Byte;
  bool     bRetVal    = true;
  uint32_t u32LineCtr = 1U;
  uint32_t u32WrIdx   = 0U;
  FILE*    CfgFile;

  /* Open file */
  CfgFile = fopen(CFG_FILE, "r");
  if(CfgFile == NULL)
  {
    printf("[app] Did not find file " CFG_FILE ", trying " CFG_FILE_LOC2 "...\n");
    CfgFile = fopen(CFG_FILE_LOC2, "r");
    if(CfgFile == NULL)
    {
      printf("[app] Failed to open configuration file " CFG_FILE_LOC2 ": %s\n", strerror(errno));
      return EXIT_FAILURE;
    }
    else
      printf("[app] Loading configuration from: " CFG_FILE_LOC2 "\n");
  }
  else
    printf("[app] Loading configuration from: " CFG_FILE "\n");

  /* Extract commands */
  prOut->u16LoadedNum = 0U;
  s32Byte             = fgetc(CfgFile);
  do
  {
    if(isupper(s32Byte) || islower(s32Byte) || isdigit(s32Byte) || ('_' == s32Byte))
    {
      /* Command found */
      if(prOut->u16LoadedNum < CFGFILE_CMD_NUMBER)
      {
        /* Free buffer found - Read and store the command */
        u32WrIdx = 0U;
        do
        {
          if(u32WrIdx + 1U < CFGFILE_CMD_LEN)
          { /* Keeping 1 byte for terminating 0 */
            /* Store byte, load next */
            prOut->aszCmd[prOut->u16LoadedNum][u32WrIdx++] = (char)s32Byte;
            s32Byte                                        = fgetc(CfgFile);
          }
          else
          {
            /* Command too long */
            printf("[app] Command on line %u does not fit in buffer\n", u32LineCtr);
            bRetVal = false;
            s32Byte = EOF; /* Exit all loops */
          }
        } while((EOF != s32Byte) && ('#' != s32Byte) && ('\n' != s32Byte));
        /* Terminate current command string */
        prOut->aszCmd[prOut->u16LoadedNum][u32WrIdx] = 0;
        prOut->u16LoadedNum++;
      }
      else
      {
        /* Command memory full */
        printf("[app] No free command buffer, too many commands in configuration file.\n");
        bRetVal = false;
        s32Byte = EOF; /* Exit all loops */
      }
    }
    else if('#' == s32Byte)
    {
      /* Skip comment */
      do
      {
        s32Byte = fgetc(CfgFile);
      } while((EOF != s32Byte) && ('\n' != s32Byte));
    }
    else if(isspace(s32Byte))
    {
      /* Skip white-space */
      if('\n' == s32Byte)
        u32LineCtr++;
      s32Byte = fgetc(CfgFile);
    }
    else if(EOF == s32Byte)
    {
      ; /* Done */
    }
    else
    {
      /* Abort (Unexpected character) */
      printf("[app] Unexpected character '%c' on line number %u\n", (char)s32Byte, u32LineCtr);
      bRetVal = false;
      s32Byte = EOF; /* Exit all loops */
    }
  } while(EOF != s32Byte);

  fclose(CfgFile);
  return bRetVal;
}

//***************************************************************************
#if(0 != APP_CFG_ENABLE_DEBUG_OUTPUT_READER)
static void* sm_text_reader_body(void* pvArg)
{
#define BUF_SIZE 4096
  int           retval, count;
  int           iFd = open(SM_DRV, O_RDONLY);
  unsigned char au8Buffer[BUF_SIZE + 1U];

  if(iFd < 0)
  {
    printf("[app] Unable to open the %s for reading\n", SM_DRV);
    return NULL;
  }

  retval = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  if(retval != 0)
  {
    errno = retval;
    exit(EXIT_FAILURE);
  }

  while(false == sm_text_reader_shall_stop((pthread_mutex_t*)pvArg))
  {
    count = read(iFd, au8Buffer, BUF_SIZE);
    if(count > 0)
    {
      au8Buffer[count] = 0x0U;
      printf("%s", au8Buffer);
    }
  }

  close(iFd);
  return NULL;
}

static bool sm_text_reader_shall_stop(pthread_mutex_t* pMutex)
{
  int retval = pthread_mutex_trylock(pMutex);

  if(EBUSY == retval)
  {
    return false; /*  Shall not stop */
  }
  else if(0 == retval)
  {
    pthread_mutex_unlock(pMutex);
    return true; /*  Shall stop */
  }
  else
  {
    return true; /*  Shall stop */
  }
}
#endif /* APP_CFG_ENABLE_DEBUG_OUTPUT_READER */

//***************************************************************************
static void ExitClearly(void)
{
  /*  In case of crash, most actions are done automatically by Linux kernel:
        * memory (also SRAM) is automatically freed,
        * file descriptors (including open drivers) are automatically closed,
    */
  /* We need to stop firmware (if started) to free CPU resources */
  if(rExitInfo.StrmFD >= 0)
    (void)SmCommand(rExitInfo.StrmFD, "app stop");
  /* (void)video_app_stop(); */
}

//***************************************************************************
/** @}*/
