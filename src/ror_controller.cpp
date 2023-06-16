
#include "ror_controller.h"

ROR_Controller::ROR_Controller()
{
  rorDebug = false;

  scopeSafeParked_LED.begin(OUTPUT_SCOPE_PARKED_SAFE_LED);
  scopeUNSafeNotParked_LED.begin(OUTPUT_SCOPE_NOT_PARKED_UNSAFE_LED);
  opened_Sensor_LED.begin(OUTPUT_OPENED_LED);
  closed_Sensor_LED.begin(OUTPUT_CLOSED_LED);
  osc_button_LED.begin(OUTPUT_OSC_BUTTON_LED);

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
ROR_Status *ROR_Controller::getRORStatus()
{
  ROR_Controller::updateRORStatus();
  return &rorStatusStruct;
  // strlcpy(aROR_Status->rorCurrentPosition, rorStatusStruct.rorCurrentPosition, sizeof(aROR_Status->rorCurrentPosition));
  // strlcpy(aROR_Status->IsScopeParkSafe, rorStatusStruct.IsScopeParkSafe, sizeof(aROR_Status->IsScopeParkSafe));
}

// Updates the RoR Structure with current status
void ROR_Controller::updateRORStatus()
{
  if (rorDebug)
  {
    Serial.println("ROR_Controller::updateRORStatus(): ");
  }

  if (itemOpen.isTrue)
  {
    strlcpy(rorStatusStruct.rorCurrentPosition, itemOpen.webName, sizeof(rorStatusStruct.rorCurrentPosition));
    rorMovingTimeCounter = 0;
    itemMoving.isTrue = false;
    itemUnknown.isTrue = false;
    if (true)
    {
      Serial.println("itemOpen.isTrue = true");
    }
  }
  if (itemClosed.isTrue)
  {
    strlcpy(rorStatusStruct.rorCurrentPosition, itemClosed.webName, sizeof(rorStatusStruct.rorCurrentPosition));
    rorMovingTimeCounter = 0;
    itemMoving.isTrue = false;
    itemUnknown.isTrue = false;
    if (true)
    {
      Serial.println("itemClosed.isTrue = true");
    }
  }
  // open and close sensor are open so roof is moving but not unknown yet
  if (!itemOpen.isTrue && !itemClosed.isTrue && !itemUnknown.isTrue)
  {
    if (rorMovingTimeCounter < rorMovingTimeReached)
    {
      ++rorMovingTimeCounter;
      itemMoving.isTrue = true;
    }
    else // roof is lost
    {
      itemMoving.isTrue = false;
      itemUnknown.isTrue = true;
    }
  }
  if (itemMoving.isTrue)
  {
    strlcpy(rorStatusStruct.rorCurrentPosition, itemMoving.webName, sizeof(rorStatusStruct.rorCurrentPosition));
    if (true)
    {
      Serial.println("itemMoving.isTrue = true");
    }
  }
  if (itemUnknown.isTrue)
  {
    strlcpy(rorStatusStruct.rorCurrentPosition, itemUnknown.webName, sizeof(rorStatusStruct.rorCurrentPosition));
    if (true)
    {
      Serial.println("itemUnknown.isTrue = true");
    }
  }
  if (itemSafe.isTrue)
  {
    strlcpy(rorStatusStruct.IsScopeParkSafe, itemSafe.webName, sizeof(rorStatusStruct.IsScopeParkSafe));
  }
  else
  {
    strlcpy(rorStatusStruct.IsScopeParkSafe, itemUnSafe.webName, sizeof(rorStatusStruct.IsScopeParkSafe));
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
    itemOpen.isTrue = true;
    itemOpen.ptrLedLight->update(true);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons():itemOpen.isTrue = true");
    }
  }
  else
  {
    itemOpen.isTrue = false;
    itemOpen.ptrLedLight->update(false);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons():itemOpen.isTrue = false");
    }
  }
  // Closed Roof Sensor if the roof is currently closed
  closed_Sensor_INPUT.read();
  if (closed_Sensor_INPUT.held())
  {
    itemClosed.isTrue = true;
    itemClosed.ptrLedLight->update(true);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons():itemClosed.isTrue = true");
    }
  }
  else
  {
    itemClosed.isTrue = false;
    itemClosed.ptrLedLight->update(false);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons():itemClosed.isTrue = false");
    }
  }
  // Scope parked Sensor
  scopePark_Sensor_INPUT.read();
  if (scopePark_Sensor_INPUT.held())
  {
    itemSafe.isTrue = true;
    itemSafe.ptrLedLight->update(true);
    itemUnSafe.isTrue = false;
    itemUnSafe.ptrLedLight->update(false);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons():itemSafe.isTrue = true");
    }
  }
  else
  {
    itemSafe.isTrue = false;
    itemSafe.ptrLedLight->update(false);
    itemUnSafe.isTrue = true;
    itemUnSafe.ptrLedLight->update(true);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons():itemUnSafe.isTrue = true;");
    }
  }
  // OSC Button
  OSC_Btn_BUTTON_INPUT.read();
  if (OSC_Btn_BUTTON_INPUT.held())
  {
    osc_button_LED.update(true);
    isEngagedRelayPulse = true;
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
  if (osc_button_LED.on == true)
  {
    temporaryDisableOSCButton();
  }
  if (isEngagedRelayPulse)
  {
    engageRelayPulse();
  }
}

void ROR_Controller::engageRelayPulse()
{
  if (isEngagedRelayPulse == false)
  {
    return;
  }
  long engagedRelayPulseTimer = millis() - engagedRelayPulseTimer;

  if (engagedRelayPulseTimer > ROR_ENGAGE_RELAY_PULSEWIDTH)
  {
    if (rorDebug)
    {
      Serial.println("ROR_Controller::relayEngagedPulse() isEnagagedRelayPulseHapping = true");
    }
    isEnagagedRelayPulseHapping = true;
  }
}

void ROR_Controller::temporaryDisableOSCButton()
{
  if (osc_button_LED.on == true && isTemporaryDisableOSCPulse == false)
  {
    if (rorDebug)
    {
      Serial.println("ROR_Controller::relayEngagedPulse() isEnagagedRelayPulseHapping = true");
    }
    isTemporaryDisableOSCHapping = true;
    OSC_Btn_BUTTON_INPUT.isEnabled = false;
  }
  else if (isTemporaryDisableOSCHapping && (millis() - temporaryDisableOSCTimer) > ROR_TEMPORARY_DISABLE_OSC_BUTTON)
  {
    osc_button_LED.update(false);
    isTemporaryDisableOSCHapping = false;
    OSC_Btn_BUTTON_INPUT.isEnabled = true;
  }
}

// This method is responsible for sending the current status of various sensors and
// states to the serial port for display or further processing. It constructs a
// string containing the current status and sends it to the serial port.

void ROR_Controller::outputStatusToSerial()
{
  if (itemOpen.isTrue)
  {
    Serial.print(itemOpen.serialName);
  }
  else if (itemClosed.isTrue)
  {
    Serial.print(itemClosed.serialName);
  }
  else if (itemMoving.isTrue)
  {
    Serial.print(itemMoving.serialName);
  }
  else if (itemUnknown.isTrue)
  {
    Serial.print(itemUnknown.serialName);
  }

  if (itemSafe.isTrue)
  {
    Serial.print(itemSafe.serialName);
  }
  else if (itemUnSafe.isTrue)
  {
    Serial.print(itemUnSafe.serialName);
  }

  if (itemOpen.isTrue && !itemClosed.isTrue && !itemRoofLost.isTrue)
  {
    Serial.print("not_moving_o#");
  }
  else if (itemClosed.isTrue && !itemOpen.isTrue && !itemRoofLost.isTrue)
  {
    Serial.print("not_moving_c#");
  }
  else if (!itemClosed.isTrue && !itemOpen.isTrue && !itemRoofLost.isTrue)
  {
    Serial.print("moving#");
  }

  if (!itemOpen.isTrue && !itemClosed.isTrue && itemRoofLost.isTrue)
  {
    Serial.print("unknown#");
  }

  Serial.println();
}