#include "AppStateStatistics.hpp"

void AppStateStatistics::onStart() {
    this->app->display.clear();
    this->app->display.print("Settings");
}

void AppStateStatistics::update(unsigned long long ms) {
    Event event;
    while(this->app->pollEvent(&event)) {
        if (EventType::Joystick == event.type && EventTypeJoystick::MOVE == event.joystick.type) {

            // Move right
            if (event.joystick.x_axis > 0) {
                // Back to main display
                this->app->setState(AppStateIDs::MAIN);
                break;
            }
        }
    }
}

void AppStateStatistics::onExit() {

}
