#include "codex/testing/assert.h"
#include "codex/mem/debug_allocator.h"
#include "codex/ds/array.h"

int main() {
	Allocator *a = form_debug_allocator(&STD_ALLOCATOR);
	DebugAllocatorCtx *ctx = a->ctx;

	int *p1 = a->alloc(a, sizeof(*p1));
	int *p2 = a->alloc(a, sizeof(*p2));

	assert_uint16_equals(ctx->allocations->len, 2);

	Array *arr = ARRAY.form_with_allocator(a, sizeof(int));
	assert_uint16_equals(ctx->allocations->len, 4);

	arr->release(&arr);
	assert_uint16_equals(ctx->allocations->len, 2);

	a->free(a, p1);
	a->free(a, p2);
	assert_uint16_equals(ctx->allocations->len, 0);

	return 0;
}
