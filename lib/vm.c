#include "vm.h"
#include "compiler.h"
#include <stdio.h>

virtual_machine vm;

void init_vm() {

}

void free_vm() {

}

static interpret_result run(){
    return INTERPRET_SUCCESS;
}

interpret_result interpret(const char* source) {
    chunk* code = new_chunk();
    if (!compile(source, code)) {
        free_chunk(code);
        return INTERPRET_COMPILE_ERROR;
    }
    vm.code = code;
    vm.ip = code->code;
    interpret_result result = run();
    free_chunk(code);
    return result;
}
