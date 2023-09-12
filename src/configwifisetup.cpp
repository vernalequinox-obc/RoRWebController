#include "configwifisetup.h"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ROR Wi-Fi Manager</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta name="apple-mobile-web-app-capable" content="yes">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
  <div class="topnav">
    <h1>ROR Wi-Fi Manager</h1>
  </div>
  <div class="content">
    <div class="card-grid">
      <div class="card">
        <form action="/" method="POST">
          <p>
            <label for="ssid">SSID:</label>
            <input type="text" id ="ssid" name="ssid"><br>
            <label for="pass">Password:</label>
            <input type="text" id ="pass" name="pass"><br>
            <label for="ip">IP Address:</label>
            <input type="text" id ="ip" name="ip" value="192.168.0.200"><br>
            <label for="sub">Subnet:</label>
            <input type="text" id ="sub" name="sub" value="255.255.255.0"><br>
            <label for="gateway">Gateway:</label>
            <input type="text" id ="gateway" name="gateway" value="192.168.0.1"><br>
            <input type ="submit" value ="Submit">
          </p>
        </form>
      </div>
    </div>
  </div>
</body>
<style>
html {
  font-family: Arial, Helvetica, sans-serif; 
  display: inline-block; 
  text-align: center;
}
h1 {
  font-size: 1.5rem; 
  color: white;
}
p { 
  font-size: 1rem;
}
.topnav { 
  overflow: hidden; 
  background-color: #0A1128;
}
body {  
  margin: 0;
}
.content { 
  padding: 10px;
}
.card-grid { 
  max-width: 325px; 
  margin: 0 auto; 
  display: grid; 
  grid-template-columns: 300px;
}
.card { 
  background-color: white; 
  box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
  padding: 1px 1px;
}
.card-title { 
  font-size: 1.2rem;
  font-weight: bold;
  color: #034078
}
input[type=submit] {
  border: none;
  color: #FEFCFB;
  background-color: #034078;
  padding: 8px 5px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
  width: 90px;
  margin-right: 10px;
  border-radius: 4px;
  transition-duration: 0.4s;
  }
input[type=submit]:hover {
  background-color: #1282A2;
}
input[type=text], input[type=number], select {
  width: 150px;
  padding: 8px 20px;
  margin: 2px;
  display: inline-block;
  border: 1px solid #ccc;
  border-radius: 4px;
  box-sizing: border-box;
}
label {
  display: inline-block;
  width: 90px;
  font-size: 1rem;
  text-align: right;	 
}
.value{
  font-size: 1rem;
  color: #1282A2;  
}
.state {
  font-size: 1rem;
  color: #1282A2;
}
button {
  border: none;
  color: #FEFCFB;
  padding: 15px 32px;
  text-align: center;
  font-size: 16px;
  width: 100px;
  border-radius: 4px;
  transition-duration: 0.4s;
}
.button-on {
  background-color: #034078;
}
.button-on:hover {
  background-color: #1282A2;
}
.button-off {
  background-color: #858585;
}
.button-off:hover {
  background-color: #252524;
} 
</style>
</html>
)rawliteral";

ConfigWiFiSetup::ConfigWiFiSetup() : serverAP(80)
{
  ssid[0] = '\0';
  pass[0] = '\0';
  ip[0] = '\0';
  sub[0] = '\0';
  gateway[0] = '\0';
  strncpy(ssidPath, "/ssid.txt", sizeof(ssidPath));
  strncpy(passPath, "/pass.txt", sizeof(passPath));
  strncpy(ipPath, "/ip.txt", sizeof(ipPath));
  strncpy(subPath, "/sub.txt", sizeof(subPath));
  strncpy(gatewayPath, "/gateway.txt", sizeof(gatewayPath));
  debugConfigWiFiSetup = false;
}

ConfigWiFiSetup::~ConfigWiFiSetup()
{
}

bool ConfigWiFiSetup::getDebugConfigWiFiSetup()
{
  return debugConfigWiFiSetup;
}

bool ConfigWiFiSetup::isThereWiFiSetting()
{
  if (debugConfigWiFiSetup)
  {
    Serial.println("ConfigWiFiSetup::isThereWiFiSetting()");
  }
  // Load values saved in SPIFFS
  readFileWiFiSettings(SPIFFS, ssidPath, ssid, sizeof(ssid) - 1);
  readFileWiFiSettings(SPIFFS, passPath, pass, sizeof(pass) - 1);
  readFileWiFiSettings(SPIFFS, ipPath, ip, sizeof(ip) - 1);
  readFileWiFiSettings(SPIFFS, subPath, sub, sizeof(sub) - 1);
  readFileWiFiSettings(SPIFFS, gatewayPath, gateway, sizeof(gateway) - 1);

  if (debugConfigWiFiSetup)
  {
    Serial.print("ConfigWiFiSetup::isThereWiFiSetting() -> Read from memory:\n  SSID: ");
    Serial.print(ssid);
    Serial.print("\n  Pass: ");
    Serial.print(pass);
    Serial.print("\n  IP: ");
    Serial.print(ip);
    Serial.print("\n  Sub: ");
    Serial.print(sub);
    Serial.print("\n  Gateway: ");
    Serial.println(gateway);
  }

  if (ssid[0] == '\0' || ip[0] == '\0')
  {
    if (debugConfigWiFiSetup)
    {
      Serial.println("ConfigWiFiSetup::isThereWiFiSetting() - return false");
    }
    return false;
  }
  if (debugConfigWiFiSetup)
  {
    Serial.println("ConfigWiFiSetup::isThereWiFiSetting() - return true");
  }
  return true;
}

char *ConfigWiFiSetup::getSSID()
{
  return ssid;
}
char *ConfigWiFiSetup::getPass()
{
  return pass;
}
char *ConfigWiFiSetup::getIP()
{
  return ip;
}
char *ConfigWiFiSetup::getSub()
{
  return sub;
}
char *ConfigWiFiSetup::getGateway()
{
  return gateway;
}

bool ConfigWiFiSetup::runAPWebServerSetup()
{
  // NULL sets an open Access Point
  const char apSsid[] = "ROR-WIFI-MANAGER";
  WiFi.softAP(apSsid, NULL);
  serverAP.onNotFound([](AsyncWebServerRequest *request)
                      { request->send(404, "text/plain", "Not found"); });

  // Send web page with input fields to client
  serverAP.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html); });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  serverAP.on("/", HTTP_POST, [this](AsyncWebServerRequest *request)
              {
    int params = request->params();
    for (int i = 0; i < params; i++)
    {
      AsyncWebParameter *p = request->getParam(i);
      if (p->isPost())
      {
        // HTTP POST ssid value
        if (p->name() == PARAM_SSID)
        {
          strncpy(ssid, p->value().c_str(), sizeof(ssid) - 1);
          ssid[sizeof(ssid) - 1] = '\0';
          writeFileWiFiSettings(SPIFFS, ssidPath, ssid);
        }
        // HTTP POST pass value
        if (p->name() == PARAM_PASS)
        {
          strncpy(pass, p->value().c_str(), sizeof(pass) - 1);
          pass[sizeof(pass) - 1] = '\0';
          writeFileWiFiSettings(SPIFFS, passPath, pass);
        }
        // HTTP POST ip value
        if (p->name() == PARAM_IP)
        {
          strncpy(ip, p->value().c_str(), sizeof(ip) - 1);
          ip[sizeof(ip) - 1] = '\0';
          writeFileWiFiSettings(SPIFFS, ipPath, ip);
        }
        // HTTP POST gateway value
        if (p->name() == PARAM_SUB)
        {
          strncpy(sub, p->value().c_str(), sizeof(sub) - 1);
          sub[sizeof(sub) - 1] = '\0';
          writeFileWiFiSettings(SPIFFS, subPath, sub);
        }
        if (p->name() == PARAM_GATEWAY)
        {
          strncpy(gateway, p->value().c_str(), sizeof(gateway) - 1);
          gateway[sizeof(gateway) - 1] = '\0';
          writeFileWiFiSettings(SPIFFS, gatewayPath, gateway);
        }
        Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }
    char response[200];
    snprintf(response, sizeof(response), "Done. ESP will restart\n SSID: %s Pass: %s\n IP: %s\n  Sub: %s\n  Gateway: %s", ssid, pass, ip, sub, gateway);
    request->send(200, "text/plain", response);
    delay(3000);
    ESP.restart(); });
  bool isPressed = false;
  serverAP.begin();
  unsigned long lastTime = 0;
  unsigned long timerDelay = 10000;
  while (true)
  {
    if ((millis() - lastTime) > timerDelay)
    {
      Serial.println("AP WiFi started and Network Credentials WebServer running...");
      Serial.print("SSID is \'ROR-WIFI-MANAGER\' @ IP address: " + WiFi.softAPIP().toString());
      Serial.println("");
      lastTime = millis();
    }
  }
  return true;
}

void ConfigWiFiSetup::notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void ConfigWiFiSetup::clearWiFiSettings()
{
  ssid[0] = '\0';
  pass[0] = '\0';
  ip[0] = '\0';
  sub[0] = '\0';
  gateway[0] = '\0';
  writeFileWiFiSettings(SPIFFS, ssidPath, ssid);
  writeFileWiFiSettings(SPIFFS, passPath, pass);
  writeFileWiFiSettings(SPIFFS, ipPath, ip);
  writeFileWiFiSettings(SPIFFS, subPath, sub);
  writeFileWiFiSettings(SPIFFS, gatewayPath, gateway);
}

// Read File from SPIFFS
void ConfigWiFiSetup::readFileWiFiSettings(fs::FS &fs, const char *path, char *destination, size_t maxSize)
{
  if (debugConfigWiFiSetup)
  {
    Serial.printf("ConfigWiFiSetup::readFileWiFiSettings() Reading file: %s\n", path);
  }

  File file = fs.open(path);
  if (!file || file.isDirectory())
  {
    if (debugConfigWiFiSetup)
    {
      Serial.println("ConfigWiFiSetup::readFileWiFiSettings() - failed to open file for reading");
    }
    return;
  }

  size_t bytesRead = file.readBytesUntil('\n', destination, maxSize - 1);
  destination[bytesRead] = '\0';
  if (debugConfigWiFiSetup)
  {
    Serial.printf("ConfigWiFiSetup::readFileWiFiSettings() -> Read %d bytes from file\n", bytesRead);
  }
}

// Write file to SPIFFS
void ConfigWiFiSetup::writeFileWiFiSettings(fs::FS &fs, const char *path, char *message)
{
  if (debugConfigWiFiSetup)
  {
    Serial.printf("Writing file: %s\r\n", path);
  }

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    if (debugConfigWiFiSetup)
    {
      Serial.println("- failed to open file for writing");
    }
    return;
  }
  size_t len = strlen(message);
  if (file.write(reinterpret_cast<const uint8_t *>(message), len) == len)
  {
    if (debugConfigWiFiSetup)
    {
      Serial.println("- file written");
    }
  }
  else
  {
    if (debugConfigWiFiSetup)
    {
      Serial.println("- write failed");
    }
  }
}
