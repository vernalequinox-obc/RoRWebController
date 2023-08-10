
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

  scopeUNSafeNotParkedLED.setDevicePin(SCOPE_MOUNT_PARK_NOT_SAFE_LED);
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
  oscPushButton.setDebounceTime(HOLD_BUTTON_DOWN_THIS_LONG_TO_TRIGGER);
  oscPushButton.setPusleTriggerDuration(TRIGGER_PULSE_DURATION);
  oscPushButton.setDisableDurationPostPulse(DISABLE_TRIGGER_PULSE_BUTTON_DURATION);
  // oscPushButton.setDeviceEnabledButton(false);
  // oscPushButton.InputButton::setDebug(true);

  roofOpenSwitch.begin();
  roofCloseSwitch.begin();
  oscPushButton.begin();
  scopeMountParkSwitch.begin();
  scopeUNSafeNotParkedLED.begin();

  isEngagedRelayPulse = false;
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

  if (rorItemOpen.isTrue)
  {
    strlcpy(rorStatusStruct.rorCurrentPosition, rorItemOpen.webName, sizeof(rorStatusStruct.rorCurrentPosition));
    rorMovingTimeCounter = 0;
    rorItemMoving.isTrue = false;
    rorItemUnknown.isTrue = false;
    if (rorDebug)
    {
      Serial.println("rorItemOpen.isTrue = true");
    }
  }
  if (rorItemClosed.isTrue)
  {
    strlcpy(rorStatusStruct.rorCurrentPosition, rorItemClosed.webName, sizeof(rorStatusStruct.rorCurrentPosition));
    rorMovingTimeCounter = 0;
    rorItemMoving.isTrue = false;
    rorItemUnknown.isTrue = false;
    if (rorDebug)
    {
      Serial.println("rorItemClosed.isTrue = true");
    }
  }
  // open and close sensor are open so roof is moving but not unknown yet
  if (!rorItemOpen.isTrue && !rorItemClosed.isTrue && !rorItemUnknown.isTrue)
  {
    if (rorMovingTimeCounter < rorMovingTimeReached)
    {
      ++rorMovingTimeCounter;
      rorItemMoving.isTrue = true;
    }
    else // roof is lost
    {
      rorItemMoving.isTrue = false;
      rorItemUnknown.isTrue = true;
    }
  }
  if (rorItemMoving.isTrue)
  {
    strlcpy(rorStatusStruct.rorCurrentPosition, rorItemMoving.webName, sizeof(rorStatusStruct.rorCurrentPosition));
    if (rorDebug)
    {
      Serial.println("rorItemMoving.isTrue = true");
    }
  }
  if (rorItemUnknown.isTrue)
  {
    strlcpy(rorStatusStruct.rorCurrentPosition, rorItemUnknown.webName, sizeof(rorStatusStruct.rorCurrentPosition));
    if (rorDebug)
    {
      Serial.println("rorItemUnknown.isTrue = true");
    }
  }
  if (rorItemSafe.isTrue)
  {
    strlcpy(rorStatusStruct.IsScopeParkSafe, rorItemSafe.webName, sizeof(rorStatusStruct.IsScopeParkSafe));
  }
  else
  {
    strlcpy(rorStatusStruct.IsScopeParkSafe, rorItemUnSafe.webName, sizeof(rorStatusStruct.IsScopeParkSafe));
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
  rorItemOpen.isTrue = roofOpenSwitch.isPressed();

  if (rorItemOpen.isTrue)
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
  rorItemClosed.isTrue = roofCloseSwitch.isPressed();
  if (rorItemClosed.isTrue)
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
  rorItemSafe.isTrue = scopeMountParkSwitch.isPressed();
  if (rorItemSafe.isTrue)
  {
    rorItemUnSafe.isTrue = false;
  }
  else
  {
    rorItemUnSafe.isTrue = true;
  }
  scopeUNSafeNotParkedLED.updateLed(rorItemUnSafe.isTrue);
  relayControl.setScopeMountParkSafeRelay(rorItemSafe.isTrue);
  if (rorDebug || relayControl.scopeMountParkSafeRelay.getDebug())
  {
    Serial.print("ROR_Controller::updatedInputSensorsButtons()::scopeMountParkSwitch.pressed() rorItemUnSafe.isTrue: ");
    Serial.println(rorItemSafe.isTrue);
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

  if (isEngagedRelayPulse)
  {
    if (rorDebug || oscPushButton.getDebugButton() || relayControl.scopeMountParkSafeRelay.getDebug())
    {
      Serial.println("ROR_Controller::updatedInputSensorsButtons()::if (isEngagedRelayPulse) = true");
    }
    relayControl.setScopeMountParkSafeRelay(rorItemSafe.isTrue);
    relayControl.updateOSCRelayPulseTime();
    if (!relayControl.getOscTriggerRelayCurrentPinState())
    {
      isEngagedRelayPulse = false;
    }
  }
}

void ROR_Controller::setIsEngagedRelayPulseTrue(void)
{
  isEngagedRelayPulse = true;
}

// This method is responsible for sending the current status of various sensors and
// states to the serial port for display or further processing. It constructs a
// string containing the current status and sends it to the serial port.

void ROR_Controller::outputStatusToSerial()
{
  if (rorItemOpen.isTrue)
  {
    Serial.print(rorItemOpen.serialName);
  }
  else if (rorItemClosed.isTrue)
  {
    Serial.print(rorItemClosed.serialName);
  }
  else if (rorItemMoving.isTrue)
  {
    Serial.print(rorItemMoving.serialName);
  }
  else if (rorItemUnknown.isTrue)
  {
    Serial.print(rorItemUnknown.serialName);
  }

  if (rorItemSafe.isTrue)
  {
    Serial.print(rorItemSafe.serialName);
  }
  else if (rorItemUnSafe.isTrue)
  {
    Serial.print(rorItemUnSafe.serialName);
  }

  if (rorItemOpen.isTrue && !rorItemClosed.isTrue && !rorItemRoofLost.isTrue)
  {
    Serial.print("not_moving_o#");
  }
  else if (rorItemClosed.isTrue && !rorItemOpen.isTrue && !rorItemRoofLost.isTrue)
  {
    Serial.print("not_moving_c#");
  }
  else if (!rorItemClosed.isTrue && !rorItemOpen.isTrue && !rorItemRoofLost.isTrue)
  {
    Serial.print("moving#");
  }

  if (!rorItemOpen.isTrue && !rorItemClosed.isTrue && rorItemRoofLost.isTrue)
  {
    Serial.print("unknown#");
  }

  Serial.println();
}