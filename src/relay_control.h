#ifndef RELAYCONTROL_H
#define RELAYCONTROL_H

#include <Arduino.h>
#include "relay.h"
#include "settings.h"

class Relay_Control
{
public:
    Relay_Control();
    ~Relay_Control();

    void setScopeMountParkSafeRelay(bool aChoice);
    void setOscTriggerRelay(bool aChoice);
    uint16_t getOscTriggerRelayCurrentPinState(void);
    uint16_t getScopeMountParkSafeRelayCurrentState(void);
    void setOSCRelayPulseTimeDuration(uint32_t aPulseTimeDuration);
    void updateOSCRelayPulseTime(void);
    Relay oscTriggerRelay;
    Relay scopeMountParkSafeRelay;

private:
    uint16_t oscTriggerRelayCurrentState;
    uint16_t scopeMountParkSafeRelayCurrentState;

    uint32_t pulseTimeDuration;
    uint32_t pulseStartTime;
    uint16_t pulseTriggerState;
    bool pulseTriggered;
    bool isScopeMountParkSafe;
};

#endif