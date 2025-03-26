#ifndef CODEX_ITERATOR
#define CODEX_ITERATOR
#include <stdbool.h>

typedef struct Iterator Iterator;

typedef struct Iterator {
	void *ctx;

	bool  (*has_next) (Iterator *self);
	void* (*next)     (Iterator *self);
	void  (*release)  (Iterator **pself);
} Iterator;

#endif 