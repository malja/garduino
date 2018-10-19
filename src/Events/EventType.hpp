#ifndef EVENT_TYPE_HPP
#define EVENT_TYPE_HPP

typedef enum class EventType {
    Joystick,
	None ///< This event type is used as a mark, that event does not hold any data.
} EventType;

#endif