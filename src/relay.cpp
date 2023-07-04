#include "relay.h"

Relay::Relay()
{
    relayCurrentState = LOW;
    deviceDebug = false;
}

Relay::~Relay()
{
}

void Relay::begin(void)
{
    if (!getDeviceEnabled())
    {
        relayCurrentState = LOW;
        return;
    }
    pinMode(devicePin, OUTPUT);
}

void Relay::updateRelay(bool aRelayState)
{
    if (!getDeviceEnabled())
    {
        relayCurrentState = LOW;
        return;
    }
    relayCurrentState = aRelayState ? HIGH : LOW;
    digitalWrite(devicePin, relayCurrentState);
    if (deviceDebug)
    {
        Serial.print("Relay::updateRelay(bool aRelayState) deviceName: ");
        Serial.print(deviceName);
        Serial.print("  - devicePin: ");
        Serial.print(devicePin);
        Serial.print("  - relayCurrentState: ");
        Serial.println(relayCurrentState);
    }
}

bool Relay::getCurrentRelayState(void)
{
    if (deviceDebug)
    {
        Serial.print("Relay::getCurrentrelayState(void) deviceName: ");
        Serial.print(deviceName);
        Serial.print("  - relayCurrentState: ");
        Serial.println(relayCurrentState);
    }
    return relayCurrentState;
}
