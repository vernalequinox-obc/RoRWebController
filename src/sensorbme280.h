#ifndef SENSORBME280_H
#define SENSORBME280_H

#include <Arduino.h>
#include "devicebase.h"
#include "sensorBME280Structure.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "settings.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define XMETERTOFEET 3.28084

class SensorBME280 : public DeviceBase
{
public:
    SensorBME280();
    ~SensorBME280();

    TwoWire *I2Ctwo;

    bool begin(uint8_t addr);
    bool begin(uint8_t addr, TwoWire *theWire);
    void getBME280Readings(SensorBMe280Structure *aStrut);

private:
    Adafruit_BME280 bme;
    SensorBMe280Structure sensorReadingStrut;
};

#endif