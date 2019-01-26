#include "../App.h"
#include "TaskWakeUp.h"

void TaskWakeUp() {
    App::StateID state = APP.getState();

    if (state == App::StateID::WakedUpByUser) {
        APP.display.ssd1306WriteCmd(SSD1306_DISPLAYON);
        
        APP.setState(App::StateID::MenuMain);
        
        // Required by ShowMenu task
        APP.setRegister(App::RegisterID::First, App::StateID::MenuMain);
        APP.setNextTask(TaskShowMenu);

    } else if (state == App::StateID::WakedUpByTimer) {
        APP.setState(App::StateID::HumidityCheckAndSleep);
        APP.setNextTask(TaskCheckHumidity);
    }

    return;
}