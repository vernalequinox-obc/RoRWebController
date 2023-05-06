
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
String ROR_Controller::getRORPosistion()
{
  int statusINT = millis() % 1000 < 50;
  String statusStr = "Not Selected";
  switch(statusINT) {
    case 0:
      statusStr = "Closed";
      break;
    case 1:
      statusStr = "Opened";
      break;
    case 2:
      statusStr = "Moving";
      break;
    default:
      statusStr = "Unknown";

  }
  return statusStr;
}
