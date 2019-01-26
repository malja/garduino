#ifndef GARDUINO_TASK_CHECK_HUMIDITY_H
#define GARDUINO_TASK_CHECK_HUMIDITY_H

/**
 * Task responsible for checking soil humidity and either running watering
 * if humidity is below set threshold or return back to menu/sleeping.
 * 
 * States:
 *  - HumidityCheckAndSleep When humidity is below threshold, state WateringAutomatic
 *                          is entered. Otherwise returns back to sleeping with
 *                          EnterSleepMode state.
 * 
 *  - HumidityCheck         Perform humidity check and return to MenuMain state (humidity 
 *                          levels are within limits) or run manual watering state
 *                          WateringManual.
*/
void TaskCheckHumidity();

#endif