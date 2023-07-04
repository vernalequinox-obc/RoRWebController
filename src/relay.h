#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>
#include "devicebase.h"

class Relay : public DeviceBase
{
public:
    Relay();
    ~Relay();

    void begin(void);
    void updateRelay(bool aRelayState);
    bool getCurrentRelayState(void);

private:
    uint16_t relayCurrentState;
};

#endif