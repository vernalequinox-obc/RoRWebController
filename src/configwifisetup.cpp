#include "configwifisetup.h"

void notFound(AsyncWebServerRequest *request);

const char *PARAM_SSID = "ssid";
const char *PARAM_PASS = "pass";
const char *PARAM_IP = "ip";
const char *PARAM_SUB = "sub";
const char *PARAM_GATEWAY = "gateway";

AsyncWebServer serverAP(80);
// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ROR Wi-Fi Manager</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta name="apple-mobile-web-app-capable" content="yes">
  <link rel="icon" href="data:,">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
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
</html>
)rawliteral";

ConfigWiFiSetup::ConfigWiFiSetup()
{
  debugConfigWiFiSetup = true;
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

  // Send web page with input fields to client
  serverAP.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html); });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  serverAP.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_SSID)) {
      inputMessage = request->getParam(PARAM_SSID)->value();
      inputParam = PARAM_SSID;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_PASS)) {
      inputMessage = request->getParam(PARAM_PASS)->value();
      inputParam = PARAM_PASS;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(PARAM_IP)) {
      inputMessage = request->getParam(PARAM_IP)->value();
      inputParam = PARAM_IP;
    }
    else if (request->hasParam(PARAM_SUB)) {
      inputMessage = request->getParam(PARAM_SUB)->value();
      inputParam = PARAM_SUB;
    }
    else if (request->hasParam(PARAM_GATEWAY)) {
      inputMessage = request->getParam(PARAM_GATEWAY)->value();
      inputParam = PARAM_GATEWAY;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>"); });

  serverAP.onNotFound(notFound);
  serverAP.begin();
  {
    Serial.println("ConfigWiFiSetup::runAPWebServerSetup() end of and return");
  }
  while (true)
  {
    // Do not return let the esp32 reboot once inputs are finished.
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
String ConfigWiFiSetup::readFile(fs::FS &fs, const char *path)
{
  if (debugConfigWiFiSetup)
  {
    Serial.printf("ConfigWiFiSetup::readFile() Reading file: %s\r\n", path);
  }

  File file = fs.open(path);
  if (!file || file.isDirectory())
  {

    Serial.println("- failed to open file for reading. ConfigWiFiSetup::readFile()");
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
void ConfigWiFiSetup::writeFile(fs::FS &fs, const char *path, const char *message)
{
  if (debugConfigWiFiSetup)
  {
    Serial.printf("ConfigWiFiSetup::writeFile() Writing file: %s\r\n", path);
  }

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("- failed to open file for writing. ConfigWiFiSetup::writeFile");
    return;
  }
  if (file.print(message))
  {
    Serial.println("- file written. ConfigWiFiSetup::writeFile");
  }
  else
  {
    Serial.println("- write failed. ConfigWiFiSetup::writeFile");
  }
}
