#ifndef CSOLID_COMPILER_H
#define CSOLID_COMPILER_H

#include <stdbool.h>
#include "chunk.h"

bool compile(const char* source, chunk* code);

#endif
