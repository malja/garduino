#ifndef APP_STATE_STATISTICS
#define APP_STATE_STATISTICS

#include "../AppState.hpp"

class AppStateStatistics : public AppState {
    void onStart();
    void update(unsigned long long ms);
    void onExit();
};

#endif