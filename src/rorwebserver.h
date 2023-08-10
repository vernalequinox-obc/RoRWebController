#ifndef RORWEBSERVER_H
#define RORWEBSERVER_H

#include "sensorBME280Structure.h"
#include "settings.h"
#include "ror_controller.h"

struct RORJsonStruct
{
  SensorBMe280Structure indoorBME280Struct;
  SensorBMe280Structure outdoorBME280Struct;
  char RoRCurrentPosition[8];
  char IsScopeParkSafe[15];
};

class RORWebServer
{
public:
  RORWebServer();
  ~RORWebServer(); // destructor declaratio

  boolean connectToWiFi();
  void initWebSocket();
  void initWebServer();
  void setJsonValues(SensorBMe280Structure *aIndoorBME280Strut, SensorBMe280Structure *aOutdoorBME280Struct, char *aRORPosition, char *aIsScopeParkSafe);
  void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
  // void onRootRequest(AsyncWebServerRequest *request);
  void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
  void notifyClients();
  void cleanUpClients();
  void setSSID(char *aSSID);
  void setPass(char *aPass);
  void setIP(char *aAddress);
  void setSub(char *aAddress);
  void setGateway(char *aAddress);
  bool getIsOSCpulseTriggered(void);
  void resetIsOSCpulseTriggered(void);

private:
  char ssid[25];
  char password[25];
  IPAddress local_IP;
  IPAddress gateway;
  IPAddress subnet;

  boolean rorwebserverDebug;
  RORJsonStruct rorjasonstrut;
  AsyncWebServer *rorWebServer;
  AsyncWebSocket *rorWebSocket;
  bool isOSCpulseTriggered;
};

#endif
