#ifndef GARDUINO_TASK_ERROR_H
#define GARDUINO_TASK_ERROR_H

/**
 * Enters pernament (infinite loop) mode which informs outside word that something went horribly wrong
 * by blinking internal led.
 * 
 * Registers:
 *  - First     Used as last time for led switch. When this task is called, first register
 *              should be filled with millis().
 *
 *  - Second    Contains error code. It will be displayed on screen (if that works).
 * 
 *  - Third     Was error code already printed onto display. Do not set this manually!
 * 
 * @see App::switchToErrorMode
*/
void TaskError();

#endif