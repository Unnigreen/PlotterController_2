#include "stdtype.hpp"

#define BUZZER_PIN  LED_BUILTIN
#define BUZZER_TOGGLE_COUNT 10
#define LED_TOGGLE_COUNT 3

typedef enum
{
  BUZZER_PATTERN_INVALID = 0,
  BUZZER_PATTERN_1 = 1,
  BUZZER_PATTERN_2 = 2,
  BUZZER_PATTERN_3 = 3,
  BUZZER_PATTERN_4 = 4,
} eBuzzerPattern;

typedef enum
{
  LED_PATTERN_INVALID = 0,
  LED_PATTERN_SYSYTEM_BUSY = 1,
  LED_PATTERN_SYSYTEM_IDLE = 2,
} eLedPattern;

class cBuzzerControl
{
  private:
    static U32 u32Iteration;
    static U32 u32RunCount;

  public:
    static void TurnOnBuzzer() {
      digitalWrite(BUZZER_PIN, HIGH);
    }
    static void TurnOffBuzzer(){
      digitalWrite(BUZZER_PIN, LOW);
    }
    static void ToggleBuzzer(){
      digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));
    }
    static void SetBuzzerPattern(eBuzzerPattern ePattern);
    static void BuzzerOperation();
};

class cLed
{
  private:
    S32 s32Pin;
    cLed();

  public:
    cLed(S32 s32PinNum) {
      s32Pin = s32PinNum;
    }
    void TurnOnLed() {
      digitalWrite(s32Pin, HIGH);
    }
    void TurnOffLed() {
      digitalWrite(s32Pin, LOW);
    }
    void ToggleLed() {
      digitalWrite(s32Pin, !digitalRead(s32Pin));
    }
};


class cLedControl
{
  private:
    static eLedPattern ePattern;
    static U32 u32RunCount;
    static cLed oLedN1;

  public:
    static void SetLedPattern(eLedPattern ePtn) {
      ePattern = ePtn;
      u32RunCount = LED_TOGGLE_COUNT;
    }
    static void LedOperation();
};
