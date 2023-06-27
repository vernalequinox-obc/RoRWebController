#include "InputButtonLED.h"

InputButtonLED::InputButtonLED()
{
}

InputButtonLED::~InputButtonLED()
{
}

void InputButtonLED::begin(void)
{
  InputButton::begin();
  LedLight::begin();
}

bool InputButtonLED::isPressed(void)
{
  LedLight::updateLed(InputButton::isPressed());
  return InputButton::isPressed();
}

bool InputButtonLED::isReleased(void)
{
  LedLight::updateLed(InputButton::isReleased());
  return InputButton::isReleased();
}

void InputButtonLED::setButtonLedPin(uint8_t aButtonPin, uint8_t aLedPin)
{
  InputButton::setButtonPin(aButtonPin);
  LedLight::setLedPin(aLedPin);
}
