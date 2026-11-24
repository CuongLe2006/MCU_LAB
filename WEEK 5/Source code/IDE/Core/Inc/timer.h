#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#define TICK 10                         // TIM2 interrupt period (ms)

enum { T_LED, T_TIMEOUT, N_TIMER };     // software timers

void setTimer(int id, int ms);
int  timeUp(int id);
void timerRun(void);

#endif
