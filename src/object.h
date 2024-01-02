#ifndef clox_object_h
#define clox_object_h 1

#include "common.h"
#include "chunk.h"
#include "value.h"

#define OBJECT_TYPE(value) (AS_OBJECT(value)->type)

#define IS_CLOSURE(value) is_object_type(value, OBJECT_CLOSURE)
#define IS_FUNCTION(value) is_object_type(value, OBJECT_FUNCTION)
#define IS_NATIVE(value) is_object_type(value, OBJECT_NATIVE)
#define IS_STRING(value) is_object_type(value, OBJECT_STRING)

#define AS_CLOSURE(value) ((object_closure_t*)AS_OBJECT(value))
#define AS_FUNCTION(value) ((object_function_t*)AS_OBJECT(value))
#define AS_NATIVE_OBJECT(value) ((object_native_t*)AS_OBJECT(value))
#define AS_NATIVE(value) (((object_native_t*)AS_OBJECT(value))->function)
#define AS_STRING(value) ((object_string_t*)AS_OBJECT(value))
#define AS_CSTRING(value) (((object_string_t*)AS_OBJECT(value))->chars)

typedef enum {
    OBJECT_CLOSURE,
    OBJECT_FUNCTION,
    OBJECT_NATIVE,
    OBJECT_STRING,
    OBJECT_UPVALUE,
} object_type_t;

struct object_t {
    object_type_t type;
    struct object_t* next;
};

typedef struct {
    object_t object;
    int arity;
    int upvalue_count;
    chunk_t chunk;
    object_string_t* name;
} object_function_t;

typedef value_t (*native_fn_t)(int arg_count, value_t* args);

typedef struct {
    object_t object;
    int arity;
    native_fn_t function;
} object_native_t;

struct object_string_t {
    object_t object;
    int length;
    char* chars;
    uint32_t hash;
};

typedef struct object_upvalue_t {
    object_t object;
    value_t* location;
    value_t closed;
    struct object_upvalue_t* next;
} object_upvalue_t;

typedef struct {
    object_t object;
    object_function_t* function;
    object_upvalue_t** upvalues;
    int upvalue_count;
} object_closure_t;

object_closure_t* new_closure(object_function_t* function);
object_function_t* new_function();
object_native_t* new_native(native_fn_t function, int arity);
object_string_t* take_string(char* chars, int length);
object_string_t* copy_string(const char* chars, int length);
object_upvalue_t* new_upvalue(value_t* slot);
void print_object(value_t value);

static inline bool is_object_type(value_t value, object_type_t type) {
    return IS_OBJECT(value) && AS_OBJECT(value)->type == type;
}

#endif
