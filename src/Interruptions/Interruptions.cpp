#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#include "Interruptions.h"
#include "../App.h"
#include "../config.h"

/**
 * Interrupt handler for Watchdog.
*/
ISR(WDT_vect) {
    
    // This interruption was expected, since garduino was in sleep mode and was waked up by
    // watchdog.
    if (APP.getState() == App::StateID::Sleeping) {
        OnTimedWakeUpInterruption();
        wdt_reset();

    // Unexpected interruption, need to restart
    } else {
        MCUSR = 0;
        WDTCSR |= 0b00011000;
        WDTCSR =  0b00001000 | 0b000000;

        // Wait for it
        while(1);
    }
}

void OnManualWakeUpInterruption() {
    DetachWakeUpInterruptions();

    APP.setState(App::StateID::WakedUpByUser);
    APP.setNextTask(TaskWakeUp);
}

void OnTimedWakeUpInterruption() {
    // This method is called from ISR, interrupts are already disabled

    uint32_t elapsed_seconds = APP.getRegister(App::RegisterID::First);
    
    // Is it time to wake up?
    if (SLEEP_NUMBER_OF_SECONDS <= elapsed_seconds) {
        DetachWakeUpInterruptions();

        APP.setState(App::StateID::WakedUpByTimer);
        APP.setNextTask(TaskWakeUp);
    } else {
        APP.setRegister(App::RegisterID::First, elapsed_seconds + SLEEP_SECONDS_PER_INTERRUPTION);
        // Keep task and registers, but change app state
        APP.setState(App::StateID::EnterSleepMode, false);
    }

    // TODO: Remove
    digitalWrite(LED_BUILTIN, 1 ^ digitalRead(LED_BUILTIN));
}

void AttachWakeUpInterruptions() {

    // TODO: Remove comments, when de-bouncing is implemented
    // Register manual wake up pin interruption
    //pinMode(INTERRUPTION_PIN_MANUAL_WAKEUP, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(INTERRUPTION_PIN_MANUAL_WAKEUP), OnManualWakeUpInterruption, LOW);

    MCUSR &= ~(1<<WDRF);
  
    // In order to change WDE or the prescaler, we need to
    // set WDCE (This will allow updates for 4 clock cycles).
    WDTCSR |= (1<<WDCE) | (1<<WDE);

    // set new watchdog timeout prescaler value
    WDTCSR = 1<<WDP0 | 1<<WDP3; // 8.0 seconds
  
    // Enable the WD interrupt
    WDTCSR |= _BV(WDIE);
}

void DetachWakeUpInterruptions() {
    // TODO: Remove comments, when de-bouncing is implemented
    //detachInterrupt(digitalPinToInterrupt(INTERRUPTION_PIN_MANUAL_WAKEUP));

    wdt_disable();
}