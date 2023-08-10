#include "dualsensorbme280.h"

DualSensorBME280::DualSensorBME280()
{

    indoorSensorBME280.setDeviceName("indoorSensorBME280");
    // indoorSensorBME280.setDebug(true);
    outdoorSensorBME280.setDeviceName("outdoorSensorBME280");
    // outdoorSensorBME280.setDebug(true);
    debugDual = false;
}

DualSensorBME280::~DualSensorBME280()
{
}

void DualSensorBME280::begin(void)
{
    I2Cone.begin(BME_SDA_1, BME_SCL_1, 100000);
    I2Ctwo.begin(BME_SDA_2, BME_SCL_2, 100000);

    if (debugDual)
    {
        Serial.println("DualSensorBME280::begin(void) Initialize BME280 sensor");
    }
    if (!indoorSensorBME280.begin(0x76, &I2Cone))
    {
        if (debugDual)
        {
            Serial.println("DualSensorBME280::begin(void) No valid indoor BME280 sensor, check wiring!");
        }
    }

    if (!outdoorSensorBME280.begin(0x76, &I2Ctwo))
    {
        if (debugDual)
        {
            Serial.println("DualSensorBME280::begin(void) No valid outdoor BME280 sensor, check wiring!");
        }
    }
}

void DualSensorBME280::getIndoorBME280Readings(SensorBMe280Structure *aStrut)
{
    indoorSensorBME280.getBME280Readings(aStrut);
}

void DualSensorBME280::getOutdoorBME280Readings(SensorBMe280Structure *aStrut)
{
    outdoorSensorBME280.getBME280Readings(aStrut);
}
