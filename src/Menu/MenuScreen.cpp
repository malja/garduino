#include "MenuScreen.h"
#include "../App.h"

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
    _multiplier = 0;
}

void MenuScreen::enterEditMode() {
    _edit_mode = true;
    _changed_since_last_render = true;
    _multiplier = 0;
}

bool MenuScreen::selectItem(uint8_t index) {
    if (index >= _num_of_items) {
        return false;
    }

    _selected_item = index;
    
    _changed_since_last_render = true;
    return true;
}

MenuItem *MenuScreen::getSelectedItem() {
    return _items[_selected_item];
}

void MenuScreen::update(bool renderOverride) {
    handleEvents();
    render(renderOverride);
}

void MenuScreen::render(bool renderOverride) {

    if (false == _changed_since_last_render && false == renderOverride) {
        return;
    }

    Display display = APP.display;
    
    display.clear();
    
    _changed_since_last_render = false;

    if (_edit_mode) {
        // If somehow edit mode is executed for item without value
        if (-1 == _items[_selected_item]->getValueIndex()) {
            _edit_mode = false;
            return;
        }

        // | MENU ITEM TEXT |
        // |                |
        // |          Value |
        // |              ^ |

        display.clear();

        // Value name
        display.write( _items[_selected_item]->getText().c_str() );
        
        // Value itself, aligned to the right
        display.setCursor(0, 2);
        display.write("=>");
        display.setCursor(
            // Number of pixels      Pixels per character   Number of characters in value                   
            display.displayWidth() - display.fontWidth() * (_items[_selected_item]->getValueLengthInChars() + 1),
            2
        );
        display.print(_items[_selected_item]->getValue());

        // Multiplier
        display.setCursor( display.displayWidth() - display.fontWidth() * (_multiplier + 2) , 3);
        display.write("^");

        return;
    }

    // Render list of items
    uint8_t first_index = _selected_item - 1 <= 0 ? 0 : _selected_item - 1;
    uint8_t last_index = min(first_index + DISPLAY_NUM_LINES - 1, _num_of_items-1);

    uint8_t line = 1;
    for (uint8_t i = first_index; i <= last_index; i++) {

        // > MENU ITEM TEXT: VALUE
        if (_selected_item == i) {
            display.setCursor(0, line);
            display.write(">");
        }

        display.setCursor(display.fontWidth() * 3, line);
        display.print(_items[i]->getText().c_str());

        // Is there a value?
        if (-1 != _items[i]->getValueIndex()) {
            // Update it
            uint32_t value = 0;
            APP.storage.read((uint8_t)_items[i]->getValueIndex(), value); // Ignoring return value
            _items[i]->setValue(value);

            // And print it
            display.print(": ");
            display.print(_items[i]->getValue());
        }

        line++;
    }

}

void MenuScreen::handleEvents() {
    
    Joystick::MoveDirection direction = APP.joystick.getDirection();
    bool is_clicked = APP.joystick.isClicked();

    // If in edit mode
    if (_edit_mode) { 
        MenuItem *item = getSelectedItem();

        switch(direction) {
            case Joystick::MoveDirection::Down: {
                item->decrementValue(_multiplier);
                _changed_since_last_render = true;
                break;
            }

            case Joystick::MoveDirection::Up: {
                item->incrementValue(_multiplier);
                _changed_since_last_render = true;
                break;
            }

            case Joystick::MoveDirection::Left: {
                if (_multiplier < 3) {
                    _multiplier += 1;
                    _changed_since_last_render = true;
                }
                break;
            }

            case Joystick::MoveDirection::Right: {
                if (_multiplier > 0) {
                    _multiplier -= 1;
                    _changed_since_last_render = true;
                }
                break;
            }
        }

        // Click ends edit mode
        if (is_clicked) {
            if (!APP.storage.update(item->getValueIndex(), item->getValue())) {
                APP.switchToErrorMode(App::ErrorCodeID::StorageWriteFailed);
            }

            _edit_mode = false;
            _changed_since_last_render = true;
        }

    // In standard (non-edit) mode
    } else {

        // Direction changes scroll down/up the list
        if (Joystick::MoveDirection::Up == direction) {    
            _selected_item = (_selected_item - 1) < 0 ? (_num_of_items - 1) : (_selected_item - 1);
            _changed_since_last_render = true;
        } else if (Joystick::MoveDirection::Down == direction) {
            _selected_item = (_selected_item + 1) == _num_of_items ? 0 : (_selected_item + 1);
            _changed_since_last_render = true;
        }

        // Click enters edit mode or follows the link
        if (is_clicked) {
            // There is no link to follow, just enter edit mode
            if (!getSelectedItem()->followLink()) {
                enterEditMode();
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
