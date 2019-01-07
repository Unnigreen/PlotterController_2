#ifndef __MOTOR_CONTROL_HPP__
#define __MOTOR_CONTROL_HPP__

#include "stdtype.hpp"

#define PLATFORM_STEPPER_MOTOR_PWM_PIN  10
#define PLATFORM_STEPPER_MOTOR_DIRECTION_PIN  11
#define AERIAL_STEPPER_MOTOR_PWM_PIN  5
#define AERIAL_STEPPER_MOTOR_DIRECTION_PIN  6
#define PLATFORM_STEPPER_MOTOR_DEFAULT_STEP_SPEED  500
#define AERIAL_STEPPER_MOTOR_DEFAULT_STEP_SPEED  500
#define PLATFORM_STEPPER_MOTOR_MAX_STEP_SPEED  1000
#define AERIAL_STEPPER_MOTOR_MAX_STEP_SPEED  1000

typedef enum
{
  MOTOR_TYPE_INVALID = -1,
  MOTOR_TYPE_PLATFORM_STEPPER,
  MOTOR_TYPE_AERIAL_STEPPER,
  MOTOR_TYPE_DC,
  MOTOR_TYPE_MAX
} eMotorType;

typedef enum
{
  MOTOR_DIRECTION_INVALID = -1,
  MOTOR_DIRECTION_FORWARD,
  MOTOR_DIRECTION_REVERSE,
  MOTOR_DIRECTION_STATIONARY,
  MOTOR_DIRECTION_MAX
} eMotorDirection;

class cMotor
{
  private:

  protected:
    eMotorType eMotor;
    eMotorDirection eDirection;

  public:
    cMotor(void) {
      eDirection = MOTOR_DIRECTION_INVALID;
      eMotor = MOTOR_TYPE_INVALID;
    }

    virtual void MotorStart(U32 u32Param) = 0;
    virtual void MotorStop() = 0;
    virtual void MotorStepProcessing() {
      return;
    };
};

class cStepperMotor : public cMotor
{
  private:
    U8 u8MotorPwmPin;
    U8 u8MotorDirPin;

    U32 u32Speed;
    U32 u32MaxStepCounts;
    U32 u32StepsRequired;
    U32 u32CurStepCount;
    U32 u32FinalStepCount;

    cStepperMotor();

  public:
    cStepperMotor(eMotorType eType, U8 u8Pwm_pin, U8 u8Dir_pin);

    void MotorStart(U32 u32Param);
    void MotorStop();
    void MotorMoveSteps(U32 u32Pos);
    void MotorStepProcessing();
    static void InitilizeStepperMotor();
};

#endif // end of __MOTOR_CONTROL_HPP__
