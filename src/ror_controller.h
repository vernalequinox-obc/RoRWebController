#ifndef ROR_CONTROLLER_H
#define ROR_CONTROLLER_H

#include <Arduino.h>


class ROR_Controller {
    public:
        ROR_Controller();
        ~ROR_Controller();
        static String processor(const String &var);

  private:
        boolean debug;
        static const int ledPin =2; // Set LED GPIO
        String ledState; // Stores LED state       
};

#endif