/*
 * leds-and-button.c
 *
 *  Created on: Jan 17, 2016
 *      Author: Macikow
 */
#include "leds-and-button.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include <stdio.h>

#define GREEN_LED (1<<13)
#define RED_LED 	(1<<14)
#define USAER_BUTTON


/** PORT - G
 * LED4 - LED GREEN - PG14
 * LED5 - LED RED   - PG13
 */
void userLedsInit(void){

	GPIO_InitTypeDef GPIO_InitStrucutre;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_InitStrucutre.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStrucutre.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStrucutre.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStrucutre.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOG,&GPIO_InitStrucutre);
}
void ledGreenOn(){
	GPIOG->BSRRL |= GREEN_LED;
}
void ledRedOn(){
	GPIOG->BSRRL |= RED_LED;
}
void ledGreenOff(){
	GPIOG->BSRRH &= ~GREEN_LED;
}
void ledRedOff(){
	GPIOG->BSRRH &= ~RED_LED;
}
void ledGreenToggle(){
	GPIOG->ODR ^= GREEN_LED;
}
void ledRedToggle(){
	GPIOG->ODR ^= RED_LED;
}

/**
 * PA0
 */
void userButtonInit(void){
	 GPIO_InitTypeDef GPIOA_InitStrucutre;
	 EXTI_InitTypeDef EXTI_InitStruct;
	 NVIC_InitTypeDef NVIC_InitStruct;

	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	 GPIOA_InitStrucutre.GPIO_Pin = GPIO_Pin_0;
	 GPIOA_InitStrucutre.GPIO_OType = GPIO_OType_PP;
	 GPIOA_InitStrucutre.GPIO_Speed = GPIO_Speed_100MHz;
	 GPIOA_InitStrucutre.GPIO_Mode = GPIO_Mode_IN;
	 GPIO_Init(GPIOA,&GPIOA_InitStrucutre);

	 SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	  EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	    /* Enable interrupt */
	  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	  /* Interrupt mode */
	  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	    /* Triggers on rising and falling edge */
	  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	    /* Add to EXTI */
	  EXTI_Init(&EXTI_InitStruct);

	  /* Add IRQ vector to NVIC */
	    /* PD0 is connected to EXTI_Line0, which has EXTI0_IRQn vector */
	  NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	    /* Set priority */
	  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	    /* Set sub priority */
	  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	    /* Enable interrupt */
	  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	    /* Add to NVIC */
	  NVIC_Init(&NVIC_InitStruct);
}

void EXTI0_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        /* Do your stuff when PB12 is changed */
    	printf("->przerwanie pochodzce od przycisku urzytkownika\n");

        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}



