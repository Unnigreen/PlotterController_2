#include "stdtype.hpp"
#include "SensorMonitor.hpp"
#include "MotorControl.hpp"

//#define MAX_HOMING_TIMOUT_COUNT 10000U
#define MAX_HOMING_TIMOUT_COUNT 10U

//typedef enum
//{
//  DEVICE_FUNCTION_INVALID = -1,
//  DEVICE_FUNCTION_HOMING,
//  DEVICE_FUNCTION_MAX
//} eDeviceFunctionalities;

typedef enum
{
  HOMING_STATE_IDLE = 0,
  HOMING_STATE_INIT,
  HOMING_STATE_WAITING_HOME,
  HOMING_STATE_END,
  HOMING_STATE_TIMEOUT,
  HOMING_STATE_MAX
} eHomingState;

typedef enum {
  DEVICE_STATE_IDLE,
  DEVICE_STATE_HOMING,
  DEVICE_STATE_MAX
} eDeviceState;

class cArmControl
{
  private:
    cMotor * poMotorObj;
    BOOL bIsHomingDone;
    eHomingState eHomingStatus;
    U32 u32HomingTimeoutCount;
    eDeviceState eState;

    cSwitchSensor * oHomeSensor;
    cSwitchSensor * oBackSensor;

    void ProcessHomingInit();
    void ProcessHomingWait();
    void ProcessHomingEnd();
    void ProcessHomingTimeout();

    cArmControl() {}

  public:

    cArmControl(cMotor * oMtr) {
      poMotorObj = oMtr;
      oHomeSensor = 0;
      oBackSensor = 0;
      bIsHomingDone = false;
      u32HomingTimeoutCount = 0;
      eState = DEVICE_STATE_IDLE;
      eHomingStatus = HOMING_STATE_IDLE;
      eHomingStatus = HOMING_STATE_INIT;
    }
    //    BOOL StartHomingOperation();
    void PerformHomingOperation();
    eDeviceState GetDeviceState() {
      return eState;
    };
    bool SetDeviceState(eDeviceState eNewState);
    void InitializeNewState();
    void MoveSteps(U32 u32Steps)
    {
      poMotorObj->MotorStart(u32Steps);
    }
    void MtrStepProcessing()
    {
      poMotorObj->MotorStepProcessing();
    }
};

class cPlatformControl : public cArmControl
{
  private:

  public:
    cPlatformControl();
};

class cAerialControl : public cArmControl
{
  private:

  public:
    cAerialControl();
};

class cDeviceController
{
  private:

  public:
    static void InitPlatformController();
    static void InitAerialController();

    static void PerformPlatformOperation();
    static void PerformAerialOperation();

    static bool SetPlatformOperation(eDeviceState eNewState);
    static bool SetAerialOperation(eDeviceState eNewState);

};
