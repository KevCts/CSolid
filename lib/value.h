#ifndef CSOLID_VALUE_H
#define CSOLID_VALUE_H

#include <stddef.h>

typedef struct obj obj;

typedef enum {
    TYPE_NIL,
    TYPE_NUMBER,
    TYPE_OBJECT,
} value_type;

typedef struct {
    value_type type;
    union {
        double number;
        obj* object;
    } as;
} value;

typedef struct {
    size_t count;
    size_t capacity;
    value* values;
} value_array;

#define NIL_VALUE ((value){TYPE_NIL, {.number = 0}})
#define NUMBER_VALUE(num) ((value){TYPE_NUMBER, {.number = (num)}})
#define OBJ_VALUE(obj) ((value){TYPE_OBJECT, {.object = (obj)}})

#define IS_NIL(val) ((val).type == TYPE_NIL)
#define IS_NUMBER(val) ((val).type == TYPE_NUMBER)
#define IS_OBJ(val) ((val).type == TYPE_OBJECT)

value_array* new_value_array();
void free_value_array(value_array* array);
size_t add_value_to_array(value val, value_array* array);
void print_value(value val);

#endif
