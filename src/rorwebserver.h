#ifndef RORWEBSERVER_H
#define RORWEBSERVER_H

#include "sensorBME280Structure.h"
#include "settings.h"
#include "ror_controller.h"

struct RORJsonStruct
{
  SensorBMe280Structure indoorBME280Struct;
  SensorBMe280Structure outdoorBME280Struct;
  ROR_Status currentRorStatus;
};

typedef struct
{
  bool idExist;
  int id; /* used for switch ID */
  bool stateExist;
  bool state; /* used for setswitch */
  bool intValueExist;
  int intValue; /* used for setswitchvalue */
  bool nameExist;
  String name; /* used for set switch name */
} switchAlpacaParameters;

/* ALPACA COMMON DATA */
struct AlpacaCommonData
{
  uint32_t clientTransactionID;
  uint32_t serverTransactionID = 0;
  uint32_t clientID;
  bool boConnect;
  switchAlpacaParameters switches;
};

const char Alp_Value[] PROGMEM = "\"Value\":";
const char Alp_CliTraId[] PROGMEM = "\"ClientTransactionID\":";
const char Alp_SerTraId[] PROGMEM = "\"ServerTransactionID\":";
const char Alp_ErrN[] PROGMEM = "\"ErrorNumber\":";
const char Alp_ErrM[] PROGMEM = "\"ErrorMessage\":";
const char Alp_NoErrors[] PROGMEM = "\"ErrorNumber\": 0,\"ErrorMessage\":\"\"";

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

private:
  char ssid[25];
  char password[25];
  IPAddress local_IP;
  IPAddress gateway;
  IPAddress subnet;


  uint32_t _ServerTransactionID = 0;
  char _ServerTransactionIDStr[11];

  uint32_t getAndIncrementServerTransactionID(void);
  char* getServerTransactionIDStr();
  uint32_t getServerTransactionID();

  boolean rorwebserverDebug;
  RORJsonStruct rorjasonstrut;
  AsyncWebServer *rorWebServer;
  AsyncWebSocket *rorWebSocket;
  bool isOSCpulseTriggered;

  void doOSCPulseTrigger(void);
  // being of Alpaca stuff
  AlpacaCommonData AlpacaData;
  void GetAlpArguments(AsyncWebServerRequest *request);
  void PutAlpArguments(AsyncWebServerRequest *request, const uint8_t *data, size_t len);
  void AlpacaHeaderSchema(AsyncResponseStream *response, AlpacaCommonData parameters);
  void AlpacaNoErrorSchema(AsyncResponseStream *response, bool comma = true);
};

#endif
