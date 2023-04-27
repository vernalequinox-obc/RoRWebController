#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "sensor_readings.h"
#include "settings.h"
#include "rorwebserver.h"

SensorReadings sensorReadings;
RORWebServer rorWebServer;
SensorBMe280_Struct mainSensorStruct;

bool debug = false;

// BME280 Loop timers
unsigned long lastTime = 0;
unsigned long timerDelay = 30000; // send readings timer

void setup()
{
  // rorWebServer = new RORWebServer();
  Serial.begin(115200);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Initialize BME280 sensor
  if (debug)
  {
    Serial.println("Main.cpp -> Initialize BME280 sensor");
  }
  if (!sensorReadings.begin())
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }

  // Set up the web server
  if (debug)
  {
    Serial.println("Main.cpp -> setupRorWebServer()");
  }
  rorWebServer.startServer();
}

void loop()
{
  if ((millis() - lastTime) > timerDelay)
  {
    mainSensorStruct = sensorReadings.getBME280Readings(mainSensorStruct);
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

    lastTime = millis();
  }
}
