#include "AppStateStatistics.hpp"
#include "../App.hpp"

void AppStateStatistics::onStart() {
    app->display.clear();
    app->display.print("Settings");
}

void AppStateStatistics::update(unsigned long long ms) {
    Event event;
    while(app->pollEvent(event)) {
        if (EventType::Joystick == event.type && EventTypeJoystick::Direction == event.joystick.type) {

            // Move right
            if (EventJoystickMoveDirection::Right == event.joystick.direction) {
                // Back to main display
                app->switchState(AppStateIDs::Main);
                break;
            }
        }
    }
}

void AppStateStatistics::onExit() {

}
