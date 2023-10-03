#ifndef RORWEBSERVER_H
#define RORWEBSERVER_H

#include <inttypes.h>
#include "sensorBME280Structure.h"
#include "settings.h"
#include "ror_controller.h"

struct RORJsonStruct
{
  SensorBMe280Structure indoorBME280Struct;
  SensorBMe280Structure outdoorBME280Struct;
  ROR_Status currentRorStatus;
};


/* ALPACA COMMON DATA */
struct AscomCommonData
{
  uint32_t clientTransactionID;
  uint32_t serverTransactionID = 0;
  uint32_t clientID;
  bool boConnect;
  char actionName[20];
  char actionParameters[20];
};

const char HeaderValue[] PROGMEM = "\"Value\":";
const char HeaderCliTraId[] PROGMEM = "\"ClientTransactionID\":";
const char HeaderSerTraId[] PROGMEM = "\"ServerTransactionID\":";
const char HeaderErrN[] PROGMEM = "\"ErrorNumber\":";
const char HeaderErrM[] PROGMEM = "\"ErrorMessage\":";
const char HeaderNoErrors[] PROGMEM = "\"ErrorNumber\": 0,\"ErrorMessage\":\"\"";

class RORWebServer
{
public:
  RORWebServer();
  ~RORWebServer(); // destructor declaratio

  boolean connectToWiFi();
  void initWebSocket();
  void initWebServer();
  void setJsonValues(SensorBMe280Structure *aIndoorBME280Strut, SensorBMe280Structure *aOutdoorBME280Struct, ROR_Status *aRorStatus);
  void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
  // void onRootRequest(AsyncWebServerRequest *request);
  // void on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest);
  void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
  void notFound(AsyncWebServerRequest *request);
  void notifyClients();
  void cleanUpClients();
  void setSSID(char *aSSID);
  void setPass(char *aPass);
  void setIP(char *aAddress);
  void setSub(char *aAddress);
  void setGateway(char *aAddress);
  bool getIsOSCpulseTriggered(void);
  void resetIsOSCpulseTriggered(void);
  void AscomMethodNotImplemented(AsyncWebServerRequest *request);
  void AscomNoActions(AsyncWebServerRequest *request);

private:
  char ssid[25];
  char password[25];
  IPAddress local_IP;
  IPAddress gateway;
  IPAddress subnet;

  // Timer variables
  unsigned long previousMillis = 0;
  const long interval = 10000; // interval to wait for Wi-Fi connection (milliseconds)

  uint32_t _ServerTransactionID = 0;
  uint32_t getAndIncrementServerTransactionID(void);
  uint32_t getServerTransactionID();
  boolean rorwebserverDebug;
  RORJsonStruct rorjasonstrut;
  AsyncWebServer *rorWebServer;
  AsyncWebSocket *rorWebSocket;
  bool isOSCpulseTriggered;

  void doOSCPulseTrigger(void);
  // being of Ascom stuff
  AscomCommonData AscomData;
  void GetAscomArguments(AsyncWebServerRequest *request);
  void PutAscomArguments(AsyncWebServerRequest *request, const uint8_t *data, size_t len);
  void AscomHeaderSchema(AsyncResponseStream *response, AscomCommonData parameters);
  void AscomNoErrorSchema(AsyncResponseStream *response, bool comma = true);
  void AscomPropertyNotImplemented(AsyncWebServerRequest *request);

  void createJsonTempHumidity(AsyncResponseStream *response);
};

#endif
