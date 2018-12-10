#ifndef MENU_ITEM_HPP
#define MENU_ITEM_HPP

#include <Arduino.h>

class MenuScreen;

class MenuItem {

    friend MenuScreen;

    public:

        /**
         * Create new MenuItem. 
         * @param text Text description which should be shown. Maximum 15 characters.
         * @param valueIndex Variable index inside Storage. Use -1 for no value. 
         * @param screenId ID of screen which should be activated after on click event.
         * Use -1 for no screen change.
         */
        MenuItem(const String &text, int8_t valueIndex, int8_t screenId);

        String getText() const {
            return _text;
        }

        uint32_t getValue() const {
            return _value;
        }

        int8_t getValueIndex() const {
            return _value_index;
        }

        int8_t getScreenId() const {
            return _screen_id;
        }

        void setValue(uint32_t value) {
            _value = value;
        }

    protected:

        String _text;
        uint32_t _value;
        int8_t _value_index;
        int8_t _screen_id;
};

#endif
