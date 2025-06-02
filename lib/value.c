#include "value.h"
#include "memory.h"
#include "object.h"
#include <stdio.h>
#include <stdlib.h>

value_array* new_value_array() {
    value_array* result = malloc(sizeof(value_array));
    result->count = 0;
    result->capacity = 0;
    result->values = NULL;
    return result;
}

void free_value_array(value_array* array) {
    FREE(array->values);
    FREE(array);
}

size_t add_value_to_array(value val, value_array* array) {
    if (array->count + 1 > array->capacity) {
        array->capacity = GROW_CAPACITY(array->capacity);
        array->values = GROW_ARRAY(value, array->values, array->capacity);
    }
    array->values[array->count] = val;
    return array->count++;
}

void print_value(value val) {
    switch (val.type) {
        case TYPE_NIL :
            printf("NIL");
            break;
        case TYPE_NUMBER :
            printf("%g", val.as.number);
            break;
        case TYPE_OBJECT :
            print_object(val.as.object);
            break;
        case TYPE_MAT_PROP :
            switch (val.as.material_prop) {
                case EX:
                    printf("EX");
                    break;
                case EY:
                    printf("EY");
                    break;
                case EZ:
                    printf("EZ");
                    break;
            }
    }
}
