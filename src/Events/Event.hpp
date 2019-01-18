#ifndef EVENT_HPP
#define EVENT_HPP

#include "EventType.hpp"
#include "EventJoystick.hpp"
#include "EventWatering.hpp"

typedef struct Event {
    EventType type;
    EventJoystick joystick;
    EventWatering watering;

    Event();
    Event &operator=(const Event &copy);
} Event;

#endif