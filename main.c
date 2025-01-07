#include <stdlib.h>
#include <stdio.h>
#include "model.h"

Model model;

int main(int argc, char* argv[]) {
    init_model();

    add_node(1., 2.12456,  0);

    list_nodes();

    return EXIT_SUCCESS;
}
