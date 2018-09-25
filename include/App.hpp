#ifndef APP_HPP
#define APP_HPP

#include "config.hpp"
#include "Storage.hpp"
#include "AppState.hpp"
#include "Event.hpp"
#include <Arduino.h>

class App {

    public:
        App();
        void update();
        void setState(AppState *state);
        bool pollEvent(const Event &event);

        Storage storage;
    
    protected:

        /*
         * Create all events from underlaying hardware.
         */
        void createEvents(unsigned long long ms);

        /**
         * Read data from joystick and create events if necessary.
         */
        void handleJoystick(unsigned long long ms);

        std::vector<Event> _events;
        AppState *_state;

        Joystick _joystick;
};

#endif
