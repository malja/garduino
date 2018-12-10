#include "MenuItem.hpp"
#include "MenuScreen.hpp"

MenuItem::MenuItem(const String &text, int8_t valueIndex, int8_t screenId) {
    _text = text;
    
    this->_value_index = valueIndex;
    this->_screen_id = screenId;
}
