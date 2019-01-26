#include <Arduino.h>
#include "TaskError.h"
#include "../App.h"

void TaskError() {

    pinMode(LED_BUILTIN, OUTPUT);
    
    // Show error code
    if (APP.getRegister(App::RegisterID::Third) == 0) {
        APP.display.clear();
        APP.display.print(APP.getRegister(App::RegisterID::Second));
        APP.setRegister(App::RegisterID::Third, 1);
    }
    
    if (millis() - APP.getRegister(App::RegisterID::First) > 1000) {
        digitalWrite(LED_BUILTIN, 1 ^ digitalRead(LED_BUILTIN));
        APP.setRegister(App::RegisterID::First, millis());
    }
}