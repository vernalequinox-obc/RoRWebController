
#include "ror_controller.h"

ROR_Controller::ROR_Controller()
{
  debug = true;
  ledState = "";
  pinMode(ledPin, OUTPUT);
}

// Destructor
ROR_Controller::~ROR_Controller()
{
}

// Replaces placeholder with LED state value
String ROR_Controller::processor(const String &var)
{
  Serial.println(var);
  if (var == "STATE")
  {
    if (digitalRead(ROR_Controller::ledPin))
    {
      Serial.print("Open");
      return("Open");
    }
    else
    {
      Serial.print("Closed");
      return("Closed");
    }
  }
  return String();
}
