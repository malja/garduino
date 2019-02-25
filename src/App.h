#ifndef GARDUINO_APP_H
#define GARDUINO_APP_H

#include <Arduino.h>

#include "Tasks/Tasks.h"
#include "Joystick/Joystick.h"
#include "Storage/Storage.h"
#include "Display/Display.h"
#include "Menu/MenuScreen.h"


class App {

    public:

        /**
         * Register names for accesing app register via get/setRegister methods.
         * 
         * Registers are shared space used by tasks to pass additional information
         * between transitions.
        */
        typedef enum RegisterID {
            First,
            Second,
            Third,
            Fourth
        } RegisterID;

        /**
         * List of error codes for easier debuging.
        */
        typedef enum ErrorCodeID {
            Generic,
            StorageReadFailed,
            StorageSetupFailed,
            StorageWriteFailed,
            StorageSetDefaultsFailed,
            UnknownState,
        } ErrorCodeID;

        /**
         * Contains list of all application states which can occur in Garduino.
        */
        typedef enum StateID {

            /// System was waked up by user clicking on control joystick.
            WakedUpByUser,
            
            /// Garduino was waked up by automatic watchdog timer.
            WakedUpByTimer,

            /// Run humidity check and if levels are okay, go back to sleep.
            /// Otherwise run automatic watering.
            HumidityCheckAndSleep,

            /// Perform manual soil humidity check. If levels are fine, go back to
            /// main menu, else run override watering.
            HumidityCheck,
            
            /// Show main menu
            MenuMain,
            
            /// Show settings menu
            MenuSettings,

            /// Show statistics
            MenuStatistics,
            
            /// Run automatic watering and then return back to sleep.
            WateringAutomatic,

            /// Run manual watering and then return to main menu.
            WateringManual,

            /// Go back to sleep.
            EnterSleepMode,
            
            /// Currently in sleep mode.
            Sleeping,

            /// 
            CalibrateHumidityLow,
            CalibrateHumidityHigh,
            CalibrateHumidityFinish,

            /// Something bad happened in app
            Error
        } StateID;

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
         * Returns current AppState.
        */
        inline App::StateID getState() const {
            return _app_state;
        }

        /**
         * Apart from registers, application state dictates what action should be performed in
         * each task.
         * 
         * @param state             Next application state.
         * @param clear_registers   Should register be cleaned up before state change?
        */
        inline void setState(App::StateID state, bool clear_registers = true) {
            _app_state = state;

            if (clear_registers) {
                _registers[0] = 0;
                _registers[1] = 0;
                _registers[2] = 0;
                _registers[3] = 0;
            }
        }

        /**
         * Switch to next task.
         * 
         * Task are void functions which perform periodical functionality based on AppState and
         * values in registers.
        */
        inline void setNextTask(Task next) {
            _current_task = next;
        }

        /**
         * Save some value into registers.
        */
        inline void setRegister(App::RegisterID id, uint32_t value) {
            _registers[(int)id] = value;
        }

        /**
         * Return value from register.
         * 
         * @param id    Which register's value should be returned.
        */
        inline uint32_t getRegister(App::RegisterID id) const {
            return _registers[(int)id];
        }
    
        /**
         * Enter infinite loop showing error code on display and blinking internal led
         * to inform that something went wrong.
         * 
         * @param error     Error identification code which should be shown on display.
         * @note This method works only in debug mode!
        */
        void switchToErrorMode(ErrorCodeID error = ErrorCodeID::Generic);

        Display display;
        Storage storage;
        Joystick joystick;
        MenuScreen *screens[APP_SCREENS_COUNT];

    protected:

        volatile uint32_t  _registers[4];
        volatile App::StateID _app_state;
        volatile Task      _current_task;

    private:
        App();
        ~App();
};

#define APP App::getInstance()

#endif
