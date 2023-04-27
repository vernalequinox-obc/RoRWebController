#ifndef SENSOR_READINGS_H
#define SENSOR_READINGS_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)


struct SensorBMe280_Struct
{
    float f_temperature;
    float f_humidity;
    float f_pressure;
    float f_altitude; 
};

class SensorReadings {
public:
    SensorReadings();
    ~SensorReadings();
    bool begin();
    SensorBMe280_Struct getBME280Readings(SensorBMe280_Struct aStrut);
    static String processor(const String &var);
    SensorBMe280_Struct sensorReadingStrut;
    
private:
    Adafruit_BME280 bme;
    boolean debug;
};

#endif
