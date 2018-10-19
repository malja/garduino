#ifndef APP_STATE_SETTINGS
#define APP_STATE_SETTINGS

#include "AppState.hpp"
#include "../Events/Event.hpp"

class AppStateSettings : public AppState {
    void onStart();
    void update(unsigned long long ms);
    void onExit();
};

#endif