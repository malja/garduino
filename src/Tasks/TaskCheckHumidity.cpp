#include "TaskCheckHumidity.h"
#include "../App.h"

void TaskCheckHumidity() {
    App::StateID state = APP.getState();
    Display display = APP.display;

    uint32_t humidity_threshold = 0;
    
    if (!APP.storage.read(HUMIDITY_THRESHOLD_INDEX, humidity_threshold)) {
        // Enter error mode to inform reading failed!
        APP.switchToErrorMode(App::ErrorCodeID::StorageReadFailed);
    }

    // Show output only when humidity check was started by user
    if (App::StateID::HumidityCheck == state) {
        display.clear();
        display.println("Measuring Humidity.");
        display.println("Wait please...");
    }

    // Humidity is too low, run watering
    if (analogRead(HUMIDITY_PIN_READ) < humidity_threshold) {

        if (App::StateID::HumidityCheckAndSleep == state) {
            // Make sure that after watering, system will shut down
            APP.setState(App::StateID::WateringAutomatic);
        } else {
            // Return to main menu after watering
            APP.setState(App::StateID::WateringManual);
        }

        APP.setNextTask(TaskRunWatering);
    
    // Humidity is within limits, skip watering
    } else {
        if (App::StateID::HumidityCheckAndSleep == state) {
            // Go back to sleep
            APP.setState(App::StateID::EnterSleepMode);
            APP.setNextTask(TaskSleepMode);
        } else {
            // Go back to main menu
            APP.setState(App::StateID::MenuMain);
            APP.setNextTask(TaskShowMenu);
        }
    }
}