#include "Event.hpp"

Event::Event() {
    type = EventType::None;
    
    joystick.type = EventTypeJoystick::None;
    joystick.pressed = false;
    joystick.x_axis = 0;
    joystick.y_axis = 0;
}

Event &Event::operator=(const Event &copy) {
    type = copy.type;

    joystick.type = copy.joystick.type;
    joystick.pressed = copy.joystick.pressed;
    joystick.x_axis = copy.joystick.x_axis;
    joystick.y_axis = copy.joystick.y_axis;
    joystick.direction = copy.joystick.direction;
}