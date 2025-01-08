#include "model.h"


void add_node(double x, double y, double z) {
    if (model.nodes_count + 1 > model.nodes_capacity) {
        model.nodes = realloc(model.nodes, (++model.nodes_capacity) * sizeof(Node));
    }

    model.nodes[model.nodes_count].x = x;
    model.nodes[model.nodes_count].y = y;
    model.nodes[model.nodes_count++].z = z;
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
        for (size_t k = 0; k < model.elements_count; k++) {
            if(model.elements[k].n1 == j)
                model.elements[k].n1 = i;
            if(model.elements[k].n2 == j)
                model.elements[k].n2 = i;
            if (model.elements[k].n1 == model.elements[k].n2)
                delete_element(k);
        }

        model.nodes[i] = model.nodes[j];
    }
}

void delete_node(size_t i) {
    if(i < model.nodes_count) {
        for (size_t k = 0; k < model.elements_count; k++) {
            if(model.elements[k].n1 == i || model.elements[k].n2 == i) {
                delete_element(k);
            }
        }
        replace_node(i, model.nodes_count - 1);
        model.nodes_count--;
    }
}
