#ifndef GARDUINO_TASK_CALIBRATE_SENSORS_H
#define GARDUINO_TASK_CALIBRATE_SENSORS_H

/**
 * States:
 *  - CalibrateHumidityLow  - 
 *  - CalibrateHumidityHigh - 
 * 
 * Registers:
 *  - First     - Last measured low humidity level. Set it to zero before execution.
 *  - Second    - Last measured high humidity level. Set it to zero before execution.
 *  - Third     - Was finish prompt printed out? Set to zero before execution.
*/
void TaskCalibrateSensors();

#endif
