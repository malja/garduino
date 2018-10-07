#ifndef APP_HPP
#define APP_HPP

#include "config.hpp"
#include "Storage.hpp"
#include "PSJoystick.hpp"
#include "Display.hpp"
#include "AppState.hpp"
#include "AllStates.hpp"
#include "Event.hpp"
#include <Arduino.h>

class App {

    public:
        App();
        void update();

		/**
		 * Change current app state. Old one will be paused.
		 * @param state One of available state IDs.
		 */
        void setState(AppStateIDs state);
        bool pollEvent(const Event &event);

        Display display;
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

        /**
         * Find event in _events array, which is not yet used and return it. Alternativelly remove the oldest event
		 * and return it.
         */
        Event* getFreeEventFromPool();

		// List of events
        Event _events[EVENTS_POOL_SIZE];
        uint8_t _events_count;	///< Holds index of last inserted event.

		// List of app states
        AppState *_states[AppStateIDs::__MAX__];
		uint8_t _current_state;

        PSJoystick _joystick;
};

#endif
