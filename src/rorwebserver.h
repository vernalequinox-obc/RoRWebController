#ifndef RORWEBSERVER_H
#define RORWEBSERVER_H

#include "sensor_readings.h"
#include "settings.h"
#include "ror_controller.h"

struct RORJsonStruct
{
  String temperature;
  String humidity;
  String pressure;
  String altitudeMeter;
  String altitudeFeet;
  String RoRPosition;
};

class RORWebServer
{
public:
  RORWebServer();
  ~RORWebServer(); // destructor declaratio

  boolean connectToWiFi();
  void initWebSocket();
  void initWebServer();
  void setJsonValues(SensorBMe280_Struct aSensorReadingStrut, String aRORPosition);
  void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
  // void onRootRequest(AsyncWebServerRequest *request);
  void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
  void notifyClients();
  void cleanUpClients();
  void setSSID(String aSSID);
  void setPass(String aPass);
  void setIP(String address);
  void setSub(String address);
  void setGateway(String address);

private:
  String ssid;
  String password;
  IPAddress local_IP;
  IPAddress gateway;
  IPAddress subnet;

  boolean rorwebserverDebug;
  String sendString;
  RORJsonStruct rorjasonstrut = {"", "", "", "", "Closed"};
  RoofStatusStruct rorStatusStruct = {RORSTATUS_PIN, false};
  AsyncWebServer *rorWebServer;
  AsyncWebSocket *rorWebSocket;
};

#endif
