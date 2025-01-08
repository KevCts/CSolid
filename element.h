#ifndef CSOLID_ELEMENT_H
#define CSOLID_ELEMENT_H

#include <stdlib.h>

typedef struct {
    size_t n1, n2;
} Element;

void add_element(size_t, size_t);

void edit_element(size_t, size_t, size_t);

void list_elements();

void delete_element(size_t);

#endif
