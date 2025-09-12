#ifndef CDX_STRING_H_
#define CDX_STRING_H_

#include <string.h>

#ifndef CDX_ALLOC
  #include "codex/mem/use_std_allocator.h"
#endif

/**
 * Creates a copy of string, allocating memory for it using generic CDX_ALLOC
 */
static inline char* cdx_strdup(const char* str) {
  // TODO: defensive-only guards
  size_t len = strlen(str);
  char *cpy = CDX_ALLOC(len + 1);
  memcpy(cpy, str, len + 1);
  return cpy;
}

#endif
