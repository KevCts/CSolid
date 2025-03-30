#include "model.h"
#include "element.h"
#include "../cvide/coomat.h"
#include "../cvide/solve.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

Model model;

void init_model() {
    model.nodes = NULL;
    model.boundaries = NULL;
    model.displacements = NULL;
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

coomat* build_stiffness() {
    coomat* res = get_element_matrix(0);

    for (size_t i = 1; i < model.elements_count; i++) {
        res = sum_coomat(res, get_element_matrix(i));
    }

    return res;
}

void solve() {
    coomat* stiffness_matrix = build_stiffness();
    coomat* reduced_stiffness = copy_coomat(stiffness_matrix);
    coomat* reduced_forces = coomat_from_array(model.nodes_count * 6, 1, model.forces);

    size_t deleted = 0;
    for (size_t i = 0; i < model.nodes_count * 6; i++) {
        if (model.boundaries[i].is_set) {
            reduced_stiffness = coomat_remove_line(coomat_remove_col(reduced_stiffness, i - deleted), i - deleted);
            reduced_forces = coomat_remove_line(reduced_forces, i - deleted);
            deleted++;
        }
    }

    coomat* reduced_displacements_coomat = minres(reduced_stiffness, reduced_forces, init_coomat(reduced_forces->size_i, reduced_forces->size_j), .00001, 10000);

    double* reduced_displacements = array_from_coomat(reduced_displacements_coomat);

    size_t j = 0; 
    for (size_t i = 0; i < model.nodes_count * 6; i++) {
        if (model.boundaries[i].is_set) {
            model.displacements[i] = model.boundaries[i].value;
        } else {
            model.displacements[i] = reduced_displacements[j++];
        }
    }

    free(reduced_displacements);

    coomat* forces_vector = prod_coomat(stiffness_matrix, coomat_from_array(model.nodes_count * 6, 1, model.displacements));

    model.reactions = array_from_coomat(sum_coomat(forces_vector, dot_coomat(-1, coomat_from_array(model.nodes_count * 6, 1, model.forces))));
}

void free_model() {
    if (model.nodes != NULL) {
        free(model.nodes);
        free(model.forces);
        free(model.displacements);
        free(model.boundaries);
    }

    if (model.reactions != NULL)
        free(model.reactions);

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
