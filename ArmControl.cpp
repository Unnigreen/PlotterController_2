#include "Arduino.h"
#include "ArmControl.hpp"
#include "NotificationControl.hpp"
#include "SensorMonitor.hpp"

cArmControl * oPlatformContoller = NULL;
cArmControl * oAerialContoller = NULL;

void cDeviceController::InitPlatformController()
{
  oPlatformContoller = new cArmControl(new cStepperMotor(MOTOR_TYPE_PLATFORM_STEPPER));
}

void cDeviceController::InitAerialController()
{
  oAerialContoller = new cArmControl(new cStepperMotor(MOTOR_TYPE_AERIAL_STEPPER));
}

bool cDeviceController::SetPlatformOperation(eDeviceState eNewState)
{
  return oPlatformContoller->SetDeviceState(eNewState);
}

bool cDeviceController::SetAerialOperation(eDeviceState eNewState)
{
  return oAerialContoller->SetDeviceState(eNewState);
}

void cDeviceController::PerformPlatformOperation()
{
  switch (oPlatformContoller->GetDeviceState())
  {
    case DEVICE_FUNCTION_HOMING:
      {
        oPlatformContoller->PerformHomingOperation();
        break;
      }
    default:
      {
        break;
      }
  }
}

void cDeviceController::PerformAerialOperation()
{
  switch (oAerialContoller->GetDeviceState())
  {
    case DEVICE_FUNCTION_HOMING:
      {
        oAerialContoller->PerformHomingOperation();
        break;
      }
    default:
      {
        break;
      }
  }
}

void cArmControl::PerformHomingOperation()
{
  switch (eHomingStatus)
  {
    case HOMING_STATE_INIT: {
        ProcessHomingInit();
        break;
      }
    case HOMING_STATE_WAITING_HOME: {
        ProcessHomingWait();
        break;
      }
    case HOMING_STATE_END: {
        ProcessHomingEnd();
        break;
      }
    case HOMING_STATE_TIMEOUT: {
        ProcessHomingTimeout();
        break;
      }
  case HOMING_STATE_IDLE: default: {
        break;
      }
  }
}

void cArmControl::ProcessHomingInit()
{
  if (oHomeSensor->GetSensorStatus() == false)
  {
    u32HomingTimeoutCount = 0;
    bIsHomingDone = false;
    eHomingStatus = HOMING_STATE_WAITING_HOME;
    //    cLedControl::SetLedPattern(LED_PATTERN_SYSYTEM_BUSY);
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
  eState = DEVICE_STATE_IDLE;
  //  cLedControl::SetLedPattern(LED_PATTERN_SYSYTEM_IDLE);
}

void cArmControl::ProcessHomingTimeout()
{
  bIsHomingDone = false;
  eState = DEVICE_STATE_IDLE;
  //  cBuzzerControl::SetBuzzerPattern(BUZZER_PATTERN_4);
  //  cLedControl::SetLedPattern(LED_PATTERN_SYSYTEM_IDLE);
}

bool cArmControl::SetDeviceState(eDeviceState eNewState)
{
  bool bStatus = false;

  if (eState == DEVICE_STATE_IDLE)
  {
    eState = eNewState;
    InitializeNewState();
    bStatus = true;
  }
}

void cArmControl::InitializeNewState()
{
  switch (eState)
  {
    case DEVICE_FUNCTION_HOMING:
      {
        eHomingStatus = HOMING_STATE_INIT;
        break;
      }
    default:
      {
        break;
      }
  }
}
