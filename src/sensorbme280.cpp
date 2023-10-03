
#include "sensorbme280.h"

SensorBME280::SensorBME280()
{
    deviceDebug = false;
}

SensorBME280::~SensorBME280()
{
}

bool SensorBME280::begin(uint8_t addr)
{
    return bme.begin(addr);
}

bool SensorBME280::begin(uint8_t addr, TwoWire *theWire)
{
    return bme.begin(addr, theWire);
}

void SensorBME280::getBME280Readings(SensorBMe280Structure *aStruct)
{
    // Temperature
    float temperature = (1.8 * bme.readTemperature() + 32);
    snprintf(aStruct->temperature, sizeof(aStruct->temperature), "%.2f", temperature);

    // Humidity
    float humidity = bme.readHumidity();
    snprintf(aStruct->humidity, sizeof(aStruct->humidity), "%.2f", humidity);

    // Pressure
    float pressure = bme.readPressure() / 100.0;
    snprintf(aStruct->pressure, sizeof(aStruct->pressure), "%.2f", pressure);

    // Altitude (in meters)
    float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    snprintf(aStruct->altitudeMeter, sizeof(aStruct->altitudeMeter), "%.2f", altitude);

    // Altitude (in feet)
    float altitudeFeet = altitude * XMETERTOFEET;
    snprintf(aStruct->altitudeFeet, sizeof(aStruct->altitudeFeet), "%.2f", altitudeFeet);
}