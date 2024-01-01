#ifndef clox_compiler_h
#define clox_compiler_h 1

#include "object.h"
#include "vm.h"

object_function_t* compile(const char* source);

#endif
