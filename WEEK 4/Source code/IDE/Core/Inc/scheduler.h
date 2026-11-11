#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>

#define TICK_MS        10      // timer interrupt period (ms)
#define SCH_MAX_TASKS  10      // max number of tasks at the same time
#define NO_TASK_ID     0       // returned when a task cannot be added

typedef struct {
  void (*pTask)(void);         // function to run
  uint32_t RunAt;              // tick at which the task must run next
  uint32_t Period;             // ticks between runs (0 = run once)
  uint32_t TaskID;             // unique ID given by SCH_Add_Task
} sTask;

void     SCH_Init(void);
void     SCH_Update(void);
void     SCH_Dispatch_Tasks(void);
uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD);
uint8_t  SCH_Delete_Task(uint32_t taskID);
uint32_t SCH_Get_Time(void);   // ms since start

#endif
