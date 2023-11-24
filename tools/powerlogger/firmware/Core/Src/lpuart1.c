/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "../inc/lpuart1.h"
#include "../inc/main.h"

/* Shared variables ----------------------------------------------------------*/
__IO uint8_t cmdReady;

/* Private variables ---------------------------------------------------------*/

/* Circular buffer for UART OUT*/
uint8_t outputBuffer[CIRCULAR];
uint8_t *outTemp;
uint8_t *outStart;
uint8_t *outEnd;
uint8_t *outHead;
uint8_t *outTail;

/* Circular buffer for UART IN*/
uint8_t inputBuffer[64];
uint8_t *inputTemp;
uint8_t *inputStart;
uint8_t *inputEnd;
uint8_t *inputHead;
uint8_t *inputTail;


void LPUART1_Configure(){
    /* Initialise the circular buffers */
    inputStart = &inputBuffer[0];
    inputEnd =   &inputBuffer[0];
    inputHead =  &inputBuffer[0];
    inputTail =  &inputBuffer[64-1];

    outStart = &outputBuffer[0];
    outEnd =   &outputBuffer[0];
    outHead =  &outputBuffer[0];
    outTail =  &outputBuffer[CIRCULAR-1];

    LPUART1_Configure_GPIO();
    LPUART1_Configure_Setup();

    cmdReady=0;
}
/**
 * Get the space in the buffer
 */
uint8_t LPUART1_Buffer_Free(void){
	if( inputEnd > inputStart )
		return inputEnd-inputStart;
	return (inputTail-inputStart)+(inputEnd-inputHead);
}
void LPUART1_Configure_GPIO(void){
  /* Enable the peripheral clock of GPIOA */
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

  /* GPIO configuration for USART1 signals */
  /* (1) Select Alternate Function (AF) mode (b10) on PA0 and PA1 */
  /* Moder LSB nibble: 1111 -> 1010 */
  GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE0|GPIO_MODER_MODE1))\
                 | (GPIO_MODER_MODE0_1 | GPIO_MODER_MODE1_1);
  /* (2) AF4 for LPUSART1 signals, the line is explained in detail below it */
  GPIOA->AFR[0] = (GPIOA->AFR[0] &~ (0x00000FF0)) | (6 << (0 * 4)) | (6 << (1 * 4));

  /* Extra info                                                                         */
  /* For the alternate functions, check the datasheet 'Alternate functions'             */
  /* AFR[0]=pin 0 to 7, AFR[1]=pin 8 to 15, each pin has 4 bits for selection           */
  /* So AFR[0] &~(0x00000FF0) = AFR[0] & 0xFFFFF00F -> Reset the bits for PA0 & PA1     */
  /* 4 << (0 * 4) -> Value 4 shifted one nibble to get to PA0 position  -> 0x00000004   */
  /* 4 << (1 * 4) -> Value 4 shifted two nibbles to get to PA1 position -> 0x00000040   */
}
void LPUART1_Configure_Setup(void){
	uint32_t tickstart;

    /* Enable the peripheral clock USART1 */
    RCC->APB1ENR |= RCC_APB1ENR_LPUART1EN;
    /* Configure USART1 */
    /* System clock is 16MHz (1MHz oversampling by 16), 19200 baud (both already divided by 100) */
    LPUART1->BRR = (256*160000)/192;//19200??

    LPUART1->CR2 |= USART_CR2_SWAP; /* Swap RX and TX this needs to be set before CR1_UE is set*/

    /* 8 data bit, 1 start bit, 1 stop bit, no parity */
    LPUART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
    /* Extra info                                 */
    /* USART2->CR1 = USART2 Control register      */
    /* 8/1/1 no parity is default                 */
    /* USART_CR1_RE = Receiver enable             */
    /* USART_CR1_TE = Transmitter Enable          */
    /* USART_CR1_UE = USART Enable                */

    /* polling idle frame Transmission */
    tickstart = Tick;
    while((LPUART1->ISR & USART_ISR_TC) != USART_ISR_TC){
    	if ((Tick - tickstart ) > I2C_TIMEOUT_VALUE){
			error = ERROR_USART_TIMEOUT;
			return;
		}
    }
	LPUART1->ICR |= USART_ICR_TCCF;  /* Clear TC flag  (no bit for receive) */
	LPUART1->CR1 |= USART_CR1_TCIE | USART_CR1_RXNEIE; /* Enable Transmission Complete and receive interrupt */

	/* Configure Interrupt */
	/* Set priority for USART1_IRQn */
	NVIC_SetPriority(LPUART1_IRQn, 0);
	/* Enable USART1_IRQn */
	NVIC_EnableIRQ(LPUART1_IRQn);
}


/**
 * Brief Send elements from an array in the send buffer till a 0x00 is found
 * Param
 * 		buffer -> Pointer to the array
 * RetVal
 * 		None
 */
void LPUART1_SendBytes( uint8_t *buffer ){
    while( *buffer != 0x00){
        *outEnd++ = *buffer++;
        if (outEnd == outTail) { // So never write on the tail!
            outEnd = outHead;    // End reached, back to head
        }
    }
    LPUART1_Check_Circular();
}

/**
 * Brief Send the given amount of elements from an array
 * Param
 * 		buffer -> Pointer to the array
 * 		length -> amount of elements to add
 * RetVal
 * 		None
 */
void LPUART1_SendArray( uint8_t *buffer, uint8_t length ){
    while( length != 0x00){
        *outEnd++ = *buffer++;
        if (outEnd == outTail) { // So never write on the tail!
            outEnd = outHead;    // End reached, back to head
        }
        length--;
    }
    LPUART1_Check_Circular();
}
void LPUART1_SendArrayReversed( uint8_t *buffer, uint8_t length ){
	buffer= buffer+length-1;
    while( length != 0x00){
        *outEnd++ = *buffer--;
        if (outEnd == outTail) { // So never write on the tail!
            outEnd = outHead;    // End reached, back to head
        }
        length--;
    }
    LPUART1_Check_Circular();
}
/**
 * Brief Send a single byte of data
 * Param
 * 		data -> The byte to send
 */
void LPUART1_SendByte( uint8_t data ){
    LPUART1_SendArray(&data,1);
}
/**
 * Brief Send a string
 * Param
 * 		buffer -> The string to send (fe."foobar")
 */
void LPUART1_SendString( const char *buffer ){
	LPUART1_SendBytes( (uint8_t*) buffer);
}
/**
 * Brief Convert a 16bit number to hex ascii and send it
 * Param
 * 		nr -> The number to send
 */
void LPUART1_SendHex( uint16_t nr ){
	uint8_t data[7]={'0','x','0','0',0,0,0};
	uint8_t tmp;
	uint8_t index=5;
	if( nr <= 0xFF ){
		index=3;
	}
	while( index > 1 ){
		tmp = nr%16;
		if(tmp>9){
			data[index]= tmp+55;
		}else{
			data[index] = tmp + '0';
		}
		nr -= tmp;
		nr /= 16;
		index --;
	}
	LPUART1_SendBytes(data);
}
void LPUART1_SendWordHexNoPrefix( uint16_t nr ){
	uint8_t data[5]={'0','0','0','0',0};
	uint8_t tmp;
	uint8_t index=3;

	while( nr!=0 ){
		tmp = nr%16;
		if(tmp>9){
			data[index]= tmp+55;
		}else{
			data[index] = tmp + '0';
		}
		nr /= 16;
		index --;
	}
	LPUART1_SendBytes(data);
}
void LPUART1_SendByteHexNoPrefix( uint8_t nr ){
	uint8_t data[5]={'0','0',0};
	uint8_t tmp;

	tmp = nr%16;
	if(tmp>9){
		data[1]= tmp+55;
	}else{
		data[1] = tmp + '0';
	}

	nr /= 16;
	if(nr>9){
		data[0]= nr+55;
	}else{
		data[0] = nr + '0';
	}
	LPUART1_SendBytes(data);
}
void LPUART1_SendDec( uint16_t nr ){
	uint8_t data[6]={'0',0,0,0,0,0};
	uint8_t index=0;
	if( nr==0 ){
		LPUART1_SendString("0");
	}else{
		while( nr!=0 ){
			data[index++]=nr%10+'0';
			nr/=10;
		}
		LPUART1_SendArrayReversed(data,index);
	}
}
/**
 * Brief Send Carriage return + linefeed
 */
void LPUART1_SendCRLF(){
	uint8_t data[2]={'\r','\n'};
	LPUART1_SendArray(data,2);
}

void LPUART1_Check_Circular(void){
    if( (LPUART1->ISR & USART_ISR_TXE) == USART_ISR_TXE){// Bit set, so TDR is empty
    	if( outStart != outEnd ){
    		LPUART1->TDR = *outStart++; // This also clears the complete flag
    		if (outStart == outTail) { // So never write on the tail!
    			outStart = outHead;    // End reached, back to head
    		}
    	}
    }
}
void LPUART1_Transfer_Buffer( void ){
    uint8_t rec[16];
    uint8_t a;

    if( inputStart == inputEnd) // This shouldn't happen, but somehow does
    	return;

    /* Clean the buffer that will be used */
    for( a=0;a<16;a++)
        rec[a]=0x00;

    a = 0x00;
    /* Move the data from the circular buffer to the local one */
    inputTemp = inputEnd;                             // Remember the current endstop for the circular buffer,because other might use it in ISR
    if (inputStart > inputTemp) {                     // If the 'end' is closer to the beginning of the buffer than the 'start'
        do{
            rec[a++] = *inputStart++;
        }while( inputStart != inputTail && a < CIRCULAR+5);  // Repeat till the 'start' has reached the end of the buffer
        inputStart = inputHead;                       // Because the end was reached, continue from the start of the buffer
    }
    do{
        rec[a++] = *inputStart++;
    }while( inputStart < inputTemp && a < CIRCULAR+5);      // Repeat the 'start' is the same as the 'end'

    executeCommand( rec );
}
void LPUART1_IRQHandler(void){
    uint8_t recChar = 0;
    uint8_t ok = 0x00;

    // Check if interrupt is due to transmit complete
    if((LPUART1->ISR & USART_ISR_TC) == USART_ISR_TC){
    	 if( outStart != outEnd ){ // meaning still data to send
    		 LPUART1->TDR = *outStart++; // This also clears the complete flag
    		 if (outStart == outTail) { // So never write on the tail!
				 outStart = outHead;    // End reached, back to head
			 }
    	 }else{
    		 LPUART1->ICR = USART_ICR_TCCF; /* Clear transfer complete flag */
    	 }
    	 ok=0x01;
    }
    if((LPUART1->ISR & USART_ISR_RXNE) == USART_ISR_RXNE){ // ISR for received data
        recChar = (uint8_t)(LPUART1->RDR); /* Receive data, clear flag */
        if( recChar >= 0x20 && recChar <= 0x7F){
            *inputEnd++ = recChar;
            if (inputEnd == inputTail) { // So never write on the tail!
                inputEnd = inputHead;
            }
        }else if(recChar==0x00){
        	ok=0x00;
        }else if( recChar == '\n'){
        }else if( recChar == '\r'){
        	cmdReady ++;
        }
        // Can't echo inside the ISR! because the processor doesn't process fast enough?
        //LPUART1_SendByte(recChar); // This gives issues
        ok=0x02;
    }
    if( ok==0x00 ){ // Meaning ISR was for unknown reason
        error = ERROR_USART_TRANSMIT; /* Report an error */
        NVIC_DisableIRQ(LPUART1_IRQn); /* Disable USART2_IRQn */
    }
}
uint8_t LPUART1_hasCmd(){
	if( cmdReady != 0){
		cmdReady=0;
		return 1;
	}
	return 0;
}
