
#include "ror_controller.h"

ROR_Controller::ROR_Controller()
{
  roofOpenSwitch.InputButton::setDeviceName("roofOpenSwitch");
  roofOpenSwitch.LedLight::setDeviceName("roofOpenSwitchLED");
  roofOpenSwitch.setButtonLedPin(ROOF_OPEN_SWITCH_INPUTPIN, ROOF_OPEN_LED);
  roofOpenSwitch.setDebounceTime(125);
  // roofOpenSwitch.InputButton::setDebug(true);

  roofCloseSwitch.InputButton::setDeviceName("roofCloseSwitch");
  roofCloseSwitch.LedLight::setDeviceName("roofCloseSwitchLED");
  roofCloseSwitch.setButtonLedPin(ROOF_CLOSE_SWITCH_INPUTPIN, ROOF_CLOSED_LED);
  roofCloseSwitch.setDebounceTime(125);
  // roofCloseSwitch.InputButton::setDebug(true);

  roofMovingLED.setLedPin(ROOF_MOVING_LED);
  roofMovingLED.setDeviceName("roofMovingLED");
  // roofMovingLED.setDebug(true);

  scopeUNSafeNotParkedLED.setLedPin(SCOPE_MOUNT_PARK_NOT_SAFE_LED);
  scopeUNSafeNotParkedLED.setDeviceName("scopeUNSafeNotParkedLED");
  // scopeUNSafeNotParkedLED.setDebug(true);

  scopeMountParkSwitch.InputButton::setDeviceName("scopeMountParkSwitch");
  scopeMountParkSwitch.LedLight::setDeviceName("scopeMountParkSwitchLED");
  scopeMountParkSwitch.setButtonLedPin(SCOPE_MOUNT_SAFE_SWITCH_INPUTPIN, SCOPE_MOUNT_PARK_SAFE_LED);
  scopeMountParkSwitch.setDebounceTime(125);
  // scopeMountParkSwitch.InputButton::setDebug(true);

  oscPushButton.InputButton::setDeviceName("oscPushButton");
  oscPushButton.LedLight::setDeviceName("oscPushButtonLED");
  oscPushButton.setButtonLedPin(OSC_PUSHBUTTON_INPUTPIN, OSC_BUTTON_LED);
  oscPushButton.setDebounceTime(1000);
  oscPushButton.setPusleTriggerDuration(250);
  oscPushButton.setDisableDurationPostPulse(5000);
  // oscPushButton.setDeviceEnabledButton(false);
  // oscPushButton.InputButton::setDebug(true);

  roofOpenSwitch.begin();
  roofCloseSwitch.begin();
  oscPushButton.begin();
  scopeMountParkSwitch.begin();
  roofMovingLED.begin();
  scopeUNSafeNotParkedLED.begin();

  rorDebug = false;
}

// Destructor
ROR_Controller::~ROR_Controller()
{
}

// The the RoR Structure for displaying to the website
ROR_Status *ROR_Controller::getRORStatus()
{
  if (rorDebug)
  {
    Serial.println("ROR_Controller::getRORStatus()");
  }
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
    roofMovingLED.updateLed(false);
    rorMovingTimeCounter = 0;
    itemMoving.isTrue = false;
    itemUnknown.isTrue = false;
    if (rorDebug)
    {
      Serial.println("itemOpen.isTrue = true");
    }
  }
  if (itemClosed.isTrue)
  {
    strlcpy(rorStatusStruct.rorCurrentPosition, itemClosed.webName, sizeof(rorStatusStruct.rorCurrentPosition));
    roofMovingLED.updateLed(false);
    rorMovingTimeCounter = 0;
    itemMoving.isTrue = false;
    itemUnknown.isTrue = false;
    if (rorDebug)
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
    roofMovingLED.updateLed(true);
    if (rorDebug)
    {
      Serial.println("itemMoving.isTrue = true");
    }
  }
  if (itemUnknown.isTrue)
  {
    strlcpy(rorStatusStruct.rorCurrentPosition, itemUnknown.webName, sizeof(rorStatusStruct.rorCurrentPosition));
    roofMovingLED.updateLed(false);
    if (rorDebug)
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
  roofOpenSwitch.updateButtonPin();
  itemOpen.isTrue = roofOpenSwitch.isPressed();

  if (itemOpen.isTrue)
  {
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons()::roofOpenSwitch.getState() = true");
    }
  }
  else
  {
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons()::roofOpenSwitch.getState() = false");
    }
  }

  // Closed Roof Sensor if the roof is currently closed
  roofCloseSwitch.updateButtonPin();
  itemClosed.isTrue = roofCloseSwitch.isPressed();
  if (itemClosed.isTrue)
  {
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons()::roofCloseSwitch.pressed() = true");
    }
  }
  else
  {
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons()::roofCloseSwitch.pressed() = false");
    }
  }
  // Scope parked Sensor
  scopeMountParkSwitch.updateButtonPin();
  itemSafe.isTrue = scopeMountParkSwitch.isPressed();
  if (itemSafe.isTrue)
  {
    itemUnSafe.isTrue = false;
    scopeUNSafeNotParkedLED.updateLed(itemUnSafe.isTrue);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons()::scopeMountParkSwitch.pressed() = true");
    }
  }
  else
  {
    itemUnSafe.isTrue = true;
    scopeUNSafeNotParkedLED.updateLed(itemUnSafe.isTrue);
    if (rorDebug)
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons()::scopeMountParkSwitch.pressed() = false;");
    }
  }
  // OSC Button
  if (rorDebug || oscPushButton.getDebugButton())
  {
    Serial.println("ROR_Controller::updatedInputSensorsButtons()::oscPushButton.update()");
  }
  oscPushButton.updateButtonPin();
  if (oscPushButton.isPulseTriggered())
  {
    isEngagedRelayPulse = true;
    if (rorDebug || oscPushButton.getDebugButton())
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons()::oscPushButton.isPulseTriggered() = true");
    }
  }
  else
  {
    if (rorDebug || oscPushButton.getDebugButton())
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons()::oscPushButton.isPulseTriggered() = false");
    }
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