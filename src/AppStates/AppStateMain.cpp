#include "AppStateMain.hpp"
#include "../App.hpp"

void AppStateMain::onStart() {
    Serial.println("Main>OnStart executed.");
    app->display.clear();
    app->display.print("BatteryLevelPlaceholder");
    app->display.print("Garduino 0.1");
}

void AppStateMain::update(unsigned long long ms) {
    Event ev;

    Serial.println("Main>Polling events...");

    // Get all events
    while(app->pollEvent(ev)) {

        // Interested only in joystick movement events
        if (EventType::Joystick == ev.type &&
            EventTypeJoystick::Direction == ev.joystick.type) {

            // Move to the rigth
            if (EventJoystickMoveDirection::Right == ev.joystick.direction) {

                Serial.println("Main>New state - statistics");

                // Show statistics
                this->app->switchState(AppStateIDs::Statistics);
                break;
            } 
            
            // Move to the left
            if (EventJoystickMoveDirection::Left == ev.joystick.direction) {
                // Show settings

                Serial.println("Main>New state settings");
                this->app->switchState(AppStateIDs::Settings);
                break;
            }
        }
    }
    Serial.println("Main>End of polling...");
}

void AppStateMain::onExit() {

}