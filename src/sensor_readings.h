#ifndef SENSOR_READINGS_H
#define SENSOR_READINGS_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "settings.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define XMETERTOFEET 3.28084

struct SensorBMe280_Struct
{
    char temperature[10];
    char humidity[10];
    char pressure[10];
    char altitudeMeter[10];
    char altitudeFeet[10];
};

class SensorReadings {
public:
    SensorReadings();
    ~SensorReadings();
    bool begin();
    void getBME280Readings(SensorBMe280_Struct *aStrut);
    SensorBMe280_Struct sensorReadingStrut;

private:
    Adafruit_BME280 bme;
    boolean debug;
};

#endif
