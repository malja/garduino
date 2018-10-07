#ifndef EVENT_HPP
#define EVENT_HPP

#include "EventType.hpp"
#include "EventJoystick.hpp"

typedef union Event {
    EventType type;
    EventJoystick joystick;
} Event;

#endif