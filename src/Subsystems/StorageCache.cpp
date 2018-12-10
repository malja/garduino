#include "StorageCache.hpp"

StorageCache::StorageCache() {
    for(uint8_t i = 0; i < getSize(); i++) {
        _cache[i] = 0;
    }
}

bool StorageCache::store(const uint8_t index, uint32_t value) {
    if (index >= getSize()) {
        return false;
    }

    _cache[index] = value;
    return true;
}

bool StorageCache::retrive(const uint8_t index, uint32_t &value) const {
    if (index >= getSize()) {
        return false;
    }

    value = _cache[index];
    return true;
}
