#ifndef CSOLID_MODEL_H
#define CSOLID_MODEL_H

#include "../cvide/coomat.h"
#include "node.h"
#include "element.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    bool is_set;
    double value;
} boundary;

typedef struct {
    size_t nodes_count, nodes_capacity;
    node* nodes;
    boundary* boundaries;
    double *forces, *reactions, *displacements;
    size_t elements_count, elements_capacity;
    element* elements;
    size_t sections_count, sections_capacity;
    section* sections;
    size_t materials_count, materials_capacity;
    material* materials;
} Model;

extern Model model;

void init_model();
coomat* build_stiffness();
void solve();
void free_model();

#endif
