#include "App.hpp"

App app;

void setup() {
    app.setState(new WelcomeState());
}

void loop() {
    app.update();
}