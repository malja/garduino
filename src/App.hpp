#ifndef APP_HPP
#define APP_HPP

#include "config.hpp"
//#include "./Subsystems/Storage.hpp"
#include "./Subsystems/Joystick/Joystick.hpp"
#include "./Subsystems/Display/Display.hpp"
#include "./AppStates/AppState.hpp"
#include "./Events/Event.hpp"

#include <Arduino.h>

class App {

    public:
        App();
        ~App();

        void setup();
        void run();

		/**
		 * Change current app state. Old one will be paused.
		 * @param state One of available state IDs.
         * @return False for invalid ID (negative or out of bounds set by APP_STATES_COUNT).
		 */
        bool switchState(AppStateIDs state);

        /**
         * Register new app state. Application is responsible for freeing the
         * memory.
         * @param id State will be available with this id.
         * @param state Pointer to new AppState instance.
         * @return False for invalid ID (negative or out of bounds set by APP_STATES_COUNT) or
         * null state pointer.
         */
        bool addState(AppStateIDs id, AppState *state);

        bool pollEvent(Event &event);
        void addEvent(const Event &event);

        Display display;
        //Storage storage;
    
    protected:

        /*
         * Create all events from underlaying hardware.
         */
        void createEvents(unsigned long long ms);

        /**
         * Read data from joystick and create events if necessary.
         */
        void handleJoystickEvents(unsigned long long ms);

        /**
         * Find event in _events array, which is not yet used and return it. Alternativelly remove the oldest event
		 * and return it.
         */
        Event* getFreeEventFromPool();

		// List of events
        Event _events[EVENTS_POOL_SIZE];
        uint8_t _events_count;	///< Holds index of last inserted event.

		// List of app states
        AppState *_states[APP_STATES_COUNT];
		int8_t _current_state;  ///< Holds one of AppStateIDs' values or -1. If positive, it sets current state.
                                ///< -1 means there is no current state set.

        Joystick _joystick;
};

#endif
