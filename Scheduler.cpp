/*
   Scheduler.cpp

    Created on: Jan 25, 2018
        Author: unni
*/
#include "Arduino.h"
#include "Scheduler.hpp"

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  SchedulerNs::Scheduler::schedulerTriggerCount++;
}

namespace SchedulerNs
{

taskControlBlock Scheduler::taskInfo[MAX_TASKS];
bool Scheduler::isSchdulerUp = false;
ULONG Scheduler::schedulerTriggerCount = 0;

void Scheduler::Init()
{
  schedulerTimerInit();
  noInterrupts();
  for (int i = 0; i < MAX_TASKS; i++)
  {
    taskInfo[i].taskId = TASK_ID_INVALID;
    taskInfo[i].isTaskEnabled = false;
  }
  schedulerTriggerCount = 0;
  interrupts();
}

void Scheduler::schedulerTimerInit()
{
  noInterrupts();

  // initialize timer1
  	TCCR1A = 0;
  	TCCR1B = 0;
  	TCNT1  = 0;
  
//  	OCR1A = 31250;            // compare match register 16MHz/256/2Hz
  	OCR1A = 312*4;
  	TCCR1B |= (1 << WGM12);   // CTC mode
  	TCCR1B |= (1 << CS12);    // 256 prescaler
  //	TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  interrupts();
}

void Scheduler::Start()
{
  	TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
}

TID Scheduler::CreateTask(USHORT taskPrio, ULONG ticksToRun, taskInitFn_ptr taskInitFn, taskRunFn_ptr taskRunFn)
{
  TID id = TASK_ID_INVALID;

  noInterrupts();
  for (int i = 0; i < MAX_TASKS; i++)
  {
    if (taskInfo[i].taskId == TASK_ID_INVALID)
    {
      taskInfo[i].taskId = (i + 1);
      taskInfo[i].taskPrio = taskPrio;
      taskInfo[i].isTaskEnabled = true;
      taskInfo[i].taskTicksToRun = ticksToRun;
      taskInfo[i].taskTickCount = 0;
      taskInfo[i].taskInitFn = taskInitFn;
      taskInfo[i].taskRunFn = taskRunFn;
      if (taskInfo[i].taskInitFn != NULL)
      {
        taskInfo[i].taskInitFn() == true ? (id = i) : (id = TASK_INIT_FAILED);
      }
      break;
    }
  }
  interrupts();

  return id;
}

void Scheduler::EnableTask(TID task)
{
  taskInfo[task].isTaskEnabled = true;
  taskInfo[task].taskTickCount = taskInfo[task].taskTicksToRun;
}

void Scheduler::DisableTask(TID task)
{
  taskInfo[task].isTaskEnabled = false;
}

void Scheduler::DecrementSchedulerTriggerCount()
{
  noInterrupts();
  schedulerTriggerCount > 0 ? (schedulerTriggerCount--) : (schedulerTriggerCount = 0);
  interrupts();
}

void Scheduler::Run()
{
  for (int i = 0; i < MAX_TASKS; i++)
  {
    if ( (taskInfo[i].taskId != TASK_ID_INVALID) && (taskInfo[i].isTaskEnabled == true) )
    {
      taskInfo[i].taskTickCount++;
      if (taskInfo[i].taskTickCount >= taskInfo[i].taskTicksToRun)
      {
        taskInfo[i].taskRunFn();
        taskInfo[i].taskTickCount = 0;
      }
    }
  }
}

}
