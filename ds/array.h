#ifndef CODEX_ARRAY_H_
#define CODEX_ARRAY_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "codex/mem/allocator.h"
#include "codex/mem/release_cb.h"
#include "codex/encoding/serializer.h"


typedef struct Array Array;

struct Array {
	Allocator *a;
	uint8_t *data;
	size_t len;
	size_t cap;
	size_t elem_size;
	ReleaseCb release_cb;

	// add value to array. pval is an address of value to be copied to the array
	void  (*add)     (Array *self, void *pval);
	void* (*get)     (Array *self, size_t i);
	// sets value of array. value on pval address will be copied
	void  (*set)     (Array *self, size_t i, void *pval);
	// fast remove without preserving order - element is just replaced by last one
	void  (*fremove) (Array *self, size_t i);

	bool  (*equals)  (Array *self, Array *other);

	void  (*sort)    (Array *self, int (*compar) (const void*, const void*));

	void  (*release) (Array **pself);

	Serializer* (*form_serializer) (Allocator *a, Serializer *item_serializer);
};

struct _ArrayStatic {
  Array prototype;

  Array* (*form) (Allocator *a, size_t elem_size);
};

extern const struct _ArrayStatic ARRAY;

#endif
