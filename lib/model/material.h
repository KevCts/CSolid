#ifndef CSOLID_MATERIAL_H
#define CSOLID_MATERIAL_H

#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    EX = 0, EY, EZ
} material_caracteristics;

typedef struct {
    double parameters[3];
} material;

void add_material();

bool set_material_caracteristic(size_t, material_caracteristics, double);
double get_material_caracteristic(size_t, material_caracteristics);

void list_materials();

#endif
