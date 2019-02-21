#ifndef GARDUINO_MENU_ITEM_H
#define GARDUINO_MENU_ITEM_H

#include <Arduino.h>

class MenuScreen;

class MenuItem {

    friend MenuScreen;

    public:

        /**
         * Create new MenuItem. 
         * @param text Text description which should be shown.
         * @param valueIndex Variable index inside Storage. Use -1 for no value. 
         * @param click_callback Pointer to function which should be executed when menu item is clicked.
         * Set it to nullptr if second value is not negative and you only want edit the value in memory.
         */
        MenuItem(const String &text, int8_t valueIndex, void (*click_callback)());

        String getText() const {
            return _text;
        }

        uint32_t getValue() const {
            return _value;
        }

        int8_t getValueIndex() const {
            return _value_index;
        }

        /**
         * Return number of characters in numerical value.
         * 
         * Example: 
         * - value 124 => 3
         * - value 68767 => 5
         * - value 0 => 1
        */
        uint8_t getValueLengthInChars() const;

        /**
         * If callback is defined, it is executed and true is returned. In other cases, false is returned.
        */
        bool followLink();

        void incrementValue(uint8_t multiplier) {
            // Without round(), value could be decreased by one due to some weird rounding
            _value += round(pow(10, multiplier));
        }

        void decrementValue(uint8_t multiplier) {
            // Without round(), value could be decreased by one due to some weird rounding
            uint32_t decrement = round(pow(10, multiplier));
            if (_value < decrement) {
                _value = 0;
            } else {
                _value -= decrement;
            }
        }

        void setValue(uint32_t value) {
            _value = value;
        }

    protected:

        String _text;
        uint32_t _value;
        int8_t _value_index;
        void (*_callback)();
};

#endif
