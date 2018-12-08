#include "Arduino.h"
#include "ArmControl.hpp"
#include "NotificationControl.hpp"

cPlatformControl::cPlatformControl()
{
  SetMotorObject(new cStepperMotor(MOTOR_TYPE_PLATFORM_STEPPER));
//  SetSenorObject(new cPositionSensor(SENSOR_TYPE_PLATFORM_HOME), new cPositionSensor(SENSOR_TYPE_PLATFORM_BACK));
}

cAerialControl::cAerialControl()
{
  SetMotorObject(new cStepperMotor(MOTOR_TYPE_AERIAL_STEPPER));
//  SetSenorObject(new cPositionSensor(SENSOR_TYPE_AERIAL_HOME), new cPositionSensor(SENSOR_TYPE_AERIAL_BACK));
}

BOOL cArmControl::StartHomingOperation()
{
  BOOL bStatus = false;

  if ((eHomingStatus == HOMING_STATE_IDLE) || (eHomingStatus == HOMING_STATE_TIMEOUT))
  {
    eHomingStatus = HOMING_STATE_START;
    bStatus = true;
  }

  return bStatus;
}

void cArmControl::PerformHomingOperation()
{
  switch (eHomingStatus)
  {
    case HOMING_STATE_START: {ProcessHomingStart(); break;}
    case HOMING_STATE_WAITING_HOME: {ProcessHomingWait(); break;}
    case HOMING_STATE_END: {ProcessHomingEnd(); break;}
    case HOMING_STATE_TIMEOUT:{ProcessHomingTimeout(); break;}
    case HOMING_STATE_IDLE: default:{ break;}
  }
}

void cArmControl::ProcessHomingStart()
{
  if (oHomeSensor->GetSensorStatus() == false)
  {
    u32HomingTimeoutCount = 0;
    bIsHomingDone = false;
    eHomingStatus = HOMING_STATE_WAITING_HOME;
    cLedControl::SetLedPattern(LED_PATTERN_SYSYTEM_BUSY);
  }
  else
  {
    eHomingStatus = HOMING_STATE_END;
  }
}
void cArmControl::ProcessHomingWait()
{
  if (oHomeSensor->GetSensorStatus() == true)
  {
    eHomingStatus = HOMING_STATE_END;
  }
  else
  {
    u32HomingTimeoutCount++;
    if (u32HomingTimeoutCount >= MAX_HOMING_TIMOUT_COUNT)
    {
      eHomingStatus = HOMING_STATE_TIMEOUT;
    }
  }
}
void cArmControl::ProcessHomingEnd()
{
  eHomingStatus = HOMING_STATE_IDLE;
  bIsHomingDone = true;
  cLedControl::SetLedPattern(LED_PATTERN_SYSYTEM_IDLE);
}

void cArmControl::ProcessHomingTimeout()
{
  bIsHomingDone = false;
  cBuzzerControl::SetBuzzerPattern(BUZZER_PATTERN_4);
  cLedControl::SetLedPattern(LED_PATTERN_SYSYTEM_IDLE);
}
