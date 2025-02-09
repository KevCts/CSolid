#include "elements/bar.h"
#include "lib/cvide/coomat.h"
#include "model.h"

void add_element(element e){
    if (++model.elements_count  > model.elements_capacity) {
        model.elements = realloc(model.elements, (++model.elements_capacity) * sizeof(element));
    }
    model.elements[model.elements_count].type = e.type;
    model.elements[model.elements_count].content = e.content;
}

void list_elements() {
    for (size_t i = 0; i < model.elements_count; i++) {
        printf("|\t Element : %ld\t|\tType = %d\t|\n", i, model.elements[i].type);
    }
}

void delete_element(size_t i) {
    if(i < model.elements_count) {
        switch (model.elements[i].type) {
            case BAR:
                free(model.elements[i].content.b);
                break;
        }
        model.elements[i] = model.elements[model.elements_count - 1];
        model.elements_count--;
    }
}

bool element_replace_node(element* e, node* n_old, node* n_new){
    switch (e->type) {
        case BAR :
            return bar_replace_node(e->content.b, n_old, n_new);
            break;
    }

}

coomat* get_element_matrix(size_t i) {
    switch (model.elements[i].type) {
        case BAR:
            return bar_stiffness_matrix(model.elements[i].content.b);
            break;
    }
}
