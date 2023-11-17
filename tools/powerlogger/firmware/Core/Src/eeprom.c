/* Includes ------------------------------------------------------------------*/
#include "../inc/eeprom.h"
/* Private variables ---------------------------------------------------------*/
uint32_t test_to_be_performed_twice = 1; //this variable is set to 2 if the first address of the page to erase is yet erased

/* Methods -------------------------------------------------------------------*/
/**
  * Brief   This function unlocks the data EEPROM and the FLASH_PECR.
  *         The data EEPROM will be ready to be erased or programmed
  *         but the program memory will be still locked till PRGLOCK is set.
  *         It first checks no flash operation is on going,
  *         then unlocks PELOCK if it is locked.
  * Param   None
  * Retval  None
  */
__INLINE void UnlockPELOCK(void)
{
  /* (1) Wait till no operation is on going */
  /* (2) Check if the PELOCK is unlocked */
  /* (3) Perform unlock sequence */
  while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (1) */
  {
    /* For robust implementation, add here time-out management */
  }
  if ((FLASH->PECR & FLASH_PECR_PELOCK) != 0) /* (2) */
  {
    FLASH->PEKEYR = FLASH_PEKEY1; /* (3) */
    FLASH->PEKEYR = FLASH_PEKEY2;
  }
}
/**
  * Brief   This function locks the NVM.
  *         It first checks no flash operation is on going,
  *         then locks the flash.
  * Param   None
  * Retval  None
  */
__INLINE void LockNVM(void)
{
  /* (1) Wait till no operation is on going */
  /* (2) Locks the NVM by setting PELOCK in PECR */
  while ((FLASH->SR & FLASH_SR_BSY) != 0) /* (1) */
  {
    /* For robust implementation, add here time-out management */
  }
  FLASH->PECR |= FLASH_PECR_PELOCK; /* (2) */
}

/**
  * Brief   This function erases a word of data EEPROM.
  *         The ERASE bit and DATA bit are set in PECR at the beginning
  *         and reset at the endof the function. In case of successive erase,
  *         these two operations could be performed outside the function.
  *         The flash interrupts must have been enabled prior to call
  *         this function.
  * Param   addr is the 32-bt word address to erase
  * Retval  None
  */
__INLINE void EepromErase(uint32_t addr)
{
  /* (1) Set the ERASE and DATA bits in the FLASH_PECR register
         to enable page erasing */
  /* (2) Write a 32-bit word value at the desired address
         to start the erase sequence */
  /* (3) Enter in wait for interrupt. The EOP check is done in the Flash ISR */
  /* (6) Reset the ERASE and DATA bits in the FLASH_PECR register
         to disable the page erase */
  FLASH->PECR |= FLASH_PECR_ERASE | FLASH_PECR_DATA; /* (1) */
  *(__IO uint32_t *)addr = (uint32_t)0; /* (2) */
  __WFI(); /* (3) */
  FLASH->PECR &= ~(FLASH_PECR_ERASE | FLASH_PECR_DATA); /* (4) */
}
__INLINE void EepromWrite(uint32_t addr, uint32_t data)
{
  /* (1) Set the DATA bits in the FLASH_PECR register
         to enable page erasing */
  /* (2) Write a 32-bit word value at the desired address
         to start the erase sequence */
  /* (3) Enter in wait for interrupt. The EOP check is done in the Flash ISR */
  /* (6) Reset the DATA bits in the FLASH_PECR register
         to disable the page erase */
  FLASH->PECR |= FLASH_PECR_DATA; /* (1) */
  *(__IO uint32_t *)addr = data; /* (2) */
  __WFI(); /* (3) */
  FLASH->PECR &= ~( FLASH_PECR_DATA); /* (4) */
}
/******************************************************************************/
/*                 STM32L0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l0xx.s).                                               */
/******************************************************************************/
/**
  * Brief   This function handles FLASH interrupt request.
  *         It handles any kind of error even if not used in this example.
  * Param   None
  * Retval  None
  */
void FLASH_IRQHandler(void)
{
  if ((FLASH->SR & FLASH_SR_EOP) != 0)  /* (3) */
  {
    FLASH->SR = FLASH_SR_EOP; /* (4) */
  }
  /* Manage the error cases */
  else if ((FLASH->SR & FLASH_SR_FWWERR) != 0) /* Check Fetch while Write error */
  {
    error |= ERROR_FETCH_DURING_PROG; /* Report the error to the main progran */
    FLASH->SR = FLASH_SR_FWWERR; /* Clear the flag by software by writing it at 1*/
  }
  else if ((FLASH->SR & FLASH_SR_NOTZEROERR) != 0) /* Check Not Zero error */
  /* This error occurs if the address content was not cleared/erased
     before the programming */
  {
    error |= ERROR_NOT_ZERO; /* Report the error to the main progran */
    FLASH->SR = FLASH_SR_NOTZEROERR; /* Clear the flag by software by writing it at 1*/
  }
  else if ((FLASH->SR & FLASH_SR_SIZERR) != 0) /* Check Size error */
  {
    error |= ERROR_SIZE; /* Report the error to the main progran */
    FLASH->SR = FLASH_SR_SIZERR; /* Clear the flag by software by writing it at 1*/
  }
  else if ((FLASH->SR & FLASH_SR_WRPERR) != 0) /* Check Write protection error */
  {
    error |= ERROR_WRITE_PROTECTION; /* Report the error to the main progran */
    FLASH->SR = FLASH_SR_WRPERR; /* Clear the flag by software by writing it at 1*/
  }
  else if ((FLASH->SR & FLASH_SR_RDERR) != 0) /* Check Read-out protection error */
  {
    error |= ERROR_READOUT_PROTECTION; /* Report the error to the main progran */
    FLASH->SR = FLASH_SR_RDERR; /* Clear the flag by software by writing it at 1*/
  }
  else if ((FLASH->SR & FLASH_SR_OPTVERR) != 0) /* Check Option valid error */
  {
    error |= ERROR_OPTION_NOT_VALID; /* Report the error to the main progran */
    FLASH->SR = FLASH_SR_OPTVERR; /* Clear the flag by software by writing it at 1*/
  }
  else if ((FLASH->SR & FLASH_SR_PGAERR) != 0) /* Check alignment error */
  {
    error |= ERROR_ALIGNMENT; /* Report the error to the main progran */
    FLASH->SR = FLASH_SR_PGAERR; /* Clear the flag by software by writing it at 1*/
  }
  else
  {
    error |= ERROR_UNKNOWN;
  }
}
