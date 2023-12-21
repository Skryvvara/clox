#ifndef clox_debug_h
#define clox_debug_h 1

#include "chunk.h"

void disassemble_chunk(chunk_t* chunk, const char* name);
int disassemble_instruction(chunk_t* chunk, int offset);

#endif

