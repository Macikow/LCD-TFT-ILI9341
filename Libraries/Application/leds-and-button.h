/*
 * leds-and-buttons.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Macikow
 */

#ifndef LEDS_AND_BUTTON_H_
#define LEDS_AND_BUTTON_H_


/** PORT - G
 * LED4 - LED GREEN - PG14
 * LED5 - LED RED   - PG13
 */
void userLedsInit(void);
void ledGreenOn(void);
void ledRedOn(void);
void ledGreenOff(void);
void ledRedOff(void);
void ledGreenToggle(void);
void ledRedToggle(void);
void userButtonInit(void);


#endif /* LEDS_AND_BUTTON_H_ */
