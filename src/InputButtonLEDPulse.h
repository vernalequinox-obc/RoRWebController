#ifndef INPUTBUTTONLEDPULSE_H
#define INPUTBUTTONLEDPULSE_H

#include <Arduino.h>
#include "settings.h"
#include "InputButtonLED.h"

const uint32_t TRIGGER_DURATION = 500; // Trigger duration in milliseconds
const uint32_t DISABLE_DELAY_NONE = 0; // Disable delay in milliseconds
const uint32_t DISABLE_DELAY = 3000;   // Disable delay in milliseconds
const uint32_t HOLD_BUTTON_DOWN_THRESHOLD = 2000;   // Adjust how long a button must be held before its state changes

class InputButtonLEDPulse : public InputButtonLED
{
public:
    InputButtonLEDPulse();
    ~InputButtonLEDPulse();

    void updateButtonPin(void);
    bool isPressed(void);
    bool isReleased(void);

private:

};

#endif
