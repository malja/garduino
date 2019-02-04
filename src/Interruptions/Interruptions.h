#ifndef GARDUINO_INTERRUPTIONS_H
#define GARDUINO_INTERRUPTIONS_H

#include <Arduino.h>

/**
 * This function is run every time timer rises an interruption. Number of elapsed
 * seconds is compared to config value. If more or equal number of seconds elapsed,
 * garduino is waked up (state: WakedUpByTimer; task: TaskWakeUp) and all
 * interruptions for waking garduino up are detached.
 * 
 * Otherwise seconds counter (first register) is increased and garduino goes back 
 * to sleep (state: EnterSleepMode; task: TaskSleepMode).
 * 
 * @note Interruptions are disabled during for runtime of this function.
 * @see AttachWakeUpInterrupt
 * @see DetachWakeUpInterrupt
*/
void OnTimedWakeUpInterrupt();

/**
 * Handler for interruption generated by user pressing control joystick. There is no additional
 * check before waking up (state: WakedUpByUser; task: TaskWakeUp). All interruptions for
 * waking garduino up are detached.
 * 
 * @note Interruptions are disabled during for runtime of this function.
 * @see AttachWakeUpInterrupt
 * @see DetachWakeUpInterrupt
*/
void OnManualWakeUpInterrupt();

/**
 * Attach timed and manual interruptions for waking garduino up.
 * 
 * @note Sleep mode is not entered. It has to be done manually from caller.
*/
void AttachWakeUpInterrupt();

/**
 * Detach timed and manual interruptions for waking garduino up.
*/
void DetachWakeUpInterrupt();

/**
 * This is callback for AttachFlowmeterInterrupt. It is called for each pulse generated by flowmeter. 
 * It increases First register value by one. Value should be read from TaskRunWatering.
 * 
 * @see AttachFlowmeterInterrupt
 * @see DetachFlowmeterInterrupt
*/
void OnFlowmeterInterrupt();

/**
 * Register interruption on INTERRUPT_PIN_FLOWMETER when watering is started. This calls OnFlowmeterInterrupt
 * for each pulse comming from flowmeter.
 * 
 * @see DetachFlowmeterInterrupt
 * @see OnFlowmeterInterrupt
*/
void AttachFlowmeterInterrupt();

/**
 * Remove Interruptions registered on INTERRUPT_PIN_FLOWMETER. This should be called after watering process is
 * ended.
 * 
 * @see OnFlowmeterInterrupt
 * @see AttachFlowmeterInterrupt
*/
void DetachFlowmeterInterrupt();

#endif
