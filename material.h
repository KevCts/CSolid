#ifndef CSOLID_MATERIAL_H
#define CSOLID_MATERIAL_H

typedef struct {
    double Ex;
} material;

void add_material(double);
void list_materials();

#endif
