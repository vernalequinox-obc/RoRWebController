#include "LedLight.h"

LedLight::LedLight()
{
    ledPin = 0;
    ledCurrentState = LOW;
}

LedLight::~LedLight()
{
}

void LedLight::setLedPin(uint8_t aLedPin)
{
    ledPin = aLedPin;
}

uint8_t LedLight::getLedPin()
{
    return ledPin;
}

void LedLight::begin()
{
    if (!getDeviceEnabled())
    {
        ledCurrentState = LOW;
        return;
    }
    pinMode(ledPin, OUTPUT);
}

void LedLight::updateLed(bool aLedState)
{
    if (!getDeviceEnabled())
    {
        ledCurrentState = LOW;
        return;
    }
    ledCurrentState = aLedState ? HIGH : LOW;
    digitalWrite(ledPin, ledCurrentState);
    if (getDebug())
    {
        Serial.print("LedLight::updateLed(bool aOn) deviceName: ");
        Serial.print(getDeviceName());
        Serial.print("  - ledCurrentState: ");
        Serial.println(ledCurrentState);
    }
}

bool LedLight::getCurrentLedState(void)
{
    if (getDebug())
    {
        Serial.print("LedLight::getCurrentLedState(void) deviceName: ");
        Serial.print(getDeviceEnabled());
        Serial.print("  - ledCurrentState: ");
        Serial.println(ledCurrentState);
    }
    return ledCurrentState;
}
