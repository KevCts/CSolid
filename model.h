#ifndef CSOLID_MODEL_H
#define CSOLID_MODEL_H

#include "lib/cvide/coomat.h"
#include "node.h"
#include "element.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    coomat *m, *c, *k;
    size_t nodes_count, nodes_capacity;
    Node* nodes;
    size_t elements_count, elements_capacity;
    Element* elements;
} Model;

extern Model model;

void init_model();
void free_model();

#endif
