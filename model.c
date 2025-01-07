#include "model.h"
#include <stdio.h>
#include <stdlib.h>

void init_model() {
    model.m = NULL;
    model.c = NULL;
    model.k = NULL;

    model.nodes = NULL;
    model.nodes_count = 0;
    model.nodes_capacity = 0;
}

bool add_node(double x, double y, double z) {
    if (model.nodes_count + 1 > model.nodes_capacity) {
        model.nodes = realloc(model.nodes, (++model.nodes_capacity) * sizeof(Node));
        if (model.nodes == NULL)
            return false;
    }

    model.nodes[model.nodes_count].x = x;
    model.nodes[model.nodes_count].y = y;
    model.nodes[model.nodes_count++].z = z;
    return true;
}

bool edit_node(size_t i, double x, double y, double z) {
    if (model.nodes_count > i) {
        model.nodes[i].x = x;
        model.nodes[i].y = y;
        model.nodes[i].z = z;
        return true;
    }
    return false;
}

void list_nodes() {
    for (size_t i = 0; i < model.nodes_count; i++) {
        printf("|\t Node : %ld\t|\tx = %lf\t|\ty = %lf\t|\tz = %lf\t|\n", i, model.nodes[i].x, model.nodes[i].y, model.nodes[i].z);
    }
}
