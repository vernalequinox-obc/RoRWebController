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
  boolean debugConfigWiFiSetup;
  boolean accessPointMode;
  // Timer variables
  unsigned long previousMillis = 0;
  const long interval = 10000; // interval to wait for Wi-Fi connection (milliseconds)
  // Search for parameter in HTTP POST request

  // Variables to save values from HTML form
  String ssid;
  String pass;
  String ip;
  String sub;
  String gateway;
  // File paths to save input values permanently
  const char *ssidPath = "/ssid.txt";
  const char *passPath = "/pass.txt";
  const char *ipPath = "/ip.txt";
  const char *subPath = "/sub.txt";
  const char *gatewayPath = "/gateway.txt";

  IPAddress localIP;
  IPAddress localGateway;
  IPAddress subnet;

  IPAddress defaultAP_localIP;
  IPAddress defaultAP_localGateway;
  IPAddress defaultAP_subnet;

  String readFile(fs::FS &fs, const char *path);
  void writeFile(fs::FS &fs, const char *path, const char *message);
  String SendHTML();

};

#endif
