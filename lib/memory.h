#ifndef CSOLID_MEMORY_H
#define CSOLID_MEMORY_H

#include <stddef.h>

#define ALLOCATE(type, number)\
    (type*)reallocate(NULL, sizeof(type) * number)
#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)
#define GROW_ARRAY(type, pointer, count)\
    (type *)reallocate(pointer, count * sizeof(type))
#define FREE(pointer) reallocate(pointer, 0)

void* reallocate(void* pointer, size_t size);

#endif
