#ifndef WATERING_STATS_HPP
#define WATERING_STATS_HPP

#include <Arduino.h>
#include "../../Helpers/typedefs.hpp"
#include "../../Events/Event.hpp"

class WateringStats {

        void start();
        void stop();

        void update(time_ms ms);
        void pulse();

    protected:

        bool _running;
        time_ms _last_time; ///< Last time of liters calculation
        uint8_t  _pulses;   ///< Counter for pulses per last two seconds.
};

#endif