#include "App.hpp"

#include "AppStates/AppStateMain.hpp"
#include "AppStates/AppStateSettings.hpp"
#include "AppStates/AppStateStatistics.hpp"

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

void App::setup() {

    addScreen(MenuScreen::Id::Main, new MenuScreen("Welcome", (MenuItem**){
        new MenuItem("Settings", -1, MenuScreen::Id::Settings),
        new MenuItem("Statistics", -1, MenuScreen::Id::Statistics),
        nullptr
    }));

    addScreen(MenuScreen::Id::Settings, new MenuScreen("Settings", (MenuItem**){
        new MenuItem("Hum. thrsld", RUNTIME_DEFAULT_HUMIDITY_THRESHOLD, MenuScreen::Id::None),
        new MenuItem("l/watering", RUNTIME_DEFAULT_WATERING_LITERS, MenuScreen::Id::None),
        new MenuItem("Back", -1, MenuScreen::Id::Main),
        nullptr
    }));
    addScreen(MenuScreen::Id::Statistics, new MenuScreen("Statistics", (MenuItem **){
        new MenuItem("Ttl liters", -1, MenuScreen::Id::None),
        new MenuItem("Cur. humidity", -1, MenuScreen::Id::None),
        new MenuItem("Back", -1, MenuScreen::Id::Main),
        nullptr
    }));

    // Set current screen
    switchScreen(MenuScreen::Id::Main);

    // Setup connection to joystick
    _joystick.setup(JOYSTICK_PIN_X_AXIS, JOYSTICK_PIN_Y_AXIS, JOYSTICK_PIN_BUTTON, JOYSTICK_UPDATE_INTERVAL);
    
    // Setup display
    display.begin(DISPLAY_TYPE, DISPLAY_I2C_ADDRESS);
    display.setFont(DISPLAY_FONT);
    display.clear();
    
    // Attach interruption for water meter pulse counting
    attachInterrupt(digitalPinToInterrupt(INTERRUPTIONS_PIN_WATER_METER), onWaterMeterPulse, FALLING);
}

void App::run() {

	// Get current time
	unsigned long long ms = millis();

    Serial.println("App>Creating events...");
    
    // Create event pool
    this->createEvents(ms);

    // Search for events which should be handled at application level
    this->handleSystemEvents();

    Serial.println("App>Getting screen...");
	
    // Get current screen or NULL when empty
	MenuScreen *screen = this->_screens[this->_current_screen];

    // Is there any screen set
    if (nullptr != screen) {
        Serial.println("App>Running screen");
        screen->update(ms);
    }
}

bool App::switchScreen(MenuScreen::Id id) {
    
    // Check ID
    int8_t i = (int8_t)id;
    if ( 0 < i || i > APP_SCREENS_COUNT ) {
        return false;
    }

    // Switch state
	_current_screen = i;
}

bool App::addScreen(MenuScreen::Id id, MenuScreen *screen) {
    uint8_t i = (uint8_t)id;

    // Don't allow to register screen with invalid ID
    if (i < 0 || i > APP_SCREENS_COUNT) {
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
    for(uint8_t i = 0; i < APP_SCREENS_COUNT; i++) {
        if (nullptr != _screens[i]) {
            delete _screens[i];
        }
    }
}