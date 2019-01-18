#ifndef BLINK_HPP
#define BLINK_HPP

#include <Arduion.h>
#include "typedefs.hpp"

/**
 * Function for debug purposes. When executed, blinks builtin led in infinite loop.
 */
void blink() {

    pinMode(LED_BUILTIN, OUTPUT);
    time_ms ms = millis();

    while(true) {
        if (millis() - ms > 1000) {
            digitalWrite(LED_BUILTIN, 1 ^ digitalRead(LED_BUILTIN));
            ms = millis();
        }
    }
}

#endif