/**
  @addtogroup       dxgrSCORE
  @{

  @file             stream_core_bitmasks.h
  @brief            Definitions of bits and bitmasks.
  @details          Used to create and handle POLL signals from firmware to userpsace application.

  Project           Video Listener

  Platform          S32V234

  SWVersion         1.1.1

  Copyright 2018 NXP
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

#ifndef STREAM_CORE_BITMASKS_H
    #define STREAM_CORE_BITMASKS_H

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
/* No include shall be here. Exceptions must be approved */

/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
/* Runtime errors - Error bits to be used in SCORE_trCtl.u32ErrMask */
/** @def SCORE_ERR_STRM_NUM_0
 *  @brief Run-time error flag indicating issues with received video stream 0.
 */
#define SCORE_ERR_STRM_NUM_0            (uint32_t)(1UL<<0)
/** @def SCORE_ERR_STRM_NUM_1
 *  @brief Run-time error flag indicating issues with received video stream 1.
 */
#define SCORE_ERR_STRM_NUM_1            (uint32_t)(1UL<<1)
/** @def SCORE_ERR_STRM_NUM_2
 *  @brief Run-time error flag indicating issues with received video stream 2.
 */
#define SCORE_ERR_STRM_NUM_2            (uint32_t)(1UL<<2)
/** @def SCORE_ERR_STRM_NUM_3
 *  @brief Run-time error flag indicating issues with received video stream 3.
 */
#define SCORE_ERR_STRM_NUM_3            (uint32_t)(1UL<<3)
/** @def SCORE_ERR_STREAM_DROP_OUT
 *  @brief Run-time error flag indicating that from one or more streams no data are being received.
 */
#define SCORE_ERR_STREAM_DROP_OUT       (uint32_t)(1UL<<4)
/** @def SCORE_ERR_OUT_OF_ORDER_FRAME
 *  @brief Run-time error flag indicating that one or more packets are missing in received stream(s).
 */
#define SCORE_ERR_OUT_OF_ORDER_FRAME    (uint32_t)(1UL<<5)
/** @def SCORE_ERR_ETH_BUFFER_OVERWRITE
 *  @brief Run-time error flag indicating that Ethernet Rx queue overflown and data were overwritten.
 */
#define SCORE_ERR_ETH_BUFFER_OVERWRITE  (uint32_t)(1UL<<6)
/** @def SCORE_ERR_STREAM_Q_FULL
 *  @brief Run-time error flag indicating that stream queue(s) overflown.
 */
#define SCORE_ERR_STREAM_Q_FULL         (uint32_t)(1UL<<7)
/** @def SCORE_ERR_STREAM_Q_BROKEN
 *  @brief Run-time error flag indicating serious error in handling of stream queues.
 */
#define SCORE_ERR_STREAM_Q_BROKEN       (uint32_t)(1UL<<8)
/** @def SCORE_ERR_DECODER_ERR
 *  @brief Run-time error flag indicating decoder hardware stall or crash.
 */
#define SCORE_ERR_DECODER_ERR           (uint32_t)(1UL<<9) /*  */
/** @def SCORE_ERR_DEC_Q_BROKEN
 *  @brief Run-time error flag indicating serious error in handling of decoder queues.
 */
#define SCORE_ERR_DEC_Q_BROKEN          (uint32_t)(1UL<<10)
/* Streaming did not start in time */
/** @def SCORE_ERR_NO_TRAFFIC
 *  @brief Run-time error flag indicating that no (or too low) Ethernet traffic is being received with matching VLAN priority.
 */
#define SCORE_ERR_NO_TRAFFIC            (uint32_t)(1UL<<11)
/** @def SCORE_ERR_FATAL_ERR
 *  @brief Run-time error flag indicating possible bug. Automatic recovery is not possible.
 */
#define SCORE_ERR_FATAL_ERR             (uint32_t)(1UL<<30)
/** @def SCORE_ERR_SM_APP_FAILED
 *  @brief Run-time error flag indicating fatal error in code handling execution of SM application.
 */
#define SCORE_ERR_SM_APP_FAILED         (uint32_t)(1UL<<31)

/* Warning bits - indication of suspicious findings to be used in SCORE_trCtl.u32WarnMask */
/* NOT part of userspace app API yet */
/* @def SCORE_WAR_UNKNOWN_STREAM_ID
 *  @brief Run-time warning flag indicating that at least one packet(s) not belonging to configured streams was received.
 *  @details Possible causes: non-stream packet was received due to its matching VLAN priority,
 *                            stream packet has unsupported format,
 *                            configured stream ID does not match the stream ID of the stream.
 *  @note Unidentified packets, before they are dropped, are occupying buffers in Ethernet Rx queue.
 *        This may lead to Ethernet queue overflow, if it is not big enough.
 */
#define SCORE_WAR_UNKNOWN_STREAM_ID     (uint32_t)(1UL<<24) /* Frame(s) dropped due to unknown stream ID */

/* Bits to be used in SCORE_trCtl.u32UsrAppMsgMask */
/** @def SCORE_MSG_RESET_DECODER
 *  @brief POLL_MSG bit, orders user app to reset decoder and send APP_KEY_START_FEEDING when done.
 */
#define SCORE_MSG_RESET_DECODER         (uint32_t)(1UL<<0)
/** @def SCORE_MSG_START_CAPTURING
 *  @brief POLL_MSG bit, orders user app to start capturing of decoded images
 */
#define SCORE_MSG_START_CAPTURING       (uint32_t)(1UL<<1)

/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

#endif /* STREAM_CORE_BITMASKS_H */
/** @}*/
