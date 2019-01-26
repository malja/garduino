#ifndef GARDUINO_JOYSTICK_H
#define GARDUINO_JOYSTICK_H

#include "../config.h"
#include "PSJoystick.h"

class Joystick : public PSJoystick {

    public:

        typedef enum MoveDirection {
            Up,
            Right,
            Down,
            Left,
            None
        } MoveDirection;

        Joystick();

        /**
         * Report latest registered joystick direction. 
         * Direction is set to None after call. And will stay so until next
         * joystick movement.
         */
        MoveDirection getDirection();
        bool isClicked();
        void update();

    protected:

        unsigned long long _last_direction_reported;
        unsigned long long _last_click_reported;

        bool _last_clicked;
        MoveDirection _last_direction;
};

#endif