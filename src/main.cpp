#include <Arduino.h>
#include "SPIFFS.h"
#include "sensor_readings.h"
#include "settings.h"
#include "rorwebserver.h"
#include "configwifisetup.h"

ConfigWiFiSetup configWiFiSetup;
SensorReadings sensorBME280Readings;
RORWebServer rorWebServer;
SensorBMe280_Struct mainSensorStruct;
ROR_Controller rorController;
String rorPositionStatusStr = "";
int pushDownCounter = 0;
boolean runAccessPointSetupMode = false; // is true every time the board is started as Access Point
boolean debugMain = true;

// BME280 Loop timers
unsigned long lastTime = 0;
unsigned long timerDelay = 3000; // send readings timer

void runGetUpdatesSendThemToClients();
void initBME280();
void initSPIFFS();
void initWebServer();
void updateBME280Reading();
void updateRORPosition();
void loop();

const int LONG_PRESS_TIME = 3000; // 1000 milliseconds

int lastState = LOW; // the previous state from the input pin
int currentState;    // the current reading from the input pin
unsigned long pressedTime = 0;
bool isPressing = false;
bool isLongDetected = false;

void checkIfModeButtonPushed()
{
  currentState = digitalRead(APSETUP_BUTTON_PIN); // read the state of the switch/button:

  if (lastState == HIGH && currentState == LOW)
  { // button is pressed
    pressedTime = millis();
    isPressing = true;
    isLongDetected = false;
  }
  else if (lastState == LOW && currentState == HIGH)
  { // button is released
    isPressing = false;
  }

  if (isPressing == true && isLongDetected == false)
  {
    long pressDuration = millis() - pressedTime;

    if (pressDuration > LONG_PRESS_TIME)
    {
      if (debugMain)
      {
        Serial.println("A long press is detected");
      }
      isLongDetected = true;
    }
  }

  lastState = currentState; // save the the last state
}

void setup()
{
  Serial.begin(115200);
  if (debugMain)
  {
    Serial.println("maincpp:setup");
  }
  pinMode(APSETUP_BUTTON_PIN, INPUT);
  pinMode(APSETUP_LED_PIN, OUTPUT);
  initSPIFFS();

  if (configWiFiSetup.isThereWiFiSetting())
  {
    initWebServer();
    initBME280();
    runGetUpdatesSendThemToClients();
  }
  else
  {
    if (debugMain)
    {
      Serial.println("maincpp:setup - run configWiFiSetup.runAPWebServerSetup() ");
    }
    configWiFiSetup.runAPWebServerSetup();
  }
}

void loop()
{
  checkIfModeButtonPushed();
  if (isLongDetected)
  {
    configWiFiSetup.clearWiFiSettings();
    ESP.restart();
  }
  rorWebServer.cleanUpClients();
  if ((millis() - lastTime) > timerDelay)
  {
    runGetUpdatesSendThemToClients();
    lastTime = millis();
  }
}

// ----------------------------------------------------------------------------
// SPIFFS initialization
// ----------------------------------------------------------------------------

void initSPIFFS()
{
  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

// ----------------------------------------------------------------------------
// RORWebServer initialization
// ----------------------------------------------------------------------------

void initWebServer()
{
  // Set up the web server
  if (debugMain)
  {
    Serial.println("Main.cpp -> initWebServer() rorWebServer.connectToWiFi() ");
  }
  rorWebServer.setSSID(configWiFiSetup.getSSID().c_str());
  rorWebServer.setPass(configWiFiSetup.getPass());
  rorWebServer.setIP(configWiFiSetup.getIP());
  rorWebServer.setSub(configWiFiSetup.getSub());
  rorWebServer.setGateway(configWiFiSetup.getGateway());
  if (rorWebServer.connectToWiFi())
  {
    Serial.println("Main.cpp -> initWebServer() rorWebServer.startRORWebServer() ");

    rorWebServer.initWebServer();
    rorWebServer.initWebSocket();
  }
  else
  {
    Serial.println("Main.cpp -> initWebServer() rorWebServer.connectToWiFi() Failed to Connect to WiFi");
  }
}

// ----------------------------------------------------------------------------
// BME280 initialization
// ----------------------------------------------------------------------------

void initBME280()
{
  if (debugMain)
  {
    Serial.println("Main.cpp -> Initialize BME280 sensor");
  }
  if (!sensorBME280Readings.begin())
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }
}

// ----------------------------------------------------------------------------
// updateBME280Reading Get sensor data and update RORWebServer with data
// ----------------------------------------------------------------------------
void updateBME280Reading()
{
  sensorBME280Readings.getBME280Readings(&mainSensorStruct);
  if (false)
  {
    Serial.println("main::updateBME280Reading() mainSensorStruct.altitude: " + mainSensorStruct.altitudeMeter);
    Serial.println("main::updateBME280Reading() mainSensorStruct.altitude: " + mainSensorStruct.altitudeFeet);
    Serial.println("main::updateBME280Reading() mainSensorStruct.humidity : " + mainSensorStruct.humidity);
    Serial.println("main::updateBME280Reading() mainSensorStruct.pressure : " + mainSensorStruct.pressure);
    Serial.println("main::updateBME280Reading() mainSensorStruct.temperature : " + mainSensorStruct.temperature);
  }
}

void updateRORPosition()
{
  rorPositionStatusStr = rorController.getRORPosistion();
  if (true)
  {
    Serial.println("maincpp: updateRORPosition() RoRPosition: " + rorPositionStatusStr);
  }
}

void runGetUpdatesSendThemToClients()
{
  updateBME280Reading();
  updateRORPosition();
  rorWebServer.setJsonValues(mainSensorStruct, rorPositionStatusStr);
  rorWebServer.cleanUpClients();
  rorWebServer.notifyClients();
}
