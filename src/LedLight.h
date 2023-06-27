#ifndef LEDLIGHT_H
#define LEDLIGHT_H

#include <Arduino.h>
#include "devicebase.h"

class LedLight : public DeviceBase
{
public:
    LedLight();
    ~LedLight();

    void begin();
    void updateLed(bool aOn);
    bool getCurrentLedState(void);
    void setLedPin(uint8_t aLedPin);
    uint8_t getLedPin();

private:
    uint8_t ledPin;
    uint16_t ledCurrentState;
};

#endif
