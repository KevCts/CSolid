#include "bar.h"
#include "../model.h"
#include <stdio.h>
#include <stdlib.h>

bar* create_bar(node* n1, node* n2, section* s, material* m) {
    if (distance(n1, n2) > 0) {
        bar* res = malloc(sizeof(bar));
        res->n1 = n1;
        res->n2 = n2;
        res->sec = s;
        res->mat = m;
        return res;
    }
    return NULL;
}

coomat* bar_stiffness_matrix(bar* b){
    // Define the traction/compression stiffness
    double h = b->mat->Ex * b->sec->A / distance(b->n1, b->n2);
    // Define 1D stiffness matrix
    coomat* res1D = init_coomat(2, 2);
    coomat_set_value(res1D, 0, 0, h);
    coomat_set_value(res1D, 1, 0, -h);
    coomat_set_value(res1D, 0, 1, -h);
    coomat_set_value(res1D, 1, 1, h);
    // Build 1D->3D transition matrix
    coomat* transiMat = init_coomat(12, 2);
    coomat_set_value(transiMat, 0, 0, (b->n2->x - b->n1->x)/distance(b->n1, b->n2));
    coomat_set_value(transiMat, 0, 1, (b->n2->y - b->n1->y)/distance(b->n1, b->n2));
    coomat_set_value(transiMat, 0, 2, (b->n2->z - b->n1->z)/distance(b->n1, b->n2));
    coomat_set_value(transiMat, 1, 6, (b->n2->x - b->n1->x)/distance(b->n1, b->n2));
    coomat_set_value(transiMat, 1, 7, (b->n2->y - b->n1->y)/distance(b->n1, b->n2));
    coomat_set_value(transiMat, 1, 8, (b->n2->z - b->n1->z)/distance(b->n1, b->n2));
    // 1D->3D transition
    coomat* res3D = prod_coomat(prod_coomat(copy_coomat(transiMat), res1D), transpose_coomat(transiMat));
    coomat* res = init_coomat(model.nodes_count * 6, model.nodes_count * 6);
    for(size_t i = 0; i < 6; i++) {
        for(size_t j = 0; j < 6; j++) {
            coomat_set_value(res, 3 * b->n1->id + i, 3 * b->n1->id + j, coomat_read_value(res3D, i, j));
            coomat_set_value(res, 3 * b->n2->id + i, 3 * b->n1->id + j, coomat_read_value(res3D, i, j));
            coomat_set_value(res, 3 * b->n1->id + i, 3 * b->n2->id + j, coomat_read_value(res3D, i, j));
            coomat_set_value(res, 3 * b->n2->id + i, 3 * b->n2->id + j, coomat_read_value(res3D, i, j));
        }
    }
    free_coomat(res3D);
    return res;
}

bool bar_replace_node(bar* b, node* n_old, node* n_new){
    if (b->n1 == n_old) {
        b->n1 = n_new;
    } else if (b->n2 == n_old) {
        b->n2 = n_new;
    }

    return distance(b->n1, b->n2) > 0;
}

void add_bar(size_t i, size_t j, size_t sec, size_t mat) {
    node* n1 = model.nodes + i;
    node* n2 = model.nodes + j;
    section* s = model.sections + sec;
    material* m = model.materials + mat;

    if (model.elements_count + 1 > model.elements_capacity) {
        model.elements = realloc(model.elements, (++model.elements_capacity) * sizeof(element));
    }
    model.elements[model.elements_count].type = BAR;
    model.elements[model.elements_count++].content.b = create_bar(n1, n2, s, m);
}
