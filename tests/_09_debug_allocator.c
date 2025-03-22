#include "testing/assert.h"
#include "allocators/debug_allocator.h"
#include "ds/array.h"

int main() {
	Allocator *a = form_debug_allocator(&std_allocator);
	DebugAllocatorCtx *ctx = a->ctx;

	int *p1 = a->alloc(a, sizeof(*p1));
	int *p2 = a->alloc(a, sizeof(*p2));

	assert_uint16_equals(ctx->allocations->len, 2);

	Array *arr = form_array(a, sizeof(int));
	assert_uint16_equals(ctx->allocations->len, 4);

	arr->release(&arr);
	assert_uint16_equals(ctx->allocations->len, 2);

	a->free(a, p1);
	a->free(a, p2);
	assert_uint16_equals(ctx->allocations->len, 0);

	return 0;
}