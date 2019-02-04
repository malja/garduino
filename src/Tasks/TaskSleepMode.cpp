#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#include "../App.h"
#include "TaskSleepMode.h"
#include "../Interruptions/Interruptions.h"

void TaskSleepMode() {

    if (App::StateID::EnterSleepMode == APP.getState()) {
        // Make sure we can wake up by user triggering 
        AttachWakeUpInterrupt();
        Serial.println("Interruptions set");
        delay(100);

        APP.setState(App::StateID::Sleeping, false);
        
        APP.display.ssd1306WriteCmd(SSD1306_DISPLAYOFF);
    }
 
    // Prepare for sleeping
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_enable();
    sleep_mode(); // From now on, arduino is in sleep mode

    // Execution continues from this point
    sleep_disable();

    // Transition to next task and app state is done automatically in 
    // interruption handler.
    // See OnTimedWakeUpInterruption and OnManualWakeUpInterruption
    // functions for further documentation.
}