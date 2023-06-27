#ifndef INPUTBUTTON_H
#define INPUTBUTTON_H

#include <Arduino.h>
#include "devicebase.h"

class InputButton : public DeviceBase
{
public:
  InputButton();
  ~InputButton();

  void setButtonPin(uint8_t aBtnPin);
  void setDebounceTime(uint32_t aDebounceTime);

  uint8_t getButtonPin(void);
  bool getCurrentPinState(void);
  bool isPressed(void);
  bool isReleased(void);

  void begin(void);
  void updateButtonPin(void);
  void setIsEnabled(bool aChoice);

protected:
  uint8_t btnPin;
  uint32_t debounceTime;
  uint32_t lastDebounceTime;
  uint16_t currentPinState;
  uint16_t lastCurrentPinState;
  bool isEnabled;

private:
};

#endif
