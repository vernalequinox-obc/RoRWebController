#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"

#define BME_SCK 18  // GIOP18 Used for weather sensor
#define BME_MISO 19 // GIOP19 Used for weather sensor
#define BME_MOSI 23 // GIOP23 Used for weather sensor
#define BME_CS 5    // GIOP5 Used for weather sensor

#define HTTP_PORT 80

#define RORSTATUS_PIN 16
#define APSETUP_BUTTON_PIN 25 // GIOP26 - Switch button for setup mode to AP
#define APSETUP_LED_PIN 26    // GIOP25 - LED for AP Setup Mode
#define NORMAL_WIFI_LED_PIN 13    // GIOP13 - LED for connected to local network

// ----------------------------------------------------------------------------
// Definition of the Led component
// ----------------------------------------------------------------------------

struct LedStruct {
    // state variables
    uint8_t pin;
    bool    on;

    // methods
    void update() {
        digitalWrite(pin, on ? HIGH : LOW);
    }
};

#endif