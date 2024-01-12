#ifndef clox_common_h
#define clox_common_h 1

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define NAN_BOXING 1
#define DEBUG_PRINT_CODE 1
#define DEBUG_TRACE_EXECUTION 1
#define DEBUG_STRESS_GC 1
#define DEBUG_LOG_GC 1
#define UINT8_COUNT (UINT8_MAX + 1)
#define LINE_MAX 1024

#undef DEBUG_PRINT_CODE
#undef DEBUG_TRACE_EXECUTION
#undef DEBUG_STRESS_GC
#undef DEBUG_LOG_GC
#endif
