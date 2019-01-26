#include "TaskRunWatering.h"
#include "../App.h"

void TaskRunWatering() {

    App::StateID state = APP.getState();

    bool vent_opened = APP.getRegister(App::RegisterID::Second);
    if (!vent_opened) {
        // TODO: Send signal to open vent
        APP.display.clear();
        APP.display.print("Watering in progress...");
    }

    uint32_t liters_used = APP.getRegister(App::RegisterID::First);
    
    uint32_t max_liters_per_watering = 0;
    APP.storage.read(LITERS_PER_WATERING_INDEX, max_liters_per_watering);

    APP.display.write(liters_used);
    APP.display.write("/");
    APP.display.write(max_liters_per_watering);

    // Watering threshold reached
    if (liters_used >= max_liters_per_watering) {

        // TODO: Send signal to close vent
        // TODO: Reset watering counter

        // We should go to sleep
        if (App::StateID::WateringAutomatic == state) {
            APP.setState(App::StateID::EnterSleepMode);
            APP.setNextTask(TaskSleepMode);
        
        // Or return to main menu
        } else {
            APP.setState(App::StateID::MenuMain);
            APP.setNextTask(TaskShowMenu);
        }
    }
}