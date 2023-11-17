/**
 * Class with methods to use the I2C1 peripheral on the STM32L0x0 device
 */
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "i2c1.h"

/* Private variables ---------------------------------------------------------*/
uint8_t * i2c_txBuffer;
uint8_t * i2c_rxBuffer_8bit;
uint16_t * i2c_rxBuffer_16bit;
uint8_t bits=0x00;
uint8_t rec=0x00;
uint16_t test=0;
/* *********************************** S E T U P *************************************************** */
/**
 * Brief Init the I2C1 peripheral and the used GPIO's
 */
__INLINE void I2C1_Configure_Master(void){

  SET_BIT(RCC->APB1ENR,RCC_APB1ENR_I2C1EN); // Enable the peripheral clock I2C1 do this before GPIO's

  /* GPIO Setup, PB6=SCL and PB7=SDA */
  SET_BIT(RCC->IOPENR,RCC_IOPENR_GPIOBEN);  // Enable the peripheral clock of GPIOA
  MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODE6|GPIO_MODER_MODE7, GPIO_MODER_MODE6_1|GPIO_MODER_MODE7_1); //Use AF
  GPIOB->AFR[0] = (GPIOB->AFR[0] &~ (0x00000FF0)) | (1 << (6 * 4)) | (1 << (7 * 4)); // Pick AF1 (I2C1)
  GPIOB->OTYPER |= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7; // Open drain

  /* Configure I2C1 as master */
  I2C1->TIMINGR = (uint32_t)0x00503D5A; // Timing setup: 100kHz mode, PCLK=16MHz, rise=100ns,fall=10ns (STM32CubeMX calculated)
  SET_BIT(I2C1->CR1,I2C_CR1_RXIE | I2C_CR1_TXIE); // Enable Receive and transmit interrupt

  /* Configure Interrupts */
  NVIC_SetPriority(I2C1_IRQn, 0);
  NVIC_EnableIRQ(I2C1_IRQn);

  SET_BIT(I2C1->CR1,I2C_CR1_PE); 		// Enable I2C peripheral
  while((I2C1->CR1 & I2C_CR1_PE) == 0); // Wait for it to be enabled
}
/* *********************************** S E N D I N G ************************************************* */
/**
 * Brief	Send data to a device on the bus
 * Param
 *   i2c_Address  - 7bit address (so omit the r/w bit)
 *   length - the amount of bytes to send
 *   data  - pointer to array containing length of bytes
 * Retval
 * 		0 -> No device
 * 		1 -> Device replied
 * 		x -> Error
 */
uint8_t I2C1_SendData( uint8_t i2c_Address, uint8_t length, uint8_t *data){
	uint32_t tickstart;

	if( length != 0x00 ){ // No use processing data if length is 0
		while((I2C1->ISR & I2C_ISR_TXE) == 0);  // Wait till transfer buffer is empty
		i2c_txBuffer = data;					// Copy the address of the data
		I2C1 ->TXDR = *i2c_txBuffer++;  		// Load the first byte
	}
	// Enable auto-end, set amount of bytes to send and the address shifted for R/W bit
	I2C1->CR2 = I2C_CR2_AUTOEND | (length << I2C_CR2_NBYTES_Pos) | (i2c_Address << 1);

	I2C1->CR2 |= I2C_CR2_START; // Send start condition

	///Wait for the START condition detection, maximum 10ms
	tickstart = Tick;
	while((I2C1->ISR & I2C_ISR_BUSY) == 0 ){ // Wait till busy flag is raised
		if ((Tick - tickstart ) > I2C_TIMEOUT_VALUE){
			return ERROR_I2C_TIMEOUT;
		}
	}
	///Then check when BUSY flags gets cleared (STOP detected), 10ms max
	tickstart = Tick;
	while((I2C1->ISR & I2C_ISR_STOPF) == 0){
		if ((Tick - tickstart ) > I2C_TIMEOUT_VALUE){
			return ERROR_I2C_TIMEOUT;
		}
	}
	SET_BIT(I2C1->ICR, I2C_ICR_STOPCF); // Clear flag
	if( I2C1->ISR & I2C_ISR_NACKF ){ // Check if NACKF is raised
		SET_BIT(I2C1->ICR, I2C_ICR_NACKCF); // Clear it
		return 0;
	}else{
		return 1;
	}
}
/**
 * Brief	Send a single byte to the given address
 * Param
 *   i2c_Address  - 7bit address (so omit the r/w bit)
 *   data  - pointer to array containing length of bytes
 * Retval
 * 		0 -> No device
 *		1 -> Device replied
 * 		x -> Error
 */
uint8_t I2C1_SendSingleByte( uint8_t i2c_Address, uint8_t data){
	if( i2c_Address==0x00)
		return 0;
	uint8_t d[1]={data};
	return I2C1_SendData( i2c_Address,1,d);
}
/* ******************************** R E C E I V I N G ************************************************* */
uint8_t I2C1_Read16bitData( uint8_t i2c_Address, uint8_t reg, uint8_t length, uint16_t *data){
	i2c_rxBuffer_16bit = data;
	length *=2; // Because the bus reads a byte at a time
	bits=16;
	rec=0;
	return I2C1_ReadData( i2c_Address, reg, length);
}
uint8_t I2C1_Read8bitData( uint8_t i2c_Address, uint8_t reg, uint8_t length, uint8_t *data){
	i2c_rxBuffer_8bit = data;
	bits=8;
	return I2C1_ReadData( i2c_Address, reg, length);
}
uint8_t I2C1_ReadData( uint8_t i2c_Address, uint8_t reg, uint8_t length){
	uint32_t tickstart;

	rec=length;

	tickstart = Tick;
	while((I2C1->ISR & I2C_ISR_TXE) == 0){  // Wait till transfer buffer is empty
		if ((Tick - tickstart ) > I2C_TIMEOUT_VALUE){
			return ERROR_I2C_TIMEOUT;
		}
	}
	I2C1 ->TXDR = reg;  		// Load the first byte

	// No auto-end, set amount of bytes to send and the address shifted for R/W bit
	I2C1->CR2 = (1 << I2C_CR2_NBYTES_Pos) | (i2c_Address << 1);

	I2C1->CR2 |= I2C_CR2_START; // Send start condition

	///Wait for the START condition detection, maximum 10ms
	tickstart = Tick;
	while((I2C1->ISR & I2C_ISR_BUSY) == 0 ){ // Wait till busy flag is raised
		if ((Tick - tickstart ) > I2C_TIMEOUT_VALUE){
			return ERROR_I2C_TIMEOUT;
		}
	}
	///Then check when TC flag gets set (TC detected), 10ms max
	tickstart = Tick;
	while( (I2C1->ISR & I2C_ISR_TC)==0 ){
		if ((Tick - tickstart ) > I2C_TIMEOUT_VALUE){
			return ERROR_I2C_TIMEOUT2;
		}
	}
	// Now the register is set, read the data?

	// Enable auto-end, set amount of bytes to receive and the address shifted for R/W bit
	I2C1->CR2 = I2C_CR2_AUTOEND | (length<<16) | I2C_CR2_RD_WRN |(i2c_Address<<1);
	I2C1->CR2 |= I2C_CR2_START; // Send start condition

	///Wait for the START condition detection, maximum 10ms
	tickstart = Tick;
	while((I2C1->ISR & I2C_ISR_BUSY) == 0 ){ // Wait till busy flag is raised
		if ((Tick - tickstart ) > I2C_TIMEOUT_VALUE){
			return ERROR_I2C_TIMEOUT3;
		}
	}
	///Then check when BUSY flags gets cleared (STOP detected), 10ms max
	tickstart = Tick;
	while((I2C1->ISR & I2C_ISR_STOPF) == 0){
		if ((Tick - tickstart ) > I2C_TIMEOUT_VALUE){
			return ERROR_I2C_TIMEOUT4;
		}
	}
	SET_BIT(I2C1->ICR, I2C_ICR_STOPCF); // Clear flag

	tickstart = Tick;
	while(rec != 0){ //wait for receiving the data?
		if ((Tick - tickstart ) > I2C_TIMEOUT_VALUE){
			return ERROR_I2C_TIMEOUT4;
		}
	}
	return 1;
}

/* *********************************** U T I L I T Y ************************************************* */
/**
 * Brief	Check if there's a device on the given address
 * Param
 *   	i2c_Address  - 7bit address (so omit the r/w bit)
 * Retval
 * 		0 -> No device
 * 		1 -> Device replied
 * 		x -> Error
 */
uint8_t I2C1_PokeDevice( uint8_t i2c_Address ){
	return I2C1_SendData( i2c_Address,0x00,0x00);
}
/* *************************************** I R Q **************************************************** */
void I2C1_IRQHandler(void){

  if(I2C1->ISR & I2C_ISR_RXNE){ // Receive buffer not empty
	  if( bits == 8 ){
		  *i2c_rxBuffer_8bit = I2C1->RXDR;
		  i2c_rxBuffer_8bit++;
		  rec--;
	  }else if( bits == 16 ){
		  if( rec%2==0 ){
			  *i2c_rxBuffer_16bit = I2C1->RXDR; /* Read receive register, will clear RXNE flag */
			  *i2c_rxBuffer_16bit = *i2c_rxBuffer_16bit <<8; //Receiving MSB first, so shift it to msb
		  }else{
			  *i2c_rxBuffer_16bit += I2C1->RXDR; // Add the LSB
			  i2c_rxBuffer_16bit++; // Increment the pointer
		  }
		  rec--;
	  }
  }else if(I2C1->ISR & I2C_ISR_TXIS){ /*  Ready to send the next byte */
	  I2C1 ->TXDR = *i2c_txBuffer++; // Put the next byte
  }else if(I2C1->ISR & I2C_ISR_NACKF){ /* NACK Received*/
	  SET_BIT(I2C1->ICR, I2C_ICR_NACKCF); // Clear flag
  }else if(I2C1->ISR & I2C_ISR_STOPF){
	  SET_BIT(I2C1->ICR, I2C_ICR_STOPCF); // Clear flag
  }else if(I2C1->ISR & I2C_ISR_TC){ // Transfer complete?
	  test=0;
  }else{
    error = ERROR_I2C; /* Report an error */
    NVIC_DisableIRQ(I2C1_IRQn); /* Disable I2C2_IRQn */
  }
}
