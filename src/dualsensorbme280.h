#ifndef DUALSENSORBME280_H
#define DUALSENSORBME280_H

#include "sensorbme280.h"
#include "sensorBME280Structure.h"

class DualSensorBME280
{
public:
    DualSensorBME280();
    ~DualSensorBME280();

    void begin(void);
    void getIndoorBME280Readings(SensorBMe280Structure *aStrut);
    void getOutdoorBME280Readings(SensorBMe280Structure *aStrut);

private:
    TwoWire I2Cone = TwoWire(0);
    TwoWire I2Ctwo = TwoWire(1);
    bool debugDual;
    SensorBME280 indoorSensorBME280;
    SensorBME280 outdoorSensorBME280;
    SensorBMe280Structure indoorSensorStruct;
    SensorBMe280Structure outdoorSensorStruct;
};

#endif