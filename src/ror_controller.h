#ifndef ROR_CONTROLLER_H
#define ROR_CONTROLLER_H

#include "settings.h"


struct RoofStatusStruct
{
    // state variables
    uint8_t pin;
    bool on;

    // methods
    void update()
    {
        on = digitalRead(RORSTATUS_PIN);
    }
};


class ROR_Controller {
    public:
        ROR_Controller();
        ~ROR_Controller();
        String getRORPosistion();
        RoofStatusStruct rorStatusStruct;

  private:
        boolean debug;
        static const int ledPin =2; // Set LED GPIO
        String ledState; // Stores LED state       
};

#endif