#include "chunk.h"
#include <stdlib.h>

chunk* new_chunk(){
    chunk* result = malloc(sizeof(chunk));
    if (result == NULL) exit(1);
    result->capacity = 0;
    result->count = 0;
    result->lines = NULL;
    result->code = NULL;
    return result;
}

void free_chunk(chunk* pointer){
    FREE(pointer->code);
    FREE(pointer->lines);
    free(pointer);
}

void write_chunk(chunk* chunk_to_write, op_code byte, int line) {
    if (chunk_to_write->capacity < chunk_to_write->count + 1) {
        chunk_to_write->capacity = GROW_CAPACITY(chunk_to_write->capacity);
        chunk_to_write->code = GROW_ARRAY(uint8_t, chunk_to_write->code, chunk_to_write->capacity);
        chunk_to_write->lines = GROW_ARRAY(int, chunk_to_write->lines, chunk_to_write->capacity);
    }
    chunk_to_write->count++;
    chunk_to_write->code[chunk_to_write->count] = byte;
    chunk_to_write->lines[chunk_to_write->count] = line;
}
