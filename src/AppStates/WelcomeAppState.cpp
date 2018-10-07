#include "WelcomeAppState.hpp"

void WelcomeAppState::onStart() {
    this->_app.display.clear();
    this->_app.display.print("BatteryLevelPlaceholder");
    this->_app.display.print("Welcome to Garduino 0.1");
}

void WelcomeAppState::update(unsigned long long ms) {
    Event ev;

    // Get all events
    while(this->_app.pollEvents(&ev)) {

        // Interested only in joystick movement events
        if (ev.type == EventType::Joystick &&
            ev.joystick.type == EventJoystickType::MOVE) {

            // Move to the rigth
            if (ev.joystick.x_axis > 0) {
                this->_app.setState();
            
            // Move to the left
            } else if (ev.joystick.x_axis < 0) {
                this->_app.setState(new OverrideAppState());
            }
        }
    }
}