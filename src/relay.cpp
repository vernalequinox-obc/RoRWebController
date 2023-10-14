#include "relay.h"

Relay::Relay()
{
    relayCurrentState = LOW;
    updateRelay(false);
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
    // relayCurrentState = aRelayState ? HIGH : LOW;
    relayCurrentState = aRelayState ? LOW : HIGH;
    digitalWrite(devicePin, relayCurrentState);
}

bool Relay::getCurrentRelayState(void)
{
    return relayCurrentState;
}
