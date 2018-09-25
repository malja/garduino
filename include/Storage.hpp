#ifndef STORAGE_H
#define STORAGE_H

#include "config.hpp"

class Storage {

    public:

        /**
         * Add new value to EEPROM.
         * @param value Value to be stored.
         * @returns Value address.
         */
        template <typename T>
        uint16_t store(const T &value);

        /**
         * Return value from EEPROM.
         * @param address Address from which should be the value read.
         * @param value Address of memory, which should be filled with new value.
         * @returns True if value was returned properly.
         */
        template <typename T>
        bool read(uint16_t address, const T &value);

        /**
         * Write value to EEPROM addres. If values don't differ, it will not be updated.
         * @param value Value to store.
         * @param address Addres to store value.
         * @return True if address was updated.
         */
        template <typename T>
        bool write(const &T value, uint16_t address);

        /**
         * Enables write protection. After 256 writes into EEPROM, writing will
         * be disabled. This functionality is to protect agains ruining
         * EEPROM memory during testing.
         */
        void enableProtection();

    protected:

        /// Next address usable for storing value.
        uin16_t _next_address;

        /// Counts writes into EEPROM for protection agains unwanted ruining memory.
        uint8_t _protection;
};

#endif