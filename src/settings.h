#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"

#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5

#define HTTP_PORT 80

#define RORSTATUS_PIN 26
#define APSETUP_BUTTON_PIN 2 // Switch button for setup mode to AP
#define APSETUP_LED_PIN 4       // LED for AP Setup Mode

#endif