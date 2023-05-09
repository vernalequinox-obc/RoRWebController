#include "configwifisetup.h"

void notFound(AsyncWebServerRequest *request);
void saveWiFiSettings();
void writeFile(fs::FS &fs, const char *path, const char *message);
String readFile(fs::FS &fs, const char *path);
// File paths to save input values permanently
const char *ssidPath = "/ssid.txt";
const char *passPath = "/pass.txt";
const char *ipPath = "/ip.txt";
const char *subPath = "/sub.txt";
const char *gatewayPath = "/gateway.txt";

const char *PARAM_SSID = "ssid";
const char *PARAM_PASS = "pass";
const char *PARAM_IP = "ip";
const char *PARAM_SUB = "sub";
const char *PARAM_GATEWAY = "gateway";
String ssid;
String pass;
String ip;
String sub;
String gateway;
bool debugConfigWiFiSetup = true;
AsyncWebServer serverAP(80);

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

ConfigWiFiSetup::ConfigWiFiSetup()
{
  ssid = "";
  pass = "";
  ip = "";
  sub = "";
  gateway = "";
}

ConfigWiFiSetup::~ConfigWiFiSetup()
{
}

boolean ConfigWiFiSetup::isThereWiFiSetting()
{
  if (debugConfigWiFiSetup)
  {
    Serial.println("ConfigWiFiSetup::isThereWiFiSetting()");
  }
  // Load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);
  ip = readFile(SPIFFS, ipPath);
  gateway = readFile(SPIFFS, gatewayPath);
  Serial.println("Read from memory: \nSSID: " + ssid + "\n  Pass: " + pass + "\n IP: " + ip + "\n  Sub: " + sub + "\n  Gateway: " + gateway);
  if (ssid == "" || ip == "")
  {
    if (debugConfigWiFiSetup)
    {
      Serial.println("ConfigWiFiSetup::isThereWiFiSetting() - Undefined SSID or IP address.");
    }
    return false;
  }
  if (debugConfigWiFiSetup)
  {
    Serial.println(ssid);
    Serial.println(pass);
    Serial.println(ip);
    Serial.println(gateway);
  }

  return true;
}

String ConfigWiFiSetup::getSSID()
{
  return ssid;
}
String ConfigWiFiSetup::getPass()
{
  return pass;
}
String ConfigWiFiSetup::getIP()
{
  return ip;
}
String ConfigWiFiSetup::getSub()
{
  return sub;
}
String ConfigWiFiSetup::getGateway()
{
  return gateway;
}

boolean ConfigWiFiSetup::runAPWebServerSetup()
{
  // Connect to Wi-Fi network with SSID and password
  if (debugConfigWiFiSetup)
  {
    Serial.println("ConfigWiFiSetup::runAPWebServerSetup() Setting AP (Access Point)");
  }
  // NULL sets an open Access Point
  WiFi.softAP("ROR-WIFI-MANAGER", NULL);

  if (debugConfigWiFiSetup)
  {
    Serial.print("ConfigWiFiSetup::runAPWebServerSetup() - AP IP address: ");
    Serial.println(WiFi.softAPIP());
  }

  serverAP.onNotFound(notFound);

  // Send web page with input fields to client
  serverAP.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html); });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  serverAP.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
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
          ssid = p->value().c_str();
          writeFile(SPIFFS, ssidPath, ssid.c_str());
        }
        // HTTP POST pass value
        if (p->name() == PARAM_PASS)
        {
          pass = p->value().c_str();
          // Write file to save value
          writeFile(SPIFFS, passPath, pass.c_str());
        }
        // HTTP POST ip value
        if (p->name() == PARAM_IP)
        {
          ip = p->value().c_str();
          // Write file to save value
          writeFile(SPIFFS, ipPath, ip.c_str());
        }
        // HTTP POST gateway value
        if (p->name() == PARAM_SUB)
        {
          sub = p->value().c_str();
          // Write file to save value
          writeFile(SPIFFS, subPath, sub.c_str());;
        }
        if (p->name() == PARAM_GATEWAY)
        {
          gateway = p->value().c_str();
          // Write file to save value
          writeFile(SPIFFS, gatewayPath, gateway.c_str());
        }
        Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }
    request->send(200, "text/plain", "Done. ESP will restart\n SSID: " + ssid + " Pass: " + pass  + "\n IP: " + ip + "\n  Sub: " + sub + "\n  Gateway: " + gateway);
    delay(3000);
    ESP.restart(); });
  serverAP.begin();
  while (true)
  {
    // Do not return let the esp32 reboot once inputs is finished.
  }
  return true;
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void ConfigWiFiSetup::clearWiFiSettings()
{
  ssid = "";
  pass = "";
  ip = "";
  sub = "";
  gateway = "";
  writeFile(SPIFFS, ssidPath, ssid.c_str());
  writeFile(SPIFFS, passPath, pass.c_str());
  writeFile(SPIFFS, ipPath, ip.c_str());
  writeFile(SPIFFS, subPath, sub.c_str());
  writeFile(SPIFFS, gatewayPath, gateway.c_str());
}

// Read File from SPIFFS
String readFile(fs::FS &fs, const char *path)
{
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory())
  {
    Serial.println("- failed to open file for reading");
    return String();
  }

  String fileContent;
  while (file.available())
  {
    fileContent = file.readStringUntil('\n');
    break;
  }
  return fileContent;
}

// Write file to SPIFFS
void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("- file written");
  }
  else
  {
    Serial.println("- write failed");
  }
}
