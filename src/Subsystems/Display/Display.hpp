#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "../../config.hpp"
#include "./driver/SSD1306Ascii.h"
#include "./driver/SSD1306AsciiAvrI2c.h"

class Display {

    public:

        /**
         * Connect I2C to display and setup it for writing.
         */
        Display();

        /**
         * Print new line on display. Automatically adds newline at the end.
         */
        void print(const char *text);

        /**
         * Clear whole display. Next print will start at the top left corner.
         */
        void clear();
    
    protected:
        uint8_t _line;
        SSD1306AsciiAvrI2c _driver;
        
};
#endif