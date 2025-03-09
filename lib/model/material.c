#include "material.h"
#include "model.h"
#include <stdlib.h>

void add_material(){
    if (model.materials_count + 1  > model.materials_capacity) {
        model.materials = realloc(model.materials, (++model.materials_capacity) * sizeof(material));
    }
}

bool set_material_caracteristic(size_t id, material_caracteristics mc, double value) {
    if (id >  model.materials_count)
        return false;
    else if (id == model.materials_count)
        add_material();
    model.materials[id].parameters[mc] = value;
    return true;
}

double get_material_caracteristic(size_t id, material_caracteristics mc) {
    if (id < model.materials_count)
        return model.materials[id].parameters[mc];
    else
        return 0;
}

void list_materials() {
    for (size_t i = 0; i < model.materials_count; i++) {
        printf("|\t Material : %ld\t|\tEx = %lf\t|\n", i, model.materials[i].parameters[0]);
    }
}
