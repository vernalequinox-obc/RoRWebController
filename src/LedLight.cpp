#include "ledlight.h"

LedLight::LedLight()
{
    ledCurrentState = LOW;
    deviceDebug = false;
}

LedLight::~LedLight()
{
}

void LedLight::begin(void)
{
    if (!getDeviceEnabled())
    {
        ledCurrentState = LOW;
        return;
    }
    pinMode(devicePin, OUTPUT);
}

void LedLight::updateLed(bool aLedState)
{
    if (!getDeviceEnabled())
    {
        ledCurrentState = LOW;
        return;
    }
    ledCurrentState = aLedState ? HIGH : LOW;
    digitalWrite(devicePin, ledCurrentState);
    if (deviceDebug)
    {
        Serial.print("LedLight::updateLed(bool aOn) deviceName: ");
        Serial.print(deviceName);
        Serial.print("  - devicePin: ");
        Serial.print(devicePin);        
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
