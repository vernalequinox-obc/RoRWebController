#ifndef SENSOR_READINGS_H
#define SENSOR_READINGS_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
#define XMETERTOFEET 3.28084


struct SensorBMe280_Struct
{
    String temperature;
    String humidity;
    String pressure;
    String altitudeMeter; 
    String altitudeFeet;
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
