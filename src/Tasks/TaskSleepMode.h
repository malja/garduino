#ifndef GARDUINO_TASK_SLEEP_MODE_H
#define GARDUINO_TASK_SLEEP_MODE_H

/**
 * This function is periodically called when garduino is in sleep mode.
 * There are two ways how to wake garduino up. First one is that user
 * uses control joystick to send manual wake interruption. Then SleepMode
 * is ended and garduino moves to wake up state. (state: WakedUpByUser; 
 * task: WakeUp).
 * Second one is watchdog interruption. Garduino is waked every 8 seconds
 * to check whether enough time passed already. If so, it is waked up fully
 * (state: WakedUpByTimer; task: WakeUp). If not, it goes back to sleep.
 * 
 * Registers: 
 *  - First     Used for counting elapsed seconds since garduino went asleep.
 *              This register should be cleaned before entering sleep mode. Do
 *              not set this register manually!
 * 
 * States: 
 *  - EnterSleepMode    Instructs this task to enter normal sleep mode.
 *  - Sleeping          Internal state used after sleeping mode initialization.
*/
void TaskSleepMode();

#endif