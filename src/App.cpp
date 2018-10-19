#include "App.hpp"

#include "AppStates/AppStateMain.hpp"
#include "AppStates/AppStateSettings.hpp"
#include "AppStates/AppStateStatistics.hpp"

App::App() {

	// Fill whole _states array with NULL. This helps to check against empty cells in update method.
	for (uint8_t i = 0; i < APP_STATES_COUNT; i++) {
		_states[i] = nullptr;
	}

    // No current state
	_current_state = -1;

    // Setup EEPROM storage
    if (DEBUG) {
        //this->storage.enableProtection();
    }

    // Fill event pool with empty events
    for(uint8_t i = 0; i < EVENTS_POOL_SIZE; i++) {
        _events[i] = Event();
    }
}

void App::setup() {

    // Register App states
    addState(AppStateIDs::Main, new AppStateMain);
    addState(AppStateIDs::Settings, new AppStateSettings);
    addState(AppStateIDs::Statistics, new AppStateStatistics);

    // Run Main state as first
    switchState(AppStateIDs::Main);

    _joystick.setup(JOYSTICK_PIN_X_AXIS, JOYSTICK_PIN_Y_AXIS, JOYSTICK_PIN_BUTTON, JOYSTICK_UPDATE_INTERVAL);
}

void App::run() {

	// Get current time
	unsigned long long ms = millis();

    Serial.println("App>Creating events...");
    // Create event pool
    this->createEvents(ms);

    Serial.println("App>Getting AppState...");
	// Get current state or NULL when empty
	AppState *state = this->_states[this->_current_state];

    // Is there any application state
    if (nullptr != state) {
        // Run app state update
        Serial.println("App>Running AppState");
        state->update(ms);
    }
}

bool App::switchState(AppStateIDs id) {

    AppState *state = nullptr;
    
    // Check ID
    int8_t i = (int8_t)id;
    if ( 0 < i || i > APP_STATES_COUNT ) {
        return false;
    }

	// If current state is set
    if ( -1 != _current_state) {
	    state = _states[_current_state];
       
        // Make sure to clear old state
        if (nullptr != state) {
            state->onExit();
        }
    }

    // Switch state
	_current_state = i;
	state = _states[_current_state];

    // Prepare state for running
    state->onStart();
}

bool App::addState(AppStateIDs id, AppState *state) {
    uint8_t i = (uint8_t)id;

    // Don't allow to register app state with invalid ID
    if (i < 0 || i > APP_STATES_COUNT) {
        return false;
    }

    // No point in adding null
    if (nullptr == state) {
        return false;
    }

    // Make sure no memory is leaked
    if (nullptr != _states[i]) {
        delete _states[i];
    }

    // Add new state
    _states[i] = state;
    _states[i]->setup(this); 
}

void App::createEvents(unsigned long long ms) {

	// When event is marked with EventType::None, it means it does not hold any data
	// This event will not be returned from polling.
	for (uint8_t i = 0; i < EVENTS_POOL_SIZE; i++) {
		_events[i].type = EventType::None;
	}

    handleJoystickEvents(ms);

}

bool App::pollEvent(Event &event) {
 
	for(uint8_t i = 0; i < EVENTS_POOL_SIZE; i++) {

        // Find valid event
        if (EventType::None != _events[i].type) {

            // Fill event structure with data
            event = _events[i];

            // Erase event in queue, so it is not used in next pollEvent call
            _events[i].type = EventType::None;
            return true;
        }
    }

    // No event found
    return false;
}

void App::addEvent(const Event &event) {
    Event *ev = this->getFreeEventFromPool();
    *ev = event;
}

Event* App::getFreeEventFromPool() {
    // Prepare event object
    if (EVENTS_POOL_SIZE == this->_events_count) {
        this->_events_count = 0;
    }

    return &(this->_events[this->_events_count++]);
}

void App::handleJoystickEvents(unsigned long long ms) {

    Serial.println("App>Handling joystick.");

    // Update JOystick
    _joystick.update(ms);
    
    EventJoystickMoveDirection direction = _joystick.getDirection();

    Serial.print("App>Current direction ");
    Serial.println((int)direction);
    
    // Handle direction event
    if (EventJoystickMoveDirection::None != direction) {
        Event *ev = this->getFreeEventFromPool();

        ev->type = EventType::Joystick;
        ev->joystick.type = EventTypeJoystick::Direction;
        ev->joystick.direction = direction;
        ev->joystick.x_axis = _joystick.getX();
        ev->joystick.y_axis = _joystick.getY();
    }
}

App::~App() {
    // Free memory
    for(uint8_t i = 0; i < APP_STATES_COUNT; i++) {
        if (nullptr != _states[i]) {
            delete _states[i];
        }
    }
}