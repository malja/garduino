#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include "../../Helpers/typedefs.hpp"
#include "../../Events/EventJoystick.hpp"
#include "../../config.hpp"
#include "../../Subsystems/Joystick/PSJoystick.hpp"

class Joystick : public PSJoystick {

    public:

        Joystick();

        /**
         * Report latest registered joystick direction. 
         * Direction is set to None after call. And will stay so until next
         * joystick movement.
         */
        EventJoystickMoveDirection getDirection();
        bool isClicked();
        void update(time_ms m);

    protected:

        time_ms _last_direction_reported;
        time_ms _last_click_reported;

        bool _last_clicked;
        EventJoystickMoveDirection _last_direction;
};

#endif