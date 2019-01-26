#ifndef GARDUINO_TASK_RUN_WATERING_H
#define GARDUINO_TASK_RUN_WATERING_H

/**
 * Registers:
 *  - First     - Contains number of liters used per this watering so far.
 *  - Second    - Is valve already opened or not? Do not fill.
 * 
 * States:
 *  - WateringAutomatic - Start watering and then return to sleep mode
 *  - ManualWatering    - Start watering and return to main menu.
*/
void TaskRunWatering();

#endif