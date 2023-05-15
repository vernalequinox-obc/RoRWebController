#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"

#define BME_SCK 18  // GIOP18 Used for weather sensor
#define BME_MISO 19 // GIOP19 Used for weather sensor
#define BME_MOSI 22 // GIOP22 Used for weather sensor
#define BME_CS 5    // GIOP05 Used for weather sensor

#define HTTP_PORT 80

// Buttons
#define INPUT_PIN_OSC_BUTTON 39 // GIOP39
#define INPUT_APSETUP_BUTTON 36 // GIOP36 - Switch button for setup mode to AP

// ROR Controller Defines
#define INPUT_PIN_OPENED_SENSOR 35            // GIOP35   roof open sensor
#define INPUT_PIN_CLOSED_SENSOR 34            // GIOP34   roof closed sensor
#define INPUT_PIN_SCOPE_PARKED_SAFE_SENSOR 33 // GIOP33   scope safety Park Sensor also Arduino built in LED

#define OUTPUT_RELAY_SCOPE_PARK_SAFE 27 // GIOP27  scope safe for Aleko acting like LM102 or gragedoor beam
#define OUTPUT_RELAY_OCS_MOMENTARY 26   // GIOP26  toggleS like a momentary push button to be used with Aleko or garagedoor opener systems

// LED Displays
#define OUTPUT_SCOPE_PARKED_SAFE_LED 25       // GIOP25 - LED for when the scope is parked and safe
#define OUTPUT_SCOPE_NOT_PARKED_UNSAFE_LED 23 // GIOP23 - LED for when the scope is NOT parked and unsafe to move roof
#define OUTPUT_APSETUP_LED 4                  // GIOP04 - LED for AP Setup Mode
#define OUTPUT_NORMAL_WIFI_LED 21             // GIOP21 - LED for connected to local network
#define OUTPUT_CLOSED_LED 17                  // GIOP17 - LED for when the closed sensor is on as roof is closed
#define OUTPUT_OPENED_LED 16                  // GIOP16 - LED for when the open sensor is on as roof is opened
#define OUTPUT_MOVING_LED 15                  // GIOP15 - LED for when the roof is moving
#define OUTPUT_UNKNOWN_LED 14                 // GIOP14 - LED for when the roof is unknown lost
#define OUTPUT_OSC_BUTTON_LED 13              // GIOP13 - LED for when the OSC button is press

const uint8_t DEBOUNCE_DELAY = 10; // Used by Button class for debounce delay in milliseconds

// ----------------------------------------------------------------------------
// Definition of the Led component
// ----------------------------------------------------------------------------



class LedLight
{
private:
  // state variables
  uint8_t espPin;

public:
  char displayName[16];
  bool on = false;
  void begin(uint8_t aEspPinNumber, const char *aName)
  {
    begin(aEspPinNumber);
    strncpy(displayName, aName, sizeof(displayName));
  }
  void begin(uint8_t aEspPinNumber)
  {
    espPin = aEspPinNumber;
    pinMode(espPin, OUTPUT);
  }
  void update(bool aOn)
  {
    on = aOn;
    digitalWrite(espPin, on ? HIGH : LOW);
  }
};

/*


How to use

Button btn1;
Button btn2;

void setup() {
  btn1.begin(2);
  btn2.begin(3);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // press button 1 to turn on the LED
  if (btn1.debounce()) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  // press button 2 to turn off the LED
  if (btn2.debounce()) {
    digitalWrite(LED_BUILTIN, LOW);
  }
}


*/

class DebounceIt
{
private:
  uint8_t espPin;
  uint16_t state;
  char displayName[16];

public:
  void begin(uint8_t aEspPinNumber, const char *aName)
  {
    begin(aEspPinNumber);
    strncpy(displayName, aName, sizeof(displayName));
  }
  void begin(uint8_t aEspPinNumber)
  {
    espPin = aEspPinNumber;
    state = 0;
    pinMode(espPin, INPUT);
  }
  bool debounce()
  {
    state = (state << 1) | digitalRead(espPin) | 0xfe00;
    if (false)
    {
      Serial.print("DebounceIt: ");
      Serial.print(displayName);
      Serial.print("state: ");
      Serial.println(state == 0xff00);
    }
    return (state == 0xff00);
  }
};

struct Button
{
  // state variables
  uint8_t pin;
  bool lastReading;
  uint32_t lastDebounceTime;
  uint16_t state;

  // methods determining the logical state of the button
  bool pressed()
  {
    if (false)
    {
      Serial.print("pressed()");
      Serial.print("state: ");
      Serial.println(state == 1);
    }
    return state == 1;
  }
  bool released()
  {
    if (false)
    {
      Serial.print("Button released()");
      Serial.print("state: ");
      Serial.println(state == 0xffff);
    }
    return state == 0xffff;
  }
  bool held(uint16_t count = 0)
  {
    if (false)
    {
      Serial.print("Button held()");
      Serial.print("state: ");
      Serial.println(state > 1 + count && state < 0xffff);
    }
    return state > 1 + count && state < 0xffff;
  }

  // method for reading the physical state of the button
  void read()
  {
    // reads the voltage on the pin connected to the button
    bool reading = digitalRead(pin);

    // if the logic level has changed since the last reading,
    // we reset the timer which counts down the necessary time
    // beyond which we can consider that the bouncing effect
    // has passed.
    if (reading != lastReading)
    {
      lastDebounceTime = millis();
    }

    // from the moment we're out of the bouncing phase
    // the actual status of the button can be determined
    if (millis() - lastDebounceTime > DEBOUNCE_DELAY)
    {
      // don't forget that the read pin is pulled-up
      bool pressed = reading == LOW;
      if (pressed)
      {
        if (state < 0xfffe)
          state++;
        else if (state == 0xfffe)
          state = 2;
      }
      else if (state)
      {
        state = state == 0xffff ? 0 : 0xffff;
      }
    }

    // finally, each new reading is saved
    lastReading = reading;
  }
};

#endif