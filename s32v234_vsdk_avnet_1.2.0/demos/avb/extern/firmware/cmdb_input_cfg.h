/**
  @addtogroup       dxgrSM_CORE
  @{

  @file             cmdb_input_cfg.h
  @brief            Configuration file for the command buffer module

  Project           Video Listener

  Platform          S32V234

  SWVersion         1.1.1

  Copyright 2018 NXP
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

#ifndef CMDB_INPUT_CFG_H
    #define CMDB_INPUT_CFG_H

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
/**
* @def CMDB_CFG_BUF_SIZE
* @brief Configures size of the command buffer 
* @details Configures size of the buffer used to store one command before it is processed. 
*          The value shall be in range 1 to 16384 inclusive.
* @hideinitializer
*/
#define CMDB_CFG_BUF_SIZE  256U

#if 0
/**
* @def CMDB_CFG_BUF_ADDR
* @brief Configures externally supplied buffer address 
* @details The macro value (if it is defined) is used as a base address of the debug buffer
*          of size DBGB_CFG_BUF_SIZE. The buffer is created internally if the macro is not defined.
*
*          It is recommended to not define this macro.
* @hideinitializer
*/
#define CMDB_CFG_BUF_ADDR 0U /* Replace zero with a valid address */
#endif
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


#endif /* CMDB_INPUT_CFG_H */
/** @}*/
