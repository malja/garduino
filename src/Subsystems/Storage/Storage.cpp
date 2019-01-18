#include "Storage.hpp"

bool Storage::setup(const uint16_t start_offset, const uint16_t memory_size) {
    _beginning_address = start_offset;
    _end_address = start_offset + memory_size;
    
    _protection = false;
    _protection_counter = 0;
    _last_written_address = start_offset + getHeaderSize();

    // Write new header, or preserve already existing one.
    return prepareMemory();
}

bool Storage::write(const uint8_t index, const uint32_t value) {

    // Disable writing when counter reach 255
    if (_protection && _protection_counter+1 >= 255) {
        return false;
    }

    // Get address for write
    uint16_t address = 0;
    if (!findEmptyCell(address)) {
        return false;
    }

    // Prepare data
    Storage::Container container;
    container.variable.index = index;
    container.variable.value = value;
    container.makeValid();

    // Write value to memory
    EEPROM.put(address, container.variable);

    // Update cache
    _cache.store(index, value);

    _is_empty = false;

    if (_protection) {
        _protection_counter++;
    }

    return true;
}

bool Storage::update(const uint8_t index, const uint32_t value) {
    
    if (_protection && _protection_counter + 1 >= 255) {
        return false;
    }

    Storage::Container container;
    if (!rawRead(index, container)) {
        return false;
    }

    uint16_t new_address = 0;
    if (!findEmptyCell(new_address)) {
        return false;
    }

    // Make old value obsolete
    container.setFree();
    EEPROM.put(container.address, container.variable);

    // Write value to new location
    container.variable.index = index;
    container.variable.value = value;
    container.makeValid();

    EEPROM.put(new_address, container.variable);

    // Update cache
    if (!_cache.store(index, value)){
        return false;
    }

    if (_protection) {
        _protection_counter++;
    }

    return true;
}

bool Storage::rawRead(const uint8_t index, Storage::Container &container) const {
    
    uint16_t address = 0;
    if (!getAddressFromIndex(index, address)) {
        return false;
    }

    // Fill container
    container.address = address;
    EEPROM.get(address, container.variable);

    return true;

}

bool Storage::getAddressFromIndex(const uint8_t index, uint16_t &address) const {
    
    Storage::Container container;

    for(uint16_t a = getMemoryBeginningAddress() + getHeaderSize(); a <= getMemoryEndAddress() - Storage::Container::getVariableSize(); ) {
        EEPROM.get(a, container.variable);

        if (container.isUpToDate() && container.getIndex() == index) {
            address = a;
            return true;
        }

        a += Storage::Container::getVariableSize();
    }

    return false;
}

bool Storage::clearData() {

    Storage::Container container;
    container.setFree();

    for(uint16_t address = getMemoryBeginningAddress() + getHeaderSize(); address <= getMemoryEndAddress() - Storage::Container::getVariableSize(); address += Storage::Container::getVariableSize()) {
        EEPROM.put(address, container.variable);
    }

    _is_empty = true;
    return true;
}

bool Storage::findEmptyCell(uint16_t &emptyAddress) {
    
    Storage::Container container;

    for(uint16_t address = _last_written_address; address <= getMemoryEndAddress() - Storage::Container::getVariableSize(); ) {
        EEPROM.get(address, container.variable);

        if (container.isFree()) {
            _last_written_address = address;
            emptyAddress = address;
            return true;
        }

        address += Storage::Container::getVariableSize();
    }

    for(uint16_t address = getMemoryBeginningAddress() + getHeaderSize(); address <= _last_written_address - Storage::Container::getVariableSize(); ) {
        EEPROM.get(address, container.variable);

        if (container.isFree()) {
            _last_written_address = address;
            emptyAddress = address;
            return true;
        }

        address += Storage::Container::getVariableSize();
    }

    return false;
}

void Storage::print() const {
    Container container;

    for(uint16_t address = getMemoryBeginningAddress() + getHeaderSize(); address <= getMemoryEndAddress() - Storage::Container::getVariableSize(); ) {

        // Fill Variable in variable container
        EEPROM.get(address, container.variable);

        address += Storage::Container::getVariableSize();
    }
}

bool Storage::populateCache() {
    Container container;
    for(uint16_t address = getMemoryBeginningAddress() + getHeaderSize(); address <= getMemoryEndAddress() - Storage::Container::getVariableSize(); ) {

        // Fill Variable in variable container
        EEPROM.get(address, container.variable);
        
        // Only fill up to date variables
        if (container.isUpToDate()) {
            if (!_cache.store(container.getIndex(), container.variable.value)) {
                return false;
            }
        }

        address += Storage::Container::getVariableSize();
    }

    return true;
}


bool Storage::prepareMemory() {
    uint8_t storage_mark = 0;
    
    // Read header, to make sure Storage is there
    storage_mark = EEPROM.read(getMemoryBeginningAddress());
    if (storage_mark != 219) {

        // Not there, write header and clear all data
        EEPROM.write(getMemoryBeginningAddress(), 219 /*0b11011011*/);
        clearData();
    }
    
    // Header is already there. Load stored data into cache and find which address
    // should be written on next Variable's update.
    return populateCache();
}