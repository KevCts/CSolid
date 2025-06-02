#include "vm.h"
#include "chunk.h"
#include "compiler.h"
#include "csolid/element.h"
#include "csolid/elements/bar.h"
#include "csolid/material.h"
#include "csolid/section.h"
#include "value.h"
#include "object.h"

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#define READ_OP_CODE() (*vm.ip++)
#define POP_CONSTANT() (vm.code->constants->values[READ_OP_CODE()])

virtual_machine vm;

void init_vm() {
    vm.stack_top = vm.stack;
    vm.objects = NULL;
}

void free_vm() {
    free_object(vm.objects);
    free_model();
}

static void error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    vm.stack_top = vm.stack;
}

static void push(value val) {
    *(vm.stack_top++) = val;
}

static value pop() {
    return *(--vm.stack_top);
}

static value see_value(int i) {
    return *(vm.stack_top - i);
}


static value see_next_next_value() {
    return *(vm.stack_top - 2);
}

static value pop_nil(value value_if_nil) {
    value val = pop();


    return IS_NIL(val) ? value_if_nil : val;
}

static int next_nil() {
    int res = 0;
    while (!IS_NIL(see_value(++res))) {}
    return res;
}

static interpret_result run(){
    for (;;) {
        uint8_t instruction = READ_OP_CODE();

        switch (instruction) {
            case OP_CONSTANT:
                push(POP_CONSTANT());
                break;
            case OP_NEGATE:
                if (IS_NUMBER(see_value(1)))
                    push(NUMBER_VALUE(-1 * pop().as.number));
                break;
            case OP_ADD:
                if (IS_NUMBER(see_value(1)) && IS_NUMBER(see_value(2)))
                    push(NUMBER_VALUE(pop().as.number + pop().as.number));
                break;
            case OP_SUBSTRACT:
                if (IS_NUMBER(see_value(1)) && IS_NUMBER(see_value(2)))
                    push(NUMBER_VALUE(pop().as.number - pop().as.number));
                break;
            case OP_MULTIPLY:
                if (IS_NUMBER(see_value(1)) && IS_NUMBER(see_value(2)))
                    push(NUMBER_VALUE(pop().as.number * pop().as.number));
                break;
            case OP_DIVIDE:
                if (IS_NUMBER(see_value(1)) && IS_NUMBER(see_value(2)))
                    push(NUMBER_VALUE(pop().as.number / pop().as.number));
                break;
            case OP_LIST_START:
                push(NIL_VALUE);
                break;
            case OP_LIST_END:
                obj_list* list = (obj_list*)new_list();
                while (!IS_NIL(see_value(1)))
                    list_append(list, pop()); 
                push(OBJ_VALUE((obj*)list));
                break;
            case OP_N:
                if (IS_NUMBER(see_value(1)) && IS_NUMBER(see_value(2)) && IS_NUMBER(see_value(3))) {
                    double x, y, z;
                    z = pop().as.number;
                    y = pop().as.number;
                    x = pop().as.number;
                    pop();
                    add_node(x, y, z);
                } else {
                    error("Invalid arguments for N");
                }
                break;
            case OP_NLIST:
                list_nodes();
                break;
            case OP_MAT:
                double prop_value;
                material_caracteristics material_prop;
                if (IS_NUMBER(see_value(1)) && IS_MAT_PROP(see_value(2))) {
                    prop_value = pop().as.number;
                    material_prop = pop().as.material_prop;
                } else
                    error("Invalid arguments for MAT");

                if (IS_NIL(see_value(1)) || IS_NUMBER(see_value(1))) {
                    if (!set_material_caracteristic(pop_nil(NUMBER_VALUE(model.materials_count)).as.number, material_prop, prop_value))
                        error("Invalid cannot edit material");
                } else
                    error("Invalid arguments for MAT");
                if (IS_NIL(see_value(1)))
                    pop();
                break;
            case OP_MLIST:
                list_materials();
                break;
            case OP_SEC:
                double para_value;
                section_parameter sec_para;
                if (IS_NUMBER(see_value(1)) && IS_SEC_PARA(see_value(2))) {
                    para_value = pop().as.number;
                    sec_para = pop().as.section_para;
                } else
                    error("Invalid arguments for SEC");

                if (IS_NIL(see_value(1)) || IS_NUMBER(see_value(1))) {
                    if (!set_section_parameter(pop_nil(NUMBER_VALUE(model.sections_count)).as.number, sec_para, para_value))
                        error("Invalid cannot edit section");
                } else
                    error("Invalid arguments for SEC");
                break;
            case OP_SLIST:
                list_sections();
                break;
            case OP_RETURN:
                if (vm.stack_top != vm.stack) {
                    print_value(pop());
                    printf("\n");
                }
                return INTERPRET_SUCCESS;
            case OP_BAR:
                size_t mat_id = model.materials_count - 1;
                size_t sec_id = model.sections_count - 1;
                switch (next_nil()) {
                    case 5:
                        if (IS_NUMBER(see_value(1))) {
                            mat_id = pop().as.number;
                        } else
                            error("Invalid arguments for BAR");
                    case 4:
                        if (IS_NUMBER(see_value(1))) {
                            sec_id = pop().as.number;
                        } else
                            error("Invalid arguments for BAR");
                    case 2:
                        if (IS_NUMBER(see_value(1)) && IS_NUMBER(see_value(2))) {
                            add_bar(pop().as.number, pop().as.number, sec_id, mat_id);
                        } else
                            error("Invalid arguments for BAR");
                        break;
                    default:
                        error("Invalid arguments for BAR");
                        break;
                }
                if (IS_NIL(see_value(1)))
                    pop();
                break;
            case OP_ELIST:
                list_elements();
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
