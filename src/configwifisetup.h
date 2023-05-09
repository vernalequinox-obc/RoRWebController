#ifndef CONFIGWIFISETUP_H
#define CONFIGWIFISETUP_H
#include "settings.h"

class ConfigWiFiSetup
{
public:
  ConfigWiFiSetup();
  ~ConfigWiFiSetup();
  boolean isThereWiFiSetting();
  boolean runAPWebServerSetup();
  void clearWiFiSettings();
  String getSSID();
  String getPass();
  String getIP();
  String getSub();
  String getGateway();

private:

  boolean accessPointMode;
  // Timer variables
  unsigned long previousMillis = 0;
  const long interval = 10000; // interval to wait for Wi-Fi connection (milliseconds)
  // Search for parameter in HTTP POST request

  // Variables to save values from HTML form



  IPAddress localIP;
  IPAddress localGateway;
  IPAddress subnet;

  IPAddress defaultAP_localIP;
  IPAddress defaultAP_localGateway;
  IPAddress defaultAP_subnet;

};

#endif
