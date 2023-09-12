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
  boolean runAccessPointSetupMode = false;
  unsigned long lastTime = 0;
  unsigned long timerDelay = WEBUPDATE;

  LedLight localWiFIConnected_LED;
  InputButtonLED apModeButton;

  boolean debugMain = false;

public:
  void setup()
  {
    Serial.begin(115200);
    // Initialize LED lights and button
    localWiFIConnected_LED.setDevicePin(WIFI_NORMAL_OPERATION_LED);
    localWiFIConnected_LED.setDeviceName("localWiFIConnected_LED");
    // localWiFIConnected_LED.setDebug(true);
    localWiFIConnected_LED.begin();

    apModeButton.setButtonLedPin(APSETUP_PUSHBUTTON_INPUTPIN, APSETUP_LED);
    apModeButton.InputButton::setDeviceName("apModeButton");
    apModeButton.LedLight::setDeviceName("apMode_LED");
    apModeButton.setDebounceTime(HOLD_BUTTON_DOWN_THRESHOLD_AP);
    apModeButton.begin();
    initSPIFFS();
    bool isThereWiFiSetting = false;
    bool isConnected = false;
    isThereWiFiSetting = configWiFiSetup.isThereWiFiSetting();
    if (isThereWiFiSetting)
    {
      int loopCount = 0;
      while (loopCount < CONNECTING_WIFI_ATTEMPTS && !isConnected)
      {
        isConnected = initWebServer();
      }
      if (isConnected)
      {
        apModeButton.LedLight::updateLed(LOW);
        localWiFIConnected_LED.updateLed(HIGH);
        readingDualSensorBME280.begin();
        runGetUpdatesSendThemToClients();
      }
    }
    if (!isThereWiFiSetting || !isConnected)
    {
      apModeButton.LedLight::updateLed(HIGH);
      localWiFIConnected_LED.updateLed(LOW);
      configWiFiSetup.runAPWebServerSetup();
    }
  }

  void loop()
  {
    if (debugMain)
    {
      Serial.println("maincpp:loop()");
    }
    rorController.updatedInputSensorsButtons();
    apModeButton.updateButtonPin();

    if (apModeButton.isPressed())
    {
      if (debugMain || apModeButton.getDebugButton())
      {
        Serial.println("maincpp::loop - if (apModeButton.isPressed()) is - true ");
      }
      apModeButton.LedLight::updateLed(HIGH);
      localWiFIConnected_LED.updateLed(LOW);
      configWiFiSetup.clearWiFiSettings();
      ESP.restart();
    }
    else
    {
      if (debugMain || apModeButton.getDebugButton())
      {
        Serial.println("maincpp::loop - if (apModeButton.isPressed()) is - false ");
      }
      apModeButton.LedLight::updateLed(LOW);
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

  void runGetUpdatesSendThemToClients()
  {
    if (debugMain)
    {
      Serial.println("Main.cpp -> runGetUpdatesSendThemToClients()");
    }
    ROR_Status currentRorStatus;
    readingDualSensorBME280.getIndoorBME280Readings(&indoorSensorStruct);
    readingDualSensorBME280.getOutdoorBME280Readings(&outdoorSensorStruct);
    rorController.getRORStatus(currentRorStatus);
    rorWebServer.setJsonValues(&indoorSensorStruct, &outdoorSensorStruct, &currentRorStatus);
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
