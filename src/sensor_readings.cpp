#include "sensor_readings.h"

SensorReadings::SensorReadings()
{
  Adafruit_BME280 bme; // 0x76 is the I2C address of the BME280 sensor
  debug = false;
  sensorReadingStrut = {0, 0, 0, 0};
}

// Destructor
SensorReadings::~SensorReadings()
{
}

bool SensorReadings::begin()
{
  return bme.begin(0x76); // Initialize the BME280 sensor with its I2C address
}

SensorBMe280_Struct SensorReadings::getBME280Readings(SensorBMe280_Struct aStrut)
{
  if (debug)
  {
    Serial.println("sensor_readings -> SensorReadings::getBME280Readings()");
  }

  aStrut.f_temperature = sensorReadingStrut.f_temperature = (1.8 * bme.readTemperature() + 32);
  aStrut.f_pressure  = sensorReadingStrut.f_pressure = bme.readPressure() / 100.0F;
  aStrut.f_humidity = sensorReadingStrut.f_humidity = bme.readHumidity();

  if (debug)
  {
    String temp = String(sensorReadingStrut.f_temperature).c_str();
    Serial.println("sensor_readings->SensorReadings::getBME280Readings() sensorReadingStrut.f_temperature: " + temp);
    String hum = String(sensorReadingStrut.f_humidity).c_str();
    Serial.println("sensor_readings->SensorReadings::getBME280Readings() sensorReadingStrut.f_humidity : " + hum);
    String press = String(sensorReadingStrut.f_pressure).c_str();
    Serial.println("sensor_readings->SensorReadings::getBME280Readings() sensorReadingStrut.f_pressure : " + press);
  }
  return aStrut;
}

String SensorReadings::processor(const String &var)
{
  /*
  if(var == "TEMPERATURE"){
    return String(sensorReadingStrut.f_temperature);
  }
  else if(var == "HUMIDITY"){
    return String(sensorReadingStrut.f_humidity);
  }
  else if(var == "PRESSURE"){
    return String(sensorReadingStrut.f_pressure);
  }
  */
  return String();
}
