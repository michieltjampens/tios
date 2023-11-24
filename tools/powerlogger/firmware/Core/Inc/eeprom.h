#ifndef INC_EEPROM_H_
	#define INC_EEPROM_H_

	/* Includes ------------------------------------------------------------------*/
	#include "./stm32l0xx.h"
	/* Private includes ----------------------------------------------------------*/
	/* Shared Variables  ---------------------------------------------------------*/
	extern __IO uint32_t Tick;
	extern __IO uint16_t error;
	/* Exported types ------------------------------------------------------------*/

	/* Exported constants --------------------------------------------------------*/

	/* Exported macro ------------------------------------------------------------*/

	/* Exported functions prototypes ---------------------------------------------*/
	void UnlockPELOCK(void);
	void LockNVM(void);
	void EepromErase(uint32_t addr);
	void EepromWrite(uint32_t addr, uint32_t data);
	/* Private defines -----------------------------------------------------------*/
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

	#define DATA_E2_ADDR 	  	 ((uint32_t)0x08080000)  /* Data EEPROM base address */
	#define DATA_E2_END_ADDR 	 ((uint32_t)0x0808007F)  /* Data EEPROM end address 128bytes */

	#define DATA_BYTE            ((uint8_t)0x12)         /* 8-bits value to be programmed */
	#define DATA_16B_WORD        ((uint16_t)0x3456)      /* 16-bits value to be programmed */
	#define DATA_32B_WORD        ((uint32_t)0x89ABCDEF)  /* 32-bits value to be programmed */

	#define CONFIG ((Eeprom_registers *) DATA_E2_ADDR)

	/* Error codes */
	#define ERROR_ERASE 0x01
	#define ERROR_PROG_BYTE  0x02
	#define ERROR_PROG_16B_WORD 0x04
	#define ERROR_PROG_32B_WORD 0x08
	#define ERROR_WRITE_PROTECTION 0x10
	#define ERROR_READOUT_PROTECTION 0x20
	#define ERROR_FETCH_DURING_PROG 0x40
	#define ERROR_SIZE 0x80
	#define ERROR_ALIGNMENT 0x100
	#define ERROR_NOT_ZERO 0x200
	#define ERROR_OPTION_NOT_VALID 0x400
	#define ERROR_UNKNOWN 0x800

#endif /* INC_EEPROM_H_ */
