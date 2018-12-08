/*
   Scheduler.hpp

    Created on: Jan 25, 2018
        Author: unni
*/

#ifndef SCHEDULER_HPP_
#define SCHEDULER_HPP_

#include "stdtype.hpp"

namespace SchedulerNs
{

#define MAX_TASKS			8

typedef void (*taskRunFn_ptr)();
typedef bool (*taskInitFn_ptr)();

typedef struct
{
  TID taskId;
  USHORT taskPrio;
  bool isTaskEnabled;
  ULONG taskTicksToRun;
  ULONG taskTickCount;
  taskInitFn_ptr taskInitFn;
  taskRunFn_ptr taskRunFn;
} taskControlBlock;

#define TASK_TICK_10MS   (1)
#define TASK_TICK_20MS   (TASK_TICK_10MS * 2)
#define TASK_TICK_50MS   (TASK_TICK_10MS * 5)
#define TASK_TICK_100MS  (TASK_TICK_10MS * 10)
#define TASK_TICK_200MS  (TASK_TICK_100MS * 2)
#define TASK_TICK_500MS  (TASK_TICK_100MS * 5)
#define TASK_TICK_1000ms (TASK_TICK_10MS * 100)

#define TASK_INPUT_CONDITIONING_TICK  TASK_TICK_20MS
#define TASK_MOTOR_CONTROL_TICK     TASK_TICK_10MS
#define TASK_USER_NOTIFICATION_TICK   TASK_TICK_50MS
#define TASK_PLOTTER_CONTROLLER_TICK  TASK_TICK_100MS

#define TASK_ID_INVALID   0
#define TASK_INIT_FAILED  -1

typedef enum
{
  TASK_CREATE_RESPONSE_INVALID = -1,
  TASK_CREATE_RESPONSE_SUCCESS,
  TASK_CREATE_RESPONSE_FAILURE,
  TASK_CREATE_RESPONSE_MAX
}eTaskCreateResponse;

class Scheduler
{
  private:
    static taskControlBlock taskInfo[MAX_TASKS];
    static bool isSchdulerUp;
  public:
    static ULONG schedulerTriggerCount;

    static void schedulerTimerInit();
    static void Init();
    static void Start();
    static void Run(void);
    static void DisableTask(TID);
    static void EnableTask(TID);
    static TID CreateTask(USHORT taskPrio, ULONG ticksToRun, taskInitFn_ptr taskInitFn, taskRunFn_ptr taskRunFn);
    static void DecrementSchedulerTriggerCount();
};

}

#endif /* SCHEDULER_HPP_ */
