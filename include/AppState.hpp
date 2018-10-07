#ifndef APP_STATE_HPP
#define APP_STATE_HPP

#include "AllStates.hpp"

extern class App;

class AppState {

    public:
        void setup(App *app);
        virtual void onStart() = 0;
        virtual void update(unsigned long long ms) = 0;
		virtual void onExit() = 0;
    protected:
        App *_app;
};

#endif