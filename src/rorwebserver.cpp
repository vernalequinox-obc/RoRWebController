#include "rorwebserver.h"

// Constructor
RORWebServer::RORWebServer()
{
  // Initialize global objects
  rorWebServer = new AsyncWebServer(HTTP_PORT);
  rorWebSocket = new AsyncWebSocket("/rorWebSocket");

  // Set up WiFi configuration


  // Set debug flag
  debug = false;
}

// Destructor
RORWebServer::~RORWebServer()
{
  delete rorWebSocket;
  delete rorWebServer;
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

// Connect to WiFi and start rorWebServer
void RORWebServer::startRORWebServer()
{
  // Initialize the rorWebServer and set up handlers for each route
  rorWebServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(SPIFFS, "/index.html", String(), false, ROR_Controller::processor); });

  rorWebServer->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(SPIFFS, "/style.css", "text/css"); });

  rorWebServer->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  // Start the rorWebServer
  rorWebServer->begin();
}

// Send BME280 sensor readings over event source
void RORWebServer::sendBME280Events(SensorBMe280_Struct aSensorReadingStrut)
{
  if (debug)
  {
    String temp = String(aSensorReadingStrut.f_temperature).c_str();
    String hum = String(aSensorReadingStrut.f_humidity).c_str();
    String press = String(aSensorReadingStrut.f_pressure).c_str();
    Serial.println("rorwebserver -> RORWebServer::sendBME280Events()");
    Serial.println("rorwebserver -> RORWebServer::sendBME280Events() sensorReadingStrut.f_temperature: " + temp);
    Serial.println("rorwebserver -> RORWebServer::sendBME280Events() sensorReadingStrut.f_humidity : " + hum);
    Serial.println("rorwebserver -> RORWebServer::sendBME280Events() sensorReadingStrut.f_pressure : " + press);
  }
}
