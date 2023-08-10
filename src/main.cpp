#include <Arduino.h>
#include "SPIFFS.h"
#include "sensorBME280Structure.h"
#include "dualsensorbme280.h"
#include "settings.h"
#include "rorwebserver.h"
#include "configwifisetup.h"
#include "ror_controller.h"

class RoRControllerApp
{
private:
  ConfigWiFiSetup configWiFiSetup;
  RORWebServer rorWebServer;
  DualSensorBME280 readingDualSensorBME280;
  SensorBMe280Structure indoorSensorStruct;
  SensorBMe280Structure outdoorSensorStruct;
  ROR_Controller rorController;

  char rorPositionStatusStr[20] = "";
  boolean runAccessPointSetupMode = false;
  unsigned long lastTime = 0;
  unsigned long timerDelay = WEBUPDATE;

  LedLight localWiFIConnected_LED;
  LedLight apMode_LED;
  InputButton apModeButton;

  boolean debugMain = false;

public:
  void setup()
  {
    Serial.begin(115200);
    if (debugMain)
    {
      Serial.println("main -> setup()");
    }

    // Initialize LED lights and button
    localWiFIConnected_LED.setDevicePin(WIFI_NORMAL_OPERATION_LED);
    localWiFIConnected_LED.setDeviceName("localWiFIConnected_LED");
    // localWiFIConnected_LED.setDebug(true);
    localWiFIConnected_LED.begin();

    apMode_LED.setDevicePin(APSETUP_LED);
    apMode_LED.setDeviceName("apMode_LED");
    // apMode_LED.setDebug(true);
    apMode_LED.begin();

    apModeButton.setDeviceEnabled(true); // **************** Disabled for coding and needs to be Enabled for running.
    apModeButton.setDevicePin(APSETUP_PUSHBUTTON_INPUTPIN);
    apModeButton.setDeviceName("apModeButton");
    apModeButton.setDebounceTime(3000);
    // apModeButton.setDebug(true);
    apModeButton.begin();


    initSPIFFS();

    if (configWiFiSetup.isThereWiFiSetting())
    {
      if (debugMain || true)
      {
        Serial.println("maincpp:setup - connect to local Wifi and Start Webserver");
      }

      if (initWebServer())
      {
        apMode_LED.updateLed(LOW);
        localWiFIConnected_LED.updateLed(HIGH);
        initBME280();
        runGetUpdatesSendThemToClients();
      }
    }
    else
    {
      if (debugMain || true)
      {
        Serial.println("maincpp:setup - run configWiFiSetup.runAPWebServerSetup() ");
      }

      apMode_LED.updateLed(HIGH);
      localWiFIConnected_LED.updateLed(LOW);
      configWiFiSetup.runAPWebServerSetup();
    }
  }

  void loop()
  {
    rorController.updatedInputSensorsButtons();
    apModeButton.updateButtonPin();

    if (apModeButton.isPressed())
    {
      if (debugMain || apModeButton.getDebug())
      {
        Serial.println("maincpp::loop - if (apModeButton.isPressed()) is - true ");
      }
      apMode_LED.updateLed(HIGH);
      localWiFIConnected_LED.updateLed(LOW);
      configWiFiSetup.clearWiFiSettings();
      ESP.restart();
    }
    else
    {
      if (debugMain || apModeButton.getDebug())
      {
        Serial.println("maincpp::loop - if (apModeButton.isPressed()) is - false ");
      }
      apMode_LED.updateLed(LOW);
      localWiFIConnected_LED.updateLed(HIGH);
    }

    rorWebServer.cleanUpClients();
    if ((millis() - lastTime) > timerDelay)
    {
      runGetUpdatesSendThemToClients();
      lastTime = millis();
    }
  }

private:
  void initSPIFFS()
  {
    if (debugMain)
    {
      Serial.println("Main.cpp -> initSPIFFS()");
    }
    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
  }

  bool initWebServer()
  {
    // Set up the web server
    if (debugMain)
    {
      Serial.println("Main.cpp -> initWebServer() setup rorWebServer ");
    }
    rorWebServer.setSSID(configWiFiSetup.getSSID());
    rorWebServer.setPass(configWiFiSetup.getPass());
    rorWebServer.setIP(configWiFiSetup.getIP());
    rorWebServer.setSub(configWiFiSetup.getSub());
    rorWebServer.setGateway(configWiFiSetup.getGateway());
    if (rorWebServer.connectToWiFi())
    {
      if (debugMain)
      {
        Serial.println("Main.cpp -> initWebServer() rorWebServer.connectToWiFi() is connected start WebServer and WebSocket");
      }
      rorWebServer.initWebServer();
      rorWebServer.initWebSocket();
      return true;
    }
    else
    {
      if (debugMain)
      {
        Serial.println("Main.cpp -> initWebServer() rorWebServer.connectToWiFi() Failed to Connect to WiFi");
      }
    }
    return false;
  }

  void initBME280()
  {
    readingDualSensorBME280.begin();
  }

  void runGetUpdatesSendThemToClients()
  {
    if (debugMain)
    {
      Serial.println("Main.cpp -> runGetUpdatesSendThemToClients()");
    }
    ROR_Status *ptrRoRStatusStruct;
    readingDualSensorBME280.getIndoorBME280Readings(&indoorSensorStruct);
    readingDualSensorBME280.getOutdoorBME280Readings(&outdoorSensorStruct);
    ptrRoRStatusStruct = rorController.getRORStatus();
    rorWebServer.setJsonValues(&indoorSensorStruct, &outdoorSensorStruct, ptrRoRStatusStruct->rorCurrentPosition, ptrRoRStatusStruct->IsScopeParkSafe);
    rorWebServer.cleanUpClients();
    rorWebServer.notifyClients();
    if (rorWebServer.getIsOSCpulseTriggered())
    {
      rorController.setIsEngagedRelayPulseTrue();
      rorWebServer.resetIsOSCpulseTriggered();
    }
  }
};

RoRControllerApp rorControllerApp1;

void setup()
{
  rorControllerApp1.setup();
}

void loop()
{
  rorControllerApp1.loop();
}
