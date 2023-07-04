#ifndef ROR_CONTROLLER_H
#define ROR_CONTROLLER_H

#include "settings.h"
#include "LedLight.h"
#include "InputButtonLED.h"
#include "InputButtonLEDPulse.h"
#include "relay_Control.h"

#define HOLD_BUTTON_DOWN_THIS_LONG_TO_TRIGGER 1000
#define TRIGGER_PULSE_DURATION 25
#define DISABLE_TRIGGER_PULSE_BUTTON_DURATION 3000


struct ROR_Status
{
    char rorCurrentPosition[8];
    char IsScopeParkSafe[15];
};

struct ROR_Item
{
    bool isTrue;
    char webName[16];
    char serialName[16];
};

class ROR_Controller
{
public:
    ROR_Controller();
    ~ROR_Controller();

    void updatedInputSensorsButtons();
    ROR_Status *getRORStatus();
    void setIsEngagedRelayPulseTrue(void);

private:
    bool rorDebug = false;
    int rorMovingTimeCounter = 0;
    const int rorMovingTimeReached = 60 / (WEBUPDATE / 1000); // The updateRORStatus checks for roof lost once it passes a time limit of about 60 seconds.
    bool isEngagedRelayPulse = false;
    bool isEnagagedRelayPulseHapping = false;
    int engagedRelayPulseTimer = 0;

    bool isTemporaryDisableOSCPulse = false;
    bool isTemporaryDisableOSCHapping = false;
    long temporaryDisableOSCTimer = 0;

    ROR_Status rorStatusStruct;

    InputButtonLED roofOpenSwitch;
    InputButtonLED roofCloseSwitch;
    InputButtonLED scopeMountParkSwitch;
    InputButtonLEDPulse oscPushButton;

    LedLight scopeUNSafeNotParkedLED;
    LedLight roofMovingLED;

    Relay_Control relayControl;

    /*
        LedLight scopeSafeParked_LED;

        LedLight opened_Sensor_LED;
        LedLight closed_Sensor_LED;
        LedLight osc_button_LED;
    */
    ROR_Item itemSafe{false, "ScopeIsParked", "safe,"};
    ROR_Item itemUnSafe{false, "ScopeNotParked", "unsafe,"};
    ROR_Item itemOpen{false, "Opened", "opened,"};
    ROR_Item itemClosed{false, "Closed", "closed,"};
    ROR_Item itemUnknown{false, "Unknown", "unknown,"};
    ROR_Item itemMoving{false, "Moving", "moving,"};
    // other that need the # (pound)
    ROR_Item itemMovingPound{false, "", "moving#"};
    ROR_Item itemUnknownPound{false, "", "unknown#"};
    ROR_Item itemNotMovingClosePound{false, "", "not_moving_c#"};
    ROR_Item itemNotMovingOpenPound{false, "", "not_moving_o#"};
    ROR_Item itemRoofLost{false, "", "unknown#,"};

    void updateRORStatus();

    /*
    // ROR Controller Defines  define in setup.h

    #define INPUT_PIN_OPENED_SENSOR 34            // roof open sensor
    #define INPUT_PIN_CLOSED_SENSOR 35            // roof closed sensor
    #define INPUT_PIN_SCOPE_PARKED_SAFE_SENSOR 36 // scope safety Park Sensor also Arduino built in LED
    */

    const int TURN_SCOPE_PARK_SAFE_ON = 1;
    const int TURN_SCOPE_PARK_SAFE_OFF = 2;
    const int OPEN_THE_ROOF = 3;
    const int STOP_OPENING_ROOF = 4;
    const int CLOSE_THE_ROOF = 5;
    const int STOP_CLOSING_ROOF = 6;
    const int PARK_STATUS = 7;
    const int GET_STATUS = 8;

    const char *OPENED = "Open";
    const char *CLOSED = "Closed";
    const char *MOVING = "Moving";
    const char *UNKNOWN = "Unkown";
    unsigned long debounceTimeLength_ScopeParkSafeSensor = 500; // scope mount park sensor switch needs a bit of debounce
    unsigned long previousDebounceTime_ScopeParkSafeSensor = 0;

    void outputStatusToSerial();
};

#endif