#ifndef EVENT_TYPE_HPP
#define EVENT_TYPE_HPP

typedef enum class EventType {
    Joystick,
    WaterMeter, ///< Generated when pulse from water meter is received.
	None ///< This event type is used as a mark, that event does not hold any data.
} EventType;

#endif