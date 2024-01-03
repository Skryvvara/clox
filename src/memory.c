#include "memory.h"

#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "object.h"
#include "vm.h"

#ifdef DEBUG_LOG_GC
#include <stdio.h>

#include "debug.h"
#endif

void* reallocate(void* pointer, size_t old_size, size_t new_size) {
    vm.bytes_allocated += new_size - old_size;

    if (new_size > old_size) {
#ifdef DEBUG_STRESS_GC
        collect_garbage();
#endif
    }

    if (vm.bytes_allocated > vm.next_gc) {
        collect_garbage();
    }

    if (new_size == 0) {
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, new_size);
    if (result == NULL) exit(EXIT_FAILURE);
    return result;
}

void mark_object(object_t* object) {
    if (object == NULL) return;
    if (object->is_marked) return;
    object->is_marked = true;

#ifdef DEBUG_LOG_GC
    printf("%p mark ", (void*)object);
    print_value(OBJECT_VAL(object));
    printf("\n");
#endif

    if (vm.gray_capacity < vm.gray_count + 1) {
        vm.gray_capacity = GROW_CAPACITY(vm.gray_capacity);
        vm.gray_stack = (object_t**)realloc(
            vm.gray_stack, sizeof(object_t*) * vm.gray_capacity);

        if (vm.gray_stack == NULL) exit(EXIT_FAILURE);
    }

    vm.gray_stack[vm.gray_count++] = object;
}

void mark_value(value_t value) {
    if (IS_OBJECT(value)) mark_object(AS_OBJECT(value));
}

void mark_array(value_array_t* array) {
    for (int i = 0; i < array->count; i++) {
        mark_value(array->values[i]);
    }
}

static void blacken_object(object_t* object) {
#ifdef DEBUG_LOG_GC
    printf("%p blacken ", (void*)object);
    print_value(OBJECT_VAL(object));
    printf("\n");
#endif

    switch (object->type) {
        case OBJECT_CLOSURE: {
            object_closure_t* closure = (object_closure_t*)object;
            mark_object((object_t*)closure->function);
            for (int i = 0; i < closure->upvalue_count; i++) {
                mark_object((object_t*)closure->upvalues[i]);
            }
            break;
        }
        case OBJECT_FUNCTION: {
            object_function_t* function = (object_function_t*)object;
            mark_object((object_t*)function->name);
            mark_array(&function->chunk.constants);
            break;
        }
        case OBJECT_UPVALUE:
            mark_value(((object_upvalue_t*)object)->closed);
            break;
        case OBJECT_NATIVE:
        case OBJECT_STRING:
            break;
    }
}

void free_object(object_t* object) {
#ifdef DEBUG_LOG_GC
    printf("%p free type %d\n", (void*)object, object->type);
#endif

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

static void mark_roots() {
    for (value_t* slot = vm.stack; slot < vm.stack_top; slot++) {
        mark_value(*slot);
    }

    for (int i = 0; i < vm.frame_count; i++) {
        mark_object((object_t*)vm.frames[i].closure);
    }

    for (object_upvalue_t* upvalue = vm.open_upvalues; upvalue != NULL;
         upvalue = upvalue->next) {
        mark_object((object_t*)upvalue);
    }

    mark_table(&vm.globals);
    mark_compiler_roots();
}

static void trace_references() {
    while (vm.gray_count > 0) {
        object_t* object = vm.gray_stack[--vm.gray_count];
        blacken_object(object);
    }
}

static void sweep() {
    object_t* previous = NULL;
    object_t* object = vm.objects;
    while (object != NULL) {
        if (object->is_marked) {
            object->is_marked = false;
            previous = object;
            object = object->next;
        } else {
            object_t* unreached = object;
            object = object->next;
            if (previous != NULL) {
                previous->next = object;
            } else {
                vm.objects = object;
            }

            free_object(unreached);
        }
    }
}

void collect_garbage() {
#ifdef DEBUG_LOG_GC
    printf("-- gc begin\n");
    size_t before = vm.bytes_allocated;
#endif

    mark_roots();
    trace_references();
    table_remove_white(&vm.strings);
    sweep();

    vm.next_gc = vm.bytes_allocated * GC_HEAP_GROW_FACTOR;

#ifdef DEBUG_LOG_GC
    printf("-- gc end\n");
    printf("   collected %zu bytes (from %zu to %zu) next at %zu\n",
           before - vm.bytes_allocated, before, vm.bytes_allocated, vm.next_gc);
#endif
}

void free_objects() {
    object_t* object = vm.objects;
    while (object != NULL) {
        object_t* next = object->next;
        free_object(object);
        object = next;
    }

    free(vm.gray_stack);
}
