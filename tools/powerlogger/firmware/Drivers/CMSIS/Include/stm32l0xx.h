/**
  ******************************************************************************
  * @file    stm32l0xx.h
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M0+ Device Peripheral Access Layer Header File. 
  *          This file contains all the peripheral register's definitions, bits 
  *          definitions and memory mapping for STM32L0xx devices.            
  *            
  *          The file is the unique include file that the application programmer
  *          is using in the C source code, usually in main.c. This file contains:
  *           - Configuration section that allows to select:
  *              - The device used in the target application
  *              - To use or not the peripheral's drivers in application code(i.e. 
  *                code will be based on direct access to peripheral's registers 
  *                rather than drivers API), this option is controlled by 
  *                "#define USE_HAL_DRIVER"
  *  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright(c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************  
  */ 

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32l0xx
  * @{
  */
    
#ifndef __STM32L0xx_H
#define __STM32L0xx_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
   
/** @addtogroup Library_configuration_section
  * @{
  */

/**
  * @brief STM32 Family
  */
#if !defined (STM32L0)
#define STM32L0
#endif /* STM32L0 */

/* Uncomment the line below according to the target STM32 device used in your
   application 
  */

#if !defined (STM32L010x4) && !defined (STM32L010x6) && !defined (STM32L010x8) && !defined (STM32L010xB) && \
    !defined (STM32L011xx) && !defined (STM32L021xx) && \
    !defined (STM32L031xx) && !defined (STM32L041xx) && \
    !defined (STM32L051xx) && !defined (STM32L052xx) && !defined (STM32L053xx) && \
    !defined (STM32L062xx) && !defined (STM32L063xx) && \
    !defined (STM32L071xx) && !defined (STM32L072xx) && !defined (STM32L073xx) && \
    !defined (STM32L081xx) && !defined (STM32L082xx) && !defined (STM32L083xx)
   #define STM32L010x4   
 
#endif
   
/*  Tip: To avoid modifying this file each time you need to switch between these
        devices, you can define the device in your toolchain compiler preprocessor.
  */

/**
  * @brief CMSIS Device version number
  */
#define __STM32L0xx_CMSIS_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __STM32L0xx_CMSIS_VERSION_SUB1   (0x09) /*!< [23:16] sub1 version */
#define __STM32L0xx_CMSIS_VERSION_SUB2   (0x01) /*!< [15:8]  sub2 version */
#define __STM32L0xx_CMSIS_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32L0xx_CMSIS_VERSION        ((__STM32L0xx_CMSIS_VERSION_MAIN     << 24)\
                                         |(__STM32L0xx_CMSIS_VERSION_SUB1 << 16)\
                                         |(__STM32L0xx_CMSIS_VERSION_SUB2 << 8 )\
                                         |(__STM32L0xx_CMSIS_VERSION_RC))
                                             
/**
  * @}
  */

/** @addtogroup Device_Included
  * @{
  */
#if defined(STM32L010xB)
  #include "stm32l010xb.h"
#elif defined(STM32L010x8)
  #include "stm32l010x8.h"
#elif defined(STM32L010x6)
  #include "stm32l010x6.h"
#elif defined(STM32L010x4)
  #include "stm32l010x4.h"
#elif defined(STM32L011xx)
  #include "stm32l011xx.h"
#elif defined(STM32L021xx)
  #include "stm32l021xx.h"
#elif defined(STM32L031xx)
  #include "stm32l031xx.h"
#elif defined(STM32L041xx)
  #include "stm32l041xx.h"
#elif defined(STM32L051xx)
  #include "stm32l051xx.h"
#elif defined(STM32L052xx)
  #include "stm32l052xx.h"
#elif defined(STM32L053xx)
  #include "stm32l053xx.h"
#elif defined(STM32L062xx)
  #include "stm32l062xx.h"
#elif defined(STM32L063xx)
  #include "stm32l063xx.h"
#elif defined(STM32L071xx)
  #include "stm32l071xx.h"
#elif defined(STM32L072xx)
  #include "stm32l072xx.h"
#elif defined(STM32L073xx)
  #include "stm32l073xx.h"
#elif defined(STM32L082xx)
  #include "stm32l082xx.h"
#elif defined(STM32L083xx)
  #include "stm32l083xx.h"
#elif defined(STM32L081xx)
  #include "stm32l081xx.h"
#else
 #error "Please select first the target STM32L0xx device used in your application (in stm32l0xx.h file)"
#endif

/**
  * @}
  */

/** @addtogroup Exported_types
  * @{
  */ 
typedef enum 
{
  RESET = 0, 
  SET = !RESET
} FlagStatus, ITStatus;

typedef enum 
{
  DISABLE = 0, 
  ENABLE = !DISABLE
} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum 
{
  SUCCESS = 0,
  ERROR = !SUCCESS
} ErrorStatus;

/**
  * @}
  */


/** @addtogroup Exported_macro
  * @{
  */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32L0xx_H */
/**
  * @}
  */

/**
  * @}
  */
  



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
