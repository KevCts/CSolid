#include "material.h"
#include "model.h"
#include <stdlib.h>

void add_material(double Ex){
    if (model.materials_count + 1  > model.materials_capacity) {
        model.materials = realloc(model.materials, (++model.materials_capacity) * sizeof(material));
    }
    model.materials[model.materials_count++].Ex = Ex;
}

void list_materials() {
    for (size_t i = 0; i < model.materials_count; i++) {
        printf("|\t Material : %ld\t|\tEx = %lf\t|\n", i, model.materials[i].Ex);
    }
}
