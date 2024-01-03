#ifndef clox_memory_h
#define clox_memory_h 1

#include "common.h"
#include "value.h"

#define GC_INIT_TRESHOLD 1048576
#define GC_HEAP_GROW_FACTOR 2

#define ALLOCATE(type, count) (type*)reallocate(NULL, 0, sizeof(type) * (count))

#define MINIMUM_CAPACITY 8

#define GROW_CAPACITY(capacity) \
    ((capacity) < MINIMUM_CAPACITY ? MINIMUM_CAPACITY : (capacity) * 2)

#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0)

#define GROW_ARRAY(type, pointer, old_count, new_count)    \
    (type*)reallocate(pointer, sizeof(type) * (old_count), \
                      sizeof(type) * (new_count))

#define FREE_ARRAY(type, pointer, old_count) \
    reallocate(pointer, sizeof(type) * (old_count), 0)

void* reallocate(void* pointer, size_t old_size, size_t new_size);
void mark_object(object_t* object);
void mark_value(value_t value);
void collect_garbage();
void free_objects();

#endif
