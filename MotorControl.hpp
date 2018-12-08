#include "stdtype.hpp"

#define PLATFORM_STEPPER_MOTOR_PWM_PIN  3
#define AERIAL_STEPPER_MOTOR_PWM_PIN  5
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

class cMotorAbs
{
  private:
    eMotorType eMotor;
    eMotorDirection eDirection;
    //    cMotorAbs * poMotorObj;

  public:
    cMotorAbs() {
      eDirection = MOTOR_DIRECTION_INVALID;
      //      poMotorObj = 0;
    }
    //    void SetMotorObject(cMotorAbs * poMotor) {
    //      poMotorObj = poMotor;
    //    }
    //    cMotorAbs * GetMotorObject() {
    //      return poMotorObj;
    //    }
    void SetMotorType(eMotorType eType) {
      ((eType > MOTOR_TYPE_INVALID) && (eType < MOTOR_TYPE_MAX)) ? eMotor = eType : eMotor = MOTOR_TYPE_INVALID;
    }
    eMotorType GetMotorType() {
      return eMotor;
    }
    void SetMotorDirection(eMotorDirection eDir)
    {
      ((eDir > MOTOR_DIRECTION_INVALID) && (eDir < MOTOR_DIRECTION_MAX)) ? eDirection = eDir : eDirection = MOTOR_DIRECTION_INVALID;
    }

    eMotorDirection GetMotorDirection() {
      return eDirection;
    }

    virtual BOOL SetMotorSpeed(U32 u32MotorSpeed) = 0;
    virtual BOOL MotorMoveSteps(U32 u32Pos) = 0;
    virtual BOOL MotorStop() = 0;
};

class cStepperMotor : public cMotorAbs
{
  private:
    U32 u32Speed;
    U32 u32MaxStepCounts;
    S32 i32MotorPwmPin;

    U32 u32StepsRequired;
    U32 u32CurStepCount;
    U32 u32FinalStepCount;

    BOOL MotorStart();
    void SetMotorPwm();
    cStepperMotor();

  public:
    cStepperMotor(eMotorType eType);

    BOOL SetMotorSpeed(U32 u32MotorSpeed);
    BOOL MotorMoveSteps(U32 u32Pos);
    BOOL MotorStop();
    void MotorStepProcessing();
};
