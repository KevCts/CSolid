#include "model.h"

void add_element(size_t n1, size_t n2){
    if (n1 < model.nodes_count && n2 < model.nodes_count && n1 != n2) {
        if (model.elements_count + 1 > model.elements_capacity) {
            model.elements = realloc(model.elements, (++model.elements_capacity) * sizeof(Element));
        }

        model.elements[model.elements_count].n1 = n1;
        model.elements[model.elements_count++].n2 = n2;
    }
}

void edit_element(size_t i, size_t n1, size_t n2) {
    if (n1 < model.nodes_count && n2 < model.nodes_count && model.elements_count > i && n1 != n2) {
        model.elements[i].n1 = n1;
        model.elements[i].n2 = n2;
    }
}

void list_elements() {
    for (size_t i = 0; i < model.elements_count; i++) {
        printf("|\t Element : %ld\t|\tNode 1 = %ld\t|\tNode 2 = %ld\t|\n", i, model.elements[i].n1, model.elements[i].n2);
    }
}

void delete_element(size_t i) {
    if(i < model.elements_count) {
        model.elements[i] = model.elements[model.elements_count - 1];
        model.elements_count--;
    }
}
