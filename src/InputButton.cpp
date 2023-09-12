#include "InputButton.h"

InputButton::InputButton()
{
  debounceTime = 50;
  currentPinState = LOW;
  lastCurrentPinState = HIGH;
  lastDebounceTime = 0;
  deviceDebug = false;
}

InputButton::~InputButton()
{
}

void InputButton::setDebounceTime(uint32_t aDebounceTime)
{
  debounceTime = aDebounceTime;
}

void InputButton::begin(void)
{
  if (InputButton::getDebug())
  {
    Serial.print("InputButton::begin() deviceName: ");
    Serial.print(getDeviceName());
    Serial.print("  Button DevicePin: ");
    Serial.println(getDevicePin());
  }
  currentPinState = LOW;
  if (!getDeviceEnabled())
  {
    return;
  }
  pinMode(getDevicePin(), INPUT_PULLUP);
}

uint16_t InputButton::getCurrentPinState(void)
{
  return currentPinState;
}

int InputButton::getDigitalRead(void)
{
  return digitalRead(getDevicePin());
}

void InputButton::updateButtonPin(void)
{
  if (!getDeviceEnabled())
  {
    currentPinState = LOW;
    return;
  }
  int readingPinState = digitalRead(getDevicePin());

  // reset the bounce timer
  if (readingPinState != lastCurrentPinState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceTime && !readingPinState)
  {
    if (!readingPinState)
    {
      currentPinState = HIGH;
    }
  }
  else
  {
    currentPinState = LOW;
  }
  if (getDebug())
  {
    Serial.print("InputButton::updateButtonPin() deviceName: ");
    Serial.print(getDeviceName());
    Serial.print("  - lastCurrentPinState: ");
    Serial.print(lastCurrentPinState);
    Serial.print("  - readingPinState: ");
    Serial.print(readingPinState);
    Serial.print("  - currentPinState: ");
    Serial.println(currentPinState);
  }
  lastCurrentPinState = readingPinState;
}

bool InputButton::isPressed(void)
{

  return currentPinState;
}

bool InputButton::isReleased(void)
{
  return currentPinState == 0xffff;
}
