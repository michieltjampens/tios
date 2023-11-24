#ifndef INC_LPUART1_H_
#define INC_LPUART1_H_


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "./stm32l0xx.h"

/* Private includes ----------------------------------------------------------*/
/* Shared Variables  ---------------------------------------------------------*/
extern __IO uint32_t Tick;
extern __IO uint16_t error;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
void LPUART1_Configure(void);
uint8_t LPUART1_Buffer_Free(void);
void LPUART1_Configure_Setup(void);
void LPUART1_Configure_GPIO(void);

void LPUART1_Check_Circular(void);

void LPUART1_SendString( const char *buffer );
void LPUART1_SendBytes( uint8_t *buffer );
void LPUART1_SendArray( uint8_t *buffer, uint8_t length );
void LPUART1_SendByte( uint8_t data );
void LPUART1_SendDec( uint16_t nr );
void LPUART1_SendHex( uint16_t number );
void LPUART1_SendByteHexNoPrefix( uint8_t nr );
void LPUART1_SendWordHexNoPrefix( uint16_t nr );
void LPUART1_SendCRLF(void);

void LPUART1_Transfer_Buffer( void );
uint8_t LPUART1_hasCmd(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ERROR_USART_TRANSMIT 0x20
#define ERROR_USART_ISR_TC 	 0x21
#define ERROR_USART_TIMEOUT  0x22

#define CIRCULAR 128

extern uint32_t Tock;
#ifdef __cplusplus
}
#endif

#endif /* INC_LPUART1_H_ */
