#include "interruptions.hpp"

void onWaterMeterPulse() {
    Event ev;
    ev.type = EventType::WaterMeterPulse;

    App::getInstance().addEvent(ev);
}
