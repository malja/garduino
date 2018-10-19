#include "MenuScreen.hpp"

MenuScreen::MenuScreen(const char *title, MenuItem *items[] ) {
    
    // Copy title
    snprintf(_title, MENU_HEADER_LENGTH, "%s", title);
    
    u8 count = 0;
    for (; items[count]; count++);
    _items = items;

    _num_of_items = count;
    _selected_item = 0;
    _edit_mode = false;
}

void MenuScreen::setup(App *app) {
    _app = app;
}

bool MenuScreen::selectItem(u8 index) {
    if (index >= _num_of_items) {
        return false;
    }

    _selected_item = index;
    return true;
}

MenuItem *MenuScreen::getSelectedItem() {
    return _items[_selected_item];
}

void MenuScreen::update(time_ms ms) {
    handleEvents();
    render();
}

void MenuScreen::render() {

    if (_edit_mode) {

        // | MENU ITEM TEXT   |
        // |                  |
        // | < VALUE        > |

        _app->display.clear();
        _app->display.set2X();
        _app->display.write( _items[_selected_item]->text );
        
        _app->display.set1X();
        _app->display.setCursor(0, 3);
        _app->display.write("< ");

        _app->display.write(_items[_selected_item]->value);
        
        _app->display.setCursor(
            _app->display.displayWidth() - _app->display.fontWidth(), 
            3
        );
        _app->display.write(">");

        return;
    }

    // Render list of items

    u8 first_index = 0;
    u8 last_index = DISPLAY_NUM_LINES;

    // Which page are we on. Each page consist of DISPLAY_NUM_LINES lines.
    float page = (_selected_item+1)/DISPLAY_NUM_LINES;

    // We are not on the first page
    if ( page > 1) {
        first_index = (int) page * DISPLAY_NUM_CHARACTERS;
        last_index = min( first_index + DISPLAY_NUM_CHARACTERS, _num_of_items );
    }

    u8 line = 1;
    for (u8 i = first_index; i < last_index; i++) {
        
        // > MENU ITEM TEXT: VALUE
        if (_selected_item == i) {
            _app->display.setCursor(0, line);
            _app->display.write(">");
        }

        _app->display.setCursor(_app->display.fontWidth() * 3, line);
        _app->display.print(_items[i]->text);
        _app->display.print(": ");
        _app->display.print(_items[i]->value);

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

                // Direction changes will increment or decrement the value   
                if (EventTypeJoystick::Direction == ev.joystick.type) {
                
                    MenuItem *item = (MenuItem*)_items[_selected_item];

                    if (EventJoystickMoveDirection::Left == ev.joystick.direction) {    
                        item->value -= 1;
                    } else if (EventJoystickMoveDirection::Right == ev.joystick.direction) {
                        item->value += 1;
                    }

                // Click ends edit mode
                } else if (EventTypeJoystick::Click == ev.joystick.type) {
                    _edit_mode = false;
                }

            // In standard (non-edit) mode
            } else {

                // Direction changes scroll down/up the list
                if (EventTypeJoystick::Direction == ev.joystick.type) {

                    if (EventJoystickMoveDirection::Up == ev.joystick.direction) {    
                        _selected_item -= 1;
                    } else if (EventJoystickMoveDirection::Right == ev.joystick.direction) {
                        _selected_item += 1;
                    }

                    if (_selected_item >= _num_of_items) {
                        _selected_item = 0;
                    } else if (_selected_item < 0) {
                        _selected_item = _num_of_items-1;
                    }

                // Click enters edit mode or follows the link
                } else if (EventTypeJoystick::Click == ev.joystick.type) {
                    MenuItem *item = (MenuItem*)_items[_selected_item];
                    if (item->link) {
                        _app->changeScreen(item->link);
                    } else {
                        _edit_mode;
                    }
                }
            }
        }
    }
}

MenuScreen::~MenuScreen() {
    for(u8 i = 0; i < _num_of_items; i++) {
        if (nullptr != _items[i]) {
            delete _items[i];
        }
    }
}