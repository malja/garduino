#ifndef WELCOME_APP_STATE_HPP
#define WELCOME_APP_STATE_HPP

#include "AppState.hpp"
#include "EventType.hpp"

class WelcomeAppState : public AppState {
    void onStart();
    void update(unsigned long long ms);
};

#endif