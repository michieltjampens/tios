/*
 * i2c1.h
 */

#ifndef INC_I2C1_H_
#define INC_I2C1_H_
/* Includes ------------------------------------------------------------------*/
#include "./stm32l0xx.h"
/* Private includes ----------------------------------------------------------*/
#include "hardware.h"
/* Shared Variables ------------------------------------------------------------*/
extern __IO uint32_t Tick;
extern __IO uint16_t error;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void I2C1_Configure_Master(void);
uint8_t I2C1_SendData( uint8_t addr, uint8_t length, uint8_t *data);
uint8_t I2C1_SendSingleByte( uint8_t addr, uint8_t data);
/* Reading */
uint8_t I2C1_ReadData( uint8_t i2c_Address, uint8_t reg, uint8_t length);
uint8_t I2C1_Read8bitData( uint8_t i2c_Address, uint8_t reg, uint8_t length, uint8_t *data);
uint8_t I2C1_Read16bitData( uint8_t i2c_Address, uint8_t reg, uint8_t length, uint16_t *data);
/* Other */
void I2C1_FindDevices(void);
uint8_t I2C1_PokeDevice( uint8_t i2c_Address );
void I2C1_IRQHandler(void);

/* Private defines -----------------------------------------------------------*/
#define ERROR_I2C 0x10
#define ERROR_I2C_TIMEOUT  0x11
#define ERROR_I2C_TIMEOUT2 0x12
#define ERROR_I2C_TIMEOUT3 0x13
#define ERROR_I2C_TIMEOUT4 0x14
#define ERROR_I2C_NONACK   0x15

#define I2C_TIMEOUT_VALUE 0x05
#define I2C_INVALID_ADDRES 0x03
#define I2C_NOTICK 0x04

#endif /* INC_I2C1_H_ */
