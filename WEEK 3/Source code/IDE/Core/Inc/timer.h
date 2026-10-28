/*
 * timer.h
 *
 *  Created on: Oct 27, 2026
 *      Author: USER
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_
#include "main.h"

#define TICK 10                      // interrupt period (ms) - only value to change (Ex 4)

enum { T_BLINK, T_SEC, T_SCAN, N_TIMER };

void setTimer(int id, int ms);
int  timeUp(int id);
void timerRun(void);

#endif
