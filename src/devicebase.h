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
    bool isDebug(void);

protected:
    char deviceName[50];
    bool deviceDebug;

private:
};
#endif