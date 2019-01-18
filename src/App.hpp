#ifndef APP_HPP
#define APP_HPP

#include "config.hpp"
#include "./Subsystems/Storage/Storage.hpp"
#include "Subsystems/Joystick/Joystick.hpp"
#include "Subsystems/Display/SSD1306AsciiAvrI2c.h"
#include "Menu/MenuScreen.hpp"
#include "interruptions.hpp"
#include "Events/Event.hpp"
#include "Subsystems/WateringStats/WateringStats.hpp"
#include "Helpers/blink.hpp"

#include <Arduino.h>

class App {

    public:

        /**
         * Get singleton instance of App.
         */
        static App &getInstance() {
            static App instance;
            return instance;
        }

        // App is singleton, no need for those :)
        App(App const &other) = delete;
        void operator=(App const &other) = delete;

        /**
         * This method should be called in Arduino's `setup` function to
         * initialize Garduino application.
         * 
         * Note: This method is required, because some Arduino specific calls,
         * for example readAnalog will fail when executed outside `setup` or
         * `loop` functions. App instance is created outside of those functions
         * and thus calling just the constructor is not enought.
         */
        bool setup();

        /**
         * This method should be called in Arduino's `loop` function. It consist
         * of all app and app state specific runtime.
         */
        void run();

		/**
		 * Change current menu screen.
		 * @param screen One of available screen IDs.
         * @return False for invalid ID (negative or out of bounds set by APP_SCREENS_COUNT).
		 */
        bool switchScreen(MenuScreen::Id screen);

        /**
         * Register new menu screen. Application is responsible for freeing the
         * memory.
         * @param id Screen will be available with this id.
         * @param screen Pointer to new MenuScreen instance.
         * @return False for invalid ID (negative or out of bounds set by APP_SCREENS_COUNT) or
         * null state pointer.
         */
        bool addScreen(MenuScreen::Id id, MenuScreen *screen);

        /**
         * Get one event from queue. Event data is filled in `event` parameter. 
         * @param event Fill this event with one of events from pool.
         * @return True if there is unhandled event and proper data was set to
         * `event` parameter. False if no event is pending.
         */
        bool pollEvent(Event &event);

        /**
         * Push new event to the event loop. 
         * @param event New event to be added. 
         */
        void addEvent(const Event &event);

        /**
         * Contains OLED display functionality. 
         */
        SSD1306AsciiAvrI2c display;

        Storage storage;
    
    protected:

        /*
         * Handle events occured from hardware - joystick movement,
         * interruptions, etc. - and fill event pool.
         */
        void createEvents(time_ms ms);

        /**
         * Read data from joystick and create events if necessary.
         */
        void handleJoystickEvents(unsigned long long ms);

        void handleSystemEvents();

        /**
         * Find event in _events array, which is not yet used and return it. Alternativelly remove the oldest event
		 * and return it.
         */
        Event* getFreeEventFromPool();

		// List of events
        Event _events[EVENTS_POOL_SIZE];
        uint8_t _events_count;	///< Holds index of last inserted event.

		// List of app states
        MenuScreen *_screens[APP_SCREENS_COUNT];
		int8_t _current_screen;  ///< Holds one of MenuScreen::ID's values or -1. If positive, it sets current screen.
                                ///< -1 means there is no current screen set.

        Joystick _joystick;
        WateringStats _stats;

    private:
        App();
        ~App();
};

#endif
