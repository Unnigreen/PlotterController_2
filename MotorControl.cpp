#include "Arduino.h"
#include "MotorControl.hpp"

cStepperMotor oPlatformStepper(MOTOR_TYPE_PLATFORM_STEPPER);
cStepperMotor oAerialStepper(MOTOR_TYPE_AERIAL_STEPPER);

void PwmTest()
{
  oPlatformStepper.Start();
}

cStepperMotor::cStepperMotor(eMotorType eType) : cMotorAbs()
{
  u32MaxStepCounts = 0U;
  u32CurStepCount = 0U;
  u32FinalStepCount = 0U;
  u32StepsRequired = 0;

  if (eType == MOTOR_TYPE_PLATFORM_STEPPER)
  {
    SetMotorType(MOTOR_TYPE_PLATFORM_STEPPER);
    u32Speed = PLATFORM_STEPPER_MOTOR_DEFAULT_STEP_SPEED;
    i32MotorPwmPin = PLATFORM_STEPPER_MOTOR_PWM_PIN;
  }
  else if (eType == MOTOR_TYPE_AERIAL_STEPPER)
  {
    SetMotorType(MOTOR_TYPE_AERIAL_STEPPER);
    u32Speed = AERIAL_STEPPER_MOTOR_DEFAULT_STEP_SPEED;
    i32MotorPwmPin = AERIAL_STEPPER_MOTOR_PWM_PIN;
  }
  else
  {
    SetMotorType(MOTOR_TYPE_INVALID);
    u32Speed = 0U;
    i32MotorPwmPin = 0;
  }
}

BOOL cStepperMotor::MotorMoveSteps(U32 u32Pos)
{
  BOOL bStatus = false;

  u32FinalStepCount = u32Pos;

  if (u32FinalStepCount > u32CurStepCount)
  {
    SetMotorDirection(MOTOR_DIRECTION_FORWARD);
    u32StepsRequired = u32FinalStepCount - u32CurStepCount;
  }
  else if (u32FinalStepCount < u32CurStepCount)
  {
    SetMotorDirection(MOTOR_DIRECTION_REVERSE);
    u32StepsRequired = u32CurStepCount - u32FinalStepCount;
  }
  else
  {
    SetMotorDirection(MOTOR_DIRECTION_STATIONARY);
    u32StepsRequired = 0;
  }

  bStatus = MotorStart();

  return bStatus;
}

BOOL cStepperMotor::MotorStart()
{
  BOOL bStatus = false;

  analogWrite(i32MotorPwmPin, 128);

  return bStatus;
}

BOOL cStepperMotor::MotorStop()
{
  analogWrite(i32MotorPwmPin, 0);
}

BOOL cStepperMotor::SetMotorSpeed(U32 u32MotorSpeed)
{
  BOOL bStatus = true;
  U32 u32StepSpeed = u32MotorSpeed;

  if (GetMotorType() == MOTOR_TYPE_PLATFORM_STEPPER)
  {
    if (u32StepSpeed > PLATFORM_STEPPER_MOTOR_MAX_STEP_SPEED)
    {
      u32StepSpeed = PLATFORM_STEPPER_MOTOR_MAX_STEP_SPEED;
    }
  }
  else if (GetMotorType() == MOTOR_TYPE_AERIAL_STEPPER)
  {
    if (u32StepSpeed > AERIAL_STEPPER_MOTOR_MAX_STEP_SPEED)
    {
      u32StepSpeed = AERIAL_STEPPER_MOTOR_MAX_STEP_SPEED;
    }
  }
  else
  {
    u32StepSpeed = 0U;
    bStatus = false;
  }
  u32Speed = u32StepSpeed;

  SetMotorPwm();

  return bStatus;
}

void cStepperMotor::SetMotorPwm()
{
  //  OCR1A = 0;
  //  TCCR1A = 0;
}

void cStepperMotor::MotorStepProcessing()
{
  if (u32StepsRequired > 0)
  {
    u32StepsRequired--;
  }

  if (u32StepsRequired == 0)
  {
    Serial.write("Stopped \n");
//    oPlatformStepper.Stop();
    u32StepsRequired = 2000;
    //    analogWrite(i32MotorPwmPin, 0);
  }
}

void cStepperMotor::Start()
{
  u32StepsRequired = 2000;

  noInterrupts();

  // initialize timer0
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0  = 0;

  //    OCR1A = 31250;            // compare match register 16MHz/256/2Hz
  //  OCR0A = 312 * 4;
  OCR0A = 150;

  TCCR0A |= (1 << WGM01);   // CTC mode
//  TCCR0B |= (1 << WGM00);   // CTC mode
  
//  TCCR0B |= (1 << WGM00);   // CTC mode
//  TCCR0B |= (1 << WGM01);   // CTC mode
  //  TCCR0B |= (1 << CS12);    // 256 prescaler
  TCCR0B |= (1 << CS01);    // 64 prescaler
  TCCR0B |= (1 << CS00);    // 64 prescaler
  //  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  interrupts();

  Serial.write("Started ************ \n");
  //  TIMSK1 |= (1 << OCIE1B);  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);  // enable timer compare interrupt
}

void cStepperMotor::Stop()
{
  //  TIMSK1 &= ~(U8)(1 << OCIE1B);  // enable timer compare interrupt
  TIMSK0 &= ~(U8)(1 << OCIE0A);  // enable timer compare interrupt
}

ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
{
  oPlatformStepper.MotorStepProcessing();
}

//ISR(TIMER1_COMPA_vect)
//{
//  oPlatformStepper.MotorStepProcessing();
//}
