/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "main.h"

/* Shared Variables  ---------------------------------------------------------*/
__IO uint32_t Tick;
__IO uint16_t error;

/* Private variables ---------------------------------------------------------*/
uint8_t delay;
uint16_t i2cError=0;
uint8_t pacAddress=0x00;
uint16_t heartBeats=0;
uint16_t recBuffer[12];
uint8_t check=0;
uint8_t pacState=0;

static settings_t _settings_in_ram;
settings_t *GLOBAL_settings_ptr = &_settings_in_ram;

static voltcur lastVoltCur;
voltcur *GLOBAL_voltcur_ptr = &lastVoltCur;

int main(void){
    /*!< At this stage the microcontroller clock setting is already configured,
         this is done through SystemInit() function which is called from startup
         file (startup_stm32l0xx.s) before to branch to application main.
         To reconfigure the default setting of SystemInit() function, refer to
         system_stm32l0xx.c file
    */
	
    SysTick_Config(2000); /* 1ms config */
    SystemClock_Config();
    SysTick_Config(16000); /* 1ms config */

	init();

	// To test RAM '.data' section initialization:
	//static int dont_panic = 42;
	while (1) {
		if( error != 0x00){

		}
		if( check==5 && pacState==PAC_FOUND){
			i2cError=I2C1_SendSingleByte(pacAddress,PAC1954_REFRESH_V);
			if( i2cError != 1){
				LPUART1_SendString("I2C:Error->");
				LPUART1_SendHex(i2cError);
				LPUART1_SendString("\r\n");
				check=0;
			}else{
				pacState=PAC_REFRESHED;
			}
		}
		if( check>=10 && pacState==PAC_REFRESHED){
			check=0;
			// Read Vbus, Vsense
			i2cError=I2C1_Read16bitData( pacAddress, PAC1954_VBUSN_REG, 8,recBuffer);
			if( i2cError == 1){
				lastVoltCur.out3_voltage = recBuffer[0];
				lastVoltCur.out4_voltage = recBuffer[1];
				lastVoltCur.out1_voltage = recBuffer[2];
				lastVoltCur.out2_voltage = recBuffer[3];

				lastVoltCur.out3_current = recBuffer[4];
				lastVoltCur.out4_current = recBuffer[5];
				lastVoltCur.out1_current = recBuffer[6];
				lastVoltCur.out2_current = recBuffer[7];
				pacState=PAC_FOUND;
				sendData();
			}else if(i2cError==0x11){
				LPUART1_SendString("I2C:TimeOut\r\n");
			}else if(i2cError==0x12){
				LPUART1_SendString("I2C:TimeOut2\r\n");
			}else if(i2cError==0x13){
				LPUART1_SendString("I2C:TimeOut3\r\n");
			}else if(i2cError==0x14){
				LPUART1_SendString("I2C:TimeOut4\r\n");
			}else{
				i2cError++;
			}
			// Read Vpower
			i2cError=I2C1_Read16bitData( pacAddress, PAC1954_VPOWERN_REG, 8,recBuffer);
			if( i2cError == 1){

				pacState=PAC_FOUND;
				sendData();
			}else if(i2cError==0x11){
				LPUART1_SendString("I2C:TimeOut\r\n");
			}else if(i2cError==0x12){
				LPUART1_SendString("I2C:TimeOut2\r\n");
			}else if(i2cError==0x13){
				LPUART1_SendString("I2C:TimeOut3\r\n");
			}else if(i2cError==0x14){
				LPUART1_SendString("I2C:TimeOut4\r\n");
			}else{
				i2cError++;
			}
		}
		if( LPUART1_hasCmd() ){
			LPUART1_Transfer_Buffer();
		}
	}
}
 
/**
  * Brief   This function configures the system clock @16MHz and voltage scale 1
  *         assuming the registers have their reset value before the call.
  *         POWER SCALE   = RANGE 1
  *         SYSTEM CLOCK  = PLL MUL8 DIV2
  *         PLL SOURCE    = HSI/4
  *         FLASH LATENCY = 0
  * Param   None
  * Retval  None
  */
__INLINE void SystemClock_Config(void){

  uint32_t tickstart;
  
  /* (1) Enable power interface clock */
  RCC->APB1ENR |= (RCC_APB1ENR_PWREN);
  
  /* (2) Select voltage scale 1 (1.65V - 1.95V) i.e. (01)  for VOS bits in PWR_CR */
  PWR->CR = (PWR->CR & ~(PWR_CR_VOS)) | PWR_CR_VOS_0;
  
  /* (3) Enable HSI divided by 4 in RCC->CR */
  RCC->CR |= RCC_CR_HSION | RCC_CR_HSIDIVEN;
  
  tickstart = Tick;
    
  /* (4) Wait for HSI ready flag and HSIDIV flag */
   while ((RCC->CR & (RCC_CR_HSIRDY |RCC_CR_HSIDIVF)) != (RCC_CR_HSIRDY |RCC_CR_HSIDIVF)){
    if ((Tick - tickstart ) > HSI_TIMEOUT_VALUE){
      error = ERROR_HSI_TIMEOUT; /* Report an error */
      return;
    }      
  } 
  /* (5) Set PLL on HSI, multiply by 8 and divided by 2 */
  RCC->CFGR |= RCC_CFGR_PLLSRC_HSI | RCC_CFGR_PLLMUL8 | RCC_CFGR_PLLDIV2;
  
  /* (6) Enable the PLL in RCC_CR register */
  RCC->CR |= RCC_CR_PLLON; 
  
  tickstart = Tick;
  
  /* (7) Wait for PLL ready flag */
  while ((RCC->CR & RCC_CR_PLLRDY)  == 0){
    if ((Tick - tickstart ) > PLL_TIMEOUT_VALUE){
      error = ERROR_PLL_TIMEOUT; /* Report an error */
      return;
    }      
  }
  
  /* (8) Select PLL as system clock */
  RCC->CFGR |= RCC_CFGR_SW_PLL; /* (8) */
  tickstart = Tick;
  
  /* (9) Wait for clock switched on PLL */
  while ((RCC->CFGR & RCC_CFGR_SWS_PLL)  == 0){
    if ((Tick - tickstart ) > CLOCKSWITCH_TIMEOUT_VALUE){
      error = ERROR_CLKSWITCH_TIMEOUT; /* Report an error */
      return;
    }      
  }
}

/* ********************************************************************************************************** */
/* *********************************** A P P L I C A T I O N  C O D E *************************************** */
/* ********************************************************************************************************** */
void init(void){
	uint8_t temp=0;

	I2C1_Configure_Master();
	LPUART1_Configure();

    configure_IO();
    HEART_ON;

    LPUART1_SendString("I>Booting...\r\n");
    delay = 10; // wait a bit
    while(delay!=0); // 10ms delay

    if( sizeof(settings_t) % 4 ==0){ // Needs to be a multiple of 4 because e²prom has 32bit pages
    	// Get the settings from e²prom
    	memcpy(GLOBAL_settings_ptr, (uint32_t*)DATA_E2_ADDR, sizeof(settings_t));

    	//if( _settings_in_ram.spare==0x00){ // nothing in there yet
    		resetSettings();
    	//}
    }
    pacAddress = _settings_in_ram.pacAddress;
    I2C1_FindDevices();

    // Check if I2C hardware is found
    LPUART1_SendString("I>PAC1954:");
    temp=I2C1_PokeDevice(pacAddress);
    if( temp==1){
    	LPUART1_SendString("OK\r\n");
    	pacState=PAC_FOUND;
    }else if( temp == ERROR_I2C_TIMEOUT ){
    	LPUART1_SendString("TimeOut\r\n");
    }else{
    	LPUART1_SendString("NOK\r\n");
    }

	delay = 10; // wait a bit
	while(delay!=0); // 10ms delay
	LPUART1_SendString("I>Init done.\r\n");
}
void clock_test(void){
	GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE0|GPIO_MODER_MODE1))
	                  | (GPIO_MODER_MODE0_0|GPIO_MODER_MODE1_0);
}
void configure_IO(void){
  
  /* Enable the peripheral clock of GPIOA and GPIOB */
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN;

  /* INPUTS */
  /* 
    A6 = U2 alert1
    A7 = U2 alert2
  */  
  GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE6 | GPIO_MODER_MODE7));

  /* OUTPUTS */
  /* Select output mode (01) on GPIOA pin 5,6,7,11 and 12 */
  /*
    PB5 = Heart beat led
  */
  GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE5))| (GPIO_MODER_MODE5_0);

}
/* *** */

/* ******************************************** I 2 C ******************************************************** */


void I2C1_FindDevices(){

	uint8_t addr;
	uint8_t tmp;
	LPUART1_SendString("I2C>");
	/* Clear the address and bytes to send spot, then fill them in*/
	for( addr=0x01;addr<127;addr++){
		if( addr%16==0)
			LPUART1_SendString("\r\nI2C>");
		//
		switch( I2C1_PokeDevice(addr) ){
			case 0x00:
				LPUART1_SendByte('.'); // Show some progress?
				break;
			case 0x01:
				LPUART1_SendHex(addr);
				tmp++;
				break;
			case ERROR_I2C_TIMEOUT:
				LPUART1_SendString(":Time Out");
				addr=0x8F;//Stop searching
				break;
		}

		delay = 50; // wait a bit
		while(delay!=0); // 50ms delay
	}
	LPUART1_SendString("\r\nI2C>Found ");
	LPUART1_SendHex(tmp);
	LPUART1_SendString(" devices\r\n");
}
uint8_t I2C1_FindPAC1954(){

	uint8_t addr;
	LPUART1_SendString("I>PAC?");
	/* Clear the address and bytes to send spot, then fill them in*/
	for( addr=0x10;addr<0x20;addr++){
		switch( I2C1_PokeDevice(addr) ){
			case 0x00:
				LPUART1_SendByte('.'); // Show some progress?
				break;
			case 0x01:
				LPUART1_SendHex(addr);
				LPUART1_SendString("\r\n");
				return addr;
			case ERROR_I2C_TIMEOUT:
				LPUART1_SendString("TIMEOUT\r\n");//Stop searching
				return 0xFF;
		}
		delay = 50; // wait a bit
		while(delay!=0); // 50ms delay
	}
	LPUART1_SendString("FAILED\r\n");
	return 0xFF;
}
/* ***************************************** U S A R T ******************************************************* */
void executeCommand( uint8_t * cmd ){
    uint8_t ok = 0xFF;

    switch( cmd[0]){
      case 'E': // E²PROM
    	  if( cmd[1]=='C'){ // Reset the e²prom
    		  resetSettings();
    	  }else if( cmd[1]=='R'){ // Read from the e²prom
    		  memcpy(GLOBAL_settings_ptr, (uint32_t*)DATA_E2_ADDR, sizeof(settings_t));
    	  }else if( cmd[1]=='W'){ // Write to the e²prom
    		  settings_write();
    	  }
      case 's': // Read data
    	  /*if( I2C1_SendSingleByte(pacAddress,PAC1954_REFRESH_N==0x01)){
    		  ok=0xFF;
    	  }*/
    	  break;
      case 'p': // Send last voltage and sense reading for all channels
    	  sendData();
    	  break;
    }
    LPUART1_SendBytes(cmd);
    if( ok != 0xFF ){
    	LPUART1_SendString(":NOK\r\n");
    }else{
    	LPUART1_SendString(":OK\r\n");
    }
}
void sendData(){
	  LPUART1_SendString("VC:");
	  LPUART1_SendHex(pacAddress);
	  LPUART1_SendString(";");
	  LPUART1_SendDec(lastVoltCur.out1_voltage);
	  LPUART1_SendString(";");
	  LPUART1_SendDec(lastVoltCur.out1_current);
	  LPUART1_SendString(";");
	  LPUART1_SendDec(lastVoltCur.out2_voltage);
	  LPUART1_SendString(";");
	  LPUART1_SendDec(lastVoltCur.out2_current);
	  LPUART1_SendString(";");
	  LPUART1_SendDec(lastVoltCur.out3_voltage);
	  LPUART1_SendString(";");
	  LPUART1_SendDec(lastVoltCur.out3_current);
	  LPUART1_SendString(";");
	  LPUART1_SendDec(lastVoltCur.out4_voltage);
	  LPUART1_SendString(";");
	  LPUART1_SendDec(lastVoltCur.out4_current);
	  LPUART1_SendString("\r\n");
}
/* ************************************* E E P R O M ************************************************************** */
void resetSettings(){
	// Find pac
	_settings_in_ram.pacAddress = I2C1_FindPAC1954();
	_settings_in_ram.out1_lowvlim=10000;
	_settings_in_ram.out1_highvlim=16000;
	_settings_in_ram.out1_curlim=2000;
	_settings_in_ram.out1_defState=0;
	_settings_in_ram.out1_spare=0xAA;

	_settings_in_ram.out2_lowvlim=10000;
	_settings_in_ram.out2_highvlim=16000;
	_settings_in_ram.out2_curlim=2000;
	_settings_in_ram.out2_defState=0;
	_settings_in_ram.out2_spare=0xAA;

	_settings_in_ram.out3_lowvlim=10000;
	_settings_in_ram.out3_highvlim=16000;
	_settings_in_ram.out3_curlim=2000;
	_settings_in_ram.out3_defState=0;
	_settings_in_ram.out3_spare=0xAA;

	_settings_in_ram.out4_lowvlim=10000;
	_settings_in_ram.out4_highvlim=16000;
	_settings_in_ram.out4_curlim=2000;
	_settings_in_ram.out4_defState=0;
	_settings_in_ram.out4_spare=0xAA;

	_settings_in_ram.overcurrentTime=10;
	_settings_in_ram.spare=1;

	settings_write();
}
uint32_t settings_write(void){

	UnlockPELOCK();

    uint32_t *src = (uint32_t*)GLOBAL_settings_ptr;
    uint32_t *dst = (uint32_t*)DATA_E2_ADDR;

    //------------------ bug of the day ------------------
    /* Flash option byte user validity error flag caused write to EEPROM operation to fail!
     * The problem did not happen when J-Link was connected. Probably J-Link starts execution
     * right from the application address, skipping the bootloader. The bootloader
     * may have had set the flag.
     */
//    FLASH_ClearFlag(FLASH_FLAG_OPTVERR);
    //------------------ bug of the day ------------------

    //write settings word (uint32_t) at a time
    FLASH->PECR |= FLASH_PECR_DATA; /* (1) */
    for (uint32_t i = 0; i < sizeof(settings_t)/sizeof(uint32_t); i++){
        if (*dst != *src){ //write only if value has been modified
        	*(__IO uint32_t *)dst = *src;
        	__WFI(); /* (3) */
        }
        src++;
        dst++;
    }
    FLASH->PECR &= ~(FLASH_PECR_DATA); /* (4) */

    LockNVM();
    return 0;
}
/******************************************************************************/
/*            Cortex-M0 Plus Processor Exceptions Handlers                    */
/******************************************************************************/

/* This function handles SysTick Handler.  */
void SysTick_Handler(void){
    Tick++;

	if( Tick % 100 == 0){
		if( pacState >= PAC_FOUND){
			check++;
		}
	}

    if( Tick % 100 == 1 ){
		/*if( I2C1_SendSingleByte(pacAddress,PAC1954_REFRESH_N)==0){
			heartBeats=0;
		}*/
	}
    if( delay != 0 ){
    	delay--;
    	if( delay == 40 ){
    		delay --;
    	}
    }

    heartBeats++;
    if( heartBeats == 0x500 ){
    	HEART_ON;
    	//LPUART1_SendString(".");
    }else if( heartBeats == 0x600){
    	HEART_OFF;
    	heartBeats=0;
    }
}
