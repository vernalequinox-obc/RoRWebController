#ifndef SETTINGS_H
#define SETTINGS_H

#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5

#define HTTP_PORT 80

const char ssid[]  = "Gentry2";
const char password[] = "LittleJack";
const IPAddress local_IP = IPAddress(192, 168, 0, 219);
const IPAddress gateway = IPAddress(192, 168, 0, 1);
const IPAddress subnet = IPAddress(255, 255, 255, 0);

#endif