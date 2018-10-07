#include "Display.hpp"

bool Display::setup() {
    oled.begin(DISPLAY_TYPE, DISPLAY_I2C_ADDRESS);
    oled.setFont(DISPLAY_FONT);
    oled.clear();

    this->_line = 0;
}

void Display::print(const char *text) {
    oled.write(text);
    oled.setCol(0);
    
    if (DISPLAY_NUM_LINES == this->_line + 1) {
        this->_line = 0;
    }

    oled.setRow(this->line++);
}

void clear() {
    oled.clear();
}

