#ifndef clox_table_h
#define clox_table_h 1

#include "common.h"
#include "value.h"

typedef struct {
    object_string_t* key;
    value_t value;
} entry_t;

typedef struct {
    int count;
    int capacity;
    entry_t* entries;
} table_t;

void init_table(table_t* table);
void free_table(table_t* table);
bool table_get(table_t* table, object_string_t* key, value_t* value);
bool table_set(table_t* table, object_string_t* key, value_t value);
bool table_delete(table_t* table, object_string_t* key);
void table_add_all(table_t* from, table_t* to);
object_string_t* table_find_string(table_t* table, const char* chars,
                                   int length, uint32_t hash);

#endif
