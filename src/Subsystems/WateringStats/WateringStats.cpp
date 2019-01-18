#include "WateringStats.hpp"
#include "../../App.hpp"

void WateringStats::start() {
    _running = true;
    _last_time = millis();
    _total = 0;
}

void WateringStats::stop() {
    _running = false;
}

void WateringStats::pulse() {
    _pulses += 1;
}

void WateringStats::update(time_ms ms) {
    
    // Do not update when watering is not running
    if (!_running) return;

    // Run once every two seconds (or anything higher that two)
    float elapsed = (ms - _last_time)/1000;
    if (elapsed < 2) return;

    uint32_t total_liters = 0;
    App::getInstance().storage.read(LITERS_TOTAL_INDEX, &total_liters);

    total_liters += elapsed * (_pulses / elapsed * 4.5);
    Serial.print("New total liters: ");
    Serial.println(total_liters);

    App::getInstance().storage.update(LITERS_TOTAL_INDEX, total_liters);
}