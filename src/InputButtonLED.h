#ifndef INPUTBUTTONLED_H
#define INPUTBUTTONLED_H

#include <Arduino.h>
#include "InputButton.h"
#include "LedLight.h"

class InputButtonLED : public InputButton, public LedLight
{
public:
    InputButtonLED();
    ~InputButtonLED();

    void begin(void);
    bool isPressed(void);
    bool isReleased(void);

    void setButtonLedPin(uint8_t aButtonPin, uint8_t aLedPin);

private:

};

#endif

