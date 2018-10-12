#include "AppStateMain.hpp"

void AppStateMain::onStart() {
    this->app->display.clear();
    this->app->display.print("BatteryLevelPlaceholder");
    this->app->display.print("Welcome to Garduino 0.1");
}

void AppStateMain::update(unsigned long long ms) {
    Event ev;

    // Get all events
    while(this->app->pollEvents(&ev)) {

        // Interested only in joystick movement events
        if (ev.type == EventType::Joystick &&
            ev.joystick.type == EventTypeJoystick::MOVE) {

            // Move to the rigth
            if (ev.joystick.x_axis > 0) {
                // Show statistics
                this->app->setState(AppStateIDs::STATISTICS);
                break;
            } 
            
            // Move to the left
            if (ev.joystick.x_axis < 0) {
                // Show settings
                this->app->setState(AppStateIDs::SETTINGS);
                break;
            }
        }
    }
}

void AppStateMain::onExit() {

}