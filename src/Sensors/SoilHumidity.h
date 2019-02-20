#ifndef GARDUINO_SOIL_HUMIDITY_SENSOR_H
#define GARDUINO_SOIL_HUMIDITY_SENSOR_H

#include "../Storage/Storage.h"

/**
 * Enables reading from soil humidity sensor for short period of time. One
 * reading is processed. Based on read value and threshold saved in configuration,
 * determines whether the soil is humid enough.
 * Sensor is then disabled again.
 * 
 * @note Each reading is written into memory slot LAST_HUMIDITY_INDEX.
 * @note Function may enter error state, when memory reading or writing fails.
 * 
 * @see DEFAULT_HUMIDITY_THRESHOLD
*/
bool IsSoilHumid();

#endif
