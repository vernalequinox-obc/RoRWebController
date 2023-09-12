#ifndef INPUTBUTTONLED_H
#define INPUTBUTTONLED_H

#include <Arduino.h>
#include "InputButton.h"
#include "LedLight.h"

class InputButtonLED : public InputButton, public LedLight
{
public:
    InputButtonLED();
    ~InputButtonLED();

    void begin(void);
    bool isPressed(void);
    bool isReleased(void);
    void setButtonLedPin(uint8_t aButtonPin, uint8_t aLedPin);

    void setDeviceLEDName(const char *aName);
    char *getDeviceLEDName(void);
    void setDeviceButtonName(const char *aName);
    char *getDeviceButtonName(void);
    
    void setDebugLED(bool aDebug);
    bool getDebugLED(void);
    void setDeviceEnabledLED(bool aIsDeviceEnabled);
    bool getDeviceEnabledLED(void);

    void setDebugButton(bool aDebug);
    bool getDebugButton(void);
    void setDeviceEnabledButton(bool aIsDeviceEnabled);
    bool getDeviceEnabledButton(void);

    void setDebug(bool aDebug);


private:
};

#endif
