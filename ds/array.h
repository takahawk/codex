#ifndef CDX_ARRAY_H_
#define CDX_ARRAY_H_

#define DEFAULT_CAP 10 
#define CAP_MULTIPLIER 2

#ifdef CDX_DEFENSIVE
  #define CDX_ARRAY_BOUNDS_CHECK 1
#endif

#ifdef CDX_ARRAY_BOUNDS_CHECK
  #include <stdio.h>
  #define CDX_BOUNDS_CHECK(a, i) \
    do { \
      if ((i) >= (a).len) { \
        fprintf(stderr, "Index out of bounds %ld (len=%ld)", i, a.len); \
        abort(); \
      } \
    } while(0);
#else
  #define CDX_BOUNDS_CHECK(arr, i) ((void) 0);
#endif

/* TODO: allocator */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
  void*  data;
  size_t elem_size;
  size_t capacity;
  size_t len;
} Array;

static inline Array cdx_array_form(size_t elem_size) {
  size_t capacity = DEFAULT_CAP;
  void *data = malloc(elem_size * capacity);

  Array array = {
    .data      = data,
    .elem_size = elem_size,
    .capacity  = capacity,
    .len       = 0,
  };

  return array;
}

static inline void* cdx_array_get(Array a, size_t i) {
  CDX_BOUNDS_CHECK(a, i);
  return (char *) a.data + (a.elem_size * i);
}

static inline void cdx_array_set(Array a, size_t i, void* dataptr) {
  CDX_BOUNDS_CHECK(a, i);
  memcpy((char *) a.data + (a.elem_size * i),
         dataptr, 
         a.elem_size);
}

static inline void cdx_array_add(Array *a, void* dataptr) {
  if (a->capacity == a->len) {
    a->capacity *= CAP_MULTIPLIER;
    a->data = realloc(a->data, a->elem_size * a->capacity);
  }

  a->len++;
  cdx_array_set(*a, a->len - 1, dataptr);
}

static inline bool cdx_array_equals(Array a, Array b) {
	if (a.elem_size != b.elem_size)
		return false;
	if (a.len != b.len)
		return false;

	return memcmp(a.data, b.data, a.elem_size * a.len) == 0;
}

static inline void cdx_array_release(Array *ptr) {
  free(ptr->data);
}

#undef DEFAULT_CAP
#undef CAP_MULTIPLIER
#undef CDX_BOUNDS_CHECK

#endif
