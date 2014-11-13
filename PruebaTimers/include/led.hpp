#ifndef LED_HPP
#define LED_HPP

#include <avr/io.h>

class Led {
    public:
        Led();
        void on();
        void off();
        void toggle();
};

#endif