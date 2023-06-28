#include "InputButton.h"

InputButton::InputButton()
{
  btnPin = 0;
  debounceTime = 50;
  currentPinState = LOW;
  lastCurrentPinState = HIGH;
  lastDebounceTime = 0;
}

InputButton::~InputButton()
{
}

void InputButton::setDebounceTime(uint32_t aDebounceTime)
{
  debounceTime = aDebounceTime;
}

void InputButton::setButtonPin(uint8_t aBtnPin)
{
  btnPin = aBtnPin;
}

uint8_t InputButton::getButtonPin(void)
{
  return btnPin;
}

void InputButton::begin(void)
{
  if (InputButton::getDebug())
  {
    Serial.print("InputButton::begin() deviceName: ");
    Serial.print(getDeviceName());
    Serial.print("  BtnPin: ");
    Serial.println(btnPin);
  }
  currentPinState = LOW;
  if (!getDeviceEnabled())
  {
    return;
  }
  pinMode(btnPin, INPUT_PULLUP);
}

bool InputButton::getCurrentPinState(void)
{
  return currentPinState;
}

void InputButton::updateButtonPin(void)
{
  if (!getDeviceEnabled())
  {
    currentPinState = LOW;
    return;
  }
  int readingPinState = digitalRead(btnPin);

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
