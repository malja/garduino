#include "App.hpp"

App::App() {

	// Fill whole _screens array with NULL. This helps to check against empty cells in update method.
	for (uint8_t i = 0; i < APP_SCREENS_COUNT; i++) {
		_screens[i] = nullptr;
	}

    // No current screen
	_current_screen = -1;

    // Setup EEPROM storage
    if (DEBUG) {
        //this->storage.enableProtection();
    }

    // Fill event pool with empty events
    for(uint8_t i = 0; i < EVENTS_POOL_SIZE; i++) {
        _events[i] = Event();
    }
}

bool App::setup() {

    // Connect to EEPROM
    if (!storage.setup(STORAGE_OFFSET_IN_BYTES, STORAGE_SIZE_IN_BYTES)) {
        Serial.println("App>Setup: Storage setup failed.");
        return false;
    }

    Serial.println("App>Setup: Storage setup is done");

    // If there are no data, write defaults
    if (storage.isEmpty()) {
        Serial.println("App>Setup: Storage is empty.");
        if (
            !storage.write(HUMIDITY_THRESHOLD_INDEX, DEFAULT_HUMIDITY_THRESHOLD) ||
            !storage.write(LITERS_PER_WATERING_INDEX, DEFAULT_LITERS_PER_WATERING) ||
            !storage.write(HUMIDITY_CHECK_INTERVAL_INDEX, DEFAULT_HUMIDITY_CHECK_INTERVAL) ||
            !storage.write(LITERS_TOTAL_INDEX, DEFAULT_LITERS_TOTAL) ||
            !storage.write(LAST_HUMIDITY_INDEX, DEFAULT_LAST_HUMIDITY)
        ){
            Serial.println("App>Setup: Unable to write defaults.");
            return false;
        }
        Serial.println("App>Setup: Defaults written.");
        storage.print();
    }

    addScreen(MenuScreen::Id::Main, new MenuScreen("Welcome", 2, 
            new MenuItem("Settings", -1, MenuScreen::Id::Settings),
            new MenuItem("Statistics", -1, MenuScreen::Id::Statistics)
        )
    );

    addScreen(MenuScreen::Id::Settings, new MenuScreen("Settings", 4,
            new MenuItem("Hum. thrsld", HUMIDITY_THRESHOLD_INDEX, -1),
            new MenuItem("l/watering", LITERS_PER_WATERING_INDEX, -1),
            new MenuItem("Chck interval", HUMIDITY_CHECK_INTERVAL_INDEX, -1),
            new MenuItem("Back", -1, MenuScreen::Id::Main)
        )
    );
    
    addScreen(MenuScreen::Id::Statistics, new MenuScreen("Statistics", 3,
            new MenuItem("Total liters", LITERS_TOTAL_INDEX, -1),
            new MenuItem("Cur. humidity", LAST_HUMIDITY_INDEX, -1),
            new MenuItem("Back", -1, MenuScreen::Id::Main)
        )
    );

    // Set current screen
    switchScreen(MenuScreen::Id::Main);

    // Setup connection to joystick
    _joystick.setup(JOYSTICK_PIN_X_AXIS, JOYSTICK_PIN_Y_AXIS, JOYSTICK_PIN_BUTTON, JOYSTICK_UPDATE_INTERVAL);
    
    // Setup display
    display.begin(DISPLAY_TYPE, DISPLAY_I2C_ADDRESS);
    display.setFont(DISPLAY_FONT);
    display.clear();
    
    // Attach interruption for water meter pulse counting
    //attachInterrupt(digitalPinToInterrupt(INTERRUPTIONS_PIN_WATER_METER), onWaterMeterPulse, FALLING);
    return true;
}

void App::run() {

	// Get current time
	time_ms ms = millis();
    
    // Create event pool
    this->createEvents(ms);

    // Search for events which should be handled at application level
    //this->handleSystemEvents();

    // Get current screen or NULL when empty
	MenuScreen *screen = this->_screens[this->_current_screen];

    // Is there any screen set
    if (nullptr != screen) {
        screen->update(ms);
    }
}

bool App::switchScreen(MenuScreen::Id id) {
    
    // Check ID
    uint8_t i = (uint8_t)id;

    if ( 0 > i || i > APP_SCREENS_COUNT - 1 ) {
        return false;
    }

    // Switch state
	_current_screen = i;
    _screens[_current_screen]->onEnter();
    return true;
}

bool App::addScreen(MenuScreen::Id id, MenuScreen *screen) {
    uint8_t i = (uint8_t)id;

    // Don't allow to register screen with invalid ID
    if (i < 0 || i > APP_SCREENS_COUNT - 1) {
        return false;
    }

    // Add new screen
    screen->setup(this);
    _screens[i] = screen;
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


    // Update JOystick
    _joystick.update(ms);
    
    EventJoystickMoveDirection direction = _joystick.getDirection();
    
    // Handle direction event
    if (EventJoystickMoveDirection::None != direction) {
        Event *ev = this->getFreeEventFromPool();

        ev->type = EventType::Joystick;
        ev->joystick.type = EventTypeJoystick::Direction;
        ev->joystick.direction = direction;
        ev->joystick.x_axis = _joystick.getX();
        ev->joystick.y_axis = _joystick.getY();
    }

    // Handle click events
    if (_joystick.isClicked()) {

        Event *ev = this->getFreeEventFromPool();

        ev->type = EventType::Joystick;
        ev->joystick.type = EventTypeJoystick::Click;
        ev->joystick.direction = direction;
        ev->joystick.x_axis = _joystick.getX();
        ev->joystick.y_axis = _joystick.getY();
    }
}

App::~App() {
    // Free memory
    for(uint8_t i = 0; i < APP_SCREENS_COUNT; i++) {
        if (nullptr != _screens[i]) {
            delete _screens[i];
        }
    }
}