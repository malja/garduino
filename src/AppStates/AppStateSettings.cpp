#include "AppStateSettings.hpp"

void AppStateSettings::onStart() {
    this->app->display.clear();
    this->app->display.print("Settings");
}

void AppStateSettings::update(unsigned long long ms) {
    Event event;
    while(this->app->pollEvent(&event)) {
        if (EventType::Joystick == event.type && EventTypeJoystick::MOVE == event.joystick.type) {
            // Move left
            if (event.joystick.x_axis < 0) {
                // Back to main display
                this->app->setState(AppStateIDs::MAIN);
                break;
            }
        }
    }
}

void AppStateSettings::onExit() {

}
