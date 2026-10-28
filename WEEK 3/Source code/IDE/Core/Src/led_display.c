/*
 * led_display.c
 *
 *  Created on: Oct 27, 2026
 *      Author: USER
 */

#include "led_display.h"
#include "timer.h"

static const uint8_t seg[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
static int buf[4], idx;

static void show7(int n) {
  for (int i = 0; i < 7; i++)          // SEG0..6 = PB0..6, ON = 0
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 << i, ((seg[n] >> i) & 1) ? 0 : 1);
}

static void scan7(int i) {
  HAL_GPIO_WritePin(GPIOA, EN0_Pin | EN1_Pin | EN2_Pin | EN3_Pin, 1);   // all off
  show7(buf[i]);
  HAL_GPIO_WritePin(GPIOA, EN0_Pin << i, 0);                            // one on
}

void dispInit(void) { setTimer(T_SCAN, SCAN_MS); }

void dispRun(void) {
  if (!timeUp(T_SCAN)) return;
  setTimer(T_SCAN, SCAN_MS);
  scan7(idx);
  idx = (idx + 1) % 4;
}

void setNum(int l, int r) {
  buf[0] = l / 10 % 10; buf[1] = l % 10;
  buf[2] = r / 10 % 10; buf[3] = r % 10;
}

// R1,Y1,G1 = PA0..PA2 ; R2,Y2,G2 = PA3..PA5 ; LED on = HIGH
void lightsOff(void) {
  HAL_GPIO_WritePin(GPIOA, R1_Pin | Y1_Pin | G1_Pin | R2_Pin | Y2_Pin | G2_Pin, 0);
}

void toggleColor(int c) {
  HAL_GPIO_TogglePin(GPIOA, (R1_Pin | R2_Pin) << c);
}

void setLight(int road, int c) {
  uint16_t base = road ? R2_Pin : R1_Pin;                       // PA0 or PA3
  HAL_GPIO_WritePin(GPIOA, base | base << 1 | base << 2, 0);    // this road all off
  HAL_GPIO_WritePin(GPIOA, base << c, 1);                       // one colour on
}
