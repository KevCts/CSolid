#include "vm.h"
#include "chunk.h"
#include "compiler.h"
#include "csolid/cvide/coomat.h"
#include "csolid/element.h"
#include "csolid/elements/bar.h"
#include "csolid/material.h"
#include "csolid/model.h"
#include "csolid/node.h"
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


static value pop_nil(value value_if_nil) {
    value val = pop();


    return IS_NIL(val) ? value_if_nil : val;
}

static bool arguments_match(int argc, value_type expected_types[], value_array* arr_to_fill){
    for (int i = argc; i >=1; i--) {
        value_type test = see_value(i).type;
        if (see_value(i).type != expected_types[argc - i])
            return false;
    }
    if (IS_NIL(see_value(argc + 1))) {
        for (int i = 0; i < argc; i++)
            add_value_to_array(NIL_VALUE, arr_to_fill);
        for (int i = 0; i < argc; i++)
            arr_to_fill->values[argc - i - 1] = pop();
        pop();
    } else
        return false;
    return true;
}

static interpret_result run(){
    for (;;) {
        uint8_t instruction = READ_OP_CODE();

        value_array* arguments = new_value_array();

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
                push(OBJ_VALUE(new_list()));
                while (!IS_NIL(see_value(2))) {
                    obj_list* list = (obj_list*)pop().as.object;
                    list_append(list, pop()); 
                    push(OBJ_VALUE((obj*)list));
                }
                obj_list* list = (obj_list*)pop().as.object;
                pop();
                push(OBJ_VALUE((obj*)list));
                break;
            case OP_N:
                if (arguments_match(3, (value_type[]) {TYPE_NUMBER, TYPE_NUMBER, TYPE_NUMBER}, arguments)) {
                    add_node(arguments->values[0].as.number, arguments->values[1].as.number, arguments->values[2].as.number);
                } else {
                    error("Invalid arguments for N");
                }
                break;
            case OP_NLIST:
                list_nodes();
                break;
            case OP_MAT:
                if (arguments_match(3, (value_type[]) {TYPE_NUMBER, TYPE_MAT_PROP, TYPE_NUMBER}, arguments)) {
                    if (!set_material_caracteristic(arguments->values[0].as.number, arguments->values[1].as.material_prop, arguments->values[2].as.number))
                        error("Cannot edit material, material id might not exist or system might not be able to add a material");
                } else if (arguments_match(2, (value_type[]) {TYPE_MAT_PROP, TYPE_NUMBER}, arguments)) {
                    if (!set_material_caracteristic(model.materials_count, arguments->values[0].as.material_prop, arguments->values[1].as.number))
                        error("Cannot create material");
                } else
                    error("Invalid arguments for MAT");
                break;
            case OP_MLIST:
                list_materials();
                break;
            case OP_SEC:
                if (arguments_match(3, (value_type[]) {TYPE_NUMBER, TYPE_SEC_PARA, TYPE_NUMBER}, arguments)) {
                    if (!set_section_parameter(arguments->values[0].as.number, arguments->values[1].as.section_para, arguments->values[2].as.number))
                        error("Cannot edit section, section id might not exist or system might not be able to add a section");
                } else if (arguments_match(2, (value_type[]) {TYPE_SEC_PARA, TYPE_NUMBER}, arguments)) {
                    if (!set_section_parameter(model.sections_count, arguments->values[0].as.section_para, arguments->values[1].as.number))
                        error("Cannot create section");
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
                if (arguments_match(4, (value_type[]) {TYPE_NUMBER, TYPE_NUMBER, TYPE_NUMBER, TYPE_NUMBER}, arguments))
                    add_bar(arguments->values[0].as.number, arguments->values[1].as.number, arguments->values[2].as.number, arguments->values[3].as.number);
                else if (arguments_match(3, (value_type[]) {TYPE_NUMBER, TYPE_NUMBER, TYPE_NUMBER}, arguments))
                    add_bar(arguments->values[0].as.number, arguments->values[1].as.number, arguments->values[2].as.number, model.sections_count);
                else if (arguments_match(2, (value_type[]) {TYPE_NUMBER, TYPE_NUMBER}, arguments))
                    add_bar(arguments->values[0].as.number, arguments->values[1].as.number, model.materials_count, model.sections_count);
                else
                    error("Invalid arguments for BAR");
                break;
            case OP_BOUND:
                if (arguments_match(3, (value_type[]) {TYPE_NUMBER, TYPE_DIR, TYPE_NUMBER}, arguments))
                    set_boundary(arguments->values[0].as.number, arguments->values[1].as.dir, arguments->values[2].as.number);
                else if (arguments_match(2, (value_type[]) {TYPE_NUMBER, TYPE_DIR}, arguments))
                    set_boundary(arguments->values[0].as.number, arguments->values[1].as.dir, 0);
                else if (arguments_match(2, (value_type[]){TYPE_NUMBER, TYPE_OBJECT}, arguments)){
                    if (arguments->values[1].as.object->type == OBJ_LIST) {
                        obj_list* list = (obj_list*)arguments->values[1].as.object;
                        for (int i = 0; i < list->size; i++) {
                            set_boundary(arguments->values[0].as.number, list->list->values[i].as.dir, 0);
                        }
                    } else
                        error("Invalid arguments for BOUND");
                }
                else if (arguments_match(1, (value_type[]){TYPE_OBJECT}, arguments)){
                    if (arguments->values[0].as.object->type == OBJ_LIST) {
                        obj_list* list = (obj_list*)arguments->values[0].as.object;
                        for (int i = 0; i < list->size; i++) {
                            for (int j = 0; j < 6; j++)
                                set_boundary(list->list->values[i].as.number, j, 0);
                        }
                    } else
                        error("Invalid arguments for BOUND");
                }
                else if (arguments_match(1, (value_type[]) {TYPE_NUMBER}, arguments)) {
                    for (int j = 0; j < 6; j++)
                        set_boundary(arguments->values[0].as.number, j, 0);
                }
                else
                    error("Invalid arguments for BOUND");
                break;
            case OP_FORCE:
                if (arguments_match(3, (value_type[]) {TYPE_NUMBER, TYPE_DIR, TYPE_NUMBER}, arguments))
                    set_force(arguments->values[0].as.number, arguments->values[1].as.dir, arguments->values[2].as.number);
                else
                    error("Invalid arguments for FORCE");
                break;
            case OP_SOLVE:
                solve();
                break;
            case OP_LDISP:
                print_coomat(coomat_from_array(6 * model.nodes_count, 1, model.displacements));
                printf("\n");
                break;
            case OP_LREAC:
                print_coomat(coomat_from_array(6 * model.nodes_count, 1, model.reactions));
                printf("\n");
                break;
            case OP_ELIST:
                list_elements();
                break;
            case OP_FLIST:
                list_forces();
                break;
            default:
                return INTERPRET_RUNTIME_ERROR;
        }
        free_value_array(arguments);
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
