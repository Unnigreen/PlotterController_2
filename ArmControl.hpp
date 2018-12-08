#include "stdtype.hpp"
#include "SensorMonitor.hpp"
#include "MotorControl.hpp"

#define MAX_HOMING_TIMOUT_COUNT 10000U

typedef enum
{
  HOMING_STATE_IDLE = 0,
  HOMING_STATE_START,
  HOMING_STATE_WAITING_HOME,
  HOMING_STATE_END,
  HOMING_STATE_TIMEOUT,
  HOMING_STATE_MAX,
} eHomingState;

class cArmControl
{
  private:
    cMotorAbs * poMotorObj;
    BOOL bIsHomingDone;
    eHomingState eHomingStatus;
    U32 u32HomingTimeoutCount;

    cSwitchSensor * oHomeSensor;
    cSwitchSensor * oBackSensor;

    void ProcessHomingStart();
    void ProcessHomingWait();
    void ProcessHomingEnd();
    void ProcessHomingTimeout();

  public:
    cArmControl() {
      poMotorObj = 0;
      oHomeSensor = 0;
      oBackSensor = 0;
      bIsHomingDone = false;
      u32HomingTimeoutCount = 0;
      eHomingStatus = HOMING_STATE_IDLE;
    }
    void SetSenorObject(cSwitchSensor * poHomeSnsr, cSwitchSensor * poBackSnsr){
      oHomeSensor = poHomeSnsr;
      oBackSensor = poBackSnsr;
    }
    void SetMotorObject(cMotorAbs * poMtrObj) {
      poMotorObj = poMtrObj;
    }
    cMotorAbs * GetMototObject() {
      return poMotorObj;
    }
    BOOL StartHomingOperation();
    void PerformHomingOperation();
    
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
