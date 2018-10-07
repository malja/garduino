#ifndef JOYSTICK_EVENT_HPP
#define JOYSTICK_EVENT_HPP

#include "EventJoystickType.hpp"

/**
 * Defines joystick event.
 */
typedef struct EventJoystick {
    EventJoystickType type; ///< One of joystick events
    bool pressed;           ///< Is joystick button pressed
    int8_t x_axis;          ///< Joystick location on horizontal axis. 
                            ///  Value from -100 to 0 means left. Value from 0 to 100 is right.
    int8_t y_axis;          ///< Joystick location on vertical axis.
                            ///  Value from -100 to 0 means up and value from 0
                            ///  to 100 means down.
} JoystickEvent;

#endif