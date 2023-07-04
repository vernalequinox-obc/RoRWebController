#ifndef DEVICEBASE_H
#define DEVICEBASE_H

#include <Arduino.h>

class DeviceBase
{
public:
    DeviceBase();
    ~DeviceBase();
    void setDevicePin(uint8_t aDevicePin);
    uint8_t getDevicePin(void);
    void setDeviceName(const char *aName);
    char *getDeviceName(void);
    void setDebug(bool aDebug);
    bool getDebug(void);
    void setDeviceEnabled(bool aIsDeviceEnabled);
    bool getDeviceEnabled(void);

protected:
    char deviceName[50];
    bool deviceDebug;
    bool isDeviceEnabled;
    uint8_t devicePin;
};
#endif