#ifndef DEVICEBASE_H
#define DEVICEBASE_H

#include <Arduino.h>

class DeviceBase
{
public:
    DeviceBase();
    ~DeviceBase();
    void setDeviceName(const char *aName);
    char *getDeviceName(void);
    void setDebug(bool aDebug);
    bool getDebug(void);
    void setDeviceEnabled(bool aIsDeviceEnabled);
    bool getDeviceEnabled(void);

private:
    char deviceName[50];
    bool deviceDebug;
    bool isDeviceEnabled;
};
#endif