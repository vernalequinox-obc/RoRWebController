
#include "ror_controller.h"

ROR_Controller::ROR_Controller()
{
  roofOpenSwitch.InputButton::setDeviceName("roofOpenSwitch");
  roofOpenSwitch.LedLight::setDeviceName("roofOpenSwitchLED");
  roofOpenSwitch.setButtonLedPin(ROOF_OPEN_SWITCH_INPUTPIN, ROOF_OPEN_LED);
  roofOpenSwitch.setDebounceTime(BUTTON_DEBOUNCE_TIME);
  // roofOpenSwitch.InputButton::setDebug(true);

  roofCloseSwitch.InputButton::setDeviceName("roofCloseSwitch");
  roofCloseSwitch.LedLight::setDeviceName("roofCloseSwitchLED");
  roofCloseSwitch.setButtonLedPin(ROOF_CLOSE_SWITCH_INPUTPIN, ROOF_CLOSED_LED);
  roofCloseSwitch.setDebounceTime(BUTTON_DEBOUNCE_TIME);
  // roofCloseSwitch.InputButton::setDebug(true);

  scopeUNSafeNotParkedLED.setDevicePin(SCOPE_MOUNT_PARK_NOT_SAFE_LED);
  scopeUNSafeNotParkedLED.setDeviceName("scopeUNSafeNotParkedLED");
  // scopeUNSafeNotParkedLED.setDebug(true);

  scopeMountParkSwitch.InputButton::setDeviceName("scopeMountParkSwitch");
  scopeMountParkSwitch.LedLight::setDeviceName("scopeMountParkSwitchLED");
  scopeMountParkSwitch.setButtonLedPin(SCOPE_MOUNT_SAFE_SWITCH_INPUTPIN, SCOPE_MOUNT_PARK_SAFE_LED);
  scopeMountParkSwitch.setDebounceTime(BUTTON_DEBOUNCE_TIME);
  // scopeMountParkSwitch.InputButton::setDebug(true);

  oscPushButton.InputButton::setDeviceName("oscPushButton");
  oscPushButton.LedLight::setDeviceName("oscPushButtonLED");
  oscPushButton.setButtonLedPin(OSC_PUSHBUTTON_INPUTPIN, OSC_BUTTON_LED);
  oscPushButton.setDebounceTime(HOLD_BUTTON_DOWN_THRESHOLD_ALEKO);
  oscPushButton.setPusleTriggerDuration(TRIGGER_PULSE_DURATION_ALEKO);
  oscPushButton.setDisableDurationPostPulse(DISABLE_TRIGGER_PULSE_BUTTON_DURATION);
  // oscPushButton.setDeviceEnabledButton(false);
  // oscPushButton.InputButton::setDebug(true);

  roofOpenSwitch.begin();
  roofCloseSwitch.begin();
  oscPushButton.begin();
  scopeMountParkSwitch.begin();
  scopeUNSafeNotParkedLED.begin();
  lastShutterState = shutterError;
  currentRorStatus.rorCurrentPosition.shutterState = shutterError;
  isOpenSensorClosed = false;
  isCloseSensorClosed = false;
  isEngagedRelayPulse = false;
  rorDebug = false;
}

// Destructor
ROR_Controller::~ROR_Controller()
{
}

// The the RoR Structure for displaying to the website
void ROR_Controller::getRORStatus(ROR_Status &destination)
{
  ROR_Controller::updateRORStatus();
  destination.scopeParkSafe.isTrue = currentRorStatus.scopeParkSafe.isTrue;
  destination.scopeParkSafe.shutterState = currentRorStatus.scopeParkSafe.shutterState;
  destination.rorCurrentPosition.isTrue = currentRorStatus.rorCurrentPosition.isTrue;
  destination.rorCurrentPosition.shutterState = currentRorStatus.rorCurrentPosition.shutterState;
}

// Updates the RoR Structure with current status
/*
  these are the states
const int shutterOpen = 0;
const int shutterOpening = 1;
const int shutterClosing = 2;
const int shutterClosed = 3;
const int shutterError = 4;
const int atPark = 0;
const int unPark = 1;
*/
// isOpenSensorClosed
// isCloseSensorClosed
// Updates the RoR Structure with current status
void ROR_Controller::updateRORStatus()
{
  // Initialize your variables if necessary
  // lastShutterState = shutterClosed; // For example
  // rorMovingTimeCounter = 0;
  // rorMovingTimeReached = 100; // Some reasonable value

  if (isOpenSensorClosed)
  {
    // The Open sensor is true, set the state to shutterOpen (0)
    currentRorStatus.rorCurrentPosition.shutterState = shutterOpen;
    lastShutterState = shutterOpen;
    rorMovingTimeCounter = 0;
  }
  else if (isCloseSensorClosed)
  {
    // The Close sensor is true, set the state to shutterClosed (3)
    currentRorStatus.rorCurrentPosition.shutterState = shutterClosed;
    lastShutterState = shutterClosed;
    rorMovingTimeCounter = 0;
  }
  else if (lastShutterState == shutterOpen)
  {
    // The roof has started to close (2)
    currentRorStatus.rorCurrentPosition.shutterState = shutterClosing;
    rorMovingTimeCounter++;
  }
  else if (lastShutterState == shutterClosed)
  {
    // The roof has started to open (1)
    currentRorStatus.rorCurrentPosition.shutterState = shutterOpening;
    rorMovingTimeCounter++;
  }
  // Check if the moving state has exceeded the threshold (4)
  if (rorMovingTimeCounter >= rorMovingTimeReached)
  {
    currentRorStatus.rorCurrentPosition.shutterState = shutterError;
  }
}


void ROR_Controller::updatedInputSensorsButtons()
{
  // Opened Roof Sensor 
  roofOpenSwitch.updateButtonPin();
  isOpenSensorClosed = roofOpenSwitch.isPressed();
  // Closed Roof Sensor 
  roofCloseSwitch.updateButtonPin();
  isCloseSensorClosed = roofCloseSwitch.isPressed();
  // Scope parked Sensor
  scopeMountParkSwitch.updateButtonPin();
  if (scopeMountParkSwitch.isPressed())
  {
    currentRorStatus.scopeParkSafe.isTrue = true;
    currentRorStatus.scopeParkSafe.shutterState = atPark;
    scopeUNSafeNotParkedLED.updateLed(false);
  }
  else
  {
    currentRorStatus.scopeParkSafe.isTrue = false;
    currentRorStatus.scopeParkSafe.shutterState = unPark;
    scopeUNSafeNotParkedLED.updateLed(true);
  }

  relayControl.setScopeMountParkSafeRelay(currentRorStatus.scopeParkSafe.isTrue);

  oscPushButton.updateButtonPin();
  if (oscPushButton.isPulseTriggered())
  {
    isEngagedRelayPulse = true;
  }
  if (isEngagedRelayPulse)
  {
    relayControl.setScopeMountParkSafeRelay(currentRorStatus.scopeParkSafe.isTrue);
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
