#include "src/garduino.hpp"
//#include "src/Subsystems/Joystick/Joystick.hpp"

//Joystick j;
void setup() {
    Serial.begin(9600);

    garduino.setup();
    //j.setup(A0, A1, 2, 250);
}

void loop() {
    //j.update(millis());
    garduino.run();

    /*Serial.print("X = ");
    Serial.print(j.getX());
    Serial.print(" Y = ");
    Serial.println(j.getY());
    
    Serial.print("Direction: ");
    Serial.println((int)j.getDirection());*/
}
