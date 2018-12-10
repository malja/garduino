#include "MenuScreen.hpp"
#include "../App.hpp"

MenuScreen::MenuScreen(String title, uint8_t numOfItems, ... ) {
    
    _title = title;
    va_list items;

    _items = new MenuItem *[numOfItems];

    va_start(items, numOfItems);
    
    // Count number of items in array
    uint8_t count = 0;
    for (; count < numOfItems; count++) {
        _items[count] = va_arg(items, MenuItem *);
    }

    va_end(items);

    _num_of_items = count;
    _selected_item = 0;
    _edit_mode = false;
    _changed_since_last_render = true;
}

void MenuScreen::setup(App *app) {
    _app = app;
}

void MenuScreen::enterEditMode() {
    _edit_mode = true;
    _changed_since_last_render = true;
}

bool MenuScreen::selectItem(uint8_t index) {
    if (index >= _num_of_items) {
        return false;
    }

    _selected_item = index;
    _changed_since_last_render = true;
    return true;
}

void MenuScreen::onEnter() {
    _changed_since_last_render = true;
}

MenuItem *MenuScreen::getSelectedItem() {
    return _items[_selected_item];
}

App *MenuScreen::getApp() {
    return _app;
}

void MenuScreen::update(time_ms ms) {
    handleEvents();
    render();
}

void MenuScreen::render() {

    if (!_changed_since_last_render) {
        return;
    }

    _app->display.clear();
    _changed_since_last_render = false;

    if (_edit_mode) {
        // If somehow edit mode is executed for item without value
        if (-1 == _items[_selected_item]->getValueIndex()) {
            _edit_mode = false;
            return;
        }

        // | MENU ITEM TEXT   |
        // |                  |
        // | < VALUE        > |

        _app->display.clear();
        _app->display.set2X();
        _app->display.write( _items[_selected_item]->getText().c_str() );
        
        _app->display.set1X();
        _app->display.setCursor(0, 3);
        _app->display.write("< ");

        _app->display.print( _items[_selected_item]->getValue() );
        
        _app->display.setCursor(
            _app->display.displayWidth() - _app->display.fontWidth(), 
            3
        );
        _app->display.write(">");

        return;
    }

    // Render list of items
    uint8_t first_index = _selected_item - 1 <= 0 ? 0 : _selected_item - 1;
    uint8_t last_index = min(first_index + DISPLAY_NUM_LINES - 1, _num_of_items-1);

    uint8_t line = 1;
    for (uint8_t i = first_index; i <= last_index; i++) {

        // > MENU ITEM TEXT: VALUE
        if (_selected_item == i) {
            _app->display.setCursor(0, line);
            _app->display.write(">");
        }

        _app->display.setCursor(_app->display.fontWidth() * 3, line);
        _app->display.print(_items[i]->getText().c_str());

        // Is there a value?
        if (-1 != _items[i]->getValueIndex()) {
            // Update it
            uint32_t value = 0;
            _app->storage.read((uint8_t)_items[i]->getValueIndex(), value); // Ignoring return value
            _items[i]->setValue(value);

            // And print it
            _app->display.print(": ");
            _app->display.print(_items[i]->getValue());
        }

        line++;
    }

}

void MenuScreen::handleEvents() {
    Event ev;
    while(_app->pollEvent(ev)) {

        // Check joystick events
        if (EventType::Joystick == ev.type) {
            
            // If in edit mode
            if (_edit_mode) { 

                MenuItem *item = (MenuItem*)_items[_selected_item];

                // Direction changes will increment or decrement the value   
                if (EventTypeJoystick::Direction == ev.joystick.type) {

                    if (EventJoystickMoveDirection::Left == ev.joystick.direction) {    
                        item->setValue(item->getValue() - 1);
                    } else if (EventJoystickMoveDirection::Right == ev.joystick.direction) {
                        item->setValue(item->getValue() + 1);
                    }

                // Click ends edit mode
                } else if (EventTypeJoystick::Click == ev.joystick.type) {
                    if (!_app->storage.update(item->getValueIndex(), item->getValue())) {
                        // TODO: Handle error 
                        Serial.println("MenuScreen>handleEvents: Failed to update storage value");
                    }
                    _edit_mode = false;
                }

                _changed_since_last_render = true;

            // In standard (non-edit) mode
            } else {

                // Direction changes scroll down/up the list
                if (EventTypeJoystick::Direction == ev.joystick.type) {

                    if (EventJoystickMoveDirection::Up == ev.joystick.direction) {    
                        _selected_item = (_selected_item - 1) < 0 ? (_num_of_items - 1) : (_selected_item - 1);
                        _changed_since_last_render = true;
                    } else if (EventJoystickMoveDirection::Down == ev.joystick.direction) {
                        _selected_item = (_selected_item + 1) == _num_of_items ? 0 : (_selected_item + 1);
                        _changed_since_last_render = true;
                    }

                // Click enters edit mode or follows the link
                } else if (EventTypeJoystick::Click == ev.joystick.type) {
                    if (_items[_selected_item]->getScreenId() >= 0) {
                        _app->switchScreen((MenuScreen::Id)_items[_selected_item]->getScreenId());
                        _changed_since_last_render = true;
                    } else if (_items[_selected_item]->getValueIndex() >= 0) {
                        enterEditMode();
                    }
                }
            }
        }
    }
}

MenuScreen::~MenuScreen() {
    for(uint8_t i = 0; i < _num_of_items; i++) {
        if (nullptr != _items[i]) {
            delete _items[i];
        }
    }
    delete _items;
}
