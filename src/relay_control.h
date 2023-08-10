#ifndef RELAYCONTROL_H
#define RELAYCONTROL_H

#include <Arduino.h>
#include "relay.h"
#include "settings.h"
/*
// Relays
#define SCOPE_MOUNT_PARK_SAFE_RELAY_OUTPUTPIN 27 // GIOP27  Scope safe relay for Aleko. This relay is energerized (outpin LOW engaged) in the safe posistion.
                                                 //         The relay when energized will complete the motor circuit. When non-energized the motor does not function.
#define OCS_PULSE_RELAY_OUTPUTPIN 26             // GIOP26  Relay toggles a momentary pulse acking like a key bod or momentery button to be used with Aleko.

*/
#define RELAY_TRIGGER_PULSE_DURATION 2000

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