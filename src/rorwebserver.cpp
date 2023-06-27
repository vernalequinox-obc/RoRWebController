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
  local_IP = IPAddress(STATIC_LOCAL_IP[0],STATIC_LOCAL_IP[1], STATIC_LOCAL_IP[2], STATIC_LOCAL_IP[3]);
  gateway = IPAddress(STATIC_GATEWAY_IP[0], STATIC_GATEWAY_IP[1], STATIC_GATEWAY_IP[2], STATIC_GATEWAY_IP[3] );
  subnet = IPAddress(STATIC_SUBNET[0], STATIC_SUBNET[1], STATIC_SUBNET[2], STATIC_SUBNET[3]);

  // Set rorwebserverDebug flag
  rorwebserverDebug = false;



}

// Destructor
RORWebServer::~RORWebServer()
{
}

void RORWebServer::setSSID(char *aSSID)
{
  strncpy(ssid, aSSID, sizeof(ssid)-1);
}
void RORWebServer::setPass(char *aPass)
{
  strncpy(password, aPass, sizeof(password)-1);
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
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(err.c_str());
      return;
    }

    const char *action = json["action"];
    if (strcmp(action, "toggle") == 0)
    {
      // led.on = !led.on;
      // Do somethings as the open roof button is pressed
      if (true)
      {
        Serial.printf("Open Roof Button Triggered.\n ");
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
  const uint8_t size = JSON_OBJECT_SIZE(12); // add the size of your struct fields to the JSON object size
  StaticJsonDocument<size> json;
  JsonObject root = json.to<JsonObject>();

  root["status"]["temperature"] = rorjasonstrut.temperature;
  root["status"]["humidity"] = rorjasonstrut.humidity;
  root["status"]["pressure"] = rorjasonstrut.pressure;
  root["status"]["altitudeMeter"] = rorjasonstrut.altitudeMeter;
  root["status"]["altitudeFeet"] = rorjasonstrut.altitudeFeet;
  root["status"]["RoRCurrentPosition"] = rorjasonstrut.RoRCurrentPosition;
  root["status"]["IsScopeParkSafe"] = rorjasonstrut.IsScopeParkSafe;

  if (rorwebserverDebug)
  {
    Serial.print("RORWebServer::notifyClients() rorjasonstrut.altitudeMeter : ");
    Serial.println(rorjasonstrut.altitudeMeter);
    Serial.print("RORWebServer::notifyClients() rorjasonstrut.altitudeFeet : ");
    Serial.println(rorjasonstrut.altitudeFeet);
    Serial.print("RORWebServer::notifyClients() rorjasonstrut.humidity : ");
    Serial.println(rorjasonstrut.humidity);
    Serial.print("RORWebServer::notifyClients() rorjasonstrut.pressure : ");
    Serial.println(rorjasonstrut.pressure);
    Serial.print("RORWebServer::notifyClients() rorjasonstrut.temperature : ");
    Serial.println(rorjasonstrut.temperature);
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
void RORWebServer::setJsonValues(SensorBMe280_Struct *aSensorReadingStrut, char *aRORPosition, char *aIsScopeParkSafe)
{
  strncpy(rorjasonstrut.altitudeMeter, aSensorReadingStrut->altitudeMeter, sizeof(rorjasonstrut.altitudeMeter) - 1);
  rorjasonstrut.altitudeMeter[sizeof(rorjasonstrut.altitudeMeter) - 1] = '\0';

  strncpy(rorjasonstrut.altitudeFeet, aSensorReadingStrut->altitudeFeet, sizeof(rorjasonstrut.altitudeFeet) - 1);
  rorjasonstrut.altitudeFeet[sizeof(rorjasonstrut.altitudeFeet) - 1] = '\0';

  strncpy(rorjasonstrut.humidity, aSensorReadingStrut->humidity, sizeof(rorjasonstrut.humidity) - 1);
  rorjasonstrut.humidity[sizeof(rorjasonstrut.humidity) - 1] = '\0';

  strncpy(rorjasonstrut.pressure, aSensorReadingStrut->pressure, sizeof(rorjasonstrut.pressure) - 1);
  rorjasonstrut.pressure[sizeof(rorjasonstrut.pressure) - 1] = '\0';

  strncpy(rorjasonstrut.temperature, aSensorReadingStrut->temperature, sizeof(rorjasonstrut.temperature) - 1);
  rorjasonstrut.temperature[sizeof(rorjasonstrut.temperature) - 1] = '\0';

  strncpy(rorjasonstrut.RoRCurrentPosition, aRORPosition, sizeof(rorjasonstrut.RoRCurrentPosition) - 1);
  rorjasonstrut.RoRCurrentPosition[sizeof(rorjasonstrut.RoRCurrentPosition) - 1] = '\0';

  strncpy(rorjasonstrut.IsScopeParkSafe, aIsScopeParkSafe, sizeof(rorjasonstrut.IsScopeParkSafe) - 1);
  rorjasonstrut.IsScopeParkSafe[sizeof(rorjasonstrut.IsScopeParkSafe) - 1] = '\0';

  if (rorwebserverDebug)
  {
    Serial.println("RORWebServer::setJsonValues()");
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.altitudeMeter : ");
    Serial.println(rorjasonstrut.altitudeMeter);
    Serial.println("RORWebServer::setJsonValues() rorjasonstrut.altitudeFeet : ");
    Serial.println(rorjasonstrut.altitudeFeet);
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.humidity : ");
    Serial.println(rorjasonstrut.humidity);
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.pressure : ");
    Serial.println(rorjasonstrut.pressure);
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.temperature : ");
    Serial.println(rorjasonstrut.temperature);
    Serial.print("RORWebServer::setJsonValues() rorjasonstrut.RoRCurrentPosition : ");
    Serial.println(rorjasonstrut.RoRCurrentPosition);
    Serial.println("RORWebServer::setJsonValues() rorjasonstrut.IsScopeParkSafe : ");
    Serial.println(rorjasonstrut.IsScopeParkSafe);
  }
}

void RORWebServer::cleanUpClients()
{
  rorWebSocket->cleanupClients();
}
