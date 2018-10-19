#ifndef STORAGE_H
#define STORAGE_H

#include "../config.hpp"
#include <Arduino.h>

/*
Hlavička
========

12345678 - 8bit podpis - vždy pevná hodnota 11011011
12345678 
    1234 - 4bit hodnota určující celkovou velikost paměti v bajtech, kde 1 znamená 2^6 a 6 znamená 2^12. Poslední bit vždy nula.
       5 - 1bit stav paměti, 1 - nebyl detekován problém, 0 - byl detekován problém
     678 - 3bit - ???
12345678 - 8bit - počet proměnných v paměti
12345678 - 8bit hodnota udávající počet prvků v normálním poli (Jsou dva typy - normální a často měněná)


Hodnota
=======

12345678 - 8bit - ID proměnné
12345678
       1 - 1bit - typ proměnné (1 - normální, 0 - často měněná)
 2345678 - 7bit - určující počet bajtů v jedné proměnné
- Následují data
*/



/**
 * Storage class manage EEPROM storage. Main purpose is to prevent fast memory
 * cell wear out. 
 * 
 * Header
 * ======
 * 
 * EEPROM memory begins with a header. It contains all information for managing
 * data. It consists of 4 bytes.
 * 
 *     ------------------- <- Base EEPROM address
 *     | Storage Mark    |
 *     |-----------------|
 *     | Memory Settings |
 *     |-----------------|
 *     | # of variables  |
 *     |-----------------|
 *     | # of elements   |
 *     |-----------------| <- First memory cell address
 *     | ... Data ...    |
 *
 * - **Storage Mark** is hardcoded 8bit value which marks the begining of
 *   Storage space. It always contains binary value `11011011`. When new storage
 *   object is created, `base EEPROM address` passed to constructor should point
 *   to Storage Mark. If not, new storage header structure will be created
 *   starting at `base EEPROM address`.
 * - **Memory Settings** contains three different things:
 *   - 4 bits - Memory size in 2^x bytes, where x is a value from 6 to 12. To represent
 *              this range, decimal value 1 stored in those 4 bits means x=6. On
 *              the other hand, decimal value of 6 means x=12.
 *   - 1 bit  - Memory status. 1 means no memory IO error was detected. 0 means
 *              at least one IO error. After this, each memory related operation
 *              will fail.
 *   - 3 bits - CRC of all header values.
 *  - **# of variables** - Third byte stores number of variables. This number
 *    cannot change during runtime. All available bytes (Memory size - header
 *    size) are equaly divided into this number of variables.
 *  - **# of elements** - Last byte contains number of elements in standard
 *    variable array. There are two types of variable arrays - standard and
 *    volatile. The volatile array type has twice as many elements, as the
 *    standard. 
 *     
 * Storage uses two array for each stored variable. First one
 * stores the actual value. Each time value should be rewritten, the actual
 * value is written to next element in the array. This increase each cell
 * lifespan. 
 * Second array holds information about in which element of the first array the
 * value is stored. Each time value changes its position, counter is increased
 * and written to the next cell. When counter value reaches 255, it overflows
 * back to 0. At this moment, whole array has to be checked. If both 255 and 0
 * values are present, 0 means latest value.
 * 
 * Background
 * ==========
 * 
 * This approach was selected because of two reasons. 
 * 
 * 1) Corrupting one cell may damage adjacent cells as well. Or take the whole
 *    EEPROM memory down with it. This means that as first corrupted cell is
 *    detected, memory is considered unusable.
 * 2) 
 */

/*
1B - 1
4B - 1
2B - 0
16B - 0
12B - 1
===
35B

150B
/
(
    1B
    +
    4B
    +
    2B*2
    +
    16B*2
    +12B
) = 150/53 = 2,8 => 2

2B
16B
16B
64B
24B
====
*/

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
        bool write(const T &value, uint16_t address);

        /**
         * Enables write protection. After 256 writes into EEPROM, writing will
         * be disabled. This functionality is to protect agains ruining
         * EEPROM memory during testing.
         */
        void enableProtection();

    protected:

        /// Next address usable for storing value.
        uint16_t _next_address;

        /// Counts writes into EEPROM for protection agains unwanted ruining memory.
        uint8_t _protection;
};

#endif