#ifndef EVENT_JOYSTICK_TYPE_HPP
#define EVENT_JOYSTICK_TYPE_HPP

/**
 * Defines all event types available for joystick.
 */
typedef enum EventJoystickType {
    MOVE,       ///< Joystick move
    BUTTON      ///< Joystick button was pressed
} EventJoystickType;

#endif