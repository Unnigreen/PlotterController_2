#include "Arduino.h"
#include "NotificationControl.hpp"

U32 cBuzzerControl::u32Iteration = 0U;
U32 cBuzzerControl::u32RunCount = 0U;

eLedPattern cLedControl::ePattern = LED_PATTERN_SYSYTEM_BUSY;
U32 cLedControl::u32RunCount;
cLed cLedControl::oLedN1(LED_BUILTIN);

void cBuzzerControl::SetBuzzerPattern(eBuzzerPattern ePattern)
{
  u32RunCount = BUZZER_TOGGLE_COUNT;
  if (ePattern != 0) {
    u32Iteration = ((U32)ePattern * 2) - 1;
    TurnOnBuzzer();
  }
  else {
    (u32Iteration = 0);
  }
}

void cBuzzerControl::BuzzerOperation()
{
  if (u32Iteration != 0)  {
    if (u32RunCount-- == 0)    {
      u32Iteration--;
      ToggleBuzzer();
      u32RunCount = BUZZER_TOGGLE_COUNT;
    }
  }
  else {
    TurnOffBuzzer();
  }
}

void cLedControl::LedOperation()
{
  switch (ePattern)  {
    case LED_PATTERN_SYSYTEM_IDLE: {
        oLedN1.TurnOnLed();
        break;
      }
    case LED_PATTERN_SYSYTEM_BUSY: {
        if (u32RunCount-- == 0) {
          oLedN1.ToggleLed();
          u32RunCount = LED_TOGGLE_COUNT;
        }
        break;
      }
    default: {
        break;
      }
  }
}
