#include "Joystick.hpp"

Joystick::Joystick() {
    _last_direction = EventJoystickMoveDirection::None;
    _last_direction_reported = 0;
}

void Joystick::update(time_ms ms) {
    PSJoystick::update(ms);

    // Is time for checking direction
    if (ms - _last_direction_reported > 1000) {
        // Check current direction
        if (abs(getX()) > 20 || abs(getY()) > 20) {
            Serial.println("Joystick>Direction updated");

            Serial.print("X = ");
            Serial.print(getX());
            Serial.print(" Y = ");
            Serial.println(getY());
            
            if (abs(getX()) >= abs(getY())) {
                if (getX() > 0) {
                    _last_direction = EventJoystickMoveDirection::Right;
                } else {
                    _last_direction = EventJoystickMoveDirection::Left;    
                }
            } else {
                if (getY() > 0) {
                    _last_direction = EventJoystickMoveDirection::Up;
                } else {
                    _last_direction = EventJoystickMoveDirection::Down;
                }
            }

            _last_direction_reported = ms;
        } else {
            Serial.println("Joystick>Direction reset");
            _last_direction = EventJoystickMoveDirection::None;
        }
    }
}

EventJoystickMoveDirection Joystick::getDirection() {

    Serial.println("Joystick>Direction returned and reset");
    EventJoystickMoveDirection direction = _last_direction;
    _last_direction = EventJoystickMoveDirection::None;
    
    return direction;
}