#ifndef CSOLID_MODEL_H
#define CSOLID_MODEL_H

#include "lib/cvide/coomat.h"
#include "node.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    coomat *m, *c, *k;
    size_t nodes_count, nodes_capacity;
    Node* nodes;
} Model;

extern Model model;

void init_model();

bool add_node(double, double, double);

bool edit_node(size_t, double, double, double);

void list_nodes();

#endif
