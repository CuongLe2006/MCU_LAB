#include "main.h"
#include "scheduler.h"

/*
 * Every task stores the tick at which it must run (RunAt).
 * The list is kept SORTED by RunAt, so the task that runs first is always tasks[0].
 *
 *  - SCH_Update (interrupt) only counts ticks: O(1), no search.
 *  - SCH_Dispatch_Tasks (main loop) runs tasks[0] while its time has come.
 *  - A periodic task gets RunAt += Period, so a late run never shifts later runs.
 */
static sTask tasks[SCH_MAX_TASKS];
static volatile uint32_t count = 0;      // number of tasks in the list
static volatile uint32_t ticks = 0;      // ticks since start
static uint32_t nextID = 1;              // IDs start at 1 (0 = no task)

/* 1 if tick a comes after tick b (works even when the counter wraps) */
static int after(uint32_t a, uint32_t b) { return (int32_t)(a - b) > 0; }

/* put t into the list, keeping it sorted by RunAt */
static void insert(sTask t) {
  uint32_t i = 0;
  while (i < count && !after(tasks[i].RunAt, t.RunAt)) i++;   // equal times keep their order
  for (uint32_t j = count; j > i; j--) tasks[j] = tasks[j - 1];
  tasks[i] = t;
  count++;
}

/* take the task at position i out of the list */
static void removeAt(uint32_t i) {
  for (uint32_t j = i; j + 1 < count; j++) tasks[j] = tasks[j + 1];
  count--;
}

void SCH_Init(void) {
  __disable_irq();
  count = 0;
  ticks = 0;
  __enable_irq();
}

/* DELAY and PERIOD are in ms. Returns the task ID, or NO_TASK_ID if the list is full */
uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD) {
  if (pFunction == 0) return NO_TASK_ID;
  uint32_t id = NO_TASK_ID;
  __disable_irq();
  if (count < SCH_MAX_TASKS) {
    sTask t = { pFunction, ticks + DELAY / TICK_MS, PERIOD / TICK_MS, nextID++ };
    insert(t);
    id = t.TaskID;
  }
  __enable_irq();
  return id;
}

/* called every TICK_MS in the timer interrupt: O(1) */
void SCH_Update(void) {
  ticks++;
}

/* called in the super loop: runs every task whose time has come, in order */
void SCH_Dispatch_Tasks(void) {
  while (1) {
    __disable_irq();
    if (count == 0 || after(tasks[0].RunAt, ticks)) {   // first task is not due yet
      __enable_irq();
      return;
    }
    sTask t = tasks[0];
    removeAt(0);
    if (t.Period > 0) {                       // periodic: schedule the next run
      t.RunAt += t.Period;
      insert(t);
    }
    __enable_irq();
    t.pTask();                                // run it (interrupts are on again)
  }
}

/* returns 1 if deleted, 0 if no task has this ID */
uint8_t SCH_Delete_Task(uint32_t taskID) {
  uint8_t ok = 0;
  if (taskID == NO_TASK_ID) return 0;
  __disable_irq();
  for (uint32_t i = 0; i < count; i++) {
    if (tasks[i].TaskID == taskID) {
      removeAt(i);
      ok = 1;
      break;
    }
  }
  __enable_irq();
  return ok;
}

uint32_t SCH_Get_Time(void) {
  return ticks * TICK_MS;
}
