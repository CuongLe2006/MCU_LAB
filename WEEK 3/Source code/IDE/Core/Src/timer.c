/*
 * timer.c
 *
 *  Created on: Oct 27, 2026
 *      Author: USER
 */
#include "timer.h"
#include "input_reading.h"

static int cnt[N_TIMER], flag[N_TIMER];

void setTimer(int id, int ms) {
  int t = (ms + TICK / 2) / TICK;    // ms -> ticks (rounded)
  if (t <= 0) t = 1;
  flag[id] = 0;
  cnt[id] = t;
}

int timeUp(int id) {
  if (!flag[id]) return 0;
  flag[id] = 0;
  return 1;
}

void timerRun(void) {
  for (int i = 0; i < N_TIMER; i++)
    if (cnt[i] > 0 && --cnt[i] == 0) flag[i] = 1;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2) {
    timerRun();
    btnRead();
  }
}
