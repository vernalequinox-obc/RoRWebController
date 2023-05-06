#include "sensor_readings.h"

SensorReadings::SensorReadings()
{
  Adafruit_BME280 bme; // 0x76 is the I2C address of the BME280 sensor
  debug = false;
}

// Destructor
SensorReadings::~SensorReadings()
{
}

bool SensorReadings::begin()
{
  return bme.begin(0x76); // Initialize the BME280 sensor with its I2C address
}

void SensorReadings::getBME280Readings(SensorBMe280_Struct *aStrut)
{
  if (debug)
  {
    Serial.println("sensor_readings -> SensorReadings::getBME280Readings()");
  }

  float f_temperature = (1.8 * bme.readTemperature() + 32);
  float f_pressure  = bme.readPressure() / 100.0F;
  float f_humidity = bme.readHumidity();
  float f_altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  aStrut->altitudeFeet = String( (f_altitude * XMETERTOFEET));
  aStrut->altitudeMeter = String(f_altitude).c_str();
  aStrut->humidity = String(f_humidity).c_str();
  aStrut->pressure = String(f_pressure).c_str();
  aStrut->temperature = String(f_temperature).c_str();

  if (debug)
  {
    Serial.println("sensor_readings->SensorReadings::getBME280Readings() aStrut.altitude: " + aStrut->altitudeMeter);
    Serial.println("sensor_readings->SensorReadings::getBME280Readings() aStrut.altitude: " + aStrut->altitudeFeet);
    Serial.println("sensor_readings->SensorReadings::getBME280Readings() aStrut.humidity : " + aStrut->humidity);
    Serial.println("sensor_readings->SensorReadings::getBME280Readings() aStrut.pressure : " + aStrut->pressure);
    Serial.println("sensor_readings->SensorReadings::getBME280Readings() aStrut.temperature : " + aStrut->temperature);
  }
  return;
}


