#include "Arduino.h"
#include "MotorControl.hpp"
#include "NotificationControl.hpp"

//cStepperMotor oPlatformStepper(MOTOR_TYPE_PLATFORM_STEPPER, PLATFORM_STEPPER_MOTOR_PWM_PIN, PLATFORM_STEPPER_MOTOR_DIRECTION_PIN);
//cStepperMotor oAerialStepper(MOTOR_TYPE_AERIAL_STEPPER, AERIAL_STEPPER_MOTOR_PWM_PIN, AERIAL_STEPPER_MOTOR_DIRECTION_PIN);

cStepperMotor::cStepperMotor(eMotorType eType, U8 u8Pwm_pin, U8 u8Dir_pin) : cMotor()
{
  u32MaxStepCounts = 0U;
  u32CurStepCount = 0U;
  u32FinalStepCount = 0U;
  u32StepsRequired = 0;
  eDirection = MOTOR_DIRECTION_INVALID;

  eMotor = MOTOR_TYPE_PLATFORM_STEPPER;
  u32Speed = PLATFORM_STEPPER_MOTOR_DEFAULT_STEP_SPEED;
  u8MotorPwmPin = u8Pwm_pin;
  u8MotorDirPin = u8Dir_pin;

  pinMode(u8MotorPwmPin, OUTPUT);
  pinMode(u8MotorDirPin, OUTPUT);
}

void cStepperMotor::MotorStart(U32 u32Pos)
{
  u32FinalStepCount = u32Pos;
  U32 u32StepsCount = 0;
  eMotorDirection eDir;

  if (eDirection == MOTOR_DIRECTION_INVALID)
  {
    if (u32FinalStepCount > u32CurStepCount)
    {
      eDir = MOTOR_DIRECTION_FORWARD;
      u32StepsCount = u32FinalStepCount - u32CurStepCount;
      digitalWrite(u8MotorDirPin, true);
    }
    else if (u32FinalStepCount < u32CurStepCount)
    {
      eDir = MOTOR_DIRECTION_REVERSE;
      u32StepsCount = u32CurStepCount - u32FinalStepCount;
      digitalWrite(u8MotorDirPin, false);
    }
    else
    {
      eDir = MOTOR_DIRECTION_INVALID;
    }

    noInterrupts();
    u32StepsRequired = u32StepsCount;
    eDirection = eDir;
    interrupts();
  }
}

void cStepperMotor::MotorStop()
{
  noInterrupts();
  u32StepsRequired = 0;
  interrupts();
}

void cStepperMotor::MotorStepProcessing()
{
  if (eDirection != MOTOR_DIRECTION_INVALID)
  {
    if (u32StepsRequired > 0)
    {
      u32StepsRequired--;
      digitalWrite(u8MotorPwmPin, !digitalRead(u8MotorPwmPin));
    }
    else
    {
      Serial.write("Stopped \n");
//      cLedControl::SetSystemBusyStatus(CONTROL_TYPE_PLATFORM, false);
      digitalWrite(u8MotorPwmPin, false);
      eDirection = MOTOR_DIRECTION_INVALID;
    }
    if (eDirection == MOTOR_DIRECTION_FORWARD)
    {
      u32CurStepCount++;
    }
    else
    {
      u32CurStepCount--;
    }
  }
}

void cStepperMotor::InitilizeStepperMotor()
{
  // initialize timer0
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0  = 0;

  OCR0A = 150;
  TCCR0A |= (1 << WGM01);   // CTC mode
  TCCR0B |= (1 << CS01);    // 64 prescaler
  TCCR0B |= (1 << CS00);    // 64 prescaler

  TIMSK0 |= (1 << OCIE0A);  // enable timer compare interrupt
//  Serial.write("Started ******** \n");

  //  oPlatformStepper.MotorStart(1000);
}
//
//ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
//{
////  oPlatformStepper.MotorStepProcessing();
////  oAerialStepper.MotorStepProcessing();
//}
