#include "devicebase.h"

DeviceBase::DeviceBase()
{
  deviceDebug = false;
}

DeviceBase::~DeviceBase()
{
}

void DeviceBase::setDeviceName(const char *aName)
{
  strncpy(this->deviceName, aName, sizeof(this->deviceName) - 1);
  this->deviceName[sizeof(this->deviceName) - 1] = '\0'; // Null-terminate the string
}

char *DeviceBase::getDeviceName(void)
{
  return deviceName;
}

void DeviceBase::setDebug(bool aDebug)
{
  deviceDebug = aDebug;
}
bool DeviceBase::isDebug(void)
{
  return deviceDebug;
}