#ifndef CONFIGWIFISETUP_H
#define CONFIGWIFISETUP_H
#include "settings.h"

#define PARAM_SSID "ssid"
#define PARAM_PASS "pass"
#define PARAM_IP "ip"
#define PARAM_SUB "sub"
#define PARAM_GATEWAY "gateway"

// File paths to save input values permanently
static constexpr char ssidPath[] = "/ssid.txt";
static constexpr char passPath[] = "/pass.txt";
static constexpr char ipPath[] = "/ip.txt";
static constexpr char subPath[] = "/sub.txt";
static constexpr char gatewayPath[] = "/gateway.txt";

static char ssid[32];
static char pass[64];
static char ip[16];
static char sub[16];
static char gateway[16];

class ConfigWiFiSetup
{
public:
  ConfigWiFiSetup();
  ~ConfigWiFiSetup();
  boolean isThereWiFiSetting();
  boolean runAPWebServerSetup();
  void clearWiFiSettings();

  static void notFound(AsyncWebServerRequest *request);
  static void saveWiFiSettings();
  static void writeFile(fs::FS &fs, const char *path, char *message);
  static void readFile(fs::FS &fs, const char *path, char *destination, size_t maxSize);

  bool debugConfigWiFiSetup = true;
  char *getSSID();
  char *getPass();
  char *getIP();
  char *getSub();
  char *getGateway();

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
