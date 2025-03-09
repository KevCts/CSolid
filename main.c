#include <stdlib.h>
#include <stdio.h>
#include "lib/cvide/coomat.h"
#include "lib/model/material.h"
#include "lib/model/model.h"
#include "lib/model/elements/bar.h"
#include "lib/model/node.h"
#include "lib/model/section.h"

Model model;

int main(int argc, char* argv[]) {
    init_model();

    add_node(0, 0, 0);
    add_node(0, 1, 0);
    add_node(1, 0, 0);

    set_material_caracteristic(0, EX, 210000);

    set_section_parameter(0, SEC, 1);

    add_bar(0, 2, 0, 0);    
    add_bar(1, 2, 0, 0);    

    add_boundary(0, UX, 0);
    add_boundary(0, UY, 0);
    add_boundary(0, UZ, 0);
    add_boundary(0, RX, 0);
    add_boundary(0, RY, 0);
    add_boundary(0, RZ, 0);
    add_boundary(1, UX, 0);
    add_boundary(1, UY, 0);
    add_boundary(1, UZ, 0);
    add_boundary(1, RX, 0);
    add_boundary(1, RY, 0);
    add_boundary(1, RZ, 0);
    add_boundary(2, UZ, 0);
    add_boundary(2, RX, 0);
    add_boundary(2, RY, 0);
    add_boundary(2, RZ, 0);

    add_force(2, UY, -210000);

    solve();

    print_coomat(coomat_from_array(18, 1, model.displacements));
    printf("\n");
    print_coomat(coomat_from_array(18, 1, model.reactions));

    free_model();

    return EXIT_SUCCESS;
}
