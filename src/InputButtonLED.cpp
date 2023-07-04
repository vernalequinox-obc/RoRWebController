#include "InputButtonLED.h"

InputButtonLED::InputButtonLED()
{
  setDeviceEnabledLED(true);
  setDeviceEnabledButton(true);
  setDebugButton(false);
  setDebugLED(false);
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
  InputButton::setDevicePin(aButtonPin);
  LedLight::setDevicePin(aLedPin);
}

void InputButtonLED::setDeviceLEDName(const char *aName)
{
  LedLight::setDeviceName(aName);
}
char *InputButtonLED::getDeviceLEDName(void)
{
  return LedLight::getDeviceName();
}
void InputButtonLED::setDeviceButtonName(const char *aName)
{
  InputButton::setDeviceName(aName);
}
char *InputButtonLED::getDeviceButtonName(void)
{
  return InputButton::getDeviceName();
}

void InputButtonLED::setDebugLED(bool aDebug)
{
  LedLight::setDebug(aDebug);
}
bool InputButtonLED::getDebugLED(void)
{
  return LedLight::getDebug();
}
void InputButtonLED::setDeviceEnabledLED(bool aIsDeviceEnabled)
{
  LedLight::setDeviceEnabled(aIsDeviceEnabled);
}
bool InputButtonLED::getDeviceEnabledLED(void)
{
  return LedLight::getDeviceEnabled();
}

void InputButtonLED::setDebugButton(bool aDebug)
{
  InputButton::setDebug(aDebug);
}
bool InputButtonLED::getDebugButton(void)
{
  return InputButton::getDebug();
}
void InputButtonLED::setDeviceEnabledButton(bool aIsDeviceEnabled)
{
  InputButton::setDeviceEnabled(aIsDeviceEnabled);
}
bool InputButtonLED::getDeviceEnabledButton(void)
{
  return InputButton::getDeviceEnabled();
}
