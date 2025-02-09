#include <stdlib.h>
#include <stdio.h>
#include "lib/cvide/coomat.h"
#include "model.h"
#include "elements/bar.h"
#include "node.h"
#include "section.h"

Model model;

int main(int argc, char* argv[]) {
    init_model();

    add_node(0., 0, 0);
    add_node(1., 0, 0);

    add_material(210000);

    add_section(1);

    add_bar(0, 1, 0, 0);    

    add_boundary(0, UX, 0);
    add_boundary(0, UY, 0);
    add_boundary(0, UZ, 0);
    add_boundary(0, RX, 0);
    add_boundary(0, RY, 0);
    add_boundary(0, RZ, 0);
    add_boundary(1, UY, 0);
    add_boundary(1, UZ, 0);
    add_boundary(1, RX, 0);
    add_boundary(1, RY, 0);
    add_boundary(1, RZ, 0);

    add_force(1, UX, 210000);

    solve();

    print_coomat(coomat_from_array(12, 1, model.displacements));
    printf("\n");
    print_coomat(coomat_from_array(12, 1, model.reactions));

    free_model();

    return EXIT_SUCCESS;
}
