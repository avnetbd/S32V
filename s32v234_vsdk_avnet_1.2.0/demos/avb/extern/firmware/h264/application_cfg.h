/**
  @addtogroup       dxgrSCORE
  @{

  @file             application_cfg.h
  @brief            Static part of Video Listener streaming application configuration
  @details

  Project           Video Listener
  Platform          S32V234

  SWVersion         1.1.1

  Copyright 2016-2018 NXP
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

#ifndef APPLICATION_CFG_H
    #define APPLICATION_CFG_H

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
/*
* @def      APP_CFG_ZERO_BUF_ENABLED
* @brief    Enables or disables insertion of one zero buffer after each video frame.
* @details  Needed for H264 decoder on S32V234. In case of JPEG decoder turn it off.
*/
#define APP_CFG_ZERO_BUF_ENABLED    TRUE
#if TRUE == APP_CFG_ZERO_BUF_ENABLED
    /*
    * @def      APP_CFG_ZERO_BUF_HEAD_LENGTH
    * @brief    Length of the header in zero buffer which is needed for the software.
    * @details  Shall reflect maximal expected length of Ethernet frame header of a video packet.
    *           Software expects that the header is always present in front of the data.
    */
    #define APP_CFG_ZERO_BUF_HEAD_LENGTH  64U
    /*
    * @def      APP_CFG_ZERO_BUF_DATA_LENGTH
    * @brief    Length of the data in zero buffer which is pushed to the decoder.
    */
    #define APP_CFG_ZERO_BUF_DATA_LENGTH  128U
#endif /* APP_CFG_ZERO_BUF_ENABLED */

#define APP_CFG_PIT_INSTANCE        0U
#define APP_CFG_PIT_CHANNEL         0U
#define APP_CFG_PIT_IRQ_ID          63U

/*  If TRUE then a PIT will measure time spent within the IRQ handler */
#define APP_CFG_DO_PERF             TRUE
#define APP_CFG_PIT_AUX1_CHANNEL    2U

/* Other application wide settings */
#define APP_CFG_MAX_STREAMS         4U

/*  Define KEYs of system calls, which shall be handled from application */
/*
* @def      APP_KEY_EXEC_CMDB
* @brief    Execute command defined by command buffer (string)
* @details  System call key to be used in sm_install_sc_handler
*/
#define APP_KEY_EXEC_CMDB           0x1009U

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

#endif /* APPLICATION_CFG_H */
/** @}*/
