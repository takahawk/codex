#ifndef ARRAY_MAP_H_
#define ARRAY_MAP_H_

#include "string.h"

#include "codex/ds/array.h"


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
/* TODO: add utility codex/string.h header for this common cases */ \
    char *map_key = CDX_ALLOC(keylen + 2);       \
    strcpy(map_key, key);                        \
    char *map_value = CDX_ALLOC(vallen + 2);     \
    strcpy(map_value, value);                    \
    cdx_array_add(am->keys, &key);               \
    cdx_array_add(am->values, &value);           \
  } while(0);

static inline void cdx_array_map_release(ArrayMap *am) {
  // TODO: free keys and values
  cdx_array_release(am->keys);
  cdx_array_release(am->values);
}

