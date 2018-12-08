#include "Arduino.h"
#include "SensorMonitor.hpp"

cSwitchSensor cSensorsMonitor::oSensors[SENSOR_TYPE_MAX];

void cSwitchSensor::InitializeSensorParameters(S32 s32Pin, U8 u8DebounceCount) {
  bSensorStatus = false;
  bSensorCurStatus = false;
  s32SensorPin = s32Pin;
  u8MaxDebounceCount = u8DebounceCount;
  pinMode(s32SensorPin, INPUT_PULLUP);
}

void cSwitchSensor::PerformSensorDebounce()
{
  bSensorCurStatus = !digitalRead(s32SensorPin);
  bSensorStatus != bSensorCurStatus ? u8CurDebounceCount++ : u8CurDebounceCount = 0;

  if (u8CurDebounceCount > u8MaxDebounceCount)
  {
    bSensorStatus = !bSensorStatus;
    u8CurDebounceCount = 0;
  }
}

BOOL cSensorsMonitor::InitSensorMonitor()
{
  U8 eType = (U8)SENSOR_TYPE_INVALID;
  S32 s32SensorPin;
  U8 u8MaxDebounceCount;

  for (eType++; eType < (U8)SENSOR_TYPE_MAX; eType++)
  {
    switch (eType)
    {
      case SENSOR_TYPE_PLATFORM_HOME: {
          s32SensorPin = PLATFORM_HOME_SENSOR_PIN;
          u8MaxDebounceCount = PLATFORM_HOME_SENSOR_DEBOUNCE_COUNT;
          oSensors[eType].InitializeSensorParameters(s32SensorPin, u8MaxDebounceCount);
          break;
        }
      case SENSOR_TYPE_PLATFORM_BACK: {
          s32SensorPin = PLATFORM_BACK_SENSOR_PIN;
          u8MaxDebounceCount = PLATFORM_BACK_SENSOR_DEBOUNCE_COUNT;
          oSensors[eType].InitializeSensorParameters(s32SensorPin, u8MaxDebounceCount);
          break;
        }
      case SENSOR_TYPE_AERIAL_HOME: {
          s32SensorPin = AERIAL_HOME_SENSOR_PIN;
          u8MaxDebounceCount = AERIAL_HOME_SENSOR_DEBOUNCE_COUNT;
          oSensors[eType].InitializeSensorParameters(s32SensorPin, u8MaxDebounceCount);
          break;
        }
      case SENSOR_TYPE_AERIAL_BACK: {
          s32SensorPin = AERIAL_BACK_SENSOR_PIN;
          u8MaxDebounceCount = AERIAL_BACK_SENSOR_DEBOUNCE_COUNT;
          oSensors[eType].InitializeSensorParameters(s32SensorPin, u8MaxDebounceCount);
          break;
        }
      default: {
          break;
        }
    }
  }
}

void cSensorsMonitor::PollAndUpdateSensorStatus()
{
  U8 eType = (U8)SENSOR_TYPE_INVALID;

  for (eType++; eType < (U8)SENSOR_TYPE_MAX; eType++)
  {
    oSensors[eType].PerformSensorDebounce();
  }
}
