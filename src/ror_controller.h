#ifndef ROR_CONTROLLER_H
#define ROR_CONTROLLER_H

#include "settings.h"



class ROR_Controller
{
public:
    ROR_Controller();
    ~ROR_Controller();

    void updatedInputSensorsButtons();
    String getRORPosistion();
    String getIsScopeParkSafe();
    // Test of the buttons
    // Button buttonTest = {INPUT_PIN_SCOPE_PARKED_SAFE_SENSOR, HIGH, 0, 0};
    Button scopePark_Sensor_INPUT = {INPUT_PIN_SCOPE_PARKED_SAFE_SENSOR, HIGH, 0, 0};


private:
    bool rorDebug;
 
    DebounceIt opened_Sensor_INPUT;
    DebounceIt closed_Sensor_INPUT;
    // DebounceIt scopePark_Sensor_INPUT;
    DebounceIt OSC_Btn_BUTTON;
    LedLight scopeSafeParked_LED;
    LedLight scopeUNSafeNotParked_LED;
    LedLight opened_Sensor_LED;
    LedLight closed_Sensor_LED;
    LedLight roof_moving_LED;
    LedLight roof_unknown_LED;
    LedLight osc_button_LED;

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

    bool isScopeParkSafeSensorEngaged = false;
    bool isScopeParkSafeSensorEnabledOverride = false;

    bool isClosedSensorEngaged = false;

    bool isOpenedSensorEngaged = false;

    int currentOSCButtonState = HIGH;
    int previousOSCButtonState = HIGH;
    bool isOSCButtonEngaged = false;

    unsigned long momentaryRelayTimeLength = 750;  // how long to keep the momentary OSC relay down for
    unsigned long previousTime_MomentaryRelay = 0; // counter for the momentary OSC relay stays to say closed to act like a push button for AleKo
    int currentMomentaryRelayState = HIGH;
    int previousMomentaryRelayState = HIGH;
    bool isMomentaryRelayEngaged = false;
    bool isMomentaryRelayTimeLimitReached = false;

    unsigned long delayOSCOperationTimeLength = 5000; // how long to keep the OSC button from operating so a user cannot just keep pushing button over and over real fast
    unsigned long previousTime_delayOSCOperation = 0; // counter for the OSC operation delay stays to say closed to act like a push button for AleKo
    bool isDelayOSCOperationEngaged = false;

    unsigned long lostRoofTimeLimit = 60000; // How to wait till the roof is lost because the opened or closed sensor is not engaged so how long before the lost roof message is shown.
    unsigned long previousTime_LostRoof = 0;
    int currentLostRoofState = HIGH;
    int previousLostRoofState = HIGH;
    bool isRoofLost = false; // roof not reporting state

    void outputStatusToSerial();
};

#endif