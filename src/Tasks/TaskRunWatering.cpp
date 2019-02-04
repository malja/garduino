#include "TaskRunWatering.h"
#include "../Interruptions/Interruptions.h"
#include "../App.h"

void TaskRunWateringInit(App::StateID state) {
    // TODO: Send signal to open vent

    // We are initialized now
    APP.setRegister(App::RegisterID::Second, 1);

    // There are zero liters used yet
    APP.setRegister(App::RegisterID::Third, 0);

    // Listen for flowmeter pulses
    AttachFlowmeterInterrupt();
    
    // Enable display when in manual mode
    if (App::StateID::WateringManual == state) {
        APP.display.ssd1306WriteCmd(SSD1306_DISPLAYON);

        APP.display.clear();
        APP.display.print("Watering in progress...");
    }
}

void TaskRunWatering() {

    App::StateID state = APP.getState();
    bool initialized = APP.getRegister(App::RegisterID::Second);
    if (!initialized) {
        TaskRunWateringInit(state);
    }

    // Preserve only one decimal number
    float liters_used = round(((float)APP.getRegister(App::RegisterID::First) / FLOWMETER_PULSES_PER_LITER) * 10) / 10;
    uint32_t max_liters_per_watering = 0;
    APP.storage.read(LITERS_PER_WATERING_INDEX, max_liters_per_watering);

    // Update liters usage only when last value is different from the current one.
    if (App::StateID::WateringManual == state &&
        APP.getRegister(App::RegisterID::Third)/10 < liters_used) {
            APP.display.clear();
            APP.display.print("Watering in progress...");
            APP.display.write(liters_used);
            APP.display.write("/");
            APP.display.print(max_liters_per_watering);

            // Preserve one decimal place
            APP.setRegister(App::RegisterID::Third, round(liters_used * 10));
    }

    // Watering threshold reached
    if (liters_used >= max_liters_per_watering) {

        // TODO: Send signal to close vent
        DetachFlowmeterInterrupt();

        // We should go to sleep
        if (App::StateID::WateringAutomatic == state) {
            APP.setState(App::StateID::EnterSleepMode);
            APP.setNextTask(TaskSleepMode);
        
        // Or return to main menu
        } else {
            APP.display.ssd1306WriteCmd(SSD1306_DISPLAYOFF);

            APP.setState(App::StateID::MenuMain);
            APP.setNextTask(TaskShowMenu);
        }
    }
}