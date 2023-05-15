
#include "ror_controller.h"

ROR_Controller::ROR_Controller()
{
  rorDebug = false;

  scopeSafeParked_LED.begin(OUTPUT_SCOPE_PARKED_SAFE_LED, "ScopeIsParked");
  scopeUNSafeNotParked_LED.begin(OUTPUT_SCOPE_NOT_PARKED_UNSAFE_LED, "ScopeNotParked");
  opened_Sensor_LED.begin(OUTPUT_OPENED_LED, "Opened");
  closed_Sensor_LED.begin(OUTPUT_CLOSED_LED, "Closed");
  roof_moving_LED.begin(OUTPUT_MOVING_LED, "Moving");
  roof_unknown_LED.begin(OUTPUT_UNKNOWN_LED, "Unknown");
  osc_button_LED.begin(OUTPUT_OSC_BUTTON_LED);

  opened_Sensor_INPUT.begin(INPUT_PIN_OPENED_SENSOR, "OpendSwitch");
  closed_Sensor_INPUT.begin(INPUT_PIN_CLOSED_SENSOR, "ClosedSwitch");

  // scopePark_Sensor_INPUT.begin(INPUT_PIN_SCOPE_PARKED_SAFE_SENSOR, "ScopeSwitch");

  pinMode(INPUT_PIN_SCOPE_PARKED_SAFE_SENSOR, INPUT);

  // Test of Buttons
  // pinMode(buttonTest.pin, INPUT);

  OSC_Btn_BUTTON.begin(INPUT_PIN_OSC_BUTTON, "OSCButton");
}

// Destructor
ROR_Controller::~ROR_Controller()
{
}

// Replaces placeholder with LED state value
String ROR_Controller::getRORPosistion()
{
  if (rorDebug)
  {
    Serial.println("ROR_Controller::getRORPosistion(): ");

    Serial.print("ROR_Controller::getIsScopeParkSafe(): opened_Sensor_LED.displayName: ");
    Serial.println(opened_Sensor_LED.displayName);
    Serial.print("ROR_Controller::getIsScopeParkSafe(): closed_Sensor_LED.displayName: ");
    Serial.println(closed_Sensor_LED.displayName);
    Serial.print("ROR_Controller::getIsScopeParkSafe(): roof_moving_LED.displayName: ");
    Serial.println(roof_moving_LED.displayName);
    Serial.print("ROR_Controller::getIsScopeParkSafe(): roof_unknown_LED.displayName: ");
    Serial.println(roof_unknown_LED.displayName);
  }

  return opened_Sensor_LED.displayName;

  if (opened_Sensor_LED.on)
  {
    return opened_Sensor_LED.displayName;
  }
  if (closed_Sensor_LED.on)
  {
    return closed_Sensor_LED.displayName;
  }
  if (roof_moving_LED.on)
  {
    return roof_moving_LED.displayName;
  }
  return roof_unknown_LED.displayName;
}

String ROR_Controller::getIsScopeParkSafe()
{

  if (rorDebug)
  {
    Serial.println("ROR_Controller::getIsScopeParkSafe(): ");
  }
  if (scopeSafeParked_LED.on)
  {
    if (true)
    {
      Serial.print("ROR_Controller::getIsScopeParkSafe(): scopeSafeParked_LED.displayName: ");
      Serial.println(scopeSafeParked_LED.displayName);
    }
    return scopeSafeParked_LED.displayName;
  }
  if (true)
  {
    Serial.print("ROR_Controller::getIsScopeParkSafe(): scopeUNSafeNotParked_LED.displayName: ");
    Serial.println(scopeUNSafeNotParked_LED.displayName);
  }
  return scopeUNSafeNotParked_LED.displayName;
}

void ROR_Controller::updatedInputSensorsButtons()
{

  if (rorDebug)
  {
    Serial.println("ROR_Controller::checkInputSensorsButtons(): ");
  }

  // Opened Roof Sensor if the roof is currently opened
  if (opened_Sensor_INPUT.debounce())
  {
    opened_Sensor_LED.update(true);
    closed_Sensor_LED.update(false);
    roof_moving_LED.update(false);
    roof_unknown_LED.update(false);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::checkInputSensorsButtons(): opened_Sensor_LED.update(true)");
    }
  }
  // Closed Roof Sensor if the roof is currently closed
  if (closed_Sensor_INPUT.debounce())
  {
    closed_Sensor_LED.update(true);
    opened_Sensor_LED.update(false);
    roof_moving_LED.update(false);
    roof_unknown_LED.update(false);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::checkInputSensorsButtons(): closed_Sensor_LED.update(true);");
    }
  }
  // Scope Park Sensor if the scope mount is parked this switch is closed
  // if (scopePark_Sensor_INPUT.debounce())
  // scopePark_Sensor_INPUT.read();
  // scopePark_Sensor_INPUT.held();
  // scopePark_Sensor_INPUT.pressed();

  scopePark_Sensor_INPUT.read();
  if (scopePark_Sensor_INPUT.held())
  {
    scopeSafeParked_LED.update(true);
    scopeUNSafeNotParked_LED.update(false);
    isScopeParkSafeSensorEngaged = true;
    if (rorDebug)
    {
      Serial.println("ROR_Controller::checkInputSensorsButtons():scopeSafeParked_LED.update(true);");
    }
  }
  else
  {
    scopeSafeParked_LED.update(false);
    scopeUNSafeNotParked_LED.update(true);
    isScopeParkSafeSensorEngaged = false;
    if (rorDebug)
    {
      Serial.println("ROR_Controller::checkInputSensorsButtons():scopeUNSafeNotParked_LED.update(true);");
    }
  }

  if (OSC_Btn_BUTTON.debounce())
  {
    osc_button_LED.update(true);
  }
  else
  {
    osc_button_LED.update(false);
  }
}

// This method is responsible for sending the current status of various sensors and
// states to the serial port for display or further processing. It constructs a
// string containing the current status and sends it to the serial port.

void ROR_Controller::outputStatusToSerial()
{
  if (isOpenedSensorEngaged)
  {
    Serial.print("opened,");
  }
  else if (isClosedSensorEngaged)
  {
    Serial.print("closed,");
  }
  else
  {
    Serial.print("unknown,");
  }

  if (isScopeParkSafeSensorEngaged)
  {
    Serial.print("safe,");
  }
  else
  {
    Serial.print("unsafe,");
  }

  if (isOpenedSensorEngaged && !isClosedSensorEngaged && !isRoofLost)
  {
    Serial.print("not_moving_o#");
  }
  else if (isClosedSensorEngaged && !isOpenedSensorEngaged && !isRoofLost)
  {
    Serial.print("not_moving_c#");
  }
  else if (!isClosedSensorEngaged && !isOpenedSensorEngaged && !isRoofLost)
  {
    Serial.print("moving#");
  }

  if (!isOpenedSensorEngaged && !isClosedSensorEngaged && isRoofLost)
  {
    Serial.print("unknown#");
  }

  Serial.println();
}