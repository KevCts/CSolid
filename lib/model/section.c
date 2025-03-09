#include "section.h"
#include "model.h"
#include <stdlib.h>

void add_section(){
    if (model.sections_count + 1 > model.sections_capacity) {
        model.sections = realloc(model.sections, (++model.sections_capacity) * sizeof(section));
    }
}

bool set_section_parameter(size_t id, section_parameter sp, double value) {
    if (id >  model.sections_count)
        return false;
    else if (id == model.sections_count)
        add_section();
    model.sections[id].parameters[sp] = value;
    return true;
}

void list_sections() {
    for (size_t i = 0; i < model.sections_count; i++) {
        printf("|\t Section : %ld\t|\tA = %lf\t|\n", i, model.sections[i].parameters[SEC]);
    }
}
