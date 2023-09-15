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
    if (rorwebserverDebug)
    {
      Serial.println("STA Failed to configure");
    }
    return false;
  }
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  unsigned long currentMillis = millis();
  previousMillis = currentMillis;
  while (WiFi.status() != WL_CONNECTED)
  {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
      Serial.println("Failed to connect.");
      return false;
    }
  }
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
                   {
        Serial.println("RORWebServer::rorWebServer->on HTTP_GET /style.css");
        request->send(SPIFFS, "/style.css", "text/css"); });

  rorWebServer->on("/api/v1/dome/0/connected", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
        Serial.println("RORWebServer::rorWebServer->on connected ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response,AlpacaData);
        AlpacaNoErrorSchema(response,false);
        response->printf("}");
        request->send(response); });

  rorWebServer->on("/api/v1/dome/0/description", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
        Serial.println("RORWebServer::rorWebServer->on description ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("%s\"Roll Off Roof Controller with Web Server\"}", Alp_Value);
        request->send(response); });

  rorWebServer->on("/api/v1/dome/0/driverinfo", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
        Serial.println("RORWebServer::rorWebServer->on driverinfo ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("%s\"RoRWebController\"}", Alp_Value);
        request->send(response); });

  rorWebServer->on("/api/v1/dome/0/driverversion", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
        Serial.println("RORWebServer::rorWebServer->on deriverversion ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("%s\"%s\"}", Alp_Value, VERSION);
        request->send(response); });

  rorWebServer->on("/api/v1/dome/0/interfaceversion", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
        Serial.println("RORWebServer::rorWebServer->on interfaceversion ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("%s1}", Alp_Value);
        request->send(response); });

  rorWebServer->on("/api/v1/dome/0/name", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
        Serial.println("RORWebServer::rorWebServer->on name ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("%s\"%s\"}", Alp_Value, ObservertoryName);
        request->send(response); });

  rorWebServer->on("/api/v1/dome/0/cansetshutter", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
        Serial.println("RORWebServer::rorWebServer->on cansetshutter ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response,AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("%strue}",Alp_Value);
        request->send(response); });

  rorWebServer->on("/api/v1/dome/0/slewing", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
        Serial.println("RORWebServer::rorWebServer->on slewing ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response,AlpacaData);
        AlpacaNoErrorSchema(response);
        bool ishutterMoving = false;
        if( rorjasonstrut.currentRorStatus.rorCurrentPosition.shutterState == shutterOpening ||
        rorjasonstrut.currentRorStatus.rorCurrentPosition.shutterState == shutterOpening )
        {
          ishutterMoving = true;
        }
        ishutterMoving ? response->printf("%sfalse}",Alp_Value) : response->printf("%strue}",Alp_Value);
        request->send(response); });

  // Open shutter or otherwise expose telescope to the sky.
  rorWebServer->on(
      "/api/v1/dome/0/openshutter", HTTP_PUT, [this](AsyncWebServerRequest *request) {}, NULL,
      [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
      {
        Serial.println("RORWebServer::rorWebServer->on openshutter ");
        PutAlpArguments(request, data, len);
        // Serial.printf("RORWebServer::rorWebServer->on openshutter AlpacaData.clientID: %d\n", AlpacaData.clientID);
        // Serial.printf("RORWebServer::rorWebServer->on openshutter AlpacaData.clientTransactionID: %d\n", AlpacaData.clientTransactionID);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        switch (rorjasonstrut.currentRorStatus.scopeParkSafe.shutterState)
        {
        case 0:
          switch (rorjasonstrut.currentRorStatus.rorCurrentPosition.shutterState)
          {
          case shutterOpen:
            response->printf("%s1035,%s\"Shutter is already open\"", Alp_ErrN, Alp_ErrM);
            break;
          case shutterClosed:
            AlpacaNoErrorSchema(response, false);
            doOSCPulseTrigger(); // Call the trigger function
            break;
          case shutterOpening:
          case shutterClosing:
            response->printf("%s1035,%s\"Shutter currently moving\"", Alp_ErrN, Alp_ErrM);
            break;
          case shutterError:
            response->printf("%s1035,%s\"Shutter Error and Unknown Location\"", Alp_ErrN, Alp_ErrM);
            break;
          default:
            response->printf("%s1035,%s\"Unknown Shutter or Park State\"", Alp_ErrN, Alp_ErrM);
          }
          break;
        case 1:
          response->printf("%s1035,%s\"Mount unParked Shutter cannot operate\"", Alp_ErrN, Alp_ErrM);
          break;
        default:
          response->printf("%s1035,%s\"Unknown Shutter or Park State\"", Alp_ErrN, Alp_ErrM);
          break;
        }
        response->print(F("}"));
        request->send(response);
      });

  // Close the shutter or otherwise shield telescope from the sky.
  rorWebServer->on(
      "/api/v1/dome/0/closeshutter", HTTP_PUT,
      [this](AsyncWebServerRequest *request) {}, // This is the first lambda function
      NULL,
      [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
      {
        // Serial.println("RORWebServer::rorWebServer->on closeshutter ");
        /*
                String jsondata;
                for (size_t i = 0; i < len; i++)
                {
                  jsondata += (char)data[i];
                }
                Serial.println("RORWebServer::rorWebServer->on closeshutter jsondata: " + jsondata);
        */
        Serial.println("RORWebServer::rorWebServer->on closeshutter ");
        PutAlpArguments(request, data, len);
        // Serial.printf("RORWebServer::rorWebServer->on closeshutter AlpacaData.clientID: %d\n", AlpacaData.clientID);
        // Serial.printf("RORWebServer::rorWebServer->on closeshutter AlpacaData.clientTransactionID: %d\n", AlpacaData.clientTransactionID);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        switch (rorjasonstrut.currentRorStatus.scopeParkSafe.shutterState)
        {
        case 0:
          switch (rorjasonstrut.currentRorStatus.rorCurrentPosition.shutterState)
          {
          case shutterOpen:
            AlpacaNoErrorSchema(response, false);
            doOSCPulseTrigger(); // Call the trigger function
            break;
          case shutterClosed:
            response->printf("%s1035,%s\"Shutter is already closed\"", Alp_ErrN, Alp_ErrM);
            break;
          case shutterOpening:
          case shutterClosing:
            response->printf("%s1035,%s\"Shutter currently moving\"", Alp_ErrN, Alp_ErrM);
            break;
          case shutterError:
            response->printf("%s1035,%s\"Shutter Error and Unknown Location\"", Alp_ErrN, Alp_ErrM);
            break;
          default:
            response->printf("%s1035,%s\"Unknown Shutter or Park State\"", Alp_ErrN, Alp_ErrM);
          }
          break;
        case 1:
          response->printf("%s1035,%s\"Mount unParked Shutter cannot operate\"", Alp_ErrN, Alp_ErrM);
          break;
        default:
          response->printf("%s1035,%s\"Unknown Shutter or Park State\"", Alp_ErrN, Alp_ErrM);
          break;
        }
        response->print(F("}"));
        request->send(response);
      });

  // Calling this method will immediately disable hardware slewing (Slaved will become False).
  rorWebServer->on(
      "/api/v1/dome/0/abortslew", HTTP_PUT,
      [this](AsyncWebServerRequest *request) {}, // This is the first lambda function
      NULL,
      [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
      {
        Serial.println("RORWebServer::rorWebServer->on abortslew ");
        /*
                String jsondata;
                for (size_t i = 0; i < len; i++)
                {
                  jsondata += (char)data[i];
                }
                Serial.println("RORWebServer::rorWebServer->on closeshutter jsondata: " + jsondata);
        */
        PutAlpArguments(request, data, len);
        // Serial.printf("RORWebServer::rorWebServer->on abortslew AlpacaData.clientID: %d\n", AlpacaData.clientID);
        // Serial.printf("RORWebServer::rorWebServer->on abortslew AlpacaData.clientTransactionID: %d\n", AlpacaData.clientTransactionID);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        switch (rorjasonstrut.currentRorStatus.scopeParkSafe.shutterState)
        {
        case 0:
          switch (rorjasonstrut.currentRorStatus.rorCurrentPosition.shutterState)
          {
          case shutterOpen:
          case shutterClosed:
            response->printf("%s1035,%s\"Shutter is not moving\"", Alp_ErrN, Alp_ErrM);
            break;
          case shutterOpening:
          case shutterClosing:
            AlpacaNoErrorSchema(response, false);
            doOSCPulseTrigger(); // Call the trigger function
            break;
          case shutterError:
            response->printf("%s1035,%s\"Shutter Error and Unknown Location\"", Alp_ErrN, Alp_ErrM);
            break;
          default:
            response->printf("%s1035,%s\"Unknown Shutter or Park State\"", Alp_ErrN, Alp_ErrM);
          }
          break;
        case 1:
          response->printf("%s1035,%s\"Mount unParked Shutter cannot operate\"", Alp_ErrN, Alp_ErrM);
          break;
        default:
          response->printf("%s1035,%s\"Unknown Shutter or Park State\"", Alp_ErrN, Alp_ErrM);
          break;
        }
        response->print(F("}"));
        request->send(response);
      });

  // Returns the status of the dome shutter or roll-off roof. 0 = Open, 1 = Closed, 2 = Opening, 3 = Closing, 4 = Shutter status error
  // Declare this at the class level to accumulate JSON data

  rorWebServer->on("/api/v1/dome/0/shutterstatus", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
        Serial.println("RORWebServer::rorWebServer->on shutterstatus");                      
        GetAlpArguments(request);
        // Serial.printf("RORWebServer::rorWebServer->on shutterstatus AlpacaData.clientID: %d\n", AlpacaData.clientID);
        // Serial.printf("RORWebServer::rorWebServer->on shutterstatus AlpacaData.clientTransactionID: %d\n", AlpacaData.clientTransactionID);        
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("\"Value\": %d}", rorjasonstrut.currentRorStatus.rorCurrentPosition.shutterState);
        request->send(response); });

  // True if the mount is in the programmed park position. Set only following a Park() operation and reset with any slew operation.
  rorWebServer->on("/api/v1/dome/0/atpark", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
        Serial.println("RORWebServer::rorWebServer->on atpark ");  
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("\"Value\": %d}", rorjasonstrut.currentRorStatus.scopeParkSafe.shutterState);
        request->send(response); });

  rorWebServer->on("/api/v1/dome/0/supportedactions", HTTP_GET, [this](AsyncWebServerRequest *request)
                   { 
        Serial.println("RORWebServer::rorWebServer->on supportedactions ");  
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("\"Value\": %d}", rorjasonstrut.currentRorStatus.scopeParkSafe.shutterState);
        request->send(response); });

  rorWebServer->on(
      "/api/v1/dome/0/action", HTTP_PUT, [this](AsyncWebServerRequest *request) {}, // This is the first lambda function
      NULL,
      [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
      {
        bool isThereAction = false;
        char *printJsonOut;
        char *jsonData;
        Serial.println("RORWebServer::rorWebServer->on action ");
        PutAlpArguments(request, data, len);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response, false);
        response->print(",\"Value\": ");
        if (strcmp(AlpacaData.actionName, "obvservingcondition") == 0)
        {
          jsonData = createJsonTempHumidity();
          response->print(jsonData);
          isThereAction = true;
        }
        else
        {
              response->print("\"\"");
        }
        response->print(F("}"));
        request->send(response);
        if(isThereAction )
        {
          free(printJsonOut);
        } });

  rorWebServer->on("/api/v1/dome/0/altitude", HTTP_GET, [this](AsyncWebServerRequest *request)
                   { AscomPropertyNotImplemented(request); });
  rorWebServer->on("/api/v1/dome/0/athome", HTTP_GET, [this](AsyncWebServerRequest *request)
                   { AscomPropertyNotImplemented(request); });
  rorWebServer->on("/api/v1/dome/0/azimuth", HTTP_GET, [this](AsyncWebServerRequest *request)
                   { AscomPropertyNotImplemented(request); });
  rorWebServer->on("/api/v1/dome/0/slaved", HTTP_PUT, [this](AsyncWebServerRequest *request)
                   { AscomMethodNotImplemented(request); });
  rorWebServer->on("/api/v1/dome/0/park", HTTP_PUT, [this](AsyncWebServerRequest *request)
                   { AscomMethodNotImplemented(request); });
  rorWebServer->on("/api/v1/dome/0/setpark", HTTP_PUT, [this](AsyncWebServerRequest *request)
                   { AscomMethodNotImplemented(request); });
  rorWebServer->on("/api/v1/dome/0/slewtoaltitude", HTTP_PUT, [this](AsyncWebServerRequest *request)
                   { AscomMethodNotImplemented(request); });
  rorWebServer->on("/api/v1/dome/0/slewtoazimuth", HTTP_PUT, [this](AsyncWebServerRequest *request)
                   { AscomMethodNotImplemented(request); });
  rorWebServer->on("/api/v1/dome/0/synctoazimuth", HTTP_PUT, [this](AsyncWebServerRequest *request)
                   { AscomMethodNotImplemented(request); });
  rorWebServer->on("/api/v1/dome/0/commandblind", HTTP_PUT, [this](AsyncWebServerRequest *request)
                   { AscomMethodNotImplemented(request); });
  rorWebServer->on("/api/v1/dome/0/commandbool", HTTP_PUT, [this](AsyncWebServerRequest *request)
                   { AscomMethodNotImplemented(request); });
  rorWebServer->on("/api/v1/dome/0/commandstring", HTTP_PUT, [this](AsyncWebServerRequest *request)
                   { AscomMethodNotImplemented(request); });

  // Declare this at the class level to accumulate JSON data

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
  switch (type)
  {
  case WS_EVT_CONNECT:
    if (rorwebserverDebug)
      notifyClients();
    break;
  case WS_EVT_DISCONNECT:
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
      // Do somethings as the open roof button is pressed
      isOSCpulseTriggered = true;
      notifyClients();
    }
  }
}

// Send BME280 sensor readings over event source
void RORWebServer::setJsonValues(SensorBMe280Structure *aIndoorBME280Struct, SensorBMe280Structure *aOutdoorBME280Struct, ROR_Status *aRorStatus)
{
  strcpy(rorjasonstrut.indoorBME280Struct.altitudeMeter, aIndoorBME280Struct->altitudeMeter);
  strcpy(rorjasonstrut.indoorBME280Struct.altitudeFeet, aIndoorBME280Struct->altitudeFeet);
  strcpy(rorjasonstrut.indoorBME280Struct.humidity, aIndoorBME280Struct->humidity);
  strcpy(rorjasonstrut.indoorBME280Struct.pressure, aIndoorBME280Struct->pressure);
  strcpy(rorjasonstrut.indoorBME280Struct.temperature, aIndoorBME280Struct->temperature);
  strcpy(rorjasonstrut.outdoorBME280Struct.altitudeMeter, aOutdoorBME280Struct->altitudeMeter);
  strcpy(rorjasonstrut.outdoorBME280Struct.altitudeFeet, aOutdoorBME280Struct->altitudeFeet);
  strcpy(rorjasonstrut.outdoorBME280Struct.humidity, aOutdoorBME280Struct->humidity);
  strcpy(rorjasonstrut.outdoorBME280Struct.pressure, aOutdoorBME280Struct->pressure);
  strcpy(rorjasonstrut.outdoorBME280Struct.temperature, aOutdoorBME280Struct->temperature);
  rorjasonstrut.currentRorStatus.rorCurrentPosition.isTrue = aRorStatus->rorCurrentPosition.isTrue;
  rorjasonstrut.currentRorStatus.rorCurrentPosition.shutterState = aRorStatus->rorCurrentPosition.shutterState;
  rorjasonstrut.currentRorStatus.scopeParkSafe.shutterState = aRorStatus->scopeParkSafe.shutterState;
  rorjasonstrut.currentRorStatus.scopeParkSafe.isTrue = aRorStatus->scopeParkSafe.isTrue;
}

char *RORWebServer::createJsonTempHumidity(void)
{
  try
  {
    const uint8_t size = JSON_OBJECT_SIZE(12);
    StaticJsonDocument<size> json;
    JsonObject root = json.to<JsonObject>();
    //    root["status"]["indoorTemperature"] = rorjasonstrut.indoorBME280Struct.temperature;
    root["status"]["indoorTemperature"] = rorjasonstrut.indoorBME280Struct.temperature;
    root["status"]["indoorHumidity"] = rorjasonstrut.indoorBME280Struct.humidity;

    root["status"]["outdoorTemperature"] = rorjasonstrut.outdoorBME280Struct.temperature;
    root["status"]["outdoorHumidity"] = rorjasonstrut.outdoorBME280Struct.humidity;

    char *buffer = (char *)malloc(1024); // Allocate memory dynamically
    if (buffer)
    {
      size_t len = serializeJson(json, buffer, 1024);
      return buffer;
    }
    else
    {
      Serial.print(F(" -> deserializeJson() failed"));
      // Handle memory allocation failure
      return nullptr;
    }
  }
  catch (const std::exception &e)
  {
    Serial.print(e.what());
    Serial.println("\n");
    return nullptr;
  }
}

void RORWebServer::notifyClients()
{
  const uint8_t size = JSON_OBJECT_SIZE(12); // add the size of your struct fields to the JSON object size was 12
  StaticJsonDocument<size> json;
  JsonObject root = json.to<JsonObject>();

  root["status"]["indoorTemperature"] = rorjasonstrut.indoorBME280Struct.temperature;
  root["status"]["indoorHumidity"] = rorjasonstrut.indoorBME280Struct.humidity;

  root["status"]["outdoorTemperature"] = rorjasonstrut.outdoorBME280Struct.temperature;
  root["status"]["outdoorHumidity"] = rorjasonstrut.outdoorBME280Struct.humidity;

  root["status"]["RoRCurrentPosition"] = rorjasonstrut.currentRorStatus.rorCurrentPosition.shutterState;
  root["status"]["IsScopeParkSafe"] = rorjasonstrut.currentRorStatus.scopeParkSafe.shutterState;

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

void RORWebServer::cleanUpClients()
{
  rorWebSocket->cleanupClients();
}

void RORWebServer::doOSCPulseTrigger(void)
{
  isOSCpulseTriggered = true;
  notifyClients();
}

bool RORWebServer::getIsOSCpulseTriggered(void)
{
  return isOSCpulseTriggered;
}

void RORWebServer::resetIsOSCpulseTriggered(void)
{
  isOSCpulseTriggered = false;
}

void RORWebServer::notFound(AsyncWebServerRequest *request)
{
  request->send(404, "application/json", "{\"message\":\"Not found\"}");
}

//  Apaca Stuff

void RORWebServer::AlpacaHeaderSchema(AsyncResponseStream *response, AlpacaCommonData parameters)
{
  response->print(F("{\"ClientTransactionID\":"));
  response->print(parameters.clientTransactionID);
  response->print(F(",\"ServerTransactionID\":"));
  parameters.serverTransactionID = getServerTransactionID();
  response->print(parameters.serverTransactionID);
  response->print(F(","));
}

void RORWebServer::PutAlpArguments(AsyncWebServerRequest *request, const uint8_t *data, size_t len)
{
  AlpacaData.clientID = 0;
  AlpacaData.clientTransactionID = 0;
  AlpacaData.boConnect = false;
  AlpacaData.actionName[0] = '\0';
  AlpacaData.actionParameters[0] = '\0';

  String jsondata;
  for (size_t i = 0; i < len; i++)
  {
    jsondata += (char)data[i];
  }
  jsondata.toLowerCase();
  StaticJsonDocument<400> jsonData;
  DeserializationError error = deserializeJson(jsonData, jsondata);
  if (!error)
  {
    // Check if the JSON contains "ClientID" and "ClientTransactionID"
    if (jsonData.containsKey("clientid") && jsonData.containsKey("clienttransactionid"))
    {
      AlpacaData.clientID = jsonData["clientid"].as<int>();
      AlpacaData.clientTransactionID = jsonData["clienttransactionid"].as<int>();
    }
    else
    {
      Serial.println("JSON data is missing ClientID or ClientTransactionID.");
      return;
    }
    if (jsonData.containsKey("actionname"))
    {
      strncpy(AlpacaData.actionName, jsonData["actionname"], sizeof(AlpacaData.actionName) - 1);
    }
    if (jsonData.containsKey("actionparameters"))
    {
      strncpy(AlpacaData.actionParameters, jsonData["actionparameters"], sizeof(AlpacaData.actionParameters) - 1);
    }
  }
  else
  {
    Serial.print("RORWebServer::PutAlpArguments - deserializeJson(jsonData, jsondata) Error: Failed to parse JSON jsonData: " + jsondata);
    Serial.print(error.c_str());
    Serial.println("\n");
  }
}

void RORWebServer::GetAlpArguments(AsyncWebServerRequest *request)
{
  // Serial.println("RORWebServer::rorWebServer->on GetAlpArguments()");
  AlpacaData.clientID = 0;
  AlpacaData.clientTransactionID = 0;
  AlpacaData.boConnect = false;
  AlpacaData.actionName[0] = '\0';
  AlpacaData.actionParameters[0] = '\0';  

  // Handle GET request
  // Serial.println("RORWebServer::GetAlpArguments() HTTP_GET");
  int paramsNr = request->params();
  String parameter;
  AlpacaData.serverTransactionID++;
  for (int i = 0; i < paramsNr; i++)
  {
    AsyncWebParameter *p = request->getParam(i);
    parameter = p->name();
    parameter.toLowerCase();
    if (parameter == "clientid")
    {
      AlpacaData.clientID = p->value().toInt();
    }
    if (parameter == "clienttransactionid")
    {
      AlpacaData.clientTransactionID = p->value().toInt();
    }
    if (parameter == "connected")
    {
      String booleanString;
      booleanString = p->value();
      booleanString.toLowerCase();
      if (booleanString == "true")
      {
        AlpacaData.boConnect = true;
      }
      else
      {
        AlpacaData.boConnect = false;
      }
    }
  }
}

void RORWebServer::AlpacaNoErrorSchema(AsyncResponseStream *response, bool comma)
{
  response->printf("%s0,%s\"\"", Alp_ErrN, Alp_ErrM);
  if (comma)
  {
    response->print(F(","));
  }
}

uint32_t RORWebServer::getAndIncrementServerTransactionID(void)
{
  // Increment the transaction ID and wrap around if it exceeds the maximum value of uint32_t
  _ServerTransactionID++;
  if (_ServerTransactionID >= UINT32_MAX)
  {
    _ServerTransactionID = 1;
  }
  return _ServerTransactionID;
}

uint32_t RORWebServer::getServerTransactionID(void)
{
  getAndIncrementServerTransactionID();
  return _ServerTransactionID;
}

void RORWebServer::AscomPropertyNotImplemented(AsyncWebServerRequest *request)
{
  GetAlpArguments(request);
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  response->printf("{%s%d,%s%d,",
                   Alp_CliTraId, AlpacaData.clientTransactionID,
                   Alp_SerTraId, AlpacaData.serverTransactionID);
  response->print(F("\"ErrorNumber\":1024,\"ErrorMessage\":\"Property not implemented\""));
  response->print(F("}"));
  request->send(response);
}

void RORWebServer::AscomMethodNotImplemented(AsyncWebServerRequest *request)
{
  GetAlpArguments(request);
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  response->printf("{%s%d,%s%d,",
                   Alp_CliTraId, AlpacaData.clientTransactionID,
                   Alp_SerTraId, AlpacaData.serverTransactionID);
  response->print(F("\"ErrorNumber\":1024,\"ErrorMessage\":\"Method not implemented\""));
  response->print(F("}"));
  request->send(response);
}

void RORWebServer::AscomNoActions(AsyncWebServerRequest *request)
{
  GetAlpArguments(request);
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  response->printf("{%s%d,%s%d,%s",
                   Alp_CliTraId, AlpacaData.clientTransactionID,
                   Alp_SerTraId, AlpacaData.serverTransactionID,
                   Alp_NoErrors);
  response->print(F(",\"Value\":[]"));
  response->print(F("}"));
  request->send(response);
}