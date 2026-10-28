/*
 * input_process.c
 *
 *  Created on: Oct 27, 2026
 *      Author: USER
 */

#include "main.h"
#include "input_process.h"
#include "input_reading.h"
#include "led_display.h"
#include "timer.h"

#define BLINK_MS 250                 // toggle every 250 ms -> 2 Hz

int tRed = 5, tAmber = 2, tGreen = 3;   // durations in seconds
static int mode = 1;
static int val;                         // value shown in modes 2-4

static int ts, left;                 // traffic state S1..S4 = 0..3, seconds left

static void showTraffic(void) {
  int r1 = left, r2 = left;
  if (ts == 0) r2 = left + tAmber;   // road 2 is red during S1 + S2
  if (ts == 2) r1 = left + tAmber;   // road 1 is red during S3 + S4
  setNum(r1, r2);
}

static void enterState(int s) {
  ts = s;
  switch (s) {
    case 0: left = tGreen; setLight(0, 2); setLight(1, 0); break;  // S1: G | R
    case 1: left = tAmber; setLight(0, 1); setLight(1, 0); break;  // S2: A | R
    case 2: left = tGreen; setLight(0, 0); setLight(1, 2); break;  // S3: R | G
    case 3: left = tAmber; setLight(0, 0); setLight(1, 1); break;  // S4: R | A
  }
  showTraffic();
}

static void trafficRun(void) {
  if (!timeUp(T_SEC)) return;
  setTimer(T_SEC, 1000);
  if (--left <= 0) enterState((ts + 1) % 4);   // time up -> next state
  else showTraffic();
}

static void enterMode(int m) {
  mode = m;
  lightsOff();
  if (mode == 1) {                   // normal mode: start traffic light at S1
    enterState(0);
    setTimer(T_SEC, 1000);
    return;
  }
  if (mode == 2) val = tRed;
  if (mode == 3) val = tAmber;
  if (mode == 4) val = tGreen;
  setNum(val, mode);                 // e.g. "05 02"
  setTimer(T_BLINK, BLINK_MS);
}

void fsmInit(void) { enterMode(1); }

static void saveVal(void) {
  if (mode == 2) {                     // red
    if (val > tAmber) {                // red must be longer than amber
      tRed = val;
      tGreen = tRed - tAmber;          // keep red = green + amber
    }
  }

  if (mode == 3) {                     // amber
    if (val + tGreen <= 99) {          // red must still fit in 2 digits
      tAmber = val;
      tRed = tAmber + tGreen;          // keep red = green + amber
    }
  }

  if (mode == 4) {                     // green
    if (tAmber + val <= 99) {          // red must still fit in 2 digits
      tGreen = val;
      tRed = tAmber + tGreen;          // keep red = green + amber
    }
  }
}

void fsmRun(void) {
  if (btnPressed(0)) enterMode(mode % 4 + 1);   // button 1: next mode

  if (mode == 1) {
    btnPressed(1);
    btnPressed(2);
    trafficRun();                    // run the traffic light
    return;
  }
  if (btnPressed(1)) {                 // button 2: value + 1
    val = val % 99 + 1;                // 1..99, then back to 1
    setNum(val, mode);
  }

  if (btnPressed(2)) saveVal();        // button 3: save

  if (timeUp(T_BLINK)) {               // blink this mode's colour at 2 Hz
    setTimer(T_BLINK, BLINK_MS);
    toggleColor(mode - 2);
  }
}
