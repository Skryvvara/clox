#ifndef clox_compiler_h
#define clox_compiler_h 1

#include "object.h"
#include "vm.h"

bool compile(const char* source, chunk_t* chunk);

#endif
