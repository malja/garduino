#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <Arduino.h>

const bool DEBUG = true;

///////////////////////////////////////////////////////////////////////////////
// JOYSTICK
///////////////////////////////////////////////////////////////////////////////

/// Update interval in ms for joystick
const uint16_t JOYSTICK_UPDATE_INTERVAL = 250;

/// Analog pin for reading X axis of connected joystick
const uint8_t JOYSTICK_PIN_X_AXIS = A0;
/// Analog pin for reading Y axis of connected joystick
const uint8_t JOYSTICK_PIN_Y_AXIS = A1;
/// Digital pin for reading push button status
const uint8_t JOYSTICK_PIN_BUTTON = 2;

///////////////////////////////////////////////////////////////////////////////
// EEPROM
///////////////////////////////////////////////////////////////////////////////

/// Number of bytes in EEPROM. Value depends on the board you are using.
const uint16_t EEPROM_SIZE_IN_BYTES = 1024;
const uint8_t EEPROM_BYTE_WATER_USAGE = 100;
const uint8_t EEPROM_BYTE_LITERS_PER_WATERING = 120;
const uint8_t 

#endif 