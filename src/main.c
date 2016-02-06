/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO(R)
**                STMicroelectronics STM32F4xx Standard Peripherals Library
**
**  Distribution: The file is distributed "as is", without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

/* Includes */
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "serial-debug.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "leds-and-button.h"
#include "stm32f4xx_it.h"
#include "spi.h"

/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
/**
 * Rejestr Systick val jest rejetrem 24 bitowym oznacza to ze
 * mozna zpisac do niego wartoc masymala 2^24-1 =  16 777 215
 */
/*
uint8_t SysTickConf(uint32_t prescaler){

	if(prescaler > 16777215) return 1; // przekroczono maksymalna wartosc rejestru systick->val;
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  // moze ale nie musi byc nadawany priotytet
	SysTick->VAL =0;
	SysTick->LOAD = prescaler - 1;
	SysTick->CTRL =  SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;
	return 0;
}
*/



int main(void)
{
	spiInit();
	SystemCoreClockUpdate();
	spiInit();
	serialDebugInit();
	userButtonInit();
	userLedsInit();
	printf("STM32F429 TFT - program v1.0 - rozpoczecie programu\n");
	//ledRedOn();
	//ledGreenOff();

  /**
  *  IMPORTANT NOTE!
  *  The symbol VECT_TAB_SRAM
  *
  *   needs to be defined when building the project
  *  if code has been located to RAM and interrupts are used. 
  *  Otherwise the interrupt table located in flash will be used.
  *  See also the <system_*.c> file and how the SystemInit() function updates 
  *  SCB->VTOR register.  
  *  E.g.  SCB->VTOR = 0x20000000;  
  */



  //STM_EVAL_LEDInit(LED3);
  //STM_EVAL_LEDInit(LED4);

 // STM_EVAL_LEDOn(LED3);
 // STM_EVAL_LEDOn(LED4);

  /* Infinite loop */
  while (1)
  {
	  printf("1\n");
	  _delay_ms(200);
	  printf("2\n");
	  _delay_ms(200);
	//  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	//  USART1->DR = 'p';
	//printf(" p \n");
  }
}

/*
 * Callback used by stm324xg_eval_i2c_ee.c.
 * Refer to stm324xg_eval_i2c_ee.h for more info.
 */
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  /* TODO, implement your code here */
  while (1)
  {
  }
}
