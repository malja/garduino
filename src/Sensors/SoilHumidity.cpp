#include "SoilHumidity.h"
#include "../App.h"
#include "../config.h"

bool IsSoilHumid() {
    // enable soil humidity sensor 
    pinMode(SOIL_HUMIDITY_SENSOR_POWER_PIN, OUTPUT);

    uint32_t humidity_threshold = 0;
    
    if (!APP.storage.read(HUMIDITY_THRESHOLD_INDEX, humidity_threshold)) {
        // Enter error mode to inform reading failed!
        APP.switchToErrorMode(App::ErrorCodeID::StorageReadFailed);
    }

    // For dry soil, value is something around 20
    // For sensor fully submerged into water, values are around 700
    uint16_t current_humidity = 1023 - analogRead(SOIL_HUMIDITY_SENSOR_READING_PIN);
    
    // Save last readings to memory
    if (!APP.storage.write(LAST_HUMIDITY_INDEX, current_humidity)) {
        // Enter error mode to inform reading failed!
        APP.switchToErrorMode(App::ErrorCodeID::StorageWriteFailed);
    }

    // disable soil humidity sensor
    pinMode(SOIL_HUMIDITY_SENSOR_POWER_PIN, INPUT);

    return current_humidity >= humidity_threshold;
}
