#include "relay_control.h"

Relay_Control::Relay_Control()
{
    pulseTimeDuration = RELAY_TRIGGER_PULSE_DURATION;
    pulseTriggered = false;
    oscTriggerRelayCurrentState = LOW;
    isScopeMountParkSafe = false;

    oscTriggerRelay.setDeviceName("oscTriggerRelay");
    oscTriggerRelay.setDevicePin(OCS_PULSE_RELAY_OUTPUTPIN);
    oscTriggerRelay.setDeviceEnabled(true);
    // oscTriggerRelay.setDebug(true);

    scopeMountParkSafeRelay.setDeviceName("scopeMountParkSafeRelay");
    scopeMountParkSafeRelay.setDevicePin(SCOPE_MOUNT_PARK_SAFE_RELAY_OUTPUTPIN);
    scopeMountParkSafeRelay.setDeviceEnabled(true);
    scopeMountParkSafeRelay.updateRelay(LOW);
    // scopeMountParkSafeRelay.setDebug(true);

    oscTriggerRelay.begin();
    scopeMountParkSafeRelay.begin();
}

Relay_Control::~Relay_Control()
{
}

void Relay_Control::setOSCRelayPulseTimeDuration(uint32_t aPulseTimeDuration)
{
    pulseTimeDuration = aPulseTimeDuration;
}

void Relay_Control::updateOSCRelayPulseTime(void)
{
    if (oscTriggerRelay.getDebug())
    {
        Serial.print("Relay_Control::updateOSCRelayPulseTime() pulseTriggered: ");
        Serial.println(pulseTriggered);
    }
    if (isScopeMountParkSafe)
    {
        if (!pulseTriggered)
        {
            pulseStartTime = millis();
            pulseTriggered = true;
            oscTriggerRelayCurrentState = HIGH;
            if (oscTriggerRelay.getDebug())
            {
                Serial.print("Relay_Control::updateOSCRelayPulseTime() -> Turn on the relay during the pulse. oscTriggerRelayCurrentState: ");
                Serial.println(oscTriggerRelayCurrentState);
            }
            oscTriggerRelay.updateRelay(oscTriggerRelayCurrentState); // Turn on the relay during the pulse
        }
        else if (pulseTriggered && (millis() - pulseStartTime) >= pulseTimeDuration)
        {
            pulseTriggered = false;
            oscTriggerRelayCurrentState = LOW;
            if (oscTriggerRelay.getDebug())
            {
                Serial.print("Relay_Control::updateOSCRelayPulseTime() -> Turn off the relay after the pulse duration.  oscTriggerRelayCurrentState: ");
                Serial.println(oscTriggerRelayCurrentState);
            }
            oscTriggerRelay.updateRelay(oscTriggerRelayCurrentState); // Turn off the relay after the pulse duration
        }
    }
}

void Relay_Control::setScopeMountParkSafeRelay(bool aChoice)
{
    isScopeMountParkSafe = aChoice;
    if (scopeMountParkSafeRelay.getDebug())
    {
        Serial.print("Relay_Control::setScopeMountParkSafeRelay(bool aChoice) isScopeMountParkSafe: ");
        Serial.println(isScopeMountParkSafe);
    }
    if (!scopeMountParkSafeRelay.getDeviceEnabled())
    {
        scopeMountParkSafeRelayCurrentState = LOW;
        return;
    }
    scopeMountParkSafeRelayCurrentState = isScopeMountParkSafe ? HIGH : LOW;
    scopeMountParkSafeRelay.updateRelay(scopeMountParkSafeRelayCurrentState);
    if (scopeMountParkSafeRelay.getDebug())
    {
        Serial.print("Relay_Control::setScopeMountParkSafeRelay(bool aChoice) deviceName: ");
        Serial.print(scopeMountParkSafeRelay.getDeviceName());
        Serial.print("  - scopeMountParkSafeRelayCurrentState: ");
        Serial.println(scopeMountParkSafeRelayCurrentState);
    }
}

uint16_t Relay_Control::getOscTriggerRelayCurrentPinState(void)
{
    return oscTriggerRelayCurrentState;
}

uint16_t Relay_Control::getScopeMountParkSafeRelayCurrentState(void)
{
    return scopeMountParkSafeRelayCurrentState;
}