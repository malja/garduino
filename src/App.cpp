#include "App.hpp"

App::App() {
    
	// Fill whole _states array with NULL. This helps to check against empty cells in update method.
	for (uint8_t i = 0; i < AppStateIDs::__MAX__; i++) {
		this->_states[i] = nullptr;
	}

	this->_current_state = 0;

	// Fill _states array with registered app states pointers
	this->_states[AppStateIDs::MAIN] = &app_state_main;
	this->_states[AppStateIDs::SETTINGS] = &app_state_settings;
	this->_states[AppStateIDs::STATISTICS] = &app_state_statistics;

    // Connect to joystick
    this->_joystick = PSJoystick();
    this->_joystick.setup(JOYSTICK_PIN_X_AXIS, JOYSTICK_PIN_Y_AXIS, JOYSTICK_PIN_BUTTON, JOYSTICK_UPDATE_INTERVAL);
    
    // Setup EEPROM storage
    if (DEBUG) {
        this->storage.enableProtection();
    }
}

void App::update() {

	// Get current time
	unsigned long long ms = millis();

    // Create event pool
    this->createEvents(ms);

	// Get current state or NULL when empty
	AppState *state = this->_states[this->_current_state];

    // Is there any application state
    if (nullptr != state) {
        // Run app state update
        state->update(ms);
    }
}

void App::setState(AppStateIDs id) {

	// Get current state
	AppState *state = this->_states[this->_current_state];

    // Make sure to clear old state
    if (nullptr != state) {
        state->onExit();
    }

    // Run new state
	this->_current_state = id;
	state = this->_states[this->_current_state];

	state->setup(this);
    state->onStart();
}

void App::createEvents(unsigned long long ms) {

	// When event is marked with EventType::None, it means it does not hold any data
	// This event will not be returned from polling.
	for (uint8_t i = 0; i < EVENTS_POOL_SIZE; i++) {
		this->_events[i].type = EventType::None;
	}

    handleJoystick(ms);

}

bool App::pollEvent(const Event &event) {
 
	for(uint8_t i = 0; i < EVENTS_POOL_SIZE; i++) {
        if (EventType::None != this->_events[i].type) {
            // TODO: Copy event union
            return true;
        }
    }
    return false;
}

Event* App::getFreeEventFromPool() {
    // Prepare event object
    if (EVENTS_POOL_SIZE == this->_events_count) {
        this->_events_count = 0;
    }

    return &(this->_events[this->_events_count++]);
}

void App::handleJoystick(unsigned long long ms) {

    // Read joystick values
    this->_joystick.update(ms);
    
    // Read X and Y axis of joystick
    int8_t x_axis = this->_joystick.getX();
    int8_t y_axis = this->_joystick.getY();

    // Joystick movement
    if (0 != x_axis || 0 != y_axis) {

        // Get pointer to one of events in pool
        Event *ev = this->getFreeEventFromPool();
        
        // Set event type to joystick
        ev->type = EventType::Joystick;

        // Fill joystick event
        ev->joystick.type = EventJoystickType::MOVE;
        ev->joystick.pressed = this->_joystick.isPressed();
        ev->joystick.x_axis = x_axis;
        ev->joystick.y_axis = y_axis;
    }

    if (this->_joystick.isPressed()) {

        // Get pointer to one of events in pool
        Event *ev = this->getFreeEventFromPool();
        
        // Set event type to joystick
        ev->type = EventType::Joystick;

        // Fill joystick event
        ev->joystick.type = EventTypeJoystick::BUTTON;
        ev->joystick.pressed = true;
        ev->joystick.x_axis = x_axis;
        ev->joystick.y_axis = y_axis;

    }
}