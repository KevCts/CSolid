#include "model.h"
#include "element.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void init_model() {
    model.m = NULL;
    model.c = NULL;
    model.k = NULL;

    model.nodes = NULL;
    model.boundaries = NULL;
    model.forces = NULL;
    model.nodes_count = 0;
    model.nodes_capacity = 0;

    model.elements = NULL;
    model.elements_count = 0;
    model.elements_capacity = 0;

    model.sections = NULL;
    model.sections_count = 0;
    model.sections_capacity = 0;

    model.materials = NULL;
    model.materials_count = 0;
    model.materials_capacity = 0;
}


void free_model() {
    if (model.nodes != NULL) {
        free(model.nodes);
        free(model.forces);
        free(model.boundaries);
    }

    if (model.elements != NULL) {
        for(size_t i = 0; i < model.elements_count; i++) {
            delete_element(i);
        }

        free(model.elements);
    }

    if (model.sections != NULL) {
        free(model.sections);
    }

    if (model.materials != NULL) {
        free(model.materials);
    }
}
