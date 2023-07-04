#ifndef INPUTBUTTONLEDPULSE_H
#define INPUTBUTTONLEDPULSE_H

#include <Arduino.h>
#include "settings.h"
#include "InputButtonLED.h"

const uint32_t TRIGGER_DURATION = 500;            // Trigger duration in milliseconds
const uint32_t DISABLE_DELAY_NONE = 0;            // Disable delay in milliseconds
const uint32_t DISABLE_DELAY = 3000;              // Disable delay in milliseconds
const uint32_t HOLD_BUTTON_DOWN_THRESHOLD = 2000; // Adjust how long a button must be held before its state changes

class InputButtonLEDPulse : public InputButtonLED
{
public:
    InputButtonLEDPulse();
    ~InputButtonLEDPulse();

    void updateButtonPin(void);
    bool isPressed(void);
    bool isReleased(void);
    void setPusleTriggerDuration(uint32_t aPulseTriggerDuration);
    bool isPulseTriggered(void);
    void setDisableDurationPostPulse(uint32_t aDisableDurationPostPulse);

private:
    uint32_t pulseTriggerDuration;
    uint32_t disableDurationPostPulse;
    uint32_t pulseStartTime;
    bool pulseTriggered;
    uint32_t disableStartTime;
    bool isPostPulseDisable;
};

#endif
