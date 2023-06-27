#include "InputButtonLEDPulse.h"

InputButtonLEDPulse::InputButtonLEDPulse()
{
}

InputButtonLEDPulse::~InputButtonLEDPulse()
{
}

void InputButtonLEDPulse::updateButtonPin(void)
{
    InputButtonLED::updateButtonPin();
}

bool InputButtonLEDPulse::isPressed(void)
{
    return InputButtonLED::isPressed();
}

bool InputButtonLEDPulse::isReleased(void)
{
    return InputButtonLED::isReleased();
}
