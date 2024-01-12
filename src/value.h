#ifndef clox_value_h
#define clox_value_h 1

#include <string.h>

#include "common.h"

typedef struct object_t object_t;
typedef struct object_string_t object_string_t;

#ifdef NAN_BOXING

#define SIGN_BIT ((uint64_t)0x8000000000000000)
#define QNAN ((uint64_t)0x7ffc000000000000)

#define TAG_NIL 1    // 01
#define TAG_FALSE 2  // 10
#define TAG_TRUE 3   // 11

typedef uint64_t value_t;

#define IS_BOOL(value) ((value | 1) == TRUE_VAL)
#define IS_NIL(value) ((value) == NIL_VAL)
#define IS_NUMBER(value) (((value) & QNAN) != QNAN)
#define IS_OBJECT(value) \
    (((value) & (QNAN | SIGN_BIT)) == (QNAN | SIGN_BIT))

#define AS_BOOL(value) ((value) == TRUE_VAL)
#define AS_NUMBER(value) value_to_num(value)
#define AS_OBJECT(value) \
    ((object_t*)(uintptr_t)((value) & ~(SIGN_BIT | QNAN)))

#define FALSE_VAL ((value_t)(uint64_t)(QNAN | TAG_FALSE))
#define TRUE_VAL ((value_t)(uint64_t)(QNAN | TAG_TRUE))
#define BOOL_VAL(b) ((b) ? TRUE_VAL : FALSE_VAL)
#define NIL_VAL ((value_t)(uint64_t)(QNAN | TAG_NIL))
#define NUMBER_VAL(num) num_to_value(num)
#define OBJECT_VAL(object) \
    (value_t)(SIGN_BIT | QNAN | (uint64_t)(uintptr_t)(object))

static inline double value_to_num(value_t value) {
    double num;
    memcpy(&num, &value, sizeof(value));
    return num;
}

static inline value_t num_to_value(double num) {
    value_t value;
    memcpy(&value, &num, sizeof(double));
    return value;
}

#else

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
    VAL_OBJECT,
} value_type_t;

typedef struct {
    value_type_t type;
    union {
        bool boolean;
        double number;
        object_t* object;
    } as;
} value_t;

#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NIL(value) ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define IS_OBJECT(value) ((value).type == VAL_OBJECT)

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
#define AS_OBJECT(value) ((value).as.object)

#define BOOL_VAL(value) ((value_t){VAL_BOOL, {.boolean = value}})
#define NIL_VAL ((value_t){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((value_t){VAL_NUMBER, {.number = value}})
#define OBJECT_VAL(value) ((value_t){VAL_OBJECT, {.object = (object_t*)value}})

#endif

typedef struct {
    int capacity;
    int count;
    value_t* values;
} value_array_t;

bool values_equal(value_t a, value_t b);
void init_value_array(value_array_t* array);
void write_value_array(value_array_t* array, value_t value);
void free_value_array(value_array_t* array);
void print_value(value_t value);

#endif
