#ifndef CSOLID_OBJECT_H
#define CSOLID_OBJECT_H

#include "value.h"

typedef enum {
    OBJ_LIST,
} object_type;

struct obj {
    object_type type;
    struct obj* next;
};

typedef struct {
    obj object;
    size_t size;
    value_array* list;
} obj_list;

obj* new_list();

void free_object(obj* pointer);

void list_append(obj_list* list, value val);

void print_object(obj* object);

#endif
