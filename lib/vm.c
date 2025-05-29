#include "vm.h"
#include "chunk.h"
#include "compiler.h"
#include "memory.h"
#include "value.h"
#include "object.h"

#include <stdint.h>
#include <stdio.h>

#define READ_OP_CODE() (*vm.ip++)
#define POP_CONSTANT() (vm.code->constants->values[READ_OP_CODE()])

virtual_machine vm;

void init_vm() {
    vm.stack_top = vm.stack;
    vm.objects = NULL;
}

void free_vm() {
    free_object(vm.objects);
}

static void push(value val) {
    *(vm.stack_top++) = val;
    ;
}

static value pop() {
    return *(--vm.stack_top);
}

static value see_next_value() {
    return *(vm.stack_top - 1);
}


static value see_next_next_value() {
    return *(vm.stack_top - 2);
}

static interpret_result run(){
    for (;;) {
        uint8_t instruction = READ_OP_CODE();

        switch (instruction) {
            case OP_CONSTANT:
                push(POP_CONSTANT());
                break;
            case OP_NEGATE:
                if (IS_NUMBER(see_next_value()))
                    push(NUMBER_VALUE(-1 * pop().as.number));
                break;
            case OP_ADD:
                if (IS_NUMBER(see_next_value()) && IS_NUMBER(see_next_next_value()))
                    push(NUMBER_VALUE(pop().as.number + pop().as.number));
                break;
            case OP_SUBSTRACT:
                if (IS_NUMBER(see_next_value()) && IS_NUMBER(see_next_next_value()))
                    push(NUMBER_VALUE(pop().as.number - pop().as.number));
                break;
            case OP_MULTIPLY:
                if (IS_NUMBER(see_next_value()) && IS_NUMBER(see_next_next_value()))
                    push(NUMBER_VALUE(pop().as.number * pop().as.number));
                break;
            case OP_DIVIDE:
                if (IS_NUMBER(see_next_value()) && IS_NUMBER(see_next_next_value()) && see_next_next_value().as.number != 0)
                    push(NUMBER_VALUE(pop().as.number / pop().as.number));
                break;
            case OP_LIST_START:
                push(NIL_VALUE);
                break;
            case OP_LIST_END:
                obj_list* list = (obj_list*)new_list();
                while (!IS_NIL(see_next_value()))
                    list_append(list, pop()); 
                push(OBJ_VALUE((obj*)list));
                break;
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
