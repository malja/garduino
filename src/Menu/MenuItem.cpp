#include "MenuItem.h"
#include "MenuScreen.h"

MenuItem::MenuItem(const String &text, int8_t valueIndex, void (*click_callback)()) {
    _text = text;
    
    this->_value_index = valueIndex;
    this->_callback = click_callback;
}

bool MenuItem::followLink() {
    if (nullptr == _callback) {
        return false;
    }

    _callback();
    return true;
}