#include "sensor_readings.h"

SensorReadings::SensorReadings() {
    debug = false;
}

SensorReadings::~SensorReadings() {}

bool SensorReadings::begin() {
    return bme.begin(0x76);
}

void SensorReadings::getBME280Readings(SensorBMe280_Struct *aStruct) {
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
