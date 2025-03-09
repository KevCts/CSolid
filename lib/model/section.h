#ifndef CSOLID_SECTION_H
#define CSOLID_SECTION_H

#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    SEC = 0,
} section_parameter;

typedef struct {
    double parameters[1];
} section;

void add_section();
bool set_section_parameter(size_t id, section_parameter sp, double value);
void list_sections();

#endif
