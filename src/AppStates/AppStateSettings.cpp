#include "AppStateSettings.hpp"
#include "../App.hpp"

void AppStateSettings::onStart() {
    this->app->display.clear();
    this->app->display.print("Settings");
}

void AppStateSettings::update(unsigned long long ms) {
    Event event;
    while(this->app->pollEvent(event)) {
        if (EventType::Joystick == event.type && EventTypeJoystick::Direction == event.joystick.type) {
            // Move left
            if (EventJoystickMoveDirection::Left == event.joystick.direction) {
                // Back to main display
                this->app->switchState(AppStateIDs::Main);
                break;
            }
        }
    }
}

void AppStateSettings::onExit() {

}
