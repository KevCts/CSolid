#ifndef CSOLID_ELEMENT_H
#define CSOLID_ELEMENT_H

#include "lib/cvide/coomat.h"
#include "elements/bar.h"
#include <stdlib.h>

typedef enum {BAR} etype;

typedef struct {
    etype type;
    union {
        bar* b;
    } content;
} element;

void add_element(element);

void list_elements();

void delete_element(size_t);

bool element_replace_node(element*, node*, node*);

coomat* get_element_matrix(size_t);

#endif
