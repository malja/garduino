#ifndef GARDUINO_TASK_WAKE_UP_H
#define GARDUINO_TASK_WAKE_UP_H

/**
 * Transition task which runs immedeatelly after garduino is waked up either
 * by user or watchdog interruption. It decides which action should be run
 * next based on wake up event. 
 * 
 * States:
 *  - WakedUpByUser     Show main menu (state: MenuMain; task: ShowMenu) 
 *                      as next step.
 *  - WakedUpByTimer    Perform automatic humidity check (state: HumidityCheckAndSleep; 
 *                      task: HumidityCheck) as next step.
*/
void TaskWakeUp();

#endif