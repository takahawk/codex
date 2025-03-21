#include "debug_allocator.h"

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#include "ds/array.h"

static void
add_allocation_entry(Allocator *self, void *ptr, size_t size) {
	DebugAllocatorCtx *ctx = self->ctx;

	void *stack[16];
	int stack_size = backtrace(stack, 16);
	DebugAllocationEntry entry = {
		.ptr = ptr,
		.size = size,
		.stack = stack,
		.stack_size = stack_size
	};

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
	Array/*DebugAllocationEntry*/ *al = ctx->allocations;
	for (size_t i = 0; i < al->len; ++i) {
		DebugAllocationEntry *entry = al->get(al, i);
		backtrace_symbols_fd(entry->stack, entry->stack_size, fileno(stderr));
		fprintf(stderr, "------------------------------------------------");
	}
}

const Allocator DEBUG_ALLOCATOR_PROTOTYPE = {
	.alloc = debug_allocator_alloc,
	.realloc = debug_allocator_realloc,
	.free = debug_allocator_free
};

Allocator*
form_debug_allocator(Allocator *a) {
	Allocator debug_a = DEBUG_ALLOCATOR_PROTOTYPE;

	// these allocations are not tracked for obvious reasons
	DebugAllocatorCtx *ctx = a->alloc(a, sizeof(DebugAllocatorCtx));

	Array *allocations = form_array(a, sizeof(DebugAllocationEntry));
	size_t total_allocated = 0;
	size_t total_freed = 0;

	*ctx = (DebugAllocatorCtx) {
		.allocations = allocations,
		.total_allocated = total_allocated,
		.total_freed = total_freed,


		.print_allocations = debug_allocator_ctx_print_allocations
	};

	return a;
}