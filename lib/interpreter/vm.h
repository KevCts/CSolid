#ifndef CSOLID_VM_H
#define CSOLID_VM_H

#include <stdint.h>
#include "../model/model.h"
#include "byte_code.h"
#include "compiler.h"

typedef enum {
    INTERPRET_SUCCESS,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} interpret_result;

typedef struct {
    byte_code* code;
    op_code* ip;
} virtual_machine;

void init_vm();
void free_vm();
interpret_result interpret(const char* source);

#endif
