/*
 * input_reading.c
 *
 *  Created on: Oct 27, 2026
 *      Author: USER
 */

#include "main.h"
#include "input_reading.h"

static const uint16_t pin[N_BTN] = {BTN1_Pin, BTN2_Pin, BTN3_Pin};   // all on GPIOB
static GPIO_PinState last[N_BTN] = {1, 1, 1};   // previous reading
static GPIO_PinState st[N_BTN]   = {1, 1, 1};   // debounced state (1 = released)
static uint8_t prs[N_BTN];                      // 1 = new press

void btnRead(void) {
  for (int i = 0; i < N_BTN; i++) {
    GPIO_PinState now = HAL_GPIO_ReadPin(GPIOB, pin[i]);
    if (now == last[i] && now != st[i]) {       // same twice, and changed
      st[i] = now;
      if (now == 0) prs[i] = 1;                 // 0 = pressed
    }
    last[i] = now;
  }
}

int btnPressed(int i) {
  if (i < 0 || i >= N_BTN || !prs[i]) return 0;
  prs[i] = 0;
  return 1;
}
