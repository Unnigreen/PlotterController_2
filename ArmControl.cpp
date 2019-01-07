#include "Arduino.h"
#include "ArmControl.hpp"
#include "MotorControl.hpp"
#include "NotificationControl.hpp"
#include "SensorMonitor.hpp"

cArmControl * oPlatformContoller = NULL;
cArmControl * oAerialContoller = NULL;

void cDeviceController::InitPlatformController()
{
  Serial.write("Init\n");
  oPlatformContoller = new cArmControl(new cStepperMotor(MOTOR_TYPE_PLATFORM_STEPPER, PLATFORM_STEPPER_MOTOR_PWM_PIN, PLATFORM_STEPPER_MOTOR_DIRECTION_PIN));
//  oPlatformContoller->MoveSteps(1000);
  Serial.write("Init done\n");
}

void cDeviceController::InitAerialController()
{
  //  oAerialContoller = new cArmControl(new cStepperMotor(MOTOR_TYPE_AERIAL_STEPPER));
}

bool cDeviceController::SetPlatformOperation(eDeviceState eNewState)
{
  Serial.write("Set Platform operation\n");
  return oPlatformContoller->SetDeviceState(eNewState);

}

bool cDeviceController::SetAerialOperation(eDeviceState eNewState)
{
  return oAerialContoller->SetDeviceState(eNewState);
}

void cDeviceController::PerformPlatformOperation()
{
  static U32 u32Sign = 0;
  U32 u32Steps = 0;
  
  Serial.write("Platform operation\n");
  switch (oPlatformContoller->GetDeviceState())
  {
    case DEVICE_STATE_HOMING:
      {
        Serial.write("Platform HOMING operation\n");
        oPlatformContoller->PerformHomingOperation();
        break;
      }
    default:
      {
        u32Sign++ % 2 == 0 ? u32Steps = 100 : u32Steps = 10;
        oPlatformContoller->MoveSteps(u32Steps);
        break;
      }
  }
}

void cDeviceController::PerformAerialOperation()
{
  switch (oAerialContoller->GetDeviceState())
  {
    case DEVICE_STATE_HOMING:
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
        Serial.write("Platform HOMING - INIT \n");
        ProcessHomingInit();
        break;
      }
    case HOMING_STATE_WAITING_HOME: {
        Serial.write("Platform HOMING - WAITING \n");
        ProcessHomingWait();
        break;
      }
    case HOMING_STATE_END: {
        Serial.write("Platform HOMING - END \n");
        ProcessHomingEnd();
        break;
      }
    case HOMING_STATE_TIMEOUT: {
        Serial.write("Platform HOMING - TIME_OUT \n");
        ProcessHomingTimeout();
        break;
      }
  case HOMING_STATE_IDLE: default: {
        Serial.write("Platform HOMING - IDLE \n");
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
    case DEVICE_STATE_HOMING:
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

ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
{
  if (oPlatformContoller != 0)
  {
//    Serial.write("2");
    oPlatformContoller->MtrStepProcessing();
  }
  //  oPlatformStepper.MotorStepProcessing();
  //  oAerialStepper.MotorStepProcessing();
}
