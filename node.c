#include "node.h"
#include "model.h"


void add_node(double x, double y, double z) {
    if (model.nodes_count + 1 > model.nodes_capacity) {
        model.nodes = realloc(model.nodes, (++model.nodes_capacity) * sizeof(node));
        model.nodes = realloc(model.forces, (++model.nodes_capacity) * sizeof(double));
        model.nodes = realloc(model.boundaries, (model.nodes_capacity) * 6 * sizeof(boundary));
    }

    model.nodes[model.nodes_count].id = model.nodes_count;
    model.nodes[model.nodes_count].x = x;
    model.nodes[model.nodes_count].y = y;
    model.nodes[model.nodes_count++].z = z;
}

void add_boundary(size_t i, direction dir, double value) {
    if (i < model.nodes_count) {
        model.boundaries[6 * i + dir].is_set = true;
        model.boundaries[6 * i + dir].value = value;
    }
}

void add_force(size_t i, direction dir, double value) {
    if (i < model.nodes_count) {
        model.forces[6 * i + dir] = value;
    }
}

void edit_node(size_t i, double x, double y, double z) {
    if (model.nodes_count > i) {
        model.nodes[i].x = x;
        model.nodes[i].y = y;
        model.nodes[i].z = z;
    }
}

void list_nodes() {
    for (size_t i = 0; i < model.nodes_count; i++) {
        printf("|\t Node : %ld\t|\tx = %lf\t|\ty = %lf\t|\tz = %lf\t|\n", i, model.nodes[i].x, model.nodes[i].y, model.nodes[i].z);
    }
}

void replace_node(size_t i, size_t j) {
    if(i < model.nodes_count && j < model.nodes_count) {
        node* n_old = model.nodes + i;
        node* n_new = model.nodes + j;

        for (size_t k = 0; k < model.elements_count; k++) {
            element_replace_node(model.elements + k, n_old, n_new);
        }

        model.nodes[i] = model.nodes[j];
        model.nodes[i].id = i;
    }
}

void delete_node(size_t i) {
    if(i < model.nodes_count) {
        replace_node(i, model.nodes_count - 1);
        model.nodes_count--;
    }
}

double distance(node* a, node* b) {
    return ((a->x - b->x) * (a->x - b->x)) + ((a->y - b->y) * (a->y - b->y)) + ((a->z - b->z) * (a->z - b->z));
}
