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
    pinMode(ledPin, OUTPUT);
}

void LedLight::updateLed(bool aLedState)
{
    ledCurrentState = aLedState ? HIGH : LOW;
    digitalWrite(ledPin, ledCurrentState);
    if (deviceDebug)
    {
        Serial.print("LedLight::updateLed(bool aOn) deviceName: ");
        Serial.print(deviceName);
        Serial.print("  - ledCurrentState: ");
        Serial.println(ledCurrentState);
    }
}

bool LedLight::getCurrentLedState(void)
{
    if (deviceDebug)
    {
        Serial.print("LedLight::getCurrentLedState(void) deviceName: ");
        Serial.print(deviceName);
        Serial.print("  - ledCurrentState: ");
        Serial.println(ledCurrentState);
    }
    return ledCurrentState;
}
