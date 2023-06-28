#include "InputButtonLEDPulse.h"

InputButtonLEDPulse::InputButtonLEDPulse()
{
    pulseTriggerDuration = 500;
    disableDurationPostPulse = 2000;
    pulseStartTime = 0;
    pulseTriggered = false;
    disableStartTime = 0;
    isPostPulseDisable = false;
}

InputButtonLEDPulse::~InputButtonLEDPulse()
{
}

void InputButtonLEDPulse::updateButtonPin(void)
{
    InputButtonLED::updateButtonPin();
    InputButtonLED::updateButtonPin();

    if (isPostPulseDisable)
    {
        if ((millis() - disableStartTime) >= disableDurationPostPulse)
        {
            isPostPulseDisable = false;
        }
    }
    else if (isPressed() && !pulseTriggered)
    {
        pulseStartTime = millis();
        pulseTriggered = true;
        updateLed(true); // Turn on the LED during the pulse
    }
    else if (pulseTriggered && (millis() - pulseStartTime) >= pulseTriggerDuration)
    {
        pulseTriggered = false;
        updateLed(false); // Turn off the LED after the pulse duration
        isPostPulseDisable = true;
        disableStartTime = millis();
    }
}

bool InputButtonLEDPulse::isPressed(void)
{
    return InputButtonLED::isPressed();
}

bool InputButtonLEDPulse::isReleased(void)
{
    return InputButtonLED::isReleased();
}

void InputButtonLEDPulse::setPusleTriggerDuration(uint32_t aPulseTriggerDuration)
{
    pulseTriggerDuration = aPulseTriggerDuration;
}

void InputButtonLEDPulse::setDisableDurationPostPulse(uint32_t aDisableDurationPostPulse)
{
    disableDurationPostPulse = aDisableDurationPostPulse;
}

bool InputButtonLEDPulse::isPulseTriggered(void)
{
    return pulseTriggerState;
}
