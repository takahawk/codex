#ifndef CODEX_ITERATOR
#define CODEX_ITERATOR
#include <stdbool.h>

#include "codex/allocators/allocator.h"

typedef bool (*FilterFunc) (void *x, void *ctx);

typedef struct Iterator Iterator;
typedef struct Iterator {
	Allocator *a;
	void      *ctx;

	bool  (*has_next) (Iterator *self);
	void* (*next)     (Iterator *self);
	void  (*release)  (Iterator **pself);

	// basic functions - prototype implementation is not supposed to be overriden
	Iterator* (*filter) (Iterator *self, FilterFunc ff, void *ctx);
} Iterator;

Iterator* iterator_filter(Iterator *self, FilterFunc ff, void *ctx);

extern const Iterator ITERATOR_PROTOTYPE;

#endif 