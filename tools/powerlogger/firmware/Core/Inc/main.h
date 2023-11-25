/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "./stm32l0xx.h"
#include "./core_cm0plus.h"

/* Variables -----------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "hardware.h"
#include "lpuart1.h"
#include "i2c1.h"
#include "eeprom.h"
#include <string.h>

/* External variables ---------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void SystemClock_Config(void);
void init(void);
void configure_IO(void);
void Output_Enable( uint8_t out );  // Toggle a specific output on
void Output_Disable( uint8_t out ); // Toggle a specific output off
void Disable_All_Outputs( void );      // Toggle all outputs off
void Enable_All_Outputs( void );
void executeCommand( uint8_t * cmd );
void sendData(void);
uint8_t I2C1_FindPAC1954(void);
void readAccumulator( uint8_t acc );
void resetSettings(void);
uint32_t settings_write(void);
void EXTI4_15_IRQ_handler(void);
/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
/* Time-out values */
#define HSI_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define PLL_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define CLOCKSWITCH_TIMEOUT_VALUE  ((uint32_t)5000) /* 5 s    */

/* Error codes used to make the orange led blinking */
#define ERROR_HSI_TIMEOUT 0x01
#define ERROR_PLL_TIMEOUT 0x02
#define ERROR_CLKSWITCH_TIMEOUT 0x03

/* NVM key definitions */
#define FLASH_PDKEY1               ((uint32_t)0x04152637) /*!< Flash power down key1 */
#define FLASH_PDKEY2               ((uint32_t)0xFAFBFCFD) /*!< Flash power down key2: used with FLASH_PDKEY1
                                                              to unlock the RUN_PD bit in FLASH_ACR */

#define FLASH_PEKEY1               ((uint32_t)0x89ABCDEF) /*!< Flash program erase key1 */
#define FLASH_PEKEY2               ((uint32_t)0x02030405) /*!< Flash program erase key: used with FLASH_PEKEY2
                                                               to unlock the write access to the FLASH_PECR register and
                                                               data EEPROM */

#define FLASH_PRGKEY1              ((uint32_t)0x8C9DAEBF) /*!< Flash program memory key1 */
#define FLASH_PRGKEY2              ((uint32_t)0x13141516) /*!< Flash program memory key2: used with FLASH_PRGKEY2
                                                               to unlock the program memory */

#define FLASH_OPTKEY1              ((uint32_t)0xFBEAD9C8) /*!< Flash option key1 */
#define FLASH_OPTKEY2              ((uint32_t)0x24252627) /*!< Flash option key2: used with FLASH_OPTKEY1 to
                                                              unlock the write access to the option byte block */

#define PAC_FOUND 0x01
#define PAC_REFRESHED 0x02

#ifdef __cplusplus
}
#endif

#endif
