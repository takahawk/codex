#include "codex/allocators/allocator.h"
#include "codex/ds/array.h"

typedef struct {
	void *ptr;
	size_t size;

	void *stack[16];
	int stack_size;

} DebugAllocationEntry;

typedef struct DebugAllocatorCtx {
	// original allocator
	Allocator *a;

	// TODO: optimize! trees, dude
	Array*/*DebugAllocationEntry*/ allocations;

	size_t total_allocated;
	size_t total_freed;

	void (*print_allocations) (struct DebugAllocatorCtx *ctx);
} DebugAllocatorCtx;

Allocator* form_debug_allocator(Allocator *a);