#include "vm.h"
#include "chunk.h"
#include "compiler.h"
#include "memory.h"
#include "value.h"

#include <stdint.h>
#include <stdio.h>

#define READ_OP_CODE() (*vm.ip++)
#define POP_CONSTANT() (vm.code->constants->values[READ_OP_CODE()])

virtual_machine vm;

void init_vm() {
    vm.stack_top = vm.stack;
}

void free_vm() {

}

static void push(value val) {
    *(vm.stack_top++) = val;
    ;
}

static value pop() {
    return *(--vm.stack_top);
}

static interpret_result run(){
    for (;;) {
        uint8_t instruction = READ_OP_CODE();

        switch (instruction) {
            case OP_CONSTANT:
                push(POP_CONSTANT());
                break;
            case OP_NEGATE:
                value val = POP_CONSTANT();
                if (IS_NUMBER(val))
                    push(NUMBER_VALUE(-1 * val.as.number));
                break;
            case OP_ADD:
            case OP_SUBSTRACT:
            case OP_MULTIPLY:
            case OP_DIVIDE:
            case OP_RETURN:
                print_value(pop());
                printf("\n");
                return INTERPRET_SUCCESS;
            default:
                return INTERPRET_RUNTIME_ERROR;
        }
    }
}

interpret_result interpret(const char* source) {
    chunk* code = new_chunk();
    if (!compile(source, code)) {
        free_chunk(code);
        return INTERPRET_COMPILE_ERROR;
    }
    vm.code = code;
    vm.ip = vm.code->code;

    interpret_result result = run();

    free_chunk(code);
    return result;
}
