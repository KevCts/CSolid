#ifndef CSOLID_CHUNK_H
#define CSOLID_CHUNK_H

#include "memory.h"
#include "value.h"
#include <stdint.h>
 
typedef enum {
    OP_BAR,
    OP_ELIST,
    OP_MAT,
    OP_MLIST,
    OP_N,
    OP_NLIST,
    OP_CONSTANT,
    OP_SEC,
    OP_SLIST,
    OP_LIST_START,
    OP_LIST_END,
    OP_NEGATE,
    OP_ADD,
    OP_SUBSTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_RETURN,
} op_code;

typedef struct {
    size_t count;
    size_t capacity;
    int* lines;
    uint8_t* code;
    value_array* constants;
} chunk;

chunk* new_chunk();
void free_chunk(chunk* pointer);

void add_constant(value val);

void write_chunk(chunk* chunk_to_write, op_code byte, int line);

#endif
