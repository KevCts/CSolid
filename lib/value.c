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

void reinit_value_array(value_array* pointer) {
    pointer->count = 0;
    pointer->capacity = 0;
    FREE(pointer->values);
    pointer->values = NULL;
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
            break;
        case TYPE_DIR:
            switch (val.as.dir) {
                case UX:
                    printf("UX");
                    break;
                case UY:
                    printf("UY");
                    break;
                case UZ:
                    printf("UZ");
                    break;
                case RX:
                    printf("RX");
                    break;
                case RY:
                    printf("RY");
                    break;
                case RZ:
                    printf("RZ");
                    break;
            }
            break;
        case TYPE_SEC_PARA:
            switch (val.as.section_para) {
                case SEC:
                    printf("A");
                    break;
            }
    }
}
