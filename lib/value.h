#ifndef CSOLID_VALUE_H
#define CSOLID_VALUE_H

#include <stddef.h>
#include "csolid/material.h"
#include "csolid/section.h"
#include "csolid/node.h"

typedef struct obj obj;

typedef enum {
    TYPE_NIL,
    TYPE_DIR,
    TYPE_MAT_PROP,
    TYPE_SEC_PARA,
    TYPE_NUMBER,
    TYPE_OBJECT,
} value_type;

typedef struct {
    value_type type;
    union {
        double number;
        direction dir;
        material_caracteristics material_prop;
        section_parameter section_para;
        obj* object;
    } as;
} value;

typedef struct {
    size_t count;
    size_t capacity;
    value* values;
} value_array;

#define NIL_VALUE ((value){TYPE_NIL, {.number = 0}})
#define DIR_VALUE(val) ((value){TYPE_DIR, {.dir = (val)}})
#define NUMBER_VALUE(num) ((value){TYPE_NUMBER, {.number = (num)}})
#define MAT_PROP_VALUE(prop) ((value){TYPE_MAT_PROP, {.material_prop = (prop)}})
#define SEC_PARA_VALUE(prop) ((value){TYPE_SEC_PARA, {.section_para = (prop)}})
#define OBJ_VALUE(obj) ((value){TYPE_OBJECT, {.object = (obj)}})

#define IS_NIL(val) ((val).type == TYPE_NIL)
#define IS_DIR(val) ((val).type == TYPE_DIR)
#define IS_NUMBER(val) ((val).type == TYPE_NUMBER)
#define IS_MAT_PROP(val) ((val).type == TYPE_MAT_PROP)
#define IS_SEC_PARA(val) ((val).type == TYPE_SEC_PARA)
#define IS_OBJ(val) ((val).type == TYPE_OBJECT)

value_array* new_value_array();
void reinit_value_array(value_array* pointer);
void free_value_array(value_array* array);
size_t add_value_to_array(value val, value_array* array);
void print_value(value val);

#endif
