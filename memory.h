#ifndef clox_memory_h
#define clox_memory_h 1

#include "common.h"

#define MINIMUM_CAPACITY 8

#define GROW_CAPACITY(capacity) \
    ((capacity) < MINIMUM_CAPACITY ? MINIMUM_CAPACITY : (capacity) * 2)

#define GROW_ARRAY(type, pointer, old_count, new_count) \
    (type*)reallocate(pointer, sizeof(type) * (old_count), \
        sizeof(type) * (new_count))

#define FREE_ARRAY(type, pointer, old_count) \
    reallocate(pointer, sizeof(type)*(old_count), 0)

void* reallocate(void* pointer, size_t old_size, size_t new_size);

#endif
