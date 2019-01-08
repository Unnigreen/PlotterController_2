#include "Arduino.h"
#include "stdtype.hpp"
#include "ArmControl.hpp"
#include "Scheduler.hpp"
#include "NotificationControl.hpp"
#include "MotorControl.hpp"

void PwmTest();

void setup() {
  // put your setup code here, to run once:

  //  Serial.begin(115200);
  Serial.begin(9600);
  SchedulerNs::Scheduler::Init();

  //  SchedulerNs::Scheduler::CreateTask(1, 50, cSensorsMonitor::InitSensorMonitor, cSensorsMonitor::PollAndUpdateSensorStatus);
    SchedulerNs::Scheduler::CreateTask(1, 1, NULL, cLedControl::LedOperation);
  //  SchedulerNs::Scheduler::CreateTask(1, 1, NULL, cBuzzerControl::BuzzerOperation);
    SchedulerNs::Scheduler::CreateTask(1, 100, cDeviceController::InitPlatformController, cDeviceController::PerformPlatformOperation);
  //  SchedulerNs::Scheduler::CreateTask(1, 1000, cDeviceController::InitAerialController, cDeviceController::PerformAerialOperation);

  //  while (1)
  //  {
  //    digitalWrite(13, !digitalRead(13));
  //    delay(1000);
  //  }

  cStepperMotor::InitilizeStepperMotor();
  //  cDeviceController::SetPlatformOperation(DEVICE_STATE_HOMING);
  //  PwmTest();

  SchedulerNs::Scheduler::Start();
}

void loop()
{
  if (SchedulerNs::Scheduler::schedulerTriggerCount != 0)
  {
    SchedulerNs::Scheduler::Run();
    SchedulerNs::Scheduler::DecrementSchedulerTriggerCount();
  }

  //  cPlatformControl oPlatformController;
  //  cAerialControl oAerialController;
  //  oAerialController.StartHomingOperation();
  //  oAerialController.PerformHomingOperation();
}
