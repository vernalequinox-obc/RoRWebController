#include "rorwebserver.h"

// Constructor
RORWebServer::RORWebServer()
{
  // Initialize global objects
  rorWebServer = new AsyncWebServer(HTTP_PORT);
  rorWebSocket = new AsyncWebSocket("/rorWebSocket");

  // Set up WiFi configuration
  ssid[0] = '\0';
  password[0] = '\0';
  local_IP = IPAddress(STATIC_LOCAL_IP[0], STATIC_LOCAL_IP[1], STATIC_LOCAL_IP[2], STATIC_LOCAL_IP[3]);
  gateway = IPAddress(STATIC_GATEWAY_IP[0], STATIC_GATEWAY_IP[1], STATIC_GATEWAY_IP[2], STATIC_GATEWAY_IP[3]);
  subnet = IPAddress(STATIC_SUBNET[0], STATIC_SUBNET[1], STATIC_SUBNET[2], STATIC_SUBNET[3]);

  isOSCpulseTriggered = false;
  strlcpy(rorjasonstrut.RoRCurrentPosition, "Unknown", sizeof(rorjasonstrut.RoRCurrentPosition));
  // Set rorwebserverDebug flag
  rorwebserverDebug = false;
}

// Destructor
RORWebServer::~RORWebServer()
{
}

void RORWebServer::setSSID(char *aSSID)
{
  strncpy(ssid, aSSID, sizeof(ssid) - 1);
}
void RORWebServer::setPass(char *aPass)
{
  strncpy(password, aPass, sizeof(password) - 1);
}
void RORWebServer::setIP(char *aAddress)
{
  local_IP.fromString(aAddress);
}
void RORWebServer::setSub(char *aAddress)
{
  subnet.fromString(aAddress);
}
void RORWebServer::setGateway(char *aAddress)
{
  gateway.fromString(aAddress);
}

boolean RORWebServer::connectToWiFi()
{
  // Connect to WiFi
  if (!WiFi.config(local_IP, gateway, subnet))
  {
    Serial.println("STA Failed to configure");
    return false;
  }
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.printf(" %s\n", WiFi.localIP().toString().c_str());
  return true;
}

/*
----------------------------------------------------------------------------
 WebServer initialization
----------------------------------------------------------------------------
  Handling client connections on the server side

*/
// Connect to WiFi and start rorWebServer
void RORWebServer::initWebServer()
{
  // Initialize the rorWebServer and set up handlers for each route

  rorWebServer->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(SPIFFS, "/style.css", "text/css"); });

  rorWebServer->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  // Start the rorWebServer
  rorWebServer->begin();
}

/*
----------------------------------------------------------------------------
 WebSocket initialization
----------------------------------------------------------------------------
  Handling client connections on the server side

*/
void RORWebServer::initWebSocket()
{
  RORWebServer *rorWebServerObj = this;

  rorWebSocket->onEvent(std::bind(&RORWebServer::onEvent, rorWebServerObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                                  std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));

  rorWebServer->addHandler(rorWebSocket);
}

void RORWebServer::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (rorwebserverDebug)
  {
    Serial.printf("RORWebServer::onEvent()\n");
  }
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    notifyClients();
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void RORWebServer::handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  if (rorwebserverDebug)
  {
    Serial.printf("RORWebServer::handleWebSocketMessage() -> ");
  }
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {

    const uint8_t size = JSON_OBJECT_SIZE(1);
    StaticJsonDocument<size> json;
    DeserializationError err = deserializeJson(json, data);
    if (err)
    {
      Serial.print(F("RORWebServer::handleWebSocketMessage -> deserializeJson() failed with code "));
      Serial.println(err.c_str());
      return;
    }

    const char *action = json["action"];
    if (strcmp(action, "toggle") == 0)
    {
      // led.on = !led.on;
      // Do somethings as the open roof button is pressed
      isOSCpulseTriggered = true;
      if (rorwebserverDebug)
      {
        Serial.println("RORWebServer::handleWebSocketMessage -> Open Roof Button Triggered - isOSCpulseTriggered = true\n ");
      }
      notifyClients();
    }
  }
}

/*
----------------------------------------------------------------------------
 WebSocket initialization - End Of
----------------------------------------------------------------------------
*/
void RORWebServer::notifyClients()
{
  if (rorwebserverDebug)
  {
    Serial.printf("RORWebServer::notifyClients()");
  }
  const uint8_t size = JSON_OBJECT_SIZE(12); // add the size of your struct fields to the JSON object size was 12
  StaticJsonDocument<size> json;
  JsonObject root = json.to<JsonObject>();

  root["status"]["indoorTemperature"] = rorjasonstrut.indoorBME280Struct.temperature;
  root["status"]["indoorHumidity"] = rorjasonstrut.indoorBME280Struct.humidity;


  root["status"]["outdoorTemperature"] = rorjasonstrut.outdoorBME280Struct.temperature;
  root["status"]["outdoorHumidity"] = rorjasonstrut.outdoorBME280Struct.humidity;

  root["status"]["RoRCurrentPosition"] = rorjasonstrut.RoRCurrentPosition;
  root["status"]["IsScopeParkSafe"] = rorjasonstrut.IsScopeParkSafe;

  if (rorwebserverDebug)
  {
    Serial.print("RORWebServer::notifyClients() rorjasonstrut.indoorBME280Struct.humidity : ");
    Serial.println(rorjasonstrut.indoorBME280Struct.humidity);
    Serial.print("RORWebServer::notifyClients() rorjasonstrut.indoorBME280Struct.temperature : ");
    Serial.println(rorjasonstrut.indoorBME280Struct.temperature);

    Serial.print("RORWebServer::notifyClients() rorjasonstrut.outdoorBME280Struct.humidity : ");
    Serial.println(rorjasonstrut.outdoorBME280Struct.humidity);
    Serial.print("RORWebServer::notifyClients() rorjasonstrut.outdoorBME280Struct.temperature : ");
    Serial.println(rorjasonstrut.outdoorBME280Struct.temperature);

    Serial.print("RORWebServer::notifyClients() rorjasonstrut.RoRCurrentPosition: ");
    Serial.println(rorjasonstrut.RoRCurrentPosition);
    Serial.print("RORWebServer::notifyClients() rorjasonstrut.IsScopeParkSafe: ");
    Serial.println(rorjasonstrut.IsScopeParkSafe);
  }

  char buffer[1024];
  size_t len = serializeJson(json, buffer);

  if (rorwebserverDebug)
  {
    // Print buffer to Serial Monitor
    Serial.println("Print buffer to Serial Monitor: ");
    for (int i = 0; i < len; i++)
    {
      Serial.print(buffer[i]);
    }

    // Print a newline character to separate the output from other Serial Monitor messages
    Serial.println();
  }

  rorWebSocket->textAll(buffer, len);
}

// Send BME280 sensor readings over event source
void RORWebServer::setJsonValues(SensorBMe280Structure *aIndoorBME280Struct, SensorBMe280Structure *aOutdoorBME280Struct, char *aRORPosition, char *aIsScopeParkSafe)
{
  strncpy(rorjasonstrut.indoorBME280Struct.altitudeMeter, aIndoorBME280Struct->altitudeMeter, sizeof(rorjasonstrut.indoorBME280Struct.altitudeMeter) - 1);
  rorjasonstrut.indoorBME280Struct.altitudeMeter[sizeof(rorjasonstrut.indoorBME280Struct.altitudeMeter) - 1] = '\0';

  strncpy(rorjasonstrut.indoorBME280Struct.altitudeFeet, aIndoorBME280Struct->altitudeFeet, sizeof(rorjasonstrut.indoorBME280Struct.altitudeFeet) - 1);
  rorjasonstrut.indoorBME280Struct.altitudeFeet[sizeof(rorjasonstrut.indoorBME280Struct.altitudeFeet) - 1] = '\0';

  strncpy(rorjasonstrut.indoorBME280Struct.humidity, aIndoorBME280Struct->humidity, sizeof(rorjasonstrut.indoorBME280Struct.humidity) - 1);
  rorjasonstrut.indoorBME280Struct.humidity[sizeof(rorjasonstrut.indoorBME280Struct.humidity) - 1] = '\0';

  strncpy(rorjasonstrut.indoorBME280Struct.pressure, aIndoorBME280Struct->pressure, sizeof(rorjasonstrut.indoorBME280Struct.pressure) - 1);
  rorjasonstrut.indoorBME280Struct.pressure[sizeof(rorjasonstrut.indoorBME280Struct.pressure) - 1] = '\0';

  strncpy(rorjasonstrut.indoorBME280Struct.temperature, aIndoorBME280Struct->temperature, sizeof(rorjasonstrut.indoorBME280Struct.temperature) - 1);
  rorjasonstrut.indoorBME280Struct.temperature[sizeof(rorjasonstrut.indoorBME280Struct.temperature) - 1] = '\0';

  strncpy(rorjasonstrut.outdoorBME280Struct.altitudeMeter, aOutdoorBME280Struct->altitudeMeter, sizeof(rorjasonstrut.outdoorBME280Struct.altitudeMeter) - 1);
  rorjasonstrut.outdoorBME280Struct.altitudeMeter[sizeof(rorjasonstrut.outdoorBME280Struct.altitudeMeter) - 1] = '\0';

  strncpy(rorjasonstrut.outdoorBME280Struct.altitudeFeet, aOutdoorBME280Struct->altitudeFeet, sizeof(rorjasonstrut.outdoorBME280Struct.altitudeFeet) - 1);
  rorjasonstrut.outdoorBME280Struct.altitudeFeet[sizeof(rorjasonstrut.outdoorBME280Struct.altitudeFeet) - 1] = '\0';

  strncpy(rorjasonstrut.outdoorBME280Struct.humidity, aOutdoorBME280Struct->humidity, sizeof(rorjasonstrut.outdoorBME280Struct.humidity) - 1);
  rorjasonstrut.outdoorBME280Struct.humidity[sizeof(rorjasonstrut.outdoorBME280Struct.humidity) - 1] = '\0';

  strncpy(rorjasonstrut.outdoorBME280Struct.pressure, aOutdoorBME280Struct->pressure, sizeof(rorjasonstrut.outdoorBME280Struct.pressure) - 1);
  rorjasonstrut.outdoorBME280Struct.pressure[sizeof(rorjasonstrut.outdoorBME280Struct.pressure) - 1] = '\0';

  strncpy(rorjasonstrut.outdoorBME280Struct.temperature, aOutdoorBME280Struct->temperature, sizeof(rorjasonstrut.outdoorBME280Struct.temperature) - 1);
  rorjasonstrut.outdoorBME280Struct.temperature[sizeof(rorjasonstrut.outdoorBME280Struct.temperature) - 1] = '\0';

  strncpy(rorjasonstrut.RoRCurrentPosition, aRORPosition, sizeof(rorjasonstrut.RoRCurrentPosition) - 1);
  rorjasonstrut.RoRCurrentPosition[sizeof(rorjasonstrut.RoRCurrentPosition) - 1] = '\0';

  strncpy(rorjasonstrut.IsScopeParkSafe, aIsScopeParkSafe, sizeof(rorjasonstrut.IsScopeParkSafe) - 1);
  rorjasonstrut.IsScopeParkSafe[sizeof(rorjasonstrut.IsScopeParkSafe) - 1] = '\0';

  if (rorwebserverDebug )
  {
    Serial.println("RORWebServer::setJsonValues()");
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.indoorBME280Struct.altitudeMeter : ");
    Serial.println(rorjasonstrut.indoorBME280Struct.altitudeMeter);
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.indoorBME280Struct.altitudeFeet : ");
    Serial.println(rorjasonstrut.indoorBME280Struct.altitudeFeet);
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.indoorBME280Struct.humidity : ");
    Serial.println(rorjasonstrut.indoorBME280Struct.humidity);
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.indoorBME280Struct.pressure : ");
    Serial.println(rorjasonstrut.indoorBME280Struct.pressure);
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.indoorBME280Struct.temperature : ");
    Serial.println(rorjasonstrut.indoorBME280Struct.temperature);

    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.outdoorBME280Struct.altitudeMeter : ");
    Serial.println(rorjasonstrut.outdoorBME280Struct.altitudeMeter);
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.outdoorBME280Struct.altitudeFeet : ");
    Serial.println(rorjasonstrut.outdoorBME280Struct.altitudeFeet);
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.outdoorBME280Struct.humidity : ");
    Serial.println(rorjasonstrut.outdoorBME280Struct.humidity);
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.outdoorBME280Struct.pressure : ");
    Serial.println(rorjasonstrut.outdoorBME280Struct.pressure);
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.outdoorBME280Struct.temperature : ");
    Serial.println(rorjasonstrut.outdoorBME280Struct.temperature);

    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.RoRCurrentPosition : ");
    Serial.println(rorjasonstrut.RoRCurrentPosition);
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.IsScopeParkSafe : ");
    Serial.println(rorjasonstrut.IsScopeParkSafe);
  }
}

void RORWebServer::cleanUpClients()
{
  rorWebSocket->cleanupClients();
}

bool RORWebServer::getIsOSCpulseTriggered(void)
{
  return isOSCpulseTriggered;
}

void RORWebServer::resetIsOSCpulseTriggered(void)
{
  isOSCpulseTriggered = false;
}