#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "ArduinoJson.h"
#include "SPIFFS.h"

#define VERSION 0.15
#define WEBUPDATE 3000
const char ObservertoryName[] = "Vernal Equinox";

#define HOLD_BUTTON_DOWN_THRESHOLD_ALEKO 2000      // For Aleko. How long a button must be held before it is triggered.
#define TRIGGER_PULSE_DURATION_ALEKO 25            // For Aleko, The pulse width once the button is triggered.
#define TRIGGER_DURATION 500                       // For Aleko. Trigger duration in milliseconds used to send a pulse to the Relay Aleko controller.
                                                   // This is for both the physical button and from the WebSite button.
#define DISABLE_TRIGGER_PULSE_BUTTON_DURATION 8000 // For Aleko. Once a relay trigger is pulsed, how long before the button becomes ready
                                                   // active again for being pressed. If pressed many times and too fast the roof could jerk.
#define RELAY_TRIGGER_PULSE_DURATION 750           // For Aleko. How long the relay will stay engaged to simulate the Aleko button being pushed or keyfob

#define HOLD_BUTTON_DOWN_THRESHOLD_AP 3000 // For AP setup, how long a button must be held down before it triggers.

#define BUTTON_DEBOUNCE_TIME 125 // How long a switch or button must be closed for debouncing before it is offically considered closed.

#define HTTP_PORT 80
const char STATIC_LOCAL_IP[] = {192, 168, 0, 219}; // Default for AP Setup IP address for setting your WiFi network
const char STATIC_GATEWAY_IP[] = {192, 168, 0, 1}; // Default for AP Setup Gateway IP or setting your WiFi network
const char STATIC_SUBNET[] = {255, 255, 255, 0};   // Default for AP Setup SubNet Mask for setting your WiFi network

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
#define SCOPE_MOUNT_PARK_SAFE_LED 25     // GIOP25 - LED for when the scope is parked and safe
#define SCOPE_MOUNT_PARK_NOT_SAFE_LED 23 // GIOP23 - LED for when the scope is NOT parked and unsafe to move roof
#define APSETUP_LED 4                    // GIOP04 - LED for AP Setup Mode
#define WIFI_NORMAL_OPERATION_LED 12     // GIOP12 - LED for connected to local network
#define ROOF_CLOSED_LED 15               // GIOP17 - LED for when the closed sensor is on as roof is closed
#define ROOF_OPEN_LED 14                 // GIOP16 - LED for when the open sensor is on as roof is opened
#define ROOF_MOVING_LED 32               // GIOP32 - 

#define OSC_BUTTON_LED 13                // GIOP13 - LED for when the OSC button is press

#define BME_SCL_1 22 // GIOP22 Used for weather indoor sensor
#define BME_SDA_1 21 // GIOP21 Used for weather indoor sensor
#define BME_SCL_2 19 // GIOP19 Used for weather outdoor sensor
#define BME_SDA_2 18 // GIOP18 Used for weather outdoor sensor





#endif
