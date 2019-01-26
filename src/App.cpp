#include "App.h"
#include "Menu/Screens.h"
#include "Menu/MenuItem.h"

App::App() {
    _registers[0] = 0;
    _registers[1] = 0;
    _registers[2] = 0;
    _registers[3] = 0;

    // Defaults
    _app_state = App::StateID::MenuMain;
    _registers[0] = App::StateID::MenuMain;
    _current_task = TaskShowMenu;
}

bool App::setup() {

    // Connect to EEPROM
    if (!storage.setup(STORAGE_OFFSET_IN_BYTES, STORAGE_SIZE_IN_BYTES)) {
        Serial.print("Setup failed!");
        //switchToErrorMode(App::ErrorCodeID::StorageSetupFailed);
        return false;
    }

    // If there are no data, write defaults
    if (storage.isEmpty()) {
        Serial.print("Storage is empty");
        if (
            !storage.write(HUMIDITY_THRESHOLD_INDEX, DEFAULT_HUMIDITY_THRESHOLD) ||
            !storage.write(LITERS_PER_WATERING_INDEX, DEFAULT_LITERS_PER_WATERING) ||
            !storage.write(HUMIDITY_CHECK_INTERVAL_INDEX, DEFAULT_HUMIDITY_CHECK_INTERVAL) ||
            !storage.write(LITERS_TOTAL_INDEX, DEFAULT_LITERS_TOTAL) ||
            !storage.write(LAST_HUMIDITY_INDEX, DEFAULT_LAST_HUMIDITY)
        ){
            Serial.print("Unable to save defaults");
            //switchToErrorMode(App::ErrorCodeID::StorageSetDefaultsFailed);
            return false;
        }
    }

    // Main menu
    screens[MenuScreenID::Main] = new MenuScreen(
        "Welcome",  // Title
        3,          // number of items

        // Item number 1
        new MenuItem("Settings", -1, []() {
            APP.setState(App::StateID::MenuSettings);
            // next task is ShowMenu
        }),

        // Item number 2
        new MenuItem("Statistics", -1, [](){
            APP.setState(App::StateID::MenuStatistics);
            // next task is ShowMenu
        }),

        // Item number 3
        new MenuItem("Turn off", -1, []() {
            APP.setState(App::StateID::EnterSleepMode);
            APP.setNextTask(TaskSleepMode);
        })
    );

    // Settings menu
    screens[MenuScreenID::Settings] = new MenuScreen(
        "Settings", // Title
        4,          // Number of items

        new MenuItem("Hum. thrsld", HUMIDITY_THRESHOLD_INDEX, nullptr),
        new MenuItem("l/watering", LITERS_PER_WATERING_INDEX, nullptr),
        new MenuItem("Chck interval", HUMIDITY_CHECK_INTERVAL_INDEX, nullptr),
        new MenuItem("Back", -1, []() {
            APP.setState(App::StateID::MenuMain);
            // next task is ShowMenu
        })
    );

    // Statistics menu
    screens[MenuScreenID::Statistics] = new MenuScreen(
        "Statistics",   // Title
        3,              // Number of items

        new MenuItem("Total liters", LITERS_TOTAL_INDEX, nullptr),
        new MenuItem("Cur. humidity", LAST_HUMIDITY_INDEX, nullptr),
        new MenuItem("Back", -1, []() {
            APP.setState(App::StateID::MenuMain);
            // Next task is ShowMenu
        })
    );

    // Setup connection to joystick
    joystick.setup(JOYSTICK_PIN_X_AXIS, JOYSTICK_PIN_Y_AXIS, JOYSTICK_PIN_BUTTON, JOYSTICK_UPDATE_INTERVAL);
    
    // Setup display
    display.begin(DISPLAY_TYPE, DISPLAY_I2C_ADDRESS);
    display.setFont(DISPLAY_FONT);
    display.clear();

    return true;
}

void App::run() {
    _current_task();
}

void App::switchToErrorMode(App::ErrorCodeID error) {

    if (!DEBUG) {
        return;
    }

    // Task error expects start time in first register
    APP.setRegister(App::RegisterID::First, millis());
    APP.setRegister(App::RegisterID::Second, (uint32_t)error);
    
    _app_state = App::StateID::Error;
    _current_task = TaskError;
}

App::~App() {
    for(int i = 0; i < APP_SCREENS_COUNT; i++) {
        delete screens[i];
    }
}