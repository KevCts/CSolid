#ifndef CSOLID_VALUE_H
#define CSOLID_VALUE_H

#include <stddef.h>

typedef enum {
    TYPE_NUMBER,
} value_type;

typedef struct {
    value_type type;
    union {
        double number;
    } as;
} value;

typedef struct {
    size_t count;
    size_t capacity;
    value* values;
} value_array;

#define NUMBER_VALUE(num) ((value){TYPE_NUMBER, {.number = (num)}})
#define IS_NUMBER(val) ((val).type == TYPE_NUMBER);

value_array* new_value_array();
void free_value_array(value_array* array);
size_t add_value_to_array(value val, value_array* array);
void print_value(value val);

#endif
