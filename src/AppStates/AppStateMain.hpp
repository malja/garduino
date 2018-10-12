#ifndef APP_STATE_MAIN_HPP
#define APP_STATE_MAIN_HPP

#include "../AppState.hpp"
#include "../Event.hpp"
#include "../EventType.hpp"

class AppStateMain : public AppState {
    void onStart();
    void update(unsigned long long ms);
    void onExit();
};

#endif