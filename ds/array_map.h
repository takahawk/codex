#ifndef ARRAY_MAP_H_
#define ARRAY_MAP_H_

#include "string.h"

#include "codex/ds/array.h"
#include "codex/cdx_string.h"

/**
 * aka Poor Man's Hashmap. String keys
 */
typedef struct {
  Array keys;
  Array values;
} ArrayMap;

static inline ArrayMap cdx_array_map_form(size_t val_elem_size) {
  return (ArrayMap) {
    .keys   = cdx_array_form(sizeof(char*)),
    .values = cdx_array_form(val_elem_size),
  };
}

#define cdx_array_map_add(am, key, value) \
  do {                                           \
    size_t keylen = strlen(key);                 \
    size_t vallen = strlen(value);               \
    char *map_key = cdx_strdup(key);             \
    cdx_array_add(&(am)->keys, &map_key);        \
    cdx_array_add(&(am)->values, &value);        \
  } while(0);

static inline void* cdx_array_map_get(ArrayMap am, char *key) {
  // TODO: _Generic get?
  for (int i = 0; i < am.keys.len; i++) {
    char *map_key = *(char **) cdx_array_get(am.keys, i);
    if (strcmp(map_key, key) == 0) {
      return cdx_array_get(am.values, i);
    }
  }
  
  // TODO: exit?
  return NULL;
}

static inline void cdx_array_map_release(ArrayMap *am) {
  // TODO: free keys and values
  cdx_array_release(&am->keys);
  cdx_array_release(&am->values);
}

#endif
