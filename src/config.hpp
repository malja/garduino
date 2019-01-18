#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <Arduino.h>
#include "./Subsystems/Display/SSD1306Ascii.h"

const bool DEBUG = true;

///////////////////////////////////////////////////////////////////////////////
// JOYSTICK
///////////////////////////////////////////////////////////////////////////////

/// Update interval in ms for joystick
#define JOYSTICK_UPDATE_INTERVAL 250

/// Analog pin for reading X axis of connected joystick
#define JOYSTICK_PIN_X_AXIS A0

/// Analog pin for reading Y axis of connected joystick
#define JOYSTICK_PIN_Y_AXIS A1

/// Digital pin for reading push button status
#define JOYSTICK_PIN_BUTTON 3

///////////////////////////////////////////////////////////////////////////////
// EEPROM
///////////////////////////////////////////////////////////////////////////////

/// Number of bytes from EEPROM start to Storage header. This points to first
/// address dedicated for Storage.
#define STORAGE_OFFSET_IN_BYTES 0

/// Number of bytes in available for Storage. Don't forget to count offset in!
#define STORAGE_SIZE_IN_BYTES 1023 // 1024 (total size) - 0 (offset) - 1 (header)

/// Number of varibales stored in EEPROM
#define STORAGE_VARIABLES_COUNT 5

/// Indexes of all variables stored in EEPROM. Those are used for
/// reading and writing variables to and from memory.
#define HUMIDITY_THRESHOLD_INDEX 0
#define LITERS_PER_WATERING_INDEX 1
#define HUMIDITY_CHECK_INTERVAL_INDEX 2
#define LITERS_TOTAL_INDEX 3
#define LAST_HUMIDITY_INDEX 4   ///< Humidity value of last measurement

///////////////////////////////////////////////////////////////////////////////
// DISPLAY
///////////////////////////////////////////////////////////////////////////////

// Display address for I2C connection
#define DISPLAY_I2C_ADDRESS 0x3C
#define DISPLAY_TYPE &Adafruit128x32
#define DISPLAY_FONT System5x7
#define DISPLAY_NUM_LINES 3
#define DISPLAY_NUM_CHARACTERS 25

#define MENU_HEADER_LENGTH 15

///////////////////////////////////////////////////////////////////////////////
// EVENTS
///////////////////////////////////////////////////////////////////////////////

#define EVENTS_POOL_SIZE 10

///////////////////////////////////////////////////////////////////////////////
// APPLICATION
///////////////////////////////////////////////////////////////////////////////

/// Number of app screens. Value is used for generating list of all app screens.
#define APP_SCREENS_COUNT 3

///////////////////////////////////////////////////////////////////////////////
// INTERRUPTIONS
///////////////////////////////////////////////////////////////////////////////

#define INTERRUPTIONS_PIN_WATER_METER 2

///////////////////////////////////////////////////////////////////////////////
// RUNTIME
///////////////////////////////////////////////////////////////////////////////

/// Default values for all variables stored in EEPROM.
#define DEFAULT_HUMIDITY_THRESHOLD 120
#define DEFAULT_LITERS_PER_WATERING 5
#define DEFAULT_HUMIDITY_CHECK_INTERVAL 30
#define DEFAULT_LITERS_TOTAL 0
#define DEFAULT_LAST_HUMIDITY 0

#endif 