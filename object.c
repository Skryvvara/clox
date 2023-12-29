#include "object.h"

#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJECT(type, object_type) \
    (type*)allocate_object(sizeof(type), object_type)

static object_t* allocate_object(size_t size, object_type_t type) {
    object_t* object = (object_t*)reallocate(NULL, 0, size);
    object->type = type;
    object->next = object;
    vm.objects = object;
    return object;
}

static object_string_t* allocate_string(char* chars, int length) {
    object_string_t* string = ALLOCATE_OBJECT(object_string_t, OBJECT_STRING);
    string->length = length;
    string->chars = chars;
    return string;
}

object_string_t* take_string(char* chars, int length) {
    return allocate_string(chars, length);
}

object_string_t* copy_string(const char* chars, int length) {
    char* heap_chars = ALLOCATE(char, length + 1);
    memcpy(heap_chars, chars, length);
    heap_chars[length] = '\0';
    return allocate_string(heap_chars, length);
}

void print_object(value_t value) {
    switch (OBJECT_TYPE(value)) {
        case OBJECT_STRING:
            printf("%s", AS_CSTRING(value));
            break;
    }
}
