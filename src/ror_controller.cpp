
#include "ror_controller.h"

ROR_Controller::ROR_Controller()
{
  rorDebug = false;

  scopeSafeParked_LED.begin(OUTPUT_SCOPE_PARKED_SAFE_LED, "ScopeIsParked");
  scopeUNSafeNotParked_LED.begin(OUTPUT_SCOPE_NOT_PARKED_UNSAFE_LED, "ScopeNotParked");
  opened_Sensor_LED.begin(OUTPUT_OPENED_LED, ROR_OPENED);
  closed_Sensor_LED.begin(OUTPUT_CLOSED_LED, ROR_CLOSED);
  roof_moving_LED.begin(OUTPUT_MOVING_LED, ROR_MOVING);
  roof_unknown_LED.begin(OUTPUT_UNKNOWN_LED, ROR_UNKNOWN);
  osc_button_LED.begin(OUTPUT_OSC_BUTTON_LED, ROR_OSCBUTTON);

  pinMode(INPUT_PIN_SCOPE_PARKED_SAFE_SENSOR, INPUT);
  pinMode(INPUT_PIN_OPENED_SENSOR, INPUT);
  pinMode(INPUT_PIN_CLOSED_SENSOR, INPUT);
  pinMode(INPUT_PIN_OSC_BUTTON, INPUT);
}

// Destructor
ROR_Controller::~ROR_Controller()
{
}

// The the RoR Structure for displaying to the website
RORStatus_Struct *ROR_Controller::getRORStatus()
{
  ROR_Controller::updateRORStatus();
  return &rorStatusStruct;
  // strlcpy(aRORStatus_Struct->rorCurrentPosition, rorStatusStruct.rorCurrentPosition, sizeof(aRORStatus_Struct->rorCurrentPosition));
  // strlcpy(aRORStatus_Struct->ScopeParkSafe, rorStatusStruct.ScopeParkSafe, sizeof(aRORStatus_Struct->ScopeParkSafe));
}

// Updates the RoR Structure with current status
void ROR_Controller::updateRORStatus()
{
  if (rorDebug)
  {
    Serial.println("ROR_Controller::updateRORStatus(): ");
  }

  if (opened_Sensor_LED.on)
  {
    strlcpy(rorStatusStruct.rorCurrentPosition, opened_Sensor_LED.displayName, sizeof(rorStatusStruct.rorCurrentPosition));
    rorMovingTimeCounter = 0;
    roof_moving_LED.update(false);
    roof_unknown_LED.update(false);
    if (true)
    {
      Serial.println("opened_Sensor_LED.on");
    }
  }
  if (closed_Sensor_LED.on)
  {
    strlcpy(rorStatusStruct.rorCurrentPosition, closed_Sensor_LED.displayName, sizeof(rorStatusStruct.rorCurrentPosition));
    rorMovingTimeCounter = 0;
    roof_moving_LED.update(false);
    roof_unknown_LED.update(false);
    if (true)
    {
      Serial.println("closed_Sensor_LED.on");
    }
  }
  // open and close sensor are open so roof is moving but not unknown yet
  if (!opened_Sensor_LED.on && !closed_Sensor_LED.on && !roof_unknown_LED.on)
  {
    if (rorMovingTimeCounter < rorMovingTimeReached)
    {
      ++rorMovingTimeCounter;
      roof_moving_LED.update(true);
    }
    else // roof is lost
    {
      roof_moving_LED.update(false);
      roof_unknown_LED.update(true);
    }
  }
  if (roof_moving_LED.on)
  {
    strlcpy(rorStatusStruct.rorCurrentPosition, roof_moving_LED.displayName, sizeof(rorStatusStruct.rorCurrentPosition));
    if (true)
    {
      Serial.println("roof_moving_LED.on");
    }
  }
  if (roof_unknown_LED.on)
  {
    strlcpy(rorStatusStruct.rorCurrentPosition, roof_unknown_LED.displayName, sizeof(rorStatusStruct.rorCurrentPosition));
    if (true)
    {
      Serial.println("roof_unknown_LED.on");
    }
  }
  if (scopeSafeParked_LED.on)
  {
    strlcpy(rorStatusStruct.ScopeParkSafe, scopeSafeParked_LED.displayName, sizeof(rorStatusStruct.ScopeParkSafe));
  }
  else
  {
    strlcpy(rorStatusStruct.ScopeParkSafe, scopeUNSafeNotParked_LED.displayName, sizeof(rorStatusStruct.ScopeParkSafe));
  }
}

void ROR_Controller::updatedInputSensorsButtons()
{
  if (rorDebug)
  {
    Serial.println("ROR_Controller::updatedInputSensorsButtons()");
  }

  // Opened Roof Sensor if the roof is currently opened
  opened_Sensor_INPUT.read();
  if (opened_Sensor_INPUT.held())
  {
    opened_Sensor_LED.update(true);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons():opened_Sensor_LED.update(true)");
    }
  }
  else
  {
    opened_Sensor_LED.update(false);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons():opened_Sensor_LED.update(false)");
    }
  }
  // Closed Roof Sensor if the roof is currently closed
  closed_Sensor_INPUT.read();
  if (closed_Sensor_INPUT.held())
  {
    closed_Sensor_LED.update(true);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons():closed_Sensor_LED.update(true)");
    }
  }
  else
  {
    closed_Sensor_LED.update(false);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons():closed_Sensor_LED.update(false)");
    }
  }
  // Scope parked Sensor
  scopePark_Sensor_INPUT.read();
  if (scopePark_Sensor_INPUT.held())
  {
    scopeSafeParked_LED.update(true);
    scopeUNSafeNotParked_LED.update(false);
    isScopeParkSafeSensorEngaged = true;
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons():scopeSafeParked_LED.update(true)");
    }
  }
  else
  {
    scopeSafeParked_LED.update(false);
    scopeUNSafeNotParked_LED.update(true);
    isScopeParkSafeSensorEngaged = false;
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons():scopeUNSafeNotParked_LED.update(true)");
    }
  }
  // OSC Button
  OSC_Btn_BUTTON_INPUT.read();
  if (OSC_Btn_BUTTON_INPUT.held())
  {
    osc_button_LED.update(true);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons():osc_button_LED.update(true)");
    }
  }
  else
  {
    osc_button_LED.update(false);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons():osc_button_LED.update(false)");
    }
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