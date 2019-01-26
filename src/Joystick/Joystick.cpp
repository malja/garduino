#include "Joystick.h"

Joystick::Joystick() {
    _last_direction = MoveDirection::None;
    _last_direction_reported = 0;
    _last_click_reported = 0;
}

void Joystick::update() {
    unsigned long long ms = millis();

    PSJoystick::update(ms);

    // Is time for checking direction
    if (ms - _last_direction_reported > 1000) {
        // Check current direction
        if (abs(getX()) > 20 || abs(getY()) > 20) {

            if (abs(getX()) >= abs(getY())) {
                if (getX() > 0) {
                    _last_direction = MoveDirection::Right;
                } else {
                    _last_direction = MoveDirection::Left;    
                }
            } else {
                if (getY() < 0) {
                    _last_direction = MoveDirection::Up;
                } else {
                    _last_direction = MoveDirection::Down;
                }
            }

            _last_direction_reported = ms;
        } else {
            _last_direction = MoveDirection::None;
        }

    } 
    
    if (ms - _last_click_reported > 1000) {
        if (isPressed()) {
            _last_clicked = true;
            _last_click_reported = ms;
        } else {
            _last_clicked = false;
        }
    }
}

Joystick::MoveDirection Joystick::getDirection() {
    Joystick::MoveDirection direction = _last_direction;
    _last_direction = Joystick::MoveDirection::None;
    
    return direction;
}

bool Joystick::isClicked() {
    bool tmp = _last_clicked;
    _last_clicked = false;
    return tmp;
}