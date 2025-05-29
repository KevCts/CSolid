#ifndef CSOLID_VM_H
#define CSOLID_VM_H

#include "csolid/model.h"
#include "chunk.h"

#define STACK_MAX 256

typedef enum {
    INTERPRET_SUCCESS,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} interpret_result;

typedef struct {
    chunk* code;
    uint8_t* ip;

    value stack[STACK_MAX];
    value* stack_top;

    obj* objects;
} virtual_machine;

extern virtual_machine vm;

void init_vm();
void free_vm();
interpret_result interpret(const char* source);

#endif
