#include "Display.hpp"

Display::Display() {
    _driver.begin(DISPLAY_TYPE, DISPLAY_I2C_ADDRESS);
    _driver.setFont(DISPLAY_FONT);
    _driver.clear();

    _line = 1;
}

void Display::print(const char *text) {

    _driver.write(text);
    _driver.setRow(_line++);
    _driver.setCol(0);

    if (DISPLAY_NUM_LINES < _line + 1) {
        _line = 1;
    }
    
}

void Display::clear() {
    _driver.clear();
}

