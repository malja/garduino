#ifndef EVENT_HPP
#define EVENT_HPP

#include "EventType.hpp"
#include "EventJoystick.hpp"

typedef struct Event {
    EventType type;
    EventJoystick joystick;

    Event();
    Event &operator=(const Event &copy);
} Event;

#endif