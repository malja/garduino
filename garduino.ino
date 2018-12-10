#include "src/App.hpp"

App garduino;

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

void setup() {
    Serial.begin(9600);

    if (!garduino.setup()){
        blink();
    }

}

void loop() {
  garduino.run();
}
