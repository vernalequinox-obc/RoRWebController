#include "devicebase.h"

DeviceBase::DeviceBase()
{
  deviceDebug = false;
  isDeviceEnabled = true;
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
bool DeviceBase::getDebug(void)
{
  return deviceDebug;
}

void DeviceBase::setDeviceEnabled(bool aIsDeviceEnabled)
{
  isDeviceEnabled = aIsDeviceEnabled;
}

bool DeviceBase::getDeviceEnabled(void)
{
  return isDeviceEnabled;
}