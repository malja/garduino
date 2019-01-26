#ifndef GARDUINO_STORAGE_CACHE_H
#define GARDUINO_STORAGE_CACHE_H

#include <Arduino.h>
#include "../config.h"

/**
 * Stores copy of EEPROM variables in RAM for fast reading. Values are updated from Storage class. 
 */
class StorageCache {

    public:
        StorageCache();

        /**
         * Write new value or update already existing one. 
         * @param index Varaible index.
         * @param value Variable value.
         * @note Index range is checked with STORAGE_VARIABLES_COUNT.
         */
        bool store(const uint8_t index, uint32_t value);

        /**
         * Read value.
         * @param index Variable index.
         * @param value Place where retrived value should be stored. Cache is zeroed at beginning,
         * so for non-loaded variables zero is returned.
         * @note Index range is checked with STORAGE_VARIABLES_COUNT.
         */
        bool retrive(const uint8_t index, uint32_t &value) const;
        
        /**
         * Return maximal number of variables which could be stored in cache. Real number of variables
         * may be different.
        */
        inline uint8_t getSize() const {
            return STORAGE_VARIABLES_COUNT;
        }

    protected:
        uint32_t _cache[STORAGE_VARIABLES_COUNT];
};

#endif
