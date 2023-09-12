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

  rorWebServer->on(
      "/api/v1/dome/0/connected", HTTP_GET, [this](AsyncWebServerRequest *request)
      {
        Serial.println("RORWebServer::rorWebServer->connected ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response,AlpacaData);
        AlpacaNoErrorSchema(response,false);
        response->printf("}");
        request->send(response); });

  rorWebServer->on(
      "/api/v1/dome/0/description", HTTP_GET, [this](AsyncWebServerRequest *request)
      {
        Serial.println("RORWebServer::rorWebServer->description ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("%s\"Roll Off Roof Controller with Web Server\"}", Alp_Value);
        request->send(response); });

  rorWebServer->on(
      "/api/v1/dome/0/driverinfo", HTTP_GET, [this](AsyncWebServerRequest *request)
      {
        Serial.println("RORWebServer::rorWebServer->driverinfo ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("%s\"RoRWebController\"}", Alp_Value);
        request->send(response); });

  rorWebServer->on(
      "/api/v1/dome/0/driverversion", HTTP_GET, [this](AsyncWebServerRequest *request)
      {
        Serial.println("RORWebServer::rorWebServer->deriverversion ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("%s\"%s\"}", Alp_Value, VERSION);
        request->send(response); });

  rorWebServer->on(
      "/api/v1/dome/0/interfaceversion", HTTP_GET, [this](AsyncWebServerRequest *request)
      {
        Serial.println("RORWebServer::rorWebServer->interfaceversion ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("%s1}", Alp_Value);
        request->send(response); });

  rorWebServer->on(
      "/api/v1/dome/0/name", HTTP_GET, [this](AsyncWebServerRequest *request)
      {
        Serial.println("RORWebServer::rorWebServer->name ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("%s\"%s\"}", Alp_Value, ObservertoryName);
        request->send(response); });

  rorWebServer->on(
      "/api/v1/dome/0/cansetshutter", HTTP_GET, [this](AsyncWebServerRequest *request)
      {
        Serial.println("RORWebServer::rorWebServer->cansetshutter ");                    
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response,AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("%strue}",Alp_Value);
        request->send(response); });

  rorWebServer->on("/api/v1/dome/0/slewing", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
        Serial.println("RORWebServer::rorWebServer->cansetshutter ");                    
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
        // Serial.println("RORWebServer::rorWebServer->openshutter ");
        PutAlpArguments(request, data, len);
        // Serial.printf("RORWebServer::rorWebServer->openshutter AlpacaData.clientID: %d\n", AlpacaData.clientID);
        // Serial.printf("RORWebServer::rorWebServer->openshutter AlpacaData.clientTransactionID: %d\n", AlpacaData.clientTransactionID);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        switch (rorjasonstrut.currentRorStatus.scopeParkSafe.shutterState)
        {
        case 0:
          switch (rorjasonstrut.currentRorStatus.rorCurrentPosition.shutterState)
          {
          case shutterOpen:
            response->printf("%s1035,%s\"Shutter is already closed\"", Alp_ErrN, Alp_ErrM);
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
        // Serial.println("RORWebServer::rorWebServer->closeshutter ");
        /*
                String jsondata;
                for (size_t i = 0; i < len; i++)
                {
                  jsondata += (char)data[i];
                }
                Serial.println("RORWebServer::rorWebServer->closeshutter jsondata: " + jsondata);
        */
        PutAlpArguments(request, data, len);
        // Serial.printf("RORWebServer::rorWebServer->closeshutter AlpacaData.clientID: %d\n", AlpacaData.clientID);
        // Serial.printf("RORWebServer::rorWebServer->closeshutter AlpacaData.clientTransactionID: %d\n", AlpacaData.clientTransactionID);
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
            response->printf("%s1035,%s\"Shutter is already open\"", Alp_ErrN, Alp_ErrM);
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
        Serial.println("RORWebServer::rorWebServer->abortslew ");
        /*
                String jsondata;
                for (size_t i = 0; i < len; i++)
                {
                  jsondata += (char)data[i];
                }
                Serial.println("RORWebServer::rorWebServer->closeshutter jsondata: " + jsondata);
        */
        PutAlpArguments(request, data, len);
        Serial.printf("RORWebServer::rorWebServer->abortslew AlpacaData.clientID: %d\n", AlpacaData.clientID);
        Serial.printf("RORWebServer::rorWebServer->abortslew AlpacaData.clientTransactionID: %d\n", AlpacaData.clientTransactionID);
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
        //Serial.println("RORWebServer::rorWebServer->shutterstatus ");                    
        GetAlpArguments(request);
        // Serial.printf("RORWebServer::rorWebServer->shutterstatus AlpacaData.clientID: %d\n", AlpacaData.clientID);
        // Serial.printf("RORWebServer::rorWebServer->shutterstatus AlpacaData.clientTransactionID: %d\n", AlpacaData.clientTransactionID);        
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("\"Value\": %d}", rorjasonstrut.currentRorStatus.rorCurrentPosition.shutterState);
        request->send(response); });

  // True if the mount is in the programmed park position. Set only following a Park() operation and reset with any slew operation.
  rorWebServer->on("/api/v1/dome/0/atpark", HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
        GetAlpArguments(request);
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        AlpacaHeaderSchema(response, AlpacaData);
        AlpacaNoErrorSchema(response);
        response->printf("\"Value\": %d}", rorjasonstrut.currentRorStatus.scopeParkSafe.shutterState);
        request->send(response); });

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
      if (rorwebserverDebug)
      {
        Serial.print(F("RORWebServer::handleWebSocketMessage -> deserializeJson() failed with code "));
        Serial.println(err.c_str());
      }
      return;
    }

    const char *action = json["action"];
    if (strcmp(action, "toggle") == 0)
    {
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

  root["status"]["RoRCurrentPosition"] = rorjasonstrut.currentRorStatus.rorCurrentPosition.shutterState;
  root["status"]["IsScopeParkSafe"] = rorjasonstrut.currentRorStatus.scopeParkSafe.shutterState;
  if (rorwebserverDebug)
  {
    Serial.println("\n");
    Serial.printf("RORWebServer::notifyClients() rorjasonstrut.currentRorStatus.rorCurrentPosition.shutterState: %d\n", rorjasonstrut.currentRorStatus.rorCurrentPosition.shutterState);
    Serial.printf("RORWebServer::notifyClients() rorjasonstrut.currentRorStatus.scopeParkSafe.shutterState: %d\n", rorjasonstrut.currentRorStatus.scopeParkSafe.shutterState);
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
  Serial.println("RORWebServer::rorWebServer->PutAlpArguments()");
  String jsondata;
  for (size_t i = 0; i < len; i++)
  {
    jsondata += (char)data[i];
  }
  jsondata.toLowerCase();
  Serial.println("RORWebServer::rorWebServer->PutAlpArguments()-> jsondata: " + jsondata);
  StaticJsonDocument<200> jsonData;
  DeserializationError error = deserializeJson(jsonData, jsondata);
  if (!error)
  {
    // Check if the JSON contains "ClientID" and "ClientTransactionID"
    if (jsonData.containsKey("clientid") && jsonData.containsKey("clienttransactionid"))
    {
      AlpacaData.clientID = jsonData["clientid"].as<int>();
      AlpacaData.clientTransactionID = jsonData["clienttransactionid"].as<int>();

      // Now you have the ClientID and ClientTransactionID, you can use them as needed.
      Serial.println("ClientID: " + String(AlpacaData.clientID));
      Serial.println("ClientTransactionID: " + String(AlpacaData.clientTransactionID));

      // The rest of your processing logic here...
    }
    else
    {
      Serial.println("JSON data is missing ClientID or ClientTransactionID.");
    }
  }
  else
  {
    Serial.println("Failed to parse JSON data.");
  }
}

void RORWebServer::GetAlpArguments(AsyncWebServerRequest *request)
{
  // Serial.println("RORWebServer::rorWebServer->GetAlpArguments()");
  AlpacaData.switches.idExist = false;
  AlpacaData.switches.intValueExist = false;
  AlpacaData.switches.nameExist = false;
  AlpacaData.switches.stateExist = false;
  AlpacaData.switches.intValueExist = false;
  AlpacaData.switches.nameExist = false;
  AlpacaData.switches.stateExist = false;
  AlpacaData.clientID = 0;
  AlpacaData.clientTransactionID = 0;
  AlpacaData.boConnect = false;
  AlpacaData.switches.id = -1;
  AlpacaData.switches.state = false;
  AlpacaData.switches.intValue = -1;

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
    if (parameter == "id")
    {
      AlpacaData.switches.idExist = true;
      AlpacaData.switches.id = p->value().toInt();
    }
    if (parameter == "value")
    {
      AlpacaData.switches.intValueExist = true;
      AlpacaData.switches.intValue = p->value().toInt();
    }
    if (parameter == "name")
    {
      AlpacaData.switches.nameExist = true;
      AlpacaData.switches.name = p->value();
    }
    if (parameter == "state")
    {
      AlpacaData.switches.stateExist = true;
      String booleanString;
      booleanString = p->value();
      booleanString.toLowerCase();
      if (booleanString == "true")
      {
        AlpacaData.switches.state = true;
      }
      else
      {
        AlpacaData.switches.state = false;
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

char *RORWebServer::getServerTransactionIDStr(void)
{
  uint32_t clientTransactionID = getAndIncrementServerTransactionID();
  snprintf(_ServerTransactionIDStr, sizeof(_ServerTransactionIDStr), "%lu", _ServerTransactionID);
  return _ServerTransactionIDStr;
}

uint32_t RORWebServer::getServerTransactionID(void)
{
  getAndIncrementServerTransactionID();
  return _ServerTransactionID;
}
