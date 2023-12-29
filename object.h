#ifndef clox_object_h
#define clox_object_h 1

#include "common.h"
#include "value.h"

#define OBJECT_TYPE(value) (AS_OBJECT(value)->type)

#define IS_STRING(value) is_object_type(value, OBJECT_STRING)

#define AS_STRING(value) ((object_string_t*)AS_OBJECT(value))
#define AS_CSTRING(value) (((object_string_t*)AS_OBJECT(value))->chars)

typedef enum {
    OBJECT_STRING,
} object_type_t;

struct object_t {
    object_type_t type;
    struct object_t* next;
};

struct object_string_t {
    object_t object;
    int length;
    char* chars;
};

object_string_t* take_string(char* chars, int length);
object_string_t* copy_string(const char* chars, int length);
void print_object(value_t value);

static inline bool is_object_type(value_t value, object_type_t type) {
    return IS_OBJECT(value) && AS_OBJECT(value)->type == type;
}

#endif
