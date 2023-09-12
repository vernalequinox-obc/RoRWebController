#ifndef INPUTBUTTONLEDPULSE_H
#define INPUTBUTTONLEDPULSE_H

#include <Arduino.h>
#include "settings.h"
#include "InputButtonLED.h"

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
