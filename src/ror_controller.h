#ifndef ROR_CONTROLLER_H
#define ROR_CONTROLLER_H

#include "settings.h"
#include "LedLight.h"
#include "InputButtonLED.h"
#include "InputButtonLEDPulse.h"
#include "relay_Control.h"

// ASCOM or Alpaca status of the dome shutter or roll-off roof. 0 = Open, 1 = Closed, 2 = Opening, 3 = Closing, 4 = Shutter status error

const int shutterOpen = 0;
const int shutterClosed = 1;
const int shutterOpening = 2;
const int shutterClosing = 3;
const int shutterError = 4;
const int atPark = 0;
const int unPark = 1;

struct ROR_Item
{
    bool isTrue;
    int shutterState;
};

struct ROR_Status
{
    ROR_Item rorCurrentPosition;
    ROR_Item scopeParkSafe;
};

class ROR_Controller
{
public:
    ROR_Controller();
    ~ROR_Controller();

    void updatedInputSensorsButtons();
    void getRORStatus(ROR_Status &destination);
    void setIsEngagedRelayPulseTrue(void);

private:
    uint32_t lastShutterState;
    bool rorDebug = false;
    int rorMovingTimeCounter = 0;
    const int rorMovingTimeReached = 60 / (WEBUPDATE / 1000); // The updateRORStatus checks for roof lost once it passes a time limit of about 60 seconds.
    bool isEngagedRelayPulse = false;
    bool isEnagagedRelayPulseHapping = false;
    int engagedRelayPulseTimer = 0;

    bool isTemporaryDisableOSCPulse = false;
    bool isTemporaryDisableOSCHapping = false;
    long temporaryDisableOSCTimer = 0;

    bool isOpenSensorClosed = false;
    bool isCloseSensorClosed = false;

    InputButtonLED roofOpenSwitch;
    InputButtonLED roofCloseSwitch;
    InputButtonLED scopeMountParkSwitch;
    InputButtonLEDPulse oscPushButton;

    LedLight scopeUNSafeNotParkedLED;

    Relay_Control relayControl;
    ROR_Status currentRorStatus;

    void updateRORStatus();

    unsigned long debounceTimeLength_ScopeParkSafeSensor = 500; // scope mount park sensor switch needs a bit of debounce
    unsigned long previousDebounceTime_ScopeParkSafeSensor = 0;
};

#endif