#ifndef RORWEBSERVER_H
#define RORWEBSERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "sensor_readings.h"
#include "settings.h"
#include "ror_controller.h"


class RORWebServer {
  public:
    RORWebServer();
    ~RORWebServer(); // destructor declaratio

    boolean connectToWiFi();
    void startRORWebServer();
    void sendBME280Events(SensorBMe280_Struct aSensorReadingStrut);

    
  private:
    static void handleRoot(AsyncWebServerRequest *request);
    static void handleStyle(AsyncWebServerRequest *request);
    static void handleOn(AsyncWebServerRequest *request);
    static void handleOff(AsyncWebServerRequest *request);
    static void handleRORClosed(AsyncWebServerRequest *request);
    

    boolean debug;
    String sendString;

    AsyncWebServer *rorWebServer;
    AsyncWebSocket *rorWebSocket;

};

#endif
