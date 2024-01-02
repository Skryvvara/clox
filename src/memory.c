#include "memory.h"

#include <stdlib.h>

#include "common.h"
#include "object.h"
#include "vm.h"

void* reallocate(void* pointer, size_t old_size, size_t new_size) {
    if (new_size == 0) {
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, new_size);
    if (result == NULL) exit(EXIT_FAILURE);
    return result;
}

void free_object(object_t* object) {
    switch (object->type) {
        case OBJECT_CLOSURE: {
            object_closure_t* closure = (object_closure_t*)object;
            FREE_ARRAY(object_upvalue_t*, closure->upvalues,
                       closure->upvalue_count);
            FREE(object_closure_t, object);
            break;
        }
        case OBJECT_FUNCTION: {
            object_function_t* function = (object_function_t*)object;
            free_chunk(&function->chunk);
            FREE(object_function_t, object);
            break;
        }
        case OBJECT_NATIVE:
            FREE(object_native_t, object);
            break;
        case OBJECT_STRING: {
            object_string_t* string = (object_string_t*)object;
            FREE_ARRAY(char, string->chars, string->length + 1);
            FREE(object_string_t, object);
            break;
        }
        case OBJECT_UPVALUE:
            FREE(object_upvalue_t, object);
            break;
    }
}

void free_objects() {
    object_t* object = vm.objects;
    while (object != NULL) {
        object_t* next = object->next;
        free_object(object);
        object = next;
    }
}
