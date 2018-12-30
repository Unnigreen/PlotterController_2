#include "stdtype.hpp"

#ifndef __SENSOR_MONITOR_HPP__
#define __SENSOR_MONITOR_HPP__

#define MAX_DEBOUNCE_COUNT                  2

#define PLATFORM_HOME_SENSOR_PIN            5
#define PLATFORM_BACK_SENSOR_PIN            6
#define AERIAL_HOME_SENSOR_PIN              7
#define AERIAL_BACK_SENSOR_PIN              8

#define PLATFORM_HOME_SENSOR_DEBOUNCE_COUNT 1
#define PLATFORM_BACK_SENSOR_DEBOUNCE_COUNT 1
#define AERIAL_HOME_SENSOR_DEBOUNCE_COUNT   1
#define AERIAL_BACK_SENSOR_DEBOUNCE_COUNT   1

typedef enum
{
  SENSOR_TYPE_INVALID = -1,
  SENSOR_TYPE_PLATFORM_HOME,
  SENSOR_TYPE_PLATFORM_BACK,
  SENSOR_TYPE_AERIAL_HOME,
  SENSOR_TYPE_AERIAL_BACK,
  SENSOR_TYPE_MAX
} eSensorType;

class cSwitchSensor
{
  private:
    BOOL bSensorStatus;
    BOOL bSensorCurStatus;
    U8 u8MaxDebounceCount;
    U8 u8CurDebounceCount;
    S32 s32SensorPin;

  public:
    //    cPositionSensor(eSensorType eType);
    cPositionSensor() {
      bSensorStatus = false;
      bSensorCurStatus = false;
      u8MaxDebounceCount = 0;
      u8CurDebounceCount = 0;
      s32SensorPin = 0;
    }
    void InitializeSensorParameters(S32 s32Pin, U8 u8DebounceCount);
    BOOL GetSensorStatus() {
      return bSensorStatus;
    }
    void PerformSensorDebounce();
};

class cSensorsMonitor
{
  private:
    static cSwitchSensor oSensors[SENSOR_TYPE_MAX];

  public:
    static BOOL InitSensorMonitor();
    static BOOL GetSensorStatus(eSensorType eType) {
      oSensors[eType].GetSensorStatus();
    }
    static void PollAndUpdateSensorStatus();
};

#endif
