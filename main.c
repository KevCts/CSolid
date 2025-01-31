#include <stdlib.h>
#include <stdio.h>
#include "lib/cvide/coomat.h"
#include "model.h"
#include "element.h"
#include "elements/bar.h"

Model model;

int main(int argc, char* argv[]) {
    init_model();

    add_node(1., 0, 0);
    add_node(2., 0, 0);

    list_nodes();

    printf("\n");

    add_material(210000);

    list_materials();

    printf("\n");

    add_section(10);

    list_sections();

    printf("\n");

    add_bar(0, 1, 0, 0);    

    list_elements();

    printf("\n");

    print_coomat(get_element_matrix(0));

    free_model();

    return EXIT_SUCCESS;
}
