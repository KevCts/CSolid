#ifndef CSOLID_NODE_H
#define CSOLID_NODE_H

#include <stdlib.h>

typedef struct {
    double x, y, z;
} Node;


void add_node(double, double, double);

void edit_node(size_t, double, double, double);

void list_nodes();

void replace_node(size_t, size_t);

void delete_node(size_t);

#endif
