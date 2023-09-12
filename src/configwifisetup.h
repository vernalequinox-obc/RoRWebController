#ifndef CONFIGWIFISETUP_H
#define CONFIGWIFISETUP_H
#include "settings.h"

#define PARAM_SSID "ssid"
#define PARAM_PASS "pass"
#define PARAM_IP "ip"
#define PARAM_SUB "sub"
#define PARAM_GATEWAY "gateway"

// File paths to save input values permanently

class ConfigWiFiSetup
{
public:
  ConfigWiFiSetup();
  ~ConfigWiFiSetup();
  AsyncWebServer serverAP;

  bool isThereWiFiSetting();
  bool runAPWebServerSetup();
  void clearWiFiSettings();
  void notFound(AsyncWebServerRequest *request);
  void writeFileWiFiSettings(fs::FS &fs, const char *path, char *message);
  void readFileWiFiSettings(fs::FS &fs, const char *path, char *destination, size_t maxSize);

  bool debugConfigWiFiSetup;
  char *getSSID();
  char *getPass();
  char *getIP();
  char *getSub();
  char *getGateway();
  bool getDebugConfigWiFiSetup();

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

  char ssidPath[11];
  char passPath[11];
  char ipPath[8];
  char subPath[9];
  char gatewayPath[13];

  char ssid[32];
  char pass[64];
  char ip[16];
  char sub[16];
  char gateway[16];
};

#endif
