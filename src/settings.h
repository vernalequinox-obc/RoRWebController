#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"

#define WEBUPDATE 3000
#define BME_SCK 18  // GIOP18 Used for weather sensor
#define BME_MISO 19 // GIOP19 Used for weather sensor
#define BME_MOSI 22 // GIOP22 Used for weather sensor
#define BME_CS 5    // GIOP05 Used for weather sensor

#define HTTP_PORT 80
const char STATIC_LOCAL_IP[] = {192, 168, 0, 219};
const char STATIC_GATEWAY_IP[] = {192, 168, 0, 1};
const char STATIC_SUBNET[] = {255, 255, 255, 0};

// Buttons
#define OSC_PUSHBUTTON_INPUTPIN 39     // GIOP39 - Push button switch to mimic Aleko Gate Controller keyfob or OSC button
#define APSETUP_PUSHBUTTON_INPUTPIN 36 // GIOP36 - Switch button for setup mode to AP

// Sensor Switches
#define ROOF_OPEN_SWITCH_INPUTPIN 35        // GIOP35   roof open sensor
#define ROOF_CLOSE_SWITCH_INPUTPIN 34       // GIOP34   roof closed sensor
#define SCOPE_MOUNT_SAFE_SWITCH_INPUTPIN 33 // GIOP33   scope safety Park Sensor also Arduino built in LED

// Relays
#define SCOPE_MOUNT_PARK_SAFE_RELAY_OUTPUTPIN 27 // GIOP27  Scope safe relay for Aleko. This relay is energerized (outpin LOW engaged) in the safe posistion.
                                                 //         The relay when energized will complete the motor circuit. When non-energized the motor does not function.
#define OCS_PULSE_RELAY_OUTPUTPIN 26             // GIOP26  Relay toggles a momentary pulse acking like a key bod or momentery button to be used with Aleko.

// LED Displays
#define SCOPE_MOUNT_PARK_SAFE_LED 25      // GIOP25 - LED for when the scope is parked and safe
#define SCOPE_MOUNT_PARK_NOT_SAFE_LED 23  // GIOP23 - LED for when the scope is NOT parked and unsafe to move roof
#define APSETUP_LED 4                     // GIOP04 - LED for AP Setup Mode
#define WIFI_NORMAL_OPERATION_LED 32      // GIOP32 - LED for connected to local network
#define ROOF_CLOSED_LED 17                // GIOP17 - LED for when the closed sensor is on as roof is closed
#define ROOF_OPEN_LED 16                  // GIOP16 - LED for when the open sensor is on as roof is opened
#define ROOF_MOVING_LED 15                // GIOP15 - LED for when the roof is moving
#define ROOF_UNKNOWN_LED 14               // GIOP14 - LED for when the roof is unknown lost
#define OSC_BUTTON_LED 13                 // GIOP13 - LED for when the OSC button is press
#define ROOF_MOTOR_ENGAGE_RELAY_LED 4     // GIOP4  - LED for when the Aleko Gate Control Motor power is enable or disable






#endif
