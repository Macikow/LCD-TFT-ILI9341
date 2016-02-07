/*
 * ILI9341.c
 *
 *  Created on: Feb 7, 2016
 *      Author: Macikow
 */
#include "STM32f4xx_gpio.h"
#include "STM32f4xx_rcc.h"
#include "STM32f4xx_spi.h"
#include "STM32f4xx_it.h"
#include "spi.h"
#include "ILI9341.h"



#define INTERFACE_PORT GPIOD

#define CS_PIN			2
#define RESET_PIN 		12

#define CD_PIN			13

#define SPEED_100_MHZ 3
#define PUSH_PULL 0
#define OUTPUT 1
#define PULL_DOWN 2
#define ILI9341_SPI SPI5

// SPI
// RESET - PD12
// D/C - PD13
//COMMAND - LOW
//DATA -HIGH
void ILI9341_SendData(uint8_t data);
void ILI9341_SendCommand(uint8_t data);
/**
 *
 * The 3-lines serial datapacket contains a data/command select bit (D/CX) and a transmission byte. If the D/CX bit is “low”, the
transmission byte is interpreted as a command byte. If the D/CX bit is “high”, the transmission byte is stored as
the display data RAM (Memory write command), or command register as parameter.
 */
#define ILI9341_CS_LOW  	(GPIO_ResetBits(GPIOC, 2))	//active
#define ILI9341_CS_HIGH 	(GPIO_SetBits(GPIOC, 2))
#define ILI9341_RST_LOW		(GPIO_ResetBits(GPIOD, 12))
#define ILI9341_RST_HIGH	(GPIO_SetBits(GPIOD, 12))
#define ILI9341_CD_LOW		(GPIO_ResetBits(GPIOD, 13))
#define ILI9341_CD_HIGH		(GPIO_SetBits(GPIOC, 13))

/* Private defines */
#define ILI9341_RESET				0x01
#define ILI9341_SLEEP_OUT			0x11
#define ILI9341_GAMMA				0x26
#define ILI9341_DISPLAY_OFF			0x28
#define ILI9341_DISPLAY_ON			0x29
#define ILI9341_COLUMN_ADDR			0x2A
#define ILI9341_PAGE_ADDR			0x2B
#define ILI9341_GRAM				0x2C
#define ILI9341_MAC					0x36
#define ILI9341_PIXEL_FORMAT		0x3A
#define ILI9341_WDB					0x51
#define ILI9341_WCD					0x53
#define ILI9341_RGB_INTERFACE		0xB0
#define ILI9341_FRC					0xB1
#define ILI9341_BPC					0xB5
#define ILI9341_DFC					0xB6
#define ILI9341_POWER1				0xC0
#define ILI9341_POWER2				0xC1
#define ILI9341_VCOM1				0xC5
#define ILI9341_VCOM2				0xC7
#define ILI9341_POWERA				0xCB
#define ILI9341_POWERB				0xCF
#define ILI9341_PGAMMA				0xE0
#define ILI9341_NGAMMA				0xE1
#define ILI9341_DTCA				0xE8
#define ILI9341_DTCB				0xEA
#define ILI9341_POWER_SEQ			0xED
#define ILI9341_3GAMMA_EN			0xF2
#define ILI9341_INTERFACE			0xF6
#define ILI9341_PRC					0xF7

#define ILI9341_WIDTH        240
#define ILI9341_HEIGHT       320
#define ILI9341_PIXEL        76800

/* Colors */
#define ILI9341_COLOR_WHITE			0xFFFF
#define ILI9341_COLOR_BLACK			0x0000
#define ILI9341_COLOR_RED       0xF800
#define ILI9341_COLOR_GREEN			0x07E0
#define ILI9341_COLOR_GREEN2		0xB723
#define ILI9341_COLOR_BLUE			0x001F
#define ILI9341_COLOR_BLUE2			0x051D
#define ILI9341_COLOR_YELLOW		0xFFE0
#define ILI9341_COLOR_ORANGE		0xFBE4
#define ILI9341_COLOR_CYAN			0x07FF
#define ILI9341_COLOR_MAGENTA		0xA254
#define ILI9341_COLOR_GRAY			0x7BEF
#define ILI9341_COLOR_BROWN			0xBBCA

/* Transparent background, only for strings and chars */
#define ILI9341_TRANSPARENT			0x80000000

uint16_t ILI9341_x;
uint16_t ILI9341_y;
uint8_t ILI9341_INT_CalledFromPuts = 0;


void InterfacePinConfig(void){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	INTERFACE_PORT->MODER |= (OUTPUT << (2*RESET_PIN)) ;
	INTERFACE_PORT->OSPEEDR |= (SPEED_100_MHZ <<  (2*RESET_PIN));
	INTERFACE_PORT->OTYPER |= (PUSH_PULL<< RESET_PIN);
	INTERFACE_PORT->PUPDR |= (PULL_DOWN <<  (2*RESET_PIN));

	INTERFACE_PORT->MODER |= (OUTPUT << (2 * CD_PIN)) ;
	INTERFACE_PORT->OSPEEDR |= (SPEED_100_MHZ << (2 * CD_PIN));
	INTERFACE_PORT->OTYPER |= (PUSH_PULL<< CD_PIN);
	INTERFACE_PORT->PUPDR |= (PULL_DOWN << (2*CD_PIN));
}

void ILI9341Init(){
	InterfacePinConfig();
	spiInit();
	ILI9341_RST_LOW;
	_delay_ms(20);
	ILI9341_RST_HIGH;
	ILI9341_SendCommand(ILI9341_RESET);
	_delay_ms(120);
	ILI9341_SendCommand(ILI9341_POWERA); //
	ILI9341_SendData(0x39);
	ILI9341_SendData(0x2C);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x34);
	ILI9341_SendData(0x02);
	ILI9341_SendCommand(ILI9341_POWERB);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0xC1);
	ILI9341_SendData(0x30);
	ILI9341_SendCommand(ILI9341_DTCA);
	ILI9341_SendData(0x85);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x78);
	ILI9341_SendCommand(ILI9341_DTCB);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendCommand(ILI9341_POWER_SEQ);
	ILI9341_SendData(0x64);
	ILI9341_SendData(0x03);
	ILI9341_SendData(0x12);
	ILI9341_SendData(0x81);
	ILI9341_SendCommand(ILI9341_PRC);
	ILI9341_SendData(0x20);
	ILI9341_SendCommand(ILI9341_POWER1);
	ILI9341_SendData(0x23);
	ILI9341_SendCommand(ILI9341_POWER2);
	ILI9341_SendData(0x10);
	ILI9341_SendCommand(ILI9341_VCOM1);
	ILI9341_SendData(0x3E);
	ILI9341_SendData(0x28);
	ILI9341_SendCommand(ILI9341_VCOM2);
	ILI9341_SendData(0x86);
	ILI9341_SendCommand(ILI9341_MAC);
	ILI9341_SendData(0x48);
	ILI9341_SendCommand(ILI9341_PIXEL_FORMAT);
	ILI9341_SendData(0x55);
	ILI9341_SendCommand(ILI9341_FRC);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x18);
	ILI9341_SendCommand(ILI9341_DFC);
	ILI9341_SendData(0x08);
	ILI9341_SendData(0x82);
	ILI9341_SendData(0x27);
	ILI9341_SendCommand(ILI9341_3GAMMA_EN);
	ILI9341_SendData(0x00);
	ILI9341_SendCommand(ILI9341_COLUMN_ADDR);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0xEF);
	ILI9341_SendCommand(ILI9341_PAGE_ADDR);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x01);
	ILI9341_SendData(0x3F);
	ILI9341_SendCommand(ILI9341_GAMMA);
	ILI9341_SendData(0x01);
	ILI9341_SendCommand(ILI9341_PGAMMA);
	ILI9341_SendData(0x0F);
	ILI9341_SendData(0x31);
	ILI9341_SendData(0x2B);
	ILI9341_SendData(0x0C);
	ILI9341_SendData(0x0E);
	ILI9341_SendData(0x08);
	ILI9341_SendData(0x4E);
	ILI9341_SendData(0xF1);
	ILI9341_SendData(0x37);
	ILI9341_SendData(0x07);
	ILI9341_SendData(0x10);
	ILI9341_SendData(0x03);
	ILI9341_SendData(0x0E);
	ILI9341_SendData(0x09);
	ILI9341_SendData(0x00);
	ILI9341_SendCommand(ILI9341_NGAMMA);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x0E);
	ILI9341_SendData(0x14);
	ILI9341_SendData(0x03);
	ILI9341_SendData(0x11);
	ILI9341_SendData(0x07);
	ILI9341_SendData(0x31);
	ILI9341_SendData(0xC1);
	ILI9341_SendData(0x48);
	ILI9341_SendData(0x08);
	ILI9341_SendData(0x0F);
	ILI9341_SendData(0x0C);
	ILI9341_SendData(0x31);
	ILI9341_SendData(0x36);
	ILI9341_SendData(0x0F);
	ILI9341_SendCommand(ILI9341_SLEEP_OUT);
	_delay_ms(200);
	ILI9341_SendCommand(ILI9341_DISPLAY_ON);
	//ILI9341_SendCommand(ILI9341_GRAM);


}

void ILI9341_SendData(uint8_t data){
	//ILI9341_CD_HIGH;
	GPIOD->BSRRL = (1<<13);
	uint8_t temp;
	GPIOC->BSRRH = (1<<2);

	SPI5->DR = data;
	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE) == RESET);
	temp = SPI5->DR;
	GPIOC->BSRRL = (1<<2);
}
void ILI9341_SendCommand(uint8_t data){
	GPIOD->BSRRH = (1<<13);
	uint8_t temp;
	GPIOC->BSRRH = (1<<2);
	SPI5->DR = data;
	while (SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE) == RESET);
	temp = SPI5->DR;
	GPIOC->BSRRL = (1<<2);
}

void ILI9341_setPixel(uint16_t x, uint16_t y , uint32_t color){
	ILI9341_SetCursorPosition(x, y, x, y);
	ILI9341_SendCommand(ILI9341_GRAM);
	ILI9341_SendData(color >> 8);
	ILI9341_SendData(color & 0xff);
}

void ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	ILI9341_SendCommand(ILI9341_COLUMN_ADDR);
	ILI9341_SendData(x1 >> 8);
	ILI9341_SendData(x1 & 0xFF);
	ILI9341_SendData(x2 >> 8);
	ILI9341_SendData(x2 & 0xFF);

	ILI9341_SendCommand(ILI9341_PAGE_ADDR);
	ILI9341_SendData(y1 >> 8);
	ILI9341_SendData(y1 & 0xFF);
	ILI9341_SendData(y2 >> 8);
	ILI9341_SendData(y2 & 0xFF);
}

