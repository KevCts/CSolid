#include "model.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void init_model() {
    model.m = NULL;
    model.c = NULL;
    model.k = NULL;

    model.nodes = NULL;
    model.nodes_count = 0;
    model.nodes_capacity = 0;

    model.elements = NULL;
    model.elements_count = 0;
    model.elements_capacity = 0;
}

void free_model() {
    if (model.nodes != NULL) {
        free(model.nodes);
    }

    if (model.elements != NULL) {
        free(model.elements);
    }
}
