#include <stdlib.h>
#include <stdio.h>
#include "model.h"

Model model;

int main(int argc, char* argv[]) {
    init_model();

    add_node(1., 0, 0);
    add_node(2., 0, 0);
    add_node(3., 0, 0);
    add_node(4., 0, 0);
    add_node(5., 0, 0);
    add_node(6., 0, 0);

    list_nodes();

    printf("\n");

    add_element(0, 1);
    add_element(1, 2);
    add_element(2, 3);
    add_element(3, 4);
    add_element(4, 5);

    list_elements();

    delete_node(3);
    list_nodes();

    printf("\n");

    list_elements();

    free_model();

    return EXIT_SUCCESS;
}
