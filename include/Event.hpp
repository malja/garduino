#ifndef EVENT_HPP
#define EVENT_HPP

#include "EventType.hpp"
#include "JoystickEvent.hpp"

typedef union Event {
    EventType type;
    JoystickEvent joystick;
} Event;

#endif