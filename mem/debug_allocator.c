#include "codex/mem/debug_allocator.h"

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "codex/ds/array.h"

static void
add_allocation_entry(Allocator *self, void *ptr, size_t size) {
	DebugAllocatorCtx *ctx = self->ctx;

	void *stack[16];
	int stack_size = backtrace(stack, 16);
	DebugAllocationEntry entry = {
		.ptr = ptr,
		.size = size,
		.stack_size = stack_size
	};
	memcpy(entry.stack, stack, sizeof(void*) * stack_size);

	ctx->total_allocated += size;

	ctx->allocations->add(ctx->allocations, &entry);
}

static void
remove_allocation_entry(Allocator *self, void *ptr) {
	DebugAllocatorCtx *ctx = self->ctx;
	Array*/*DebugAllocationEntry*/ al = ctx->allocations;

	for (size_t i = 0; i < al->len; ++i) {
		DebugAllocationEntry *entry = al->get(al, i);
		if (entry->ptr == ptr) {
			ctx->total_freed += entry->size;
			al->fremove(al, i);
			return;
		}
	}

	fprintf(stderr, "attempt to free non-allocated pointer: %p", ptr);
}

static void*
debug_allocator_alloc(Allocator *self, size_t size) {
	DebugAllocatorCtx *ctx = self->ctx;
	Allocator *a = ctx->a;

	void* p = a->alloc(a, size);
	add_allocation_entry(self, p, size);

	return p;
}

static void*
debug_allocator_realloc(Allocator *self, void *p, size_t size) {
	DebugAllocatorCtx *ctx = self->ctx;
	Allocator *a = ctx->a;

	remove_allocation_entry(self, p);
	p = a->realloc(a, p, size);
	add_allocation_entry(self, p, size);

	return p;
}

static void
debug_allocator_free(Allocator *self, void *p) {
	DebugAllocatorCtx *ctx = self->ctx;
	Allocator *a = ctx->a;

	remove_allocation_entry(self, p);
	a->free(a, p);
}

static void
debug_allocator_ctx_print_allocations(DebugAllocatorCtx *ctx) {
}

const Allocator DEBUG_ALLOCATOR_PROTOTYPE = {
	.alloc = debug_allocator_alloc,
	.realloc = debug_allocator_realloc,
	.free = debug_allocator_free
};

Allocator*
form_debug_allocator(Allocator *a) {
	// these allocations are not tracked for obvious reasons
	Allocator* debug_a = a->alloc(a, sizeof(Allocator));
	DebugAllocatorCtx *ctx = a->alloc(a, sizeof(DebugAllocatorCtx));

	*ctx = (DebugAllocatorCtx) {
		.a = a,
		.allocations = ARRAY.form_with_allocator(a, sizeof(DebugAllocationEntry)),
		.total_allocated = 0,
		.total_freed = 0,

		.print_allocations = debug_allocator_ctx_print_allocations
	};

	*debug_a = DEBUG_ALLOCATOR_PROTOTYPE;
	debug_a->ctx = ctx;

	return debug_a;
}
