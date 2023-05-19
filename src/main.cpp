#include <Arduino.h>
#include "SPIFFS.h"
#include "sensor_readings.h"
#include "settings.h"
#include "rorwebserver.h"
#include "configwifisetup.h"
#include "ror_controller.h"

ConfigWiFiSetup configWiFiSetup;
SensorReadings sensorBME280Readings;
RORWebServer rorWebServer;
SensorBMe280_Struct mainSensorStruct;
ROR_Controller rorController;
char rorPositionStatusStr[20] = "";
boolean runAccessPointSetupMode = false; // is true every time the board is started as Access Point
boolean debugMain = true;


// BME280 Loop timers
unsigned long lastTime = 0;
unsigned long timerDelay = WEBUPDATE; // send readings timer

void runGetUpdatesSendThemToClients();
void initBME280();
void initSPIFFS();
bool initWebServer();
void updateBME280Status();
void updateRORStatus();
void loop();
void printMemory();

const int LONG_PRESS_TIME = 3000; // 1000 milliseconds

int lastState = LOW; // the previous state from the input pin
int currentState;    // the current reading from the input pin
unsigned long pressedTime = 0;
bool isPressing = false;
bool isLongDetected = false;
LedLight apMode_LED;
LedLight localWiFIConnected_LED;

// LedLight localWiFIConnected; = {NORMAL_WIFI_LED, false};

void checkIfModeButtonPushed()
{
  currentState = digitalRead(INPUT_APSETUP_BUTTON); // read the state of the switch/button:

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

  apMode_LED.begin(OUTPUT_APSETUP_LED, "ROR Wi-Fi Manager");
  localWiFIConnected_LED.begin(OUTPUT_NORMAL_WIFI_LED, "WiFi Local Network");

  pinMode(INPUT_APSETUP_BUTTON, INPUT);

  initSPIFFS();

  if (configWiFiSetup.isThereWiFiSetting())
  {
    if (debugMain)
    {
      Serial.println("maincpp:setup - connect to local Wifi and Start Webserver");
    }

    if (initWebServer())
    {
      apMode_LED.update(false);
      localWiFIConnected_LED.update(true);
      initBME280();
      runGetUpdatesSendThemToClients();
    }
  }
  else
  {
    if (debugMain)
    {
      Serial.println("maincpp:setup - run configWiFiSetup.runAPWebServerSetup() ");
    }

    apMode_LED.update(true);
    localWiFIConnected_LED.update(false);
    configWiFiSetup.runAPWebServerSetup();
  }
}

void loop()
{
  // rem this out later once finished to find memory
  // printMemory();
  rorController.updatedInputSensorsButtons();
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

bool initWebServer()
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
    return true;
  }
  else
  {
    Serial.println("Main.cpp -> initWebServer() rorWebServer.connectToWiFi() Failed to Connect to WiFi");
  }
  return false;
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
    if (debugMain)
    {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
    }
  }
}

void runGetUpdatesSendThemToClients()
{
  if (debugMain)
  {
    Serial.println("Main.cpp -> runGetUpdatesSendThemToClients()");
  }
  RORStatus_Struct *ptrRoRStatusStruct;
  sensorBME280Readings.getBME280Readings(&mainSensorStruct);
  ptrRoRStatusStruct = rorController.getRORStatus();
  rorWebServer.setJsonValues(mainSensorStruct, ptrRoRStatusStruct->rorCurrentPosition, ptrRoRStatusStruct->ScopeParkSafe);
  rorWebServer.cleanUpClients();
  rorWebServer.notifyClients();
}

// Remove this once the program is finished it just print memory left
void printMemory()
{
  static unsigned long lastTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= 5000)
  { // Print every 5 seconds
    lastTime = currentTime;

    // Get free heap memory
    uint32_t freeHeap = ESP.getFreeHeap();
    Serial.print("Free Heap Memory: ");
    Serial.print(freeHeap);
    Serial.println(" bytes");
  }
}