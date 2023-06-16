#ifndef ROR_CONTROLLER_H
#define ROR_CONTROLLER_H

#include "settings.h"

#define ROR_ENGAGE_RELAY_PULSEWIDTH 500
#define ROR_TEMPORARY_DISABLE_OSC_BUTTON 2500

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
    LedLight *ptrLedLight;
};

class ROR_Controller
{
public:
    ROR_Controller();
    ~ROR_Controller();

    void updatedInputSensorsButtons();
    ROR_Status *getRORStatus();

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

    // Test of the buttons
    // Button buttonTest = {INPUT_PIN_SCOPE_PARKED_SAFE_SENSOR, HIGH, 0, 0};
    Button scopePark_Sensor_INPUT = {INPUT_PIN_SCOPE_PARKED_SAFE_SENSOR, HIGH, 0, 0, HIGH};
    Button opened_Sensor_INPUT = {INPUT_PIN_OPENED_SENSOR, HIGH, 0, 0, HIGH};
    Button closed_Sensor_INPUT = {INPUT_PIN_CLOSED_SENSOR, HIGH, 0, 0, HIGH};
    Button OSC_Btn_BUTTON_INPUT = {INPUT_PIN_OSC_BUTTON, HIGH, 0, 0, HIGH};

    LedLight scopeSafeParked_LED;
    LedLight scopeUNSafeNotParked_LED;
    LedLight opened_Sensor_LED;
    LedLight closed_Sensor_LED;
    LedLight osc_button_LED;

    ROR_Item itemSafe{false, "ScopeIsParked", "safe,", &scopeSafeParked_LED};
    ROR_Item itemUnSafe{false, "ScopeNotParked", "unsafe,", &scopeUNSafeNotParked_LED};
    ROR_Item itemOpen{false, "Opened", "opened,", &opened_Sensor_LED};
    ROR_Item itemClosed{false, "Closed", "closed,", &closed_Sensor_LED};
    ROR_Item itemUnknown{false, "Unknown", "unknown,", nullptr};
    ROR_Item itemMoving{false, "Moving", "moving,", nullptr};
    // other that need the # (pound)
    ROR_Item itemMovingPound{false, "", "moving#", nullptr};
    ROR_Item itemUnknownPound{false, "", "unknown#", nullptr};
    ROR_Item itemNotMovingClosePound{false, "", "not_moving_c#", nullptr};
    ROR_Item itemNotMovingOpenPound{false, "", "not_moving_o#", nullptr};
    ROR_Item itemRoofLost{false, "", "unknown#,", nullptr};

    void updateRORStatus();
    void engageRelayPulse();
    void temporaryDisableOSCButton();

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