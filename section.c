#include "section.h"
#include "model.h"
#include <stdlib.h>

void add_section(double A){
    if (model.sections_count + 1 > model.sections_capacity) {
        model.sections = realloc(model.sections, (++model.sections_capacity) * sizeof(section));
    }
    model.sections[model.sections_count++].A = A;
}

void list_sections() {
    for (size_t i = 0; i < model.sections_count; i++) {
        printf("|\t Section : %ld\t|\tA = %lf\t|\n", i, model.sections[i].A);
    }
}
