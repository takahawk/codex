#include "codex/allocators/allocator.h"

#include <stdio.h>
#include <stdlib.h>

static void*
std_allocator_alloc(Allocator *self, size_t size) {
	void* p = malloc(size);

	if (NULL == p) {
		perror("malloc failure");
		exit(EXIT_FAILURE);
	}

	return p;
}

static void*
std_allocator_realloc(Allocator *self, void *p, size_t size) {
	p = realloc(p, size);

	if (NULL == p) {
		perror("realloc failure");
		exit(EXIT_FAILURE);
	}

	return p;
}

static void
std_allocator_free(Allocator *self, void *p) {
	if (NULL == p) {
		perror("attempt to release NULL pointer\n");
		exit(EXIT_FAILURE);
	}

	free(p);
}

const Allocator STD_ALLOCATOR = {
	.ctx = NULL, // unused for std allocation

	.alloc = std_allocator_alloc,
	.realloc = std_allocator_realloc,
	.free = std_allocator_free
};

