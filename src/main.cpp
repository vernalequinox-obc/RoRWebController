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
    bool isConnected = false;
    bool isThereWiFiSetting = configWiFiSetup.isThereWiFiSetting();
    // Check if the apModeButton is pressed during setup
    if (isThereWiFiSetting)
    {
      bool wasApModeButtonPressed = false;
      while (!isConnected)
      {
        localWiFIConnected_LED.updateLed(HIGH);
        apModeButton.LedLight::updateLed(LOW);
        isConnected = initWebServer();
        if (!isConnected)
        {
          int counter = 0;
          while (counter <= 4)  // Flash the LEDs to signal no connection or bad credentials and give time to hold the apModeButton down.
          {
            localWiFIConnected_LED.updateLed(LOW);
            apModeButton.LedLight::updateLed(HIGH);
            delay(150);
            localWiFIConnected_LED.updateLed(HIGH);
            apModeButton.LedLight::updateLed(LOW);
            delay(150);
            wasApModeButtonPressed = isApModeHeldDownAtStartupDebounce();
            if (wasApModeButtonPressed)
            {
              apModeButton.LedLight::updateLed(HIGH);
              localWiFIConnected_LED.updateLed(LOW);
              configWiFiSetup.runAPWebServerSetup();
            }
            ++counter;
          }
        }
      }
      if (isConnected)
      {
        apModeButton.LedLight::updateLed(LOW);
        localWiFIConnected_LED.updateLed(HIGH);
        readingDualSensorBME280.begin();
        runGetUpdatesSendThemToClients();
      }
    }
    else
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

  bool isApModeHeldDownAtStartupDebounce()
  {
    unsigned long startupTime = millis();
    int buttonState = apModeButton.getDigitalRead(); // Initial state of the button
    while (millis() - startupTime < BUTTON_DEBOUNCE_TIME)
    {
      // Check the button state continuously for a specified time (BUTTON_HOLD_TIME)
      if (buttonState == 1)
      {
        // Button is released at any point, return false
        return false;
      }
      buttonState = apModeButton.getDigitalRead(); // Update button state
    }
    // Button was held down continuously for BUTTON_HOLD_TIME, return true
    return true;
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
    rorWebServer.setSSID(configWiFiSetup.getSSID());
    rorWebServer.setPass(configWiFiSetup.getPass());
    rorWebServer.setIP(configWiFiSetup.getIP());
    rorWebServer.setSub(configWiFiSetup.getSub());
    rorWebServer.setGateway(configWiFiSetup.getGateway());
    if (rorWebServer.connectToWiFi())
    {
      Serial.println("WiFi is connected start WebServer and WebSocket");
      rorWebServer.initWebServer();
      rorWebServer.initWebSocket();
      return true;
    }
    Serial.println("WiFi Failed to Connect");
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
