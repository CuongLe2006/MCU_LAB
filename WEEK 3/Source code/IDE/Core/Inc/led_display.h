/*
 * led_display.h
 *
 *  Created on: Oct 27, 2026
 *      Author: USER
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include "main.h"

#define SCAN_MS 50                   // ms per digit

void dispInit(void);
void dispRun(void);
void setNum(int l, int r);           // l -> digits 1-2, r -> digits 3-4
void lightsOff(void);
void toggleColor(int c);             // c: 0 = red, 1 = amber, 2 = green (both roads)
void setLight(int road, int c);

#endif /* INC_LED_DISPLAY_H_ */
