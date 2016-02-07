/*
 * ILI9341.h
 *
 *  Created on: Feb 7, 2016
 *      Author: Macikow
 */

#ifndef ILI9341_H_
#define ILI9341_H_

void InterfacePinConfig(void);
void ILI9341Init(void);
void ILI9341_setPixel(uint16_t x, uint16_t y , uint32_t color);

void ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);


#endif /* ILI9341_H_ */
