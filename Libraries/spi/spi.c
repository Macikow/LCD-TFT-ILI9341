/*
 * spi.c
 *
 *  Created on: Jan 24, 2016
 *      Author: Macikow
 */


#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_gpio.h"


/**SPI5 Init function
 * AF5
 * MOSI -> PF9
 * MISO -> PF8
 * SCK -> PF7
 * CS -> PC2
 */

#define MISO_PORT	 	GPIOF
#define MOSI_PORT		GPIOF
#define CS_PORT			GPIOC
#define SCK_PORT		GPIOF
#define RESET			GPIOD

#define SPI_MOSI_PIN	9
#define SPI_MISO_PIN	8
#define SPI_SCK_PIN		7
#define CS_PIN			GPIO_Pin_2
#define RESET_PIN 		GPIO_Pin_12

#define MODER_ALT_FUNC 2
#define SPEED_100MHZ 3
#define NO_PULL	0
#define SPI_ALT_FUN 5


#define MSTR_Msk 2
#define CPOL_Msk 1
#define CPHA_Msk 0
#define SPE_Msk 	6

void spiInit(){

	//set clocking
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
	//RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI5, ENABLE);

	//SPI PIN init

	// MODER = alternative function 10 <<
	// 01 GEneral output mode
	MISO_PORT->MODER |= ( MODER_ALT_FUNC << (2*SPI_MISO_PIN) );
	MOSI_PORT->MODER |= ( MODER_ALT_FUNC << (2*SPI_MOSI_PIN));
	SCK_PORT->MODER  |= ( MODER_ALT_FUNC << (2*SPI_SCK_PIN));
	CS_PORT->MODER 	 |= ( 1 << CS_PIN);
	// set speed
	MISO_PORT->OSPEEDR 	|= 	( SPEED_100MHZ << (2*SPI_MISO_PIN) );
	MOSI_PORT->OSPEEDR	|= 	( SPEED_100MHZ << (2*SPI_MOSI_PIN) );
	SCK_PORT->OSPEEDR	|= 	( SPEED_100MHZ << (2*SPI_SCK_PIN) );
	CS_PORT->OSPEEDR	|=  	( SPEED_100MHZ << CS_PIN );
	// set speed
	MISO_PORT->OTYPER |= 	( SPEED_100MHZ << 9 );
	MOSI_PORT->OTYPER |= 	( SPEED_100MHZ << 8 );
	SCK_PORT->OTYPER  |= 	( SPEED_100MHZ << 7 );
	CS_PORT->OTYPER   |= 	( SPEED_100MHZ << 2 );
	// reset pull
	MISO_PORT->PUPDR |= 	( NO_PULL << 9 );
	MOSI_PORT->PUPDR |= 	( NO_PULL << 8 );
	SCK_PORT->PUPDR  |= 	( NO_PULL<< 7 );
	CS_PORT->PUPDR   |= 	( NO_PULL<< 2 );

	SCK_PORT->AFR[0] |= ( SPI_ALT_FUN << 28 );
	MOSI_PORT->AFR[1] |= ( SPI_ALT_FUN << 4 );
	MISO_PORT->AFR[1] |= ( SPI_ALT_FUN << 0);

	SPI_InitTypeDef SPI_InitStructure;

	/* SPI Config */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI5, &SPI_InitStructure);

	while(SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE) == RESET) ;








}
