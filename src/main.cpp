#include <Arduino.h>
#include "SPIFFS.h"
#include "sensor_readings.h"
#include "settings.h"
#include "rorwebserver.h"

SensorReadings sensorBME280Readings;
RORWebServer rorWebServer;
SensorBMe280_Struct mainSensorStruct;

bool debug = false;

// BME280 Loop timers
unsigned long lastTime = 0;
unsigned long timerDelay = 30000; // send readings timer

// ----------------------------------------------------------------------------
// SPIFFS initialization
// ----------------------------------------------------------------------------

void initSPIFFS()
{
  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

// ----------------------------------------------------------------------------
// RORWebServer initialization
// ----------------------------------------------------------------------------

void initWebServer()
{
  // Set up the web server
  if (debug)
  {
    Serial.println("Main.cpp -> initWebServer() rorWebServer.connectToWiFi() ");
  }
  if (rorWebServer.connectToWiFi())
  {
    Serial.println("Main.cpp -> initWebServer() rorWebServer.startRORWebServer() ");
    rorWebServer.startRORWebServer();
  }
  else {
        Serial.println("Main.cpp -> initWebServer() rorWebServer.connectToWiFi() Failed to Connect to WiFi");
  }
}

// ----------------------------------------------------------------------------
// BME280 initialization
// ----------------------------------------------------------------------------

void initBME280()
{
  if (debug)
  {
    Serial.println("Main.cpp -> Initialize BME280 sensor");
  }
  if (!sensorBME280Readings.begin())
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }
}

// ----------------------------------------------------------------------------
// updateBME280Reading Get sensor data and update RORWebServer with data
// ----------------------------------------------------------------------------
void updateBME280Reading()
{
  mainSensorStruct = sensorBME280Readings.getBME280Readings(mainSensorStruct);
  if (debug)
  {
    Serial.println("main.cpp->Loop()->sensorReadings.getBME280Readings()");
    String temp = String(mainSensorStruct.f_temperature).c_str();
    Serial.println("main.cpp->Loop()->sensorReadings.getBME280Readings() sensorBMEstrut.f_temperature: " + temp);
    String hum = String(mainSensorStruct.f_humidity).c_str();
    Serial.println("main.cpp->Loop()->sensorReadings.getBME280Readings() sensorBMEstrut.f_humidity : " + hum);
    String press = String(mainSensorStruct.f_pressure).c_str();
    Serial.println("main.cpp->Loop()->sensorReadings.getBME280Readings() sensorBMEstrut.f_pressure : " + press);
  }
  // Send Events to the Web Server with the Sensor Readings
  rorWebServer.sendBME280Events(mainSensorStruct);
}

void setup()
{
  Serial.begin(115200);
  initSPIFFS();
  initWebServer();
  initBME280();
}

void loop()
{
  if ((millis() - lastTime) > timerDelay)
  {
    updateBME280Reading();
    lastTime = millis();
  }
}
