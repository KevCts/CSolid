#ifndef CSOLID_VM_H
#define CSOLID_VM_H

#include "csolid/model.h"
#include "chunk.h"

typedef enum {
    INTERPRET_SUCCESS,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} interpret_result;

typedef struct {
    chunk* code;
    uint8_t* ip;
} virtual_machine;

void init_vm();
void free_vm();
void push(value* val);
interpret_result interpret(const char* source);

#endif
