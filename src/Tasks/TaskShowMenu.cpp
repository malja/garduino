#include "TaskShowMenu.h"
#include "../App.h"
#include "../Menu/Screens.h"

void TaskShowMenu() {

    // Handle joystick events
    APP.joystick.update();

    bool render = false;
    App::StateID state = APP.getState();
    // If current state does not match the old one, override rendering process and make sure everithing
    // is rendered properly.

    if ( (int)state != APP.getRegister(App::RegisterID::First)) {
        Serial.print("Different state, re-rendering");
        render = true;
        // Set last menu state
        APP.setRegister(App::RegisterID::First, state);
    }

    switch(state) {
        case App::StateID::MenuMain:
            APP.screens[MenuScreenID::Main]->update(render);
        break;

        case App::StateID::MenuSettings:
            APP.screens[MenuScreenID::Settings]->update(render);
        break;

        case App::StateID::MenuStatistics:
            APP.screens[MenuScreenID::Statistics]->update(render);
        break;
    }

    return;
}