#include "App.hpp"

App::App() {
    this->_state = nullptr;
    this->_last_time = millis();

    // Connect to joystick
    this->_joystick = PSJoystick();
    this->_joystick.setup(JOYSTICK_PIN_X_AXIS, JOYSTICK_PIN_Y_AXIS, JOYSTICK_PIN_BUTTON, JOYSTICK_UPDATE_INTERVAL);
    
    // Setup EEPROM storage
    if (DEBUG) {
        this->storage.enableProtection();
    }
}

void App::update() {

    // Calculate delta time
    unsigned long long ms = millis();

    // Create event pool
    this->createEvents(ms);

    // Is there any application state
    if (nullptr != this->_state) {
        // Run app state update
        this->_state->update(dt);
    }
}

void App::setState(AppState *state) {

    // Make sure to clear old state
    if (this->_state != nullptr) {
        this->_state->onExit();
        delete this->_state;
    }

    // Run new state
    this->_state = state;
    this->_state->setup(this);
    this->_state->onStart();
}

void App::createEvents(unsigned long long ms) {

    // Clear old events
    this->_events.clear()

    handleJoystick(ms);

}

void App::handleJoystick(unsigned long long ms) {

    // Read joystick values
    this->_joystick.update(ms);

    // Prepare event object
    Event ev;
    
    // Set event type to joystick
    ev.type = EventType::Joystick;
    
    // Read X and Y axis of joystick
    int8_t x_axis = this->_joystick.getX();
    int8_t y_axis = this->_joystick.getY();

    // Non-default position on X axis
    if (0 != x_axis) {

        // Fill joystick event
        ev.joystick.type = x_axis < 0 ? JoystickEventType::MOVE_LEFT : JoystickEventType::MOVE_RIGHT;
        ev.joystick.pressed = this->_joystick.isPressed();
        ev.joystick.x_axis = x_axis;
        ev.joystick.y_axis = y_axis;

        this->_events.push(ev);
    }

    // Non-default position on Y axis
    if (0 != y_axis) {

        // Fill joystick event
        ev.joystick.type = y_axis < 0 ? JoystickEventType::MOVE_UP : JoystickEventType::MOVE_DOWN;
        ev.joystick.pressed = this->_joystick.isPressed();
        ev.joystick.x_axis = x_axis;
        ev.joystick.y_axis = y_axis;

        this->_events.push(ev);
    }

    if (this->_joystick->isPressed()) {

        // Fill joystick event
        ev.joystick.type = JoystickEventType::BUTTON;
        ev.joystick.pressed = true;
        ev.joystick.x_axis = x_axis;
        ev.joystick.y_axis = y_axis;

        this->_events.push(ev);
    }
}