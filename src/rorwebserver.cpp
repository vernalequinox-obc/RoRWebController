#include "rorwebserver.h"

// Constructor
RORWebServer::RORWebServer()
{
  // Initialize global objects
  events = new AsyncEventSource("/events");
  server = new AsyncWebServer(80);

  // Set up WiFi configuration
  ssid = "Gentry2";
  password = "LittleJack";
  local_IP = IPAddress(192, 168, 0, 219);
  gateway = IPAddress(192, 168, 0, 1);
  subnet = IPAddress(255, 255, 255, 0);

  // Set debug flag
  debug = false;
}

// Destructor
RORWebServer::~RORWebServer()
{
  delete events;
  delete server;
}

// Connect to WiFi and start server
void RORWebServer::startServer()
{
  // Connect to WiFi
  if (!WiFi.config(local_IP, gateway, subnet))
  {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Initialize the server and set up handlers for each route
  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
             { request->send(SPIFFS, "/index.html", String(), false, ROR_Controller::processor); });

  server->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
             { request->send(SPIFFS, "/style.css", "text/css"); });

 // Start the server
  server->begin();
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

  events->send("ping", NULL, millis());
  events->send(String(aSensorReadingStrut.f_temperature).c_str(), "temperature", millis());
  events->send(String(aSensorReadingStrut.f_humidity).c_str(), "humidity", millis());
  events->send(String(aSensorReadingStrut.f_pressure).c_str(), "pressure", millis());
}
