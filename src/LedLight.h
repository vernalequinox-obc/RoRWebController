#ifndef LEDLIGHT_H
#define LEDLIGHT_H

#include <Arduino.h>
#include "devicebase.h"

class LedLight : public DeviceBase
{
public:
    LedLight();
    ~LedLight();

    void begin(void);
    void updateLed(bool aOn);
    bool getCurrentLedState(void);

private:
    uint16_t ledCurrentState;
};

#endif
