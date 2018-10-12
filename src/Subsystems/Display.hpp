#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "../config.hpp"
#include "./Display/SSD1306Ascii.h"
#include "./Display/SSD1306AsciiAvrI2c.h"

class Display {

    public:

        /**
         * Connect I2C to display and setup it for writing.
         */
        bool setup();

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
        
};
#endif