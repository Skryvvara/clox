#include "object.h"

#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "table.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJECT(type, object_type) \
    (type*)allocate_object(sizeof(type), object_type)

static object_t* allocate_object(size_t size, object_type_t type) {
    object_t* object = (object_t*)reallocate(NULL, 0, size);
    object->type = type;
    object->is_marked = false;

    object->next = vm.objects;
    vm.objects = object;

#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)object, size, type);
#endif

    return object;
}

object_class_t* new_class(object_string_t* name) {
    object_class_t* klass = ALLOCATE_OBJECT(object_class_t, OBJECT_CLASS);
    klass->name = name;
    return klass;
}

object_closure_t* new_closure(object_function_t* function) {
    object_upvalue_t** upvalues =
        ALLOCATE(object_upvalue_t*, function->upvalue_count);
    for (int i = 0; i < function->upvalue_count; i++) {
        upvalues[i] = NULL;
    }

    object_closure_t* closure =
        ALLOCATE_OBJECT(object_closure_t, OBJECT_CLOSURE);
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalue_count = function->upvalue_count;
    return closure;
}

object_function_t* new_function() {
    object_function_t* function =
        ALLOCATE_OBJECT(object_function_t, OBJECT_FUNCTION);
    function->arity = 0;
    function->upvalue_count = 0;
    function->name = NULL;
    init_chunk(&function->chunk);
    return function;
}

object_instance_t* new_instance(object_class_t* klass) {
    object_instance_t* instance =
        ALLOCATE_OBJECT(object_instance_t, OBJECT_INSTANCE);
    instance->klass = klass;
    init_table(&instance->fields);
    return instance;
}

object_native_t* new_native(native_fn_t function, int arity) {
    object_native_t* native = ALLOCATE_OBJECT(object_native_t, OBJECT_NATIVE);
    native->arity = arity;
    native->function = function;
    return native;
}

static object_string_t* allocate_string(char* chars, int length,
                                        uint32_t hash) {
    object_string_t* string = ALLOCATE_OBJECT(object_string_t, OBJECT_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;

    push(OBJECT_VAL(string));
    table_set(&vm.strings, string, NIL_VAL);
    pop();
    return string;
}

static uint32_t hash_string(const char* key, int length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

object_string_t* take_string(char* chars, int length) {
    uint32_t hash = hash_string(chars, length);
    object_string_t* interned =
        table_find_string(&vm.strings, chars, length, hash);

    if (interned != NULL) {
        FREE_ARRAY(char, chars, length + 1);
        return interned;
    }

    return allocate_string(chars, length, hash);
}

object_string_t* copy_string(const char* chars, int length) {
    uint32_t hash = hash_string(chars, length);
    object_string_t* interned =
        table_find_string(&vm.strings, chars, length, hash);

    if (interned != NULL) return interned;

    char* heap_chars = ALLOCATE(char, length + 1);
    memcpy(heap_chars, chars, length);
    heap_chars[length] = '\0';
    return allocate_string(heap_chars, length, hash);
}

object_upvalue_t* new_upvalue(value_t* slot) {
    object_upvalue_t* upvalue =
        ALLOCATE_OBJECT(object_upvalue_t, OBJECT_UPVALUE);
    upvalue->closed = NIL_VAL;
    upvalue->location = slot;
    upvalue->next = NULL;
    return upvalue;
}

static void print_function(object_function_t* function) {
    if (function->name == NULL) {
        printf("<script>");
        return;
    }
    printf("<fn %s>", function->name->chars);
}

void print_object(value_t value) {
    switch (OBJECT_TYPE(value)) {
        case OBJECT_CLASS:
            printf("%s", AS_CLASS(value)->name->chars);
            break;
        case OBJECT_CLOSURE:
            print_function(AS_CLOSURE(value)->function);
            break;
        case OBJECT_FUNCTION:
            print_function(AS_FUNCTION(value));
            break;
        case OBJECT_INSTANCE:
            printf("%s instance", AS_INSTANCE(value)->klass->name->chars);
            break;
        case OBJECT_NATIVE:
            printf("<native fn>");
            break;
        case OBJECT_STRING:
            printf("%s", AS_CSTRING(value));
            break;
        case OBJECT_UPVALUE:
            printf("upvalue");
            break;
    }
}
