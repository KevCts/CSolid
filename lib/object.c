#include "object.h"
#include "vm.h"
#include <stdlib.h>
#include <stdio.h>

obj* new_list() {
    obj_list* result = malloc(sizeof(obj_list));
    result->object.type = OBJ_LIST;
    result->object.next = vm.objects;
    vm.objects = (obj*)result;
    result->size = 0;
    result->list = new_value_array();

    return (obj*)result;
}

void free_object(obj* pointer) {
    if (pointer != NULL) {
        switch (pointer->type) {
            case OBJ_LIST:
                free_value_array(((obj_list*)pointer)->list);
                break;
        }
        free_object(pointer->next);
        free(pointer);
    }
}

void list_append(obj_list* list, value val) {
    list->size++;
    add_value_to_array(val, list->list);
}

value list_value(obj_list* list, size_t i) {
    size_t lsize = list->size;
    if(i < lsize)
        return list->list->values[lsize - i - 1];
    return NIL_VALUE;
}

static void print_list(obj_list* list) {
    printf("[");
    size_t lsize = list->size;
    for (size_t i = lsize - 1; i > 0; i--) {
        print_value(list->list->values[i]);
        printf(" ");
    }
    print_value(list->list->values[0]);
    printf("]");
}

void print_object(obj* object) {
    switch (object->type) {
        case OBJ_LIST :
            print_list((obj_list*)object);
            break;
    }
}
