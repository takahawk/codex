#ifndef CDX_ARRAY_H_
#define CDX_ARRAY_H_

#define CDX_ARRAY_BASE_CAP 10 
#define CDX_ARRAY_CAP_MULTIPLIER 2

#ifndef CDX_ALLOC
  #include "codex/mem/use_std_allocator.h"
#endif

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

#define cdx_array_form(X) \
  ((Array) {                                       \
   .data      = CDX_ALLOC(X * CDX_ARRAY_BASE_CAP), \
   .elem_size = X,                                 \
   .capacity  = CDX_ARRAY_BASE_CAP,                \
   .len       = 0,                                 \
   })


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

#define cdx_array_add(a, dataptr) \
  do {                                                                  \
  if ((a)->capacity == (a)->len) {                                      \
    (a)->capacity *= CDX_ARRAY_CAP_MULTIPLIER;                          \
    (a)->data = CDX_REALLOC((a)->data, (a)->elem_size * (a)->capacity); \
  }                                                                     \
  cdx_array_set(*(a), (a)->len++, dataptr);                             \
  } while (0);

/**
 * Fast remove - remove without preserving order
 */
static inline void cdx_array_fremove(Array *a, size_t i) {
  CDX_BOUNDS_CHECK(a, i);
	if (a->len != 1) {
		memcpy(a->data + (i * a->elem_size),
			   a->data + ((a->len - 1) * a->elem_size),
			   a->elem_size);
	}

	a->len--;
}

static inline bool cdx_array_equals(Array a, Array b) {
	if (a.elem_size != b.elem_size)
		return false;
	if (a.len != b.len)
		return false;

	return memcmp(a.data, b.data, a.elem_size * a.len) == 0;
}

static inline void cdx_array_sort(Array a, int (*compar) (const void*, const void*)) {
	qsort(a.data, a.len, a.elem_size, compar);
}

#define cdx_array_release(a) \
  CDX_FREE((a)->data); (a)->data = NULL;

#undef CDX_BOUNDS_CHECK

#endif
