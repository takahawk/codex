#ifndef CODEX_ALLOCATOR_H_
#define CODEX_ALLOCATOR_H_

#include <stddef.h>

typedef struct Allocator {
	void *ctx;

	void* (*const alloc)   (struct Allocator *self, size_t size);
	void* (*const realloc) (struct Allocator *self, void *ptr, size_t size);
	void  (*const free)    (struct Allocator *self, void *ptr); 

} Allocator;

Allocator form_std_allocator();
#endif