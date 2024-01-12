#include "value.h"

#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"

bool values_equal(value_t a, value_t b) {
#ifdef NAN_BOXING
    if (IS_NUMBER(a) && IS_NUMBER(b)) {
        return AS_NUMBER(a) == AS_NUMBER(b);
    }
    return a == b;
#else
    if (a.type != b.type) return false;
    switch (a.type) {
        case VAL_BOOL:
            return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL:
            return true;
        case VAL_NUMBER:
            return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_OBJECT:
            return AS_OBJECT(a) == AS_OBJECT(b);
        default:
            return false;
    }
#endif
}

void init_value_array(value_array_t* array) {
    array->capacity = 0;
    array->count = 0;
    array->values = NULL;
}

void write_value_array(value_array_t* array, value_t value) {
    if (array->capacity < array->count + 1) {
        int old_capacity = array->capacity;
        array->capacity = GROW_CAPACITY(old_capacity);
        array->values =
            GROW_ARRAY(value_t, array->values, old_capacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}

void free_value_array(value_array_t* array) {
    FREE_ARRAY(value_t, array->values, array->capacity);
    init_value_array(array);
}

void print_value(value_t value) {
#ifdef NAN_BOXING
    if (IS_BOOL(value)) {
        printf(AS_BOOL(value) ? "true" : "false");
    } else if (IS_NIL(value)) {
        printf("nil");
    } else if (IS_NUMBER(value)) {
        printf("%g", AS_NUMBER(value));
    } else if (IS_OBJECT(value)) {
        print_object(value);
    }
#else
    switch (value.type) {
        case VAL_BOOL:
            printf(AS_BOOL(value) ? "true" : "false");
            break;
        case VAL_NIL:
            printf("nil");
            break;
        case VAL_NUMBER:
            printf("%g", AS_NUMBER(value));
            break;
        case VAL_OBJECT:
            print_object(value);
            break;
    }
#endif
}
