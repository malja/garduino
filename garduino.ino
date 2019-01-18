#include "src/App.hpp"

App garduino = App::getInstance();

void setup() {
    Serial.begin(9600);

    if (!garduino.setup()){
        blink();
    }

}

void loop() {
  garduino.run();
}
