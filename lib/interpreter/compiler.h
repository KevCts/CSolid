#ifndef CSOLID_COMPILER
#define CSOLID_COMPILER

#include <stdbool.h>
#include "byte_code.h"

bool compile(const char* source, byte_code* code);

#endif
