#include "rorwebserver.h"

// Constructor
RORWebServer::RORWebServer()
{
  // Initialize global objects
  rorWebServer = new AsyncWebServer(HTTP_PORT);
  rorWebSocket = new AsyncWebSocket("/rorWebSocket");

  // Set up WiFi configuration
  ssid = "Gentry2";
  password = "LittleJack";
  local_IP = IPAddress(192, 168, 0, 219);
  gateway = IPAddress(192, 168, 0, 1);
  subnet = IPAddress(255, 255, 255, 0);

  // Set debug flag
  debug = true;
}

// Destructor
RORWebServer::~RORWebServer()
{
}

void RORWebServer::setSSID(String aSSID)
{
  ssid = aSSID;
}
void RORWebServer::setPass(String aPass)
{
  password = aPass;
}
void RORWebServer::setIP(String address)
{
  local_IP.fromString(address);
}
void RORWebServer::setSub(String address)
{
  subnet.fromString(address);
}
void RORWebServer::setGateway(String address)
{
  gateway.fromString(address);
}

boolean RORWebServer::connectToWiFi()
{
  // Connect to WiFi
  if (!WiFi.config(local_IP, gateway, subnet))
  {
    Serial.println("STA Failed to configure");
    return false;
  }
  WiFi.begin(ssid.c_str(), password.c_str());
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
  if (debug)
  {
    Serial.printf("RORWebServer::onEvent()");
  }
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
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
  if (debug)
  {
    Serial.printf("RORWebServer::handleWebSocketMessage()");
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
  if (debug)
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
  root["status"]["RoRPosition"] = rorjasonstrut.RoRPosition;

  if (debug)
  {
    Serial.println("RORWebServer::notifyClients()");
    Serial.println("RORWebServer::notifyClients() rorjasonstrut.altitudeMeter : " + rorjasonstrut.altitudeMeter);
    Serial.println("RORWebServer::notifyClients() rorjasonstrut.altitudeFeet : " + rorjasonstrut.altitudeFeet);
    Serial.println("RORWebServer::notifyClients() rorjasonstrut.humidity : " + rorjasonstrut.humidity);
    Serial.println("RORWebServer::notifyClients() rorjasonstrut.pressure : " + rorjasonstrut.pressure);
    Serial.println("RORWebServer::notifyClients() rorjasonstrut.temperature : " + rorjasonstrut.temperature);
    Serial.println("RORWebServer::notifyClients() rorjasonstrut.RoRPosition : " + rorjasonstrut.RoRPosition);
  }

  char buffer[1024];
  size_t len = serializeJson(json, buffer);

  if (debug)
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
void RORWebServer::setJsonValues(SensorBMe280_Struct aSensorReadingStrut, String aRORPosition)
{

  rorjasonstrut.altitudeMeter = aSensorReadingStrut.altitudeMeter;
  rorjasonstrut.altitudeFeet = aSensorReadingStrut.altitudeFeet;
  rorjasonstrut.humidity = aSensorReadingStrut.humidity;
  rorjasonstrut.pressure = aSensorReadingStrut.pressure;
  rorjasonstrut.temperature = aSensorReadingStrut.temperature;
  rorjasonstrut.RoRPosition = aRORPosition;

  if (debug)
  {
    Serial.println("RORWebServer::setJsonValues()");
    Serial.println("RORWebServer::setJsonValues() rorjasonstrut.altitudeMeter : " + rorjasonstrut.altitudeMeter);
    Serial.println("RORWebServer::setJsonValues() rorjasonstrut.altitudeFeet : " + rorjasonstrut.altitudeFeet);
    Serial.println("RORWebServer::setJsonValues() rorjasonstrut.humidity : " + rorjasonstrut.humidity);
    Serial.println("RORWebServer::setJsonValues() rorjasonstrut.pressure : " + rorjasonstrut.pressure);
    Serial.println("RORWebServer::setJsonValues() rorjasonstrut.temperature : " + rorjasonstrut.temperature);
    Serial.println("RORWebServer::setJsonValues() rorjasonstrut.RoRPosition : " + rorjasonstrut.RoRPosition);
  }
}

void RORWebServer::cleanUpClients()
{
  rorWebSocket->cleanupClients();
}
