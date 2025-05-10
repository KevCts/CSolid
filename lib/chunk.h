#ifndef CSOLID_CHUNK_H
#define CSOLID_CHUNK_H

#include "memory.h"
 
typedef enum {
    OP_RETURN,
} op_code;

typedef struct {
    size_t count;
    size_t capacity;
    int* lines;
    op_code* code;
} chunk;

chunk* new_chunk();
void free_chunk(chunk* pointer);

void write_chunk(chunk* chunk_to_write, op_code byte, int line);

#endif
