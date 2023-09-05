#include <stdlib.h>

#include "memory.h"

// manage memory for clox
void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }

  void* result = realloc(pointer, newSize);
  if (result == NULL) exit(1); // allocation failed
  return result;
}
