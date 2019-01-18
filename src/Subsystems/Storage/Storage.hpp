#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <EEPROM.h>

#include "../config.hpp"
#include "StorageCache.hpp"

/**
 * Storage class handle storing data in EEPROM. Data is persistent between bootups.
 * Because writing into EEPROM wears it out, Storage class moves variables across
 * whole space dedicated in EEPROM.
 * 
 * To make sure data in EEPROM is untouched, on every bootup, content is checked
 * for Storage header. It's one byte containing value 0b11011011. In addition
 * it marks begining of EEPROM space dedicated to Storage. After Storage header,
 * variables are stored.
 * 
 * Each variable is stored as 5 bytes. First one contains variable index - unique
 * ID. First bit of index is storing variable state. 0 means slot contains older
 * value and 1 means value is up to date.
 * Rest 4 bytes contain actual value. See Storage::Container::Variable for 
 * structure.
 */
class Storage {

    public:

        /**
         * Container is small wrapper around variable to help handle operations
         * with variable index.
         */
        struct Container {

            Container() {
                address = 0;
                variable.index = 0;
                variable.value = 0;
            }

            /**
             * Return size of variable in bytes.
             */
            inline static uint8_t getVariableSize() {
                return sizeof(Variable);
            }

            /**
             * If index's first bit is 1, value is up to date.
             */
            inline bool isUpToDate() const {
                return variable.index >> 7 == 1;
            }

            /**
             * Return actual index without first bit. 
             */
            uint8_t getIndex() const {
                return variable.index & ~(1 << 7);
            }

            /**
             * Switch first bit of index to zero and thus make it invalid.
             */
            inline void invalidate() {
                variable.index = getIndex();
            }

            /**
             * Switch first bit of index to one and make it valid again.
             */
            inline void makeValid() {
                variable.index |= (1 << 7);
            }

            inline bool isFree() const {
                return !isUpToDate() && variable.value == UINT32_MAX;
            }

            inline void setFree() {
                invalidate();
                variable.value = UINT32_MAX;
            }

            /**
             * Actual structure stored in EEPROM.
             */
            struct Variable {
                uint8_t index;    ///< Unique variable ID.
                uint32_t value;   ///< Content of the variable.
            };

            uint16_t address;   ///< Address of EEPROM memory variable was read from.

            Variable variable;  ///< Variable itself.
        };

        

        /**
         * Make sure current header in EEPROM is valid and up to date.
         * @param start_offset Offset in bytes counted from EEPROM begining.
         * @param memory_size EEPROM memory size in bytes.
         * @return If memory is prepared.
         */
        bool setup(const uint16_t start_offset, const uint16_t memory_size);

        /**
         * Create new value stored in EEPROM. This method MUST NOT be
         * used for updating already written variables.
         * @param index ID of value.
         * @returns If value was stored.
         * @see update
         */
        bool write(const uint8_t index, const uint32_t value);

        /**
         * Update already existing variable stored in EEPROM. This method MUST NOT
         * be used for creating new variables.
         * @param index ID of value.
         * @param value New value.
         * @returns If value was updated.
         * @see write
         */
        bool update(const uint8_t index, const uint32_t value);

        inline bool read(const uint8_t index, uint32_t &value) const {
            return _cache.retrive(index, value);
        }

        /**
         * Enable write protection. After 256 writes into EEPROM, writing will
         * be disabled. This functionality is to protect agains ruining
         * EEPROM memory during testing.
         */
        inline void enableProtection() {
            _protection = true;
        }

        /**
         * Remove all stored values and write header again. 
         */
        bool clear();

        /**
         * Return address of beginning of EEPROM block allocated for Storage.
         * After calling setup(), this address should point to Storage header.
         */
        uint16_t getMemoryBeginningAddress() const {
            return _beginning_address;
        }

        uint16_t getMemoryEndAddress() const {
            return _end_address;
        }

        /**
         * Return Header's size in bytes.
         */
        inline uint8_t getHeaderSize() const {
            return 1;
        }

        /**
         * When whole memory is zeroed, returns true.
        */
        inline bool isEmpty() const {
            return _is_empty;
        }

        void print() const;

    protected:

        /**
         * Make sure header is stored at first address allocated for Storage. If so, cache is updated.
         * On the other hand, whole EEPROM block dedicated for Storage is cleaned and header is written.
         */
        bool prepareMemory();

        /**
         * Fill cache with data from EEPROM. This method is called on each startup, to load data into
         * RAM memory. Readings are done from cache. Only when value is updated, both values in cache 
         * and EEPROM are updated.
         */
        bool populateCache();

        /**
         * Update _next_address_to_write to next free address for writing.
         */
        bool findEmptyCell(uint16_t &emptyAddress);

        /**
         * Fill all EEPROM dedicated to storage with zeroes. 
         */
        bool clearData();
        
        /**
         * Return value from EEPROM.
         * @param index Address from which should be the value read.
         * @param container Container with variable, where should data be stored.
         * @returns True if value was returned properly.
         */
        bool rawRead(const uint8_t index, Storage::Container &container) const;

        /**
         * Finds up to date variable with required index and returns its address inside EEPROM. 
         */
        bool getAddressFromIndex(const uint8_t index, uint16_t &address) const;

        uint16_t _beginning_address;        ///< Address of beginning of the block allocated for Storage.
                                            ///< This address is filled with Header.
        uint16_t _end_address;              ///< Address of last EEPROM cell usable with Storage.

        uint16_t _last_written_address;     ///< Last written address. Searching for address next time should be written to, is starting from this point.

        /// Counts writes into EEPROM for protection agains unwanted ruining memory.
        uint8_t _protection_counter;
        bool _protection;

        bool _is_empty;                     ///< When prepareMemory is called on EEPROM without header, and whole
                                            ///< memory is cleaned, this flag is set to true. In this case, default
                                            ///< values from config are written.

        StorageCache _cache;
};

#endif