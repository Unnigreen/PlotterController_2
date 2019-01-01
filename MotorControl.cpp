#include "Arduino.h"
#include "MotorControl.hpp"

cStepperMotor oPlatformStepper(MOTOR_TYPE_PLATFORM_STEPPER);
cStepperMotor oAerialStepper(MOTOR_TYPE_AERIAL_STEPPER);

cStepperMotor::cStepperMotor(eMotorType eType) : cMotorAbs()
{
  u32MaxStepCounts = 0U;
  u32CurStepCount = 0U;
  u32FinalStepCount = 0U;
  u32StepsRequired = 0;

  if (eType == MOTOR_TYPE_PLATFORM_STEPPER)
  {
    eMotor = MOTOR_TYPE_PLATFORM_STEPPER;
    u32Speed = PLATFORM_STEPPER_MOTOR_DEFAULT_STEP_SPEED;
    i32MotorPwmPin = PLATFORM_STEPPER_MOTOR_PWM_PIN;
  }
  else if (eType == MOTOR_TYPE_AERIAL_STEPPER)
  {
    eMotor = MOTOR_TYPE_AERIAL_STEPPER;
    u32Speed = AERIAL_STEPPER_MOTOR_DEFAULT_STEP_SPEED;
    i32MotorPwmPin = AERIAL_STEPPER_MOTOR_PWM_PIN;
  }
  else
  {
    eMotor = MOTOR_TYPE_INVALID;
    u32Speed = 0U;
    i32MotorPwmPin = 0;
  }
}

void cStepperMotor::MotorStart(U32 u32Pos)
{
  u32FinalStepCount = u32Pos;
  U32 u32StepsCount = 0;
  eMotorDirection eDir;

  if (u32FinalStepCount > u32CurStepCount)
  {
    eDir = MOTOR_DIRECTION_FORWARD;
    u32StepsCount = u32FinalStepCount - u32CurStepCount;
  }
  else if (u32FinalStepCount < u32CurStepCount)
  {
    eDir = MOTOR_DIRECTION_REVERSE;
    u32StepsCount = u32CurStepCount - u32FinalStepCount;
  }
  else
  {
    eDir = MOTOR_DIRECTION_STATIONARY;
  }

  noInterrupts();
  u32StepsRequired = u32StepsCount;
  eDirection = eDir;
  interrupts();
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
      digitalWrite(i32MotorPwmPin, !digitalRead(i32MotorPwmPin));
    }

    if (u32StepsRequired == 0)
    {
      Serial.write("Stopped \n");
      digitalWrite(i32MotorPwmPin, false);
      eDirection = MOTOR_DIRECTION_INVALID;
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
  Serial.write("Started \n");
}

ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
{
  oPlatformStepper.MotorStepProcessing();
  oAerialStepper.MotorStepProcessing();
}
