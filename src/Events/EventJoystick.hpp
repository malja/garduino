#ifndef JOYSTICK_EVENT_HPP
#define JOYSTICK_EVENT_HPP

#include <Arduino.h>

typedef enum class EventJoystickMoveDirection {
    Up,
    Right,
    Down,
    Left,
    None
} EventJoystickMoveDirection;

/**
 * Defines all event types available for joystick.
 */
typedef enum class EventTypeJoystick {
    Move,       ///< Joystick move
    Direction,
    Button,      ///< Joystick button pressed
    Click,       ///< Joystick button clicked. This is similar to Direction event type.
                 ///  Click is registered only once per second (or so).
    None
} EventTypeJoystick;

/**
 * Defines joystick event.
 */
typedef struct EventJoystick {
    EventTypeJoystick type; ///< One of joystick events
    bool pressed;           ///< Is joystick button pressed
    int x_axis;          ///< Joystick location on horizontal axis. 
                            ///  Value from -100 to 0 means left. Value from 0 to 100 is right.
    int y_axis;          ///< Joystick location on vertical axis.
                            ///  Value from -100 to 0 means up and value from 0
                            ///  to 100 means down.
    EventJoystickMoveDirection direction;
} JoystickEvent;

#endif